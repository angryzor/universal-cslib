#pragma once
#include "types.h"
#include <ucsl-reflection/util/memory.h>
#include <ucsl-reflection/providers/types.h>

namespace ucsl::reflection::accessors {
	//class BinaryAccessor {
	//	void* obj;

	//public:
	//	inline BinaryAccessor(void* obj) : obj{ obj } {}

	//	BinaryAccessor get_field(const char* name, size_t offset) { return { util::addptr(obj, offset) }; }
	//	BinaryAccessor get_item(size_t index, size_t itemSize) { return { util::addptr(obj, itemSize * index) }; }
	//};

	struct opaque_obj {};

	template<typename GameInterface, typename Refl>
	void construct(Refl refl, void* ptr, size_t count = 1);

	template<typename GameInterface, typename Refl>
	void destroy(Refl refl, void* ptr, size_t count = 1);

	template<typename Refl>
	void copy(Refl refl, void* to, void* from, size_t count = 1);

	template<typename Refl>
	void move(Refl refl, void* to, void* from, size_t count = 1);

	template<template<typename, typename> typename A, typename GameInterface, typename Refl>
	class RflArray : public A<opaque_obj, typename GameInterface::AllocatorSystem> {
	public:
		void change_allocator(Refl refl, GameInterface::AllocatorSystem::allocator_type* new_allocator) {
			if (!new_allocator || this->allocator == new_allocator)
				return;

			if (this->is_deallocation_disabled() || !this->buffer) {
				this->allocator = new_allocator;
				return;
			}

			opaque_obj* new_buffer = static_cast<opaque_obj*>(new_allocator->Alloc(this->capacity() * refl.get_size(), 16));

			move(refl, new_buffer, this->buffer, this->length);

			if (this->allocator && !this->is_deallocation_disabled())
				this->allocator->Free(this->buffer);

			this->buffer = new_buffer;
			this->allocator = new_allocator;
		}

		void reserve(Refl refl, size_t desired_cap) {
			auto* allocator = this->get_allocator();
			auto cap = this->capacity();

			if (desired_cap <= cap)
				return;

			auto newcap = std::max(2 * cap, desired_cap);

			opaque_obj* new_buffer = static_cast<opaque_obj*>(allocator->Alloc(newcap * refl.get_size(), 16));

			if (this->buffer) {
				move(refl, new_buffer, this->buffer, this->length);

				if (!this->is_deallocation_disabled())
					allocator->Free(this->buffer);
			}

			this->buffer = new_buffer;
			this->capacity_and_flags = newcap;
		}

		template<typename... Args>
		opaque_obj& emplace_back(Refl refl, Args&&... args) {
			this->reserve(refl, this->length + 1);

			opaque_obj& loc = at(this->length, refl);

			construct(refl, &loc);
			//if (member->GetSubType() == GameInterface::RflSystem::RflClassMember::Type::STRUCT)
			//	GameInterface::RflTypeInfoRegistry::GetInstance()->ConstructObject(this->get_allocator(), &at(this->length, member), member->GetClass()->GetName());
			//else
			//	memset(&at(this->length, member), 0, member->GetSubTypeSize());

			this->length++;

			return loc;
		}

		void remove(Refl refl, size_t i) {
			if (i >= this->length)
				return;

			//if (member->GetSubType() == GameInterface::RflSystem::RflClassMember::Type::STRUCT)
			//	GameInterface::RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(&at(i, member), member->GetName());

			destroy(refl, &at(refl, i));
			move(refl, &at(refl, i), &at(refl, i + 1), this->length - i - 1);

			this->length--;
		}

		void clear(Refl refl) {
			if (this->empty())
				return;

			for (size_t i = 0; i < this->length; i++)
				destroy(refl, &at(refl, i));
				//if (member->GetSubType() == GameInterface::RflSystem::RflClassMember::Type::STRUCT)
				//	GameInterface::RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(&at(i, member), member->GetName());

			this->length = 0;
		}

		opaque_obj& at(Refl refl, size_t i) {
			return *util::addptr(this->buffer, refl.get_size() * i);
		}
	};

	template<typename GameInterface>
	struct binary {
		template<typename Refl, typename ParentRefl, typename RootRefl, typename Parent, typename Root> class ValueBinaryAccessor;

		template<typename Refl, typename ParentRefl, typename RootRefl, typename Parent, typename Root>
		class AccessorBase {
		public:
			Refl refl;
			std::optional<Parent> parent;
			std::optional<Root> root;

