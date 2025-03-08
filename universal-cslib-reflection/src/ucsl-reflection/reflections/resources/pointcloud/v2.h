#pragma once
#include <ucsl/resources/pointcloud/v2.h>

namespace ucsl::resources::pointcloud::v2::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::pointcloud::v2;

	using InstanceData = structure<impl::InstanceData, "InstanceData", void,
		field<const char*, "name">,
		field<const char*, "resourceName">,
		field<ucsl::math::Position, "position">,
		field<ucsl::math::Position, "rotation">,
		field<constant<unsigned int, 1>, "unk1">,
		field<ucsl::math::Position, "scale">,
		field<constant<unsigned int, 0>, "unk2">
	>;

	using PointcloudData = structure<impl::PointcloudData, "PointcloudData", void,
		field<constant<unsigned int, 0x43495043>, "magic">,
		field<constant<unsigned int, 2>, "version">,
		field<dynamic_carray<InstanceData, impl::PointcloudData, [](const impl::PointcloudData& parent) -> size_t { return parent.instanceCount; }>*, "instances">,
		field<size_t, "instanceCount">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::pointcloud::v2::InstanceData> { using type = ucsl::resources::pointcloud::v2::reflections::InstanceData; };
	template<> struct canonical<ucsl::resources::pointcloud::v2::PointcloudData> { using type = ucsl::resources::pointcloud::v2::reflections::PointcloudData; };
}
