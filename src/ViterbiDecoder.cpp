/*
 * ViterbiDecoder.cpp
 *
 *  Created on: Jul 30, 2017
 *      Author: tomas1
 */

#include <ViterbiDecoder.h>
#include <algorithm>
#include <array>
#include <iterator>
#include <limits>

namespace dvb {

ViterbiDecoder::ViterbiDecoder(const myConfig_t&c, int tb) :
		config { c }, state { 0 }, tracebackBuf(tb * c.statesCount), tracebackBit(tb * c.statesCount), traceback(
				tb) {
	current = std::make_shared<std::vector<int>>(config.statesCount);
	next = std::make_shared<std::vector<int>>(config.statesCount);

	std::fill(begin(*next), end(*next), std::numeric_limits<int>::max());
	std::fill(begin(*current), end(*current), 0);
	std::fill(begin(tracebackBuf), end(tracebackBuf), 0);
}

ViterbiDecoder::~ViterbiDecoder() {
}

int ViterbiDecoder::hamming(unsigned char a, unsigned char b) {
	auto c = (a ^ b) & 0x3;

	return c;
}

myBufferB_t ViterbiDecoder::update(myBitset_t& bitset) {
	auto result = myBufferB_t(bitCount * 2 / 3 / 8);
	for (auto tb { 0 }; tb < traceback; tb++) {
		std::fill(begin(*next), end(*next), std::numeric_limits<int>::max());
		for (auto currentState { 0 }; currentState < config.statesCount;
				currentState++) {

			auto data = bitset[tb * 2 + 1] << 1 | bitset[tb * 2];
			for (auto bit { 0 }; bit < 2; bit++) {
				auto h = hamming(config.outputs[bit][currentState], data);
				auto nextState = config.nextStates[bit][currentState];

				if(auto pm = h + (*current)[currentState]; pm < (*next)[nextState]) {
					(*next)[nextState] = pm;
					tracebackBuf[tb * config.statesCount + nextState] = currentState;
					tracebackBuf[tb * config.statesCount + nextState] = bit;
				}
			}
		}
		swap(current, next);
	}

	return result;
}

} /* namespace dvb */
