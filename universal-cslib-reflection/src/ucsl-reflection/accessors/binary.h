#pragma once
#include <ucsl/containers/arrays/array.h>
#include <ucsl/containers/arrays/tarray.h>
#include <ucsl-reflection/providers/types.h>
#include <ucsl-reflection/util/memory.h>
#include <ucsl-reflection/bound-reflection.h>

namespace ucsl::reflection::accessors {
	template<typename GameInterface>
	struct binary {
		struct opaque_value {};

		template<typename Refl>
		void construct(Refl refl, void* ptr, size_t count = 1);
		//{
		//	for (size_t i = 0; i < count; i++) {
		//		auto item_size = refl.get_size()
		//		auto* item_ptr = 

		//		return refl.visit([&](auto r) {
		//			if constexpr (decltype(r)::kind == providers::TypeKind::PRIMITIVE) return f(PrimitiveAccessor<decltype(r)>{ this->value, r });
		//			else if constexpr (decltype(r)::kind == providers::TypeKind::ENUM) return f(EnumAccessor<decltype(r)>{ this->value, r });
		//			//else if constexpr (decltype(r)::kind == providers::TypeKind::FLAGS) return f(flags(this->value, refl.is_erased(), r));
		//			else if constexpr (decltype(r)::kind == providers::TypeKind::ARRAY) return f(ArrayAccessor<ucsl::containers::arrays::Array, decltype(r)>{ this->value, r });
		//			else if constexpr (decltype(r)::kind == providers::TypeKind::TARRAY) return f(ArrayAccessor<ucsl::containers::arrays::TArray, decltype(r)>{ this->value, r });
		//			else if constexpr (decltype(r)::kind == providers::TypeKind::POINTER) return f(PointerAccessor<decltype(r)>{ this->value, r });
		//			else if constexpr (decltype(r)::kind == providers::TypeKind::CARRAY) return f(CArrayAccessor<decltype(r)>{ this->value, r });
		//			//else if constexpr (decltype(r)::kind == providers::TypeKind::UNION) return f(union(this->value, parent, r));
		//			else if constexpr (decltype(r)::kind == providers::TypeKind::STRUCTURE) {
		//				r.visit_fields([&](auto f) {
		//					construct(f.get_type(), util::addptr(ptr, field_refl.get_offset()));
		//				});
		//			}
		//			else static_assert(false, "invalid type kind");
		//		});
		//	}
		//}

		template<typename Refl>
		void destroy(Refl refl, void* ptr, size_t count = 1);

		template<typename Refl>
		void copy(Refl refl, void* to, void* from, size_t count = 1);

		template<typename Refl>
		void move(Refl refl, void* to, void* from, size_t count = 1);

		template<template<typename, typename> typename A, typename Refl>
		class RflArray : public A<opaque_value, typename GameInterface::AllocatorSystem> {
		public:
			void change_allocator(auto accessor, GameInterface::AllocatorSystem::allocator_type* new_allocator) {
				if (!new_allocator || this->allocator == new_allocator)
					return;

				if (this->is_deallocation_disabled() || !this->buffer) {
					this->allocator = new_allocator;
					return;
				}

				opaque_value* new_buffer = static_cast<opaque_value*>(new_allocator->Alloc(this->capacity() * accessor.refl.get_item_type().get_size(accessor), 16));

				move(accessor.refl, new_buffer, this->buffer, this->length);

				if (this->allocator && !this->is_deallocation_disabled())
					this->allocator->Free(this->buffer);

				this->buffer = new_buffer;
				this->allocator = new_allocator;
			}

			void reserve(auto accessor, size_t desired_cap) {
				auto* allocator = this->get_allocator();
				auto cap = this->capacity();

				if (desired_cap <= cap)
					return;

				auto newcap = std::max(2 * cap, desired_cap);

				opaque_value* new_buffer = static_cast<opaque_value*>(allocator->Alloc(newcap * accessor.refl.get_item_type().get_size(accessor), 16));

				if (this->buffer) {
					move(accessor.refl, new_buffer, this->buffer, this->length);

					if (!this->is_deallocation_disabled())
						allocator->Free(this->buffer);
				}

				this->buffer = new_buffer;
				this->capacity_and_flags = newcap;
			}

