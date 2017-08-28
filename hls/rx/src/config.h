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
#define NUM_CONTINUAL_PILOTS 177

#if 0
typedef ap_int<15> int_t;
typedef ap_fixed<18,15> real_t;
typedef std::complex<ap_fixed<12,1> > sample_t;
typedef std::complex<ap_fixed<12,6> > acc_t;

#else

typedef int int_t;
typedef float real_t;
typedef std::complex<float > sample_t;
typedef std::complex<float > acc_t;

#endif

typedef hls::stream<sample_t> stream_t;

extern const int_t continual_pilots_indices[NUM_CONTINUAL_PILOTS];

#endif /* __CONFIG_H__ */
