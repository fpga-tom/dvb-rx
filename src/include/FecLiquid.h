/*
 * FecLiquid.h
 *
 *  Created on: Aug 19, 2017
 *      Author: tomas1
 */

#ifndef SRC_FECLIQUID_H_
#define SRC_FECLIQUID_H_

#include <mytypes.h>

namespace dvb {

class FecLiquid {
	const myConfig_t& config;
public:
	FecLiquid(const myConfig_t&);
	virtual ~FecLiquid();
};

} /* namespace dvb */

#endif /* SRC_FECLIQUID_H_ */
