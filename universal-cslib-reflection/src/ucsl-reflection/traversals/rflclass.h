#pragma once
#include <tuple>
#include <ucsl/containers/arrays/array.h>
#include <ucsl/containers/arrays/tarray.h>
#include <ucsl/rfl/rflclass.h>
#include <ucsl/math.h>
#include <ucsl/colors.h>
#include <ucsl/strings/variable-string.h>
#include <ucsl-reflection/opaque.h>
#include "types.h"

namespace ucsl::reflection::traversals {
	namespace internal {
		template<typename GameInterface>
		struct rflclass_traversal_for {
			template<typename Algorithm, typename = std::make_index_sequence<Algorithm::arity>>
			class traversal;
			template<typename Algorithm, std::size_t... S>
			class traversal<Algorithm, std::index_sequence<S...>> {
				template<std::size_t, typename T> using Spread = T;
				template<std::size_t, typename T = opaque_obj> using Obj = T&;

				Algorithm algorithm;

				using MemberType = typename GameInterface::RflSystem::RflClassMember::Type;

				template<typename U>
				typename Algorithm::result_type process_primitive(Obj<S>... objs) {
					return algorithm.visit_primitive(reinterpret_cast<U&>(objs)..., PrimitiveInfo<U>{ alignof(U), sizeof(U), false, std::nullopt });
				}

				typename Algorithm::result_type process_primitive(Obj<S>... objs, const MemberType type) {
					switch (type) {
					case MemberType::BOOL: return process_primitive<bool>(objs...);
					case MemberType::SINT8: return process_primitive<int8_t>(objs...);
					case MemberType::UINT8: return process_primitive<uint8_t>(objs...);
					case MemberType::SINT16: return process_primitive<int16_t>(objs...);
					case MemberType::UINT16: return process_primitive<uint16_t>(objs...);
					case MemberType::SINT32: return process_primitive<int32_t>(objs...);
					case MemberType::UINT32: return process_primitive<uint32_t>(objs...);
					case MemberType::SINT64: return process_primitive<int64_t>(objs...);
					case MemberType::UINT64: return process_primitive<uint64_t>(objs...);
					case MemberType::FLOAT: return process_primitive<float>(objs...);
					case MemberType::VECTOR2: return process_primitive<math::Vector2>(objs...);
					case MemberType::VECTOR3: return process_primitive<math::Vector3>(objs...);
					case MemberType::VECTOR4: return process_primitive<math::Vector4>(objs...);
					case MemberType::QUATERNION: return process_primitive<math::Quaternion>(objs...);
					case MemberType::MATRIX34: return process_primitive<math::Matrix34>(objs...);
					case MemberType::MATRIX44: return process_primitive<math::Matrix44>(objs...);
					case MemberType::CSTRING: return process_primitive<const char*>(objs...);
					case MemberType::STRING: return process_primitive<strings::VariableString>(objs...);
					case MemberType::OBJECT_ID: return process_primitive<typename GameInterface::RflSystem::TypeSet::ObjectId>(objs...);
					case MemberType::COLOR_BYTE: return process_primitive<colors::Color8>(objs...);
					case MemberType::COLOR_FLOAT: return process_primitive<colors::Colorf>(objs...);
					case MemberType::POSITION: return process_primitive<math::Position>(objs...);
					default:
						assert(!"reflective operation assertion failed: unknown primitive type");
						return typename Algorithm::result_type{};
					}
				}

				typename Algorithm::result_type process_single(Obj<S>... objs, const GameInterface::RflSystem::RflClassMember* member, const MemberType type)
				{
					switch (type) {
					case MemberType::STRUCT:
						return process_struct(objs..., member->GetClass());
					default:
						return process_primitive(objs..., type);
					}
				}

				typename Algorithm::result_type process_array(Obj<S>... objs, const GameInterface::RflSystem::RflClassMember* member)
				{
					std::tuple<Spread<S, OpaqueRflArray<containers::arrays::Array, GameInterface>>...> arrs{ { reinterpret_cast<containers::arrays::Array<opaque_obj>&>(objs), member }... };

					return algorithm.visit_array(
						std::get<S>(arrs)...,
						ArrayInfo{ member->GetSubTypeSize(), member->GetSubTypeAlignment() },
						[&]() {
							void* obj = GameInterface::get_fallback_allocator()->Alloc(member->GetSubTypeSize(), 16);

							if (member->GetSubType() == MemberType::STRUCT)
								GameInterface::RflTypeInfoRegistry::GetInstance()->ConstructObject(GameInterface::get_fallback_allocator(), obj, member->GetClass()->GetName());

							return obj;
						},
						[&](void* obj) {
							if (member->GetSubType() == MemberType::STRUCT)
								GameInterface::RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(obj, member->GetClass()->GetName());

							GameInterface::get_fallback_allocator()->Free(obj);
						},
						[&](Obj<S>... objs) { return process_single(objs..., member, member->GetSubType()); }
					);
				}