			inline AccessorBase(const Refl& refl, const std::optional<Parent>& parent, const std::optional<Root>& root) : refl{ refl }, parent{ parent }, root{ root } {}
		};

		template<typename Refl, typename ParentRefl, typename RootRefl, typename Parent, typename Root, typename T = opaque_obj>
		class BinaryAccessor : public AccessorBase<Refl, ParentRefl, RootRefl, Parent, Root> {
		protected:
			T& obj;
		
		public:
			inline BinaryAccessor(void* obj, const Refl& refl = Refl{}, const std::optional<Parent>& parent = std::nullopt, const std::optional<Root>& root = std::nullopt) : AccessorBase<Refl, ParentRefl, RootRefl, Parent, Root>{ refl, parent, root }, obj{ *(T*)obj } {}
		};

		template<typename Refl, typename ParentRefl, typename RootRefl, typename Parent, typename Root>
		class PrimitiveDataBinaryAccessor : public BinaryAccessor<Refl, ParentRefl, RootRefl, Parent, Root, typename Refl::repr> {
		public:
			using BinaryAccessor<Refl, ParentRefl, RootRefl, Parent, Root, typename Refl::repr>::BinaryAccessor;
			
			PrimitiveDataBinaryAccessor<Refl, ParentRefl, RootRefl, Parent, Root>& operator=(typename Refl::repr v) {
				//if constexpr (this->refl.is_erased())
				//	this->obj = {};
				//else {
				//	if constexpr (this->refl.get_constant_value().has_value())
				//		this->obj = this->refl.get_constant_value().value();
				//	else
						this->obj = v;
				//}
				return *this;
			}

			operator typename Refl::repr() const {
				//if constexpr (this->refl.get_constant_value().has_value())
				//	return this->refl.get_constant_value().value();
				//else
					return this->obj;
			}
		};

		template<typename Refl, typename ParentRefl, typename RootRefl, typename Parent, typename Root>
		class PrimitiveBinaryAccessor : public BinaryAccessor<Refl, ParentRefl, RootRefl, Parent, Root> {
		public:
			using BinaryAccessor<Refl, ParentRefl, RootRefl, Parent, Root>::BinaryAccessor;

			//template<typename T>
			//inline T& as() {
			//	return this->refl.visit([&](auto r){ return f(PrimitiveDataBinaryAccessor<decltype(r)::repr, ParentRefl, RootRefl, Parent, Root>{ &this->obj, r, this->parent, this->root }); });
			//}

			//template<typename T>
			//inline const T& as() const {
			//	return (const T&)this->obj;
			//}

			template<typename F>
			inline auto visit(F f) {
				return this->refl.visit([&](auto r){ return f(PrimitiveDataBinaryAccessor<decltype(r), ParentRefl, RootRefl, Parent, Root>{ &this->obj, r, this->parent, this->root }); });
			}

			template<typename F>
			inline const auto visit(F f) const {
				return this->refl.visit([&](auto r){ return f(PrimitiveDataBinaryAccessor<decltype(r), ParentRefl, RootRefl, Parent, Root>{ &this->obj, r, this->parent, this->root }); });
			}
		};

		template<typename Refl, typename ParentRefl, typename RootRefl, typename Parent, typename Root>
		class EnumBinaryAccessor : public PrimitiveBinaryAccessor<Refl, ParentRefl, RootRefl, Parent, Root> {
		public:
			using PrimitiveBinaryAccessor<Refl, ParentRefl, RootRefl, Parent, Root>::PrimitiveBinaryAccessor;
		};

		template<typename Refl, typename ParentRefl, typename RootRefl, typename Parent, typename Root>
		class StructureBinaryAccessor : public BinaryAccessor<Refl, ParentRefl, RootRefl, Parent, Root> {
		public:
			using BinaryAccessor<Refl, ParentRefl, RootRefl, Parent, Root>::BinaryAccessor;

			template<typename FieldRefl>
			inline auto operator[](const FieldRefl& field_refl) {
				auto type = field_refl.get_type();

				return ValueBinaryAccessor<decltype(type), Refl, RootRefl, StructureBinaryAccessor<Refl, ParentRefl, RootRefl, Parent, Root>, Root>{ util::addptr(&this->obj, field_refl.get_offset()), type, *this, this->root };
			}

