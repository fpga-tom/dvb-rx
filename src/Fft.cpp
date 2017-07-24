/*
 * Fft.cpp
 *
 *  Created on: Jul 21, 2017
 *      Author: tomas1
 */

#include "Fft.h"

#include <ext/type_traits.h>
#include <algorithm>
#include <deque>
#include <iterator>

namespace dvb {

Fft::Fft(const myConfig_t& c) :
		config { c } {
//	inBuf = std::unique_ptr<fftwf_complex> {
//			reinterpret_cast<fftwf_complex*>(fftwf_malloc(
//					sizeof(fftwf_complex) * config.fft_len)),
//			reinterpret_cast<void (*)(fftwf_complex*)>(fftwf_free) };
//
//	outBuf = std::unique_ptr<fftwf_complex> {
//			reinterpret_cast<fftwf_complex*>(fftwf_malloc(
//					sizeof(fftwf_complex) * config.fft_len)), fftwf_free };
//
//	plan = std::shared_ptr<fftwf_plan_s> { fftwf_plan_dft_1d(config.fft_len,
//			inBuf.get(), outBuf.get(), FFTW_FORWARD, FFTW_ESTIMATE),
//			fftwf_destroy_plan };

	inBuf = reinterpret_cast<fftwf_complex*>(fftwf_malloc(
			sizeof(fftwf_complex) * config.fft_len));

	outBuf = reinterpret_cast<fftwf_complex*>(fftwf_malloc(
			sizeof(fftwf_complex) * config.fft_len));

	plan = fftwf_plan_dft_1d(config.fft_len, inBuf, outBuf, FFTW_FORWARD,
	FFTW_ESTIMATE);
}

Fft::~Fft() {
	fftwf_free(inBuf);
	fftwf_free(outBuf);
	fftwf_destroy_plan(plan);
}

myBuffer_t Fft::update(const myBuffer_t& in) {
	auto out = myBuffer_t(config.fft_len);

	// to avoid ISI, we sample at half of cyclic prefix
	std::copy(begin(in) + config.cp_len, end(in),
			reinterpret_cast<myComplex_t*>(inBuf));

	fftwf_execute(plan);

	// correct rotation introduced by samplig at half of cyclic prefix
	auto tmp = myBuffer_t(config.fft_len);
	auto phase { 0.f };
	const auto phaseinc = 2 * M_PI * config.cp_len / 2 / config.fft_len;
	std::transform(reinterpret_cast<myComplex_t*>(outBuf),
			reinterpret_cast<myComplex_t*>(outBuf) + config.fft_len,
			begin(tmp),
			[&](auto a) {
				return a;
//				auto result = a * myComplex_t {std::cos(phase), std::sin(phase)};
//				phase += phaseinc;
//				return result;
			});

//	std::transform(reinterpret_cast<myComplex_t*>(outBuf) + config.fft_len / 2,
//			reinterpret_cast<myComplex_t*>(outBuf) + config.fft_len,
//			begin(tmp) + config.fft_len / 2,
//			[&](auto a) {
//				auto result = a * std::exp(myComplex_t {std::cos(phase), std::sin(phase)});
//				phase -= phaseinc;
//				return result;
//			});

	std::copy(begin(tmp), begin(tmp) + config.fft_len / 2,
			begin(out) + config.fft_len / 2);
	std::copy(begin(tmp) + config.fft_len / 2, begin(tmp) + config.fft_len,
			begin(out));

	return out;
}

}
