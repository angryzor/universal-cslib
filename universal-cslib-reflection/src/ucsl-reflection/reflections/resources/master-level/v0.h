#pragma once
#include <ucsl/bitset.h>
#include <ucsl/math.h>

namespace ucsl::resources::master_level::v0::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::master_level::v0;

	using DependencyData = structure<impl::DependencyData, "DependencyData", void,
		field<const char*, "name">,
		field<unsigned long long, "unk1">
	>;

	using ResourceData = structure<impl::ResourceData, "ResourceData", void,
		field<const char*, "name">,
		field<const char*, "resourceType">,
		field<unsigned long long, "unk1">
	>;

	using LevelData = structure<impl::LevelData, "LevelData", void,
		field<const char*, "name">,
		field<unsigned int, "resourceCount">,
		field<unsigned int, "dependencyCount">,
		field<dynamic_carray<ResourceData*, impl::LevelData, [](const LevelData& mlevel) -> size_t { return mlevel.resourceCount; }>*, "resources">,
		field<dynamic_carray<DependencyData*, impl::LevelData, [](const LevelData& mlevel) -> size_t { return mlevel.dependencyCount; }>*, "dependencies">,
		field<bool, "isPublic">,
		field<bool, "hasFiles">
	>;

	using MasterLevelData = structure<impl::MasterLevelData, "MasterLevelData", void,
		field<unsigned int, "magic">,
		field<unsigned int, "version">,
		field<unsigned int, "levelCount">,
		field<dynamic_carray<LevelData*, impl::MasterLevelData, [](const MasterLevelData& mlevel) -> size_t { return mlevel.levelCount; }>*, "levels">
	>;
};

namespace simplerfl {
	template<> struct canonical<ucsl::resources::master_level::v0::DependencyData> { using type = ucsl::resources::master_level::v0::reflections::DependencyData; };
	template<> struct canonical<ucsl::resources::master_level::v0::ResourceData> { using type = ucsl::resources::master_level::v0::reflections::ResourceData; };
	template<> struct canonical<ucsl::resources::master_level::v0::LevelData> { using type = ucsl::resources::master_level::v0::reflections::LevelData; };
	template<> struct canonical<ucsl::resources::master_level::v0::MasterLevelData> { using type = ucsl::resources::master_level::v0::reflections::MasterLevelData; };
}