			template<typename FieldRefl>
			inline const auto operator[](const FieldRefl& field_refl) const {
				auto type = field_refl.get_type();

				return ValueBinaryAccessor<decltype(type), Refl, RootRefl, StructureBinaryAccessor<Refl, ParentRefl, RootRefl, Parent, Root>, Root>{ util::addptr(&this->obj, field_refl.get_offset()), type, *this, this->root };
			}

			inline auto get_base() const {
				auto base = this->refl.get_base();

				return base.has_value() ? std::make_optional(StructureBinaryAccessor<std::remove_reference_t<decltype(base.value())>, ParentRefl, RootRefl, Parent, Root>{ &this->obj, base.value(), this->parent, this->root }) : std::nullopt;
			}
		};

		template<typename Refl, typename ParentRefl, typename RootRefl, typename Parent, typename Root>
		class CArrayBinaryAccessor : public BinaryAccessor<Refl, ParentRefl, RootRefl, Parent, Root> {
		public:
			using BinaryAccessor<Refl, ParentRefl, RootRefl, Parent, Root>::BinaryAccessor;

			inline size_t get_length() const {
				return this->refl.get_length(this->parent.value());
			}

			inline auto operator[](size_t idx) {
				auto item_refl = this->refl.get_item_type();

				assert(idx < this->refl.get_length(this->parent.value()));

				return ValueBinaryAccessor<decltype(item_refl), ParentRefl, RootRefl, Parent, Root>{ util::addptr(&this->obj, idx * item_refl.get_size(this->parent, this->root, this)), item_refl, this->parent, this->root };
			}

			inline const auto operator[](size_t idx) const {
				auto item_refl = this->refl.get_item_type();

				assert(idx < this->refl.get_length(this->parent.value()));

				return ValueBinaryAccessor<decltype(item_refl), ParentRefl, RootRefl, Parent, Root>{ util::addptr(&this->obj, idx * item_refl.get_size(this->parent, this->root, this)), item_refl, this->parent, this->root };
			}
		};

		template<typename Refl, typename ParentRefl, typename RootRefl, typename Parent, typename Root>
		class PointerBinaryAccessor : public BinaryAccessor<Refl, ParentRefl, RootRefl, Parent, Root, opaque_obj*> {
		public:
			using BinaryAccessor<Refl, ParentRefl, RootRefl, Parent, Root, opaque_obj*>::BinaryAccessor;

			inline void clear() {
				this->obj = nullptr;
			}

			inline void set(const BinaryAccessor<Refl, ParentRefl, RootRefl, Parent, Root>& other) {
				this->obj = &other.obj;
			}

			inline auto get() {
				auto target_type = this->refl.get_target_type();

				return this->obj == nullptr ? std::nullopt : std::make_optional<ValueBinaryAccessor<decltype(target_type), ParentRefl, RootRefl, Parent, Root>>({ this->obj, target_type, this->parent, this->root });
			}

			inline auto get() const {
				auto target_type = this->refl.get_target_type();

				return this->obj == nullptr ? std::nullopt : std::make_optional<const ValueBinaryAccessor<decltype(target_type), ParentRefl, RootRefl, Parent, Root>>({ this->obj, target_type, this->parent, this->root });
			}
		};

		template<template<typename, typename> typename A, typename Refl, typename ParentRefl, typename RootRefl, typename Parent, typename Root>
		class ArrayBinaryAccessor : public BinaryAccessor<Refl, ParentRefl, RootRefl, Parent, Root, RflArray<A, GameInterface, Refl>> {
		public:
			class iterator {
				ArrayBinaryAccessor& accessor;
				size_t idx{};

			public:
				iterator(ArrayBinaryAccessor& accessor, size_t idx) : accessor{ accessor }, idx{ idx } {}
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
				ArrayBinaryAccessor& accessor;
				size_t idx{};

			public:
				const_iterator(ArrayBinaryAccessor& accessor, size_t idx) : accessor{ accessor }, idx{ idx } {}
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

			using BinaryAccessor<Refl, ParentRefl, RootRefl, Parent, Root>::BinaryAccessor;

			GameInterface::AllocatorSystem::allocator_type* get_allocator() const { return this->obj.get_allocator(); }
			void change_allocator(GameInterface::AllocatorSystem::allocator_type* new_allocator) { this->obj.change_allocator(this->refl, new_allocator); }
			void reserve(size_t len) { this->obj.reserve(this->refl, len); }

