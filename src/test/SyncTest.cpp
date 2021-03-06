/*
 * SyncTest.cpp
 *
 *  Created on: Jul 22, 2017
 *      Author: tomas1
 */

#include <config.h>
#include <Equalizer.h>
#include <Fft.h>
#include <FineTimingOffset.h>
#include <IntegerFrequencyOffset.h>
#include <mytypes.h>
#include <Nco.h>
#include <SamplingFrequencyOffset.h>
#include <Sync.h>
#include <test/SyncTest.h>
#include <complex>
#include <deque>
#include <fstream>
#include <string>
#include <vector>

namespace dvb {

SyncTest::SyncTest(const myConfig_t& c, const std::string& cf,
		const std::string& of) :
		Sync(c), cfile { cf }, ofile { of } {

}

SyncTest::~SyncTest() {
	// TODO Auto-generated destructor stub
}

void SyncTest::testCorrelation() {
	auto inFile = std::ifstream(cfile);
	auto buf = myBuffer_t(config.sym_len);
	auto c { 0 };

	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			buf.size() * sizeof(myComplex_t))) {

		auto _out = correlate(buf, delay, accDelay, acc);
		auto outFile = std::ofstream { ofile + std::to_string(c++),
				std::ios::binary };
		auto i { 0 };
		for (auto t : _out) {
			outFile << i++ << "\t" << abs(t) << std::endl;
		}
		outFile.close();
	}

}

void SyncTest::testAlign() {
	const myConfig_t config = DVBT_CONFIG_1;

	auto inFile = std::ifstream(cfile);
	auto buf = myBuffer_t(config.sym_len);
	auto delay1 = myBuffer_t(config.fft_len + config.sym_len);
	auto accDelay1 = myDelay_t(config.cp_len);
	auto acc1 = myComplex_t { 0 };
	auto c { 0 };

	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			buf.size() * sizeof(myComplex_t))) {

		auto _corr = correlate(buf, delay, accDelay, acc);
		auto _peak = findPeak(_corr);
		auto _align = align(buf, _peak);
		auto _out = correlate(_align, delay1, accDelay1, acc1);
		auto outFile = std::ofstream { ofile + std::to_string(c++),
				std::ios::binary };
		auto i { 0 };
		for (auto t : _out) {
			outFile << i++ << "\t" << abs(t) << std::endl;
		}
		outFile.close();
	}

}

void SyncTest::testPeak() {
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
	auto integral = myReal_t { 0 };

	auto ftoPrev = myReal_t { 0.f };
	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			buf.size() * sizeof(myComplex_t))) {

		auto _nco = nco.update(buf, _ifo, f, _rfo);
		auto [_sync, _f, _locked] = sync.update(_nco, _fto);
		auto __sro = sro.update(_sync, sync.getSro());
		f = _f;
		auto _fft = fft.update(__sro);
		_sro = sro.sro(_fft);
		_rfo = sro.rfo(_fft);
		_ifo = ifo.update(_fft);
		auto [_eq, _cpilots] = eq.update(_fft);
		_fto = fto.update(_cpilots);
		auto _out = sync.peak;

		outFile << i++ << "\t" << _out << std::endl;
	}
	outFile.close();

}


}
