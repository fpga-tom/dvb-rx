/*
 * Rx.h
 *
 *  Created on: Jul 23, 2017
 *      Author: tomas1
 */

#ifndef SRC_RX_H_
#define SRC_RX_H_

#include <mytypes.h>
#include <deque>
#include <iostream>
#include <string>
#include <boost/atomic.hpp>

namespace dvb {

class Rx {
	const myConfig_t config;
	const std::string cfile;
	const std::string ofile;

	std::deque<bool> q0, q1, q2, q3, q4, q5, q6, q7, q8;
	std::tuple<bool, myInteger_t> synchronize(const myBufferB_t&);

public:
	Rx(const myConfig_t&, const std::string&, const std::string&);
	virtual ~Rx();
	void rx();
};

}

#endif /* SRC_RX_H_ */
