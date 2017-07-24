#include "CpilotsSelectorTest.h"
#include "EqualizerTest.h"
#include "FtoTest.h"
#include "Rx.h"

/*
 * SyncTest.cpp
 *
 *  Created on: Jul 19, 2017
 *      Author: tomas1
 */
#define UNITTEST_SYMBOL

#include "FftTest.h"
#include "IfoTest.h"
#include "NcoTest.h"
#include "SyncTest.h"


#include "Sync.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "config.h"
#include "Fft.h"
#include "Ifo.h"
#include "mytypes.h"
#include "Nco.h"

const std::string cfile = "/opt/dvb/input/dvb_res_small2.cfile";
const std::string ofile = "/opt/dvb/output/";

//
//std::string exec(const char* cmd) {
//	std::array<char, 128> buffer;
//	std::string result;
//	std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
//	if (!pipe)
//		throw std::runtime_error("popen() failed!");
//	while (!feof(pipe.get())) {
//		if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
//			result += buffer.data();
//	}
//	return result;
//}
//


void testCorrelation() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	auto syncTest = SyncTest { DVBT_CONFIG_1, cfile, ofile + "corr." };
	syncTest.testCorrelation();
}

void testAlign() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	auto syncTest = SyncTest { DVBT_CONFIG_1, cfile, ofile + "align." };
	syncTest.testAlign();
}

void testFft() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	auto fftTest = FftTest { DVBT_CONFIG_1, cfile, ofile + "fft." };
	fftTest.testFft();
}

void testIfo() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	auto ifoTest = IfoTest { DVBT_CONFIG_1, cfile, ofile + "ifo." };
	ifoTest.testIfo();
}

void testNco() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	auto ncoTest = NcoTest { DVBT_CONFIG_1, cfile, ofile + "nco." };
	ncoTest.testNco();
}

void testNcoFractional() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	auto ncoTest = NcoTest { DVBT_CONFIG_1, cfile, ofile + "fractional." };
	ncoTest.testNcoFractional();
}

void testCpilots() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	auto cpilotsTest = CpilotsSelectorTest { DVBT_CONFIG_1, cfile, ofile
			+ "selector." };
	cpilotsTest.testSelector();
}

void testCir() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	auto cirTest = EqualizerTest { DVBT_CONFIG_1, cfile, ofile + "cir." };
	cirTest.testCir();
}

void testFto() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	auto ftoTest = FtoTest { DVBT_CONFIG_1, cfile, ofile + "fto." };
	ftoTest.testFto();
}

void rx() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	auto rxTest = Rx { DVBT_CONFIG_1, cfile, ofile + "rx." };
	rxTest.rx();
}

int main(int argc, char **argv) {
	testCorrelation();
	testAlign();
	testFft();
	testIfo();
	testNco();
	testNcoFractional();
	testCpilots();
	testCir();
	testFto();

//	rx();
	return 0;
}
