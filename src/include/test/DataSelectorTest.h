/*
 * DataSelectorTest.h
 *
 *  Created on: Jul 24, 2017
 *      Author: tomas1
 */

#ifndef SRC_DATASELECTORTEST_H_
#define SRC_DATASELECTORTEST_H_

#include <mytypes.h>
#include <string>

namespace dvb {

class DataSelectorTest {
	const myConfig_t config;
	const std::string cfile;
	const std::string ofile;
public:
	DataSelectorTest(const myConfig_t&, const std::string&, const std::string&);
	virtual ~DataSelectorTest();
	void testDataSelector();
};

} /* namespace dvb */

#endif /* SRC_DATASELECTORTEST_H_ */
