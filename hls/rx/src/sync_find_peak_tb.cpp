#include "tb.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

#include "Sync.h"

int sync_find_peak_tb() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	std::ifstream inFile("/opt/dvb-rx/input/dvb_res5.cfile");
	const std::string ofile = "/opt/dvb-rx/output/peak.";
	std::vector<std::complex<float> > buf(SYM_LEN);

	bool frame_valid = false;
	real_t freq;
	int c = 0;
	int_t peak = 0;

	std::ofstream outFile ( ofile + std::to_string(c++),	std::ios::binary );
	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			buf.size() * sizeof(std::complex<float>))) {


		for(int i = 0;i < buf.size(); i++) {

			sample_t d_in;
			acc_t corr_out;
			d_in.real(buf[i].real() / SCALING_FACTOR);
			d_in.imag(buf[i].imag() / SCALING_FACTOR);


			sync_correlate(d_in, corr_out);
			sync_clk(peak, frame_valid);
			sync_find_peak(corr_out, peak, frame_valid);

			if(frame_valid == true) {
				outFile << c++ << "\t" << peak << std::endl;
			}
		}
	}

	outFile.close();
	inFile.close();

	return 0;
}

