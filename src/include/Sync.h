/*
 * Sync.h
 *
 *  Created on: Jul 19, 2017
 *      Author: tomas1
 */

#ifndef SRC_SYNC_H_
#define SRC_SYNC_H_

#include <mytypes.h>
#include <stddef.h>
#include <volk/volk_complex.h>
#include <cmath>
#include <deque>
#include <memory>
#include <tuple>

namespace dvb {

const myReal_t SYNC_P_GAIN = 1e-10;
const myReal_t SYNC_I_GAIN = 9e-4;

const myInteger_t lockCount = 5;

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
	std::deque<myInteger_t> peakDelay;
	int accPeak;
	myReal_t integral;
	myReal_t peak;

	int currentLock;

	// framer variables
	std::shared_ptr<myBuffer_t> current;		// current frame
	std::shared_ptr<myBuffer_t> next;			// next frame

	lv_32fc_t *in;
	lv_32fc_t *out;

	// helper methods
	myBuffer_t correlate(const myBuffer_t&, myBuffer_t&, myDelay_t& accDelay,
			myComplex_t& acc);
	size_t findPeak(const myBuffer_t&);
	myBuffer_t align(const myBuffer_t&, myInteger_t);
public:
	Sync(const myConfig_t&);
	virtual ~Sync();
	std::tuple<myBuffer_t, myReal_t, bool> update(const myBuffer_t& in,
			const myReal_t);
	myReal_t getSro() const {
		static auto integral = 0.f;
		auto diff = (std::floor(peak) - peak);
		integral += diff * 2e-3;
//		return diff * 1e-5 + integral;
		return 1 + diff;
	}
};

} /* namespace dvb */

#endif /* SRC_SYNC_H_ */
