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

}

Nco::~Nco() {
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

auto Nco::freqShift(myBuffer_t& in, myReal_t& corr) {
	assert(in.size() == config.sym_len);
	auto result = myBuffer_t(config.sym_len);

	auto frequency = PI2 * corr / config.sample_rate;
	lv_32fc_t phase_increment = lv_cmake(std::cos(frequency),
			std::sin(frequency));
	volk_32fc_s32fc_x2_rotator_32fc(result.data(), in.data(), phase_increment,
			&phase,
			config.sym_len);

	return result;
}

myBuffer_t Nco::update(myBuffer_t& in, myReal_t ifo, myReal_t f, myReal_t r) {
	auto c = correction(ifo, f, r);
	return freqShift(in, c);
}

}
