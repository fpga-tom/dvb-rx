/*
 * CpilotsSelector.h
 *
 *  Created on: Jul 22, 2017
 *      Author: tomas1
 */

#ifndef SRC_CPILOTSSELECTOR_H_
#define SRC_CPILOTSSELECTOR_H_

#include "mytypes.h"

class CpilotsSelector {
	FRIEND_TEST(CpilotsSelectorTest);
	const myConfig_t config;
public:
	CpilotsSelector(const myConfig_t&);
	virtual ~CpilotsSelector();
	myBuffer_t update(myBuffer_t&);
};

#endif /* SRC_CPILOTSSELECTOR_H_ */
