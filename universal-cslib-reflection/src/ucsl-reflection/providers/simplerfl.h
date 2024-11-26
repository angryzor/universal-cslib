#pragma once
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/opaque.h>
#include <ucsl-reflection/util/memory.h>
#include <vector>
#include "types.h"
#include "rflclass.h"

namespace ucsl::reflection::providers {
	using namespace ::simplerfl;

	template<typename GameInterface>
	struct simplerfl {
		template<typename T>
		struct dynamic_size_of_struct;
		template<typename Repr, strlit name, typename Base, typename... Fields>
		struct dynamic_size_of_struct<structure<Repr, name, Base, Fields...>> {
			static size_t get(const opaque_obj& self) {
				size_t offset{};

				if constexpr (!std::is_same_v<Base, void>)
					offset = dynamic_size_of<Base>(self, self);

				((
					offset = util::align(offset, dynamic_align_of<typename Fields::type>(self)),
					offset += dynamic_size_of<typename Fields::type>(self, *util::addptr(&self, offset))
					), ...);

				return util::align(offset, dynamic_align_of<structure<Repr, name, Base, Fields...>>(self));
			}
		};

		template<typename T>
		struct dynamic_align_of_struct;
		template<typename Repr, strlit name, typename Base, typename... Fields>
		struct dynamic_align_of_struct<structure<Repr, name, Base, Fields...>> {
			static size_t get() {
				size_t maxAlign{};

				if constexpr (!std::is_same_v<Base, void>)
					maxAlign = std::max(maxAlign, dynamic_align_of<Base>(*(const opaque_obj*)nullptr));

				((maxAlign = std::max(maxAlign, dynamic_align_of<typename Fields::type>(*(const opaque_obj*)nullptr))), ...);

				return maxAlign;
			}
		};

		template<typename T>
		static size_t dynamic_size_of(const opaque_obj& parent, const opaque_obj& self) {
			if constexpr (desugar_t<T>::desc_type == DESCTYPE_RFLCLASS)
				return GameInterface::RflClassNameRegistry::GetInstance()->GetClassByName(desugar_t<T>::resolver((const typename desugar_t<T>::parent&)parent))->GetSize();
			else if constexpr (desugar_t<T>::desc_type == DESCTYPE_COMPONENT_DATA)
				return GameInterface::GameObjectSystem::GetInstance()->goComponentRegistry->GetComponentInformationByName(desugar_t<T>::resolver((const typename desugar_t<T>::parent&)parent))->rflClass->GetSize();
			else if constexpr (desugar_t<T>::desc_type == DESCTYPE_SPAWNER_DATA_RFLCLASS)
				return GameInterface::GameObjectSystem::GetInstance()->gameObjectRegistry->GetGameObjectClassByName(desugar_t<T>::resolver((const typename desugar_t<T>::parent&)parent))->spawnerDataRflClass->GetSize();
			else if constexpr (desugar_t<T>::desc_type == DESCTYPE_DYNAMIC_CARRAY)
				return desugar_t<T>::resolver((const typename desugar_t<T>::parent&)parent) * dynamic_size_of<typename desugar_t<T>::type>(parent, self);
			else if constexpr (desugar_t<T>::desc_type == DESCTYPE_STATIC_CARRAY)
				return desugar_t<T>::size * dynamic_size_of<typename desugar_t<T>::type>(parent, self);
			else if constexpr (desugar_t<T>::desc_type == DESCTYPE_STRUCTURE)
				return dynamic_size_of_struct<desugar_t<T>>::get(self);
			else
				return size_of_v<T>;
		}

		template<typename T>
		static size_t dynamic_align_of(const opaque_obj& parent) {
			if constexpr (is_realigned_v<T>)
				return align_of_v<T>;
			else if constexpr (desugar_t<T>::desc_type == DESCTYPE_RFLCLASS)
				return GameInterface::RflClassNameRegistry::GetInstance()->GetClassByName(desugar_t<T>::resolver((const typename desugar_t<T>::parent&)parent))->GetAlignment();
			else if constexpr (desugar_t<T>::desc_type == DESCTYPE_COMPONENT_DATA)
				return GameInterface::GameObjectSystem::GetInstance()->goComponentRegistry->GetComponentInformationByName(desugar_t<T>::resolver((const typename desugar_t<T>::parent&)parent))->rflClass->GetAlignment();
			else if constexpr (desugar_t<T>::desc_type == DESCTYPE_SPAWNER_DATA_RFLCLASS)
				return GameInterface::GameObjectSystem::GetInstance()->gameObjectRegistry->GetGameObjectClassByName(desugar_t<T>::resolver((const typename desugar_t<T>::parent&)parent))->spawnerDataRflClass->GetAlignment();
			else if constexpr (desugar_t<T>::desc_type == DESCTYPE_DYNAMIC_CARRAY)
				return dynamic_align_of<typename desugar_t<T>::type>(parent);
			else if constexpr (desugar_t<T>::desc_type == DESCTYPE_STATIC_CARRAY)
				return dynamic_align_of<typename desugar_t<T>::type>(parent);
			else if constexpr (desugar_t<T>::desc_type == DESCTYPE_STRUCTURE)
				return dynamic_align_of_struct<desugar_t<T>>::get();
			else
				return align_of_v<T>;
		}

