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
	delay[delayHead] = d_in.sample;
	delayHead = (delayHead + 1) % FFT_LEN;

	// multiply conjugate
	sample_t cj = d_in.sample * std::conj(tmp);
	acc.sample += cj - accDelay[accDelayHead];

	// cyclic buffer for accumulator
	accDelay[accDelayHead] = cj;
	accDelayHead = (accDelayHead + 1) % CP_LEN;

	d_out.sample = acc.sample;
	d_out.tlast = d_in.tlast;
}

int sync_update_tb() {
	std::ifstream inFile("/opt/dvb-rx/input/dvb_res5.cfile");
	const std::string ofile = "/opt/dvb-rx/output/sync_update.";
	std::vector<std::complex<float> > buf(SYM_LEN);
	std::vector<std::complex<float> > _out(SYM_LEN);

	int_t peakValid = 0;
	bool sync = false;
	int counter = 0;

	stream_t stream_in("stream_in"), stream_out("stream_out");

	int c = 0;
	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			buf.size() * sizeof(std::complex<float>))) {

		for(int i = 0;i < buf.size(); i++) {

			data_t d_in_scaled, d_out;

			d_in_scaled.sample.real(buf[i].real() / 512);
			d_in_scaled.sample.imag(buf[i].imag() / 512);

			int_t peak;
			bool valid = false;

			stream_in.write(d_in_scaled);
			sync_update(stream_in, stream_out);
			stream_out.read(d_out);


			if(d_out.tlast == true) {
				sync = true;
				_out.clear();
				counter=0;
			}

			if(sync == true) {
				if(counter == SYM_LEN) {
					std::ofstream outFile ( ofile + std::to_string(c++),	std::ios::binary );
					for(int j = 0; j < buf.size(); j++) {
						outFile << j << "\t" << abs(_out[j]) << std::endl;
					}
					_out.clear();
					outFile.close();
					counter = 0;
				}

				sync_correlate2(d_out, d_out);
				std::complex<float> o (d_out.sample.real(), d_out.sample.imag());
				_out.push_back(o);
				counter++;
			}
		}
	}

	inFile.close();

	return 0;
}