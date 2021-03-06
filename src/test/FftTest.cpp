/*
 * FftTest.cpp
 *
 *  Created on: Jul 22, 2017
 *      Author: tomas1
 */

#include <ext/type_traits.h>
#include <Fft.h>
#include <Sync.h>
#include <test/FftTest.h>
#include <algorithm>
#include <complex>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace dvb {

FftTest::FftTest(const myConfig_t& c, const std::string& cf,
		const std::string& of) :
		config { c }, cfile { cf }, ofile { of } {

}

FftTest::~FftTest() {
}

void FftTest::testFft() {
	auto sync = Sync { config };
	auto fft = Fft { config };
	auto inFile = std::ifstream(cfile);
	auto buf = myBuffer_t(config.sym_len);
	auto c { 0 };

	auto _fto { 0.f };
	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			buf.size() * sizeof(myComplex_t))) {



		auto [_sync, f, _locked] = sync.update(buf, _fto);
		auto _out = fft.update(_sync);
		auto outFile = std::ofstream { ofile + std::to_string(c++),
				std::ios::binary };
		auto i { 0 };
		for (auto t : _out) {
			outFile << i++ << "\t" << abs(t) << std::endl;
		}
		outFile.close();
	}
}

}
