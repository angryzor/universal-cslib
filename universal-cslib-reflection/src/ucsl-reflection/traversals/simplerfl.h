#pragma once
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/util/memory.h>
#include <ucsl-reflection/traversals/rflclass.h>
#include <ucsl-reflection/opaque.h>
#include "types.h"

namespace ucsl::reflection::traversals {
	namespace internal {
		using namespace simplerfl;

		template<typename GameInterface>
		struct simplerfl_traversal_for {
			template<typename T>
			struct dynamic_align_of_struct;
			template<typename Repr, strlit name, typename Base, typename... Fields>
			struct dynamic_align_of_struct<structure<Repr, name, Base, Fields...>> {
			    static size_t get(void* parent) {
			        size_t maxAlign{};

			        if constexpr (!std::is_same_v<Base, void>)
			            maxAlign = std::max(maxAlign, dynamic_align_of<Base>(parent));

			        ((maxAlign = std::max(maxAlign, dynamic_align_of<typename Fields::type>(parent))), ...);

			        return maxAlign;
			    }
			};

			template<typename T>
			struct dynamic_size_of_struct;
			template<typename Repr, strlit name, typename Base, typename... Fields>
			struct dynamic_size_of_struct<structure<Repr, name, Base, Fields...>> {
			    static size_t get(void* parent, void* self) {
			        size_t offset{};

			        if constexpr (!std::is_same_v<Base, void>)
			            offset = dynamic_size_of<Base>(parent, self);

			        ((
			            offset = util::align(offset, dynamic_align_of<typename Fields::type>(parent)),
			            offset += dynamic_size_of<typename Fields::type>(parent, util::addptr(self, offset))
			        ), ...);

			        return util::align(offset, dynamic_align_of<structure<Repr, name, Base, Fields...>>(parent));
			    }
			};

