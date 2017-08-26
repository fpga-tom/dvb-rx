#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <complex>
#include <hls_stream.h>
#include <ap_int.h>
#include <ap_fixed.h>

#define FFT_LEN 8192
#define SYM_LEN 10240
#define CP_LEN 2048
#define ZEROS_LEFT 688
#define ZEROS_RIGHT 687
#define CARRIERS 6817

typedef ap_uint<14> int_t;
typedef ap_fixed<18,15> real_t;
typedef std::complex<ap_int<12> > sample_t;


typedef struct {
	sample_t sample;
	bool tlast;
} data_t;

typedef hls::stream<data_t> stream_t;

#endif /* __CONFIG_H__ */
