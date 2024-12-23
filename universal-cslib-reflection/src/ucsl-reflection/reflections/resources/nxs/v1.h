#pragma once
#include <ucsl/resources/nxs/v1.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::nxs::v1::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::nxs::v1;

	using NXSData = structure<impl::NXSData, "NXSData", void,
		field<unsigned int, "magic">,
		field<unsigned int, "version">,
		field<unsigned int, "vertexCount">,
		field<unsigned int, "triangleCount">,
		field<size_t, "unk2Count">,
		field<size_t, "mesh1Size">,
		field<size_t, "mesh2Size">,
		field<size_t, "mesh3Size">,
		field<dynamic_carray<size_t, impl::NXSData, [](const impl::NXSData& parent) -> size_t { return parent.unk2Count; }>*, "unk2">,
		field<dynamic_carray<uint8_t, impl::NXSData, [](const impl::NXSData& parent) -> size_t { return parent.mesh1Size; }>*, "mesh1">,
		field<dynamic_carray<uint8_t, impl::NXSData, [](const impl::NXSData& parent) -> size_t { return parent.mesh2Size; }>*, "mesh2">,
		field<dynamic_carray<uint8_t, impl::NXSData, [](const impl::NXSData& parent) -> size_t { return parent.mesh3Size; }>*, "mesh3">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::nxs::v1::NXSData> { using type = ucsl::resources::nxs::v1::reflections::NXSData; };
}
