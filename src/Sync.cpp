/*
 * Sync.cpp
 *
 *  Created on: Jul 19, 2017
 *      Author: tomas1
 */

#include <stddef.h>
#include <SamplingFrequencyOffset.h>
#include <Sync.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <complex>
#include <deque>
#include <iostream>
#include <iterator>
#include <tuple>
#include <vector>

namespace dvb {

Sync::Sync(const myConfig_t &c) :
		config { c }, delay(config.fft_len + config.sym_len), accDelay(
				config.cp_len), acc { 0 }, current_size { 0 }, peakDelay(
				lockCount), accPeak { 0 }, integral { 0 }, peak { 0 }, currentLock {
				0 } {
	current = std::make_shared<myBuffer_t>(config.sym_len);
	next = std::make_shared<myBuffer_t>(config.sym_len);


}

Sync::~Sync() {
}

/**
 * Computes correlation of input buffer with delayed buffer
 */
myBuffer_t Sync::correlate(const myBuffer_t& in, myBuffer_t& delay,
		myDelay_t& accDelay, myComplex_t& acc) {
	myBuffer_t b(config.sym_len);

	assert(in.size() == config.sym_len);
	assert(in.size() == b.size());

	std::transform(begin(in), end(in), begin(delay) + config.fft_len,
			[](auto s) {
				return conj(s);
			});

	std::transform(begin(in), end(in), begin(delay), begin(b),
			[&](auto sample, auto delayedSample) {
				auto prod = sample * delayedSample;
				acc = acc + prod;
				acc = acc - accDelay.front();
				accDelay.pop_front();
				accDelay.push_back(prod);

				return acc;
			});

	std::copy(begin(delay) + config.sym_len, end(delay), begin(delay));

	return b;
}

/**
 * Looks for peak in correlated data
 */
size_t Sync::findPeak(const myBuffer_t& b) {
	auto max = std::max_element(begin(b), end(b),
			[](auto a, auto b) {return std::abs(a) < std::abs(b);});

	auto peakIdx = std::distance(begin(b), max);

	auto last = peakDelay.front();
	accPeak = accPeak + peakIdx - last;
	peakDelay.pop_front();
	peakDelay.push_back(peakIdx);
	return accPeak / (myReal_t) lockCount;
}

/**
 * Aligns data to start of frame
 */
myBuffer_t Sync::align(const myBuffer_t& in, myInteger_t peak) {
	assert(peak >= 0);
	assert(peak < config.sym_len);
	auto n = config.sym_len - current_size;
	std::copy_n(begin(in), n, begin(*current) + current_size);
	current_size = in.size() - peak;
	std::copy_n(begin(in) + peak, current_size, begin(*next));
	assert(current->size() == config.sym_len);
	std::swap(next, current);
	return *next;

}

/**
 * computing method
 */
std::tuple<myBuffer_t, myReal_t> Sync::update(const myBuffer_t& in,
		const myReal_t fineTiming) {



	auto b = correlate(in, delay, accDelay, acc);
	if (currentLock < lockCount + 10) {
		peak = findPeak(b);
		integral = peak;
		currentLock++;
	} else {
		auto ft = -fineTiming;
		auto proportional = SYNC_P_GAIN * ft;
		integral += SYNC_I_GAIN * ft;
		peak = proportional + integral;
	}
//	while (peak >= config.sym_len) {
//		peak -= config.sym_len;
//	}
//	while (peak < 0) {
//		peak += config.sym_len;
//	}
	auto freq = std::arg(b[std::round(peak)]) / 2.0 / M_PI / config.fft_len
			* config.sample_rate;
	auto result = align(in, std::round(peak));
	return {result, freq};
}
}