				typename Algorithm::result_type process_old_array(Obj<S>... objs, const GameInterface::RflSystem::RflClassMember* member)
				{
					std::tuple<Spread<S, OpaqueRflArray<GameInterface::template TArray, GameInterface>>...> arrs{ { reinterpret_cast<typename GameInterface::template TArray<opaque_obj>&>(objs), member }... };

					return algorithm.visit_array(
						std::get<S>(arrs)...,
						ArrayInfo{ member->GetSubTypeSize(), member->GetSubTypeAlignment() },
						[&]() {
							void* obj = GameInterface::get_fallback_allocator()->Alloc(member->GetSubTypeSize(), 16);

							if (member->GetSubType() == MemberType::STRUCT)
								GameInterface::RflTypeInfoRegistry::GetInstance()->ConstructObject(GameInterface::get_fallback_allocator(), obj, member->GetClass()->GetName());

							return obj;
						},
						[&](void* obj) {
							if (member->GetSubType() == MemberType::STRUCT)
								GameInterface::RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(obj, member->GetClass()->GetName());

							GameInterface::get_fallback_allocator()->Free(obj);
						},
						[&](Obj<S>... objs) { return process_single(objs..., member, member->GetSubType() == MemberType::UINT32 ? MemberType::OBJECT_ID : member->GetSubType()); } // We're overriding this here because Forces seems to use Type::OLD_ARRAY,Type::UINT32 for object ID arrays...
					);
				}

				typename Algorithm::result_type process_enum(Obj<S>... objs, const GameInterface::RflSystem::RflClassMember* member) {
					return algorithm.visit_enum(objs..., EnumInfo{ member->GetEnum()->GetValues() }, [&](Obj<S>... objs) { return process_single(objs..., member, member->GetSubType()); });
				}

				typename Algorithm::result_type process_flags(Obj<S>... objs, const GameInterface::RflSystem::RflClassMember* member) {
					return algorithm.visit_flags(objs..., FlagsInfo{ member->GetFlagValues() }, [&](Obj<S>... objs) { return process_single(objs..., member, member->GetSubType()); });
				}

				typename Algorithm::result_type process_pointer(Obj<S>... objs, const GameInterface::RflSystem::RflClassMember* member) {
					return algorithm.visit_pointer(reinterpret_cast<Obj<S, opaque_obj*>>(objs)..., PointerInfo{ member->GetSubTypeAlignment(), member->GetSubTypeSize() }, [&](Obj<S>... objs) { return process_single(objs..., member, member->GetSubType()); });
				}

				typename Algorithm::result_type process_type(Obj<S>... objs, const GameInterface::RflSystem::RflClassMember* member) {
					return algorithm.visit_type(objs..., TypeInfo{ member->GetAlignment(), member->GetSize() }, [&](Obj<S>... objs) {
						if constexpr (GameInterface::RflSystem::TypeSet::supports_old_array)
							if (member->GetType() == MemberType::OLD_ARRAY)
								return process_old_array(objs..., member);

						switch (member->GetType()) {
						case MemberType::ARRAY: return process_array(objs..., member);
						case MemberType::POINTER: return process_pointer(objs..., member);
						case MemberType::ENUM: return process_enum(objs..., member);
						case MemberType::FLAGS: return process_flags(objs..., member);
						case MemberType::SIMPLE_ARRAY:
							assert(!"This RflClass member type (SIMPLE_ARRAY) is not implemented yet because it is unused.");
							return typename Algorithm::result_type{};
						default: return process_single(objs..., member, member->GetType());
						}
					});
				}

				typename Algorithm::result_type process_class_member(Obj<S>... objs, const GameInterface::RflSystem::RflClassMember* member) {
					return algorithm.visit_field(objs..., FieldInfo{ member->GetName() }, [&](Obj<S>... objs) {
						size_t constArrSizeOrZero = member->GetArrayLength();

						return constArrSizeOrZero == 0
							? process_type(objs..., member)
							: algorithm.visit_carray(&objs..., CArrayInfo{ constArrSizeOrZero, member->GetSingleSize() }, [&](Obj<S>... objs) { return process_type(objs..., member); });
					});
				}

				typename Algorithm::result_type process_class_members(Obj<S>... objs, const GameInterface::RflSystem::RflClass* rflClass) {
					const GameInterface::RflSystem::RflClass* parent = rflClass->GetParent();

					typename Algorithm::result_type result{};

					if (parent != nullptr)
						result |= process_base_struct(objs..., parent);

					for (const auto& member : rflClass->GetMembers())
						result |= process_class_member(*util::addptr(&objs, member.GetOffset())..., &member);

					return result;
				}

				typename Algorithm::result_type process_base_struct(Obj<S>... objs, const GameInterface::RflSystem::RflClass* rflClass) {
					return algorithm.visit_base_struct(objs..., StructureInfo{ rflClass->GetName(), rflClass->GetAlignment() }, [&](Obj<S>... objs) { return process_class_members(objs..., rflClass); });
				}

			public:
				template<typename ...Args>
				traversal(Args&& ...objs) : algorithm{ std::forward<Args>(objs)... } {}

				typename Algorithm::result_type operator()(Obj<S>... objs, const GameInterface::RflSystem::RflClass* rflClass) {
					return process_struct(objs..., rflClass);
				}

				typename Algorithm::result_type process_struct(Obj<S>... objs, const GameInterface::RflSystem::RflClass* rflClass) {
					return algorithm.visit_struct(objs..., StructureInfo{ rflClass->GetName(), rflClass->GetAlignment() }, [&](Obj<S>... objs) { return process_class_members(objs..., rflClass); });
				}

				//template<typename ...Args>
				//static typename Algorithm::result_type apply(Obj<S>... objs, const GameInterface::RflSystem::RflClass* rflClass, Args&& ...objs) {
				//	return rflclass_traversal<Algorithm>{ std::forward<Args>(objs)... }(objs..., rflClass);
				//}
				//template<typename R, typename ...Args>
				//static typename Algorithm::result_type apply(Obj<S, R*>... objs, Args&& ...objs) {
				//	return rflclass_traversal<Algorithm>(objs..., &R::rflClass, std::forward<Args>(objs)...);
				//}
			};
		};
	};

	template<typename GameInterface>
	using rflclass = internal::rflclass_traversal_for<GameInterface>::traversal;
}
