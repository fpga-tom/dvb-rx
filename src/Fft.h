/*
 * Fft.h
 *
 *  Created on: Jul 21, 2017
 *      Author: tomas1
 */

#ifndef SRC_FFT_H_
#define SRC_FFT_H_

#include <fftw3.h>
#include <memory>

#include "mytypes.h"

struct fftwf_plan_s;

class Fft {
	FRIEND_TEST(FftTest);

	myConfig_t config;

	fftwf_complex *inBuf, *outBuf;
	fftwf_plan_s* plan;

public:
	Fft(const myConfig_t&);
	virtual ~Fft();

	myBuffer_t update(const myBuffer_t& in);
};

#endif /* SRC_FFT_H_ */
