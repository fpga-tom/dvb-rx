/*
 * DataSelectorTest.cpp
 *
 *  Created on: Jul 24, 2017
 *      Author: tomas1
 */

#include <DataSelector.h>
#include <Equalizer.h>
#include <Fft.h>
#include <FineTimingOffset.h>
#include <IntegerFrequencyOffset.h>
#include <mytypes.h>
#include <Nco.h>
#include <Sync.h>
#include <test/DataSelectorTest.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace dvb {

DataSelectorTest::DataSelectorTest(const myConfig_t& c, const std::string& cf,
		const std::string& of) :
		config { c }, cfile { cf }, ofile { of } {

}

DataSelectorTest::~DataSelectorTest() {
}

void DataSelectorTest::testDataSelector() {
	auto sync = Sync { config };
	auto nco = Nco { config };
	auto fft = Fft { config };
	auto ifo = IntegerFrequencyOffset { config };
	auto fto = FineTimingOffset { config };
	auto eq = Equalizer { config };
	auto ds = DataSelector { config };
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
		auto _cpilots = eq.selCpilots(_fft);
		auto _eq = eq.update(_fft);
		_fto = fto.update(_cpilots);
		auto [_ds, _frame] = ds.update(_eq);
		auto _out = _ds;

		auto outFile = std::ofstream { ofile + std::to_string(c++),
				std::ios::binary };
		for (auto t : _out) {
			outFile << real(t) << "\t" << imag(t) << std::endl;
		}
		outFile.close();
	}
}



} /* namespace dvb */