		struct EnumMember {
			const long long index{};
			const char* name{};

			long long GetIndex() const { return index; }
			const char* const GetEnglishName() const { return name; }
		};

		template<typename T> struct get_enum_member;
		template<strlit name> struct get_enum_member<option<name>> {
			static constexpr EnumMember call(long long& counter) { return { counter++, name }; }
		};
		template<strlit name, long long value> struct get_enum_member<fixed_option<name, value>> {
			static constexpr EnumMember call(long long& counter) { counter = value + 1; return { value, name }; }
		};

		template<typename... Options>
		consteval static std::vector<EnumMember> get_enum_members(std::tuple<Options...>) {
			long long counter{};

			return { get_enum_member<Options>::call(counter)... };
		}


		template<typename T>
		struct Primitive {
			constexpr static TypeKind kind = TypeKind::PRIMITIVE;
			template<typename F>
			constexpr auto visit(F f) const { return f(PrimitiveData<typename T::repr>{}); }
		};

		template<typename T>
		struct Constant {
			constexpr static TypeKind kind = TypeKind::PRIMITIVE;
			template<typename F>
			constexpr auto visit(F f) const { return f(PrimitiveData<typename T::repr>{ .constant_value = T::value }); }
		};

		template<typename T>
		struct Type;

		template<typename T>
		struct Enum {
			constexpr static TypeKind kind = TypeKind::ENUM;
			constexpr auto get_underlying_primitive() const { return Primitive<primitive<typename T::underlying>>{}; }
			constexpr std::vector<EnumMember> get_options() const { return get_enum_members(typename T::options{}); }
		};

		//struct Flags {
		//	auto get_flags();
		//};

		template<typename T>
		struct Pointer {
			constexpr static TypeKind kind = TypeKind::POINTER;
			constexpr auto get_target_type() const { return Type<typename T::target>{}; }
		};

		template<typename T>
		struct Array {
			constexpr static TypeKind kind = TypeKind::ARRAY;
			constexpr auto get_item_type() const { return Type<typename T::type>{}; }
			constexpr auto get_accessor(opaque_obj& obj) const { return OpaqueArray<representation_t<T>, GameInterface>{ (representation_t<T>&)obj }; }
		};

		template<typename T>
		struct TArray {
			constexpr static TypeKind kind = TypeKind::TARRAY;
			constexpr auto get_item_type() const { return Type<typename T::type>{}; }
			constexpr auto get_accessor(opaque_obj& obj) const { return OpaqueArray<representation_t<T>, GameInterface>{ (representation_t<T>&)obj }; }
		};

		template<typename T>
		struct StaticCArray {
			constexpr static TypeKind kind = TypeKind::CARRAY;
			constexpr auto get_item_type() const { return Type<typename T::type>{}; }
			constexpr size_t get_length(opaque_obj& parent) const { return T::size; }
		};

		template<typename T>
		struct DynamicCArray {
			constexpr static TypeKind kind = TypeKind::CARRAY;
			constexpr auto get_item_type() const { return Type<typename T::type>{}; }
			size_t get_length(const opaque_obj& parent) const { return T::resolver((typename T::parent&)parent); }
		};

		template<typename T>
		struct Field {
			size_t offset{};

			constexpr const char* get_name() const { return T::name; }
			constexpr size_t get_offset() const { return offset; }
			constexpr auto get_type() const { return Type<typename T::type>{}; }
		};

		template<typename T>
		struct Union {
			constexpr static TypeKind kind = TypeKind::UNION;
			template<typename F>
			void visit_fields(F f) const { _visit_fields(f, typename T::fields{}); }

			template<typename F>
			void visit_current_field(const opaque_obj& parent, F f) const { _visit_current_field(parent, f, typename T::fields{}, std::make_index_sequence<std::tuple_size_v<typename T::fields>>{}); }

