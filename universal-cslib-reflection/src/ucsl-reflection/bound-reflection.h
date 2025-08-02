#pragma once
#include <ucsl-reflection/providers/types.h>
#include <ucsl-reflection/accessors/types.h>

namespace ucsl::reflection::providers {
	struct BoundProvider {
		template<typename Refl, accessors::StructureAccessor Parent, accessors::StructureAccessor Root>
		struct ReflectionBase {
			Refl refl;
			Parent parent;
			Root root;

			ReflectionBase(Refl refl, Parent parent, Root root) : refl{ refl }, parent{ parent }, root{ root } {}
		};
		
		template<typename Refl, accessors::StructureAccessor Parent, accessors::StructureAccessor Root>
		struct Type;

		template<typename Refl, accessors::StructureAccessor Parent, accessors::StructureAccessor Root>
		struct Primitive : public ReflectionBase<Refl, Parent, Root> {
			constexpr static TypeKind kind = TypeKind::PRIMITIVE;

			using ReflectionBase<Refl, Parent, Root>::ReflectionBase;

			template<typename F> constexpr auto visit(F&& f) const { return this->refl.visit(std::forward<F>(f)); }
		};

		template<typename Refl, accessors::StructureAccessor Parent, accessors::StructureAccessor Root>
		struct Enum : public ReflectionBase<Refl, Parent, Root> {
			constexpr static TypeKind kind = TypeKind::ENUM;

			using ReflectionBase<Refl, Parent, Root>::ReflectionBase;

			constexpr auto get_options() const { return this->refl.get_options(); }
			template<typename F> constexpr auto visit(F&& f) const { return this->refl.visit(std::forward<F>(f)); }
		};

		//struct Flags {
		//	auto get_flags();
		//};

		template<typename Refl, accessors::StructureAccessor Parent, accessors::StructureAccessor Root>
		struct Pointer : public ReflectionBase<Refl, Parent, Root> {
			constexpr static TypeKind kind = TypeKind::POINTER;

			using ReflectionBase<Refl, Parent, Root>::ReflectionBase;

			constexpr auto get_target_type() const { return Type{ this->refl.get_target_type(), this->parent, this->root }; }
			constexpr bool is_weak() const { return this->refl.is_weak(); }
		};

		template<typename Refl, accessors::StructureAccessor Parent, accessors::StructureAccessor Root>
		struct Array : public ReflectionBase<Refl, Parent, Root> {
			constexpr static TypeKind kind = TypeKind::ARRAY;

			using ReflectionBase<Refl, Parent, Root>::ReflectionBase;

			constexpr auto get_item_type() const { return Type{ this->refl.get_item_type(), this->parent, this->root }; }
		};

		template<typename Refl, accessors::StructureAccessor Parent, accessors::StructureAccessor Root>
		struct TArray : public ReflectionBase<Refl, Parent, Root> {
			constexpr static TypeKind kind = TypeKind::TARRAY;

			using ReflectionBase<Refl, Parent, Root>::ReflectionBase;

			constexpr auto get_item_type() const { return Type{ this->refl.get_item_type(), this->parent, this->root }; }
		};

		template<typename Refl, accessors::StructureAccessor Parent, accessors::StructureAccessor Root>
		struct CArray : public ReflectionBase<Refl, Parent, Root> {
			constexpr static TypeKind kind = TypeKind::CARRAY;

			using ReflectionBase<Refl, Parent, Root>::ReflectionBase;

			constexpr auto get_item_type() const { return Type{ this->refl.get_item_type(), this->parent, this->root }; }
			constexpr size_t get_length() const { return this->refl.get_length(this->parent); }
		};

		template<typename Refl, accessors::StructureAccessor Parent, accessors::StructureAccessor Root>
		struct Field : public ReflectionBase<Refl, Parent, Root> {
			using ReflectionBase<Refl, Parent, Root>::ReflectionBase;

			size_t offset{};

			constexpr const char* get_name() const { return this->refl.get_name(); }
			constexpr size_t get_offset() const { return this->refl.get_offset(); }
			constexpr auto get_type(auto new_parent) const { return Type<decltype(this->refl.get_type()), decltype(new_parent), Root>{ this->refl.get_type(), new_parent, this->root }; }
		};

