/*
 * ViterbiDecoder.h
 *
 *  Created on: Jul 30, 2017
 *      Author: tomas1
 */

#ifndef SRC_VITERBIDECODER_H_
#define SRC_VITERBIDECODER_H_

#include <mytypes.h>
#include <memory>

namespace dvb {

class ViterbiDecoder {
	const myConfig_t& config;
	int parentRead[2];
	int parentWrite[2];
	public:
	ViterbiDecoder(const myConfig_t&);
	virtual ~ViterbiDecoder();
	myBufferB_t update(myBitset_t&);

};

} /* namespace dvb */

#endif /* SRC_VITERBIDECODER_H_ */
