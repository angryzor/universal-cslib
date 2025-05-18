#pragma once

namespace ucsl::resources::path::v1 {
	struct PathIdData {
		const char* name;
		unsigned int id;
	};

	struct NodeData {
		float distance;
		unsigned int unk2;
		ucsl::math::Position position;
		ucsl::math::Position normal;
		ucsl::math::Position tangent;
		float unk3;
	};

	struct PathData {
		const char* name;
		unsigned int id;
		unsigned int unk2;
		float unk3;
		unsigned int nodeCount;
		NodeData* nodes;
		unsigned int extraVertexCount;
		ucsl::math::Position* extraVertices;
	};

	struct PathsData {
		unsigned int magic;
		unsigned int version;
		unsigned int pathIdCount;
		PathIdData* pathIds;
		unsigned int pathCount;
		PathData* paths;
		PathData* stageGuidePath;
		unsigned int objPathSize;
		PathData** objPaths;
		unsigned int qsPathSize;
		PathData** qsPaths;
		unsigned int svPathSize;
		PathData** svPaths;
		unsigned int grPathSize;
		PathData** grPaths;
		unsigned int unkPathSize;
		PathData** unkPaths;
	};
}
