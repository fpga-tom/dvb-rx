/*
 * Cir.h
 *
 *  Created on: Jul 23, 2017
 *      Author: tomas1
 */

#ifndef SRC_EQUALIZER_H_
#define SRC_EQUALIZER_H_

#include <fftw3.h>
#include <tuple>

#include "mytypes.h"

class Equalizer {
	FRIEND_TEST(EqualizerTest);

	const myConfig_t config;
	// inverse fft variables
	fftwf_complex *inBufInverse, *outBufInverse;
	fftwf_plan_s *planInverse;

	// forward fft variables
	fftwf_complex *inBufForward, *outBufForward;
	fftwf_plan_s *planForward;
public:
	Equalizer(const myConfig_t&);
	virtual ~Equalizer();
	myBuffer_t update(const myBuffer_t&, const myBuffer_t&);
};

#endif /* SRC_EQUALIZER_H_ */