			template<typename T>
			static size_t dynamic_size_of(void* parent, void* self) {
				if constexpr (desugar_t<T>::desc_type == DESCTYPE_RFLCLASS)
					return GameInterface::RflClassNameRegistry::GetInstance()->GetClass(desugar_t<T>::resolver(*(typename desugar_t<T>::parent*)parent))->GetSize();
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_COMPONENT_DATA)
					return GameInterface::RflClassNameRegistry::GetInstance()->GetClass("GOCActivatorSpawner")->GetSize();
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_SPAWNER_DATA_RFLCLASS)
					return GameInterface::RflClassNameRegistry::GetInstance()->GetClass(GameInterface::get_spawner_data_class(desugar_t<T>::resolver(*(typename desugar_t<T>::parent*)parent)))->GetSize();
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_DYNAMIC_CARRAY)
					return desugar_t<T>::resolver(*(typename desugar_t<T>::parent*)parent) * dynamic_size_of<typename desugar_t<T>::type>(parent, self);
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_STATIC_CARRAY)
					return desugar_t<T>::size * dynamic_size_of<typename desugar_t<T>::type>(parent, self);
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_STRUCTURE)
					return dynamic_size_of_struct<T>::get(parent, self);
				else
					return simplerfl::size_of_v<T>;
			}

			template<typename T>
			static size_t dynamic_align_of(void* parent) {
				if constexpr (desugar_t<T>::desc_type == DESCTYPE_RFLCLASS)
					return GameInterface::RflClassNameRegistry::GetInstance()->GetClass(desugar_t<T>::resolver(*(typename desugar_t<T>::parent*)parent))->GetAlignment();
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_COMPONENT_DATA)
					return GameInterface::RflClassNameRegistry::GetInstance()->GetClass("GOCActivatorSpawner")->GetAlignment();
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_SPAWNER_DATA_RFLCLASS)
					return GameInterface::RflClassNameRegistry::GetInstance()->GetClass(GameInterface::get_spawner_data_class(desugar_t<T>::resolver(*(typename desugar_t<T>::parent*)parent)))->GetAlignment();
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_DYNAMIC_CARRAY)
					return dynamic_align_of<typename desugar_t<T>::type>(parent);
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_STATIC_CARRAY)
					return dynamic_align_of<typename desugar_t<T>::type>(parent);
				else if constexpr (desugar_t<T>::desc_type == DESCTYPE_STRUCTURE)
					return dynamic_align_of_struct<T>::get(parent);
				else
					return simplerfl::align_of_v<T>;
			}

			struct EnumMember {
				const long long index{};
				const char* name{};

				long long GetIndex() const { return index; }
				const char* const GetEnglishName() const { return name; }
			};

			template<typename T> struct getEnumMember;
			template<strlit name> struct getEnumMember<option<name>> {
				static constexpr EnumMember call(long long& counter) { return { counter++, name }; }
			};
			template<strlit name, long long value> struct getEnumMember<fixed_option<name, value>> {
				static constexpr EnumMember call(long long& counter) { counter = value + 1; return { value, name }; }
			};

			template<typename Options, typename I = std::make_index_sequence<std::tuple_size_v<Options>>>
			struct getEnumMembers;
			template<typename Options, size_t... Is>
			struct getEnumMembers<Options, std::index_sequence<Is...>> {
				template<std::size_t, typename T> using Spread = T;

				constexpr std::vector<EnumMember> operator()() {
					long long counter{};

					return { getEnumMember<std::tuple_element_t<Is, Options>>::call(counter)... };
				}
			};

			template<typename Algorithm, typename = std::make_index_sequence<Algorithm::arity>>
			class traversal;
			template<typename Algorithm, std::size_t... S>
			class traversal<Algorithm, std::index_sequence<S...>> {
				template<std::size_t, typename T> using Spread = T;
				template<std::size_t, typename T = opaque_obj> using Obj = T&;

				Algorithm algorithm;

				template<typename Fields, typename I = std::make_index_sequence<std::tuple_size_v<Fields>>>
				struct FieldIterator;
				template<typename Fields, size_t... Is>
				struct FieldIterator<Fields, std::index_sequence<Is...>> {
					traversal<Algorithm, std::index_sequence<S...>>& t;

					typename Algorithm::result_type operator()(Obj<S>... objs, Obj<S>... parents, size_t& offset) {
						typename Algorithm::result_type result{};

						((
							offset = util::align(offset, dynamic_align_of<typename std::tuple_element_t<Is, Fields>::type>(&parents...)), // TODO: this should probably use the dynamic version.
							result |= t.process_field<std::tuple_element_t<Is, Fields>>(*util::addptr(&objs, offset)..., parents...),
							offset += dynamic_size_of<typename std::tuple_element_t<Is, Fields>::type>(&parents..., util::addptr(&objs, offset)...)
						), ...);

						return result;
					}
				};

				//template<typename Fields, typename I = std::make_index_sequence<std::tuple_size_v<Fields>>>
				//struct UnionFieldSwitcher;
				//template<typename Fields, size_t... Is>
				//struct UnionFieldSwitcher<Fields, std::index_sequence<Is...>> {
				//	traversal<Algorithm, std::index_sequence<S...>>& t;

				//	typename Algorithm::result_type operator()(Obj<S>... objs, Obj<S>... parents, size_t idx) {
				//		typename Algorithm::result_type result{};

				//		((idx == Is ? (result = t.

				//			result |= t.process_field<>(*util::addptr(&objs, offset)..., parents...),
				//			offset += size_of_v<typename std::tuple_element_t<Is, Fields>::type>
				//		), ...);

				//		return result;
				//	}
				//};

				template<typename T, typename Repr>
				typename Algorithm::result_type process_primitive_base(Obj<S>... objs, const std::optional<Repr>& constant_value = std::nullopt) {
					return algorithm.visit_primitive(reinterpret_cast<Repr&>(objs)..., PrimitiveInfo<Repr>{ align_of_v<T>, size_of_v<T>, ucsl::reflection::is_erased_v<T>, constant_value });
				}

				template<typename T>
				typename Algorithm::result_type process_primitive(Obj<S>... objs) {
					static_assert(desugar_t<T>::desc_type == DESCTYPE_PRIMITIVE);

					return process_primitive_base<T, typename desugar_t<T>::repr>(objs...);
				}

				template<typename T>
				typename Algorithm::result_type process_constant(Obj<S>... objs) {
					static_assert(desugar_t<T>::desc_type == DESCTYPE_CONSTANT);

					return process_primitive_base<T, typename desugar_t<T>::repr>(objs..., desugar_t<T>::value);
				}

				template<typename T>
				typename Algorithm::result_type process_enumeration(Obj<S>... objs) {
					static_assert(desugar_t<T>::desc_type == DESCTYPE_ENUMERATION);

					std::vector<EnumMember> enumMembers = getEnumMembers<typename desugar_t<T>::options>{}();

					return algorithm.visit_enum(objs..., EnumInfo{ enumMembers }, [&](Obj<S>... objs) { return process_primitive_base<T, typename desugar_t<T>::underlying>(objs...); });
				}

				template<typename T>
				typename Algorithm::result_type process_array(Obj<S>... objs, Obj<S>... parents) {
					static_assert(desugar_t<T>::desc_type == DESCTYPE_ARRAY);

					using InnerType = typename desugar_t<T>::type;

					std::tuple<Spread<S, OpaqueArray<representation_t<T>, GameInterface>>...> arrs{ { reinterpret_cast<representation_t<T>&>(objs) }... };

					return algorithm.visit_array(
						std::get<S>(arrs)...,
						ArrayInfo{ dynamic_align_of<InnerType>(&parents), dynamic_size_of<InnerType>(&parents, &objs) }...,
						[]() { return new representation_t<InnerType>{}; },
						[](representation_t<InnerType>* obj) { delete obj; },
						[&](Obj<S>... objs) { return process_type<InnerType>(objs..., parents...); }
					);
				}

				template<typename T>
				typename Algorithm::result_type process_tarray(Obj<S>... objs, Obj<S>... parents) {
					static_assert(desugar_t<T>::desc_type == DESCTYPE_TARRAY);

					using InnerType = typename desugar_t<T>::type;

					std::tuple<Spread<S, OpaqueArray<representation_t<T>, GameInterface>>...> arrs{ { reinterpret_cast<representation_t<T>&>(objs) }... };

					return algorithm.visit_tarray(
						std::get<S>(arrs)...,
						ArrayInfo{ dynamic_align_of<InnerType>(&parents), dynamic_size_of<InnerType>(&parents, &objs) }...,
						[]() { return new representation_t<InnerType>{}; },
						[](representation_t<InnerType>* obj) { delete obj; },
						[&](Obj<S>... objs) { return process_type<InnerType>(objs..., parents...); }
					);
				}

				template<typename T>
				typename Algorithm::result_type process_pointer(Obj<S>... objs, Obj<S>... parents) {
					static_assert(desugar_t<T>::desc_type == DESCTYPE_POINTER);

					using Target = typename desugar_t<T>::target;

					return algorithm.visit_pointer((Obj<S, opaque_obj*>)objs..., PointerInfo{ dynamic_align_of<Target>(&parents), dynamic_size_of<Target>(&parents, &objs) }..., [&](Obj<S>...objs) { return process_type<Target>(objs..., parents...); });
				}

				template<typename T>
				typename Algorithm::result_type process_dynamic_carray(Obj<S>... objs, Obj<S>... parents) {
					static_assert(desugar_t<T>::desc_type == DESCTYPE_DYNAMIC_CARRAY);

					using Parent = typename desugar_t<T>::parent;
					using InnerType = typename desugar_t<T>::type;

					return algorithm.visit_carray(&objs..., CArrayInfo{ desugar_t<T>::resolver((Parent&)parents), size_of_v<InnerType> }..., [&](Obj<S>...objs) { return process_type<InnerType>(objs..., parents...); });
				}

				template<typename T>
				typename Algorithm::result_type process_static_carray(Obj<S>... objs, Obj<S>... parents) {
					static_assert(desugar_t<T>::desc_type == DESCTYPE_STATIC_CARRAY);

					using InnerType = typename desugar_t<T>::type;

					return algorithm.visit_carray(&objs..., (objs, CArrayInfo{ desugar<T>::size, size_of_v<InnerType> })..., [&](Obj<S>... objs) { return process_type<InnerType>(objs..., parents...); });
				}

				template<typename T>
				typename Algorithm::result_type process_rflclass(Obj<S>... objs, Obj<S>... parents) {
					static_assert(desugar_t<T>::desc_type == DESCTYPE_RFLCLASS);

					using Parent = typename desugar_t<T>::parent;

					typename rflclass_traversal_for<GameInterface>::template traversal<Algorithm> t{ algorithm };

					return t.process_struct(objs..., GameInterface::RflClassNameRegistry::GetInstance()->GetClass(desugar_t<T>::resolver((Parent&)parents))...);
				}

				template<typename T>
				typename Algorithm::result_type process_component_data(Obj<S>... objs, Obj<S>... parents) {
					static_assert(desugar_t<T>::desc_type == DESCTYPE_COMPONENT_DATA);

					using Parent = typename desugar_t<T>::parent;

					typename rflclass_traversal_for<GameInterface>::template traversal<Algorithm> t{ algorithm };

					return t.process_struct(objs..., GameInterface::RflClassNameRegistry::GetInstance()->GetClass(((Parent&)parents, "GOCActivatorSpawner"))...);
				}

				template<typename T>
				typename Algorithm::result_type process_spawner_data(Obj<S>... objs, Obj<S>... parents) {
					static_assert(desugar_t<T>::desc_type == DESCTYPE_SPAWNER_DATA_RFLCLASS);

					using Parent = typename desugar_t<T>::parent;

					typename rflclass_traversal_for<GameInterface>::template traversal<Algorithm> t{ algorithm };

					return t.process_struct(objs..., GameInterface::RflClassNameRegistry::GetInstance()->GetClass(GameInterface::get_spawner_data_class(desugar_t<T>::resolver((Parent&)parents)))...);
				}

				template<typename Fields, size_t... Is>
				typename Algorithm::result_type switch_union_fields(Obj<S>... objs, Obj<S>... parents, size_t idx, std::index_sequence<Is...>) {
					typename Algorithm::result_type result{};

					((idx == Is ? (result = process_type<typename std::tuple_element_t<Is, Fields>::type>(objs..., parents...), true) : false) || ...);

					return result;
				}

				template<typename T>
				typename Algorithm::result_type process_union(Obj<S>... objs, Obj<S>... parents) {
					using Parent = typename desugar_t<T>::parent;

					std::tuple<Obj<S>...> parents_tuple{ parents... };
					opaque_obj& first_parent = std::get<0>(parents_tuple);

					// TODO: handle different types
					return switch_union_fields<typename desugar_t<T>::fields>(objs..., parents..., desugar_t<T>::resolver((Parent&)first_parent), std::make_index_sequence<std::tuple_size_v<typename desugar_t<T>::fields>>{});
				}

				template<typename T>
				typename Algorithm::result_type process_type(Obj<S>... objs, Obj<S>... parents) {
					return algorithm.visit_type(objs..., TypeInfo{ dynamic_align_of<T>(&parents), dynamic_size_of<T>(&parents, &objs) }..., [&](Obj<S>... objs) {
						if constexpr (desugar_t<T>::desc_type == DESCTYPE_STRUCTURE)
							return process_struct<T>(objs...);
						else if constexpr (desugar_t<T>::desc_type == DESCTYPE_UNION)
							return process_union<T>(objs..., parents...);
						else if constexpr (desugar_t<T>::desc_type == DESCTYPE_PRIMITIVE)
							return process_primitive<T>(objs...);
						else if constexpr (desugar_t<T>::desc_type == DESCTYPE_CONSTANT)
							return process_constant<T>(objs...);
						else if constexpr (desugar_t<T>::desc_type == DESCTYPE_ENUMERATION)
							return process_enumeration<T>(objs...);
						else if constexpr (desugar_t<T>::desc_type == DESCTYPE_POINTER)
							return process_pointer<T>(objs..., parents...);
						else if constexpr (desugar_t<T>::desc_type == DESCTYPE_DYNAMIC_CARRAY)
							return process_dynamic_carray<T>(objs..., parents...);
						else if constexpr (desugar_t<T>::desc_type == DESCTYPE_STATIC_CARRAY)
							return process_static_carray<T>(objs..., parents...);
						else if constexpr (desugar_t<T>::desc_type == DESCTYPE_RFLCLASS)
							return process_rflclass<T>(objs..., parents...);
						else if constexpr (desugar_t<T>::desc_type == DESCTYPE_COMPONENT_DATA)
							return process_component_data<T>(objs..., parents...);
						else if constexpr (desugar_t<T>::desc_type == DESCTYPE_SPAWNER_DATA_RFLCLASS)
							return process_spawner_data<T>(objs..., parents...);
						else if constexpr (desugar_t<T>::desc_type == DESCTYPE_ARRAY)
							return process_array<T>(objs..., parents...);
						else if constexpr (desugar_t<T>::desc_type == DESCTYPE_TARRAY)
							return process_tarray<T>(objs..., parents...);
						else
							static_assert("unknown desc type");
					});
				}

				template<typename T>
				typename Algorithm::result_type process_field(Obj<S>... objs, Obj<S>... parents) {
					using Type = typename T::type;

					return algorithm.visit_field(*util::align(&objs, align_of_v<Type>)..., FieldInfo{ .name = T::name, .erased = ucsl::reflection::is_erased_v<Type> }, [&](Obj<S>... objs) { return process_type<Type>(objs..., parents...); });
				}

				template<typename T>
				typename Algorithm::result_type process_fields(Obj<S>... objs) {
					using Base = typename T::base;
					using Fields = typename T::fields;

					typename Algorithm::result_type result{};
					size_t offset{};

					if constexpr (!std::is_same_v<Base, primitive<void>>) {
						result |= process_base_struct<Base>(objs...);
						offset = size_of_v<Base>;
					}

					result |= FieldIterator<Fields>{ *this }(objs..., objs..., offset);

					return result;
				}

				template<typename T>
				typename Algorithm::result_type process_base_struct(Obj<S>... objs) {
					return algorithm.visit_base_struct(objs..., StructureInfo{ T::name, align_of_v<T> }, [&](Obj<S>... objs) { return process_fields<T>(objs...); });
				}

				template<typename T>
				typename Algorithm::result_type process_struct(Obj<S>... objs) {
					static_assert(desugar_t<T>::desc_type == DESCTYPE_STRUCTURE);
					return algorithm.visit_struct(objs..., StructureInfo{ T::name, align_of_v<T> }, [&](Obj<S>... objs) { return process_fields<T>(objs...); });
				}

				template<typename T>
				typename Algorithm::result_type process_root(Obj<S>... objs) {
					return algorithm.visit_root(objs..., RootInfo{ dynamic_align_of<T>(&objs...), dynamic_size_of<T>(&objs..., &objs...)}, [&](Obj<S>...objs) { return process_type<T>(objs..., objs...); });
				}

			public:
				template<typename ...Args>
				traversal(Args&& ...objs) : algorithm{ std::forward<Args>(objs)... } {}

				//template<typename T>
				//typename Algorithm::result_type operator()(Obj<S>... objs) {
				//	return process_root<T>(objs...);
				//}

				template<typename T>
				typename Algorithm::result_type operator()(Obj<S, T>... objs) {
					return process_root<simplerfl::canonical_t<T>>(((Obj<S>)objs)...);
				}

				template<typename T, typename ...Args>
				static typename Algorithm::result_type apply(Obj<S>... objs, Args&& ...args) {
					traversal<Algorithm> t{ std::forward<Args>(args)... };

					return t.operator()<T>(objs...);
				}

				//template<typename T, typename R, typename ...Args>
				//static typename Algorithm::result_type apply(Obj<S, R*>... objs, Args&& ...args) {
				//	return traversal<Algorithm>::apply(((void*)objs)..., std::forward<Args>(args)...);
				//}
			};
		};
	}

	template<typename GameInterface>
	using simplerfl = internal::simplerfl_traversal_for<GameInterface>::traversal;
}