			opaque_value& emplace_back(auto accessor) {
				this->reserve(accessor.refl, this->length + 1);

				opaque_value& loc = at(accessor, this->length);

				construct(accessor.refl, &loc);
				//if (member->GetSubType() == GameInterface::RflSystem::RflClassMember::Type::STRUCT)
				//	GameInterface::RflTypeInfoRegistry::GetInstance()->ConstructObject(this->get_allocator(), &at(this->length, member), member->GetClass()->GetName());
				//else
				//	memset(&at(this->length, member), 0, member->GetSubTypeSize());

				this->length++;

				return loc;
			}

			void remove(auto accessor, size_t i) {
				if (i >= this->length)
					return;

				//if (member->GetSubType() == GameInterface::RflSystem::RflClassMember::Type::STRUCT)
				//	GameInterface::RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(&at(i, member), member->GetName());

				destroy(accessor.refl, &at(accessor, i));
				move(accessor.refl, &at(accessor, i), &at(accessor, i + 1), this->length - i - 1);

				this->length--;
			}

			void clear(auto accessor) {
				if (this->empty())
					return;

				for (size_t i = 0; i < this->length; i++)
					destroy(accessor.refl, &at(accessor, i));
					//if (member->GetSubType() == GameInterface::RflSystem::RflClassMember::Type::STRUCT)
					//	GameInterface::RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(&at(i, member), member->GetName());

				this->length = 0;
			}

			opaque_value& at(auto accessor, size_t i) {
				return *util::addptr(this->buffer, accessor.refl.get_item_type().get_size(accessor) * i);
			}
		};

		template<typename Refl, typename T = opaque_value>
		class AccessorBase {
		public:
			T& value;
			Refl refl;

			template<typename U>
			inline AccessorBase(U& value, const Refl& refl) : value{ (T&)value }, refl{ refl } {}
		};

		template<typename Refl>
		class ValueAccessor;

		template<typename Refl>
		class PrimitiveDataAccessor : public AccessorBase<Refl, typename Refl::repr> {
		public:
			using AccessorBase<Refl, typename Refl::repr>::AccessorBase;
			
			PrimitiveDataAccessor<Refl>& operator=(typename Refl::repr v) {
				if constexpr (std::is_fundamental_v<typename Refl::repr>)
					this->value = this->refl.is_erased ? typename Refl::repr{} : this->refl.constant_value.has_value() ? this->refl.constant_value.value() : v;
				else
					this->value = this->refl.constant_value.has_value() ? this->refl.constant_value.value() : v;
				return *this;
			}

			operator typename Refl::repr() const {
				if constexpr (std::is_fundamental_v<typename Refl::repr>)
					return this->refl.is_erased ? typename Refl::repr{} : this->refl.constant_value.has_value() ? this->refl.constant_value.value() : this->value;
				else
					return this->refl.constant_value.has_value() ? this->refl.constant_value.value() : this->value;
			}
		};

		template<typename Refl>
		class PrimitiveAccessor : public AccessorBase<Refl> {
		public:
			using AccessorBase<Refl>::AccessorBase;

			template<typename F>
			inline auto visit(F f) {
				return this->refl.visit([&](auto r){ return f(PrimitiveDataAccessor<decltype(r)>{ this->value, r }); });
			}

			template<typename F>
			inline const auto visit(F f) const {
				return this->refl.visit([&](auto r){ return f(PrimitiveDataAccessor<decltype(r)>{ this->value, r }); });
			}
		};

		template<typename Refl>
		class EnumAccessor : public AccessorBase<Refl> {
		public:
			using AccessorBase<Refl>::AccessorBase;
			
			EnumAccessor<Refl>& operator=(long long v) {
				this->refl.visit([&](auto r){
					PrimitiveDataAccessor<decltype(r)> pd{ this->value, r };

					pd = static_cast<typename decltype(r)::repr>(v);
				});
				return *this;
			}

			operator long long () const {
				return this->refl.visit([&](auto r){
					PrimitiveDataAccessor<decltype(r)> pd{ this->value, r };

					return static_cast<long long>(pd);
				});
			}
		};

		template<typename Refl>
		class StructureAccessor : public AccessorBase<Refl> {
		public:
			using AccessorBase<Refl>::AccessorBase;

