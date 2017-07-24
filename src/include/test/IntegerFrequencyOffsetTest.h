/*
 * IfoTest.h
 *
 *  Created on: Jul 22, 2017
 *      Author: tomas1
 */

#ifndef SRC_IFOTEST_H_
#define SRC_IFOTEST_H_

#include <IntegerFrequencyOffset.h>
#include "mytypes.h"

namespace dvb {

class IntegerFrequencyOffsetTest: public IntegerFrequencyOffset {
	const std::string cfile;
	const std::string ofile;
public:
	IntegerFrequencyOffsetTest(const myConfig_t&, const std::string&, const std::string&);
	virtual ~IntegerFrequencyOffsetTest();
	void testIfo();
};

}

#endif /* SRC_IFOTEST_H_ */
