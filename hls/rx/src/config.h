#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <complex>
#include <hls_stream.h>
#include <ap_int.h>

#define FFT_LEN 8192
#define SYM_LEN 10240
#define CP_LEN 2048
#define ZEROS_LEFT 688
#define ZEROS_RIGHT 687
#define CARRIERS 6817


typedef std::complex<ap_int<14> > data_t;
typedef hls::stream<data_t> stream_t;

#endif /* __CONFIG_H__ */
