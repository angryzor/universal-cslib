#pragma once
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/accessors/types.h>
#include <ucsl-reflection/opaque.h>
#include <ucsl-reflection/util/memory.h>
#include <vector>
#include <algorithm>
#include "types.h"
#include "rflclass.h"

namespace ucsl::reflection::providers {
	using namespace ::simplerfl;

	template<typename GameInterface>
	struct simplerfl {
		struct NullValueAccessor {
			const auto visit(auto f) const { return f(4); }
			auto visit(auto f) { return f(4); }
		};

		struct NullStructureAccessor {
			template<typename FieldRefl> inline auto operator[](const FieldRefl& field_refl) { return NullValueAccessor{}; }
			template<typename FieldRefl> inline const auto operator[](const FieldRefl& field_refl) const { return NullValueAccessor{}; }
		};

		struct EnumMember {
			const long long index{};
			const char* name{};

			long long GetIndex() const { return index; }
			const char* const GetEnglishName() const { return name; }
		};

		template<typename T> struct get_enum_member;
		template<strlit name> struct get_enum_member<option<name>> {
			constexpr static EnumMember call(long long& counter) { return { counter++, name }; }
		};
		template<strlit name, long long value> struct get_enum_member<fixed_option<name, value>> {
			constexpr static EnumMember call(long long& counter) { counter = value + 1; return { value, name }; }
		};

		template<typename... Options>
		consteval static auto get_enum_members(std::tuple<Options...>) {
			long long counter{};

			return std::array{ get_enum_member<Options>::call(counter)... };
		}

		template<accessors::StructureAccessor Parent, accessors::StructureAccessor Root>
		struct ReflectionBase {
			Parent parent;
			Root root;

			constexpr ReflectionBase(const Parent& parent, const Root& root) : parent{ parent }, root{ root } {}
		};

		template<typename T, accessors::StructureAccessor Parent, accessors::StructureAccessor Root>
		struct Type;

		template<typename T, bool erased>
		struct Primitive {
			constexpr static TypeKind kind = TypeKind::PRIMITIVE;
			template<typename F>
			constexpr static auto visit(F f) { return f(PrimitiveData<typename T::repr>{ .is_erased = erased }); }
		};

		template<typename T, bool erased>
		struct Constant {
			constexpr static TypeKind kind = TypeKind::PRIMITIVE;
			template<typename F>
			constexpr static auto visit(F f) { return f(PrimitiveData<typename T::repr>{ .is_erased = erased, .constant_value = T::value }); }
		};

		template<typename T, bool erased>
		struct Enum {
			constexpr static TypeKind kind = TypeKind::ENUM;
			constexpr static auto get_options() { return get_enum_members(typename T::options{}); }
			template<typename F>
			constexpr static auto visit(F f) { return f(PrimitiveData<typename T::underlying>{ .is_erased = erased }); }
		};

		template<typename T, accessors::StructureAccessor Parent, accessors::StructureAccessor Root>
		struct BitfieldComponent : ReflectionBase<Parent, Root> {
			using ReflectionBase<Parent, Root>::ReflectionBase;

			constexpr static const char* get_name() { return T::name; }
			constexpr static size_t get_offset() { return T::offset; }
			constexpr static size_t get_width() { return T::width; }
			constexpr auto get_type() const { return Type<typename T::type, Parent, Root>{ this->parent, this->root }; }
		};

		template<typename T, accessors::StructureAccessor Parent, accessors::StructureAccessor Root, bool erased>
		struct Bitfield : ReflectionBase<Parent, Root> {
			using ReflectionBase<Parent, Root>::ReflectionBase;

			using Components = typename T::components;

			constexpr static TypeKind kind = TypeKind::BITFIELD;
			constexpr static auto visit_underlying(auto f) { return f(PrimitiveData<typename T::underlying>{ .is_erased = erased }); }

			template<strlit component_name>
			constexpr auto get_component() const {
				return _get_component<component_name>(Components{});
			}

			constexpr void visit_components(auto f) const {
				_visit_components(f, Components{});
			}

		private:
			template<strlit component_name, typename... Components>
			constexpr auto _get_component(std::tuple<Components...>) const {
				return BitfieldComponent<find_named_type_t<component_name, Components...>, Parent, Root>{ this->parent, this->root };
			}

