#pragma once
#include <string>
#include <vector>
#include <optional>
#include <ucsl/rfl/rflclass.h>

namespace ucsl::reflection::game_interfaces::standalone {
	struct StandaloneRflSystem {
		class RflClass;

		struct TypeSet {
			enum class MemberType : unsigned char {
				VOID,
				BOOL,
				SINT8,
				UINT8,
				SINT16,
				UINT16,
				SINT32,
				UINT32,
				SINT64,
				UINT64,
				FLOAT,
				VECTOR2,
				VECTOR3,
				VECTOR4,
				QUATERNION,
				MATRIX34,
				MATRIX44,
				POINTER,
				ARRAY,
				OLD_ARRAY,
				SIMPLE_ARRAY,
				ENUM,
				STRUCT,
				FLAGS,
				CSTRING,
				STRING,
				OBJECT_ID_V1,
				OBJECT_ID_V2,
				POSITION,
				COLOR_BYTE,
				COLOR_FLOAT,
			};

			inline static const Metadata<MemberType> metadata[] = {
				{ MemberType::VOID, "void", (size_t)-1, (size_t)-1 },
				{ MemberType::BOOL, "bool", sizeof(bool), alignof(bool) },
				{ MemberType::SINT8, "sint8", sizeof(char), alignof(char) },
				{ MemberType::UINT8, "uint8", sizeof(unsigned char), alignof(unsigned char) },
				{ MemberType::SINT16, "sint16", sizeof(short), alignof(short) },
				{ MemberType::UINT16, "uint16", sizeof(unsigned short), alignof(unsigned short) },
				{ MemberType::SINT32, "sint32", sizeof(int), alignof(int) },
				{ MemberType::UINT32, "uint32", sizeof(unsigned int), alignof(unsigned int) },
				{ MemberType::SINT64, "sint64", sizeof(long long), alignof(long long) },
				{ MemberType::UINT64, "uint64", sizeof(unsigned long long), alignof(unsigned long long) },
				{ MemberType::FLOAT, "float", sizeof(float), alignof(float) },
				{ MemberType::VECTOR2, "vector2", sizeof(math::Vector2), alignof(math::Vector2) },
				{ MemberType::VECTOR3, "vector3", sizeof(math::Vector3), alignof(math::Vector3) },
				{ MemberType::VECTOR4, "vector4", sizeof(math::Vector4), alignof(math::Vector4) },
				{ MemberType::QUATERNION, "quaternion", sizeof(math::Quaternion), alignof(math::Quaternion) },
				{ MemberType::MATRIX34, "matrix34", sizeof(math::Matrix34), alignof(math::Matrix34) },
				{ MemberType::MATRIX44, "matrix44", sizeof(math::Matrix44), alignof(math::Matrix44) },
				{ MemberType::POINTER, "pointer", sizeof(void*), alignof(void*) },
				{ MemberType::ARRAY, "array", sizeof(containers::arrays::Array<size_t, ucsl::containers::arrays::dummy_allocator_system>), alignof(containers::arrays::Array<size_t, ucsl::containers::arrays::dummy_allocator_system>) },
				{ MemberType::OLD_ARRAY, "oldarray", sizeof(containers::arrays::TArray<size_t, ucsl::containers::arrays::dummy_allocator_system>), alignof(containers::arrays::TArray<size_t, ucsl::containers::arrays::dummy_allocator_system>) },
				{ MemberType::SIMPLE_ARRAY, "simplearray", 0x10, 0x8 },
				{ MemberType::ENUM, "enum", (size_t)-1, (size_t)-1 },
				{ MemberType::STRUCT, "struct", (size_t)-1, (size_t)-1 },
				{ MemberType::FLAGS, "flags", (size_t)-1, (size_t)-1 },
				{ MemberType::CSTRING, "cstring", sizeof(const char*), alignof(const char*) },
				{ MemberType::STRING, "string", sizeof(strings::VariableString), alignof(strings::VariableString) },
				{ MemberType::OBJECT_ID_V1, "csetobjectidv1", sizeof(objectids::ObjectIdV1), alignof(objectids::ObjectIdV1) },
				{ MemberType::OBJECT_ID_V2, "csetobjectidv2", sizeof(objectids::ObjectIdV2), alignof(objectids::ObjectIdV2) },
				{ MemberType::POSITION, "position", sizeof(math::Position), alignof(math::Position) },
				{ MemberType::COLOR_BYTE, "color8", sizeof(colors::Color8), alignof(colors::Color8) },
				{ MemberType::COLOR_FLOAT, "colorf", sizeof(colors::Colorf), alignof(colors::Colorf) },
			};

