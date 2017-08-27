#include "ofdm.h"

void _ofdm_serial_to_parallel(sample_t& d_in, sample_t d_out[SYM_LEN], bool frame_valid) {
	static int_t count =0;

	d_out[count++] = d_in;

	if(frame_valid == true) {
			count = 0;
	}
}

void _ofdm_ifft(sample_t d_in[FFT_LEN], sample_t d_out[FFT_LEN]) {
#pragma HLS DATA_PACK variable=d_out
#pragma HLS DATA_PACK variable=d_in
#pragma HLS DATAFLOW
#pragma HLS INTERFACE ap_fifo depth=8192 port=d_out
#pragma HLS INTERFACE ap_fifo depth=8192 port=d_in
	config_t fft_config1;
#pragma HLS DATA_PACK variable=fft_config1
	status_t fft_status1;

	fft_config1.setDir(0);
	fft_config1.setSch(0x2AB);

	fft_data_t fft_in[FFT_LEN], fft_out[FFT_LEN];

	for(int_t i=0; i < FFT_LEN; i++) {
		fft_in[i] = d_in[i];
	}

	hls::fft<param1>(fft_in, fft_out, &fft_status1, &fft_config1);

	for(int_t i=0; i < FFT_LEN; i++) {
		if(i < FFT_LEN / 2 ) {
			d_out[i] = fft_out[i + FFT_LEN / 2];
		} else {
			d_out[i] = fft_out[i - FFT_LEN / 2];
		}
	}
}
