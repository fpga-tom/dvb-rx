/*
 * FtoTest.cpp
 *
 *  Created on: Jul 23, 2017
 *      Author: tomas1
 */

#include <Equalizer.h>
#include <Fft.h>
#include <FineTimingOffset.h>
#include <IntegerFrequencyOffset.h>
#include <mytypes.h>
#include <Nco.h>
#include <Sync.h>
#include <test/FineTimingOffsetTest.h>
#include <fstream>
#include <vector>

namespace dvb {


FineTimingOffsetTest::FineTimingOffsetTest(const myConfig_t& c, const std::string& cf,
		const std::string& of) :
		config { c }, cfile { cf }, ofile { of } {
}

FineTimingOffsetTest::~FineTimingOffsetTest() {
}

void FineTimingOffsetTest::testFto() {
	auto sync = Sync { config };
	auto nco = Nco { config };
	auto fft = Fft { config };
	auto ifo = IntegerFrequencyOffset { config };
	auto eq = Equalizer { config };
	auto fto = FineTimingOffset { config };
	auto inFile = std::ifstream(cfile);
	auto buf = myBuffer_t(config.sym_len);
	auto c { 0 };

	auto i { 0 };
	auto _ifo { 0.f };
	auto _fto { 0.f };
	auto f { 0.f };
	auto outFile =
			std::ofstream { ofile + std::to_string(c++), std::ios::binary };
	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			buf.size() * sizeof(myComplex_t))) {

		auto _nco = nco.update(buf, _ifo, f);
		auto [_sync, _f] = sync.update(_nco, _fto);
		f = _f;
		auto _fft = fft.update(_sync);
		_ifo = ifo.update(_fft);
		auto _cpilots = eq.selCpilots(_fft);
		_fto = fto.update(_cpilots);
		auto _out = _fto;


			outFile << i++ << "\t" << _out << std::endl;
	}
		outFile.close();
}

}
