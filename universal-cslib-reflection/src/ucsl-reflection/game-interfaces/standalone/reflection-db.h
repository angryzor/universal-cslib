#pragma once
#include "rflsystem.h"
#include "schema.h"
#include <map>

namespace ucsl::reflection::game_interfaces::standalone {
	template<typename RflSystem>
	class ReflectionDB {
	public:
		std::map<std::string, std::shared_ptr<const typename RflSystem::RflClass>> rflClasses{};
		std::map<std::string, std::string> spawnerDataRflClasses{};

		void load_schema(Schema schema) {
			for (auto& [name, classDef] : schema.classes)
				rflClasses[name] = classDef;

			for (auto& [name, objectDef] : schema.objects)
				spawnerDataRflClasses[name] = objectDef.class_name;
		}
	};
}
