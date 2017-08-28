#ifndef __SYNC_H__
#define __SYNC_H__

#include <hls_stream.h>

#include "config.h"

void sync_clk(int_t peak, bool& frame_valid);
void sync_correlate(sample_t& d_in, acc_t& d_out);
void sync_find_peak(acc_t& d_in, int_t& peak, bool frame_valid);
void sync_update(sample_t& d_in, bool& frame_valid, real_t& freq);

#endif /* __SYNC_H__ */
