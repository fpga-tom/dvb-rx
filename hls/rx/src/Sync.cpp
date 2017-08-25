#include <cmath>
#include "Sync.h"


void sync_correlate(stream_t& d_in, stream_t& d_out) {
	static sample_t delay[FFT_LEN];
	static int delayHead = 0;
	static sample_t accDelay[CP_LEN];
	static int accDelayHead = 0;
	static data_t acc;

	// cyclic buffer for delay
	data_t d;
	d_in.read(d);
	sample_t tmp = delay[delayHead];
	delay[delayHead] = d.sample;
	delayHead = (delayHead + 1) % FFT_LEN;

	// multiply conjugate
	sample_t cj = d.sample * std::conj(tmp);
	acc.sample += cj - accDelay[accDelayHead];

	// cyclic buffer for accumulator
	accDelay[accDelayHead] = cj;
	accDelayHead = (accDelayHead + 1) % CP_LEN;

	static int sampleCount = 0;
	sampleCount++;
	if(sampleCount == SYM_LEN) {
		acc.tlast = true;
		sampleCount = 0;
	} else {
		acc.tlast = false;
	}

	d_out.write(acc);
}

void sync_align(stream_t& d_in, stream_t& d_out, int_t& peak) {
	static int_t count = 0;
	data_t d;
	d_in.read(d);

	if(d.tlast == true) {
		count = 0;
	} else {
		if(count == peak) {
			d.tlast = true;
		} else {
			d.tlast = false;
		}
		count++;
	}
	d_out.write(d);
}

void sync_findPeak(stream_t& d_in, int_t& peak, bool& valid) {
	static real_t max = 0;
	static int_t idx = 0;
	static int_t count = 0;

	data_t d;
	d_in.read(d);
	if(d.tlast == true) {
		peak = idx;
		valid = true;
		max = 0;
		count = 0;
		idx = 0;
	} else {
		real_t a = real(d.sample)*real(d.sample) + imag(d.sample)*imag(d.sample);
		if(max < a) {
			max = a;
			idx = count;
		}
		count++;
		valid=false;
	}
}

void sync_update(stream_t& d_in, stream_t& d_out) {
	static stream_t c_out;
	int_t peakTmp, peak;
	bool valid;

	sync_correlate(d_in, c_out);
	sync_findPeak(c_out, peakTmp, valid);
	if(valid == true) {
		peak = peakTmp;
	}
	sync_align(d_in, d_out, peak);
}
