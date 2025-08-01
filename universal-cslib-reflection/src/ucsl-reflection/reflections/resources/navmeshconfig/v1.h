#pragma once
#include <ucsl/math.h>

namespace ucsl::resources::navmeshconfig::v1::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::navmeshconfig::v1;

	using Config = structure<impl::Config, "Config", void,
		field<ucsl::math::Position, "navMeshsize">,
		field<ucsl::math::Vector2, "size">,
		field<unsigned int, "tileCount">,
		field<unsigned int, "unk1">
	>;

	using NavMeshConfigData = structure<impl::NavMeshConfigData, "NavMeshConfigData", void,
		field<unsigned int, "magic">,
		field<unsigned int, "version">,
		field<Config, "config">
	>;
};

namespace simplerfl {
	template<> struct canonical<ucsl::resources::navmeshconfig::v1::Config>            { using type = ucsl::resources::navmeshconfig::v1::reflections::Config; };
	template<> struct canonical<ucsl::resources::navmeshconfig::v1::NavMeshConfigData> { using type = ucsl::resources::navmeshconfig::v1::reflections::NavMeshConfigData; };
}
