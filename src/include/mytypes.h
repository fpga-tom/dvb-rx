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
	size_t fft_len;
	size_t cp_len;
	size_t sym_len;
	size_t zeros_left;
	size_t zeros_right;
	size_t carriers;
	size_t carrier_center;
	size_t data_carrier_count;
	myReal_t sample_rate;
	std::vector<int> continual_pilots;
	size_t continual_pilots_count;
	std::vector<myReal_t> continual_pilots_value;
	std::vector<int> tps;
	std::vector<std::vector<int>> scattered_pilots;
	std::vector<std::vector<myReal_t>> scattered_pilots_value;
	size_t scattered_pilots_count;
	std::vector<int> H;
	std::vector<int> bit_table;
	std::vector<std::vector<int>> nextStates;
	std::vector<std::vector<int>> outputs;
	size_t statesCount;
} myConfig_t;

#ifndef UNITTEST_SYMBOL
#define FRIEND_TEST(f) friend class f
#else
#define FRIEND_TEST(f)
#endif

#endif /* MYTYPES_H_ */
