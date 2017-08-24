#ifndef __SYNC_H__
#define __SYNC_H__

#include <hls_stream.h>

#include "config.h"

class Sync {
	void correlate(stream_t& d_in, stream_t& d_out);
	void align(stream_t& d_in, stream_t& d_out);
public:
	Sync();
	~Sync();
};

#endif /* __SYNC_H__ */
