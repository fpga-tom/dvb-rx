#include "tb.h"
#include "Sync.h"
#include "ofdm.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

int ofdm_ifft_tb() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	std::ifstream inFile("/opt/dvb-rx/input/dvb_res5.cfile");
	const std::string ofile = "/opt/dvb-rx/output/fft.";
	std::vector<std::complex<float> > buf(SYM_LEN);
	std::vector<std::complex<float> > _out(FFT_LEN);

	bool frame_valid = false;
	real_t freq;

	int c = 0;
	sample_t d_parallel[SYM_LEN], d_fft[FFT_LEN];
	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			buf.size() * sizeof(std::complex<float>))) {

		float max = 0;
		for(int i = 0;i < buf.size(); i++) {

			if(max < buf[i].real()) {
				max = buf[i].real();
			}
			sample_t d_in_scaled, d_out;

			d_in_scaled.real(buf[i].real() / SCALING_FACTOR);
			d_in_scaled.imag(buf[i].imag() / SCALING_FACTOR);

			_sync_update(d_in_scaled, frame_valid, freq);
			_ofdm_serial_to_parallel(d_in_scaled, d_parallel, frame_valid);

			if(frame_valid == true) {
				_ofdm_ifft(d_parallel, d_fft);
				_out.clear();
				for(int j=0; j < FFT_LEN; j++) {
					std::complex<float> o (d_fft[j].real(), d_fft[j].imag());
					_out.push_back(o);
				}

				std::ofstream outFile ( ofile + std::to_string(c++),	std::ios::binary );
				for(int j = 0; j < _out.size(); j++) {
					outFile << j << "\t" << abs(_out[j]) << std::endl;
				}
				outFile.close();
			}
		}
//		std::cout << max << std::endl;
	}

	inFile.close();

	return 0;
}
