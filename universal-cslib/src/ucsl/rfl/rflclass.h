#pragma once
#include <span>
#include <ranges>
#include <optional>
#include <ucsl/colors.h>
#include <ucsl/math.h>
#include <ucsl/object-id.h>
#include <ucsl/containers/arrays/array.h>
#include <ucsl/containers/arrays/tarray.h>
#include <ucsl/strings/variable-string.h>

namespace ucsl::rfl {
	template<typename T>
	class RflArray {
		const T* const items{};
		const unsigned int count{};
	public:
		std::span<const T> GetItems() const { return { items, count }; }
	};

	template<typename TypeSet, typename RflClass>
	struct common {
		class RflCustomAttribute {
			const char* const name{};
			const void* const data{};
			const RflClass* const type{};

		public:
			const char* GetName() const { return name; }
			template<typename T> const T* GetData() const { return static_cast<const T*>(data); }
			const RflClass* GetType() const { return type; }
		};

		class RflCustomAttributes {
			const RflArray<const RflCustomAttribute> items{};

		public:
			const RflCustomAttribute* GetAttribute(const char* name) const {
				auto s = items.GetItems();
				auto res = std::find_if(s.begin(), s.end(), [name](auto& attr) { return attr.name == name || !strcmp(attr.name, name); });

				return res == s.end() ? nullptr : &*res;
			}
		};

		class RflClassEnumMember {
			int index{};
			const char* const englishName{};
			const char* const japaneseName{};
			const unsigned long long flags{};

		public:
			int GetIndex() const { return index; }
			const char* GetEnglishName() const { return englishName; }
			const char* GetJapaneseName() const { return japaneseName; }
			unsigned long long GetFlags() const { return flags; }
		};

		class RflClassEnum {
			const char* const name{};
			const RflArray<const RflClassEnumMember> values{};

		public:
			const char* GetName() const { return name; }
			std::span<const RflClassEnumMember> GetValues() const { return values.GetItems(); }
		};

		class RflClassMember {
			using Type = typename TypeSet::MemberType;

			const char* name{};
			const RflClass* const classDef{};
			const RflClassEnum* const enumDef{};
			const Type type{};
			const Type subType{};
			const unsigned int arrayLength{};
			const unsigned int flags{};
			const unsigned int offset{};
			const RflCustomAttributes* const attributes{};

		public:
			const char* GetName() const { return name; }
			const RflClass* GetClass() const { return classDef; }
			const RflClassEnum* GetEnum() const { return enumDef; }
			Type GetType() const { return type; }
			Type GetSubType() const { return subType; }
			unsigned int GetArrayLength() const { return arrayLength; }
			unsigned int GetFlags() const { return flags; }
			unsigned int GetOffset() const { return offset; }
			const RflCustomAttributes* GetAttributes() const { return attributes; }
			const RflCustomAttribute* GetAttribute(const char* name) const {
				auto* attributes = GetAttributes();

				return attributes == nullptr ? nullptr : attributes->GetAttribute(name);
			}
			std::optional<std::span<const RflClassEnumMember>> GetFlagValues() const {
				auto* attr = GetAttribute("DisplayIndex");
				auto* flags = attr == nullptr ? nullptr : static_cast<const RflArray<const RflClassEnumMember>*>(attr->GetData());

				return flags == nullptr ? std::nullopt : flags->GetItems();
			}
			size_t GetSubTypeSize() const {
				switch (GetSubType()) {
				case Type::VOID:
					return 0;
				case Type::STRUCT:
					return GetClass()->GetSize();
				default:
					return TypeSet::metadata[GetSubType()].size;
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
					return TypeSet::metadata[GetType()].size;
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
					return TypeSet::metadata[GetSubType()].alignment;
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
					return TypeSet::metadata[GetType()].alignment;
				}
			}
		};
	};

	template<typename TS>
	struct v1 {
		class RflClass;

		using TypeSet = TS;
		using RflCustomAttribute = typename common<TypeSet, RflClass>::RflCustomAttribute;
		using RflCustomAttributes = typename common<TypeSet, RflClass>::RflCustomAttributes;
		using RflClassEnumMember = typename common<TypeSet, RflClass>::RflClassEnumMember;
		using RflClassEnum = typename common<TypeSet, RflClass>::RflClassEnum;
		using RflClassMember = typename common<TypeSet, RflClass>::RflClassMember;

		class RflClass {
			const char* name{};
			const RflClass* const parent{};
			const unsigned int size{};
			const RflArray<const RflClassEnum> enums{};
			const RflArray<const RflClassMember> members{};
			const RflCustomAttributes* const attributes{};

		public:
			const char* GetName() const { return name; }
			const RflClass* GetParent() const { return parent; }
			unsigned int GetSize() const { return size; }
			std::span<const RflClassEnum> GetEnums() const { return enums.GetItems(); }
			std::span<const RflClassMember> GetMembers() const { return members.GetItems(); }
			const RflCustomAttributes* GetAttributes() const { return attributes; }
			const RflCustomAttribute* GetAttribute(const char* name) const {
				auto* attributes = GetAttributes();

				return attributes == nullptr ? nullptr : attributes->GetAttribute(name);
			}
			size_t GetAlignment() const {
				size_t maxAlignment = 0;

				if (GetParent())
					maxAlignment = GetParent()->GetAlignment();

				for (auto& member : GetMembers()) {
					size_t memberAlignment = member.GetAlignment();

					if (memberAlignment > maxAlignment)
						maxAlignment = memberAlignment;
				}

				return maxAlignment;
			}
		};
	};

