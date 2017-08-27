#ifndef __OFDM_H__
#define __OFDM_H__

#include <hls_fft.h>
#include "config.h"

void _ofdm_serial_to_parallel(sample_t& d_in, sample_t d_out[SYM_LEN], bool frame_valid);

const char FFT_CONFIG_WIDTH = 16;
const char FFT_STATUS_WIDTH = 8;

struct param1 : hls::ip_fft::params_t {
	static const unsigned ordering_opt = hls::ip_fft::natural_order;
	static const unsigned config_width = FFT_CONFIG_WIDTH;
	static const unsigned status_width = FFT_STATUS_WIDTH;
	static const unsigned max_nfft = 13;
	static const unsigned input_width = 16;
	static const unsigned output_width = 16;
	static const unsigned arch_opt = hls::ip_fft::pipelined_streaming_io;
};

typedef hls::ip_fft::config_t<param1> config_t;
typedef hls::ip_fft::status_t<param1> status_t;

typedef std::complex<ap_fixed<16,1> > fft_data_t;

void _ofdm_ifft(sample_t d_in[FFT_LEN], sample_t d_out[FFT_LEN]);

#endif /* __OFDM_H__ */
