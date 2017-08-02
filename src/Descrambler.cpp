/*
 * Descrambler.cpp
 *
 *  Created on: Aug 1, 2017
 *      Author: tomas1
 */

#include <Descrambler.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cassert>

namespace dvb {

Descrambler::Descrambler(const myConfig_t& c) :
		config { c } {
	pipe(parentRead);
	pipe(parentWrite);
	pid_t childpid = fork();
	if (childpid == -1) {
		perror("fork");
		exit(1);
	}

	if (childpid == 0) {
		close(0);
		dup(parentWrite[0]);
		close(1);
		dup(parentRead[1]);
		execlp("/opt/reedsolomon/Debug/reedsolomon",
				"/opt/reedsolomon/Debug/reedsolomon", nullptr);
		perror("execlp");
		exit(1);
	} else {
		close(parentRead[1]);
		close(parentWrite[0]);
	}
}

Descrambler::~Descrambler() {
	close(parentWrite[1]);
	close(parentRead[0]);
}

myBufferB_t Descrambler::update(const myBufferB_t& buf) {
	assert(buf.size() == 1632);
	if (write(parentWrite[1], buf.data(), buf.size()) != buf.size()) {
		perror("write");
	}


	auto result = myBufferB_t(1504);
	auto count = 0;
	do {
		auto r = read(parentRead[0], result.data() + count,
				result.size() - count);
		if (r <= 0) {
			perror("deinterleaver read");
		}
		count += r;
	} while (count != result.size());

	return result;

}

} /* namespace dvb */
