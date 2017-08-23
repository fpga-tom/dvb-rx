/*
 * DemapLiquid.h
 *
 *  Created on: Aug 18, 2017
 *      Author: tomas1
 */

#ifndef SRC_DEMAPLIQUID_H_
#define SRC_DEMAPLIQUID_H_

#include <mytypes.h>
#include <liquid/liquid.h>

namespace dvb {

class DemapLiquid {
	FRIEND_TEST(DemapTest);
	const myConfig_t config;
	modem demod;
	int demap(const myComplex_t&);
public:
	DemapLiquid(const myConfig_t&);
	virtual ~DemapLiquid();
	myBitset_t update(const myBuffer_t&);
};

} /* namespace dvb */

#endif /* SRC_DEMAPLIQUID_H_ */
