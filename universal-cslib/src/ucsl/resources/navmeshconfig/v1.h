#pragma once

namespace ucsl::resources::navmeshconfig::v1 {
    struct Config {
        csl::math::Position navMeshSize;
        csl::math::Vector2 size;
        unsigned int tileCount;
        unsigned int unk1;
    };

    struct NavMeshConfigData {
        unsigned int magic;
        unsigned int version;
        Config config;
    };
}
