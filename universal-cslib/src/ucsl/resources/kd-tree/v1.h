#pragma once

namespace ucsl::resources::kd_tree::v1 {
	enum class ResKdTreeNodeType {
		SPLIT_X,
		SPLIT_Y,
		SPLIT_Z,
		LEAF,
	};

	struct ResKdTreeNodeData {
		float deadZoneStartCoordOrLeafIndexAndNodeType; // Type is in lower 2 bits for intermediate nodes, and deadzone start is in the other bits. For leaf nodes the other bits contain the leaf index.
		float deadZoneEndCoord;
	};

	struct ResKdTreeLeafNodeData {
		int numObjects;
		int startObjectIndex;
	};

	struct ResKdTreeData {
		int depth;
		int nodeCount;
		ResKdTreeNodeData* nodes;
		int leafCount;
		ResKdTreeLeafNodeData* leaves;
		int objectCount;
		unsigned int* objects;
	};
}
