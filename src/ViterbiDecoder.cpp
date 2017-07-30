/*
 * ViterbiDecoder.cpp
 *
 *  Created on: Jul 30, 2017
 *      Author: tomas1
 */

#include "ViterbiDecoder.h"

namespace dvb {

ViterbiDecoder::ViterbiDecoder(const myConfig_t&c, int tb) :
		config { c }, state { 0 }, traceback(tb * 4) {
			current = std::shared_ptr<std::vector<int>>(4);
			prev = std::shared_ptr<std::vector<int>>(4);
}

ViterbiDecoder::~ViterbiDecoder() {
}

myBufferB_t ViterbiDecoder::update(myBitset_t& bitset) {
	auto result = myBufferB_t(36288 * 2 / 3);

	return result;
}

} /* namespace dvb */
