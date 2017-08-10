/*
 * ViterbiDecoder.cpp
 *
 *  Created on: Jul 30, 2017
 *      Author: tomas1
 */

#include <unistd.h>
#include <ViterbiDecoder.h>
#include <algorithm>
#include <bitset>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <vector>

namespace dvb {

ViterbiDecoder::ViterbiDecoder(const myConfig_t&c) :
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
		execlp("/opt/viterbi/Release/viterbi", "/opt/viterbi/Release/viterbi",
				nullptr);
		perror("execlp");
		exit(1);
	} else {
		close(parentRead[1]);
		close(parentWrite[0]);
	}

	}

ViterbiDecoder::~ViterbiDecoder() {
	close(parentWrite[1]);
	close(parentRead[0]);
}


myBufferB_t ViterbiDecoder::update(myBitset_t& bitset) {
	auto buf = std::vector<char>(4536);
	std::fill(begin(buf), end(buf), 0);
	for (auto i { 0 }; i < bitCount; i++) {
		buf[i / 8] |= bitset[i] << (i % 8);
	}
	if (write(parentWrite[1], buf.data(), buf.size()) != buf.size()) {
		perror("write");
	}

	auto result = myBufferB_t(3024);
	auto count = 0;
	do {
		count += read(parentRead[0], result.data() + count,
				result.size() - count);
	} while (count != result.size());

	return result;
}

} /* namespace dvb */
