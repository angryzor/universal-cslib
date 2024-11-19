#pragma once
#include <ucsl/containers/arrays/array.h>
#include <ucsl/math.h>

namespace ucsl::resources::vertex_animation_texture::v1_rangers {
    struct VertexAnimationTextureRangeData {
        float min;
        float max;
    };

    struct VertexAnimationTextureData {
        unsigned int version;
        const char* textureName;
        const char* normalTextureName;
        unsigned int type;
        VertexAnimationTextureRangeData positionRange;
        unsigned int fps;
        unsigned int frameCount;
    };
}
