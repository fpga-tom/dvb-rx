#include <iostream>
#include <iterator>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <ctime>

#define NUM_STATES 64
#define NUM_INPUT_SYMBOLS 2
#define NUM_OUTPUT_SYMBOLS 4
#define NUM_BLOCKS 1024
#define TRACEBACK 64

__constant__ const char outputs[NUM_STATES][NUM_INPUT_SYMBOLS] = { { 0, 3 }, {
		3, 0 }, { 1, 2 }, { 2, 1 }, { 0, 3 }, { 3, 0 }, { 1, 2 }, { 2, 1 }, { 3,
		0 }, { 0, 3 }, { 2, 1 }, { 1, 2 }, { 3, 0 }, { 0, 3 }, { 2, 1 },
		{ 1, 2 }, { 3, 0 }, { 0, 3 }, { 2, 1 }, { 1, 2 }, { 3, 0 }, { 0, 3 }, {
				2, 1 }, { 1, 2 }, { 0, 3 }, { 3, 0 }, { 1, 2 }, { 2, 1 },
		{ 0, 3 }, { 3, 0 }, { 1, 2 }, { 2, 1 }, { 2, 1 }, { 1, 2 }, { 3, 0 }, {
				0, 3 }, { 2, 1 }, { 1, 2 }, { 3, 0 }, { 0, 3 }, { 1, 2 },
		{ 2, 1 }, { 0, 3 }, { 3, 0 }, { 1, 2 }, { 2, 1 }, { 0, 3 }, { 3, 0 }, {
				1, 2 }, { 2, 1 }, { 0, 3 }, { 3, 0 }, { 1, 2 }, { 2, 1 },
		{ 0, 3 }, { 3, 0 }, { 2, 1 }, { 1, 2 }, { 3, 0 }, { 0, 3 }, { 2, 1 }, {
				1, 2 }, { 3, 0 }, { 0, 3 }, };

typedef struct {
	char w;
} bm_t;

typedef struct {
	char w;
	char prev;
} pm_t;

__global__ void calc_bm(char *encData, char *decData,
		pm_t (*pm)[NUM_BLOCKS][TRACEBACK][NUM_STATES]) {
	int state = threadIdx.x;
	int block = blockIdx.x;
	int offset = block * TRACEBACK;

	for (int tms = 1; tms < TRACEBACK; tms++) {

		int _tms = tms + offset;

		int prevState = (state & (~32)) << 1;
		int prevSymbol = (state >> 5) & 1;

		char data = encData[_tms];
		char d0 = data ^ outputs[prevState][prevSymbol];
		char d1 = data ^ outputs[prevState + 1][prevSymbol];

		char c0 = ((d0 >> 1) & 1) + (d0 & 1);
		char c1 = ((d1 >> 1) & 1) + (d1 & 1);

		char w0 = c0 + (*pm)[block][tms][prevState].w;
		char w1 = c1 + (*pm)[block][tms][prevState + 1].w;

		int idx = ((tms + 1) & (TRACEBACK - 1));

//		(*pm)[block][idx][state].w = w0 < w1 ? w0 : w1;
//		(*pm)[block][idx][state].symbol = prevSymbol;
//		(*pm)[block][idx][state].prev = w0 < w1 ? prevState : prevState + 1;

		(*pm)[block][idx][state] = {
			.w = w0 < w1 ? w0 : w1,
			.prev = w0 < w1 ? char(prevState) : char(prevState + 1)
		};

		__syncthreads();
	}

	if (state == 0) {
		char min = 120;
		int minIdx = 0;
		int tms = 0;
		for (int i = 0; i < NUM_STATES; i++) {
			char w = (*pm)[block][tms][i].w;
			if (min > w) {
				min = w;
				minIdx = i;
			}
		}

		int count = 0;
		int _tms = TRACEBACK - 1;
		while (count < TRACEBACK) {
			decData[_tms] = (minIdx & 32) >> 5;
			minIdx = (*pm)[block][tms][minIdx].prev;
			tms--;
			while (tms < 0) {
				tms += TRACEBACK;
			}
			_tms = tms + offset - 1;
			count++;
		}
	}

}

std::vector<int> traceback(pm_t (*pm)[NUM_BLOCKS][TRACEBACK][NUM_STATES],
		unsigned long tm) {

	int min = 100000;
	int minIdx = -1;

	int tms = tm % TRACEBACK;

	std::vector<int> tb;
	for (int i = 0; i < NUM_STATES; i++) {
		if (min > (*pm)[0][tms][i].w) {
			min = (*pm)[0][tms][i].w;
			minIdx = i;
		}
	}

	int count = 0;

	while (count < TRACEBACK) {
		tb.insert(tb.begin(), /*(*pm)[0][tms][minIdx].symbol*/
		(minIdx & 32) >> 5);

		minIdx = (*pm)[0][tms][minIdx].prev;
		tms--;
		while (tms < 0) {
			tms += TRACEBACK;
		}
		count++;
	}

	return tb;
}

// rng(2)
const char data[128] = { 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1,
		1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1,
		1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0,
		1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1 };

// rng(3)
//const char data[128] = { 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0,
//		1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1,
//		0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1,
//		0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0,
//		1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1,
//		0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0 };

int main(int argc, char **argv) {

	pm_t (*pm)[NUM_BLOCKS][TRACEBACK][NUM_STATES];
	char *encData;
	char *decData;
	cudaMallocManaged(&pm, NUM_BLOCKS * TRACEBACK * NUM_STATES * sizeof(pm_t));
	cudaMallocManaged(&encData, TRACEBACK * NUM_BLOCKS);
	cudaMallocManaged(&decData, TRACEBACK * NUM_BLOCKS);

	for (int i = 0; i < TRACEBACK * NUM_BLOCKS; i++) {
		int d = data[(i % 64) * 2] << 1 | data[(i % 64) * 2 + 1];
		encData[i] = d;
	}

	int count = 1000;
	clock_t begin = clock();

	for (int q = 0; q < count; q++) {
		calc_bm<<<dim3(NUM_BLOCKS), dim3(NUM_STATES)>>>(encData, decData, pm);
		cudaDeviceSynchronize();

	}

	clock_t end = clock();
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < 64; i++)
			std::cout << int(decData[i]) << ", ";
		std::cout << std::endl;
	}

	std::cout << std::endl;
	std::cout << "done "
			<< ((count * TRACEBACK * NUM_BLOCKS * 2)
					/ (double(end - begin) / CLOCKS_PER_SEC)) << " " << " "
			<< (double(end - begin) / CLOCKS_PER_SEC) << std::endl;

	cudaFree(pm);
	cudaFree(encData);
	cudaFree(decData);

	return 0;
}
