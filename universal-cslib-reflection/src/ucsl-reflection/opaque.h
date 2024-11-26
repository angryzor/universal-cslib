#pragma once
#include <ucsl/memory/iallocator.h>
#include <ucsl-reflection/util/memory.h>
#include <iterator>

namespace ucsl::reflection {
	class opaque_obj {};

	template<template<typename, typename> typename A, typename GameInterface>
	class OpaqueRflArray {
	protected:
		template<template<typename, typename> typename A2>
		class RflArray : public A<opaque_obj, typename GameInterface::AllocatorSystem> {
		public:
			void change_allocator(ucsl::memory::IAllocator* new_allocator, const typename GameInterface::RflSystem::RflClassMember* member) {
				if (!new_allocator || this->allocator == new_allocator)
					return;

				if (this->is_deallocation_disabled() || !this->buffer) {
					this->allocator = new_allocator;
					return;
				}

				size_t subtype_size = member->GetSubTypeSize();
				opaque_obj* new_buffer = static_cast<opaque_obj*>(new_allocator->Alloc(this->capacity() * subtype_size, 16));

				memcpy(new_buffer, this->buffer, subtype_size * this->length);

				if (this->allocator && !this->is_deallocation_disabled())
					this->allocator->Free(this->buffer);

				this->buffer = new_buffer;
				this->allocator = new_allocator;
			}

			void reserve(size_t desired_cap, const typename GameInterface::RflSystem::RflClassMember* member) {
				auto* allocator = this->get_allocator();
				auto cap = this->capacity();

				if (desired_cap <= cap)
					return;

				auto newcap = std::max(2 * cap, desired_cap);

				size_t subtype_size = member->GetSubTypeSize();
				opaque_obj* new_buffer = static_cast<opaque_obj*>(allocator->Alloc(subtype_size * newcap, 16));

				if (this->buffer) {
					memcpy(new_buffer, this->buffer, subtype_size * this->length);

					if (!this->is_deallocation_disabled())
						allocator->Free(this->buffer);
				}

				this->buffer = new_buffer;
				this->capacity_and_flags = newcap;
			}

			template<typename... Args>
			void emplace_back(const typename GameInterface::RflSystem::RflClassMember* member, Args&&... args) {
				this->reserve(this->length + 1, member);

				if (member->GetSubType() == GameInterface::RflSystem::RflClassMember::Type::STRUCT)
					GameInterface::RflTypeInfoRegistry::GetInstance()->ConstructObject(this->get_allocator(), &at(this->length, member), member->GetClass()->GetName());
				else
					memset(&at(this->length, member), 0, member->GetSubTypeSize());

				this->length++;
			}

			void remove(size_t i, const typename GameInterface::RflSystem::RflClassMember* member) {
				if (i >= this->length)
					return;

				if (member->GetSubType() == GameInterface::RflSystem::RflClassMember::Type::STRUCT)
					GameInterface::RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(&at(i, member), member->GetName());

				memmove(&at(i, member), &at(i + 1, member), member->GetSubTypeSize() * (this->length - i - 1));

				this->length--;
			}

			void clear(const typename GameInterface::RflSystem::RflClassMember* member) {
				if (this->empty())
					return;

				for (size_t i = 0; i < this->length; i++)
					if (member->GetSubType() == GameInterface::RflSystem::RflClassMember::Type::STRUCT)
						GameInterface::RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(&at(i, member), member->GetName());

				this->length = 0;
			}

			opaque_obj& at(size_t i, const typename GameInterface::RflSystem::RflClassMember* member) {
				return *util::addptr(this->buffer, member->GetSubTypeSize() * i);
			}
		};

	public:
		RflArray<A>& underlying;
		const typename GameInterface::RflSystem::RflClassMember* member;

		class iterator {
			OpaqueRflArray& accessor;
			size_t idx{};

		public:
			iterator(OpaqueRflArray& accessor, size_t idx) : accessor{ accessor }, idx{ idx } {}
			iterator(const iterator& other) : accessor{ other.accessor }, idx{ other.idx } {}

			iterator& operator++() {
				idx++;
				return *this;
			}

			iterator operator++(int) {
				iterator result{ *this };
				idx++;
				return result;
			}

			iterator& operator--() {
				idx--;
				return *this;
			}

			iterator operator--(int) {
				iterator result{ *this };
				idx--;
				return result;
			}

			bool operator==(const iterator& other) const { return idx == other.idx; }
			bool operator!=(const iterator& other) const { return idx != other.idx; }
			bool operator<(const iterator& other) const { return idx < other.idx; }
			bool operator>(const iterator& other) const { return idx > other.idx; }
			bool operator<=(const iterator& other) const { return idx <= other.idx; }
			bool operator>=(const iterator& other) const { return idx >= other.idx; }
			opaque_obj& operator*() { return accessor[idx]; }
		};

		class const_iterator {
			OpaqueRflArray& accessor;
			size_t idx{};

		public:
			const_iterator(OpaqueRflArray& accessor, size_t idx) : accessor{ accessor }, idx{ idx } {}
			const_iterator(const const_iterator& other) : accessor{ other.accessor }, idx{ other.idx } {}

			const_iterator& operator++() {
				idx++;
				return *this;
			}

			const_iterator operator++(int) {
				const_iterator result{ *this };
				idx++;
				return result;
			}

			const_iterator& operator--() {
				idx--;
				return *this;
			}

			const_iterator operator--(int) {
				const_iterator result{ *this };
				idx--;
				return result;
			}

