#pragma once
#include <cstddef>
#include <iterator>
#include <ranges>
#include <memory>
#include <ucsl/memory/iallocator.h>

namespace ucsl::containers::arrays {
	typedef memory::IAllocator* AllocatorGetterFn(void* arr);

	template<typename T, typename S, AllocatorGetterFn* get_allocator>
	class ArrayBase {
	protected:
		static constexpr unsigned int DONT_DEALLOCATE_FLAG = 0x80000000;
		static constexpr unsigned int CAPACITY_MASK = ~DONT_DEALLOCATE_FLAG;

		static constexpr AllocatorGetterFn* get_allocator = get_allocator;

		T* buffer{};
		S length{};
		S capacity_and_flags{ DONT_DEALLOCATE_FLAG };

	public:
		using value_type = T;
		using reference = T&;
		using const_reference = const T&;
		using iterator = T*;
		using const_iterator = const T*;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using difference_type = std::ptrdiff_t;
		using size_type = std::size_t;

	private:
		struct measurement {
			difference_type existing_count{};
			difference_type new_items_to_assign{};
			difference_type new_items_to_construct{};
			difference_type existing_items_to_assign{};
			difference_type existing_items_to_construct{};
		};

	public:
		ArrayBase() {}
		ArrayBase(size_type capacity) { reserve(capacity); }
		ArrayBase(const ArrayBase<T, S, get_allocator>& other) : ArrayBase<T, S, get_allocator>{ other.capacity() } { insert(cbegin(), other.cbegin(), other.cend()); }
		ArrayBase(ArrayBase<T, S, get_allocator>&& other) : buffer{ other.buffer }, length{ other.length }, capacity{ other.capacity } {
			other.buffer = nullptr;
			other.length = 0;
			other.capacity = DONT_DEALLOCATE_FLAG;
		}
		~ArrayBase() {
			auto* allocator = get_allocator(this);

			std::destroy(begin(), end());

			if (allocator && !is_deallocation_disabled())
				allocator->Free(buffer);
		}

		// CSL specifics
		bool is_deallocation_disabled() {
			return capacity_and_flags & DONT_DEALLOCATE_FLAG;
		}

		// Element access
		reference at(size_type pos) { return buffer[pos]; }
		const_reference at(size_type pos) const { return buffer[pos]; }

		reference operator[](size_type pos) { return buffer[pos]; }
		const_reference operator[](size_type pos) const { return buffer[pos]; }

		reference front() { return buffer[0]; }
		const_reference front() const { return buffer[0]; }

		reference back() { return buffer[length - 1]; }
		const_reference back() const { return buffer[length - 1]; }

		T* data() { return buffer; }
		const T* data() const { return buffer; }

		// Iterators
		iterator begin() { return buffer; }
		const_iterator cbegin() const { return buffer; }

		iterator end() { return &buffer[length]; }
		const_iterator cend() const { return &buffer[length]; }

		reverse_iterator rbegin() { return &buffer[length]; }
		const_reverse_iterator crbegin() const { return &buffer[length]; }

		reverse_iterator rend() { return buffer; }
		const_reverse_iterator crend() const { return buffer; }

		// Capacity
		bool empty() const { return size() == 0; }
		size_type size() const { return length; }
		size_type max_size() const { return CAPACITY_MASK; }
		size_type capacity() const { return capacity_and_flags & CAPACITY_MASK; }

		void reserve(size_type desired_cap)
		{
			auto* allocator = get_allocator(this);
			auto cap = capacity();

			if (desired_cap <= cap)
				return;

			auto newcap = std::max(2 * cap, desired_cap);

			T* new_buffer = static_cast<T*>(allocator->Alloc(sizeof(T) * newcap, alignof(T)));

			if (buffer) {
				std::uninitialized_move(begin(), end(), new_buffer);
				std::destroy(begin(), end());

				if (!is_deallocation_disabled())
					allocator->Free(buffer);
			}

			buffer = new_buffer;
			capacity_and_flags = newcap;
		}

		// Modifiers
		void clear() {
			if (empty())
				return;

			std::destroy(begin(), end());

			length = 0;
		}

		iterator insert(const_iterator pos, const T& value) {
			return insert(pos, 1, value);
		}

		iterator insert(const_iterator pos, T&& value) {
			difference_type index = pos - begin();
			measurement m = measure(index, 1);
			if (index == length) {
				reserve(length + 1);
				new (buffer + length) T{ std::move(value) };
				length++;
			}
			else {
				expand(index, 1, m);
				buffer[index] = std::move(value);
			}
			return buffer + index;
		}

		iterator insert(const_iterator pos, size_type count, const T& value) {
			difference_type index = pos - begin();
			measurement m = measure(index, count);
			expand(index, count, m);

			iterator target = buffer + index;
			std::uninitialized_fill_n(end(), m.new_items_to_construct, value);
			std::fill_n(buffer + index, m.new_items_to_assign, value);
			return target;
		}

		template<typename InputIt>
		iterator insert(const_iterator pos, InputIt first, InputIt last) {
			size_type count = std::distance(first, last);
			difference_type index = pos - begin();
			measurement m = measure(index, count);
			expand(index, count, m);

			iterator target = buffer + index;
			std::uninitialized_copy_n(first + m.new_items_to_assign, m.new_items_to_construct, end());
			std::copy_n(first, m.new_items_to_assign, target);
			return target;
		}

		template<typename T>
		iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
			auto count = ilist.size();
			difference_type index = pos - begin();
			measurement m = measure(index, count);
			expand(index, count, m);

			iterator target = buffer + index;
			std::uninitialized_copy_n(ilist.begin() + m.new_items_to_assign, m.new_items_to_construct, end());
			std::copy_n(ilist.begin(), m.new_items_to_assign, target);
			return target;
		}

