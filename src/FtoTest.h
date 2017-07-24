/*
 * FtoTest.h
 *
 *  Created on: Jul 23, 2017
 *      Author: tomas1
 */

#ifndef SRC_FTOTEST_H_
#define SRC_FTOTEST_H_

#include <string>

#include "mytypes.h"

class FtoTest {
	const myConfig_t config;
	const std::string cfile;
	const std::string ofile;
public:
	FtoTest(const myConfig_t&, const std::string&, const std::string&);
	virtual ~FtoTest();
	void testFto();
};

#endif /* SRC_FTOTEST_H_ */
