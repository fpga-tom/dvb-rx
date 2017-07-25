/*
 * Fto.cpp
 *
 *  Created on: Jul 23, 2017
 *      Author: tomas1
 */

#include <FineTimingOffset.h>
#include <algorithm>
#include <cassert>
#include <complex>
#include <iostream>
#include <iterator>
#include <vector>

namespace dvb {

FineTimingOffset::FineTimingOffset(const myConfig_t& c) :
		config { c } {
//	inBuf = reinterpret_cast<fftwf_complex*>(fftwf_malloc(
//			sizeof(fftwf_complex) * config.fft_len));
//
//	outBuf = reinterpret_cast<fftwf_complex*>(fftwf_malloc(
//			sizeof(fftwf_complex) * config.fft_len));
//
//	plan = fftwf_plan_dft_1d(config.fft_len, inBuf, outBuf, FFTW_BACKWARD,
//	FFTW_ESTIMATE);


}

FineTimingOffset::~FineTimingOffset() {
//	fftwf_free(inBuf);
//	fftwf_free(outBuf);
//	fftwf_destroy_plan(plan);
}

myReal_t FineTimingOffset::update(const myBuffer_t& in) {
	assert(in.size() == config.continual_pilots_count);
	auto z = myBuffer_t(config.continual_pilots_count - 1);
	auto x = myBuffer_t(config.continual_pilots_count - 1);
	auto y = myBuffer_t(config.continual_pilots_count - 1);

	std::transform(begin(in), end(in) - 1, begin(in) + 1, begin(z),
			[](auto a, auto b) {
				return std::conj(a) * b;
			});

	std::transform(begin(config.continual_pilots_value),
			end(config.continual_pilots_value) - 1,
			begin(config.continual_pilots_value) + 1, begin(x),
			[](auto a, auto b) {
				return std::conj(a) * b;
			});


	std::transform(begin(x), end(x), begin(z), begin(y), [](auto a, auto b) {
		return std::conj(a) * b;
	});
	auto acc = myComplex_t { 0, 0 };
	for (auto a : y) {
		acc += a;
	}
	return config.continual_pilots_count * std::arg(acc) / 2.0 / M_PI;
//	auto z = myBuffer_t(config.fft_len);
//	std::fill(begin(z), end(z), 0);
//	auto it = begin(in);
//	for (auto i : config.continual_pilots) {
//		z[config.zeros_left + i] = *it++;
//	}
//	std::copy(begin(z), end(z), reinterpret_cast<myComplex_t*>(inBuf));
//	fftwf_execute(plan);
//
//	auto first = std::find_if(reinterpret_cast<myComplex_t*>(outBuf),
//			reinterpret_cast<myComplex_t*>(outBuf)
//					+ config.fft_len / 2,
//			[&](auto a) {
//				return std::abs(a) > 550;
//			});
//
//	auto idx = std::distance(reinterpret_cast<myComplex_t*>(outBuf), first);
//
////	std::for_each(reinterpret_cast<myComplex_t*>(outBuf),
////			reinterpret_cast<myComplex_t*>(outBuf) + config.fft_len / 2,
////			[](auto a) {
////				std::cout << std::abs(a) << ",";
////			});
////
////	std::cout << std::endl;
////
////	std::cout << idx << std::endl;
//
//	return idx;


}

}
