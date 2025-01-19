#pragma once

namespace ucsl::resources::probe::v2 {
    struct ProbeData {
        math::Matrix34 matrix;
        math::Position extents;
        float unk1;
        const char* textureName;
        unsigned int type1;
        void* texture; // needle::Texture loaded at runtime
        unsigned int type2;
        unsigned int type3;
        unsigned int unkCount3;
        unsigned int unk4;
    };

    struct ResProbeData {
        unsigned int magic;
        unsigned int version;
        ProbeData* probes;
        unsigned int probeCount;
    };
}
