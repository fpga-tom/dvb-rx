/*
 * CpilotsSelectorTest.h
 *
 *  Created on: Jul 22, 2017
 *      Author: tomas1
 */

#ifndef SRC_CPILOTSSELECTORTEST_H_
#define SRC_CPILOTSSELECTORTEST_H_

#include "CpilotsSelector.h"

class CpilotsSelectorTest {
	const myConfig_t config;
	const std::string cfile;
	const std::string ofile;
	CpilotsSelector selector;
public:
	CpilotsSelectorTest(const myConfig_t&, const std::string&,
			const std::string&);
	virtual ~CpilotsSelectorTest();
	void testSelector();
};

#endif /* SRC_CPILOTSSELECTORTEST_H_ */