			template<typename... Components>
			constexpr void _visit_components(auto f, std::tuple<Components...>) const {
				(f(BitfieldComponent<Components, Parent, Root>{ this->parent, this->root }), ...);
			}
		};

		template<typename T, accessors::StructureAccessor Parent, accessors::StructureAccessor Root, bool weak>
		struct Pointer : ReflectionBase<Parent, Root> {
			using ReflectionBase<Parent, Root>::ReflectionBase;

			constexpr static TypeKind kind = TypeKind::POINTER;
			constexpr auto get_target_type() const { return Type<typename T::target, Parent, Root>{ this->parent, this->root }; }
			constexpr static bool is_weak() { return weak; }
		};

		//template<typename T>
		//struct Offset {
		//	constexpr static TypeKind kind = TypeKind::OFFSET;
		//	constexpr static auto get_target_type() { return Type<typename T::target>{}; }
		//	template<typename F>
		//	constexpr static auto visit(F f) { return f(PrimitiveData<typename T::repr>{}); }
		//};

		template<typename T, accessors::StructureAccessor Parent, accessors::StructureAccessor Root>
		struct Array : ReflectionBase<Parent, Root> {
			using ReflectionBase<Parent, Root>::ReflectionBase;

			constexpr static TypeKind kind = TypeKind::ARRAY;
			constexpr auto get_item_type() const { return Type<typename T::type, Parent, Root>{ this->parent, this->root }; }
			//constexpr static auto get_accessor(auto& obj) { return OpaqueArray<representation_t<T>, GameInterface>{ (representation_t<T>&)obj }; }
		};

		template<typename T, accessors::StructureAccessor Parent, accessors::StructureAccessor Root>
		struct TArray : ReflectionBase<Parent, Root> {
			using ReflectionBase<Parent, Root>::ReflectionBase;

			constexpr static TypeKind kind = TypeKind::TARRAY;
			constexpr auto get_item_type() const { return Type<typename T::type, Parent, Root>{ this->parent, this->root }; }
			//constexpr static auto get_accessor(auto& obj) { return OpaqueArray<representation_t<T>, GameInterface>{ (representation_t<T>&)obj }; }
		};

		template<typename T, accessors::StructureAccessor Parent, accessors::StructureAccessor Root>
		struct StaticCArray : ReflectionBase<Parent, Root> {
			using ReflectionBase<Parent, Root>::ReflectionBase;

			constexpr static TypeKind kind = TypeKind::CARRAY;
			constexpr auto get_item_type() const { return Type<typename T::type, Parent, Root>{ this->parent, this->root }; }
			constexpr static size_t get_length() { return T::size; }
		};

		template<typename T, accessors::StructureAccessor Parent, accessors::StructureAccessor Root>
		struct DynamicCArray : ReflectionBase<Parent, Root> {
			using ReflectionBase<Parent, Root>::ReflectionBase;

			constexpr static TypeKind kind = TypeKind::CARRAY;
			constexpr auto get_item_type() const { return Type<typename T::type, Parent, Root>{ this->parent, this->root }; }
			constexpr size_t get_length() const { return resolve<typename T::resolver>(this->parent, this->root); }
		};

		template<typename T, accessors::StructureAccessor Parent, accessors::StructureAccessor Root>
		struct Field : ReflectionBase<Parent, Root> {
			size_t offset{};

			constexpr Field(const Parent& parent, const Root& root, size_t offset) : ReflectionBase<Parent, Root>{ parent, root }, offset{ offset } {}

			constexpr static const char* get_name() { return T::name; }
			constexpr size_t get_offset() const { return offset; }
			constexpr auto get_type() const { return Type<typename T::type, Parent, Root>{ this->parent, this->root }; }
			constexpr auto get_type(accessors::StructureAccessor auto new_parent) const {
				if constexpr (std::is_same_v<Root, NullStructureAccessor>)
					return Type<typename T::type, decltype(new_parent), decltype(new_parent)>{ new_parent, new_parent };
				else
					return Type<typename T::type, decltype(new_parent), Root>{ new_parent, this->root };
			}
		};

		template<typename T, accessors::StructureAccessor Parent, accessors::StructureAccessor Root>
		struct Union : ReflectionBase<Parent, Root> {
			using ReflectionBase<Parent, Root>::ReflectionBase;

