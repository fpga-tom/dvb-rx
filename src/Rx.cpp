/*
 * Rx.cpp
 *
 *  Created on: Jul 23, 2017
 *      Author: tomas1
 */

#include <FineTimingOffset.h>
#include <IntegerFrequencyOffset.h>
#include "Rx.h"

#include <fstream>
#include <vector>

#include "Equalizer.h"
#include "Fft.h"
#include "Nco.h"
#include "Sync.h"

namespace dvb {

Rx::Rx(const myConfig_t& c, const std::string& cf, const std::string& of) :
		config { c }, cfile { cf }, ofile { of } {
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
	auto inFile = std::ifstream(cfile);
	auto buf = myBuffer_t(config.sym_len);
	auto c { 0 };

	auto _ifo { 0.f };
	auto f { 0.f };
	auto _fto { 0.f };
	auto outFile =
			std::ofstream { ofile + std::to_string(c++), std::ios::binary };

	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			buf.size() * sizeof(myComplex_t))) {

		auto _nco = nco.update(buf, _ifo, f);
		auto [_sync, _f] = sync.update(_nco, _fto);
		f = _f;
		auto _fft = fft.update(_sync);
		_ifo = ifo.update(_fft);
		auto _eq = eq.update(_fft);
		_fto = fto.update(_eq);
		auto _out = _eq;

		outFile.write(reinterpret_cast<char*>(_out.data()),
				_out.size() * sizeof(myComplex_t));
	}
		outFile.close();
}

}
