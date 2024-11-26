#pragma once
#include <string>
#include <vector>
#include <optional>
#include <ucsl/rfl/rflclass.h>

namespace ucsl::reflection::game_interfaces::standalone {
	struct StandaloneRflSystem {
		class RflClass;

		using TypeSet = ucsl::rfl::type_sets::wars;
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
			size_t GetSubTypeSize() const {
				switch (GetSubType()) {
				case Type::VOID:
					return 0;
				case Type::STRUCT:
					return GetClass()->GetSize();
				default:
					return rfl::type_sets::rangers::metadata[static_cast<unsigned int>(GetSubType())].size;
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
					return rfl::type_sets::rangers::metadata[static_cast<unsigned int>(GetType())].size;
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
					return rfl::type_sets::rangers::metadata[static_cast<unsigned int>(GetSubType())].alignment;
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
					return rfl::type_sets::rangers::metadata[static_cast<unsigned int>(GetType())].alignment;
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
