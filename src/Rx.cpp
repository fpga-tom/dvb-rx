/*
 * Rx.cpp
 *
 *  Created on: Jul 23, 2017
 *      Author: tomas1
 */

#include <DataSelector.h>
#include <ext/type_traits.h>
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
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <vector>

namespace dvb {

Rx::Rx(const myConfig_t& c, const std::string& cf, const std::string& of) :
		config { c }, cfile { cf }, ofile { of } {
	rtObj.initialize();
}

Rx::~Rx() {
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

	auto _b8 { 0 };
	auto _b8sync { 0 };
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
		auto _out = _ds;

		if (_frame == 0) {
			frameZeroCount++;
		}

		if (frameZeroCount > 30) {
			outFile.write(reinterpret_cast<char*>(_out.data()),
					_out.size() * sizeof(myComplex_t));

			std::copy(begin(_out), end(_out),
					reinterpret_cast<myComplex_t*>(rtObj.rtU.decoderIn));
			rtObj.step();
			auto buf = std::vector<unsigned char>(189);
			std::copy(rtObj.rtY.decoderOut, rtObj.rtY.decoderOut + 189,
					begin(buf));

			outFile1.write(reinterpret_cast<const char*>(buf.data()),
					buf.size());
		}
	}
	outFile.close();
	outFile1.close();
}

}
