/*
 * DemapTest.cpp
 *
 *  Created on: Jul 28, 2017
 *      Author: tomas1
 */

#include <Demap.h>
#include <test/DemapTest.h>
#include <cassert>
#include <iostream>

namespace dvb {

DemapTest::DemapTest(const myConfig_t& c) :
		config { c } {
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

} /* namespace dvb */
