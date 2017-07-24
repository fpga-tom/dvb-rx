/*
 * Rx.h
 *
 *  Created on: Jul 23, 2017
 *      Author: tomas1
 */

#ifndef SRC_RX_H_
#define SRC_RX_H_

#include <string>

#include "mytypes.h"

namespace dvb {

class Rx {
	const myConfig_t config;
	const std::string cfile;
	const std::string ofile;
public:
	Rx(const myConfig_t&, const std::string&, const std::string&);
	virtual ~Rx();
	void rx();
};

}

#endif /* SRC_RX_H_ */
