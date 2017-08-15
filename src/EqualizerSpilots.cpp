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
		config { c }, scatteredPilotsInverse(std::vector<myBuffer_t>(4)) {
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

	for (auto frame { 0 }; frame < 4; frame++) {
		auto tmp = myBufferR_t(config.scattered_pilots_count);
		std::transform(begin(config.scattered_pilots_value[frame]),
				end(config.scattered_pilots_value[frame]),
				begin(tmp), [](auto v) {
					return 1.0f/ v;
				});
		scatteredPilotsInverse[frame] = myBuffer_t(
				config.scattered_pilots_count);
		auto it = begin(scatteredPilotsInverse[frame]);
		for (auto c : tmp) {
			*it++ = myComplex_t { c, 0.f };
		}
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
	assert(in.size() == config.fft_len);

	auto result = myBuffer_t(config.scattered_pilots_count);
	auto i = begin(config.scattered_pilots[frame]);
	std::generate(begin(result), end(result), [&]() {
		return in[config.zeros_left + *i++];
	});
	return result;
}


myBuffer_t EqualizerSpilots::update(const myBuffer_t& in, int frame) {
	auto spilots = selSpilots(in, frame);

	assert(spilots.size() == config.scattered_pilots_count);
	// calculate cir
	volk_32fc_x2_multiply_32fc(reinterpret_cast<lv_32fc_t*>(inBufInverse),
			spilots.data(),
			scatteredPilotsInverse[frame].data(),
			config.scattered_pilots_count);

	// interpolate cir
	fftwf_execute(planInverse);

	std::transform(reinterpret_cast<myComplex_t*>(outBufInverse),
			reinterpret_cast<myComplex_t*>(outBufInverse)
					+ config.scattered_pilots_count,
			reinterpret_cast<myComplex_t*>(outBufInverse), [&](auto a) {
				return a / (float)config.scattered_pilots_count;
			});

	// zero input buffer
	std::fill(reinterpret_cast<myComplex_t*>(inBufForward),
			reinterpret_cast<myComplex_t*>(inBufForward) + config.carriers, 0);

	int cc = config.scattered_pilots_count / 2;
	// align data
	std::copy(reinterpret_cast<myComplex_t*>(outBufInverse),
			reinterpret_cast<myComplex_t*>(outBufInverse) + cc,
			reinterpret_cast<myComplex_t*>(inBufForward));


	std::copy(reinterpret_cast<myComplex_t*>(outBufInverse) + cc,
			reinterpret_cast<myComplex_t*>(outBufInverse)
					+ config.scattered_pilots_count,
			reinterpret_cast<myComplex_t*>(inBufForward) + config.carriers
					- cc);



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

	return result;
}

} /* namespace dvb */
