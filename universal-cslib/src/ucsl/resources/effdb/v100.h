#pragma once
#include <ucsl/bitset.h>
#include <ucsl/math.h>

// reversed from files

namespace ucsl::resources::effdb::v100 {
    struct Particle {
        enum class Flag : unsigned char {
            IGNORE_RELATIVE_POSITION,
            USE_POSITION,
            USE_ROTATION,
            USE_SCALE,
        };

        bool attachedToBone;
        ucsl::bits::Bitset<Flag> flags;
        const char* particleName;
        const char* boneName;
        ucsl::math::Vector3 position;
        ucsl::math::Quaternion rotation;
        ucsl::math::Vector3 scale;
    };

    struct State {
        const char* stateName;
        unsigned int particleCount;
        Particle* particles;
        unsigned int soundNameCount;
        const char** soundNames;
    };

    struct EffdbData{
        unsigned int magic;
        unsigned int version;
        unsigned int stateCount;
        State* states;
    };
}