#pragma once
#include <cassert>
#include <ucsl/containers/arrays/array.h>
#include <ucsl/containers/arrays/tarray.h>
#include <ucsl/rfl/rflclass.h>
#include <ucsl/math.h>
#include <ucsl/colors.h>
#include <ucsl/strings/variable-string.h>
#include <ucsl-reflection/opaque.h>
#include <ucsl-reflection/util/memory.h>
#include <vector>
#include "types.h"

namespace ucsl::reflection::providers {
	template<typename GameInterface>
	struct rflclass {
		using MemberType = typename GameInterface::RflSystem::RflClassMember::Type;

		struct Primitive {
			MemberType type;

			constexpr static TypeKind kind = TypeKind::PRIMITIVE;
			template<typename F>
			auto visit(F f) {
				switch (type) {
				case MemberType::BOOL: return f(PrimitiveData<bool>{});
				case MemberType::SINT8: return f(PrimitiveData<int8_t>{});
				case MemberType::UINT8: return f(PrimitiveData<uint8_t>{});
				case MemberType::SINT16: return f(PrimitiveData<int16_t>{});
				case MemberType::UINT16: return f(PrimitiveData<uint16_t>{});
				case MemberType::SINT32: return f(PrimitiveData<int32_t>{});
				case MemberType::UINT32: return f(PrimitiveData<uint32_t>{});
				case MemberType::SINT64: return f(PrimitiveData<int64_t>{});
				case MemberType::UINT64: return f(PrimitiveData<uint64_t>{});
				case MemberType::FLOAT: return f(PrimitiveData<float>{});
				case MemberType::VECTOR2: return f(PrimitiveData<math::Vector2>{});
				case MemberType::VECTOR3: return f(PrimitiveData<math::Vector3>{});
				case MemberType::VECTOR4: return f(PrimitiveData<math::Vector4>{});
				case MemberType::QUATERNION: return f(PrimitiveData<math::Quaternion>{});
				case MemberType::MATRIX34: return f(PrimitiveData<math::Matrix34>{});
				case MemberType::MATRIX44: return f(PrimitiveData<math::Matrix44>{});
				case MemberType::CSTRING: return f(PrimitiveData<const char*>{});
				case MemberType::STRING: return f(PrimitiveData<strings::VariableString>{});
				case MemberType::OBJECT_ID: return f(PrimitiveData<typename GameInterface::RflSystem::TypeSet::ObjectId>{});
				case MemberType::COLOR_BYTE: return f(PrimitiveData<colors::Color8>{});
				case MemberType::COLOR_FLOAT: return f(PrimitiveData<colors::Colorf>{});
				case MemberType::POSITION: return f(PrimitiveData<math::Position>{});
				default: assert(!"reflective operation assertion failed: unknown primitive type"); return f(PrimitiveData<bool>{});
				}
			}
		};

		struct SubType;
		struct Type;

		struct Pointer {
			const GameInterface::RflSystem::RflClassMember* member;

			constexpr static TypeKind kind = TypeKind::POINTER;
			SubType get_target_type() const { return { member }; }
		};

		struct Array {
			const GameInterface::RflSystem::RflClassMember* member;

			constexpr static TypeKind kind = TypeKind::ARRAY;
			SubType get_item_type() const { return { member }; }
			constexpr auto get_accessor(opaque_obj& obj) const { return OpaqueRflArray<containers::arrays::Array, GameInterface>{ (containers::arrays::Array<opaque_obj>&)obj, member }; }
		};

		struct TArray {
			const GameInterface::RflSystem::RflClassMember* member;

			constexpr static TypeKind kind = TypeKind::TARRAY;
			SubType get_item_type() const { return { member }; }
			constexpr auto get_accessor(opaque_obj& obj) const { return OpaqueRflArray<GameInterface::template TArray, GameInterface>{ (GameInterface::template TArray<opaque_obj>&)obj, member }; }
		};

