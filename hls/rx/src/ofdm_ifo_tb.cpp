#include "tb.h"
#include "Sync.h"
#include "ofdm.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

int ofdm_ifo_tb() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	std::ifstream inFile("/opt/dvb-rx/input/dvb_res5.cfile");
	const std::string ofile = "/opt/dvb-rx/output/ifo.";
	std::vector<std::complex<float> > buf(SYM_LEN);

	bool frame_valid = false;
	real_t freq;
	int_t ifo = 0xff;

	int c = 0;
	sample_t d_parallel[SYM_LEN], d_fft[FFT_LEN];
	std::ofstream outFile ( ofile + std::to_string(c),	std::ios::binary );

	while (inFile.read(reinterpret_cast<char*>(buf.data()),
			buf.size() * sizeof(std::complex<float>))) {

		for(int i = 0;i < buf.size(); i++) {

			sample_t d_in_scaled, d_out;

			d_in_scaled.real(buf[i].real() / SCALING_FACTOR);
			d_in_scaled.imag(buf[i].imag() / SCALING_FACTOR);

			sync_update(d_in_scaled, frame_valid, freq);
			ofdm_serial_to_parallel(d_in_scaled, d_parallel, frame_valid);


			if(frame_valid == true) {
				ofdm_ifft(d_parallel+CP_LEN, d_fft);
				ofdm_ifo(d_fft, ifo);


				outFile << c++ << "\t" << ifo << std::endl;
			}
		}
	}

	outFile.close();
	inFile.close();

	return 0;
}
