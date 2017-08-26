#include <cmath>
#include "Sync.h"

void sync_tlast(data_t& d_in, data_t& d_out) {
	static int sampleCount = 0;
	sampleCount++;
	d_out = d_in;

	if(sampleCount == SYM_LEN) {
		d_out.tlast = true;
		sampleCount = 0;
	} else {
		d_out.tlast = false;
	}
}

void sync_correlate(data_t& d_in, data_t& d_out) {
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

void sync_align(data_t& d_in, data_t& d_out, int_t& peak) {
	static int_t count = 0;

	d_out.sample = d_in.sample;
	if(count == peak) {
		d_out.tlast = true;
	} else {
		d_out.tlast = false;
	}

	if(d_in.tlast == true) {
		count = 0;
	} else {
		count++;
	}
}

void sync_find_peak(data_t& d_in, int_t& peak, bool& valid) {
	static real_t max = 0;
	static int_t idx = 0;
	static int_t count = 0;


	if(d_in.tlast == true) {
		peak = idx;
		valid = true;
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
		valid=false;
	}
}

void sync_update(stream_t& d_in, stream_t& d_out) {
	data_t data_in, corr_out, data_out;
	int_t peakTmp, peak;
	bool valid;

	d_in.read(data_in);
	sync_correlate(data_in, corr_out);
	sync_find_peak(corr_out, peakTmp, valid);
	if(valid == true) {
		peak = peakTmp;
	}
	sync_align(data_in, data_out, peak);

	d_out.write(data_out);
}
