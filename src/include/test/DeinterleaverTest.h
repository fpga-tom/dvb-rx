/*
 * DeinterleaverTest.h
 *
 *  Created on: Jul 29, 2017
 *      Author: tomas1
 */

#ifndef SRC_TEST_DEINTERLEAVERTEST_H_
#define SRC_TEST_DEINTERLEAVERTEST_H_

#include <mytypes.h>

namespace dvb {

class DeinterleaverTest {
	const myConfig_t& config;
	const std::string in;
	const std::string out;
public:
	DeinterleaverTest(const myConfig_t&, const std::string&, const std::string&);
	virtual ~DeinterleaverTest();
	void testSymbol();
	void testBit();
};

} /* namespace dvb */

#endif /* SRC_TEST_DEINTERLEAVERTEST_H_ */
