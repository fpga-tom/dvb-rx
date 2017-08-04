/*
 * Descrambler.h
 *
 *  Created on: Aug 1, 2017
 *      Author: tomas1
 */

#ifndef SRC_DESCRAMBLER_H_
#define SRC_DESCRAMBLER_H_

#include <mytypes.h>

namespace dvb {

class Descrambler {
	const myConfig_t& config;
	int parentRead[2];
	int parentWrite[2];
public:
	Descrambler(const myConfig_t&);
	virtual ~Descrambler();
	myBufferB_t update(const myBufferB_t&);
};

} /* namespace dvb */

#endif /* SRC_DESCRAMBLER_H_ */
