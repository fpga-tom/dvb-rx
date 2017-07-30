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
#include <vector>

namespace dvb {

class ViterbiDecoder {
	const myConfig_t& config;
	int state;
	std::vector<int> traceback;
	std::shared_ptr<std::vector<int>> prev;
	std::shared_ptr<std::vector<int>> current;
public:
	ViterbiDecoder(const myConfig_t&, int);
	virtual ~ViterbiDecoder();
	myBufferB_t update(myBitset_t&);

};

} /* namespace dvb */

#endif /* SRC_VITERBIDECODER_H_ */
