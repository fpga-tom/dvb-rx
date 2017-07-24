#define UNITTEST_SYMBOL
#include <test/DataSelectorTest.h>
#include <config.h>
#include <Rx.h>
#include <test/EqualizerTest.h>
#include <test/FftTest.h>
#include <test/FineTimingOffsetTest.h>
#include <test/IntegerFrequencyOffsetTest.h>
#include <test/NcoTest.h>
#include <test/SyncTest.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>



const std::string cfile = "/opt/dvb/input/dvb_res_small.cfile";
const std::string ofile = "/opt/dvb/output/";

using namespace dvb;

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
	auto ifoTest = IntegerFrequencyOffsetTest { DVBT_CONFIG_1, cfile, ofile + "ifo." };
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
	auto cpilotsTest =
			EqualizerTest { DVBT_CONFIG_1, cfile, ofile
			+ "selector." };
	cpilotsTest.testSelector();
}

void testEqualizer() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	auto equalizerTest = EqualizerTest { DVBT_CONFIG_1, cfile, ofile + "cir." };
	equalizerTest.testEqualizer();
}

void testFto() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	auto ftoTest = FineTimingOffsetTest { DVBT_CONFIG_1, cfile, ofile + "fto." };
	ftoTest.testFto();
}

void testDataSelector() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	auto dataSelectorTest = DataSelectorTest { DVBT_CONFIG_1, cfile, ofile
			+ "ds." };
	dataSelectorTest.testDataSelector();
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
	testEqualizer();
	testFto();
	testDataSelector();

//	rx();
	return 0;
}
