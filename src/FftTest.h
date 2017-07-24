/*
 * FftTest.h
 *
 *  Created on: Jul 22, 2017
 *      Author: tomas1
 */

#ifndef SRC_FFTTEST_H_
#define SRC_FFTTEST_H_

#include "Fft.h"

#include "mytypes.h"

class FftTest {
	const myConfig_t config;
	const std::string cfile;
	const std::string ofile;
public:
	FftTest(const myConfig_t&, const std::string&, const std::string&);
	virtual ~FftTest();
	void testFft();
};

#endif /* SRC_FFTTEST_H_ */
