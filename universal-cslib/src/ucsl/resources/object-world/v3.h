#pragma once
#include <ucsl/containers/arrays/array.h>
#include <ucsl/strings/variable-string.h>
#include <ucsl/object-id.h>
#include <ucsl/bitset.h>
#include <ucsl/math.h>

namespace ucsl::resources::object_world::v3 {
    struct ObjectTransformData {
        ucsl::math::Position position;
        ucsl::math::Position rotation;
    };

    struct ComponentData {
        unsigned long long unk1;
        const char* type;
        unsigned long long size;
        void* data;
    };

    template<typename AllocatorSystem>
    struct ObjectData {
        enum class Flag : unsigned int {
            DEALLOCATE,
            DEALLOCATE_SPAWNER_DATA,
        };

        ucsl::bits::Bitset<Flag> flags;
        const char* gameObjectClass;
        ucsl::strings::VariableString name;
        ucsl::objectids::ObjectIdV2 id;
        ucsl::objectids::ObjectIdV2 parentID;
        ObjectTransformData transform;
        ObjectTransformData localTransform;
        ucsl::containers::arrays::Array<ComponentData*, AllocatorSystem> componentData;
        void* spawnerData;
    };

    template<typename AllocatorSystem>
    struct ObjectWorldData {
        unsigned int unk1;
        unsigned int unk2;
        unsigned char flags;
        ucsl::containers::arrays::Array<ObjectData<AllocatorSystem>*, AllocatorSystem> objects;
    };
}