			template<typename... Args> auto emplace_back(Args&&... args) {
				auto item_type = this->refl->get_item_type();

				return ValueBinaryAccessor<decltype(item_type), ParentRefl, RootRefl, Parent, Root>{ this->obj.emplace_back(this->refl, std::forward<Args>(args)...), item_type, this->parent, this->root };
			}
			void remove(size_t i) { this->obj.remove(this->refl, i); }
			void clear() { this->obj.clear(this->refl); }

			iterator begin() { return { *this, 0 }; }
			const_iterator begin() const { return { *this, 0 }; }
			const_iterator cbegin() const { return { *this, 0 }; }
			iterator end() { return { *this, size() }; }
			const_iterator end() const { return { *this, size() }; }
			const_iterator cend() const { return { *this, size() }; }

			auto operator[](size_t i) {
				auto item_type = this->refl->get_item_type();

				return ValueBinaryAccessor<decltype(item_type), ParentRefl, RootRefl, Parent, Root>{ this->obj.at(this->refl, i), item_type, this->parent, this->root };
			}
			const auto operator[](size_t i) const {
				auto item_type = this->refl->get_item_type();

				return ValueBinaryAccessor<decltype(item_type), ParentRefl, RootRefl, Parent, Root>{ this->obj.at(this->refl, i), item_type, this->parent, this->root };
			}

			size_t size() const { return this->obj.size(); }
			size_t capacity() const { return this->obj.capacity(); }
		};

		template<typename Refl, typename ParentRefl, typename RootRefl, typename Parent, typename Root>
		class ValueBinaryAccessor : public BinaryAccessor<Refl, ParentRefl, RootRefl, Parent, Root> {
		public:
			using BinaryAccessor<Refl, ParentRefl, RootRefl, Parent, Root>::BinaryAccessor;

			const auto visit(auto f) const {
				return this->refl.visit(this->parent, this->root, [&](auto r) {
					if constexpr (decltype(r)::kind == providers::TypeKind::PRIMITIVE) return f(PrimitiveBinaryAccessor<decltype(r), ParentRefl, RootRefl, Parent, Root>{ &this->obj, r, this->parent, this->root });
					else if constexpr (decltype(r)::kind == providers::TypeKind::ENUM) return f(EnumBinaryAccessor<decltype(r), ParentRefl, RootRefl, Parent, Root>{ &this->obj, r, this->parent, this->root });
					//else if constexpr (decltype(r)::kind == providers::TypeKind::FLAGS) return f(flags(this->obj, refl.is_erased(), r));
					else if constexpr (decltype(r)::kind == providers::TypeKind::ARRAY) return f(ArrayBinaryAccessor<ucsl::containers::arrays::Array, decltype(r), ParentRefl, RootRefl, Parent, Root>{ &this->obj, r, this->parent, this->root });
					else if constexpr (decltype(r)::kind == providers::TypeKind::TARRAY) return f(ArrayBinaryAccessor<ucsl::containers::arrays::TArray, decltype(r), ParentRefl, RootRefl, Parent, Root>{ &this->obj, r, this->parent, this->root });
					else if constexpr (decltype(r)::kind == providers::TypeKind::POINTER) return f(PointerBinaryAccessor<decltype(r), ParentRefl, RootRefl, Parent, Root>{ &this->obj, r, this->parent, this->root });
					else if constexpr (decltype(r)::kind == providers::TypeKind::CARRAY) return f(CArrayBinaryAccessor<decltype(r), ParentRefl, RootRefl, Parent, Root>{ &this->obj, r, this->parent, this->root });
					//else if constexpr (decltype(r)::kind == providers::TypeKind::UNION) return f(union(this->obj, parent, r));
					else if constexpr (decltype(r)::kind == providers::TypeKind::STRUCTURE) return f(StructureBinaryAccessor<decltype(r), ParentRefl, RootRefl, Parent, Root>{ &this->obj, r, this->parent, this->root });
					else static_assert(false, "invalid type kind");
				});
			}