			template<typename FieldRefl>
			inline auto operator[](const FieldRefl& field_refl) {
				auto type = field_refl.get_type(*this);

				return ValueAccessor<decltype(type)>{ *util::addptr(&this->value, field_refl.get_offset()), type };
			}

			template<typename FieldRefl>
			inline const auto operator[](const FieldRefl& field_refl) const {
				auto type = field_refl.get_type(*this);

				return ValueAccessor<decltype(type)>{ *util::addptr(&this->value, field_refl.get_offset()), type };
			}

			inline auto get_base() const {
				auto base = this->refl.get_base();

				return base.has_value() ? std::make_optional(StructureAccessor<std::remove_reference_t<decltype(base.value())>>{ this->value, base.value() }) : std::nullopt;
			}
		};

		template<typename Refl>
		class CArrayAccessor : public AccessorBase<Refl> {
		public:
			using AccessorBase<Refl>::AccessorBase;

			inline size_t get_length() const {
				return this->refl.get_length();
			}

			inline auto operator[](size_t idx) {
				auto item_refl = this->refl.get_item_type();

				assert(idx < this->refl.get_length());

				return ValueAccessor<decltype(item_refl)>{ *util::addptr(&this->value, idx * item_refl.get_size(*this)), item_refl };
			}

			inline const auto operator[](size_t idx) const {
				auto item_refl = this->refl.get_item_type();

				assert(idx < this->refl.get_length());

				return ValueAccessor<decltype(item_refl)>{ *util::addptr(&this->value, idx * item_refl.get_size(*this)), item_refl };
			}
		};

		template<typename Refl>
		class PointerAccessor : public AccessorBase<Refl, opaque_value*> {
		public:
			using AccessorBase<Refl, opaque_value*>::AccessorBase;

			inline void clear() {
				this->value = nullptr;
			}

			inline void set(const AccessorBase<Refl>& other) {
				this->value = &other.value;
			}

			inline auto get() {
				auto target_type = this->refl.get_target_type();

				return this->value == nullptr ? std::nullopt : std::make_optional<ValueAccessor<decltype(target_type)>>({ *this->value, target_type });
			}

			inline auto get() const {
				auto target_type = this->refl.get_target_type();

				return this->value == nullptr ? std::nullopt : std::make_optional<const ValueAccessor<decltype(target_type)>>({ *this->value, target_type });
			}
		};

		template<template<typename, typename> typename A, typename Refl>
		class ArrayAccessor : public AccessorBase<Refl, RflArray<A, Refl>> {
		public:
			class iterator {
				ArrayAccessor& accessor;
				size_t idx{};

			public:
				iterator(ArrayAccessor& accessor, size_t idx) : accessor{ accessor }, idx{ idx } {}
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
				auto operator*() { return accessor[idx]; }
			};

			class const_iterator {
				ArrayAccessor& accessor;
				size_t idx{};

			public:
				const_iterator(ArrayAccessor& accessor, size_t idx) : accessor{ accessor }, idx{ idx } {}
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
				const auto operator*() const { return accessor[idx]; }
			};

			//template<typename T> OpaqueReflArray(A<T, typename GameInterface::AllocatorSystem>& underlying, Refl refl) : underlying{ static_cast<RflArray<A>&>(underlying) }, refl{ refl } {}
			//template<typename T> OpaqueReflArray(const OpaqueReflArray<A, GameInterface, Refl>& other, Refl refl) : underlying{ other.underlying }, refl{ other.refl } {}
			
			using AccessorBase<Refl, RflArray<A, Refl>>::AccessorBase;

			GameInterface::AllocatorSystem::allocator_type* get_allocator() const { return this->value.get_allocator(); }
			void change_allocator(GameInterface::AllocatorSystem::allocator_type* new_allocator) { this->value.change_allocator(*this, new_allocator); }
			void reserve(size_t len) { this->value.reserve(*this, len); }

			auto emplace_back() {
				auto item_type = this->refl.get_item_type();

				return ValueAccessor<decltype(item_type)>{ this->value.emplace_back(*this), item_type };
			}
			void remove(size_t i) { this->value.remove(*this, i); }
			void clear() { this->value.clear(*this); }

			iterator begin() { return { *this, 0 }; }
			const_iterator begin() const { return { *this, 0 }; }
			const_iterator cbegin() const { return { *this, 0 }; }
			iterator end() { return { *this, size() }; }
			const_iterator end() const { return { *this, size() }; }
			const_iterator cend() const { return { *this, size() }; }

