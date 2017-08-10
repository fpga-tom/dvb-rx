/*
 * Nco.h
 *
 *  Created on: Jul 22, 2017
 *      Author: tomas1
 */

#ifndef SRC_NCO_H_
#define SRC_NCO_H_

#include <cmath>

#include "mytypes.h"

namespace dvb {

const myReal_t PI2 = 2 * M_PI;
const myReal_t NCO_P_GAIN = 1e-8;
const myReal_t NCO_I_GAIN = 5e-2;

class Nco {
	FRIEND_TEST(NcoTest);

	const myConfig_t config;
	myReal_t phase;

	// pi controller integral part
	myReal_t integral;

	// helper methods
	auto correction(myReal_t&, myReal_t&, myReal_t&);
	auto freqShift(myBuffer_t&, myReal_t&);
public:
	Nco(const myConfig_t&);
	virtual ~Nco();
	myBuffer_t update(myBuffer_t&, myReal_t, myReal_t, myReal_t);
};

}

#endif /* SRC_NCO_H_ */
