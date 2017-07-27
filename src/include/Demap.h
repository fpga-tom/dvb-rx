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

const float DEMAP_X = 7;
const float DEMAP_Y = 7;
const int DEMAP_DEPTH = 6;

class Demap {
	const myConfig_t config;
	int demap(const myComplex_t&, int)
public:
	Demap(myConfig_t&);
	virtual ~Demap();
	std::vector<bool> update(const myBuffer_t&)

};

} /* namespace dvb */

#endif /* SRC_DEMAP_H_ */
