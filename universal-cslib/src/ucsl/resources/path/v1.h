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
		ucsl::math::Position up;
		ucsl::math::Position forward;
		float unk3;
	};

	struct PathData {
		const char* name;
		unsigned int unk1;
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
		PathData* paths2;
		unsigned int pathSet1Size;
		PathData** pathSet1;
		unsigned int pathSet2Size;
		PathData** pathSet2;
		unsigned int pathSet3Size;
		PathData** pathSet3;
		unsigned int pathSet4Size;
		PathData** pathSet4;
		unsigned int pathSet5Size;
		PathData** pathSet5;
	};
}
