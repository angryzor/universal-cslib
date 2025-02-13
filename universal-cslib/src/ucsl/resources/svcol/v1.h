#pragma once
#include <ucsl/math.h>

namespace ucsl::resources::svcol::v1 {
	struct SectorFilterData {
		unsigned char sectorId; // value between 0 and 127
		bool enabled;
	};

	enum class Shape : unsigned char {
		SPHERE, // assumed from fxcol similarities, but ignored in the engine
		CYLINDER, // assumed from fxcol similarities, but ignored in the engine
		OBB,
	};

	struct ShapeData {
		const char* name;
		unsigned char priority;
		Shape type;
		float depth;
		float width;
		float height;
		math::Position position;
		math::Rotation rotation;
		math::Position aabbMin;
		math::Position aabbMax;
		unsigned char skySectorId; // value between 0 and 127
		unsigned int groundSectorFilterCount;
		SectorFilterData* groundSectorFilters;
	};

	// The ShapeData array is immediately followed by the KdTreeNodeData array and then the KdTreeLeafData array.
	// There is no pointer to these structures. The node or leaf count is also not saved anywhere.
	// The only way to know the correct count is to parse the KD tree.
	// The KD tree is not used in the engine, it builds its own ResKdTree from the shape AABBs, so it can be omitted.
	enum class KdTreeNodeType {
		SPLIT_X,
		SPLIT_Y,
		SPLIT_Z,
		LEAF,
	};

	// This is very similar to the node list in ResKdTree, with the single difference that an invalid node
	// has the deadZoneStartCoord set to -1 instead of 0.
	struct KdTreeNodeData {
		int deadZoneStartCoordOrLeafIndexAndNodeType; // Type is in lower 2 bits for intermediate nodes, and deadzone start is in the other bits. For leaf nodes the other bits contain the leaf index (the index is shifted left 2 bits).
		float deadZoneEndCoord;
	};

	struct KdTreeLeafData {
		unsigned int shapeCount;
		int shapeOffset;
		math::Position aabbMin;
		math::Position aabbMax;
	};

	struct SvColData {
		unsigned int magic;
		unsigned int version;
		unsigned int shapeCount;
		ShapeData* shapes;
	};
}
