#pragma once
#include <new>

namespace ucsl::memory
{
	class IAllocator
	{
	public:
		virtual ~IAllocator() = default;

		virtual void* Alloc(size_t size, size_t alignment) = 0;
		virtual void Free(void* ptr) = 0;
	};
}

inline void* operator new(size_t count, ucsl::memory::IAllocator* allocator) {
	return allocator->Alloc(count, 8);
}

inline void* operator new(size_t count, std::align_val_t alignment, ucsl::memory::IAllocator* allocator) {
	return allocator->Alloc(count, static_cast<size_t>(alignment));
}

inline void* operator new[](size_t count, ucsl::memory::IAllocator* allocator) {
	return operator new(count, allocator);
}

inline void* operator new[](size_t count, std::align_val_t alignment, ucsl::memory::IAllocator* allocator) {
	return operator new(count, alignment, allocator);
}

inline void operator delete(void* ptr, ucsl::memory::IAllocator* allocator) noexcept {
	allocator->Free(ptr);
}

inline void operator delete(void* ptr, std::align_val_t alignment, ucsl::memory::IAllocator* allocator) noexcept {
	allocator->Free(ptr);
}

inline void operator delete[](void* ptr, ucsl::memory::IAllocator* allocator) noexcept {
	operator delete(ptr, allocator);
}

inline void operator delete[](void* ptr, std::align_val_t alignment, ucsl::memory::IAllocator* allocator) noexcept {
	operator delete(ptr, alignment, allocator);
}
