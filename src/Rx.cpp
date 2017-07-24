/*
 * Rx.cpp
 *
 *  Created on: Jul 23, 2017
 *      Author: tomas1
 */

#include "Rx.h"

#include <fstream>
#include <vector>

#include "CpilotsSelector.h"
#include "Equalizer.h"
#include "Fft.h"
#include "Fto.h"
#include "Ifo.h"
#include "Nco.h"
#include "Sync.h"

Rx::Rx(const myConfig_t& c, const std::string& cf, const std::string& of) :
		config { c }, cfile { cf }, ofile { of } {
}

Rx::~Rx() {
}

void Rx::rx() {
	auto sync = Sync { config };
	auto nco = Nco { config };
	auto fft = Fft { config };
	auto ifo = Ifo { config };
	auto fto = Fto { config };
	auto cpilots = CpilotsSelector { config };
	auto cir = Equalizer { config };
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
		auto _cpilots = cpilots.update(_fft);
		auto _cir = cir.update(_fft, _cpilots);
		_fto = fto.update(_cpilots);
		auto _out = _cir;

		outFile.write(reinterpret_cast<char*>(_out.data()),
				_out.size() * sizeof(myComplex_t));
	}
		outFile.close();
}
