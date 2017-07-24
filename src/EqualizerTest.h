/*
 * CirTest.h
 *
 *  Created on: Jul 23, 2017
 *      Author: tomas1
 */

#ifndef SRC_EQUALIZERTEST_H_
#define SRC_EQUALIZERTEST_H_

#include <string>

#include "mytypes.h"

class EqualizerTest {
	const myConfig_t config;
	const std::string cfile;
	const std::string ofile;
public:
	EqualizerTest(const myConfig_t&, const std::string&, const std::string&);
	virtual ~EqualizerTest();
	void testCir();
};

#endif /* SRC_EQUALIZERTEST_H_ */
