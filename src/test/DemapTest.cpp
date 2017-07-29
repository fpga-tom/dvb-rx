/*
 * DemapTest.cpp
 *
 *  Created on: Jul 28, 2017
 *      Author: tomas1
 */

#include <Demap.h>
#include <test/DemapTest.h>
#include <algorithm>
#include <bitset>
#include <cassert>
#include <complex>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace dvb {

DemapTest::DemapTest(const myConfig_t& c, const std::string& i,
		const std::string& o) :
		config { c }, in { i }, out { o } {
}

DemapTest::~DemapTest() {
	// TODO Auto-generated destructor stub
}

void DemapTest::testDemap() {
	auto dem = Demap { config };
	auto i { 0 };
	for (auto y { 7 }; y >= -7; y -= 2) {
		for (auto x { -7 }; x <= 7; x += 2) {
			auto out = dem.demap(myComplex_t { x, y }, 0);
			auto b = DEMAP_TEST_CONSTELLATION[i++];
			assert(b == out);
		}
	}
}

void DemapTest::testUpdate() {
	auto dem = Demap { config };
	auto inFile = std::ifstream(in, std::ios::binary);
	auto outFile = std::ifstream(out, std::ios::binary);
	auto buf = myBuffer_t(config.data_carrier_count);
	auto buf1 = std::vector<unsigned char>(config.data_carrier_count * 6);
	auto outer { 0 };
	auto dif { 0 };
	auto total { 0 };
	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			sizeof(myComplex_t) * buf.size())) {
		if(inFile.gcount() != sizeof(myComplex_t)*buf.size()) {
			continue;
		}
		auto tmp = myBuffer_t(config.data_carrier_count);
		std::transform(begin(buf), end(buf), begin(tmp), [](auto a) {
			return a * 0.7955f;
		});
		auto _dem = dem.update(tmp);
		if (outFile.read(reinterpret_cast<char*>(buf1.data()), buf1.size())) {

			for (auto it { 0 }; it < config.data_carrier_count * 6; it++) {
//			printf("%d:%x:", outer, buf1[it]);
//			std::cout << _dem[it] << ":" << it << std::endl;
				dif += (buf1[it] != _dem[it]);
//				assert(dif <= 1);
				total++;
			}
		}
		outer++;
	}
	assert(total > 0);
//	std::cout << dif << " " << total << " " << (100 * dif / (float) total)
//			<< std::endl;
	assert(dif == 0);
	inFile.close();
	outFile.close();

}

} /* namespace dvb */
