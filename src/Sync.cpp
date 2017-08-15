/*
 * Sync.cpp
 *
 *  Created on: Jul 19, 2017
 *      Author: tomas1
 */

#include <Sync.h>
#include <volk/volk.h>
#include <volk/volk_malloc.h>
#include <algorithm>
#include <cassert>
#include <complex>
#include <iostream>
#include <iterator>
#include <vector>

namespace dvb {

Sync::Sync(const myConfig_t &c) :
		config { c }, delay(config.fft_len + config.sym_len), accDelay(
				config.cp_len), acc { 0 }, current_size { 0 }, peakDelay(
				lockCount), accPeak { 0 }, integral { 0 }, peak { 0 }, currentLock {
				0 } {
	current = std::make_shared<myBuffer_t>(config.sym_len);
	next = std::make_shared<myBuffer_t>(config.sym_len);

	unsigned int alignment = volk_get_alignment();
	in = (lv_32fc_t*) volk_malloc(
			sizeof(lv_32fc_t) * config.sym_len, alignment);
	out = (lv_32fc_t*) volk_malloc(
			sizeof(lv_32fc_t) * config.sym_len, alignment);
}

Sync::~Sync() {
	volk_free(in);
	volk_free(out);
}

/**
 * Computes correlation of input buffer with delayed buffer
 */
myBuffer_t Sync::correlate(const myBuffer_t& in, myBuffer_t& delay,
		myDelay_t& accDelay, myComplex_t& acc) {
	myBuffer_t b(config.sym_len);

	assert(in.size() == config.sym_len);
	assert(in.size() == b.size());

	std::copy(begin(in), end(in), begin(delay) + config.fft_len);

	volk_32fc_x2_multiply_conjugate_32fc(b.data(), in.data(), delay.data(), config.sym_len);

	std::copy(begin(delay) + config.sym_len, end(delay), begin(delay));

	std::transform(begin(b), end(b),  begin(b),
			[&](auto prod) {
				acc = acc + prod;
				acc = acc - accDelay.front();
				accDelay.pop_front();
				accDelay.push_back(prod);

				return acc;
			});


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
	auto result = accPeak / (myReal_t) lockCount;
	assert(result >= 0);
	assert(result < config.sym_len);
	return result;
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
std::tuple<myBuffer_t, myReal_t, bool> Sync::update(const myBuffer_t& in,
		const myReal_t fineTiming) {



	auto b = correlate(in, delay, accDelay, acc);
	auto peakFind = findPeak(b);
	if (currentLock < lockCount + 5) {
		peak = peakFind;
		integral = peak;
		currentLock++;
	} else {
		auto ft = -fineTiming;
		auto proportional = SYNC_P_GAIN * ft;
		integral += SYNC_I_GAIN * ft;
		peak = proportional + integral;
	}

	while (peak >= config.sym_len) {
		peak -= config.sym_len;
	}
	while (peak < 0) {
		peak += config.sym_len;
	}

	bool locked =
			!(currentLock >= lockCount + 5
			&& std::abs(peak - peakFind) > 20);
	if (!locked) {
		std::cerr << "Lost sync " << fineTiming << " " << peak << " "
				<< peakFind << std::endl;
		// restart sync
		currentLock = 0;
	}
	auto freq = std::arg(b[std::floor(peak)]) / 2.0 / M_PI / config.fft_len
			* config.sample_rate;
	auto result = align(in, std::floor(peak));
	return {result, freq, locked};
}
}
