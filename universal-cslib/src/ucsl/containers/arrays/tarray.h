#pragma once
#include "array-base.h"

namespace ucsl::containers::arrays {
	namespace internal {
		template<typename AllocatorSystem, typename A>
		struct TArrayArrayAllocatorSystem {
			using allocator_type = typename AllocatorSystem::allocator_type;

			inline static allocator_type* get_allocator(const A& arr) { return AllocatorSystem::get_allocator(); }
		};
	}

	template<typename T, typename AllocatorSystem>
	class TArray : public ArrayBase<T, size_t, internal::TArrayArrayAllocatorSystem<AllocatorSystem, TArray<T, AllocatorSystem>>> {
	public:
		using ArrayBase<T, size_t, internal::TArrayArrayAllocatorSystem<AllocatorSystem, TArray<T, AllocatorSystem>>>::ArrayBase;
	};
}
