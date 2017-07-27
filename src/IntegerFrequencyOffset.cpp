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
		auto tmp = myBuffer_t(config.continual_pilots_count);
		std::transform(begin(config.continual_pilots),
				end(config.continual_pilots), begin(tmp), [&](auto t) {
					auto arg0 = in[base + t];
					auto arg1 = prev[base + t];
					return arg0 * std::conj(arg1);
				});
		auto acc = std::accumulate(begin(tmp), end(tmp),
				myComplex_t { 0, 0 });
//		for (auto t : config.continual_pilots) {
//			auto arg0 = in[base + t];
//			auto arg1 = prev[base + t];
//			acc += arg0 * std::conj(arg1);
//		}
		maxs[i + hi] = std::abs(acc);
	}

	std::copy(begin(in), end(in), begin(prev));

	auto max = std::max_element(begin(maxs), end(maxs));
	return std::distance(begin(maxs), max) - hi;

}

}
