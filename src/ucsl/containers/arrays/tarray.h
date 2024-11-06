#pragma once
#include "array-base.h"

namespace ucsl::containers::arrays {
	template<typename T, AllocatorGetterFn* get_allocator>
	class TArray : public ArrayBase<T, size_t, get_allocator>
	{
	public:
		using ArrayBase<T, size_t, get_allocator>::ArrayBase;
	};
}
