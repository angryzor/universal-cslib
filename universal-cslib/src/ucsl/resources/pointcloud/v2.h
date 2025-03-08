#pragma once
#include <ucsl/bitset.h>
#include <ucsl/math.h>

namespace ucsl::resources::pointcloud::v2 {
	enum class RotationOrder : unsigned int {
		NONE,
		XYZ,
		YZX,
		ZXY,
		XZY,
		YXZ,
		ZYX,
	};

	struct InstanceData {
		const char* name;
		const char* resourceName;
		ucsl::math::Position position;
		ucsl::math::Position rotation;
		RotationOrder rotationOrder;
		ucsl::math::Position scale;
		unsigned int unk2;
	};

	struct PointcloudData {
		unsigned int magic;
		unsigned int version;
		InstanceData* instances;
		unsigned int instanceCount;
	};
}
