/*
 * Demap.h
 *
 *  Created on: Jul 27, 2017
 *      Author: tomas1
 */

#ifndef SRC_DEMAP_H_
#define SRC_DEMAP_H_

#include <mytypes.h>

namespace dvb {

const float DEMAP_X = 8;
const float DEMAP_Y = 8;
const int DEMAP_DEPTH = 2;

class Demap {
	FRIEND_TEST(DemapTest);
	const myConfig_t config;
	int demap(const myComplex_t&, int);
public:
	Demap(const myConfig_t&);
	virtual ~Demap();
	myBitset_t update(const myBuffer_t&);

};

} /* namespace dvb */

#endif /* SRC_DEMAP_H_ */
