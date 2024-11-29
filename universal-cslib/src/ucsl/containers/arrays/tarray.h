#pragma once
#include "array-base.h"

namespace ucsl::containers::arrays {
	template<typename T, typename AllocatorSystem>
	class TArray;

	template<typename T, typename AllocatorSystem>
	struct TArrayArrayAllocatorSystem {
		using allocator_type = typename AllocatorSystem::allocator_type;

		inline static allocator_type* get_allocator(const ArrayBase<T, size_t, TArrayArrayAllocatorSystem<T, AllocatorSystem>>& arr) { return AllocatorSystem::get_allocator(); }
	};

	template<typename T, typename AllocatorSystem>
	class TArray : public ArrayBase<T, size_t, TArrayArrayAllocatorSystem<T, AllocatorSystem>> {
	public:
		using ArrayBase<T, size_t, TArrayArrayAllocatorSystem<T, AllocatorSystem>>::ArrayBase;
	};
}
