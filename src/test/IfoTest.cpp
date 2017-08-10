/*
 * IfoTest.cpp
 *
 *  Created on: Jul 22, 2017
 *      Author: tomas1
 */

#include <Fft.h>
#include <mytypes.h>
#include <Sync.h>
#include <test/IntegerFrequencyOffsetTest.h>
#include <fstream>
#include <string>
#include <vector>

namespace dvb {

IntegerFrequencyOffsetTest::IntegerFrequencyOffsetTest(const myConfig_t& c, const std::string& cf,
		const std::string& of) :
		IntegerFrequencyOffset { c }, cfile { cf }, ofile { of } {

}

IntegerFrequencyOffsetTest::~IntegerFrequencyOffsetTest() {
}

void IntegerFrequencyOffsetTest::testIfo() {
	auto sync = Sync { config };
	auto fft = Fft { config };
	auto inFile = std::ifstream(cfile);
	auto buf = myBuffer_t(config.sym_len);
	auto c { 0 };
	auto outFile =
			std::ofstream { ofile + std::to_string(c++), std::ios::binary };
	auto i { 0 };
	auto _fto { 0.f };
	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			buf.size() * sizeof(myComplex_t))) {

		auto [_sync, f, _locked] = sync.update(buf, _fto);
		auto _fft = fft.update(_sync);
		auto _out = update(_fft);

		outFile << i++ << "\t" << _out << std::endl;
	}
	outFile.close();
}

}
