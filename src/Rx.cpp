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
#include <ext/type_traits.h>
#include <Equalizer.h>
#include <EqualizerSpilots.h>
#include <Fft.h>
#include <FineTimingOffset.h>
#include <IntegerFrequencyOffset.h>
#include <Nco.h>
#include <Rx.h>
#include <SamplingFrequencyOffset.h>
#include <Sync.h>
#include <ViterbiDecoderSSE.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iterator>
#include <vector>
#include <cassert>

namespace dvb {

Rx::Rx(const myConfig_t& c, const std::string& cf, const std::string& of) :
		config { c }, cfile { cf }, ofile { of }, inSync { 0 }, syncCounter { -1 } {

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

void Rx::getOutputs(std::ofstream& outFile1, myBufferB_t& out, int to_out) {

//	std::cout << std::endl;
	if (!inSync) {
		for (auto it { 0 }; it < out.size() - to_out; it++) {
			auto a = out[it];
//		printf("%X", a);
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

//	if (inSync) {
//		outFile1.write(
//				reinterpret_cast<const char*>(out.data() + syncCounter),
//				out.size() - syncCounter - to_out);
////		outFile1.write(
////				reinterpret_cast<const char*>(rtObj.rtY.decoderOut)
////				+ syncCounter, 189 - syncCounter);
////		syncCounter = 0;
//	}
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
	auto buf = myBuffer_t(config.sym_len);
	auto c { 0 };

	auto _ifo { 0.f };
	auto f { 0.f };
	auto _fto { 0.f };
	auto outFile =
			std::ofstream { ofile + std::to_string(c++), std::ios::binary };

	auto outFile1 = std::ofstream { ofile + "viterbi", std::ios::binary };
	auto outFile2 = std::ofstream { ofile + "ts", std::ios::binary };

	auto frameZeroCount { 0 };
	auto _sro { 0.f };
	auto _rfo { 0.f };
	auto readBytes { 0 };
	auto coeff = std::sqrt(42.0f);
	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			buf.size() * sizeof(myComplex_t))) {

		readBytes += buf.size() * sizeof(myComplex_t);
//		std::cout << readBytes << std::endl;
		auto _nco = nco.update(buf, _ifo, f, _rfo);
		auto [_sync, _f, _locked] = sync.update(_nco, _fto );
		if (!_locked) {
			inSync = false;
		}
		auto __sro = sro.update(_sync, sync.getSro());
		f = _f;
		auto _fft = fft.update(__sro);
		_sro = sro.sro(_fft);
		_rfo = sro.rfo(_fft);
		_ifo = ifo.update(_fft);
		auto [_eq, _cpilots] = eq.update(_fft);
		_fto = fto.update(_cpilots);
		auto _frame = ds.frameNum(_eq);
		auto _eqs = eqs.update(_fft, _frame);
		auto _ds = ds.update(_eqs, _frame);
		auto _mul = myBuffer_t(config.data_carrier_count);
		std::transform(begin(_ds), end(_ds), begin(_mul), [&](auto a) {
			return a * coeff;
		});
		auto _dem = dem.update(_mul);
		auto _deint = deint.update(_dem, _frame);
		auto _out = _ds;

		if (_frame == 0) {
			frameZeroCount++;
		}

		if (frameZeroCount > 30) {
			auto notReset = inSync || _frame != 0;
			auto _viterbi = viterbi.update(_deint, notReset);
//			outFile.write(reinterpret_cast<char*>(_out.data()),
//					_out.size() * sizeof(myComplex_t));

			auto viterbiEnd = (_frame == 0 ? 0 : 0);
			getOutputs(outFile1, _viterbi, viterbiEnd);
			static std::deque<char> queue;
			if (!notReset) {
				queue.clear();
			}
			if (inSync) {
				static auto sync2 { 0 };
				auto sbuf = myBufferB_t(3024 - syncCounter - viterbiEnd);
				assert(3024 - syncCounter - viterbiEnd > 0);
				std::copy(begin(_viterbi) + syncCounter,
						end(_viterbi) - viterbiEnd,
						begin(sbuf));
				for (auto ch : sbuf) {
					queue.push_back(ch);
				}
				if (queue.size() >= 1632) {
					auto sbuf1 = myBufferB_t(1632);
					for (auto qi { 0 }; qi < 1632; qi++) {
						sbuf1[qi] = queue.front();
						queue.pop_front();
					}
					auto result = descrambler.update(sbuf1);
					if (syncCounter == 0) {
						if (sync2++ > 2) {
							outFile2.write(
									reinterpret_cast<const char*>(result.data()),
									result.size());
						}
					}
				}
				syncCounter = 0;
			}
		}
	}
	outFile.close();
	outFile1.close();
}

}
