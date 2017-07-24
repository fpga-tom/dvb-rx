/*
 * CpilotsSelectorTest.cpp
 *
 *  Created on: Jul 22, 2017
 *      Author: tomas1
 */

#include "CpilotsSelectorTest.h"

#include <fstream>
#include <string>
#include <vector>

#include "Fft.h"
#include "Ifo.h"
#include "mytypes.h"
#include "Nco.h"
#include "Sync.h"

CpilotsSelectorTest::CpilotsSelectorTest(const myConfig_t& c,
		const std::string& cf, const std::string& of) :
		config { c },
		cfile { cf },
		ofile { of },
		selector { c } {

}

CpilotsSelectorTest::~CpilotsSelectorTest() {
}

void CpilotsSelectorTest::testSelector() {
	auto sync = Sync { config };
	auto nco = Nco { config };
	auto fft = Fft { config };
	auto ifo = Ifo { config };
	auto cpilots = CpilotsSelector { config };
	auto inFile = std::ifstream(cfile);
	auto buf = myBuffer_t(config.sym_len);
	auto c { 0 };

	auto _ifo { 0.f };
	auto f { 0.f };
	auto _fto { 0.f };
	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			buf.size() * sizeof(myComplex_t))) {

		auto _nco = nco.update(buf, _ifo, f);
		auto [_sync, _f] = sync.update(_nco, _fto);
		f = _f;
		auto _fft = fft.update(_sync);
		_ifo = ifo.update(_fft);
		auto _cpilots = cpilots.update(_fft);
		auto _out = _cpilots;

		auto outFile = std::ofstream { ofile + std::to_string(c++),
				std::ios::binary };
		for (auto t : _out) {
			outFile << real(t) << "\t" << imag(t) << std::endl;
		}
		outFile.close();
	}
}
