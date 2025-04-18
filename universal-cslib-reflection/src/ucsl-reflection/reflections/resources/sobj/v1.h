#pragma once
#include <ucsl/resources/sobj/v1.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::sobj::v1::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::sobj::v1;

	using ObjectTransformData = structure<impl::ObjectTransformData, "ObjectTransformData", void,
		field<math::Position, "position">,
		field<math::Position, "rotation">
	>;

	template<typename AllocatorSystem>
	using ObjectData = structure<impl::ObjectData<AllocatorSystem>, "ObjectData", void,
		field<ucsl::objectids::ObjectIdV1, "id">,
		field<unsigned int, "objectClassId">,
		field<unsigned int, "bvhNode">,
		field<float, "replicationInterval">,
		field<float, "m_distance">,
		field<float, "m_range">,
		field<containers::arrays::TArray<ObjectTransformData, AllocatorSystem>, "instances">,
		field<spawner_data_rflclass_with_root<impl::ObjectData<AllocatorSystem>, impl::SetObjectData<AllocatorSystem>, [](const impl::ObjectData<AllocatorSystem>& parent, const impl::SetObjectData<AllocatorSystem>& root) -> const char* {
			for (size_t i = 0; i < root.objectTypeCount; i++) {
				auto& type = root.objectTypes[i];
				for (size_t j = 0; j < type.objectIndexCount; j++) {
					auto objectIdx = type.objectIndices[j];
					if (root.objects[objectIdx] == &parent)
						return type.name;
				}
			}
			return nullptr;
		}>, "spawnerData">
	>;

	using ObjectTypeData = structure<impl::ObjectTypeData, "ObjectTypeData", void,
		field<const char*, "name">,
		field<unsigned int, "objectIndexCount">,
		field<dynamic_carray<unsigned short, impl::ObjectTypeData, [](const impl::ObjectTypeData& parent) -> size_t { return parent.objectIndexCount; }>*, "objectIndices">
	>;

	template<typename AllocatorSystem>
	using SetObjectData = structure<impl::SetObjectData<AllocatorSystem>, "SetObjectData", void,
		field<constant<unsigned int, 0x534F424A>, "magic">,
		field<constant<unsigned int, 1>, "version">,
		field<unsigned int, "objectTypeCount">,
		field<dynamic_carray<ObjectTypeData, impl::SetObjectData<AllocatorSystem>, [](const impl::SetObjectData<AllocatorSystem>& parent) -> size_t { return parent.objectTypeCount; }>*, "objectTypes">,
		field<size_t, "bvh">,
		field<dynamic_carray<ObjectData<AllocatorSystem>*, impl::SetObjectData<AllocatorSystem>, [](const impl::SetObjectData<AllocatorSystem>& parent) -> size_t { return parent.objectCount; }>*, "objects">,
		field<unsigned int, "objectCount">,
		field<unsigned int, "bvhNodeCount">,
		field<unsigned int, "objectInstanceCount">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::sobj::v1::ObjectTransformData> { using type = ucsl::resources::sobj::v1::reflections::ObjectTransformData; };
	template<typename AllocatorSystem> struct canonical<ucsl::resources::sobj::v1::ObjectData<AllocatorSystem>> { using type = ucsl::resources::sobj::v1::reflections::ObjectData<AllocatorSystem>; };
	template<> struct canonical<ucsl::resources::sobj::v1::ObjectTypeData> { using type = ucsl::resources::sobj::v1::reflections::ObjectTypeData; };
	template<typename AllocatorSystem> struct canonical<ucsl::resources::sobj::v1::SetObjectData<AllocatorSystem>> { using type = ucsl::resources::sobj::v1::reflections::SetObjectData<AllocatorSystem>; };
}
