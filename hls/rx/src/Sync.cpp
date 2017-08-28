#include <cmath>
#include <ap_shift_reg.h>
#include "Sync.h"

void _sync_clk(int_t peak, bool& frame_valid) {
	static int_t sample_count = 0;
	static int_t clk_count = 0;
	sample_count++;

	if(sample_count == SYM_LEN) {
		frame_valid = true;
		if(clk_count++ < 35) {
			if(peak < SYM_LEN / 2) {
				sample_count = -peak >> 2;
			} else {
				sample_count = -(peak - SYM_LEN)  >> 2;
			}
		} else {
			sample_count = 0;
		}
	} else {
		frame_valid = false;
	}
}

void _sync_correlate(sample_t& d_in, acc_t& d_out) {
	static ap_shift_reg<sample_t, FFT_LEN> delay;
	static ap_shift_reg<acc_t, CP_LEN> accDelay;
	static acc_t acc;

	sample_t tmp = delay.shift(d_in);

	// multiply conjugate
	acc_t cj = d_in * std::conj(tmp);
	acc += cj - accDelay.shift(cj);

	d_out = acc;
}


void _sync_find_peak(acc_t& d_in, int_t& peak, bool frame_valid) {
	static real_t max = 0;
	static int_t idx = 0;
	static int_t count = 0;


	if(frame_valid == true) {
		peak = idx;
		max = 0;
		count = 0;
		idx = 0;
	} else {
		real_t r = d_in.real();// >> 4;
		real_t i = d_in.imag();// >> 4;
		real_t a = r*r + i*i;
		if(max < a) {
			max = a;
			idx = count;
		}
		count++;
	}
}

void _sync_update(sample_t& d_in, bool& frame_valid, real_t& freq) {
	acc_t corr_out;
	static int_t peak = 0;

	_sync_correlate(d_in, corr_out);
	_sync_clk(peak, frame_valid);
	_sync_find_peak(corr_out, peak, frame_valid);

}
