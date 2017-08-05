/*
 * Nco.cpp
 *
 *  Created on: Jul 22, 2017
 *      Author: tomas1
 */

#include "Nco.h"

#include <algorithm>
#include <complex>
#include <iterator>
#include <vector>

namespace dvb {

Nco::Nco(const myConfig_t& c) :
		config { c }, phase { 0 }, integral { 0 } {
}

Nco::~Nco() {
}

auto Nco::correction(myReal_t& ifo, myReal_t& f, myReal_t& r) {
	auto fcorr { 0.f };
	if (std::abs(ifo) >= 1) {
		fcorr = -ifo * config.sample_rate / config.fft_len;
	} else {
		if (std::abs(f) >= 1) {
			fcorr = -f;
		} else {
			fcorr = r;
		}
	}
	auto proportional = NCO_P_GAIN * fcorr;
	integral += NCO_I_GAIN * fcorr;
	return proportional + integral;
}

auto Nco::freqShift(myBuffer_t& in, myReal_t& corr) {
	auto result = myBuffer_t(config.sym_len);

	auto inc = PI2 * corr / config.sample_rate;
	std::transform(begin(in), end(in), begin(result), [&](auto a) {
		phase += inc;
		while(phase > PI2) {
			phase -= PI2;
		}
		while(phase < PI2) {
			phase += PI2;
		}
		return a * myComplex_t {std::cos(phase), std::sin(phase)};
	});
	return result;
}

myBuffer_t Nco::update(myBuffer_t& in, myReal_t ifo, myReal_t f, myReal_t r) {
	auto c = correction(ifo, f, r);
	return freqShift(in, c);
}

}
