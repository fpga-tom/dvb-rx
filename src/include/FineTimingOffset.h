/*
 * Fto.h
 *
 *  Created on: Jul 23, 2017
 *      Author: tomas1
 */

#ifndef SRC_FTO_H_
#define SRC_FTO_H_

#include <fftw3.h>

#include "mytypes.h"

namespace dvb {

class FineTimingOffset {
	FRIEND_TEST(FtoTest);

	const myConfig_t config;
public:
	FineTimingOffset(const myConfig_t&);
	virtual ~FineTimingOffset();
	myReal_t update(const myBuffer_t&);
};

}

#endif /* SRC_FTO_H_ */
