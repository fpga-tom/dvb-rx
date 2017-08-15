/*
 * MyAllocator.h
 *
 *  Created on: Aug 15, 2017
 *      Author: eclipse
 */

#ifndef SRC_MYALLOCATOR_H_
#define SRC_MYALLOCATOR_H_

#include <stddef.h>
#include <volk/volk.h>
#include <volk/volk_malloc.h>
#include <cassert>
#include <iostream>
#include <memory>

namespace dvb {

template<class _Tp>
class MyAllocator: public std::allocator<_Tp> {
public:
	typedef size_t size_type;
	typedef _Tp* pointer;
	typedef const _Tp* const_pointer;

	template<typename _Tp1>
	struct rebind {
		typedef MyAllocator<_Tp1> other;
	};
	pointer allocate(size_type n, const void *hint = 0) {
		auto alignment = volk_get_alignment();
		auto result = (_Tp*) volk_malloc(n * sizeof(_Tp), alignment);
		assert(result != nullptr);
		return result;
	}

	void deallocate(pointer p, size_type n) {
		volk_free(p);
	}
	~MyAllocator() throw() { }
	MyAllocator() throw () :
			std::allocator<_Tp>() {
	}
	MyAllocator(const MyAllocator &a) throw () :
			std::allocator<_Tp>(a) {
	}
	template<class U>
	MyAllocator(const MyAllocator<U> &a) throw () :
			std::allocator<_Tp>(a) {
	}
};

} /* namespace dvb */

#endif /* SRC_MYALLOCATOR_H_ */
