/*
 * DataSelectorTest.cpp
 *
 *  Created on: Jul 24, 2017
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
#include <SamplingFrequencyOffset.h>
#include <Sync.h>
#include <test/DataSelectorTest.h>
#include <algorithm>
#include <deque>
#include <fstream>
#include <iterator>
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
	auto eqs = EqualizerSpilots { config };
	auto ds = DataSelector { config };
	auto sro = SamplingFrequencyOffset { config };
	auto inFile = std::ifstream(cfile);
	auto buf = myBuffer_t(config.sym_len);
	auto c { 0 };

	auto _ifo { 0.f };
	auto f { 0.f };
	auto _fto { 0.f };
	auto _sro { 0.f };
	auto _rfo { 0.f };
	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			buf.size() * sizeof(myComplex_t))) {

		auto _nco = nco.update(buf, _ifo, f, _rfo);
		auto [_sync, _f] = sync.update(_nco, _fto);
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
