#pragma once
#include <ucsl/containers/arrays/tarray.h>
#include <ucsl/math.h>
#include <ucsl/object-id.h>

namespace ucsl::resources::sobj::v1 {
	struct ObjectTransformData {
		ucsl::math::Position position;
		ucsl::math::Position rotation;
	};

	template<typename AllocatorSystem>
	struct ObjectData {
		ucsl::objectids::ObjectIdV1 id;
		unsigned int objectClassId;
		unsigned int bvhNode;
		float replicationInterval;
		float m_distance;
		float m_range;
		containers::arrays::TArray<ObjectTransformData, AllocatorSystem> instances;
		// parameter data follows, aligned at 16
	};

	struct ObjectTypeData {
		const char* name;
		unsigned int objectIndexCount;
		unsigned short* objectIndices;
	};

	template<typename AllocatorSystem>
	struct SetObjectData {
		unsigned int magic;
		unsigned int version;
		unsigned int objectTypeCount;
		ObjectTypeData* objectTypes;
		int bvh;
		ObjectData<AllocatorSystem>** objects;
		unsigned int objectCount;
		unsigned int bvhNodeCount;
		unsigned int objectInstanceCount;
	};
}
