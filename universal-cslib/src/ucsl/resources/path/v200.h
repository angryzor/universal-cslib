#pragma once
#include <ucsl/resources/kd-tree/v1.h>

namespace ucsl::resources::path::v200 {
	enum class TagType {
		UINT,
		UNK1,
		STRING,
	};

	union TagValue {
		unsigned int uintValue;
		const char* stringValue;
	};

	struct TagData {
		const char* name;
		TagType type;
		TagValue value;
	};

	struct PathData {
		const char* name;
		bool unk1;
		unsigned short nodeCount;
		float length;
		bool* unk2;
		float* distances;
		ucsl::math::Position* positions;
		ucsl::math::Position* upVectors;
		ucsl::math::Position* forwardVectors;
		unsigned short extraVertexCount;
		ucsl::math::Position* extraVertices;
		ucsl::math::Position aabbMin;
		ucsl::math::Position aabbMax;
		unsigned short tagCount;
		TagData* tags;
		unsigned short unk3;
		kd_tree::v1::ResKdTreeData* kdTree;
	};

	struct PathsData {
		unsigned int magic;
		unsigned int version;
		unsigned int pathCount;
		PathData* paths;
	};
}
