#ifndef __SERIAL_TO_PARALLEL_H__
#define __SERIAL_TO_PARALLEL_H__

#include "config.h"

void serial_to_parallel(data_t& d_in, data_t d_out[SYM_LEN], bool frame_valid);

#endif /* __SERIAL_TO_PARALLEL_H__ */
