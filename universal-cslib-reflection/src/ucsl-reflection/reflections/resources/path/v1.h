#pragma once
#include <ucsl/resources/path/v1.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::path::v1::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::path::v1;

	using PathIdData = structure<impl::PathIdData, "PathIdData", void,
		field<const char*, "name">,
		field<unsigned int, "id">
	>;

	using NodeData = structure<impl::NodeData, "NodeData", void,
		field<float, "distance">,
		field<unsigned int, "unk2">,
		field<ucsl::math::Position, "position">,
		field<ucsl::math::Position, "up">,
		field<ucsl::math::Position, "forward">,
		field<float, "unk3">
	>;

	using PathData = structure<impl::PathData, "PathData", void,
		field<const char*, "name">,
		field<unsigned int, "unk1">,
		field<unsigned int, "unk2">,
		field<float, "unk3">,
		field<unsigned int, "nodeCount">,
		field<dynamic_carray<NodeData, impl::PathData, [](const impl::PathData& path) -> size_t { return path.nodeCount; }>*, "nodes">,
		field<unsigned int, "extraVertexCount">,
		field<dynamic_carray<ucsl::math::Position, impl::PathData, [](const impl::PathData& path) -> size_t { return path.extraVertexCount; }>*, "extraVertices">
	>;

	using PathsData = structure<impl::PathsData, "PathsData", void,
		field<unsigned int, "magic">,
		field<unsigned int, "version">,
		field<unsigned int, "pathIdCount">,
		field<dynamic_carray<PathIdData, impl::PathsData, [](const impl::PathsData& parent) -> size_t { return parent.pathIdCount; }>*, "pathIds">,
		field<unsigned int, "pathCount">,
		field<dynamic_carray<PathData, impl::PathsData, [](const impl::PathsData& parent) -> size_t { return parent.pathCount; }>*, "paths">,
		field<void*, "paths2">,
		field<unsigned int, "pathSet1Size">,
		field<dynamic_carray<void*, impl::PathsData, [](const impl::PathsData& parent) -> size_t { return parent.pathSet1Size; }>*, "pathSet1">,
		field<unsigned int, "pathSet2Size">,
		field<dynamic_carray<void*, impl::PathsData, [](const impl::PathsData& parent) -> size_t { return parent.pathSet2Size; }>*, "pathSet2">,
		field<unsigned int, "pathSet3Size">,
		field<dynamic_carray<void*, impl::PathsData, [](const impl::PathsData& parent) -> size_t { return parent.pathSet3Size; }>*, "pathSet3">,
		field<unsigned int, "pathSet4Size">,
		field<dynamic_carray<void*, impl::PathsData, [](const impl::PathsData& parent) -> size_t { return parent.pathSet4Size; }>*, "pathSet4">,
		field<unsigned int, "pathSet5Size">,
		field<dynamic_carray<void*, impl::PathsData, [](const impl::PathsData& parent) -> size_t { return parent.pathSet5Size; }>*, "pathSet5">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::path::v1::PathIdData> { using type = ucsl::resources::path::v1::reflections::PathIdData; };
	template<> struct canonical<ucsl::resources::path::v1::NodeData> { using type = ucsl::resources::path::v1::reflections::NodeData; };
	template<> struct canonical<ucsl::resources::path::v1::PathData> { using type = ucsl::resources::path::v1::reflections::PathData; };
	template<> struct canonical<ucsl::resources::path::v1::PathsData> { using type = ucsl::resources::path::v1::reflections::PathsData; };
}