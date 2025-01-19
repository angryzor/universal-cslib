#pragma once
#include <ucsl/bitset.h>
#include <ucsl/math.h>

namespace ucsl::resources::master_level::v0 {
	struct DependencyData {
		const char* name;
		unsigned long long unk1;
	};

	struct ResourceData {
		const char* name;
		const char* resourceType;
		unsigned long long unk1;
	};

	struct LevelData {
		const char* name;
		unsigned int resourceCount;
		unsigned int dependencyCount;
		ResourceData** resources;
		DependencyData** dependencies;
		bool loadImmediately;
		bool hasResources;
	};

	struct MasterLevelData {
		unsigned int magic;
		unsigned int version;
		unsigned int levelCount;
		LevelData** levels;
	};
};