			constexpr static TypeKind kind = TypeKind::UNION;
			template<typename AddrType> constexpr size_t get_size(const accessors::UnionAccessor auto& obj) const { return _get_size<AddrType>(obj, typename T::fields{}); }
			template<typename AddrType> constexpr size_t get_alignment(const accessors::UnionAccessor auto& obj) const { return _get_alignment<AddrType>(obj, typename T::fields{}); }

			template<typename F>
			constexpr void visit_fields(F f) const { _visit_fields(f, typename T::fields{}); }

			template<typename F>
			constexpr void visit_current_field(F f) const { _visit_current_field(f, typename T::fields{}, std::make_index_sequence<std::tuple_size_v<typename T::fields>>{}); }

		private:
			template<typename AddrType, typename... Fields> constexpr size_t _get_size(const accessors::UnionAccessor auto& obj, std::tuple<Fields...>) const {
				size_t maxSize{};

				((maxSize = std::max(maxSize, Field<Fields, Parent, Root>{ this->parent, this->root, 0 }.get_type(obj).template get_size<AddrType>(obj_[Field<Fields, Parent, Root>{ this->parent, this->root, 0 }]))), ...);

				return util::align(maxSize, get_alignment(obj));
			}

			template<typename AddrType, typename... Fields> constexpr size_t _get_alignment(const accessors::UnionAccessor auto& obj, std::tuple<Fields...>) const {
				size_t maxAlign{};

				((maxAlign = std::max(maxAlign, Field<Fields, Parent, Root>{ this->parent, this->root, 0 }.get_type(obj).template get_alignment<AddrType>(obj_[Field<Fields, Parent, Root>{ this->parent, this->root, 0 }]))), ...);

				return maxAlign;
			}

			template<typename F, typename... Fields>
			constexpr void _visit_fields(F f, std::tuple<Fields...>) const {
				(f(Field<Fields, Parent, Root>{ this->parent, this->root, 0 }), ...);
			}

			template<typename F, typename Fields, size_t... Is>
			constexpr void _visit_current_field(F f, Fields, std::index_sequence<Is...>) const {
				size_t idx = resolve<typename desugar_t<T>::resolver>(this->parent, this->root);

				((idx == Is ? (f(Field<std::tuple_element_t<Is, Fields>, Parent, Root>{ this->parent, this->root, 0 }), true) : false) || ...);
			}
		};

		struct EmptyStruct {
			constexpr static TypeKind kind = TypeKind::STRUCTURE;
			constexpr static const char* get_name() { return nullptr; }
			constexpr static auto get_base() { return std::optional<EmptyStruct>{}; }
			constexpr static size_t get_size(const auto& obj) { return 0; }
			constexpr static size_t get_alignment(const auto& obj) { return 0; }
			template<typename AddrType, typename F> constexpr static void visit_fields(const auto& obj, F f) {}
		};

		template<typename T, accessors::StructureAccessor Parent, accessors::StructureAccessor Root>
		struct Structure : ReflectionBase<Parent, Root> {
			using ReflectionBase<Parent, Root>::ReflectionBase;

			using Base = typename T::base;
			using Fields = typename T::fields;

			// TODO: Refactor this. I'm doing this for RFL DnD. I may prefer to pass refl structs into traversal algos,
			// but don't know how to send those around in ImGui DnD data.
			const GameInterface::RflSystem::RflClass* rflClass;

			constexpr static TypeKind kind = TypeKind::STRUCTURE;
			constexpr static const char* get_name() { return T::name; }
			template<typename AddrType> constexpr size_t get_size(const accessors::StructureAccessor auto& obj) const { return _get_size<AddrType>(obj, Fields{}); }
			template<typename AddrType> constexpr size_t get_alignment(const accessors::StructureAccessor auto& obj) const { return _get_alignment<AddrType>(obj, Fields{}); }
			constexpr static auto get_base() {
				if constexpr (!std::is_same_v<Base, primitive<void>>)
					return std::make_optional(Structure<Base, Parent, Root>{});
				else
					return std::optional<EmptyStruct>{};
			}

