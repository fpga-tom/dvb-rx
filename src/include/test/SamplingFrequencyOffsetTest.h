/*
 * SamplingFrequencyOffsetTest.h
 *
 *  Created on: Aug 5, 2017
 *      Author: tomas1
 */

#ifndef SRC_TEST_SAMPLINGFREQUENCYOFFSETTEST_H_
#define SRC_TEST_SAMPLINGFREQUENCYOFFSETTEST_H_

#include <mytypes.h>
#include <string>

namespace dvb {

class SamplingFrequencyOffsetTest {
	const myConfig_t config;
	const std::string cfile;
	const std::string ofile;
public:
	SamplingFrequencyOffsetTest(const myConfig_t&, const std::string&,
			const std::string&);
	virtual ~SamplingFrequencyOffsetTest();
	void testSRO();
};

} /* namespace dvb */

#endif /* SRC_TEST_SAMPLINGFREQUENCYOFFSETTEST_H_ */
