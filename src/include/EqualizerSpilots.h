/*
 * EqualizerSpilots.h
 *
 *  Created on: Jul 25, 2017
 *      Author: tomas1
 */

#ifndef SRC_EQUALIZERSPILOTS_H_
#define SRC_EQUALIZERSPILOTS_H_

#include <fftw3.h>
#include <mytypes.h>

namespace dvb {

class EqualizerSpilots {

	const myConfig_t config;
	std::vector<myBuffer_t> spilotsBuf;
	std::vector<int> scattered_indices;
	// inverse fft variables
	fftwf_complex *inBufInverse, *outBufInverse;
	fftwf_plan_s *planInverse;

	// forward fft variables
	fftwf_complex *inBufForward, *outBufForward;
	fftwf_plan_s *planForward;

	// helper methods
	myBuffer_t selSpilots(const myBuffer_t&, int frame);
public:
	EqualizerSpilots(const myConfig_t&);
	virtual ~EqualizerSpilots();
	myBuffer_t update(const myBuffer_t&, int frame);
};

} /* namespace dvb */

#endif /* SRC_EQUALIZERSPILOTS_H_ */
