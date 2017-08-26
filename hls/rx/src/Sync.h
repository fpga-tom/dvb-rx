#ifndef __SYNC_H__
#define __SYNC_H__

#include <hls_stream.h>

#include "config.h"

void sync_tlast(data_t& d_in, data_t& d_out);
void sync_correlate(data_t& d_in, data_t& d_out);
void sync_find_peak(data_t& d_in, int_t& peak, bool& valid);
void sync_align(data_t& d_in, data_t& d_out, int_t& peak);
void sync_update(stream_t& d_in, stream_t& dout);

#endif /* __SYNC_H__ */
