#pragma once
#include <ucsl/resources/pcmodel/v2.h>

namespace ucsl::resources::pcmodel::v2::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::pcmodel::v2;

	using ModelInstanceData = structure<impl::ModelInstanceData, "ModelInstanceData", void,
		field<const char*, "name">,
		field<const char*, "resourceName">,
		field<ucsl::math::Position, "position">,
		field<ucsl::math::Position, "rotation">,
		field<constant<unsigned int, 1>, "unk1">,
		field<ucsl::math::Position, "scale">,
		field<constant<unsigned int, 0>, "unk2">
	>;

	using PointCloudModelData = structure<impl::PointCloudModelData, "PointCloudModelData", void,
		field<constant<unsigned int, 0x43495043>, "magic">,
		field<constant<unsigned int, 2>, "version">,
		field<dynamic_carray<ModelInstanceData, impl::PointCloudModelData, [](const impl::PointCloudModelData& parent) -> size_t { return parent.instanceCount; }>*, "instances">,
		field<size_t, "instanceCount">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::pcmodel::v2::ModelInstanceData> { using type = ucsl::resources::pcmodel::v2::reflections::ModelInstanceData; };
	template<> struct canonical<ucsl::resources::pcmodel::v2::PointCloudModelData> { using type = ucsl::resources::pcmodel::v2::reflections::PointCloudModelData; };
}