			auto visit(auto f) {
				return this->refl.visit(this->parent, this->root, [&](auto r) {
					if constexpr (decltype(r)::kind == providers::TypeKind::PRIMITIVE) return f(PrimitiveBinaryAccessor<decltype(r), ParentRefl, RootRefl, Parent, Root>{ &this->obj, r, this->parent, this->root });
					else if constexpr (decltype(r)::kind == providers::TypeKind::ENUM) return f(EnumBinaryAccessor<decltype(r), ParentRefl, RootRefl, Parent, Root>{ &this->obj, r, this->parent, this->root });
					//else if constexpr (decltype(r)::kind == providers::TypeKind::FLAGS) return f(flags(this->obj, refl.is_erased(), r));
					else if constexpr (decltype(r)::kind == providers::TypeKind::ARRAY) return f(ArrayBinaryAccessor<ucsl::containers::arrays::Array, decltype(r), ParentRefl, RootRefl, Parent, Root>{ &this->obj, r, this->parent, this->root });
					else if constexpr (decltype(r)::kind == providers::TypeKind::TARRAY) return f(ArrayBinaryAccessor<ucsl::containers::arrays::TArray, decltype(r), ParentRefl, RootRefl, Parent, Root>{ &this->obj, r, this->parent, this->root });
					else if constexpr (decltype(r)::kind == providers::TypeKind::POINTER) return f(PointerBinaryAccessor<decltype(r), ParentRefl, RootRefl, Parent, Root>{ &this->obj, r, this->parent, this->root });
					else if constexpr (decltype(r)::kind == providers::TypeKind::CARRAY) return f(CArrayBinaryAccessor<decltype(r), ParentRefl, RootRefl, Parent, Root>{ &this->obj, r, this->parent, this->root });
					//else if constexpr (decltype(r)::kind == providers::TypeKind::UNION) return f(union(this->obj, parent, r));
					else if constexpr (decltype(r)::kind == providers::TypeKind::STRUCTURE) return f(StructureBinaryAccessor<decltype(r), ParentRefl, RootRefl, Parent, Root>{ &this->obj, r, this->parent, this->root });
					else static_assert(false, "invalid type kind");
				});
			}

			auto as_primitive() {
				return this->refl.visit(this->parent, this->root, [&](auto r) {
					if constexpr (decltype(r)::kind == providers::TypeKind::PRIMITIVE) { return PrimitiveBinaryAccessor<decltype(r), ParentRefl, RootRefl, Parent, Root>{ &this->obj, r, this->parent, this->root }; } else static_assert(false, "not a primitive");
				});
			}

			auto as_enum() {
				return this->refl.visit(this->parent, this->root, [&](auto r) {
					if constexpr (decltype(r)::kind == providers::TypeKind::ENUM) { return EnumBinaryAccessor<decltype(r), ParentRefl, RootRefl, Parent, Root>{ &this->obj, r, this->parent, this->root }; } else static_assert(false, "not an enum");
				});
			}

			auto as_array() {
				return this->refl.visit(this->parent, this->root, [&](auto r) {
					if constexpr (decltype(r)::kind == providers::TypeKind::ARRAY) { return PrimitiveBinaryAccessor<decltype(r), ParentRefl, RootRefl, Parent, Root>{ &this->obj, r, this->parent, this->root }; } else static_assert(false, "not a array");
				});
			}

			auto as_tarray() {
				return this->refl.visit(this->parent, this->root, [&](auto r) {
					if constexpr (decltype(r)::kind == providers::TypeKind::TARRAY) { return PrimitiveBinaryAccessor<decltype(r), ParentRefl, RootRefl, Parent, Root>{ &this->obj, r, this->parent, this->root }; } else static_assert(false, "not a tarray");
				});
			}

			auto as_carray() {
				return this->refl.visit(this->parent, this->root, [&](auto r) {
					if constexpr (decltype(r)::kind == providers::TypeKind::CARRAY) { return CArrayBinaryAccessor<decltype(r), ParentRefl, RootRefl, Parent, Root>{ &this->obj, r, this->parent, this->root }; } else static_assert(false, "not a carray");
				});
			}

			auto as_pointer() {
				return this->refl.visit(this->parent, this->root, [&](auto r) {
					if constexpr (decltype(r)::kind == providers::TypeKind::POINTER) { return PointerBinaryAccessor<decltype(r), ParentRefl, RootRefl, Parent, Root>{ &this->obj, r, this->parent, this->root }; } else static_assert(false, "not a pointer");
				});
			}

			auto as_structure() {
				return this->refl.visit(this->parent, this->root, [&](auto r) {
					if constexpr (decltype(r)::kind == providers::TypeKind::STRUCTURE) { return StructureBinaryAccessor<decltype(r), ParentRefl, RootRefl, Parent, Root>{ &this->obj, r, this->parent, this->root }; } else static_assert(false, "not a structure");
				});
			}
		};
	};
}