/*
 * Nco.cpp
 *
 *  Created on: Jul 22, 2017
 *      Author: tomas1
 */

#include <Nco.h>
#include <volk/volk_complex.h>
#include <volk/volk_malloc.h>
#include <algorithm>
#include <cassert>
#include <complex>
#include <iostream>
#include <iterator>
#include <vector>

namespace dvb {

Nco::Nco(const myConfig_t& c) :
		config { c }, phase { lv_cmake(1.0f, 0.0f) }
, integral {0} {
	unsigned int alignment = volk_get_alignment();
	in = (lv_32fc_t*) volk_malloc(sizeof(lv_32fc_t) * config.sym_len,
			alignment);
	out = (lv_32fc_t*) volk_malloc(
			sizeof(lv_32fc_t) * config.sym_len, alignment);

}

Nco::~Nco() {
	volk_free(in);
	volk_free(out);
}

auto Nco::correction(myReal_t& ifo, myReal_t& f, myReal_t& r) {
	auto fcorr { 0.f };
	if (std::abs(ifo) >= 1.0f) {
		fcorr = -ifo * config.sample_rate / config.fft_len;
	} else {
		if (std::abs(f) >= 0.5f) {
			fcorr = -f;
		} else {
			fcorr = -r;
		}
	}
	auto proportional = NCO_P_GAIN * fcorr;
	integral += NCO_I_GAIN * fcorr;
	return proportional + integral;
}

auto Nco::freqShift(myBuffer_t& inData, myReal_t& corr) {
	assert(inData.size() == config.sym_len);
	auto result = myBuffer_t(config.sym_len);

	std::copy(begin(inData), end(inData), reinterpret_cast<myComplex_t*>(in));

	auto frequency = PI2 * corr / config.sample_rate;
	lv_32fc_t phase_increment = lv_cmake(std::cos(frequency),
			std::sin(frequency));
	volk_32fc_s32fc_x2_rotator_32fc(out, in, phase_increment, &phase,
			config.sym_len);

	std::copy(reinterpret_cast<myComplex_t*>(out),
			reinterpret_cast<myComplex_t*>(out) + config.sym_len,
			begin(result));

	return result;
}

myBuffer_t Nco::update(myBuffer_t& in, myReal_t ifo, myReal_t f, myReal_t r) {
	auto c = correction(ifo, f, r);
	return freqShift(in, c);
}

}
