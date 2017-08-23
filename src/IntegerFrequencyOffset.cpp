/*
 * Ifo.cpp
 *
 *  Created on: Jul 21, 2017
 *      Author: tomas1
 */

#include <IntegerFrequencyOffset.h>
#include <parallel/algorithm>
#include <parallel/numeric>
#include <complex>
#include <iterator>
#include <vector>

namespace dvb {

IntegerFrequencyOffset::IntegerFrequencyOffset(const myConfig_t& c) :
		config { c }, prev(config.fft_len) {
}

IntegerFrequencyOffset::~IntegerFrequencyOffset() {
}

int IntegerFrequencyOffset::update(myBuffer_t& in) {

	const int low = -4;
	const int hi = 4;

	std::vector<myReal_t> maxs(hi - low + 1);

	for (auto i { low }; i <= hi; i++) {

		auto base { config.zeros_left + i };
		auto tmp1 = myBuffer_t(config.continual_pilots_count);
		auto tmp2 = myBuffer_t(config.continual_pilots_count);

		std::transform(begin(config.continual_pilots),
				end(config.continual_pilots), begin(tmp1), [&](auto t) {
					return in[base + t];
				});

		std::transform(begin(config.continual_pilots),
				end(config.continual_pilots), begin(tmp2), [&](auto t) {
					return prev[base + t];
		});

		auto acc = myComplex_t { 0.f, 0.f };
		volk_32fc_x2_conjugate_dot_prod_32fc(&acc, tmp1.data(), tmp2.data(), config.continual_pilots_count);

		maxs[i + hi] = std::abs(acc);
	}

	std::copy(begin(in), end(in), begin(prev));

	auto max = std::max_element(begin(maxs), end(maxs));
	return std::distance(begin(maxs), max) - hi;

}

}