		template<typename Refl, accessors::StructureAccessor Parent, accessors::StructureAccessor Root>
		struct Union : public ReflectionBase<Refl, Parent, Root> {
			constexpr static TypeKind kind = TypeKind::UNION;

			using ReflectionBase<Refl, Parent, Root>::ReflectionBase;

			template<typename F> constexpr void visit_fields(F&& f) { this->refl.visit_fields([&](auto r) { f(Field{ r, this->parent, this->root }); }); }
			template<typename F> constexpr void visit_current_field(F f) { this->refl.visit_current_field([&](auto r) { f(Field{ r, this->parent, this->root }); }); }
		};

		template<typename Refl, accessors::StructureAccessor Parent, accessors::StructureAccessor Root>
		struct Structure : public ReflectionBase<Refl, Parent, Root> {
			constexpr static TypeKind kind = TypeKind::STRUCTURE;

			using ReflectionBase<Refl, Parent, Root>::ReflectionBase;

			//// TODO: Refactor this. I'm doing this for RFL DnD. I may prefer to pass refl structs into traversal algos,
			//// but don't know how to send those around in ImGui DnD data.
			//const GameInterface::RflSystem::RflClass* rflClass{}; // Not working atm.

			constexpr const char* get_name() const { return this->refl.get_name(); }
			constexpr size_t get_size(auto obj) const { return this->refl.get_size(this->parent, this->root, obj); }
			constexpr size_t get_alignment() const { return this->refl.get_alignment(this->parent, this->root); }
			constexpr auto get_base() const {
				auto base = this->refl.get_base();

				return base.has_value() ? std::make_optional(Structure<decltype(base.value()), Parent, Root>{ base.value(), this->parent, this->root }) : std::nullopt;
			}
			template<strlit field_name> constexpr auto get_field() const { return Field{ this->refl.get_field<field_name>(), this->parent, this->root }; }
			template<typename F> constexpr void visit_fields(F&& f) { this->refl.visit_fields([&](auto r) { f(Field{ r, this->parent, this->root }); }); }
		};

		template<typename Refl, accessors::StructureAccessor Parent, accessors::StructureAccessor Root>
		struct Type : public ReflectionBase<Refl, Parent, Root> {
			using ReflectionBase<Refl, Parent, Root>::ReflectionBase;

			constexpr size_t get_size(auto obj) const { return this->refl.get_size(this->parent, this->root, obj); }
			constexpr size_t get_alignment() const { return this->refl.get_alignment(this->parent, this->root); }
			template<typename F> constexpr auto visit(F f) const {
				return this->refl.visit(this->parent, this->root, [&](auto r){
					if constexpr (r.kind == TypeKind::PRIMITIVE) return f(Primitive{ r, this->parent, this->root });
					else if constexpr (r.kind == TypeKind::ENUM) return f(Enum{ r, this->parent, this->root });
					else if constexpr (r.kind == TypeKind::POINTER) return f(Pointer{ r, this->parent, this->root });
					else if constexpr (r.kind == TypeKind::ARRAY) return f(Array{ r, this->parent, this->root });
					else if constexpr (r.kind == TypeKind::TARRAY) return f(TArray{ r, this->parent, this->root });
					else if constexpr (r.kind == TypeKind::CARRAY) return f(CArray{ r, this->parent, this->root });
					else if constexpr (r.kind == TypeKind::UNION) return f(Union{ r, this->parent, this->root });
					else if constexpr (r.kind == TypeKind::STRUCTURE) return f(Structure{ r, this->parent, this->root });
					else static_assert("invalid desc type");
				});
			}
		};

		struct NullValueAccessor {
			const auto visit(auto f) const { return f(4); }
			auto visit(auto f) { return f(4); }
		};

		struct NullStructureAccessor {
			template<typename FieldRefl> inline auto operator[](const FieldRefl& field_refl) { return NullValueAccessor{}; }
			template<typename FieldRefl> inline const auto operator[](const FieldRefl& field_refl) const { return NullValueAccessor{}; }
		};

		template<typename Refl>
		struct RootType : public Type<Refl, NullStructureAccessor, NullStructureAccessor> {
			RootType(Refl refl) : Type<Refl, NullStructureAccessor, NullStructureAccessor>{ refl, {}, {} } {}
		};
	};
}
