#pragma once
#include <ucsl/math.h>
#include <ucsl/containers/arrays/array.h>
#include <ucsl/resources/object-world/v3.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::object_world::v3::reflections {
    using namespace simplerfl;
    using namespace ucsl::reflection;

    namespace impl = ucsl::resources::object_world::v3;

    using ObjectTransformData = structure<impl::ObjectTransformData, "ObjectTransformData", void,
        field<math::Position, "position">,
        field<math::Position, "rotation">
    >;

    using ComponentData = structure<impl::ComponentData, "ComponentData", void,
        field<erased<unsigned long long>, "unk1">,
        field<const char*, "type">,
        field<unsigned long long, "size">,
        field<component_data_rflclass<impl::ComponentData, [](const impl::ComponentData& parent) -> const char* { return parent.type; }>*, "data">
    >;

    template<typename AllocatorSystem>
    using ObjectData = structure<impl::ObjectData<AllocatorSystem>, "ObjectData", void,
        field<erased<unsigned int>, "flags">,
        field<const char*, "gameObjectClass">,
        field<strings::VariableString, "name">,
        field<objectids::ObjectIdV2, "id">,
        field<objectids::ObjectIdV2, "parentID">,
        field<ObjectTransformData, "transform">,
        field<ObjectTransformData, "localTransform">,
        field<containers::arrays::Array<ComponentData*, AllocatorSystem>, "componentData">,
        field<spawner_data_rflclass<impl::ObjectData<AllocatorSystem>, [](const impl::ObjectData<AllocatorSystem>& parent) -> const char* { return parent.gameObjectClass; }>*, "spawnerData">
    >;

    template<typename AllocatorSystem>
    using ObjectWorldData = structure<impl::ObjectWorldData<AllocatorSystem>, "ObjectWorldData", void,
        field<erased<unsigned int>, "unk1">,
        field<erased<unsigned int>, "unk2">,
        field<erased<unsigned char>, "flags">,
        field<containers::arrays::Array<ObjectData<AllocatorSystem>*, AllocatorSystem>, "objects">
    >;
}

namespace simplerfl {
    template<> struct canonical<ucsl::resources::object_world::v3::ObjectTransformData> { using type = ucsl::resources::object_world::v3::reflections::ObjectTransformData; };
    template<> struct canonical<ucsl::resources::object_world::v3::ComponentData> { using type = ucsl::resources::object_world::v3::reflections::ComponentData; };
    template<typename AllocatorSystem> struct canonical<ucsl::resources::object_world::v3::ObjectData<AllocatorSystem>> { using type = ucsl::resources::object_world::v3::reflections::ObjectData<AllocatorSystem>; };
    template<typename AllocatorSystem> struct canonical<ucsl::resources::object_world::v3::ObjectWorldData<AllocatorSystem>> { using type = ucsl::resources::object_world::v3::reflections::ObjectWorldData<AllocatorSystem>; };
}