		// insert_range?
		// append_range?

		template<typename... Args>
		iterator emplace(const_iterator pos, Args&&... args) {
			difference_type index = pos - begin();
			measurement m = measure(index, 1);
			if (index == length) {
				reserve(length + 1);
				new (buffer + index) T{ std::forward<Args>(args)... };
				length++;
			}
			else {
				expand(index, 1, m);
				T elem{ std::forward<Args>(args)... };
				buffer[index] = std::move(elem);
			}
			return buffer + index;
		}

		iterator erase(iterator pos) {
			return erase(pos, pos + 1);
		}

		iterator erase(iterator first, iterator last) {
			auto count = last - first;
			std::move(last, end(), first);
			std::destroy(end() - count, end());
			length -= count;
			return first;
		}

		void push_back(const T& value) {
			reserve(length + 1);
			new (end()) T{ value };
			length++;
		}

		template<typename... Args>
		reference emplace_back(Args... args) {
			reserve(length + 1);
			new (end()) T{ std::forward<Args>(args...) };
			length++;
			return back();
		}

		void pop_back() {
			back().~T();
			length--;
		}

		void resize(size_type count) {
			reserve(length + count);
			std::uninitialized_default_construct_n(end(), count);
			length += count;
		}

		void resize(size_type count, const value_type& value) {
			reserve(length + count);
			std::uninitialized_fill_n(end(), count, value);
		}

		void swap(ArrayBase<T, S, get_allocator>& other) noexcept {
			std::swap(buffer, other.buffer);
			std::swap(length, other.length);
			std::swap(capacity_and_flags, other.capacity_and_flags);
		}

	private:
		void expand(difference_type index, size_type count, const measurement& m) {
			reserve(length + count);
			iterator target = buffer + index;
			std::uninitialized_move(end() - m.existing_items_to_construct, end(), end() + m.new_items_to_construct);
			std::move_backward(target, target + m.existing_items_to_assign, target + m.existing_items_to_assign + count);
			length += count;
		}

		measurement measure(difference_type index, size_type count) {
			difference_type existing_count = length - index;
			difference_type new_items_to_assign = std::min<difference_type>(count, existing_count);
			difference_type new_items_to_construct = count - new_items_to_assign;
			difference_type existing_items_to_assign = std::max<difference_type>(existing_count - new_items_to_assign, 0);
			difference_type existing_items_to_construct = existing_count - existing_items_to_assign;

			return { existing_count, new_items_to_assign, new_items_to_construct, existing_items_to_assign, existing_items_to_construct };
		}
	};
}

template<typename T, typename S, ucsl::containers::arrays::AllocatorGetterFn* get_allocator> bool operator==(const ucsl::containers::arrays::ArrayBase<T, S, get_allocator>& lhs, const ucsl::containers::arrays::ArrayBase<T, S, get_allocator>& rhs) { return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }
template<typename T, typename S, ucsl::containers::arrays::AllocatorGetterFn* get_allocator> bool operator!=(const ucsl::containers::arrays::ArrayBase<T, S, get_allocator>& lhs, const ucsl::containers::arrays::ArrayBase<T, S, get_allocator>& rhs) { return !operator==(lhs, rhs); }
template<typename T, typename S, ucsl::containers::arrays::AllocatorGetterFn* get_allocator> bool operator<(const ucsl::containers::arrays::ArrayBase<T, S, get_allocator>& lhs, const ucsl::containers::arrays::ArrayBase<T, S, get_allocator>& rhs) { return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }
template<typename T, typename S, ucsl::containers::arrays::AllocatorGetterFn* get_allocator> bool operator>(const ucsl::containers::arrays::ArrayBase<T, S, get_allocator>& lhs, const ucsl::containers::arrays::ArrayBase<T, S, get_allocator>& rhs) { return std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end()); }
template<typename T, typename S, ucsl::containers::arrays::AllocatorGetterFn* get_allocator> bool operator<=(const ucsl::containers::arrays::ArrayBase<T, S, get_allocator>& lhs, const ucsl::containers::arrays::ArrayBase<T, S, get_allocator>& rhs) { return !operator<(lhs, rhs); }
template<typename T, typename S, ucsl::containers::arrays::AllocatorGetterFn* get_allocator> bool operator>=(const ucsl::containers::arrays::ArrayBase<T, S, get_allocator>& lhs, const ucsl::containers::arrays::ArrayBase<T, S, get_allocator>& rhs) { return !operator>(lhs, rhs); }

namespace std {
	template<typename T, typename S, ucsl::containers::arrays::AllocatorGetterFn* get_allocator> void swap(ucsl::containers::arrays::ArrayBase<T, S, get_allocator>& lhs, ucsl::containers::arrays::ArrayBase<T, S, get_allocator>& rhs) { lhs.swap(rhs); }

	template<typename T, typename S, ucsl::containers::arrays::AllocatorGetterFn* get_allocator, typename U> ucsl::containers::arrays::ArrayBase<T, S, get_allocator>::size_type erase(ucsl::containers::arrays::ArrayBase<T, S, get_allocator>& c, const U& value) {
		auto it = std::remove(c.begin(), c.end(), value);
		auto r = c.end() - it;
		c.erase(it, c.end());
		return r;
	}
	template<typename T, typename S, ucsl::containers::arrays::AllocatorGetterFn* get_allocator, typename Pred> ucsl::containers::arrays::ArrayBase<T, S, get_allocator>::size_type erase(ucsl::containers::arrays::ArrayBase<T, S, get_allocator>& c, Pred pred) {
		auto it = std::remove_if(c.begin(), c.end(), pred);
		auto r = c.end() - it;
		c.erase(it, c.end());
		return r;
	}
}