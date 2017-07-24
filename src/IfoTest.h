/*
 * IfoTest.h
 *
 *  Created on: Jul 22, 2017
 *      Author: tomas1
 */

#ifndef SRC_IFOTEST_H_
#define SRC_IFOTEST_H_

#include "Ifo.h"

#include "mytypes.h"

class IfoTest: public Ifo {
	const std::string cfile;
	const std::string ofile;
public:
	IfoTest(const myConfig_t&, const std::string&, const std::string&);
	virtual ~IfoTest();
	void testIfo();
};

#endif /* SRC_IFOTEST_H_ */
