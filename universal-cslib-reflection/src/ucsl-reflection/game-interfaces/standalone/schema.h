#pragma once
#include <map>
#include <string>
#include <optional>
#include <ucsl/rfl/rflclass.h>
#include "rflsystem.h"

namespace ucsl::reflection::game_interfaces::standalone {
	struct Schema {
		struct ObjectInfo {
			std::optional<std::string> category{};
			std::optional<std::shared_ptr<typename StandaloneRflSystem::RflClass>> rfl_class{};
		};

		struct ComponentInfo {
			std::optional<std::shared_ptr<typename StandaloneRflSystem::RflClass>> rfl_class{};
		};

		std::map<std::string, std::shared_ptr<typename StandaloneRflSystem::RflClass>> classes{};
		std::map<std::string, ObjectInfo> objects{};
		std::map<std::string, ComponentInfo> components{};
	};
}