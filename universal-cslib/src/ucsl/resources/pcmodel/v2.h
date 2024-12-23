#pragma once
#include <ucsl/bitset.h>
#include <ucsl/math.h>

namespace ucsl::resources::pcmodel::v2 {
	struct ModelInstanceData {
		const char* name;
		const char* resourceName;
		ucsl::math::Position position;
		ucsl::math::Position rotation;
		unsigned int unk1;
		ucsl::math::Position scale;
		unsigned int unk2;
	};

	struct PointCloudModelData {
		unsigned int magic;
		unsigned int version;
		ModelInstanceData* instances;
		size_t instanceCount;
	};
}
