#pragma once
#include "rflsystem.h"
#include "schema.h"
#include <map>

namespace ucsl::reflection::game_interfaces::standalone {
	class GameObjectClass {
	public:
		std::string name{};
		std::optional<std::string> category{};
		std::optional<std::shared_ptr<typename StandaloneRflSystem::RflClass>> spawnerDataRflClass{};

		inline const char* GetName() const { return name.c_str(); }
		template<typename T>
		inline const T* GetAttributeValue(const char* name) const {
			if constexpr (std::is_same_v<T, const char*>) {
				if (name == "category")
					return category.value();
			}
			return nullptr;
		}
		inline const typename StandaloneRflSystem::RflClass* GetSpawnerDataClass() const { return spawnerDataRflClass.has_value() ? &*spawnerDataRflClass.value() : nullptr; }
	};

	class GOComponentRegistryItem {
	public:
		std::string name{};
		std::optional<std::shared_ptr<typename StandaloneRflSystem::RflClass>> spawnerDataRflClass{};

		inline const char* GetName() const { return name.c_str(); }
		inline const typename StandaloneRflSystem::RflClass* GetSpawnerDataClass() const { return spawnerDataRflClass.has_value() ? &*spawnerDataRflClass.value() : nullptr; }
	};

	template<typename RflSystem>
	class ReflectionDB {
	public:
		std::map<std::string, std::shared_ptr<const typename RflSystem::RflClass>> rflClasses{};
		std::map<std::string, GameObjectClass> gameObjectClasses{};
		std::map<std::string, GOComponentRegistryItem> componentRegistryItems{};

		void load_schema(Schema schema) {
			for (auto& [name, classDef] : schema.classes)
				rflClasses[name] = classDef;

			for (auto& [name, objectDef] : schema.objects)
				gameObjectClasses[name] = { name, objectDef.category, objectDef.rfl_class };

			for (auto& [name, componentDef] : schema.components)
				componentRegistryItems[name] = { name, componentDef.rfl_class };
		}
	};
}
