/*
 * Demap.cpp
 *
 *  Created on: Jul 27, 2017
 *      Author: tomas1
 */

#include "Demap.h"

namespace dvb {

Demap::Demap(const myConfig_t& c) :
		config { c } {
}

Demap::~Demap() {
}

int Demap::demap(const myComplex_t& complex, int depth) {
	myReal_t re = real(complex);
	myReal_t im = imag(complex);

	bool x = std::signbit(re);
	bool y = std::signbit(im);

	auto result = y << 1 | x;

	if (depth < DEMAP_DEPTH) {
		auto b = 1.f / (2 << depth);
		auto a = myComplex_t { std::abs(re) - DEMAP_X * b, std::abs(im)
				- DEMAP_Y * b };
		result = demap(a, depth + 1) << 2 | result;
	}

	return result;
}

myBitset_t Demap::update(const myBuffer_t& complex) {
	auto result = myBitset_t { };
	auto it { 0 };
	for (auto c : complex) {
		auto d = demap(c, 0);
		for (int i { 0 }; i < 6; i++) {
			result[it++] = d & 1;
			d = d >> 1;
		}
	}
	return result;
}

} /* namespace dvb */
