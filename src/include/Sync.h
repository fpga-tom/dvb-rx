/*
 * Sync.h
 *
 *  Created on: Jul 19, 2017
 *      Author: tomas1
 */

#ifndef SRC_SYNC_H_
#define SRC_SYNC_H_

#include <memory>

#include "mytypes.h"

namespace dvb {

const myReal_t SYNC_P_GAIN = 1e-5;
const myReal_t SYNC_I_GAIN = 3e-2;

const int lockCount = 3;

/**
 * Takes input stream of complex samples and generates
 * synchronized frames.
 */
class Sync {
	FRIEND_TEST(SyncTest);

	const myConfig_t config; // config variable

	// correlator variables
	myBuffer_t delay; 							// correlator delay reg
	myDelay_t accDelay;  				// correlator accumulator helper delay
	myComplex_t acc; 							// corelator accumulator
	unsigned long current_size;

	// peak finder variables
	std::deque<int> peakDelay;
	int accPeak;
	myReal_t integral;
	myReal_t peak;

	int currentLock;

	// framer variables
	std::shared_ptr<myBuffer_t> current;		// current frame
	std::shared_ptr<myBuffer_t> next;			// next frame

	// helper methods
	myBuffer_t correlate(const myBuffer_t&, myBuffer_t&, myDelay_t& accDelay,
			myComplex_t& acc);
	size_t findPeak(const myBuffer_t&);
	myBuffer_t align(const myBuffer_t&, size_t);
public:
	Sync(const myConfig_t&);
	virtual ~Sync();
	std::tuple<myBuffer_t, myReal_t> update(const myBuffer_t& in,
			const myReal_t);
};

} /* namespace dvb */

#endif /* SRC_SYNC_H_ */
