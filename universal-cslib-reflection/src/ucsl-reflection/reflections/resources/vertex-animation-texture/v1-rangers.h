#pragma once
#include <ucsl/resources/vertex-animation-texture/v1-rangers.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::vertex_animation_texture::v1_rangers::reflections {
    using namespace simplerfl;
    using namespace ucsl::reflection;

    namespace impl = ::ucsl::resources::vertex_animation_texture::v1_rangers;

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
        field<unsigned int, "fps">,
        field<unsigned int, "frameCount">
    >;
}

namespace simplerfl {
    template<> struct canonical<ucsl::resources::vertex_animation_texture::v1_rangers::VertexAnimationTextureRangeData> { using type = ucsl::resources::vertex_animation_texture::v1_rangers::reflections::VertexAnimationTextureRangeData; };
    template<> struct canonical<ucsl::resources::vertex_animation_texture::v1_rangers::VertexAnimationTextureData> { using type = ucsl::resources::vertex_animation_texture::v1_rangers::reflections::VertexAnimationTextureData; };
}
