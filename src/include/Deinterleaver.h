/*
 * Deinterleaver.h
 *
 *  Created on: Jul 28, 2017
 *      Author: tomas1
 */

#ifndef SRC_DEINTERLEAVER_H_
#define SRC_DEINTERLEAVER_H_

#include <mytypes.h>

namespace dvb {

class Deinterleaver {
	FRIEND_TEST(DeinterleaverTest);
	const myConfig_t config;

	// helper methods
	myBitset_t symbol(const myBitset_t&, int frame);
	myBitset_t bit(const myBitset_t&);
public:
	Deinterleaver(const myConfig_t&);
	virtual ~Deinterleaver();
	myBitset_t update(const myBitset_t&, int);

};

} /* namespace dvb */

#endif /* SRC_DEINTERLEAVER_H_ */
