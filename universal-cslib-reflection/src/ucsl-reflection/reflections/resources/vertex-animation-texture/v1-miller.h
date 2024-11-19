#pragma once
#include <ucsl/resources/vertex-animation-texture/v1-miller.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::vertex_animation_texture::v1_miller::reflections {
    using namespace simplerfl;
    using namespace ucsl::reflection;

    namespace impl = ::ucsl::resources::vertex_animation_texture::v1_miller;

    using VertexAnimationTextureRangeData = structure<impl::VertexAnimationTextureRangeData, "VertexAnimationTextureRangeData", void,
        field<float, "min">,
        field<float, "max">
    >;

    using VertexAnimationTextureData = structure<impl::VertexAnimationTextureData, "VertexAnimationTextureData", void,
        field<constant<unsigned int, 1>, "version">,
        field<const char*, "textureName">,
        field<const char*, "normalTextureName">,
        field<unsigned int, "type">,
        field<VertexAnimationTextureRangeData, "positionRange">,
        field<VertexAnimationTextureRangeData, "rotationRange">,
        field<VertexAnimationTextureRangeData, "scaleRange">,
        field<unsigned int, "fps">,
        field<unsigned int, "frameCount">
    >;
}

namespace simplerfl {
    template<> struct canonical<ucsl::resources::vertex_animation_texture::v1_miller::VertexAnimationTextureRangeData> { using type = ucsl::resources::vertex_animation_texture::v1_miller::reflections::VertexAnimationTextureRangeData; };
    template<> struct canonical<ucsl::resources::vertex_animation_texture::v1_miller::VertexAnimationTextureData> { using type = ucsl::resources::vertex_animation_texture::v1_miller::reflections::VertexAnimationTextureData; };
}
