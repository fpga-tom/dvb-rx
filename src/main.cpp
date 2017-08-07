#include <test/DeinterleaverTest.h>
#include <test/DemapTest.h>
#include <test/ResidualFrequencyTest.h>
#include <test/SamplingFrequencyOffsetTest.h>

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



const std::string cfile = "/opt/dvb/input/dvb_res.cfile";
const std::string ofile = "/opt/dvb/output/";
const std::string demap_input_file = "/opt/dvb/output/rx.0";
const std::string demap_output_file = "/opt/dvb/output/demap.out";
const std::string deint_input_file = "/opt/dvb/output/deint.in";
const std::string deint_output_file = "/opt/dvb/output/deint.out";
const std::string deint_input_file_bit = "/opt/dvb/output/deint.out";
const std::string deint_output_file_bit = "/opt/dvb/output/deint2.out";

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

void testDemap() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	auto demapTest = DemapTest { DVBT_CONFIG_1, demap_input_file, demap_output_file };
	demapTest.testDemap();
}

void testDemapUpdate() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	auto demapTest = DemapTest { DVBT_CONFIG_1, demap_input_file, demap_output_file };
	demapTest.testUpdate();
}

void testDeinterleaverSymbol() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	auto deintTest = DeinterleaverTest { DVBT_CONFIG_1, deint_input_file, deint_output_file };
	deintTest.testSymbol();
}

void testDeinterleaverBit() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	auto deintTest = DeinterleaverTest { DVBT_CONFIG_1, deint_input_file_bit, deint_output_file_bit };
	deintTest.testBit();
}

void testSro() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	auto sroTest = SamplingFrequencyOffsetTest { DVBT_CONFIG_1, cfile, ofile
			+ "sro." };
	sroTest.testSRO();
}

void testRfo() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	auto rfoTest =
			ResidualFrequencyTest { DVBT_CONFIG_1, cfile, ofile + "rfo." };
	rfoTest.testRFO();
}

void rx() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	auto rxTest = Rx { DVBT_CONFIG_1, cfile, ofile + "rx." };
	rxTest.rx();
}

int main(int argc, char **argv) {
//#define  TEST
#ifdef TEST
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
	testDemap();
	testDemapUpdate();
	testDeinterleaverSymbol();
	testDeinterleaverBit();
	testSro();
	testRfo();
#else
	rx();
	std::cout << "Rx done" << std::endl;
#endif
	return 0;
}
