/*
 * Fto.cpp
 *
 *  Created on: Jul 23, 2017
 *      Author: tomas1
 */

#include <FineTimingOffset.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <complex>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

namespace dvb {

FineTimingOffset::FineTimingOffset(const myConfig_t& c) :
		config { c }, x (config.continual_pilots_count - 1) {

			std::transform(begin(config.continual_pilots_value),
						end(config.continual_pilots_value) - 1,
						begin(config.continual_pilots_value) + 1, begin(x),
						[](auto a, auto b) {
							return std::conj(a) * b;
						});
}

FineTimingOffset::~FineTimingOffset() {
}


myReal_t FineTimingOffset::update(const myBuffer_t& in) {
	assert(in.size() == config.continual_pilots_count);
	auto z = myBuffer_t(config.continual_pilots_count - 1);
	auto y = myBuffer_t(config.continual_pilots_count - 1);


	std::copy(begin(in) + 1, end(in), begin(y));

	volk_32fc_x2_multiply_conjugate_32fc(z.data(), y.data(), in.data(), config.continual_pilots_count -1);

	auto acc =  myComplex_t { 0.f, 0.f };
	volk_32fc_x2_conjugate_dot_prod_32fc(&acc, z.data(), x.data(), config.continual_pilots_count - 1);

	auto result = (config.fft_len) * std::arg(acc) / 2.0 / M_PI;
	return result;
}

}
