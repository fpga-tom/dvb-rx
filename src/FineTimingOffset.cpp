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
		config { c } {
}

FineTimingOffset::~FineTimingOffset() {
//	fftwf_free(inBuf);
//	fftwf_free(outBuf);
//	fftwf_destroy_plan(plan);
}

// wrap to [-pi,pi]
double angle_norm(double x) {
	x = std::remainder(x + .5 * M_PI, 1.0 * M_PI);
	if (x < 0)
		x += M_PI;
	return x - .5 * M_PI;
}

double phase_unwrap(double prev, double now) {
	return prev + angle_norm(now - prev);
}

myReal_t FineTimingOffset::update(const myBuffer_t& in) {
	assert(in.size() == config.continual_pilots_count);
	auto z = myBuffer_t(config.continual_pilots_count - 1);
	auto x = myBuffer_t(config.continual_pilots_count - 1);
	auto y = myBuffer_t(config.continual_pilots_count - 1);


	std::transform(begin(in), end(in) - 1, begin(in) + 1, begin(z),
			[](auto a, auto b) {
				return std::conj(a) * b;
			});

	std::transform(begin(config.continual_pilots_value),
			end(config.continual_pilots_value) - 1,
			begin(config.continual_pilots_value) + 1, begin(x),
			[](auto a, auto b) {
				return std::conj(a) * b;
			});

	std::transform(begin(x), end(x), begin(z), begin(y), [](auto a, auto b) {
		return std::conj(a) * b;
	});

	auto acc = std::accumulate(begin(y), end(y), myComplex_t { 0, 0 });
	auto result = (config.fft_len) * std::arg(acc) / 2.0 / M_PI;
	return result;
}

}
