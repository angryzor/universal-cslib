#pragma once
#include <ucsl/math.h>

namespace ucsl::resources::sobj::v1 {
	struct ObjectTransformData {
		ucsl::math::Position position;
		ucsl::math::Position rotation;
	};

	struct ObjectInstanceData {
		ObjectTransformData transform;
		ObjectTransformData localTransform;
	};

	struct ObjectData {
		unsigned int id;
		unsigned int unk2;
		unsigned int unk3;
		float unk4;
		float m_distance;
		float m_range;
		ObjectInstanceData* instances;
		unsigned int instanceCount;
		// parameter data follows, aligned at 16
	};

	struct ObjectTypeData {
		const char* name;
		unsigned int objectIndexCount;
		unsigned short* objectIndices;
	};

	struct SetObjectData {
		unsigned int magic;
		unsigned int version;
		unsigned int objectTypeCount;
		ObjectTypeData* objectTypes;
		int unk1;
		ObjectData** objects;
		unsigned int objectCount;
		unsigned int unk2;
		unsigned int unk3;
	};
}