		struct CArray {
			const GameInterface::RflSystem::RflClassMember* member;

			constexpr static TypeKind kind = TypeKind::CARRAY;
			Type get_item_type() const { return { member, false }; }
			size_t get_length(opaque_obj& parent) const { return member->GetArrayLength(); }
		};

		struct Enum {
			const GameInterface::RflSystem::RflClassMember* member;

			constexpr static TypeKind kind = TypeKind::ENUM;
			Primitive get_underlying_primitive() const { return { member->GetSubType() }; }
			auto get_options() { return member->GetEnum()->GetValues(); }
		};

		struct Flags {
			const GameInterface::RflSystem::RflClassMember* member;

			constexpr static TypeKind kind = TypeKind::FLAGS;
			Primitive get_underlying_primitive() const { return { member->GetSubType() }; }
			auto get_values() { return member->GetFlagValues(); }
		};

		struct Field {
			const GameInterface::RflSystem::RflClassMember* member;
			
			const char* get_name() const { return member->GetName(); }
			size_t get_offset() const { return member->GetOffset(); }
			Type get_type() const { return { member, true }; }
		};

		struct Structure {
			const GameInterface::RflSystem::RflClass* rflClass;

			constexpr static TypeKind kind = TypeKind::STRUCTURE;
			const char* get_name() const { return rflClass->GetName(); }
			size_t get_alignment() const { return rflClass->GetAlignment(); }

			std::optional<Structure> get_base() const {
				auto* parent = rflClass->GetParent();
				
				return parent != nullptr ? std::make_optional(Structure{ parent }) : std::nullopt;
			}

			template<typename F>
			void visit_fields(const opaque_obj& obj, F f) const {
				for (const auto& member : rflClass->GetMembers())
					f(Field{ &member });
			}
		};

		struct SubType {
			const GameInterface::RflSystem::RflClassMember* member;

			bool is_erased() const { return false; }

			size_t get_size(const opaque_obj& parent, const opaque_obj& self) { return member->GetSubTypeSize(); }
			size_t get_alignment(const opaque_obj& parent) { return member->GetSubTypeAlignment(); }

			template<typename F>
			auto visit(const opaque_obj& parent, F f) const {
				switch (member->GetSubType()) {
				case MemberType::STRUCT: return f(Structure{ member->GetClass() });
				default: return f(Primitive{ member->GetSubType() });
				}
			}
		};

		struct Type {
			const GameInterface::RflSystem::RflClassMember* member;
			bool allowCArray;

			bool is_erased() const { return false; }

			size_t get_size(const opaque_obj& parent, const opaque_obj& self) {
				if (allowCArray) return member->GetSize();
				else return member->GetSingleSize();
			}
			size_t get_alignment(const opaque_obj& parent) { return member->GetAlignment(); }

			template<typename F>
			auto visit(const opaque_obj& parent, F f) const {
				if (allowCArray && member->GetArrayLength() > 0)
					return f(CArray{ member });

				if constexpr (GameInterface::RflSystem::TypeSet::supports_old_array)
					if (member->GetType() == MemberType::OLD_ARRAY)
						return f(TArray{ member });

				switch (member->GetType()) {
				case MemberType::ARRAY: return f(Array{ member });
				case MemberType::POINTER: return f(Pointer{ member });
				case MemberType::ENUM: return f(Enum{ member });
				case MemberType::FLAGS: return f(Flags{ member });
				case MemberType::SIMPLE_ARRAY: assert(!"This RflClass member type (SIMPLE_ARRAY) is not implemented yet because it is unused."); return f(Primitive{ MemberType::VOID });
				case MemberType::STRUCT: return f(Structure{ member->GetClass() });
				default: return f(Primitive{ member->GetType() });
				}
			}
		};

		static constexpr Structure reflect(const GameInterface::RflSystem::RflClass* rflClass) {
			return Structure{ rflClass };
		}
	};
}