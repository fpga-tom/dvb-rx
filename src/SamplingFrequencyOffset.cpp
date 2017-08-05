/*
 * SamplingFrequencyOffset.cpp
 *
 *  Created on: Aug 4, 2017
 *      Author: tomas1
 */

#include <SamplingFrequencyOffset.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <complex>
#include <deque>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

namespace dvb {

SamplingFrequencyOffset::SamplingFrequencyOffset(const myConfig_t& c) :
		config { c }, prev(c.continual_pilots_count), pilotsSquareSum { 0.f }, delayA(
				SRO_N), delayB(SRO_N), integral { 0.f } {
	auto tmp = myBufferR_t(config.continual_pilots_count);
	std::transform(begin(config.continual_pilots), end(config.continual_pilots),
			begin(tmp), [&](auto a) {
				return a*a;
			});
	pilotsSquareSum = std::accumulate(begin(tmp), end(tmp), myReal_t { 0.f })
			* (2 * M_PI * (config.sym_len) / config.fft_len);
	pilotsSquareSum = 1.0f / pilotsSquareSum;

	std::fill(begin(delayA), end(delayA), 0);
	std::fill(begin(delayB), end(delayB), 0);
}

SamplingFrequencyOffset::~SamplingFrequencyOffset() {
}

myReal_t SamplingFrequencyOffset::sro(const myBuffer_t& b) {
	assert(b.size() == config.fft_len);
	auto complex = cpilots(b);
	assert(complex.size() == config.continual_pilots_count);
	auto tmp = myBufferR_t(config.continual_pilots_count);
	auto it = begin(config.continual_pilots);
	std::transform(begin(complex), end(complex), begin(prev), begin(tmp),
			[&](auto a, auto b) {
				return std::arg(a*std::conj(b)) * (*it++);
			});
	auto result = std::accumulate(begin(tmp), end(tmp), myReal_t { 0 })
			* pilotsSquareSum;

	std::copy(begin(complex), end(complex), begin(prev));
	auto proportional = result * SRO_P_GAIN;
	integral += result * SRO_I_GAIN;
	return integral + proportional;
}

myBuffer_t SamplingFrequencyOffset::cpilots(const myBuffer_t& complex) {
	assert(complex.size() == config.fft_len);
	auto result = myBuffer_t(config.continual_pilots_count);
	std::transform(begin(config.continual_pilots), end(config.continual_pilots),
			begin(result), [&](auto a) {
				return complex[a + config.zeros_left];
			});
	return result;
}

myReal_t SamplingFrequencyOffset::binom(myReal_t n, myReal_t k) {
	return 1 / ((n + 1) * std::beta(n - k + 1, k + 1));
}


myBufferR_t SamplingFrequencyOffset::coeff(const myReal_t d) {
//	std::cout << d << std::endl;
	assert(std::abs(d) < .5f);
	auto count = SRO_N + 1;
	auto range = myBufferI_t(count);
	auto result = myBufferR_t(count);
	std::iota(begin(range), end(range), 0);
	std::transform(begin(range) + 1, end(range), begin(result) + 1,
			[&](auto k) {
				auto prod = std::accumulate(begin(range)+1,end(range), 1.0f, [&](auto p, auto n) {
							auto result = p*(d + n)/(d + n + k);
							return result;
						});
				return std::pow(-1, k) * binom(SRO_N, k) * prod;
	});
	result[0] = 1.0f;
	return result;
}

myBuffer_t SamplingFrequencyOffset::filter(const myBuffer_t& complex,
		const myBufferR_t& cof) {
	assert(complex.size() == config.sym_len);
	auto tmp = myBuffer_t(cof.size() - 1);
	auto tmp1 = myBuffer_t(cof.size() - 1);
	auto result = myBuffer_t(complex.size());
	auto it = begin(result);

	myComplex_t mid { 0.f, 0.f };

	for (auto sample : complex) {

		std::transform(begin(delayB), end(delayB),
				rbegin(cof) + 1, begin(tmp),
			[&](auto a, auto b) {
				return a * b;
			});

		std::transform(begin(delayA), end(delayA),
				begin(cof) + 1,
				begin(tmp1),
				[&](auto a, auto b) {
					return a * -b;
				});

		delayB.pop_back();
		delayB.push_front(sample);

		mid = std::accumulate(begin(tmp), end(tmp),
				myComplex_t { 0, 0 })
				+ std::accumulate(begin(tmp1), end(tmp1), myComplex_t { 0, 0 })
				+ sample * cof[SRO_N];

		delayA.pop_back();
		delayA.push_front(mid);
		*it++ = mid;
	}
	return result;
}

myBuffer_t SamplingFrequencyOffset::update(const myBuffer_t& complex,
		const myReal_t _sro) {
	assert(complex.size() == config.sym_len);
	auto co = coeff(_sro);
	auto result = filter(complex, co);
	return result;
}


} /* namespace dvb */