			static constexpr bool supports_old_array = true;
			static constexpr bool supports_object_id_v1 = true;
			static constexpr bool supports_object_id_v2 = true;
		};

		class RflClassEnumMember {
		public:
			int index{};
			std::string englishName{};
			std::string japaneseName{};

			int GetIndex() const { return index; }
			const char* GetEnglishName() const { return englishName.c_str(); }
			const char* GetJapaneseName() const { return japaneseName.c_str(); }
		};

		class RflClassEnum {
		public:
			std::string name{};
			std::vector<RflClassEnumMember> values{};

			const char* GetName() const { return name.c_str(); }
			const std::vector<RflClassEnumMember>& GetValues() const { return values; }
		};

		class RflClassMember {
		public:
			using Type = typename TypeSet::MemberType;

			std::string name{};
			std::optional<std::shared_ptr<RflClass>> classDef{};
			std::optional<std::shared_ptr<RflClassEnum>> enumDef{};
			std::optional<std::vector<RflClassEnumMember>> flagValues{};
			Type type{};
			Type subType{};
			unsigned int arrayLength{};
			unsigned int offset{};

			const char* GetName() const { return name.c_str(); }
			const RflClass* GetClass() const { return classDef.has_value() ? &*classDef.value() : nullptr; }
			const RflClassEnum* GetEnum() const { return enumDef.has_value() ? &*enumDef.value() : nullptr; }
			Type GetType() const { return type; }
			Type GetSubType() const { return subType; }
			unsigned int GetArrayLength() const { return arrayLength; }
			unsigned int GetOffset() const { return offset; }
			const std::optional<std::vector<RflClassEnumMember>>& GetFlagValues() const { return flagValues; }
			template<typename Range>
			const Range* GetRange() const { return nullptr; }
			const char* GetCaption() const { return nullptr; }
			size_t GetSubTypeSize() const {
				switch (GetSubType()) {
				case Type::VOID:
					return 0;
				case Type::STRUCT:
					return GetClass()->GetSize();
				default:
					return TypeSet::metadata[static_cast<unsigned int>(GetSubType())].size;
				}
			}
			size_t GetSingleSize() const {
				switch (GetType()) {
				case Type::VOID:
					return 0;
				case Type::STRUCT:
					return GetClass()->GetSize();
				case Type::ENUM:
				case Type::FLAGS:
					return GetSubTypeSize();
				default:
					return TypeSet::metadata[static_cast<unsigned int>(GetType())].size;
				}
			}
			size_t GetSize() const {
				return GetArrayLength() ? GetArrayLength() * GetSingleSize() : GetSingleSize();
			}
			size_t GetSubTypeAlignment() const {
				switch (GetSubType()) {
				case Type::VOID:
					return 0;
				case Type::STRUCT:
					return GetClass()->GetAlignment();
				default:
					return TypeSet::metadata[static_cast<unsigned int>(GetSubType())].alignment;
				}
			}
			size_t GetAlignment() const {
				switch (GetType()) {
				case Type::VOID:
					return 0;
				case Type::STRUCT:
					return GetClass()->GetAlignment();
				case Type::ENUM:
				case Type::FLAGS:
					return GetSubTypeAlignment();
				default:
					return TypeSet::metadata[static_cast<unsigned int>(GetType())].alignment;
				}
			}
		};

		class RflClass {
		public:
			std::string name{};
			std::optional<std::shared_ptr<RflClass>> parent{};
			unsigned int size{};
			std::vector<std::shared_ptr<RflClassEnum>> enums{};
			std::vector<std::shared_ptr<RflClassMember>> members{};
			unsigned int nameHash{};

			const char* GetName() const { return name.c_str(); }
			const RflClass* GetParent() const { return parent.has_value() ? &*parent.value() : nullptr; }
			unsigned int GetSize() const { return size; }
			auto GetEnums() const { return std::views::all(enums) | std::views::transform([](const auto& e) -> const RflClassEnum& { return *e; }); }
			auto GetMembers() const { return std::views::all(members) | std::views::transform([](const auto& m) -> const RflClassMember& { return *m; }); }
			unsigned int GetNameHash() const { return nameHash; }
			size_t GetAlignment() const {
				size_t maxAlignment = 0;

				if (GetParent())
					maxAlignment = GetParent()->GetAlignment();

				for (const auto& member : GetMembers()) {
					size_t memberAlignment = member.GetAlignment();

					if (memberAlignment > maxAlignment)
						maxAlignment = memberAlignment;
				}

				return maxAlignment;
			}
		};
	};
}