			auto operator[](size_t i) {
				auto item_type = this->refl.get_item_type();

				return ValueAccessor<decltype(item_type)>{ this->value.at(*this, i), item_type };
			}
			const auto operator[](size_t i) const {
				auto item_type = this->refl.get_item_type();

				return ValueAccessor<decltype(item_type)>{ this->value.at(*this, i), item_type };
			}

			size_t size() const { return this->value.size(); }
			size_t capacity() const { return this->value.capacity(); }
		};

		template<typename Refl>
		class ValueAccessor : public AccessorBase<Refl> {
		public:
			using AccessorBase<Refl>::AccessorBase;

			const auto visit(auto f) const {
				return this->refl.visit([&](auto r) {
					if constexpr (decltype(r)::kind == providers::TypeKind::PRIMITIVE) return f(PrimitiveAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == providers::TypeKind::ENUM) return f(EnumAccessor<decltype(r)>{ this->value, r });
					//else if constexpr (decltype(r)::kind == providers::TypeKind::FLAGS) return f(flags(this->value, refl.is_erased(), r));
					else if constexpr (decltype(r)::kind == providers::TypeKind::ARRAY) return f(ArrayAccessor<ucsl::containers::arrays::Array, decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == providers::TypeKind::TARRAY) return f(ArrayAccessor<ucsl::containers::arrays::TArray, decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == providers::TypeKind::POINTER) return f(PointerAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == providers::TypeKind::CARRAY) return f(CArrayAccessor<decltype(r)>{ this->value, r });
					//else if constexpr (decltype(r)::kind == providers::TypeKind::UNION) return f(union(this->value, parent, r));
					else if constexpr (decltype(r)::kind == providers::TypeKind::STRUCTURE) return f(StructureAccessor<decltype(r)>{ this->value, r });
					else static_assert(false, "invalid type kind");
				});
			}

			auto visit(auto f) {
				return this->refl.visit([&](auto r) {
					if constexpr (decltype(r)::kind == providers::TypeKind::PRIMITIVE) return f(PrimitiveAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == providers::TypeKind::ENUM) return f(EnumAccessor<decltype(r)>{ this->value, r });
					//else if constexpr (decltype(r)::kind == providers::TypeKind::FLAGS) return f(flags(this->value, refl.is_erased(), r));
					else if constexpr (decltype(r)::kind == providers::TypeKind::ARRAY) return f(ArrayAccessor<ucsl::containers::arrays::Array, decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == providers::TypeKind::TARRAY) return f(ArrayAccessor<ucsl::containers::arrays::TArray, decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == providers::TypeKind::POINTER) return f(PointerAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == providers::TypeKind::CARRAY) return f(CArrayAccessor<decltype(r)>{ this->value, r });
					//else if constexpr (decltype(r)::kind == providers::TypeKind::UNION) return f(union(this->value, parent, r));
					else if constexpr (decltype(r)::kind == providers::TypeKind::STRUCTURE) return f(StructureAccessor<decltype(r)>{ this->value, r });
					else static_assert(false, "invalid type kind");
				});
			}

			auto as_primitive() {
				return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == providers::TypeKind::PRIMITIVE) return PrimitiveAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not a primitive"); });
			}

			auto as_enum() {
				return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == providers::TypeKind::ENUM) return EnumAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not an enum"); });
			}

			auto as_array() {
				return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == providers::TypeKind::ARRAY) return ArrayAccessor<ucsl::containers::arrays::Array, decltype(r)>{ this->value, r }; else static_assert(false, "not a array"); });
			}

			auto as_tarray() {
				return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == providers::TypeKind::TARRAY) return ArrayAccessor<ucsl::containers::arrays::TArray, decltype(r)>{ this->value, r }; else static_assert(false, "not a tarray"); });
			}

			auto as_carray() {
				return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == providers::TypeKind::CARRAY) return CArrayAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not a carray"); });
			}

			auto as_pointer() {
				return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == providers::TypeKind::POINTER) return PointerAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not a pointer"); });
			}

			auto as_structure() {
				return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == providers::TypeKind::STRUCTURE) return StructureAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not a structure"); });
			}
		};
	};
}
