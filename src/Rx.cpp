/*
 * Rx.cpp
 *
 *  Created on: Jul 23, 2017
 *      Author: tomas1
 */


#include <DataSelector.h>
#include <Deinterleaver.h>
#include <Demap.h>
#include <Descrambler.h>
#include <Equalizer.h>
#include <EqualizerSpilots.h>
#include <Fft.h>
#include <FineTimingOffset.h>
#include <IntegerFrequencyOffset.h>
#include <Nco.h>
#include <Rx.h>
#include <syscall.h>
#include <SamplingFrequencyOffset.h>
#include <Sync.h>
#include <unistd.h>
#include <volk/volk.h>
#include <volk/volk_complex.h>
#include <ViterbiDecoderSSE.h>
#include <cassert>
#include <cmath>
#include <complex>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <thread>
#include <tuple>
#include <vector>

#include <boost/atomic/detail/atomic_template.hpp>
#include <boost/lockfree/policies.hpp>
#include <boost/lockfree/spsc_queue.hpp>

namespace dvb {

Rx::Rx(const myConfig_t& c, const std::string& cf, const std::string& of) :
		config { c }, cfile { cf }, ofile { of } {

	q0 = std::deque<bool>(204 * 8);
	q1 = std::deque<bool>(204 * 7);
	q2 = std::deque<bool>(204 * 6);
	q3 = std::deque<bool>(204 * 5);
	q4 = std::deque<bool>(204 * 4);
	q5 = std::deque<bool>(204 * 3);
	q6 = std::deque<bool>(204 * 2);
	q7 = std::deque<bool>(204 * 1);
}

Rx::~Rx() {
}

std::tuple<bool, myInteger_t> Rx::synchronize(const myBufferB_t& out) {

	auto inSync = false;
	auto syncCounter = 0;

	if (!inSync) {
		for (auto it { 0 }; it < out.size(); it++) {
			auto a = out[it];
			if (!inSync) {
				inSync = q0.front() && q1.front() && q2.front() && q3.front()
						&& q4.front() && q5.front() && q6.front() && q7.front();
				syncCounter = it;
			}

			q0.pop_front();
			q1.pop_front();
			q2.pop_front();
			q3.pop_front();
			q4.pop_front();
			q5.pop_front();
			q6.pop_front();
			q7.pop_front();

			q0.push_back((bool) (a == 0xb8));
			q1.push_back((bool) (a == 0x47));
			q2.push_back((bool) (a == 0x47));
			q3.push_back((bool) (a == 0x47));
			q4.push_back((bool) (a == 0x47));
			q5.push_back((bool) (a == 0x47));
			q6.push_back((bool) (a == 0x47));
			q7.push_back((bool) (a == 0x47));

		}
	}
	return {inSync, syncCounter};
}

void Rx::rx() {
	auto traceback = 16;
	auto sync = Sync { config };
	auto nco = Nco { config };
	auto sro = SamplingFrequencyOffset { config };
	auto fft = Fft { config };
	auto ifo = IntegerFrequencyOffset { config };
	auto fto = FineTimingOffset { config };
	auto eq = Equalizer { config };
	auto eqs = EqualizerSpilots { config };
	auto ds = DataSelector { config };
	auto dem = Demap { config };
	auto deint = Deinterleaver { config };
	auto viterbi = ViterbiDecoderSSE { config, traceback };
	auto descrambler = Descrambler { config };
	auto inFile = std::ifstream(cfile);

	auto c { 0 };

	auto _ifo { 0.f };
	auto f { 0.f };
	auto _fto { 0.f };
	auto outFile =
			std::ofstream { ofile + std::to_string(c++), std::ios::binary };

	auto outFile1 = std::ofstream { ofile + "viterbi", std::ios::binary };
	auto outFile2 = std::ofstream { ofile + "ts", std::ios::binary };

	auto _sro { 0.f };
	auto _rfo { 0.f };
	auto readBytes { 0 };
	auto coeff = lv_cmake(std::sqrt(42.0f), 0.f);

	boost::lockfree::spsc_queue<std::tuple<myBitset_t*, int, int, bool>*,
			boost::lockfree::capacity<128>> queueViterbi;

	boost::lockfree::spsc_queue<std::tuple<myBuffer_t*, myBuffer_t*, bool>*,
			boost::lockfree::capacity<128>> queueSync;

	boost::lockfree::spsc_queue<std::tuple<myBufferB_t*, myInteger_t, bool>*,
			boost::lockfree::capacity<128>> queueReedSolomon;

	auto done = boost::atomic<bool>(false);

	auto reedSolomonThread = std::thread([&]() {
						std::cout << "reedSolomon thread id: " << syscall(SYS_gettid) << std::endl;
						static auto sync2 {0};
						while(!done) {
							std::tuple<myBufferB_t*, myInteger_t, bool>* value;
							while(!queueReedSolomon.pop(value)) {};
							auto [sbuf1, syncCounter, locked] = *value;

							if(!locked) {
								sync2 = 0;
							}

							auto result = descrambler.update(*sbuf1);
							if (syncCounter == 0) {
								if (sync2++ > 2) {
									outFile2.write(
											reinterpret_cast<const char*>(result.data()),
											result.size());
								}
							}
							delete sbuf1;
							delete value;
						}
	});

	auto viterbiThread =
			std::thread(
					[&]() {

						std::cout << "viterbi thread id: " << syscall(SYS_gettid) << std::endl;

						auto inSync = false;
						auto syncCounter = 0;
						while(!done) {
							std::tuple<myBitset_t*, int, int, bool>* value;
							while(!queueViterbi.pop(value)) {};
							auto [_deint, _frame, frameZeroCount, locked] = *value;

							if (frameZeroCount > 30) {
								auto notReset = locked || _frame != 0;
								auto _viterbi = viterbi.update(*_deint, notReset);

								static auto sbufCounter {0};

								if(!notReset || !inSync) {
									auto [_inSync, _syncCounter] = synchronize( _viterbi);
									inSync = _inSync;
									syncCounter = _syncCounter;
									sbufCounter = 0;
								}
								if (inSync) {
									static auto sbuf = myBufferB_t(3024*2);
									assert(3024 - syncCounter > 0);
									auto count = 3024 - syncCounter;
									assert(count > 0);
									std::copy(begin(_viterbi) + syncCounter,
											begin(_viterbi) + count + syncCounter,
											begin(sbuf) + sbufCounter);
									sbufCounter += count;

									while (sbufCounter >= 1632) {
										auto sbuf1 = myBufferB_t(1632);
										std::copy(begin(sbuf), begin(sbuf) + 1632, begin(sbuf1));

										auto out = new myBufferB_t(sbuf1);
										auto val = new std::tuple<myBufferB_t*, myInteger_t, bool>(out, syncCounter, locked);

										while (!queueReedSolomon.push(val))
										;

										std::copy(begin(sbuf) + 1632, begin(sbuf) + sbufCounter, begin(sbuf));
										sbufCounter -= 1632;
									}
									std::copy(begin(_viterbi) + count + syncCounter, end(_viterbi), begin(sbuf) + sbufCounter);
									syncCounter = 0;
								}
							}
							delete _deint;
							delete value;
						}
					});

	auto syncThread =
			std::thread(
					[&]() {
						std::cout << "sync thread id: " << syscall(SYS_gettid) << std::endl;
						auto buf = myBuffer_t(config.sym_len);
						while (inFile.read(reinterpret_cast<char*>(buf.data()),
										buf.size() * sizeof(myComplex_t))) {

							auto _nco = nco.update(buf, _ifo, f, _rfo);
							auto [_sync, _f, _locked] = sync.update(_nco, _fto);
							auto __sro = sro.update(_sync, sync.getSro());
							f = _f;
							auto _fft = fft.update(__sro);
							_sro = sro.sro(_fft);
							_rfo = sro.rfo(_fft);
							_ifo = ifo.update(_fft);
							auto [_eq, _cpilots] = eq.update(_fft);
							_fto = fto.update(_cpilots);

							auto out = new myBuffer_t(_eq);
							auto outfft = new myBuffer_t(_fft);
							auto val = new std::tuple<myBuffer_t*, myBuffer_t*, bool>(out, outfft, _locked);

							while (!queueSync.push(val))
							;
						}
						done = true;
						exit(0);
					});

	auto frameZeroCount { 0 };

	auto demapThread =
			std::thread(
					[&]() {

						std::cout << "demap thread id: " << syscall(SYS_gettid) << std::endl;
						while(!done) {
							std::tuple<myBuffer_t*, myBuffer_t*, bool> *value;
							while(!queueSync.pop(value))
							;
							auto [_eq, _fft, _locked] = *value;
							auto _frame = ds.frameNum(*_eq);
							auto _eqs = eqs.update(*_fft, _frame);
							auto _ds = ds.update(_eqs, _frame);
							auto _mul = myBuffer_t(config.data_carrier_count);

							volk_32fc_s32fc_multiply_32fc(_mul.data(), _ds.data(), coeff,
									config.data_carrier_count);

							auto _dem = dem.update(_mul);
							auto _deint = deint.update(_dem, _frame);
							auto _out = _ds;

							if (_frame == 0) {
								frameZeroCount++;
							}

							myBitset_t* __deint = new myBitset_t(_deint);
							auto val = new std::tuple<myBitset_t*, int, int, bool>(__deint, _frame,
									frameZeroCount, _locked);
							while (!queueViterbi.push(val))
							;
							delete _eq;
							delete _fft;
							delete value;
						}
					});

	syncThread.join();
	viterbiThread.join();
	demapThread.join();
	reedSolomonThread.join();

	outFile.close();
	outFile1.close();
	outFile2.close();
}

}