			template<size_t index, typename AddrType>
			constexpr auto get_field_by_index(const accessors::StructureAccessor auto& obj) const {
				return _get_field_by_index<index, AddrType>(obj, Fields{});
			}

			template<strlit field_name, typename AddrType>
			constexpr auto get_field(const accessors::StructureAccessor auto& obj) const {
				return _get_field<field_name, AddrType>(obj, Fields{});
			}

			template<typename AddrType, typename F>
			constexpr void visit_fields(const accessors::StructureAccessor auto& obj, F f) const { _visit_fields<AddrType>(obj, f, Fields{}); }

		private:
			template<typename AddrType, typename... Fields> constexpr size_t _get_size(const accessors::StructureAccessor auto& obj, std::tuple<Fields...>) const {
				size_t offset{};

				if constexpr (constexpr auto base = get_base())
					offset = base.value().template get_size<AddrType>(obj);

				((
					offset = util::align(offset, Field<Fields, Parent, Root>{ this->parent, this->root, offset }.get_type(obj).template get_alignment<AddrType>(obj[Field<Fields, Parent, Root>{ this->parent, this->root, offset }])),
					offset += Field<Fields, Parent, Root>{ this->parent, this->root, offset }.get_type(obj).template get_size<AddrType>(obj[Field<Fields, Parent, Root>{ this->parent, this->root, offset }])
					), ...);

				return util::align(offset, get_alignment<AddrType>(obj));
			}

			template<typename AddrType, typename... Fields> constexpr size_t _get_alignment(const accessors::StructureAccessor auto& obj, std::tuple<Fields...>) const {
				size_t alignment{};
				size_t offset{};
				size_t maxAlign{};

				if constexpr (constexpr auto base = get_base())
					maxAlign = base.value().template get_alignment<AddrType>(obj);

				((
					alignment = Field<Fields, Parent, Root>{ this->parent, this->root, offset }.get_type(obj).template get_alignment<AddrType>(obj[Field<Fields, Parent, Root>{ this->parent, this->root, offset }]),
					offset = util::align(offset, alignment),
					maxAlign = std::max(maxAlign, alignment),
					offset += Field<Fields, Parent, Root>{ this->parent, this->root, offset }.get_type(obj).template get_size<AddrType>(obj[Field<Fields, Parent, Root>{ this->parent, this->root, offset }])
				), ...);

				return maxAlign;
			}

			template<size_t index, typename AddrType, typename... Fields>
			constexpr auto _get_field_by_index(const accessors::StructureAccessor auto& obj, std::tuple<Fields...>) const {
				using F = std::tuple_element_t<index, std::tuple<Fields...>>;

				size_t offset{};
				size_t thisOffset{};

				if constexpr (constexpr auto base = get_base())
					offset = base.value().template get_size<AddrType>(obj);

				((
					offset = thisOffset = util::align(offset, Field<Fields, Parent, Root>{ this->parent, this->root, offset }.get_type(obj).template get_alignment<AddrType>(obj[Field<Fields, Parent, Root>{ this->parent, this->root, offset }])),
					offset += Field<Fields, Parent, Root>{ this->parent, this->root, offset }.get_type(obj).template get_size<AddrType>(obj[Field<Fields, Parent, Root>{ this->parent, this->root, offset }]),
					!std::is_same_v<F, Fields>
				) && ...);

				return Field<F, Parent, Root>{ this->parent, this->root, thisOffset };
			}

			template<strlit field_name, typename AddrType, typename... Fields>
			constexpr auto _get_field(const accessors::StructureAccessor auto& obj, std::tuple<Fields...>) const {
				using F = find_field_t<field_name, T>;

				size_t offset{};
				size_t thisOffset{};

				if constexpr (constexpr auto base = get_base())
					offset = base.value().template get_size<AddrType>(obj);

				((
					offset = thisOffset = util::align(offset, Field<Fields, Parent, Root>{ this->parent, this->root, offset }.get_type(obj).template get_alignment<AddrType>(obj[Field<Fields, Parent, Root>{ this->parent, this->root, offset }])),
					offset += Field<Fields, Parent, Root>{ this->parent, this->root, offset }.get_type(obj).template get_size<AddrType>(obj[Field<Fields, Parent, Root>{ this->parent, this->root, offset }]),
					!std::is_same_v<F, Fields>
				) && ...);

				return Field<F, Parent, Root>{ this->parent, this->root, thisOffset };
			}

