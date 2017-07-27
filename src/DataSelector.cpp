/*
 * DataSelector.cpp
 *
 *  Created on: Jul 24, 2017
 *      Author: tomas1
 */

#include <DataSelector.h>
#include <algorithm>
#include <cassert>
#include <complex>
#include <iterator>
#include <numeric>
#include <vector>

namespace dvb {

DataSelector::DataSelector(const myConfig_t& c) :
		config { c } {

	dataIdx = std::vector<std::vector<int>>(4);
	for (auto i { 0 }; i < 4; i++) {

		auto tmp = std::vector<int>(config.carriers);

		std::iota(begin(tmp), end(tmp), 0);

		auto without_tps = std::vector<int>();
		std::set_difference(begin(tmp), end(tmp), begin(config.tps),
				end(config.tps),
				inserter(without_tps, begin(without_tps)));

		auto without_cpilots = std::vector<int>();

		std::set_difference(begin(without_tps), end(without_tps),
				begin(config.continual_pilots), end(config.continual_pilots),
				inserter(without_cpilots, begin(without_cpilots)));

		auto without_spilots = std::vector<int>();

		std::set_difference(begin(without_cpilots),
				end(without_cpilots),
				begin(config.scattered_pilots[i]),
				end(config.scattered_pilots[i]),
				inserter(without_spilots, begin(without_spilots)));

		dataIdx[i] = without_spilots;
	}

}

DataSelector::~DataSelector() {
}

int DataSelector::frameNum(const myBuffer_t& in) {
	assert(in.size() == config.carriers);
	auto f = myBuffer_t(config.scattered_pilots_count);
	auto maxs = std::vector<myReal_t>(4);

	for (int i { 0 }; i < 4; i++) {
		std::transform(begin(config.scattered_pilots[i]),
				end(config.scattered_pilots[i]), begin(f), [&](auto a) {
					return in[a];
				});
		auto acc = myComplex_t { 0, 0 };
		auto it = begin(config.scattered_pilots_value[i]);
		for (auto a : f) {
			acc += a * std::conj(*it++);
		}
		maxs[i] = std::abs(acc);
	}

	auto max = std::max_element(begin(maxs), end(maxs));
	return std::distance(begin(maxs), max);

}

myBuffer_t DataSelector::update(const myBuffer_t& in,
		int frame) {
	assert(in.size() == config.carriers);
	auto result = myBuffer_t(config.data_carrier_count);

	auto it = begin(result);
	for (auto a : dataIdx[frame]) {
		*it++ = in[a];
	}

	return result;
}

} /* namespace dvb */
