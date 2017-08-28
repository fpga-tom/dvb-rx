#include "Sync.h"
#include "ofdm.h"

void rx_update(sample_t& d_in, int_t& ifo, bool& valid) {
	static sample_t d_parallel[SYM_LEN];
	sample_t d_fft[FFT_LEN];
	bool frame_valid = false;
	real_t freq;

	sync_update(d_in, frame_valid, freq);
	ofdm_serial_to_parallel(d_in, d_parallel, frame_valid);


	if(frame_valid == true) {
		ofdm_ifft(d_parallel+CP_LEN, d_fft);
		ofdm_ifo(d_fft, ifo);
		valid = frame_valid;
	}
}
