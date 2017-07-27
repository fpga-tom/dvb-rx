/*
 * Demap.cpp
 *
 *  Created on: Jul 27, 2017
 *      Author: tomas1
 */

#include "Demap.h"

namespace dvb {

Demap::Demap(myConfig_t& c) :
		config { c } {
}

Demap::~Demap() {
}

int Demap::demap(const myComplex_t& complex, int depth) {
	myReal_t re = real(complex);
	myReal_t im = imag(complex);

	bool x = std::signbit(re);
	bool y = std::signbit(im);

	if (depth < DEMAP_DEPTH) {
		auto b = 1.f / (2 << depth);
		auto sx = x == true ? -1 : 1;
		auto sy = y == true ? -1 : 1;
		auto a = myComplex_t { re - sx * DEMAP_X * b, im - sy * DEMAP_Y * b };
		demap(a, depth + 1);
	}

}

std::vector<bool> Demap::update(const myBuffer_t& complex) {
}

} /* namespace dvb */
