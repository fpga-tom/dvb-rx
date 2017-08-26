#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

#include "Sync.h"

int sync_find_peak_tb() {
	std::ifstream inFile("/opt/dvb-rx/input/dvb_res5.cfile");
	const std::string ofile = "/opt/dvb-rx/output/peak.";
	std::vector<std::complex<float> > buf(SYM_LEN);

	int c = 0;
	std::ofstream outFile ( ofile + std::to_string(c++),	std::ios::binary );
	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			buf.size() * sizeof(std::complex<float>))) {


		for(int i = 0;i < buf.size(); i++) {

			data_t d_in, d_tlast, d_out;
			d_in.sample.real(buf[i].real() / 512);
			d_in.sample.imag(buf[i].imag() / 512);

			int_t peak;
			bool valid;

			sync_tlast(d_in, d_tlast);
			sync_correlate(d_tlast, d_out);
			sync_find_peak(d_out, peak, valid);

			if(valid == true) {
				outFile << c++ << "\t" << peak << std::endl;
			}
		}
	}

	outFile.close();
	inFile.close();

	return 0;
}

