/*
 * SyncTest.h
 *
 *  Created on: Jul 22, 2017
 *      Author: tomas1
 */

#ifndef SRC_SYNCTEST_H_
#define SRC_SYNCTEST_H_

#include "Sync.h"

namespace dvb {

class SyncTest: public Sync {
	const std::string cfile;
	const std::string ofile;
public:
	SyncTest(const myConfig_t&, const std::string&, const std::string&);
	virtual ~SyncTest();
	void testCorrelation();
	void testAlign();
};

}

#endif /* SRC_SYNCTEST_H_ */
