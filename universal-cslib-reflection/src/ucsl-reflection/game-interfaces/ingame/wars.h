#pragma once
#include <cstdlib>
#include <ucsl/memory/iallocator.h>
#include <ucsl/containers/arrays/tarray.h>
#include <ucsl/reflection/rflclass.h>

namespace ucsl::reflection::game_interfaces::ingame {
	struct IngameGameInterface {
		inline static memory::IAllocator* get_fallback_allocator() { return (memory::IAllocator*)hh::fnd::MemoryRouter::GetDebugAllocator(); }
		inline static memory::IAllocator* get_fallback_allocator_for_tarray(void* arr) { return get_fallback_allocator(); }

		template<typename T> struct TArray : public containers::arrays::TArray<T, get_fallback_allocator_for_tarray> {};

		using RflSystem = rfl::wars;
		using RflTypeInfoRegistry = typename hh::fnd::RflTypeInfoRegistry;
	};
}
