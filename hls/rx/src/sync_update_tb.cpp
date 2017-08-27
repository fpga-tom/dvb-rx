#include "Sync.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

void sync_correlate2(data_t& d_in, data_t& d_out) {
	static sample_t delay[FFT_LEN];
	static int delayHead = 0;
	static sample_t accDelay[CP_LEN];
	static int accDelayHead = 0;
	static data_t acc;

	// cyclic buffer for delay
	sample_t tmp = delay[delayHead];
	delay[delayHead] = d_in;
	delayHead = (delayHead + 1) % FFT_LEN;

	// multiply conjugate
	sample_t cj = d_in * std::conj(tmp);
	acc += cj - accDelay[accDelayHead];

	// cyclic buffer for accumulator
	accDelay[accDelayHead] = cj;
	accDelayHead = (accDelayHead + 1) % CP_LEN;

	d_out = acc;
}

int sync_update_tb() {
	std::ifstream inFile("/opt/dvb-rx/input/dvb_res5.cfile");
	const std::string ofile = "/opt/dvb-rx/output/align.";
	std::vector<std::complex<float> > buf(SYM_LEN);
	std::vector<std::complex<float> > _out(SYM_LEN);

	bool frame_valid = false;
	real_t freq;

	int c = 0;
	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			buf.size() * sizeof(std::complex<float>))) {

		for(int i = 0;i < buf.size(); i++) {

			data_t d_in_scaled, d_out;

			d_in_scaled.real(buf[i].real() / 512);
			d_in_scaled.imag(buf[i].imag() / 512);

			_sync_update(d_in_scaled, frame_valid, freq);
			sync_correlate2(d_in_scaled, d_out);

			std::complex<float> o (d_out.real(), d_out.imag());
			_out.push_back(o);

			if(frame_valid == true) {
				std::ofstream outFile ( ofile + std::to_string(c++),	std::ios::binary );
				for(int j = 0; j < buf.size(); j++) {
					outFile << j << "\t" << abs(_out[j]) << std::endl;
				}
				_out.clear();
				outFile.close();
			}
		}
	}

	inFile.close();

	return 0;
}
