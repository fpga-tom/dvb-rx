#include "Sync.h"

Sync::Sync() {
}

Sync::~Sync() {
}

void Sync::correlate(stream_t& d_in, stream_t& d_out) {
	static data_t delay[FFT_LEN];
	static int delayHead = 0;
	static data_t accDelay[CP_LEN];
	static int accDelayHead = 0;
	static data_t acc(0.f,0.f);

	// cyclic buffer for delay
	data_t d;
	d_in.read(d);
	data_t tmp = delay[delayHead];
	delay[delayHead] = d;
	delayHead = (delayHead + 1) % FFT_LEN;

	// multiply conjugate
	data_t cj = d * std::conj(tmp);
	acc += cj - accDelay[accDelayHead];

	// cyclic buffer for accumulator
	accDelay[accDelayHead] = cj;
	accDelayHead = (accDelayHead + 1) % CP_LEN;

	d_out.write(acc);
}

void Sync::align(stream_t& d_in, stream_t& d_out) {
}
