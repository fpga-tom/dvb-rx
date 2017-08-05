/*
 * Cir.h
 *
 *  Created on: Jul 23, 2017
 *      Author: tomas1
 */

#ifndef SRC_EQUALIZER_H_
#define SRC_EQUALIZER_H_

#include <fftw3.h>
#include <mytypes.h>
#include <tuple>

namespace dvb {

class Equalizer {
	FRIEND_TEST(EqualizerTest);
	FRIEND_TEST(FineTimingOffsetTest);
	FRIEND_TEST(DataSelectorTest);
	FRIEND_TEST(SamplingFrequencyOffsetTest);

	const myConfig_t config;
	// inverse fft variables
	fftwf_complex *inBufInverse, *outBufInverse;
	fftwf_plan_s *planInverse;

	// forward fft variables
	fftwf_complex *inBufForward, *outBufForward;
	fftwf_plan_s *planForward;

	// helper methods
	myBuffer_t selCpilots(const myBuffer_t&);
public:
	Equalizer(const myConfig_t&);
	virtual ~Equalizer();
	std::tuple<myBuffer_t, myBuffer_t> update(const myBuffer_t&);
};

} /* namespace dvb */

#endif /* SRC_EQUALIZER_H_ */
