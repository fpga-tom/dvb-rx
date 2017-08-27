#include <cmath>
#include <ap_shift_reg.h>
#include "Sync.h"

void _sync_clk(int_t peak, bool& frame_valid) {
	static int_t sample_count = 0;
	sample_count++;

	if(sample_count == SYM_LEN) {
		frame_valid = true;
		if(peak < SYM_LEN / 2) {
			sample_count = -peak >> 2;
		} else {
			sample_count = -(peak - SYM_LEN)  >> 2;
		}
	} else {
		frame_valid = false;
	}
}

void _sync_correlate(data_t& d_in, data_t& d_out) {
	static ap_shift_reg<sample_t, FFT_LEN> delay;
	static ap_shift_reg<sample_t, CP_LEN> accDelay;
	static data_t acc;

	sample_t tmp = delay.shift(d_in.sample);

	// multiply conjugate
	sample_t cj = d_in.sample * std::conj(tmp);
	acc.sample += cj - accDelay.shift(cj);

	d_out.sample = acc.sample;
}


void _sync_find_peak(data_t& d_in, int_t& peak, bool frame_valid) {
	static real_t max = 0;
	static int_t idx = 0;
	static int_t count = 0;


	if(frame_valid == true) {
		peak = idx;
		max = 0;
		count = 0;
		idx = 0;
	} else {
		real_t r = real(d_in.sample) >> 4;
		real_t i = imag(d_in.sample) >> 4;
		real_t a = r*r + i*i;
		if(max < a) {
			max = a;
			idx = count;
		}
		count++;
	}
}

void _sync_update(data_t& d_in, bool& frame_valid, real_t& freq) {
	data_t corr_out;
	static int_t peak = 0;

	_sync_correlate(d_in, corr_out);
	_sync_clk(peak, frame_valid);
	_sync_find_peak(corr_out, peak, frame_valid);

}
