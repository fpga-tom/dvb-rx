#include "Sync.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char **argv) {
	std::ifstream inFile("/opt/dvb-rx/input/dvb_res5.cfile");
	const std::string ofile = "/opt/dvb-rx/output/corr.";
	std::vector<std::complex<float> > buf(SYM_LEN);
	std::vector<std::complex<float> > _out(SYM_LEN);

	stream_t stream_in("stream_in"), stream_out("stream_out");

	int c = 0;
	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			buf.size() * sizeof(std::complex<float>))) {

		for(int i = 0;i < buf.size(); i++) {

			data_t d;
			d.sample.real(buf[i].real() / CP_LEN * 2);
			d.sample.imag(buf[i].imag() / CP_LEN * 2);
			stream_in.write(d);

			sync_correlate(stream_in, stream_out);

			stream_out.read(d);

			std::complex<float> o (d.sample.real(), d.sample.imag());
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
