/*
 * DataSelector.h
 *
 *  Created on: Jul 24, 2017
 *      Author: tomas1
 */

#ifndef SRC_DATASELECTOR_H_
#define SRC_DATASELECTOR_H_

#include <mytypes.h>
#include <tuple>

namespace dvb {

class DataSelector {
	const myConfig_t config;
	std::vector<std::vector<int>> dataIdx;

	// helper methods
	int frameNum(const myBuffer_t&);
public:
	DataSelector(const myConfig_t&);
	virtual ~DataSelector();
	std::tuple<myBuffer_t, int> update(const myBuffer_t&);
};

} /* namespace dvb */

#endif /* SRC_DATASELECTOR_H_ */
