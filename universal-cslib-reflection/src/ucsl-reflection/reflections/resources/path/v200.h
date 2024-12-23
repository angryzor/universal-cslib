#include <ucsl/resources/path/v200.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>
#include <ucsl-reflection/reflections/resources/kd-tree/v1.h>

namespace ucsl::resources::path::v200::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::path::v200;

	using TagType = enumeration<impl::TagType, "TagType", unsigned int,
		option<"UINT">,
		option<"UNK1">,
		option<"STRING">
	>;

	using TagValue = unionof<impl::TagValue, "TagValue", impl::TagData, [](const impl::TagData& parent) -> size_t {
		switch (parent.type) {
		case impl::TagType::UINT: return 0;
		case impl::TagType::STRING: return 1;
		}
	},
		field<unsigned int, "uintValue">,
		field<const char*, "stringValue">
	>;

	using TagData = structure<impl::TagData, "TagData", void,
		field<const char*, "name">,
		field<TagType, "type">,
		field<TagValue, "value">
	>;

	using PathData = structure<impl::PathData, "PathData", void,
		field<const char*, "name">,
		field<bool, "unk1">,
		field<unsigned short, "nodeCount">,
		field<float, "length">,
		field<dynamic_carray<bool, impl::PathData, [](const impl::PathData& path) -> size_t { return path.nodeCount; }>*, "unk2">,
		field<dynamic_carray<float, impl::PathData, [](const impl::PathData& path) -> size_t { return path.nodeCount; }>*, "distances">,
		field<dynamic_carray<ucsl::math::Position, impl::PathData, [](const impl::PathData& path) -> size_t { return path.nodeCount; }>*, "positions">,
		field<dynamic_carray<ucsl::math::Position, impl::PathData, [](const impl::PathData& path) -> size_t { return path.nodeCount; }>*, "upVectors">,
		field<dynamic_carray<ucsl::math::Position, impl::PathData, [](const impl::PathData& path) -> size_t { return path.nodeCount; }>*, "forwardVectors">,
		field<unsigned short, "extraVertexCount">,
		field<dynamic_carray<ucsl::math::Position, impl::PathData, [](const impl::PathData& path) -> size_t { return path.extraVertexCount; }>*, "extraVertices">,
		field<ucsl::math::Position, "aabbMin">,
		field<ucsl::math::Position, "aabbMax">,
		field<unsigned short, "tagCount">,
		field<dynamic_carray<TagData, impl::PathData, [](const impl::PathData& path) -> size_t { return path.tagCount; }>*, "tags">,
		field<unsigned short, "unk3">,
		field<kd_tree::v1::reflections::ResKdTreeData*, "kdTree">
	>;

	using PathsData = structure<impl::PathsData, "PathsData", void,
		field<unsigned int, "magic">,
		field<unsigned int, "version">,
		field<unsigned int, "pathCount">,
		field<dynamic_carray<PathData, impl::PathsData, [](const impl::PathsData& paths) -> size_t { return paths.pathCount; }>*, "paths">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::path::v200::TagType> { using type = ucsl::resources::path::v200::reflections::TagType; };
	template<> struct canonical<ucsl::resources::path::v200::TagValue> { using type = ucsl::resources::path::v200::reflections::TagValue; };
	template<> struct canonical<ucsl::resources::path::v200::TagData> { using type = ucsl::resources::path::v200::reflections::TagData; };
	template<> struct canonical<ucsl::resources::path::v200::PathData> { using type = ucsl::resources::path::v200::reflections::PathData; };
	template<> struct canonical<ucsl::resources::path::v200::PathsData> { using type = ucsl::resources::path::v200::reflections::PathsData; };
}
