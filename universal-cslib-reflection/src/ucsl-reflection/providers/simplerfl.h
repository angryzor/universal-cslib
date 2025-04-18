#pragma once
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
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
		template<typename T>
		struct dynamic_size_of_struct;
		template<typename Repr, strlit name, typename Base, typename... Fields>
		struct dynamic_size_of_struct<structure<Repr, name, Base, Fields...>> {
			static size_t get(const opaque_obj& self, const opaque_obj& root) {
				size_t offset{};

				if constexpr (!std::is_same_v<Base, void>)
					offset = dynamic_size_of_struct<Base>::get(self, root);

				((
					offset = util::align(offset, dynamic_align_of<typename Fields::type>(self, root)),
					offset += dynamic_size_of<typename Fields::type>(self, root, *util::addptr(&self, offset))
				), ...);

				return util::align(offset, align_of_v<structure<Repr, name, Base, Fields...>>);
			}
		};

		//template<typename T>
		//struct dynamic_align_of_struct;
		//template<typename Repr, strlit name, typename Base, typename... Fields>
		//struct dynamic_align_of_struct<structure<Repr, name, Base, Fields...>> {
		//	static size_t get() {
		//		size_t maxAlign{};

		//		if constexpr (!std::is_same_v<Base, void>)
		//			maxAlign = std::max(maxAlign, dynamic_align_of<Base>(*(const opaque_obj*)nullptr));

		//		((maxAlign = std::max(maxAlign, dynamic_align_of<typename Fields::type>(*(const opaque_obj*)nullptr))), ...);

		//		return maxAlign;
		//	}
		//};

		template<typename T>
		static size_t dynamic_size_of(const opaque_obj& parent, const opaque_obj& root, const opaque_obj& self) {
			if constexpr (desugar_t<T>::desc_type == DESCTYPE_RFLCLASS)
				return GameInterface::RflClassNameRegistry::GetInstance()->GetClassByName(desugar_t<T>::resolver((const typename desugar_t<T>::parent&)parent))->GetSize();
			else if constexpr (desugar_t<T>::desc_type == DESCTYPE_COMPONENT_DATA)
				return GameInterface::GameObjectSystem::GetInstance()->goComponentRegistry->GetComponentInformationByName(desugar_t<T>::resolver((const typename desugar_t<T>::parent&)parent))->GetSpawnerDataClass()->GetSize();
			else if constexpr (desugar_t<T>::desc_type == DESCTYPE_SPAWNER_DATA_RFLCLASS)
				return GameInterface::GameObjectSystem::GetInstance()->gameObjectRegistry->GetGameObjectClassByName(desugar_t<T>::resolver((const typename desugar_t<T>::parent&)parent))->GetSpawnerDataClass()->GetSize();
			else if constexpr (desugar_t<T>::desc_type == DESCTYPE_SPAWNER_DATA_RFLCLASS_WITH_ROOT)
				return GameInterface::GameObjectSystem::GetInstance()->gameObjectRegistry->GetGameObjectClassByName(desugar_t<T>::resolver((const typename desugar_t<T>::parent&)parent, (const typename desugar_t<T>::root&)root))->GetSpawnerDataClass()->GetSize();
			else if constexpr (desugar_t<T>::desc_type == DESCTYPE_DYNAMIC_CARRAY)
				return desugar_t<T>::resolver((const typename desugar_t<T>::parent&)parent) * dynamic_size_of<typename desugar_t<T>::type>(parent, root, self);
			else if constexpr (desugar_t<T>::desc_type == DESCTYPE_STATIC_CARRAY)
				return desugar_t<T>::size * dynamic_size_of<typename desugar_t<T>::type>(parent, root, self);
			else if constexpr (desugar_t<T>::desc_type == DESCTYPE_STRUCTURE)
				return dynamic_size_of_struct<desugar_t<T>>::get(self, root);
			else
				return size_of_v<T>;
		}

		template<typename T>
		static size_t dynamic_align_of(const opaque_obj& parent, const opaque_obj& root) {
			if constexpr (is_realigned_v<T>)
				return align_of_v<T>;
			else if constexpr (desugar_t<T>::desc_type == DESCTYPE_RFLCLASS)
				return GameInterface::RflClassNameRegistry::GetInstance()->GetClassByName(desugar_t<T>::resolver((const typename desugar_t<T>::parent&)parent))->GetAlignment();
			else if constexpr (desugar_t<T>::desc_type == DESCTYPE_COMPONENT_DATA)
				return GameInterface::GameObjectSystem::GetInstance()->goComponentRegistry->GetComponentInformationByName(desugar_t<T>::resolver((const typename desugar_t<T>::parent&)parent))->GetSpawnerDataClass()->GetAlignment();
			else if constexpr (desugar_t<T>::desc_type == DESCTYPE_SPAWNER_DATA_RFLCLASS)
				return GameInterface::GameObjectSystem::GetInstance()->gameObjectRegistry->GetGameObjectClassByName(desugar_t<T>::resolver((const typename desugar_t<T>::parent&)parent))->GetSpawnerDataClass()->GetAlignment();
			else if constexpr (desugar_t<T>::desc_type == DESCTYPE_SPAWNER_DATA_RFLCLASS_WITH_ROOT)
				return GameInterface::GameObjectSystem::GetInstance()->gameObjectRegistry->GetGameObjectClassByName(desugar_t<T>::resolver((const typename desugar_t<T>::parent&)parent, (const typename desugar_t<T>::root&)root))->GetSpawnerDataClass()->GetAlignment();
			else if constexpr (desugar_t<T>::desc_type == DESCTYPE_DYNAMIC_CARRAY)
				return dynamic_align_of<typename desugar_t<T>::type>(parent, root);
			else if constexpr (desugar_t<T>::desc_type == DESCTYPE_STATIC_CARRAY)
				return dynamic_align_of<typename desugar_t<T>::type>(parent, root);
			//else if constexpr (desugar_t<T>::desc_type == DESCTYPE_STRUCTURE)
			//	return dynamic_align_of_struct<desugar_t<T>>::get();
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


		template<typename T>
		struct Primitive {
			constexpr static TypeKind kind = TypeKind::PRIMITIVE;
			template<typename F>
			constexpr static auto visit(F f) { return f(PrimitiveData<typename T::repr>{}); }
		};

		template<typename T>
		struct Constant {
			constexpr static TypeKind kind = TypeKind::PRIMITIVE;
			template<typename F>
			constexpr static auto visit(F f) { return f(PrimitiveData<typename T::repr>{ .constant_value = T::value }); }
		};

		template<typename T>
		struct Type;

		template<typename T>
		struct Enum {
			constexpr static TypeKind kind = TypeKind::ENUM;
			constexpr static auto get_options() { return get_enum_members(typename T::options{}); }
			template<typename F>
			constexpr static auto visit(F f) { return f(PrimitiveData<typename T::underlying>{}); }
		};

		//struct Flags {
		//	auto get_flags();
		//};

		template<typename T>
		struct Pointer {
			constexpr static TypeKind kind = TypeKind::POINTER;
			constexpr static auto get_target_type() { return Type<typename T::target>{}; }
		};

		//template<typename T>
		//struct Offset {
		//	constexpr static TypeKind kind = TypeKind::OFFSET;
		//	constexpr static auto get_target_type() { return Type<typename T::target>{}; }
		//	template<typename F>
		//	constexpr static auto visit(F f) { return f(PrimitiveData<typename T::repr>{}); }
		//};

		template<typename T>
		struct Array {
			constexpr static TypeKind kind = TypeKind::ARRAY;
			constexpr static auto get_item_type() { return Type<typename T::type>{}; }
			constexpr static auto get_accessor(opaque_obj& obj) { return OpaqueArray<representation_t<T>, GameInterface>{ (representation_t<T>&)obj }; }
		};

		template<typename T>
		struct TArray {
			constexpr static TypeKind kind = TypeKind::TARRAY;
			constexpr static auto get_item_type() { return Type<typename T::type>{}; }
			constexpr static auto get_accessor(opaque_obj& obj) { return OpaqueArray<representation_t<T>, GameInterface>{ (representation_t<T>&)obj }; }
		};

		template<typename T>
		struct StaticCArray {
			constexpr static TypeKind kind = TypeKind::CARRAY;
			constexpr static auto get_item_type() { return Type<typename T::type>{}; }
			constexpr static size_t get_length(opaque_obj& parent) { return T::size; }
		};

		template<typename T>
		struct DynamicCArray {
			constexpr static TypeKind kind = TypeKind::CARRAY;
			constexpr static auto get_item_type() { return Type<typename T::type>{}; }
			constexpr static size_t get_length(const opaque_obj& parent) { return T::resolver((typename T::parent&)parent); }
		};

		template<typename T>
		struct Field {
			size_t offset{};

			constexpr static const char* get_name() { return T::name; }
			constexpr size_t get_offset() const { return offset; }
			constexpr static auto get_type() { return Type<typename T::type>{}; }
		};

		template<typename T>
		struct Union {
			constexpr static TypeKind kind = TypeKind::UNION;
			template<typename F>
			constexpr static void visit_fields(F f) { _visit_fields(f, typename T::fields{}); }

			template<typename F>
			constexpr static void visit_current_field(const opaque_obj& parent, F f) { _visit_current_field(parent, f, typename T::fields{}, std::make_index_sequence<std::tuple_size_v<typename T::fields>>{}); }

		private:
			template<typename F, typename... Fields>
			constexpr static void _visit_fields(F f, std::tuple<Fields...>) {
				(f(Field<Fields>{ 0 }), ...);
			}

			template<typename F, typename Fields, size_t... Is>
			constexpr static void _visit_current_field(const opaque_obj& parent, F f, Fields, std::index_sequence<Is...>) {
				size_t idx = T::resolver((typename T::parent&)parent);

				((idx == Is ? (f(Field<std::tuple_element_t<Is, Fields>>{ 0 }), true) : false) || ...);
			}
		};

		struct EmptyStruct {
			constexpr static TypeKind kind = TypeKind::STRUCTURE;
			constexpr static const char* get_name() { return nullptr; }
			constexpr static auto get_base() { return std::optional<EmptyStruct>{}; }
			constexpr static size_t get_size(const opaque_obj& parent, const opaque_obj& root, const opaque_obj& obj) { return 0; }
			constexpr static size_t get_alignment(const opaque_obj& parent, const opaque_obj& root) { return 0; }
			template<typename F> constexpr static void visit_fields(const opaque_obj& obj, const opaque_obj& root, F f) {}
		};

		template<typename T>
		struct Structure {
			using Base = typename T::base;
			using Fields = typename T::fields;

			// TODO: Refactor this. I'm doing this for RFL DnD. I may prefer to pass refl structs into traversal algos,
			// but don't know how to send those around in ImGui DnD data.
			const GameInterface::RflSystem::RflClass* rflClass;

			constexpr static TypeKind kind = TypeKind::STRUCTURE;
			constexpr static const char* get_name() { return T::name; }
			constexpr static size_t get_size(const opaque_obj& parent, const opaque_obj& root, const opaque_obj& obj) { return dynamic_size_of<T>(parent, root, obj); }
			constexpr static size_t get_alignment(const opaque_obj& parent, const opaque_obj& root) { return dynamic_align_of<T>(parent, root); }
			constexpr static auto get_base() {
				if constexpr (!std::is_same_v<Base, primitive<void>>)
					return std::make_optional(Structure<Base>{});
				else
					return std::optional<EmptyStruct>{};
			}

			template<typename F>
			constexpr static void visit_fields(const opaque_obj& obj, const opaque_obj& root, F f) { _visit_fields(obj, root, f, Fields{}); }

		private:
			template<typename F, typename... Fields>
			constexpr static void _visit_fields(const opaque_obj& obj, const opaque_obj& root, F f, std::tuple<Fields...>) {
				size_t offset{};
				size_t thisOffset{};

				if constexpr (!std::is_same_v<Base, primitive<void>>)
					offset = size_of_v<Base>;

				(f((
					offset = thisOffset = util::align(offset, dynamic_align_of<typename Fields::type>(obj, root)),
					offset += dynamic_size_of<typename Fields::type>(obj, root, *util::addptr(&obj, thisOffset)),
					Field<Fields>{ thisOffset }
				)), ...);
			}
		};

		//	template<typename F>
		//	constexpr static void visit_fields(const opaque_obj& obj, const opaque_obj& root, F f) {
		//		_visit_fields(obj, root, f, std::make_index_sequence<std::tuple_size_v<Fields>>{});
		//	}

		//private:
		//	template<typename F, size_t... Is>
		//	constexpr static void _visit_fields(const opaque_obj& obj, const opaque_obj& root, F f, std::index_sequence<Is...>) {
		//		auto fields = _get_fields(obj, root, Fields{});
		//		constexpr auto prios = _get_sorted_field_indices(std::index_sequence<Is...>{});

		//		(f(std::get<prios[Is].first>(fields)), ...);
		//	}

		//	template<size_t... Is>
		//	consteval static auto _get_sorted_field_indices(std::index_sequence<Is...>) {
		//		auto prios = std::array<std::pair<size_t, char>, std::tuple_size_v<Fields>>{ std::pair{ Is, get_priority_v<typename std::tuple_element_t<Is, Fields>::type> }... };

		//		std::sort(prios.begin(), prios.end(), [](const auto& a, const auto& b) { return a.second == b.second ? a.first < b.first : a.second < b.second; });

		//		return prios;
		//	}

		//	template<typename... Fields>
		//	constexpr static auto _get_fields(const opaque_obj& obj, const opaque_obj& root, std::tuple<Fields...>) {
		//		size_t offset{};
		//		size_t thisOffset{};

		//		if constexpr (!std::is_same_v<Base, primitive<void>>)
		//			offset = size_of_v<Base>;

		//		return std::tuple{ (
		//			offset = thisOffset = util::align(offset, dynamic_align_of<typename Fields::type>(obj, root)),
		//			offset += dynamic_size_of<typename Fields::type>(obj, root, *util::addptr(&obj, thisOffset)),
		//			Field<Fields>{ thisOffset }
		//		)... };
		//	}
		//};

		template<typename T>
		struct Type {
			constexpr static size_t get_size(const opaque_obj& parent, const opaque_obj& root, const opaque_obj& obj) { return dynamic_size_of<T>(parent, root, obj); }
			constexpr static size_t get_alignment(const opaque_obj& parent, const opaque_obj& root) { return dynamic_align_of<T>(parent, root); }

			constexpr static bool is_erased() { return ucsl::reflection::is_erased_v<T>; }
			constexpr static bool is_weak() { return ucsl::reflection::is_weak_v<T>; }

			template<typename F>
			constexpr static auto visit(const opaque_obj& parent, const opaque_obj& root, F f) {
				if constexpr (desugar_t<T>::desc_type == DESCTYPE_PRIMITIVE) return f(Primitive<desugar_t<T>>{});
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_CONSTANT) return f(Constant<desugar_t<T>>{});
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_ENUMERATION) return f(Enum<desugar_t<T>>{});
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_POINTER) return f(Pointer<desugar_t<T>>{});
				//else if constexpr (desugar_t<T>::desc_type == DESCTYPE_OFFSET) return f(Offset<desugar_t<T>>{});
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_ARRAY) return f(Array<desugar_t<T>>{});
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_TARRAY) return f(TArray<desugar_t<T>>{});
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_DYNAMIC_CARRAY) return f(DynamicCArray<desugar_t<T>>{});
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_STATIC_CARRAY) return f(StaticCArray<desugar_t<T>>{});
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_UNION) return f(Union<desugar_t<T>>{});
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_STRUCTURE) return f(Structure<desugar_t<T>>{});
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_RFLCLASS)
					return f(typename rflclass<GameInterface>::Structure{ GameInterface::RflClassNameRegistry::GetInstance()->GetClassByName(desugar_t<T>::resolver((typename desugar_t<T>::parent&)parent)) });
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_COMPONENT_DATA)
					return f(typename rflclass<GameInterface>::Structure{ GameInterface::GameObjectSystem::GetInstance()->goComponentRegistry->GetComponentInformationByName(desugar_t<T>::resolver((const typename desugar_t<T>::parent&)parent))->GetSpawnerDataClass() });
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_SPAWNER_DATA_RFLCLASS)
					return f(typename rflclass<GameInterface>::Structure{ GameInterface::GameObjectSystem::GetInstance()->gameObjectRegistry->GetGameObjectClassByName(desugar_t<T>::resolver((const typename desugar_t<T>::parent&)parent))->GetSpawnerDataClass() });
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_SPAWNER_DATA_RFLCLASS_WITH_ROOT)
					return f(typename rflclass<GameInterface>::Structure{ GameInterface::GameObjectSystem::GetInstance()->gameObjectRegistry->GetGameObjectClassByName(desugar_t<T>::resolver((const typename desugar_t<T>::parent&)parent, (const typename desugar_t<T>::root&)root))->GetSpawnerDataClass() });
				else static_assert("invalid desc type");
			}
		};

		template<typename T>
		static constexpr Type<canonical_t<T>> reflect() { return {}; }

		template<typename T>
		static constexpr Type<canonical_t<T>> reflect(const T&) { return {}; }
	};
}