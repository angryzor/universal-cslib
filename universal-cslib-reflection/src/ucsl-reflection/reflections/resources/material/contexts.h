#pragma once
#include <ucsl/resources/map/v1.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::material::contexts::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::material::contexts;

    using TextureWrapMode = enumeration<impl::TextureWrapMode, "TextureWrapMode", unsigned char,
        option<"REPEAT">,
        option<"MIRROR">,
        option<"CLAMP">,
        option<"MIRROR_ONCE">,
        option<"BORDER">,
        option<"UNK5">,
        option<"UNK6">
    >;

    using BlendMode = enumeration<impl::BlendMode, "BlendMode", unsigned char,
        option<"MODE_2_4_0">,
        option<"MODE_2_1_0">,
        option<"MODE_0_7_0">,
        option<"MODE_2_1_2">
    >;

    using TextureData = structure<impl::TextureData, "TextureData", void,
        field<const char*, "name">,
        field<unsigned char, "texCoordIndex">,
        field<TextureWrapMode, "wrapModeU">,
        field<TextureWrapMode, "wrapModeV">,
        field<const char*, "type">
    >;

    template<typename T>
    using MaterialParamData = structure<impl::MaterialParamData<T>, "MaterialParamData", void,
        field<unsigned char, "flag1">,
        field<unsigned char, "flag2">,
        field<unsigned char, "valueCount">,
        field<unsigned char, "flag3">,
        field<const char*, "name">,
        field<dynamic_carray<T[4], impl::MaterialParamData<T>, [](const impl::MaterialParamData<T>& parent) -> size_t { return parent.valueCount; }>*, "values">
    >;

    using ContextsData = structure<impl::ContextsData, "ContextsData", void,
        field<const char*, "shaderName">,
        field<const char*, "subShaderName">,
        field<dynamic_carray<const char*, impl::ContextsData, [](const impl::ContextsData& parent) -> size_t { return parent.textureCount; }>*, "textureNames">,
        field<dynamic_carray<TextureData*, impl::ContextsData, [](const impl::ContextsData& parent) -> size_t { return parent.textureCount; }>*, "textures">,
        field<unsigned char, "alphaThreshold">,
        field<bool, "noBackfaceCulling">,
        field<BlendMode, "blendMode">,
        field<unsigned char, "unknownFlag1">,
        field<unsigned char, "floatParamCount">,
        field<unsigned char, "uintParamCount">,
        field<unsigned char, "boolParamCount">,
        field<unsigned char, "textureCount">,
        field<dynamic_carray<MaterialParamData<float>*, impl::ContextsData, [](const impl::ContextsData& parent) -> size_t { return parent.floatParamCount; }>*, "floatParams">,
        field<dynamic_carray<MaterialParamData<unsigned int>*, impl::ContextsData, [](const impl::ContextsData& parent) -> size_t { return parent.uintParamCount; }>*, "uintParams">,
        field<dynamic_carray<MaterialParamData<bool>*, impl::ContextsData, [](const impl::ContextsData& parent) -> size_t { return parent.boolParamCount; }>*, "boolParams">
    >;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::material::contexts::BlendMode> { using type = ucsl::resources::material::contexts::reflections::BlendMode; };
    template<> struct canonical<ucsl::resources::material::contexts::TextureData> { using type = ucsl::resources::material::contexts::reflections::TextureData; };
    template<typename T> struct canonical<ucsl::resources::material::contexts::MaterialParamData<T>> { using type = ucsl::resources::material::contexts::reflections::MaterialParamData<T>; };
	template<> struct canonical<ucsl::resources::material::contexts::ContextsData> { using type = ucsl::resources::material::contexts::reflections::ContextsData; };
}
