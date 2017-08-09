/*
 * EqualizerSpilots.cpp
 *
 *  Created on: Jul 25, 2017
 *      Author: tomas1
 */

#include <EqualizerSpilots.h>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <vector>

namespace dvb {

EqualizerSpilots::EqualizerSpilots(const myConfig_t& c) :
		config { c }, spilotsBuf { std::vector<myBuffer_t>(4) }, scattered_indices(
				config.scattered_pilots_count * 4) {
	inBufInverse = reinterpret_cast<fftwf_complex*>(fftwf_malloc(
			sizeof(fftwf_complex) * config.scattered_pilots_count * 4));
	outBufInverse = reinterpret_cast<fftwf_complex*>(fftwf_malloc(
			sizeof(fftwf_complex) * config.scattered_pilots_count * 4));

	planInverse = fftwf_plan_dft_1d(config.scattered_pilots_count * 4,
			inBufInverse,
			outBufInverse, FFTW_BACKWARD, FFTW_ESTIMATE);

	inBufForward = reinterpret_cast<fftwf_complex*>(fftwf_malloc(
			sizeof(fftwf_complex) * config.carriers));
	outBufForward = reinterpret_cast<fftwf_complex*>(fftwf_malloc(
			sizeof(fftwf_complex) * config.carriers));

	planForward = fftwf_plan_dft_1d(config.carriers, inBufForward,
			outBufForward, FFTW_FORWARD, FFTW_ESTIMATE);

	auto tmp = myBufferI_t(config.carriers);
	std::fill(begin(tmp), end(tmp), 0);
	for (auto i { 0 }; i < 4; i++) {
		spilotsBuf[i] = myBuffer_t(config.scattered_pilots_count);
		for (auto c : config.scattered_pilots[i]) {
			tmp[c] = 1;
		}
	}
	auto i { 0 };
	auto it = begin(scattered_indices);
	for (auto c : tmp) {
		if (c != 0) {
			*it++ = i;
		}
		i++;
	}
}

EqualizerSpilots::~EqualizerSpilots() {
	fftwf_free(inBufInverse);
	fftwf_free(outBufInverse);
	fftwf_destroy_plan(planInverse);
	fftwf_free(inBufForward);
	fftwf_free(outBufForward);
	fftwf_destroy_plan(planForward);
}

myBuffer_t EqualizerSpilots::selSpilots(const myBuffer_t& in, int frame) {
	assert(in.size() == config.carriers);

	auto result = myBuffer_t(config.scattered_pilots_count);
	auto i = begin(config.scattered_pilots[frame]);
	std::generate(begin(result), end(result), [&]() {
		return in[*i++];
	});
	return result;
}


myBuffer_t EqualizerSpilots::update(const myBuffer_t& in, int frame) {
	assert(in.size() == config.fft_len);

	// copy usefull carriers
	auto tmp = myBuffer_t(config.carriers);
	std::copy(begin(in) + config.zeros_left, end(in) - config.zeros_right,
			begin(tmp));

	auto spilots = selSpilots(tmp, frame);

	assert(spilots.size() == config.scattered_pilots_count);
	// calculate cir
	auto c = begin(config.scattered_pilots_value[frame]);
	std::transform(begin(spilots), end(spilots), begin(spilotsBuf[frame]),
			[&](auto v) {
//				return v * std::conj(*c++);
				return v / (*c++);
			});
	auto carriers = myBuffer_t(config.carriers);
	std::fill(begin(carriers), end(carriers), myComplex_t { 0.f, 0.f });
	for (auto i { 0 }; i < 4; i++) {
		auto it = begin(config.scattered_pilots[i]);
		for (auto c : spilotsBuf[i]) {
			carriers[*it++] = c;
		}
	}
	auto it = begin(scattered_indices);
	std::generate(reinterpret_cast<myComplex_t*>(inBufInverse),
			reinterpret_cast<myComplex_t*>(inBufInverse)
					+ config.scattered_pilots_count * 4, [&]() {
				return carriers[*it++];
			});

	// interpolate cir
	fftwf_execute(planInverse);

	std::transform(reinterpret_cast<myComplex_t*>(outBufInverse),
			reinterpret_cast<myComplex_t*>(outBufInverse)
					+ config.scattered_pilots_count * 4,
			reinterpret_cast<myComplex_t*>(outBufInverse), [&](auto a) {
				return a / (float)(config.scattered_pilots_count * 4);
			});

	// zero input buffer
	std::fill(reinterpret_cast<myComplex_t*>(inBufForward),
			reinterpret_cast<myComplex_t*>(inBufForward) + config.carriers, 0);

	int cc = config.scattered_pilots_count * 4 / 2;
	// align data
	std::copy(reinterpret_cast<myComplex_t*>(outBufInverse),
			reinterpret_cast<myComplex_t*>(outBufInverse) + cc,
			reinterpret_cast<myComplex_t*>(inBufForward));


		std::copy(reinterpret_cast<myComplex_t*>(outBufInverse) + cc,
			reinterpret_cast<myComplex_t*>(outBufInverse)
					+ config.scattered_pilots_count * 4,
			reinterpret_cast<myComplex_t*>(inBufForward) + config.carriers
					- cc);



	// execute interpolation
	fftwf_execute(planForward);

	// apply cir
	auto result = myBuffer_t(config.carriers);

	assert(tmp.size() == result.size());

	std::transform(begin(tmp), end(tmp),
			reinterpret_cast<myComplex_t*>(outBufForward), begin(result),
			[&](auto a, auto b) {
				return a / b;
			});

	return result;
}

} /* namespace dvb */
