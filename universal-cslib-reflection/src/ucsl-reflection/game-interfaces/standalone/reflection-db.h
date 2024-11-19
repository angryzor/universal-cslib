#pragma once
#include "rflsystem.h"
#include <map>

namespace ucsl::reflection::game_interfaces::standalone {
	template<typename RflSystem>
	class ReflectionDB {
	public:
		std::map<std::string, std::shared_ptr<const typename RflSystem::RflClass>> rflClasses{};
		std::map<std::string, std::string> spawnerDataRflClasses{};
	};
}
