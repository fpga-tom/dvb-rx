/*
 * CpilotsSelector.cpp
 *
 *  Created on: Jul 22, 2017
 *      Author: tomas1
 */

#include "CpilotsSelector.h"

#include <algorithm>
#include <iterator>
#include <vector>

CpilotsSelector::CpilotsSelector(const myConfig_t& c) :
		config { c } {

}

CpilotsSelector::~CpilotsSelector() {
}

myBuffer_t CpilotsSelector::update(myBuffer_t& in) {
	auto result = myBuffer_t(config.continual_pilots.size());
	auto i = begin(config.continual_pilots);
	std::generate(begin(result), end(result), [&]() {
		return in[config.zeros_left + *i++];
	});
	return result;
}
