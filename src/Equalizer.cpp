/*
 * Cir.cpp
 *
 *  Created on: Jul 23, 2017
 *      Author: tomas1
 */

#include <algorithm>
#include <cassert>
#include <complex>
#include <iostream>
#include <iterator>
#include <vector>
#include "Equalizer.h"

namespace dvb {

Equalizer::Equalizer(const myConfig_t& c) :
		config { c } {
	inBufInverse = reinterpret_cast<fftwf_complex*>(fftwf_malloc(
			sizeof(fftwf_complex) * config.continual_pilots_count));
	outBufInverse = reinterpret_cast<fftwf_complex*>(fftwf_malloc(
			sizeof(fftwf_complex) * config.continual_pilots_count));

	planInverse = fftwf_plan_dft_1d(config.continual_pilots_count,
			inBufInverse, outBufInverse, FFTW_BACKWARD, FFTW_ESTIMATE);

	inBufForward = reinterpret_cast<fftwf_complex*>(fftwf_malloc(
			sizeof(fftwf_complex) * config.carriers));
	outBufForward = reinterpret_cast<fftwf_complex*>(fftwf_malloc(
			sizeof(fftwf_complex) * config.carriers));

	planForward = fftwf_plan_dft_1d(config.carriers, inBufForward,
			outBufForward, FFTW_FORWARD, FFTW_ESTIMATE);
}

Equalizer::~Equalizer() {
	fftwf_free(inBufInverse);
	fftwf_free(outBufInverse);
	fftwf_destroy_plan(planInverse);
	fftwf_free(inBufForward);
	fftwf_free(outBufForward);
	fftwf_destroy_plan(planForward);
}

myBuffer_t Equalizer::selCpilots(const myBuffer_t& in) {
	assert(in.size() == config.fft_len);

	auto result = myBuffer_t(config.continual_pilots.size());
	auto i = begin(config.continual_pilots);
	std::generate(begin(result), end(result), [&]() {
		return in[config.zeros_left + *i++];
	});
	return result;
}


std::tuple<myBuffer_t, myBuffer_t> Equalizer::update(const myBuffer_t& in) {

	auto cpilots = selCpilots(in);

	assert(cpilots.size() == config.continual_pilots_count);
	// calculate cir
	auto c = begin(config.continual_pilots_value);
	std::transform(begin(cpilots), end(cpilots),
			reinterpret_cast<myComplex_t*>(inBufInverse), [&](auto v) {
				return v * std::conj(*c++);
//			return v / (*c++);
		});

	// interpolate cir
	fftwf_execute(planInverse);


	std::transform(reinterpret_cast<myComplex_t*>(outBufInverse),
			reinterpret_cast<myComplex_t*>(outBufInverse)
					+ config.continual_pilots_count,
			reinterpret_cast<myComplex_t*>(outBufInverse), [&](auto a) {
				return a / (float)config.continual_pilots_count;
			});

	// zero input buffer
	std::fill(reinterpret_cast<myComplex_t*>(inBufForward),
			reinterpret_cast<myComplex_t*>(inBufForward) + config.carriers, 0);

	// align data
	std::copy(reinterpret_cast<myComplex_t*>(outBufInverse),
			reinterpret_cast<myComplex_t*>(outBufInverse)
					+ config.carrier_center,
			reinterpret_cast<myComplex_t*>(inBufForward));

	std::copy(
			reinterpret_cast<myComplex_t*>(outBufInverse)
					+ config.carrier_center + 1,
			reinterpret_cast<myComplex_t*>(outBufInverse)
					+ config.continual_pilots_count,
			reinterpret_cast<myComplex_t*>(inBufForward) + config.carriers
					- config.carrier_center);



	// execute interpolation
	fftwf_execute(planForward);

	// copy usefull carriers
	auto tmp = myBuffer_t(config.carriers);
	std::copy(begin(in) + config.zeros_left, end(in) - config.zeros_right,
			begin(tmp));

	// apply cir
	auto result = myBuffer_t(config.carriers);

	assert(tmp.size() == result.size());

	std::transform(begin(tmp), end(tmp),
			reinterpret_cast<myComplex_t*>(outBufForward), begin(result),
			[&](auto a, auto b) {
				return a / b;
	});


	return {result, cpilots};

}

}
