/*
 * DeinterleaverTest.cpp
 *
 *  Created on: Jul 29, 2017
 *      Author: tomas1
 */

#include <Deinterleaver.h>
#include <test/DeinterleaverTest.h>
#include <bitset>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace dvb {

DeinterleaverTest::DeinterleaverTest(const myConfig_t& c, const std::string& i,
		const std::string& o) :
		config { c }, in { i }, out { o } {

}

DeinterleaverTest::~DeinterleaverTest() {
}

void DeinterleaverTest::testSymbol() {
	auto deint = Deinterleaver { config };
	auto inFile = std::ifstream(in, std::ios::binary);
	auto outFile = std::ifstream(out, std::ios::binary);
	auto buf = std::vector<unsigned char>(config.data_carrier_count * 6);
	auto buf1 = std::vector<unsigned char>(config.data_carrier_count * 6);
	auto outer { 0 };
	auto dif { 0 };
	auto total { 0 };
	auto frame { 0 };
	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			sizeof(unsigned char) * buf.size())) {
		if (inFile.gcount() != sizeof(unsigned char) * buf.size()) {
			continue;
		}
		auto tmp = myBitset_t();
		auto it { 0 };
		for (auto b : buf) {
			tmp[it++] = b;
		}

		auto _deint = deint.symbol(tmp, frame++);
		if (outFile.read(reinterpret_cast<char*>(buf1.data()), buf1.size())) {

			for (auto it { 0 }; it < config.data_carrier_count * 6; it++) {
				//			printf("%d:%x:", outer, buf1[it]);
				//			std::cout << _dem[it] << ":" << it << std::endl;
				dif += (buf1[it] != _deint[it]);
				//				assert(dif <= 1);
				total++;
			}
		}
		outer++;
	}
	assert(total > 0);
//		std::cout << dif << " " << total << " " << (100 * dif / (float) total)
//				<< std::endl;
	assert(dif == 0);
	inFile.close();
	outFile.close();
}

void DeinterleaverTest::testBit() {
	auto deint = Deinterleaver { config };
		auto inFile = std::ifstream(in, std::ios::binary);
		auto outFile = std::ifstream(out, std::ios::binary);
		auto buf = std::vector<unsigned char>(config.data_carrier_count * 6);
		auto buf1 = std::vector<unsigned char>(config.data_carrier_count * 6);
		auto outer { 0 };
		auto dif { 0 };
		auto total { 0 };
		auto frame { 0 };
		while (inFile.read(reinterpret_cast<char*>(buf.data()),
				sizeof(unsigned char) * buf.size())) {
			if (inFile.gcount() != sizeof(unsigned char) * buf.size()) {
				continue;
			}
			auto tmp = myBitset_t();
			auto it { 0 };
			for (auto b : buf) {
				tmp[it++] = b;
			}

			auto _deint = deint.bit(tmp);
			if (outFile.read(reinterpret_cast<char*>(buf1.data()), buf1.size())) {

				for (auto it { 0 }; it < config.data_carrier_count * 6; it++) {
					//			printf("%d:%x:", outer, buf1[it]);
					//			std::cout << _dem[it] << ":" << it << std::endl;
					dif += (buf1[it] != _deint[it]);
					//				assert(dif <= 1);
					total++;
				}
			}
			outer++;
		}
		assert(total > 0);
//			std::cout << dif << " " << total << " " << (100 * dif / (float) total)
//					<< std::endl;
		assert(dif == 0);
		inFile.close();
		outFile.close();
}

} /* namespace dvb */
