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
public:
	DeinterleaverTest(const myConfig_t&);
	virtual ~DeinterleaverTest();
};

} /* namespace dvb */

#endif /* SRC_TEST_DEINTERLEAVERTEST_H_ */
