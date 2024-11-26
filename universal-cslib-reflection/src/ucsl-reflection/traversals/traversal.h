#pragma once
#include <ucsl-reflection/opaque.h>
#include <ucsl-reflection/providers/types.h>
#include "types.h"

namespace ucsl::reflection::traversals {
	template<typename Algorithm, typename = std::make_index_sequence<Algorithm::arity>>
	class traversal;
	template<typename Algorithm, std::size_t... S>
	class traversal<Algorithm, std::index_sequence<S...>> {
		template<std::size_t, typename T> using Spread = T;

		//using Objs = Spread<S, opaque_obj&>;

		Algorithm algorithm;
			
		template<typename Primitive>
		typename Algorithm::result_type process_primitive(Spread<S, opaque_obj&>... objs, bool erased, Primitive refl) {
			return refl.visit([&](auto repr) {
				using Repr = typename decltype(repr)::repr;

				return algorithm.visit_primitive(reinterpret_cast<Repr&>(objs)..., PrimitiveInfo<Repr>{ erased, repr.constant_value });
			});
		}

		template<typename Enum>
		typename Algorithm::result_type process_enum(Spread<S, opaque_obj&>... objs, bool erased, Enum refl) {
			return algorithm.visit_enum(objs..., EnumInfo{ refl.get_options() }, [&](Spread<S, opaque_obj&>... underlyings) { return process_primitive(underlyings..., erased, refl.get_underlying_primitive()); });
		}

		template<typename Flags>
		typename Algorithm::result_type process_flags(Spread<S, opaque_obj&>... objs, bool erased, Flags refl) {
			return algorithm.visit_flags(objs..., FlagsInfo{ refl.get_values() }, [&](Spread<S, opaque_obj&>... objs) { return process_primitive(objs..., erased, refl.get_underlying_primitive()); });
		}

		template<typename Array>
		typename Algorithm::result_type process_array(Spread<S, opaque_obj&>... objs, Spread<S, opaque_obj&>... parents, Array refl) {
			auto item = refl.get_item_type();
			auto accessors = std::tuple{ refl.get_accessor(objs)... };

			return algorithm.visit_array(
				std::get<S>(accessors)...,
				ArrayInfo{ item.get_alignment(parents), item.get_size(parents, objs) }...,
				[]() { return nullptr; },
				[](void* obj) {},
				[&](Spread<S, opaque_obj&>... items) { return process_type(items..., parents..., item); }
			);
		}

		template<typename TArray>
		typename Algorithm::result_type process_tarray(Spread<S, opaque_obj&>... objs, Spread<S, opaque_obj&>... parents, TArray refl) {
			auto item = refl.get_item_type();
			auto accessors = std::tuple{ refl.get_accessor(objs)... };

			return algorithm.visit_tarray(
				std::get<S>(accessors)...,
				ArrayInfo{ item.get_alignment(parents), item.get_size(parents, objs) }...,
				[]() { return nullptr; },
				[](void* obj) {},
				[&](Spread<S, opaque_obj&>... items) { return process_type(items..., parents..., item); }
			);
		}

		template<typename Pointer>
		typename Algorithm::result_type process_pointer(Spread<S, opaque_obj&>... objs, Spread<S, opaque_obj&>... parents, Pointer refl) {
			auto target = refl.get_target_type();

			return algorithm.visit_pointer((Spread<S, opaque_obj*&>)objs..., PointerInfo{ target.get_alignment(parents), target.get_size(parents, objs) }..., [&](Spread<S, opaque_obj&>...targets) { return process_type(targets..., parents..., target); });
		}

		template<typename CArray>
		typename Algorithm::result_type process_carray(Spread<S, opaque_obj&>... objs, Spread<S, opaque_obj&>... parents, CArray refl) {
			auto item = refl.get_item_type();

			return algorithm.visit_carray(&objs..., CArrayInfo{ refl.get_length(parents), item.get_size(parents, objs) }..., [&](Spread<S, opaque_obj&>... items) { return process_type(items..., parents..., item); });
		}

		template<typename Union>
		typename Algorithm::result_type process_union(Spread<S, opaque_obj&>... objs, Spread<S, opaque_obj&>... parents, Spread<S, size_t>... sizes, Union refl) {
			return algorithm.visit_union(objs..., UnionInfo{ sizes }..., [&](Spread<S, opaque_obj&>... objs) {
				typename Algorithm::result_type result{};

				refl.visit_current_field(*std::get<0>(std::tuple{ &parents... }), [&](auto chosen) { result = process_type(objs..., parents..., chosen.get_type()); });

				return result;
			});
		}

