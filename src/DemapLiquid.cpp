/*
 * DemapLiquid.cpp
 *
 *  Created on: Aug 18, 2017
 *      Author: tomas1
 */

#include <DemapLiquid.h>
#include <iterator>
#include <vector>
#include <algorithm>

namespace dvb {

DemapLiquid::DemapLiquid(const myConfig_t& c) :
		config { c } {
	myComplex_t constellation[64];
	constellation[1] = myComplex_t { -7, 7 };
	constellation[17] = myComplex_t { -5, 7 };
	constellation[21] = myComplex_t { -3, 7 };
	constellation[5] = myComplex_t { -1, 7 };
	constellation[4] = myComplex_t { 1, 7 };
	constellation[20] = myComplex_t { 3, 7 };
	constellation[16] = myComplex_t { 5, 7 };
	constellation[0] = myComplex_t { 7, 7 };
	constellation[33] = myComplex_t { -7, 5 };
	constellation[49] = myComplex_t { -5, 5 };
	constellation[53] = myComplex_t { -3, 5 };
	constellation[37] = myComplex_t { -1, 5 };
	constellation[36] = myComplex_t { 1, 5 };
	constellation[52] = myComplex_t { 3, 5 };
	constellation[48] = myComplex_t { 5, 5 };
	constellation[32] = myComplex_t { 7, 5 };
	constellation[41] = myComplex_t { -7, 3 };
	constellation[57] = myComplex_t { -5, 3 };
	constellation[61] = myComplex_t { -3, 3 };
	constellation[45] = myComplex_t { -1, 3 };
	constellation[44] = myComplex_t { 1, 3 };
	constellation[60] = myComplex_t { 3, 3 };
	constellation[56] = myComplex_t { 5, 3 };
	constellation[40] = myComplex_t { 7, 3 };
	constellation[9] = myComplex_t { -7, 1 };
	constellation[25] = myComplex_t { -5, 1 };
	constellation[29] = myComplex_t { -3, 1 };
	constellation[13] = myComplex_t { -1, 1 };
	constellation[12] = myComplex_t { 1, 1 };
	constellation[28] = myComplex_t { 3, 1 };
	constellation[24] = myComplex_t { 5, 1 };
	constellation[8] = myComplex_t { 7, 1 };
	constellation[11] = myComplex_t { -7, -1 };
	constellation[27] = myComplex_t { -5, -1 };
	constellation[31] = myComplex_t { -3, -1 };
	constellation[15] = myComplex_t { -1, -1 };
	constellation[14] = myComplex_t { 1, -1 };
	constellation[30] = myComplex_t { 3, -1 };
	constellation[26] = myComplex_t { 5, -1 };
	constellation[10] = myComplex_t { 7, -1 };
	constellation[43] = myComplex_t { -7, -3 };
	constellation[59] = myComplex_t { -5, -3 };
	constellation[63] = myComplex_t { -3, -3 };
	constellation[47] = myComplex_t { -1, -3 };
	constellation[46] = myComplex_t { 1, -3 };
	constellation[62] = myComplex_t { 3, -3 };
	constellation[58] = myComplex_t { 5, -3 };
	constellation[42] = myComplex_t { 7, -3 };
	constellation[35] = myComplex_t { -7, -5 };
	constellation[51] = myComplex_t { -5, -5 };
	constellation[55] = myComplex_t { -3, -5 };
	constellation[39] = myComplex_t { -1, -5 };
	constellation[38] = myComplex_t { 1, -5 };
	constellation[54] = myComplex_t { 3, -5 };
	constellation[50] = myComplex_t { 5, -5 };
	constellation[34] = myComplex_t { 7, -5 };
	constellation[3] = myComplex_t { -7, -7 };
	constellation[19] = myComplex_t { -5, -7 };
	constellation[23] = myComplex_t { -3, -7 };
	constellation[7] = myComplex_t { -1, -7 };
	constellation[6] = myComplex_t { 1, -7 };
	constellation[22] = myComplex_t { 3, -7 };
	constellation[18] = myComplex_t { 5, -7 };
	constellation[2] = myComplex_t { 7, -7 };

	demod = modem_create_arbitrary(constellation, 64);
}

DemapLiquid::~DemapLiquid() {
	modem_destroy(demod);
}

int DemapLiquid::demap(const myComplex_t& x) {
	unsigned int s;
	modem_demodulate(demod, x, &s);
	return s;
}

myBitset_t DemapLiquid::update(const myBuffer_t& complex) {
	auto tmp = myBufferI_t(complex.size());
	std::transform(begin(complex), end(complex), begin(tmp), [&](auto x) {
		return demap(x);
	});

	auto result = myBitset_t();
	auto it { 0 };
	for (auto d : tmp) {
		for (int i { 0 }; i < 6; i++) {
			result[it++] = (d >> (i)) & 1;
		}
	}
	return result;
}

} /* namespace dvb */
