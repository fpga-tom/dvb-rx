/*
 * NcoTest.h
 *
 *  Created on: Jul 22, 2017
 *      Author: tomas1
 */

#ifndef SRC_NCOTEST_H_
#define SRC_NCOTEST_H_

#include "Nco.h"

#include <string>

#include "mytypes.h"

class NcoTest {
	const myConfig_t config;
	const std::string cfile;
	const std::string ofile;
public:
	NcoTest(const myConfig_t&, const std::string&, const std::string&);
	virtual ~NcoTest();
	void testNco();
	void testNcoFractional();
};

#endif /* SRC_NCOTEST_H_ */