			template<typename AddrType, typename F, typename... Fields>
			constexpr void _visit_fields(const accessors::StructureAccessor auto& obj, F f, std::tuple<Fields...>) const {
				size_t offset{};
				size_t thisOffset{};

				if constexpr (constexpr auto base = get_base())
					offset = base.value().template get_size<AddrType>(obj);

				(f((
					offset = thisOffset = util::align(offset, Field<Fields, Parent, Root>{ this->parent, this->root, offset }.get_type(obj).template get_alignment<AddrType>(obj[Field<Fields, Parent, Root>{ this->parent, this->root, offset }])),
					offset += Field<Fields, Parent, Root>{ this->parent, this->root, offset }.get_type(obj).template get_size<AddrType>(obj[Field<Fields, Parent, Root>{ this->parent, this->root, offset }]),
					Field<Fields, Parent, Root>{ this->parent, this->root, thisOffset }
				)), ...);
			}
		};

		template<typename T, accessors::StructureAccessor Parent, accessors::StructureAccessor Root>
		struct Type : ReflectionBase<Parent, Root> {
			using ReflectionBase<Parent, Root>::ReflectionBase;

			template<typename AddrType> constexpr size_t get_size(const accessors::ValueAccessor auto& obj) const {
				if constexpr (desugar_t<T>::desc_type == DESCTYPE_PRIMITIVE) {
					if constexpr (std::is_same_v<typename desugar_t<T>::repr, const char*>) return sizeof(AddrType);
					else if constexpr (std::is_same_v<typename desugar_t<T>::repr, ucsl::strings::VariableString>) return sizeof(AddrType) * 2;
					else return sizeof(typename desugar_t<T>::repr);
				}
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_CONSTANT) return sizeof(typename desugar_t<T>::repr);
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_ENUMERATION) return sizeof(typename desugar_t<T>::repr);
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_BITFIELD) return sizeof(typename desugar_t<T>::underlying);
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_POINTER) return sizeof(AddrType);
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_ARRAY) return sizeof(AddrType) * 4;
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_TARRAY) return sizeof(AddrType) * 3;
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_DYNAMIC_CARRAY) return resolve<typename desugar_t<T>::resolver>(this->parent, this->root) == 0 ? 0 : resolve<typename desugar_t<T>::resolver>(this->parent, this->root) * DynamicCArray<desugar_t<T>, Parent, Root>{ this->parent, this->root }.get_item_type().template get_size<AddrType>(obj.as_carray()[0]);
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_STATIC_CARRAY) return desugar_t<T>::size * StaticCArray<desugar_t<T>, Parent, Root>{ this->parent, this->root }.get_item_type().template get_size<AddrType>(obj.as_carray()[0]);
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_UNION) return Union<desugar_t<T>, Parent, Root>{ this->parent, this->root }.template get_size<AddrType>(obj.as_union());
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_STRUCTURE) return Structure<desugar_t<T>, Parent, Root>{ this->parent, this->root }.template get_size<AddrType>(obj.as_structure());
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_RFLCLASS) return typename rflclass<GameInterface>::Structure{ GameInterface::RflClassNameRegistry::GetInstance()->GetClassByName(resolve<typename desugar_t<T>::resolver>(this->parent, this->root).c_str()) }.template get_size<AddrType>(obj.as_structure());
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_COMPONENT_DATA) return typename rflclass<GameInterface>::Structure{ GameInterface::GameObjectSystem::GetInstance()->goComponentRegistry->GetComponentInformationByName(resolve<typename desugar_t<T>::resolver>(this->parent, this->root).c_str())->GetSpawnerDataClass() }.template get_size<AddrType>(obj.as_structure());
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_SPAWNER_DATA_RFLCLASS) return typename rflclass<GameInterface>::Structure{ GameInterface::GameObjectSystem::GetInstance()->gameObjectRegistry->GetGameObjectClassByName(resolve<typename desugar_t<T>::resolver>(this->parent, this->root).c_str())->GetSpawnerDataClass() }.template get_size<AddrType>(obj.as_structure());
				else static_assert(false, "getting size of unknown type");
			}
			template<typename AddrType> constexpr size_t get_alignment(const accessors::ValueAccessor auto& obj) const {
				if constexpr (is_realigned_v<T>) return align_of_v<T>;
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_PRIMITIVE) {
					if constexpr (std::is_same_v<typename desugar_t<T>::repr, const char*>) return alignof(AddrType);
					else if constexpr (std::is_same_v<typename desugar_t<T>::repr, ucsl::strings::VariableString>) return alignof(AddrType);
					else return alignof(typename desugar_t<T>::repr);
				}
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_CONSTANT) return alignof(typename desugar_t<T>::repr);
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_ENUMERATION) return alignof(typename desugar_t<T>::repr);
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_BITFIELD) return alignof(typename desugar_t<T>::underlying);
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_POINTER) return alignof(AddrType);
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_ARRAY) return alignof(AddrType);
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_TARRAY) return alignof(AddrType);
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_DYNAMIC_CARRAY) return DynamicCArray<desugar_t<T>, Parent, Root>{ this->parent, this->root }.get_item_type().template get_alignment<AddrType>(obj.as_carray()[0]);
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_STATIC_CARRAY) return StaticCArray<desugar_t<T>, Parent, Root>{ this->parent, this->root }.get_item_type().template get_alignment<AddrType>(obj.as_carray()[0]);
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_UNION) return Union<desugar_t<T>, Parent, Root>{ this->parent, this->root }.template get_alignment<AddrType>(obj.as_union());
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_STRUCTURE) return Structure<desugar_t<T>, Parent, Root>{ this->parent, this->root }.template get_alignment<AddrType>(obj.as_structure());
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_RFLCLASS) return typename rflclass<GameInterface>::Structure{ GameInterface::RflClassNameRegistry::GetInstance()->GetClassByName(resolve<typename desugar_t<T>::resolver>(this->parent, this->root).c_str()) }.template get_alignment<AddrType>(obj.as_structure());
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_COMPONENT_DATA) return typename rflclass<GameInterface>::Structure{ GameInterface::GameObjectSystem::GetInstance()->goComponentRegistry->GetComponentInformationByName(resolve<typename desugar_t<T>::resolver>(this->parent, this->root).c_str())->GetSpawnerDataClass() }.template get_alignment<AddrType>(obj.as_structure());
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_SPAWNER_DATA_RFLCLASS) return typename rflclass<GameInterface>::Structure{ GameInterface::GameObjectSystem::GetInstance()->gameObjectRegistry->GetGameObjectClassByName(resolve<typename desugar_t<T>::resolver>(this->parent, this->root).c_str())->GetSpawnerDataClass() }.template get_alignment<AddrType>(obj.as_structure());
				else static_assert(false, "getting alignment of unknown type");
			}

			template<typename F>
			constexpr auto visit(F f) const {
				if constexpr (desugar_t<T>::desc_type == DESCTYPE_PRIMITIVE) return f(Primitive<desugar_t<T>, ucsl::reflection::is_erased_v<T>>{});
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_CONSTANT) return f(Constant<desugar_t<T>, ucsl::reflection::is_erased_v<T>>{});
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_ENUMERATION) return f(Enum<desugar_t<T>, ucsl::reflection::is_erased_v<T>>{});
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_BITFIELD) return f(Bitfield<desugar_t<T>, Parent, Root, ucsl::reflection::is_erased_v<T>>{ this->parent, this->root });
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_POINTER) return f(Pointer<desugar_t<T>, Parent, Root, ucsl::reflection::is_weak_v<T>>{ this->parent, this->root });
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_ARRAY) return f(Array<desugar_t<T>, Parent, Root>{ this->parent, this->root });
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_TARRAY) return f(TArray<desugar_t<T>, Parent, Root>{ this->parent, this->root });
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_DYNAMIC_CARRAY) return f(DynamicCArray<desugar_t<T>, Parent, Root>{ this->parent, this->root });
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_STATIC_CARRAY) return f(StaticCArray<desugar_t<T>, Parent, Root>{ this->parent, this->root });
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_UNION) return f(Union<desugar_t<T>, Parent, Root>{ this->parent, this->root });
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_STRUCTURE) return f(Structure<desugar_t<T>, Parent, Root>{ this->parent, this->root });
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_RFLCLASS) return f(typename rflclass<GameInterface>::Structure{ GameInterface::RflClassNameRegistry::GetInstance()->GetClassByName(resolve<typename desugar_t<T>::resolver>(this->parent, this->root).c_str()) });
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_COMPONENT_DATA) return f(typename rflclass<GameInterface>::Structure{ GameInterface::GameObjectSystem::GetInstance()->goComponentRegistry->GetComponentInformationByName(resolve<typename desugar_t<T>::resolver>(this->parent, this->root).c_str())->GetSpawnerDataClass() });
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_SPAWNER_DATA_RFLCLASS) return f(typename rflclass<GameInterface>::Structure{ GameInterface::GameObjectSystem::GetInstance()->gameObjectRegistry->GetGameObjectClassByName(resolve<typename desugar_t<T>::resolver>(this->parent, this->root).c_str())->GetSpawnerDataClass() });
				else static_assert(false, "invalid desc type");
			}
		};

		template<typename Resolver, typename Scope, std::enable_if_t<std::is_enum_v<typename Resolver::result>, bool> = true>
		static typename Resolver::result _resolve_field_resolver(const Scope& scope_) {
			Scope& scope = const_cast<Scope&>(scope_);

			return scope.template get_field<Resolver::field>().visit([](const auto& v) {
				if constexpr (decltype(v.refl)::kind == ucsl::reflection::providers::TypeKind::ENUM) return static_cast<typename Resolver::result>(static_cast<long long>(v));
				else static_assert(false, "unsupported resolution type");
			});
		}

		template<typename Resolver, typename Scope, std::enable_if_t<!std::is_enum_v<typename Resolver::result>, bool> = true>
		static typename Resolver::result _resolve_field_resolver(const Scope& scope_) {
			Scope& scope = const_cast<Scope&>(scope_);

			return scope.template get_field<Resolver::field>().visit([](const auto& v) {
				if constexpr (decltype(v.refl)::kind == ucsl::reflection::providers::TypeKind::PRIMITIVE) return v.visit([](const auto& v) -> typename Resolver::result { return static_cast<typename Resolver::result>(v); });
				else static_assert(false, "unsupported resolution type");
			});
		}

		template<typename Resolver, typename Parent, typename Root>
		static typename Resolver::result resolve_field_resolver(const Parent& parent, const Root& root) {
			if constexpr (Resolver::scope == RESOLVER_SCOPE_PARENT) return _resolve_field_resolver<Resolver, Parent>(parent);
			else if constexpr (Resolver::scope == RESOLVER_SCOPE_ROOT) return _resolve_field_resolver<Resolver, Root>(root);
			else static_assert(false, "unsupported resolver scope");
		}

		template<typename Resolver, typename Parent, typename Root, typename... Sources>
		static typename Resolver::result _resolve_selector_resolver(const Parent& parent, const Root& root, std::tuple<Sources...>) {
			return Resolver::function(resolve<Sources, Parent>(parent, root)...);
		}

		template<typename Resolver, typename Parent, typename Root>
		static typename Resolver::result resolve_selector_resolver(const Parent& parent, const Root& root) {
			return _resolve_selector_resolver<Resolver, Parent>(parent, root, typename Resolver::sources{});
		}

		template<typename Resolver, typename Parent, typename Root>
		static auto resolve(const Parent& parent, const Root& root) {
			if constexpr (Resolver::resolver_type == RESOLVER_TYPE_FIELD) return resolve_field_resolver<Resolver, Parent, Root>(parent, root);
			else if constexpr (Resolver::resolver_type == RESOLVER_TYPE_SELECTOR) return resolve_selector_resolver<Resolver, Parent, Root>(parent, root);
			else if constexpr (Resolver::resolver_type == RESOLVER_TYPE_CUSTOM) return typename Resolver::f{}(parent, root);
			else static_assert(false, "invalid resolver type");
		}

		template<typename T>
		struct RootType : public Type<T, NullStructureAccessor, NullStructureAccessor> {
			RootType() : Type<T, NullStructureAccessor, NullStructureAccessor>{ {}, {} } {}
		};
		//template<typename T>
		//static constexpr Type<canonical_t<T>> reflect() { return {}; }

		//template<typename T>
		//static constexpr Type<canonical_t<T>> reflect(const T&) { return {}; }
	};
}
