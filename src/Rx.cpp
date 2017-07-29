/*
 * Rx.cpp
 *
 *  Created on: Jul 23, 2017
 *      Author: tomas1
 */

#include <DataSelector.h>
#include <Deinterleaver.h>
#include <Demap.h>
#include <Equalizer.h>
#include <EqualizerSpilots.h>
#include <Fft.h>
#include <FineTimingOffset.h>
#include <IntegerFrequencyOffset.h>
#include <Nco.h>
#include <Rx.h>
#include <Sync.h>
#include <algorithm>
#include <deque>
#include <fstream>
#include <iterator>
#include <vector>

namespace dvb {

Rx::Rx(const myConfig_t& c, const std::string& cf, const std::string& of) :
		config { c }, cfile { cf }, ofile { of }, inSync { 0 }, syncCounter { 0 } {
	rtObj.initialize();

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

#if 0
void Rx::rt_OneStep(std::ofstream& outFile, myBuffer_t& _out) {
	static boolean_T OverrunFlags[3] = {0, 0, 0};

	static boolean_T eventFlags[3] = {0, 0, 0}; // Model has 3 rates

	static int_T taskCounter[3] = {0, 0, 0};

	int_T i;

	// Disable interrupts here

	// Check base rate for overrun
	if (OverrunFlags[0]) {
		rtmSetErrorStatus(rtObj.getRTM(), "Overrun");
		return;
	}

	OverrunFlags[0] = true;

	// Save FPU context here (if necessary)
	// Re-enable timer or interrupt here

	//
	//  For a bare-board target (i.e., no operating system), the
	//  following code checks whether any subrate overruns,
	//  and also sets the rates that need to run this time step.

	for (i = 1; i < 3; i++) {
		if (taskCounter[i] == 0) {
			if (eventFlags[i]) {
				OverrunFlags[0] = false;
				OverrunFlags[i] = true;

				// Sampling too fast
				rtmSetErrorStatus(rtObj.getRTM(), "Overrun");
				return;
			}

			eventFlags[i] = true;
		}
	}

	taskCounter[1]++;
	if (taskCounter[1] == 3) {
		taskCounter[1] = 0;
	}

	taskCounter[2]++;
	if (taskCounter[2] == 48) {
		taskCounter[2] = 0;
	}

	// Set model inputs associated with base rate here

	// Step the model for base rate
	rtObj.step0();

	// Get model outputs here
//	getOutputs(outFile);

	// Indicate task for base rate complete
	OverrunFlags[0] = false;

	// Step the model for any subrate
	for (i = 1; i < 3; i++) {
		// If task "i" is running, don't run any lower priority task
		if (OverrunFlags[i]) {
			return;
		}

		if (eventFlags[i]) {
			OverrunFlags[i] = true;

			// Set model inputs associated with subrates here

			// Step the model for subrate "i"
			switch (i) {
				case 1:
				rtObj.step1();

				// Get model outputs here
				getOutputs(outFile);
				break;

				case 2:
				for (auto it {0}; it < 6048; it++) {
					rtObj.rtU.decoderIn[it].re = real(_out[it]);
					rtObj.rtU.decoderIn[it].im = imag(_out[it]);
				}
				rtObj.step2();

				// Get model outputs here
//				getOutputs(outFile);
				break;

				default:
				break;
			}

			// Indicate task complete for sample time "i"
			OverrunFlags[i] = false;
			eventFlags[i] = false;
		}
	}

	// Disable interrupts here
	// Restore FPU context here (if necessary)
	// Enable interrupts here
}
#endif

void Rx::getOutputs(std::ofstream& outFile1) {

//	auto buf = std::vector<unsigned char>(sizeof(rtObj.rtY.Out1));
//	std::copy(rtObj.rtY.Out1, rtObj.rtY.Out1 + sizeof, begin(buf));

//	std::cout << std::endl;
	if (!inSync) {
		for (auto it { 0 }; it < sizeof(rtObj.rtY.Out1); it++) {
			auto a = rtObj.rtY.Out1[it];
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

	if (inSync) {
		outFile1.write(
				reinterpret_cast<const char*>(&rtObj.rtY.Out1[syncCounter]),
				sizeof(rtObj.rtY.Out1) - syncCounter);
//		outFile1.write(
//				reinterpret_cast<const char*>(rtObj.rtY.decoderOut)
//				+ syncCounter, 189 - syncCounter);
		syncCounter = 0;
	}
}

void Rx::rx() {
	auto sync = Sync { config };
	auto nco = Nco { config };
	auto fft = Fft { config };
	auto ifo = IntegerFrequencyOffset { config };
	auto fto = FineTimingOffset { config };
	auto eq = Equalizer { config };
	auto eqs = EqualizerSpilots { config };
	auto ds = DataSelector { config };
	auto dem = Demap { config };
	auto deint = Deinterleaver { config };
	auto inFile = std::ifstream(cfile);
	auto buf = myBuffer_t(config.sym_len);
	auto c { 0 };

	auto _ifo { 0.f };
	auto f { 0.f };
	auto _fto { 0.f };
	auto outFile =
			std::ofstream { ofile + std::to_string(c++), std::ios::binary };

	auto outFile1 = std::ofstream { ofile + "viterbi", std::ios::binary };

	auto frameZeroCount { 0 };

	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			buf.size() * sizeof(myComplex_t))) {

		auto _nco = nco.update(buf, _ifo, f);
		auto [_sync, _f] = sync.update(_nco, _fto);
		f = _f;
		auto _fft = fft.update(_sync);
		_ifo = ifo.update(_fft);
		auto [_eq, _cpilots] = eq.update(_fft);
		_fto = fto.update(_cpilots);
		auto _frame = ds.frameNum(_eq);
		auto _eqs = eqs.update(_eq, _frame);
		auto _ds = ds.update(_eqs, _frame);
		auto _mul = myBuffer_t(config.data_carrier_count);
		std::transform(begin(_ds), end(_ds), begin(_mul), [](auto a) {
			return a * 0.7955f;
		});
		auto _dem = dem.update(_mul);
		auto _deint = deint.update(_dem, _frame);
		auto _out = _ds;

		if (_frame == 0) {
			frameZeroCount++;
		}

		if (frameZeroCount > 30) {
			outFile.write(reinterpret_cast<char*>(_out.data()),
					_out.size() * sizeof(myComplex_t));
//			outFile1 << _deint;

			for (auto it { 0 }; it < sizeof(rtObj.rtU.In1); it++) {
				rtObj.rtU.In1[it] = _deint[it];
			}
			rtObj.step();
			getOutputs(outFile1);
//			for (auto l { 0 }; l < 48; l++) {
//				rt_OneStep(outFile1, _out);
//			}
		}
	}
	outFile.close();
	outFile1.close();
}

}
