#ifndef __SYNC_H__
#define __SYNC_H__

#include <hls_stream.h>

#include "config.h"

void _sync_clk(int_t peak, bool& frame_valid);
void _sync_correlate(data_t& d_in, data_t& d_out);
void _sync_find_peak(data_t& d_in, int_t& peak, bool frame_valid);
void _sync_update(data_t& d_in, bool& frame_valid, real_t& freq);

#endif /* __SYNC_H__ */
