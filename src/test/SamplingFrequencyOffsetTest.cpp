/*
 * SamplingFrequencyOffsetTest.cpp
 *
 *  Created on: Aug 5, 2017
 *      Author: tomas1
 */

#include <Equalizer.h>
#include <Fft.h>
#include <FineTimingOffset.h>
#include <IntegerFrequencyOffset.h>
#include <Nco.h>
#include <SamplingFrequencyOffset.h>
#include <Sync.h>
#include <test/SamplingFrequencyOffsetTest.h>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <vector>

namespace dvb {

SamplingFrequencyOffsetTest::SamplingFrequencyOffsetTest(const myConfig_t& c,
		const std::string& cf, const std::string& of) :
		config { c }, cfile { cf }, ofile { of } {
}

SamplingFrequencyOffsetTest::~SamplingFrequencyOffsetTest() {
}

void SamplingFrequencyOffsetTest::testSRO() {
	auto sync = Sync { config };
	auto nco = Nco { config };
	auto fft = Fft { config };
	auto ifo = IntegerFrequencyOffset { config };
	auto eq = Equalizer { config };
	auto fto = FineTimingOffset { config };
	auto sro = SamplingFrequencyOffset { config };
	auto inFile = std::ifstream(cfile);
	auto buf = myBuffer_t(config.sym_len);
	auto c { 0 };

	auto i { 0 };
	auto _ifo { 0.f };
	auto _fto { 0.f };
	auto f { 0.f };
	auto _sro { 0.f };
	auto _rfo { 0.f };
	auto outFile =
			std::ofstream { ofile + std::to_string(c++), std::ios::binary };
	auto count { 0 };
	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			buf.size() * sizeof(myComplex_t))) {

		auto _nco = nco.update(buf, _ifo, f, _rfo);


		auto [_sync, _f, _locked] = sync.update(_nco, _fto );
		auto __sro = sro.update(_sync, sync.getSro());
		f = _f;
		auto _fft = fft.update(__sro);
		_sro = sro.sro(_fft);
		_rfo = sro.rfo(_fft);
		_ifo = ifo.update(_fft);
		auto _cpilots = eq.selCpilots(_fft);
		_fto = fto.update(_cpilots);
		auto _out = _sro;

		outFile << i++ << "\t" << _out << std::endl;
	}
	outFile.close();
}

} /* namespace dvb */
