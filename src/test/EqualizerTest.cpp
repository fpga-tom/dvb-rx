/*
 * CirTest.cpp
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
#include <SamplingFrequencyOffset.h>
#include <Sync.h>
#include <test/EqualizerTest.h>
#include <fstream>
#include <string>
#include <vector>

namespace dvb {

EqualizerTest::EqualizerTest(const myConfig_t& c, const std::string& cf,
		const std::string& of) :
		config { c }, cfile { cf }, ofile { of } {
}

EqualizerTest::~EqualizerTest() {
}

void EqualizerTest::testEqualizer() {
	auto sync = Sync { config };
	auto nco = Nco { config };
	auto fft = Fft { config };
	auto ifo = IntegerFrequencyOffset { config };
	auto fto = FineTimingOffset { config };
	auto eq = Equalizer { config };
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
		f = _f;
		auto _fft = fft.update(_sync);
		_sro = sro.sro(_fft);
		_rfo = sro.rfo(_fft);
		_ifo = ifo.update(_fft);
		auto [_cir, _cpilots] = eq.update(_fft);
		_fto = fto.update(_cpilots);
		auto _out = _cir;

		auto outFile = std::ofstream { ofile + std::to_string(c++),
				std::ios::binary };
		for (auto t : _out) {
			outFile << real(t) << "\t" << imag(t) << std::endl;
		}
		outFile.close();
	}
}

void EqualizerTest::testSelector() {
	auto sync = Sync { config };
	auto nco = Nco { config };
	auto fft = Fft { config };
	auto ifo = IntegerFrequencyOffset { config };
	auto eq = Equalizer { config };
	auto inFile = std::ifstream(cfile);
	auto buf = myBuffer_t(config.sym_len);
	auto c { 0 };

	auto _ifo { 0.f };
	auto f { 0.f };
	auto _fto { 0.f };
	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			buf.size() * sizeof(myComplex_t))) {

		auto _nco = nco.update(buf, _ifo, f, 0);
		auto [_sync, _f] = sync.update(_nco, _fto);
		f = _f;
		auto _fft = fft.update(_sync);
		_ifo = ifo.update(_fft);
		auto _cpilots = eq.selCpilots(_fft);
		auto _out = _cpilots;

		auto outFile = std::ofstream { ofile + std::to_string(c++),
				std::ios::binary };
		for (auto t : _out) {
			outFile << real(t) << "\t" << imag(t) << std::endl;
		}
		outFile.close();
	}
}

}
