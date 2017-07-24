/*
 * SyncTest.cpp
 *
 *  Created on: Jul 22, 2017
 *      Author: tomas1
 */

#include "Sync.h"
#include "SyncTest.h"

#include <complex>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "config.h"
#include "mytypes.h"

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
