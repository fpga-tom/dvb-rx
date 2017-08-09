/*
 * mytypes.h
 *
 *  Created on: Jul 18, 2017
 *      Author: tomas1
 */

#ifndef MYTYPES_H_
#define MYTYPES_H_

#include <stddef.h>
#include <bitset>
#include <complex>
#include <deque>
#include <vector>

const int bitCount  = 36288;

typedef float myReal_t;
typedef int myInteger_t;
typedef std::complex<myReal_t> myComplex_t;
typedef std::vector<myComplex_t> myBuffer_t;
typedef std::vector<myReal_t> myBufferR_t;
typedef std::vector<unsigned char> myBufferB_t;
typedef std::vector<myInteger_t> myBufferI_t;
typedef std::deque<myComplex_t> myDelay_t;
typedef std::bitset<bitCount> myBitset_t;

typedef struct {
	myInteger_t fft_len;
	myInteger_t cp_len;
	myInteger_t sym_len;
	myInteger_t zeros_left;
	myInteger_t zeros_right;
	myInteger_t carriers;
	myInteger_t carrier_center;
	myInteger_t data_carrier_count;
	myReal_t sample_rate;
	std::vector<myInteger_t> continual_pilots;
	size_t continual_pilots_count;
	std::vector<myReal_t> continual_pilots_value;
	std::vector<myInteger_t> tps;
	std::vector<std::vector<myInteger_t>> scattered_pilots;
	std::vector<std::vector<myReal_t>> scattered_pilots_value;
	size_t scattered_pilots_count;
	std::vector<myInteger_t> H;
	std::vector<myInteger_t> bit_table;
	std::vector<std::vector<myInteger_t>> nextStates;
	std::vector<std::vector<myInteger_t>> outputs;
	size_t statesCount;
} myConfig_t;

#ifndef UNITTEST_SYMBOL
#define FRIEND_TEST(f) friend class f
#else
#define FRIEND_TEST(f)
#endif

#endif /* MYTYPES_H_ */
