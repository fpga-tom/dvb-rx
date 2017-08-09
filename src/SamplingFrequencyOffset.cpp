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
		config { c }, prev(c.continual_pilots_count), prevrfo(
				c.continual_pilots_count), pilotsSquareSum { 0.f }, delayA(
				SRO_N), delayB(SRO_N), integral { 0.f }, integral_rfo { 0.f } {
	auto tmp = myBufferR_t(config.continual_pilots_count);
	std::transform(begin(config.continual_pilots), end(config.continual_pilots),
			begin(tmp), [&](auto a) {
				auto t = a;
				return t*t;
			});
	pilotsSquareSum = std::accumulate(begin(tmp),
			end(tmp), myReal_t { 0.f })
			* (2 * M_PI * (config.sym_len) / config.fft_len);
	pilotsSquareSum = 1.0f / pilotsSquareSum;

	std::fill(begin(delayA), end(delayA), myComplex_t { 0.f, 0.f });
	std::fill(begin(delayB), end(delayB), myComplex_t { 0.f, 0.f });
	std::fill(begin(prev), end(prev), 0);
}

SamplingFrequencyOffset::~SamplingFrequencyOffset() {
}

myReal_t SamplingFrequencyOffset::sro(const myBuffer_t& b) {
	assert(b.size() == config.fft_len);
	auto complex = cpilots(b);
	assert(complex.size() == config.continual_pilots_count);
	auto tmp = myBufferR_t(config.continual_pilots_count);
	auto it = begin(config.continual_pilots);
	std::transform(begin(complex), end(complex),
			begin(prev),
			begin(tmp),
			[&](auto a, auto b) {
//				return a * std::conj(b);
				auto angle = std::arg(a*std::conj(b));
				auto idx = *it++;
				auto result = angle*( idx );
				return result;
			});

	auto sum1 = std::accumulate(begin(tmp),
			end(tmp),
			myReal_t { 0.f });
	auto result = sum1 * pilotsSquareSum;
//	auto sum2 = std::accumulate(begin(tmp) + config.carrier_center + 1,
//			end(tmp), myComplex_t { 0.f, 0.f });
//	auto result = (std::arg(sum2) - std::arg(sum1))
//			/ ((2.0f * M_PI * config.sym_len / config.fft_len)
//					* (config.continual_pilots_count / 2.0f));
	std::copy(begin(complex), end(complex), begin(prev));
	auto proportional = result * SRO_P_GAIN;
	integral += result * SRO_I_GAIN;
//	return proportional + integral;
	return result;
}

myReal_t SamplingFrequencyOffset::rfo(const myBuffer_t& b) {
	assert(b.size() == config.fft_len);
	auto complex = cpilots(b);
	assert(complex.size() == config.continual_pilots_count);
	auto tmp = myBufferR_t(config.continual_pilots_count);
	std::transform(begin(complex), end(complex), begin(prevrfo), begin(tmp),
			[&](auto a, auto b) {
				return std::arg(a*std::conj(b));
			});
	auto result = std::accumulate(begin(tmp), end(tmp), myReal_t { 0.f })
			/ (2 * M_PI * (config.sym_len) / config.fft_len)
			/ config.continual_pilots_count;

	std::copy(begin(complex), end(complex), begin(prevrfo));
	auto proportional = result * SRO_P_GAIN_RFO;
	integral_rfo += result * SRO_I_GAIN_RFO;
//	return proportional + integral_rfo;
	return result;
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
//	std::cout << ":" << d << std::endl;
	assert(std::abs(d) <= 1.0f);
	auto count = SRO_N + 1;
	auto range = myBufferI_t(count);
	auto result = myBufferR_t(count);
	std::iota(begin(range), end(range), 0);
	std::transform(begin(range) + 1, end(range), begin(result) + 1,
			[&](auto k) {
				auto D = SRO_N + d;
				auto nom = myReal_t {1.0f};
				auto den = myReal_t {1.0f};
				for(auto q {0}; q < k; q++) {
					nom *= (D - (SRO_N - q));
					den *= (D + (q+1));
				}
				auto prod = nom / den;
				return std::pow(-1, k) * binom(SRO_N, k) * prod;
	});
	result[0] = 1.0f;
	return result;
}

myBuffer_t SamplingFrequencyOffset::filter(const myBuffer_t& complex,
		myReal_t sro) {
	assert(complex.size() == config.sym_len);
	auto tmp = myBuffer_t(delayB.size());
	auto tmp1 = myBuffer_t(delayA.size());
	auto result = myBuffer_t(complex.size());
	auto it = begin(result);

	myComplex_t mid { 0.f, 0.f };
	myBufferR_t cof = coeff(sro);

	auto count { 0 };

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

		assert(!isnanf(cof[SRO_N]));
		auto sum1 = std::accumulate(begin(tmp), end(tmp), myComplex_t { 0, 0 });
		auto sum2 = std::accumulate(begin(tmp1), end(tmp1),
				myComplex_t { 0, 0 });

		mid = sum1 + sum2 + sample * cof[SRO_N];

		delayA.pop_back();
		delayA.push_front(mid);
		if (count++ > SRO_N) {
			*it++ = mid;
		}
	}
	return result;
}

myBuffer_t SamplingFrequencyOffset::update(const myBuffer_t& complex,
		const myReal_t _sro) {
	assert(complex.size() == config.sym_len);
//	auto co = coeff(_sro);
	auto result = filter(complex, _sro);
	return result;
}


} /* namespace dvb */

