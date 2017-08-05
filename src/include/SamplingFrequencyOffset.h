/*
 * SamplingFrequencyOffset.h
 *
 *  Created on: Aug 4, 2017
 *      Author: tomas1
 */

#ifndef SRC_SAMPLINGFREQUENCYOFFSET_H_
#define SRC_SAMPLINGFREQUENCYOFFSET_H_

#include <mytypes.h>

namespace dvb {


const int SRO_N = 20;
const myReal_t SRO_P_GAIN = 1e-5;
const myReal_t SRO_I_GAIN = 1e-2;

class SamplingFrequencyOffset {
	FRIEND_TEST(SamplingFrequencyOffsetTest);
	const myConfig_t config;
	myBuffer_t prev;
	myReal_t pilotsSquareSum;
	std::deque<myComplex_t> delayA;
	std::deque<myComplex_t> delayB;
	myReal_t integral;
	// helper methods
	myBuffer_t cpilots(const myBuffer_t&);
	myBufferR_t coeff(const myReal_t);
	myReal_t binom(myReal_t, myReal_t);
	myBuffer_t filter(const myBuffer_t&, const myBufferR_t&);
public:
	SamplingFrequencyOffset(const myConfig_t&);
	virtual ~SamplingFrequencyOffset();
	myReal_t sro(const myBuffer_t&);
	myBuffer_t update(const myBuffer_t&, const myReal_t);
};

} /* namespace dvb */

#endif /* SRC_SAMPLINGFREQUENCYOFFSET_H_ */