		template<typename Field>
		typename Algorithm::result_type process_field(Spread<S, opaque_obj&>... objs, Field refl) {
			return algorithm.visit_field(*util::addptr(&objs, refl.get_offset())..., FieldInfo{ .name = refl.get_name(), .erased = false }, [&](Spread<S, opaque_obj&>... fields) { return process_type(fields..., objs..., refl.get_type()); });
		}

		template<typename Structure>
		typename Algorithm::result_type process_fields(Spread<S, opaque_obj&>... objs, Spread<S, size_t>... alignments, Structure refl) {
			typename Algorithm::result_type result{};

			auto base = refl.get_base();

			if (base.has_value())
				result |= process_base_struct(objs..., alignments..., base.value());

			refl.visit_fields(*std::get<0>(std::tuple{ &objs... }), [&](auto field) { result |= process_field(objs..., field); });

			return result;
		}

		template<typename Structure>
		typename Algorithm::result_type process_base_struct(Spread<S, opaque_obj&>... objs, Spread<S, size_t>... alignments, Structure refl) {
			return algorithm.visit_base_struct(objs..., StructureInfo{ refl.get_name(), alignments }..., [&](Spread<S, opaque_obj&>... objs) { return process_fields(objs..., alignments..., refl); });
		}

		template<typename Structure>
		typename Algorithm::result_type process_struct(Spread<S, opaque_obj&>... objs, Spread<S, size_t>... alignments, Structure refl) {
			return algorithm.visit_struct(objs..., StructureInfo{ refl.get_name(), alignments }..., [&](Spread<S, opaque_obj&>... objs) { return process_fields(objs..., alignments..., refl); });
		}

		template<typename Type>
		typename Algorithm::result_type process_type(Spread<S, opaque_obj&>... objs, Spread<S, opaque_obj&>... parents, Type refl) {
			return algorithm.visit_type(objs..., TypeInfo{ refl.get_alignment(parents), refl.get_size(parents, objs) }..., [&](Spread<S, opaque_obj&>... objs) {
				return refl.visit(*std::get<0>(std::tuple{ &parents... }), [&](auto r) {
					if constexpr (decltype(r)::kind == providers::TypeKind::PRIMITIVE) return process_primitive(objs..., refl.is_erased(), r);
					else if constexpr (decltype(r)::kind == providers::TypeKind::ENUM) return process_enum(objs..., refl.is_erased(), r);
					else if constexpr (decltype(r)::kind == providers::TypeKind::FLAGS) return process_flags(objs..., refl.is_erased(), r);
					else if constexpr (decltype(r)::kind == providers::TypeKind::ARRAY) return process_array(objs..., parents..., r);
					else if constexpr (decltype(r)::kind == providers::TypeKind::TARRAY) return process_tarray(objs..., parents..., r);
					else if constexpr (decltype(r)::kind == providers::TypeKind::POINTER) return process_pointer(objs..., parents..., r);
					else if constexpr (decltype(r)::kind == providers::TypeKind::CARRAY) return process_carray(objs..., parents..., r);
					else if constexpr (decltype(r)::kind == providers::TypeKind::UNION) return process_union(objs..., parents..., refl.get_size(parents, objs)..., r);
					else if constexpr (decltype(r)::kind == providers::TypeKind::STRUCTURE) return process_struct(objs..., refl.get_alignment(parents)..., r);
					else assert(false && "invalid type kind"); return typename Algorithm::result_type{};
				});
			});
		}

		template<typename Type>
		typename Algorithm::result_type process_root(Spread<S, opaque_obj&>... objs, Type refl) {
			return algorithm.visit_root(objs..., RootInfo{ refl.get_alignment(objs), refl.get_size(objs, objs) }..., [&](Spread<S, opaque_obj&>...objs) { return process_type(objs..., objs..., refl); });
		}

	public:
		template<typename ...Args>
		traversal(Args&& ...objs) : algorithm{ std::forward<Args>(objs)... } {}

		//template<typename T>
		//typename Algorithm::result_type operator()(Spread<S, opaque_obj&>... objs) {
		//	return process_root<T>(objs...);
		//}

		template<typename T, typename Refl>
		typename Algorithm::result_type operator()(Spread<S, T&>... objs, Refl refl) {
			return process_root(((Spread<S, opaque_obj&>)objs)..., refl);
		}

		template<typename T, typename ...Args>
		static typename Algorithm::result_type apply(Spread<S, opaque_obj&>... objs, Args&& ...args) {
			traversal<Algorithm> t{ std::forward<Args>(args)... };

			return t.operator()<T>(objs...);
		}
	};
}
