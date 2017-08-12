/*
 * ViterbiDecoderSSE.cpp
 *
 *  Created on: Aug 10, 2017
 *      Author: tomas1
 */

#include <emmintrin.h>
#include <ViterbiDecoderSSE.h>
#include <algorithm>
#include <bitset>
#include <cmath>
#include <iostream>
#include <iterator>
#include <vector>

#include "d_viterbi.h"

namespace dvb {


static __m128i metric0[4] __attribute__ ((aligned(16)));
static __m128i metric1[4] __attribute__ ((aligned(16)));
static __m128i path0[4] __attribute__ ((aligned(16)));
static __m128i path1[4] __attribute__ ((aligned(16)));

ViterbiDecoderSSE::ViterbiDecoderSSE(const myConfig_t& c, const myInteger_t tb) :
		config { c }, d_ntraceback { tb }, d_init { 0 } {

	int amp = 100;
	float RATE = 0.5;
	float ebn0 = 12.0;
	float esn0 = RATE * pow(10.0, ebn0 / 10);
	d_gen_met(mettab, amp, esn0, 0.0, 4);

	d_viterbi_chunks_init_sse2(metric0, path0);


}

ViterbiDecoderSSE::~ViterbiDecoderSSE() {
	// TODO Auto-generated destructor stub
}

myBufferB_t ViterbiDecoderSSE::update(const myBitset_t& bitset,
		myInteger_t frame) {
	if (frame == 0) {
		d_init = 0;
		d_viterbi_chunks_init_sse2(metric0, path0);
	}
	auto noutput_items = 3024;
	auto out_count = 0;
	auto d_m = 6;
	const unsigned char d_puncture[4] = { 1, 1, 0, 1 };
	auto d_k = 2;
	auto d_n = 3;

	auto d_bsize = 12096;

	auto d_nsymbols = d_bsize * d_n / d_m;
	// Number of bits after depuncturing a block (before decoding)
	auto d_nbits = 2 * d_k * d_bsize;
	// Number of output bytes after decoding
	auto d_nout = d_nbits / 2 / 8;
	auto out = myBufferB_t(d_nout);

	auto d_inbits = std::vector<unsigned char>(d_nbits);

	int nblocks = 8 * noutput_items / (d_bsize * d_k);

	auto in = myBufferB_t(bitset.size() / d_m);
	std::fill(begin(in), end(in), 0);

	for (auto i { 0 }; i < bitset.size(); i++) {
		in[i / d_m] |= bitset[i] << (d_m - 1 - (i % d_m));
	}


	for (int n = 0; n < nblocks; n++) {
		/*
		 * Depuncture and unpack a block.
		 * We receive the symbol (d_m bits/byte) in one byte (e.g. for QAM16 00001111).
		 * Create a buffer of bytes containing just one bit/byte.
		 * Also depuncture according to the puncture vector.
		 */
		for (int count = 0, i = 0; i < d_nsymbols; i++) {
			for (int j = (d_m - 1); j >= 0; j--) {
				// Depuncture
				while (d_puncture[count % (2 * d_k)] == 0)
					d_inbits[count++] = 2;

				// Insert received bits
				d_inbits[count++] = (in[(n * d_nsymbols) + i] >> j) & 1;

				// Depuncture
				while (d_puncture[count % (2 * d_k)] == 0)
					d_inbits[count++] = 2;
			}
		}

		/*
		 * Decode a block.
		 */
		for (int in_count = 0; in_count < d_nbits; in_count++) {
			if ((in_count % 4) == 0) //0 or 3
					{
				d_viterbi_butterfly2_sse2(&d_inbits[in_count & 0xfffffffc],
						metric0, metric1, path0, path1);
				//d_viterbi_butterfly2(&d_inbits[in_count & 0xfffffffc], mettab, state0, state1);

				if ((in_count > 0) && (in_count % 16) == 8) // 8 or 11
						{
					unsigned char c;

					d_viterbi_get_output_sse2(metric0, path0, d_ntraceback, &c);
					//d_viterbi_get_output(state0, &c);

					if (d_init == 0) {
						if (out_count >= d_ntraceback) {
							out[out_count] = c;
//							printf("out_init[%i]: %x\n",
//									out_count - d_ntraceback,
//									out[out_count - d_ntraceback]);
						} else {
							out[out_count] = 0;
						}
					} else {
						out[out_count] = c;
						//printf("out[%i]: %x\n", out_count, out[out_count]);
					}

					out_count++;
				}
			}
		}
	}

	if (d_init == 0) {
		d_init = 1;
	}

	return out;

}

} /* namespace dvb */
