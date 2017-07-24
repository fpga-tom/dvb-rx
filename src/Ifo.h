/*
 * Ifo.h
 *
 *  Created on: Jul 21, 2017
 *      Author: tomas1
 */

#ifndef SRC_IFO_H_
#define SRC_IFO_H_

#include "mytypes.h"

/**
 * calculates integer frequency offset
 */
class Ifo {
	FRIEND_TEST(IfoTest);

	myConfig_t config;
	// previous ofdm symbol
	myBuffer_t prev;
public:
	Ifo(const myConfig_t&);
	virtual ~Ifo();

	int update(myBuffer_t& in);
};

#endif /* SRC_IFO_H_ */
