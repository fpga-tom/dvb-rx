#include "serial_to_parallel.h"

void serial_to_parallel(data_t& d_in, data_t d_out[SYM_LEN], bool frame_valid) {
	static int_t count =0;

	d_out[count++] = d_in;

	if(frame_valid == true) {
			count = 0;
	}
}
