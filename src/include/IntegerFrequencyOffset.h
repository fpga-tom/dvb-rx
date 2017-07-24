/*
 * Ifo.h
 *
 *  Created on: Jul 21, 2017
 *      Author: tomas1
 */

#ifndef SRC_IFO_H_
#define SRC_IFO_H_

#include "mytypes.h"

namespace dvb {
/**
 * calculates integer frequency offset
 */
class IntegerFrequencyOffset {
	FRIEND_TEST(IntegerFrequencyOffsetTest);

	myConfig_t config;
	// previous ofdm symbol
	myBuffer_t prev;
public:
	IntegerFrequencyOffset(const myConfig_t&);
	virtual ~IntegerFrequencyOffset();

	int update(myBuffer_t& in);
};

}

#endif /* SRC_IFO_H_ */
