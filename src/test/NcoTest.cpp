/*
 * NcoTest.cpp
 *
 *  Created on: Jul 22, 2017
 *      Author: tomas1
 */

#include <Fft.h>
#include <IntegerFrequencyOffset.h>
#include <Nco.h>
#include <SamplingFrequencyOffset.h>
#include <Sync.h>
#include <test/NcoTest.h>
#include <fstream>
#include <vector>

namespace dvb {

NcoTest::NcoTest(const myConfig_t& c, const std::string& cf,
		const std::string& of) :
		config { c },
		cfile { cf }, ofile { of } {

}

NcoTest::~NcoTest() {
}

void NcoTest::testNco() {

	auto sync = Sync { config };
	auto nco = Nco { config };
	auto fft = Fft { config };
	auto ifo = IntegerFrequencyOffset { config };
	auto inFile = std::ifstream(cfile);
	auto buf = myBuffer_t(config.sym_len);
	auto c { 0 };
	auto outFile =
			std::ofstream { ofile + std::to_string(c++), std::ios::binary };
	auto i { 0 };
	auto _ifo { 0.f };
	auto f { 0.f };
	auto _fto { 0.f };
	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			buf.size() * sizeof(myComplex_t))) {
		auto _nco = nco.update(buf, _ifo, f, 0);
		auto [_sync, _f, _locked] = sync.update(_nco, _fto);
		f = _f;
		auto _fft = fft.update(_nco);
		auto _out = ifo.update(_fft);
		_ifo = _out;

		outFile << i++ << "\t" << _out << std::endl;
	}
	outFile.close();
}

void NcoTest::testNcoFractional() {
	auto nco = Nco { config };
	auto sync = Sync { config };
	auto fft = Fft { config };
	auto ifo = IntegerFrequencyOffset { config };
	auto sro = SamplingFrequencyOffset { config };
	auto inFile = std::ifstream(cfile);
	auto buf = myBuffer_t(config.sym_len);
	auto c { 0 };
	auto outFile =
			std::ofstream { ofile + std::to_string(c++), std::ios::binary };
	auto i { 0 };
	auto _ifo { 0.f };
	auto f { 0.f };
	auto _fto { 0.f };
	auto _sro { 0.f };
	auto _rfo { 0.f };
	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			buf.size() * sizeof(myComplex_t))) {

		auto _nco = nco.update(buf, _ifo, f, _rfo);
		auto __sro = sro.update(_nco, _sro);
		auto [_sync, _f, _locked] = sync.update(__sro, _fto);
		f = _f;
		auto _fft = fft.update(_nco);
		_sro = sro.sro(_fft);
		_rfo = sro.rfo(_fft);
		auto _out = ifo.update(_fft);
		_ifo = _out;

		outFile << i++ << "\t" << f << std::endl;
	}
	outFile.close();
}

}
