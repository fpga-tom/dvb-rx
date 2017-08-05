/*
 * ResidualFrequencyTest.h
 *
 *  Created on: Aug 5, 2017
 *      Author: tomas1
 */

#ifndef SRC_TEST_RESIDUALFREQUENCYTEST_H_
#define SRC_TEST_RESIDUALFREQUENCYTEST_H_

namespace dvb {

class ResidualFrequencyTest {
	const myConfig_t config;
	const std::string cfile;
	const std::string ofile;
public:
	ResidualFrequencyTest(const myConfig_t&, const std::string&,
			const std::string&);
	virtual ~ResidualFrequencyTest();
	void testRFO();
};

} /* namespace dvb */

#endif /* SRC_TEST_RESIDUALFREQUENCYTEST_H_ */
