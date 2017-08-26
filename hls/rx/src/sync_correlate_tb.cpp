#include "Sync.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

int sync_correlate_tb() {
	std::ifstream inFile("/opt/dvb-rx/input/dvb_res5.cfile");
	const std::string ofile = "/opt/dvb-rx/output/corr.";
	std::vector<std::complex<float> > buf(SYM_LEN);
	std::vector<std::complex<float> > _out(SYM_LEN);

	int c = 0;
	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			buf.size() * sizeof(std::complex<float>))) {

		for(int i = 0;i < buf.size(); i++) {

			data_t d_in, d_out;
			d_in.sample.real(buf[i].real() / 512);
			d_in.sample.imag(buf[i].imag() / 512);

			_sync_correlate(d_in, d_out);

			std::complex<float> o (d_out.sample.real(), d_out.sample.imag());
			_out.push_back(o);
		}

		std::ofstream outFile ( ofile + std::to_string(c++),	std::ios::binary );
		for(int i = 0; i < buf.size(); i++) {
			outFile << i << "\t" << abs(_out[i]) << std::endl;
		}
		_out.clear();
		outFile.close();
	}

	inFile.close();

	return 0;
}