	template<typename TS>
	struct v2 {
		class RflClass;

		using TypeSet = TS;
		using RflCustomAttribute = typename common<TypeSet, RflClass>::RflCustomAttribute;
		using RflCustomAttributes = typename common<TypeSet, RflClass>::RflCustomAttributes;
		using RflClassEnumMember = typename common<TypeSet, RflClass>::RflClassEnumMember;
		using RflClassEnum = typename common<TypeSet, RflClass>::RflClassEnum;
		using RflClassMember = typename common<TypeSet, RflClass>::RflClassMember;

		class RflClass {
			const char* name{};
			const RflClass* const parent{};
			const unsigned int size{};
			const RflArray<const RflClassEnum> enums{};
			const RflArray<const RflClassMember> members{};
			const RflCustomAttributes* const attributes{};
			const unsigned int nameHash{};

		public:
			const char* GetName() const { return name; }
			const RflClass* GetParent() const { return parent; }
			unsigned int GetSize() const { return size; }
			std::span<const RflClassEnum> GetEnums() const { return enums.GetItems(); }
			std::span<const RflClassMember> GetMembers() const { return members.GetItems(); }
			const RflCustomAttributes* GetAttributes() const { return attributes; }
			const RflCustomAttribute* GetAttribute(const char* name) const {
				auto* attributes = GetAttributes();

				return attributes == nullptr ? nullptr : attributes->GetAttribute(name);
			}
			unsigned int GetNameHash() const { return nameHash; }
			size_t GetAlignment() const {
				size_t maxAlignment = 0;

				if (GetParent())
					maxAlignment = GetParent()->GetAlignment();

				for (auto& member : GetMembers()) {
					size_t memberAlignment = member.GetAlignment();

					if (memberAlignment > maxAlignment)
						maxAlignment = memberAlignment;
				}

				return maxAlignment;
			}
		};
	};

	namespace type_sets {
		template<typename MemberType>
		struct Metadata {
			MemberType id;
			const char* name;
			size_t size;
			size_t alignment;
		};

		struct wars {
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
				OBJECT_ID,
				POSITION,
				COLOR_BYTE,
				COLOR_FLOAT,
			};

			using ObjectId = objectids::ObjectIdV1;

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
				{ MemberType::ARRAY, "array", sizeof(containers::arrays::Array<void*>), alignof(containers::arrays::Array<void*>) },
				{ MemberType::OLD_ARRAY, "oldarray", 0x18, 0x8 },
				{ MemberType::SIMPLE_ARRAY, "simplearray", 0x10, 0x8 },
				{ MemberType::ENUM, "enum", (size_t)-1, (size_t)-1 },
				{ MemberType::STRUCT, "struct", (size_t)-1, (size_t)-1 },
				{ MemberType::FLAGS, "flags", (size_t)-1, (size_t)-1 },
				{ MemberType::CSTRING, "cstring", sizeof(const char*), alignof(const char*) },
				{ MemberType::STRING, "string", sizeof(strings::VariableString), alignof(strings::VariableString) },
				{ MemberType::OBJECT_ID, "csetobjectid", sizeof(ObjectId), alignof(ObjectId) },
				{ MemberType::POSITION, "position", sizeof(math::Position), alignof(math::Position) },
				{ MemberType::COLOR_BYTE, "color8", sizeof(colors::Color8), alignof(colors::Color8) },
				{ MemberType::COLOR_FLOAT, "colorf", sizeof(colors::Colorf), alignof(colors::Colorf) },
			};

			static constexpr bool supports_old_array = true;
		};

		struct rangers {
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
				SIMPLE_ARRAY,
				ENUM,
				STRUCT,
				FLAGS,
				CSTRING,
				STRING,
				OBJECT_ID,
				POSITION,
				COLOR_BYTE,
				COLOR_FLOAT,
			};

			using ObjectId = objectids::ObjectIdV2;

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
				{ MemberType::ARRAY, "array", sizeof(containers::arrays::Array<void*>), alignof(containers::arrays::Array<void*>) },
				{ MemberType::SIMPLE_ARRAY, "simplearray", 0x10, 0x8 },
				{ MemberType::ENUM, "enum", (size_t)-1, (size_t)-1 },
				{ MemberType::STRUCT, "struct", (size_t)-1, (size_t)-1 },
				{ MemberType::FLAGS, "flags", (size_t)-1, (size_t)-1 },
				{ MemberType::CSTRING, "cstring", sizeof(const char*), alignof(const char*) },
				{ MemberType::STRING, "string", sizeof(strings::VariableString), alignof(strings::VariableString) },
				{ MemberType::OBJECT_ID, "csetobjectid", sizeof(ObjectId), alignof(ObjectId) },
				{ MemberType::POSITION, "position", sizeof(math::Position), alignof(math::Position) },
				{ MemberType::COLOR_BYTE, "color8", sizeof(colors::Color8), alignof(colors::Color8) },
				{ MemberType::COLOR_FLOAT, "colorf", sizeof(colors::Colorf), alignof(colors::Colorf) },
			};

			static constexpr bool supports_old_array = false;
		};
	}

	using wars = v1<type_sets::wars>;
	using rangers = v1<type_sets::rangers>;
	using miller = v2<type_sets::rangers>;
}
