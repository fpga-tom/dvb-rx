/*
 * DemapTest.h
 *
 *  Created on: Jul 28, 2017
 *      Author: tomas1
 */

#ifndef SRC_TEST_DEMAPTEST_H_
#define SRC_TEST_DEMAPTEST_H_

#include <mytypes.h>

namespace dvb {

const int DEMAP_TEST_CONSTELLATION[] = {
	1, 17, 21, 5, 4, 20, 16, 0, 33, 49, 53,
		37, 36, 52, 48, 32, 41, 57, 61, 45, 44, 60, 56, 40, 9, 25, 29, 13, 12,
		28, 24, 8, 11, 27, 31, 15, 14, 30, 26, 10, 43, 59, 63, 47, 46, 62, 58,
		42, 35, 51, 55, 39, 38, 54, 50, 34, 3, 19, 23, 7, 6, 22, 18,
	2
};

class DemapTest {
	const myConfig_t config;
	const std::string in;
	const std::string out;
public:
	DemapTest(const myConfig_t&, const std::string&, const std::string&);
	virtual ~DemapTest();
	void testDemap();
	void testUpdate();
};

} /* namespace dvb */

#endif /* SRC_TEST_DEMAPTEST_H_ */
