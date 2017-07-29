/*
 * Deinterleaver.cpp
 *
 *  Created on: Jul 28, 2017
 *      Author: tomas1
 */

#include "Deinterleaver.h"

namespace dvb {

Deinterleaver::Deinterleaver(const myConfig_t& c) :
		config { c } {

}

Deinterleaver::~Deinterleaver() {
}

myBitset_t Deinterleaver::symbol(const myBitset_t& bitset, int frame) {
	auto tmp = myBitset_t { };
	if (frame % 2 == 0) {
		for (auto it { 0U }; it < config.H.size(); it++) {
			tmp[it] = bitset[config.H[it]];
		}
	} else {
		for (auto it { 0U }; it < config.H.size(); it++) {
			tmp[config.H[it]] = bitset[it];
		}
	}
	return tmp;
}

myBitset_t Deinterleaver::bit(const myBitset_t& tmp) {
	auto result = myBitset_t { };
	for (auto outer { 0 }; outer < 48; outer++) {
		for (auto inner { 0 }; inner < 756; inner++) {
			result[config.bit_table[inner] - 1 + outer * 756] = tmp[inner
					+ outer * 756];
		}
	}
	return result;
}

myBitset_t Deinterleaver::update(const myBitset_t& bitset, int frame) {
	auto tmp = symbol(bitset, frame);
	return bit(tmp);
}

} /* namespace dvb */
