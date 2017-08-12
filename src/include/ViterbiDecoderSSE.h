/*
 * ViterbiDecoderSSE.h
 *
 *  Created on: Aug 10, 2017
 *      Author: tomas1
 */

#ifndef SRC_VITERBIDECODERSSE_H_
#define SRC_VITERBIDECODERSSE_H_

#include <mytypes.h>
#include <tuple>

namespace dvb {

class ViterbiDecoderSSE {
	const myConfig_t config;
	const myInteger_t d_ntraceback;
	int mettab[2][256];
	int d_init;
public:
	ViterbiDecoderSSE(const myConfig_t&, const myInteger_t);
	virtual ~ViterbiDecoderSSE();
	myBufferB_t update(const myBitset_t&, myInteger_t frame);
};

} /* namespace dvb */

#endif /* SRC_VITERBIDECODERSSE_H_ */