			bool operator==(const const_iterator& other) const { return idx == other.idx; }
			bool operator!=(const const_iterator& other) const { return idx != other.idx; }
			bool operator<(const const_iterator& other) const { return idx < other.idx; }
			bool operator>(const const_iterator& other) const { return idx > other.idx; }
			bool operator<=(const const_iterator& other) const { return idx <= other.idx; }
			bool operator>=(const const_iterator& other) const { return idx >= other.idx; }
			const opaque_obj& operator*() { return accessor[idx]; }
		};

		template<typename T> OpaqueRflArray(A<T, typename GameInterface::AllocatorSystem>& underlying, const typename GameInterface::RflSystem::RflClassMember* member) : underlying{ static_cast<RflArray<A>&>(underlying) }, member{ member } {}
		template<typename T> OpaqueRflArray(const OpaqueRflArray<A, GameInterface>& other, const typename GameInterface::RflSystem::RflClassMember* member) : underlying{ other.underlying }, member{ other.member } {}

		void change_allocator(ucsl::memory::IAllocator* new_allocator) { underlying.change_allocator(new_allocator, member); }
		void reserve(size_t len) { underlying.reserve(len, member); }

		template<typename... Args> void emplace_back(Args&&... args) { underlying.emplace_back(member, std::forward<Args>(args)...); }
		void remove(size_t i) { underlying.remove(i, member); }
		void clear() { underlying.clear(member); }

		iterator begin() { return { *this, 0 }; }
		const_iterator begin() const { return { *this, 0 }; }
		const_iterator cbegin() const { return { *this, 0 }; }
		iterator end() { return { *this, size() }; }
		const_iterator end() const { return { *this, size() }; }
		const_iterator cend() const { return { *this, size() }; }
		opaque_obj& operator[](size_t i) { return underlying.at(i, member); }

		size_t size() const { return underlying.size(); }
		size_t capacity() const { return underlying.capacity(); }
	};

	template<typename A, typename GameInterface>
	class OpaqueArray {
	protected:
	public:
		A& underlying;

		class iterator {
			OpaqueArray& accessor;
			size_t idx{};

		public:
			iterator(OpaqueArray& accessor, size_t idx) : accessor{ accessor }, idx{ idx } {}
			iterator(const iterator& other) : accessor{ other.accessor }, idx{ other.idx } {}

			iterator& operator++() {
				idx++;
				return *this;
			}

			iterator operator++(int) {
				iterator result{ *this };
				idx++;
				return result;
			}

			iterator& operator--() {
				idx--;
				return *this;
			}

			iterator operator--(int) {
				iterator result{ *this };
				idx--;
				return result;
			}

			bool operator==(const iterator& other) const { return idx == other.idx; }
			bool operator!=(const iterator& other) const { return idx != other.idx; }
			bool operator<(const iterator& other) const { return idx < other.idx; }
			bool operator>(const iterator& other) const { return idx > other.idx; }
			bool operator<=(const iterator& other) const { return idx <= other.idx; }
			bool operator>=(const iterator& other) const { return idx >= other.idx; }
			opaque_obj& operator*() { return accessor[idx]; }
		};

		class const_iterator {
			OpaqueArray& accessor;
			size_t idx{};

		public:
			const_iterator(OpaqueArray& accessor, size_t idx) : accessor{ accessor }, idx{ idx } {}
			const_iterator(const const_iterator& other) : accessor{ other.accessor }, idx{ other.idx } {}

			const_iterator& operator++() {
				idx++;
				return *this;
			}

			const_iterator operator++(int) {
				const_iterator result{ *this };
				idx++;
				return result;
			}

			const_iterator& operator--() {
				idx--;
				return *this;
			}

			const_iterator operator--(int) {
				const_iterator result{ *this };
				idx--;
				return result;
			}

			bool operator==(const const_iterator& other) const { return idx == other.idx; }
			bool operator!=(const const_iterator& other) const { return idx != other.idx; }
			bool operator<(const const_iterator& other) const { return idx < other.idx; }
			bool operator>(const const_iterator& other) const { return idx > other.idx; }
			bool operator<=(const const_iterator& other) const { return idx <= other.idx; }
			bool operator>=(const const_iterator& other) const { return idx >= other.idx; }
			const opaque_obj& operator*() { return accessor[idx]; }
		};

		OpaqueArray(A& underlying) : underlying{ underlying } {}
		OpaqueArray(const OpaqueArray<A, GameInterface>& other) : underlying{ other.underlying } {}

		void change_allocator(ucsl::memory::IAllocator* new_allocator) { underlying.change_allocator(new_allocator); }
		void reserve(size_t len) { underlying.reserve(len); }

		template<typename... Args> void emplace_back(Args&&... args) { underlying.emplace_back(std::forward<Args>(args)...); }
		void remove(size_t i) { underlying.remove(i); }
		void clear() { underlying.clear(); }

		iterator begin() { return { *this, 0 }; }
		const_iterator begin() const { return { *this, 0 }; }
		const_iterator cbegin() const { return { *this, 0 }; }
		iterator end() { return { *this, size() }; }
		const_iterator end() const { return { *this, size() }; }
		const_iterator cend() const { return { *this, size() }; }
		opaque_obj& operator[](size_t i) { return reinterpret_cast<opaque_obj&>(underlying[i]); }

		size_t size() const { return underlying.size(); }
		size_t capacity() const { return underlying.capacity(); }
	};
}
