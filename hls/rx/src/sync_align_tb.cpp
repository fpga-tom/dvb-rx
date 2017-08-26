#include "Sync.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

void sync_correlate1(data_t& d_in, data_t& d_out) {
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

int sync_align_tb() {
	std::ifstream inFile("/opt/dvb-rx/input/dvb_res5.cfile");
	const std::string ofile = "/opt/dvb-rx/output/align.";
	std::vector<std::complex<float> > buf(SYM_LEN);
	std::vector<std::complex<float> > _out(SYM_LEN);

	int_t peakValid = 0;
	bool sync = false;
	int counter = 0;


	int c = 0;
	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			buf.size() * sizeof(std::complex<float>))) {

		for(int i = 0;i < buf.size(); i++) {

			data_t d_in_scaled,d_tlast, d_out, d_aligned;

			d_in_scaled.sample.real(buf[i].real() / 512);
			d_in_scaled.sample.imag(buf[i].imag() / 512);

			int_t peak;
			bool valid = false;



			sync_tlast(d_in_scaled, d_tlast);
			sync_correlate(d_tlast, d_out);
			sync_find_peak(d_out, peak, valid);
			if(valid == true && c < 5) {
				peakValid = peak;
			}
			sync_align(d_tlast, d_aligned, peakValid);


			if(c < 5 && d_aligned.tlast == true) {
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

				sync_correlate1(d_aligned, d_out);
				std::complex<float> o (d_out.sample.real(), d_out.sample.imag());
				_out.push_back(o);
				counter++;
			}
		}
	}

	inFile.close();

	return 0;
}