		private:
			template<typename F, typename... Fields>
			void _visit_fields(F f, std::tuple<Fields...>) const {
				(f(Field<Fields>{ 0 }), ...);
			}

			template<typename F, typename Fields, size_t... Is>
			void _visit_current_field(const opaque_obj& parent, F f, Fields, std::index_sequence<Is...>) const {
				size_t idx = T::resolver((typename T::parent&)parent);

				((idx == Is ? (f(Field<std::tuple_element_t<Is, Fields>>{ 0 }), true) : false) || ...);
			}
		};

		struct EmptyStruct {
			constexpr static TypeKind kind = TypeKind::STRUCTURE;
			constexpr const char* get_name() const { return nullptr; }
			constexpr auto get_base() const { return std::optional<EmptyStruct>{}; }
			template<typename F> void visit_fields(const opaque_obj& obj, F f) const {}
		};

		template<typename T>
		struct Structure {
			using Base = typename T::base;
			using Fields = typename T::fields;

			constexpr static TypeKind kind = TypeKind::STRUCTURE;
			constexpr const char* get_name() const { return T::name; }
			constexpr auto get_base() const {
				if constexpr (!std::is_same_v<Base, primitive<void>>)
					return std::make_optional(Structure<Base>{});
				else
					return std::optional<EmptyStruct>{};
			}

			template<typename F>
			void visit_fields(const opaque_obj& obj, F f) const { _visit_fields(obj, f, Fields{}); }

		private:
			template<typename F, typename... Fields>
			void _visit_fields(const opaque_obj& obj, F f, std::tuple<Fields...>) const {
				size_t offset{};
				size_t thisOffset{};

				if constexpr (!std::is_same_v<Base, primitive<void>>)
					offset = size_of_v<Base>;

				(f((
					offset = thisOffset = util::align(offset, dynamic_align_of<typename Fields::type>(obj)),
					offset += dynamic_size_of<typename Fields::type>(obj, *util::addptr(&obj, thisOffset)),
					Field<Fields>{ thisOffset }
				)), ...);
			}
		};

		template<typename T>
		struct Type {
			size_t get_size(const opaque_obj& parent, const opaque_obj& obj) const { return dynamic_size_of<T>(parent, obj); }
			size_t get_alignment(const opaque_obj& parent) const { return dynamic_align_of<T>(parent); }

			constexpr bool is_erased() const { return ucsl::reflection::is_erased_v<T>; }

			template<typename F>
			constexpr auto visit(const opaque_obj& parent, F f) const {
				if constexpr (desugar_t<T>::desc_type == DESCTYPE_PRIMITIVE) return f(Primitive<desugar_t<T>>{});
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_CONSTANT) return f(Constant<desugar_t<T>>{});
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_ENUMERATION) return f(Enum<desugar_t<T>>{});
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_POINTER) return f(Pointer<desugar_t<T>>{});
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_ARRAY) return f(Array<desugar_t<T>>{});
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_TARRAY) return f(TArray<desugar_t<T>>{});
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_DYNAMIC_CARRAY) return f(DynamicCArray<desugar_t<T>>{});
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_STATIC_CARRAY) return f(StaticCArray<desugar_t<T>>{});
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_UNION) return f(Union<desugar_t<T>>{});
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_STRUCTURE) return f(Structure<desugar_t<T>>{});
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_RFLCLASS)
					return f(typename rflclass<GameInterface>::Structure{ GameInterface::RflClassNameRegistry::GetInstance()->GetClassByName(desugar_t<T>::resolver((typename desugar_t<T>::parent&)parent)) });
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_COMPONENT_DATA)
					return f(typename rflclass<GameInterface>::Structure{ GameInterface::GameObjectSystem::GetInstance()->goComponentRegistry->GetComponentInformationByName(desugar_t<T>::resolver((const typename desugar_t<T>::parent&)parent))->rflClass });
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_SPAWNER_DATA_RFLCLASS)
					return f(typename rflclass<GameInterface>::Structure{ GameInterface::GameObjectSystem::GetInstance()->gameObjectRegistry->GetGameObjectClassByName(desugar_t<T>::resolver((const typename desugar_t<T>::parent&)parent))->spawnerDataRflClass});
				else static_assert("invalid desc type");
			}
		};

		template<typename T>
		static constexpr Type<canonical_t<T>> reflect() { return {}; }

		template<typename T>
		static constexpr Type<canonical_t<T>> reflect(const T&) { return {}; }
	};
}