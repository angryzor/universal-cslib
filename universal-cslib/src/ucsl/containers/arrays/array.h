#pragma once
#include "array-base.h"

namespace ucsl::containers::arrays {
	inline memory::IAllocator* get_array_allocator(void* arr) { return *reinterpret_cast<memory::IAllocator**>(reinterpret_cast<size_t>(arr) + 0x18); }

	template<typename T>
	class Array : public ArrayBase<T, size_t, get_array_allocator> {
		memory::IAllocator* allocator{};

	public:
		Array() : ArrayBase<T, size_t>{} {}
		Array(memory::IAllocator* allocator) : allocator{ allocator }, ArrayBase<T, size_t, get_array_allocator>{} {}
		Array(memory::IAllocator* allocator, size_t capacity) : allocator{ allocator }, ArrayBase<T, size_t, get_array_allocator>{ capacity } {}
		Array(const Array<T>& other) : allocator{ other.allocator } {
			this->ArrayBase<T, size_t, get_array_allocator>::ArrayBase(other);
		}
		Array(Array<T>&& other) : allocator{ other.allocator } {
			this->ArrayBase<T, size_t, get_array_allocator>::ArrayBase(std::move(other));
		}

		void change_allocator(memory::IAllocator* new_allocator) {
			if (!new_allocator || allocator == new_allocator)
				return;

			if (this->is_deallocation_disabled() || !this->buffer) {
				allocator = new_allocator;
				return;
			}

			T* new_buffer = static_cast<T*>(new_allocator->Alloc(this->capacity() * sizeof(T), 16));

			std::uninitialized_move(this->begin(), this->end(), new_buffer);
			std::destroy(this->begin(), this->end());

			if (allocator && !this->is_deallocation_disabled())
				allocator->Free(this->buffer);

			this->buffer = new_buffer;
			allocator = new_allocator;
		}

		void swap(Array<T>& other) noexcept {
			ArrayBase<T, size_t, get_array_allocator>::swap(*this, other);
			std::swap(allocator, other.allocator);
		}
	};

	template<typename T, size_t Len>
	class InplaceArray : public Array<T> {
		T reserved[Len];

	public:
		InplaceArray(memory::IAllocator* allocator) : Array<T>{ allocator } {
			this->capacity_and_flags = Len | this->DONT_DEALLOCATE_FLAG;
			this->buffer = reserved;
		}
	};
}
