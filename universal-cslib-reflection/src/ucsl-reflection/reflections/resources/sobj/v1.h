#pragma once
#include <ucsl/math.h>
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

	using ObjectInstanceData = structure<impl::ObjectInstanceData, "ObjectInstanceData", void,
		field<ObjectTransformData, "transform">,
		field<ObjectTransformData, "localTransform">
	>;

	using ObjectData = structure<impl::ObjectData, "ObjectData", void,
		field<unsigned int, "id">,
		field<unsigned int, "unk2">,
		field<unsigned int, "unk3">,
		field<float, "unk4">,
		field<float, "m_distance">,
		field<float, "m_range">,
		field<dynamic_carray<ObjectInstanceData, impl::ObjectData, [](const impl::ObjectData& parent) -> size_t { return parent.instanceCount; }>*, "instances">,
		field<unsigned int, "instanceCount">,
		field<unsigned int, "unk5">,
		field<spawner_data_rflclass_with_root<impl::ObjectData, impl::SetObjectData, [](const impl::ObjectData& parent, const impl::SetObjectData& root) -> const char* {
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

	using SetObjectData = structure<impl::SetObjectData, "SetObjectData", void,
		field<unsigned int, "magic">,
		field<unsigned int, "version">,
		field<unsigned int, "objectTypeCount">,
		field<dynamic_carray<ObjectTypeData, impl::SetObjectData, [](const impl::SetObjectData& parent) -> size_t { return parent.objectTypeCount; }>*, "objectTypes">,
		field<int, "unk1">,
		field<dynamic_carray<ObjectData*, impl::SetObjectData, [](const impl::SetObjectData& parent) -> size_t { return parent.objectCount; }>*, "objects">,
		field<unsigned int, "objectCount">,
		field<unsigned int, "unk2">,
		field<unsigned int, "unk3">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::sobj::v1::ObjectTransformData> { using type = ucsl::resources::sobj::v1::reflections::ObjectTransformData; };
	template<> struct canonical<ucsl::resources::sobj::v1::ObjectInstanceData> { using type = ucsl::resources::sobj::v1::reflections::ObjectInstanceData; };
	template<> struct canonical<ucsl::resources::sobj::v1::ObjectData> { using type = ucsl::resources::sobj::v1::reflections::ObjectData; };
	template<> struct canonical<ucsl::resources::sobj::v1::ObjectTypeData> { using type = ucsl::resources::sobj::v1::reflections::ObjectTypeData; };
	template<> struct canonical<ucsl::resources::sobj::v1::SetObjectData> { using type = ucsl::resources::sobj::v1::reflections::SetObjectData; };
}
