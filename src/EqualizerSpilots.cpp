/*
 * EqualizerSpilots.cpp
 *
 *  Created on: Jul 25, 2017
 *      Author: tomas1
 */

#include <ext/type_traits.h>
#include <EqualizerSpilots.h>
#include <algorithm>
#include <cassert>
#include <complex>
#include <deque>
#include <iterator>
#include <vector>

namespace dvb {

EqualizerSpilots::EqualizerSpilots(const myConfig_t& c) :
		config { c } {
	inBufInverse = reinterpret_cast<fftwf_complex*>(fftwf_malloc(
			sizeof(fftwf_complex) * config.scattered_pilots_count));
	outBufInverse = reinterpret_cast<fftwf_complex*>(fftwf_malloc(
			sizeof(fftwf_complex) * config.scattered_pilots_count));

	planInverse = fftwf_plan_dft_1d(config.scattered_pilots_count, inBufInverse,
			outBufInverse, FFTW_BACKWARD, FFTW_ESTIMATE);

	inBufForward = reinterpret_cast<fftwf_complex*>(fftwf_malloc(
			sizeof(fftwf_complex) * config.carriers));
	outBufForward = reinterpret_cast<fftwf_complex*>(fftwf_malloc(
			sizeof(fftwf_complex) * config.carriers));

	planForward = fftwf_plan_dft_1d(config.carriers, inBufForward,
			outBufForward, FFTW_FORWARD, FFTW_ESTIMATE);
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
	auto spilots = selSpilots(in, frame);

	assert(spilots.size() == config.scattered_pilots_count);
	// calculate cir
	auto c = begin(config.scattered_pilots_value[frame]);
	std::transform(begin(spilots), end(spilots),
			reinterpret_cast<myComplex_t*>(inBufInverse), [&](auto v) {
				return v * std::conj(*c++);
			});

	// interpolate cir
	fftwf_execute(planInverse);

	// zero input buffer
	std::fill(reinterpret_cast<myComplex_t*>(inBufForward),
			reinterpret_cast<myComplex_t*>(inBufForward) + config.carriers, 0);

	int cc = config.scattered_pilots_count / 2;
	// align data
	std::copy(reinterpret_cast<myComplex_t*>(outBufInverse),
			reinterpret_cast<myComplex_t*>(outBufInverse)
					+ cc,
			reinterpret_cast<myComplex_t*>(inBufForward));

	std::copy(
			reinterpret_cast<myComplex_t*>(outBufInverse)
					+ cc + 1,
			reinterpret_cast<myComplex_t*>(outBufInverse)
					+ config.continual_pilots_count,
			reinterpret_cast<myComplex_t*>(inBufForward) + config.carriers
					- cc);

	// execute interpolation
	fftwf_execute(planForward);

	// copy usefull carriers
	auto tmp = myBuffer_t(config.carriers);
	std::copy(begin(in), end(in),
			begin(tmp));

	// apply cir
	auto result = myBuffer_t(config.carriers);

	assert(tmp.size() == result.size());

	std::transform(begin(tmp), end(tmp),
			reinterpret_cast<myComplex_t*>(outBufForward), begin(result),
			[&](auto a, auto b) {
				return a / b * (float)config.fft_len;
			});

	return result;
}

} /* namespace dvb */
