#pragma once
#include <ucsl/resources/fxcol/v1.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::fxcol::v1::reflections {
    using namespace simplerfl;
    using namespace ucsl::reflection;

    namespace impl = ucsl::resources::fxcol::v1;

    using Shape = enumeration<impl::FxColCollisionShapeData::Shape, "Shape", unsigned char,
        option<"SPHERE">,
        option<"CYLINDER">,
        option<"ANISOTROPIC_OBB">,
        option<"ISOTROPIC_OBB">
    >;

    using Type = enumeration<impl::FxColCollisionShapeData::Type, "Type", unsigned char,
        option<"SCENE_PARAMETER_INDEX">,
        option<"LIGHT_PARAMETER_INDEX">,
        option<"CAMERA">,
        option<"HEAT_HAZE">
    >;

    using SphereExtents = structure<impl::FxColCollisionShapeData::SphereExtents, "SphereExtents", void,
        field<float, "radius">,
        field<float, "borderThickness">
    >;

    using CylinderExtents = structure<impl::FxColCollisionShapeData::CylinderExtents, "CylinderExtents", void,
        field<float, "radius">,
        field<float, "halfHeight">,
        field<float, "borderThickness">
    >;

    using AnisotropicObbExtents = structure<impl::FxColCollisionShapeData::AnisotropicObbExtents, "AnisotropicObbExtents", void,
        field<float, "depth">,
        field<float, "width">,
        field<float, "height">,
        field<float, "maybeWidthAndHeightBorderThickness">,
        field<float, "positiveDepthBorderThickness">,
        field<float, "negativeDepthBorderThickness">
    >;

    using IsotropicObbExtents = structure<impl::FxColCollisionShapeData::IsotropicObbExtents, "IsotropicObbExtents", void,
        field<float, "depth">,
        field<float, "width">,
        field<float, "height">,
        field<float, "borderThickness">
    >;

    using Extents = unionof<impl::FxColCollisionShapeData::Extents, "Extents", impl::FxColCollisionShapeData, [](const impl::FxColCollisionShapeData& parent) -> size_t {
        switch (parent.shape) {
        case impl::FxColCollisionShapeData::Shape::SPHERE: return 0;
        case impl::FxColCollisionShapeData::Shape::CYLINDER: return 1;
        case impl::FxColCollisionShapeData::Shape::ANISOTROPIC_OBB: return 2;
        case impl::FxColCollisionShapeData::Shape::ISOTROPIC_OBB: return 3;
        }
    }, 
        field<SphereExtents, "sphere">,
        field<CylinderExtents, "cylinder">,
        field<AnisotropicObbExtents, "anisotropicObb">,
        field<IsotropicObbExtents, "isotropicObb">
    >;

    using SceneParameterIndexParameters = structure<impl::FxColCollisionShapeData::SceneParameterIndexParameters, "SceneParameterIndexParameters", void,
        field<unsigned int, "sceneParameterIndex">,
        field<float, "interpolationTime">
    >;

    using LightParameterIndexParameters = structure<impl::FxColCollisionShapeData::LightParameterIndexParameters, "LightParameterIndexParameters", void,
        field<unsigned int, "lightParameterIndex">
    >;

    using HeatHazeParameters = structure<impl::FxColCollisionShapeData::HeatHazeParameters, "HeatHazeParameters", void,
        field<float, "heatHazeMaxHeight">,
        field<float, "interpolationTime">
    >;

    using CameraParameters = structure<impl::FxColCollisionShapeData::CameraParameters, "CameraParameters", void,
        field<unsigned int, "unk1">,
        field<unsigned int, "unk2">,
        field<float, "bloomScale">,
        field<float, "autoExposureMiddleGray">,
        field<unsigned int, "unk3">,
        field<float, "luminance">,
        field<float, "autoExposureAdaptedRatio">,
        field<float, "interpolationTime">
    >;

    using Parameters = unionof<impl::FxColCollisionShapeData::Parameters, "Parameters", impl::FxColCollisionShapeData, [](const impl::FxColCollisionShapeData& parent) -> size_t {
        switch (parent.type) {
        case impl::FxColCollisionShapeData::Type::SCENE_PARAMETER_INDEX: return 0;
        case impl::FxColCollisionShapeData::Type::LIGHT_PARAMETER_INDEX: return 1;
        case impl::FxColCollisionShapeData::Type::HEAT_HAZE: return 2;
        case impl::FxColCollisionShapeData::Type::CAMERA: return 3;
        }
    },
        field<SceneParameterIndexParameters, "sceneParameterIndex">,
        field<LightParameterIndexParameters, "lightParameterIndex">,
        field<HeatHazeParameters, "heatHaze">,
        field<CameraParameters, "camera">
    >;

    using FxColCollisionShapeData = structure<impl::FxColCollisionShapeData, "FxColCollisionShapeData", void,
        field<const char*, "name">,
        field<Shape, "shape">,
        field<Type, "type">,
        field<unsigned char, "unk1">,
        field<unsigned char, "priority">,
        field<Extents, "extents">,
        field<Parameters, "parameters">,
        field<const char*, "unk2">,
        field<math::Position, "position">,
        field<math::Rotation, "rotation">
    >;

    using FxColBoundingVolumeData = structure<impl::FxColBoundingVolumeData, "FxColBoundingVolumeData", void,
        field<unsigned int, "shapeCount">,
        field<int, "shapeStartIdx">,
        field<math::Position, "aabbMin">,
        field<math::Position, "aabbMax">
    >;

    using FxColUnk1Data = structure<impl::FxColUnk1Data, "FxColUnk1Data", void,
        field<int, "unk1">,
        field<int, "unk2">
    >;

    using FxColData = structure<impl::FxColData, "FxColData", void,
        field<constant<unsigned int, 0x4658434F>, "magic">,
        field<constant<unsigned int, 1>, "version">,
        field<unsigned int, "collisionShapeCount">,
        field<dynamic_carray<FxColCollisionShapeData, impl::FxColData, [](const impl::FxColData& parent) -> size_t { return parent.collisionShapeCount; }>*, "collisionShapes">,
        field<unsigned int, "boundingVolumeCount">,
        field<dynamic_carray<FxColBoundingVolumeData, impl::FxColData, [](const impl::FxColData& parent) -> size_t { return parent.boundingVolumeCount; }>*, "boundingVolumes">,
        field<unsigned int, "unk1Count">,
        field<dynamic_carray<FxColUnk1Data, impl::FxColData, [](const impl::FxColData& parent) -> size_t { return parent.unk1Count; }>*, "unk1s">
    >;
}

namespace simplerfl {
    template<> struct canonical<ucsl::resources::fxcol::v1::FxColCollisionShapeData::Shape> { using type = ucsl::resources::fxcol::v1::reflections::Shape; };
    template<> struct canonical<ucsl::resources::fxcol::v1::FxColCollisionShapeData::Type> { using type = ucsl::resources::fxcol::v1::reflections::Type; };
    template<> struct canonical<ucsl::resources::fxcol::v1::FxColCollisionShapeData::SphereExtents> { using type = ucsl::resources::fxcol::v1::reflections::SphereExtents; };
    template<> struct canonical<ucsl::resources::fxcol::v1::FxColCollisionShapeData::CylinderExtents> { using type = ucsl::resources::fxcol::v1::reflections::CylinderExtents; };
    template<> struct canonical<ucsl::resources::fxcol::v1::FxColCollisionShapeData::AnisotropicObbExtents> { using type = ucsl::resources::fxcol::v1::reflections::AnisotropicObbExtents; };
    template<> struct canonical<ucsl::resources::fxcol::v1::FxColCollisionShapeData::IsotropicObbExtents> { using type = ucsl::resources::fxcol::v1::reflections::IsotropicObbExtents; };
    template<> struct canonical<ucsl::resources::fxcol::v1::FxColCollisionShapeData::Extents> { using type = ucsl::resources::fxcol::v1::reflections::Extents; };
    template<> struct canonical<ucsl::resources::fxcol::v1::FxColCollisionShapeData::SceneParameterIndexParameters> { using type = ucsl::resources::fxcol::v1::reflections::SceneParameterIndexParameters; };
    template<> struct canonical<ucsl::resources::fxcol::v1::FxColCollisionShapeData::LightParameterIndexParameters> { using type = ucsl::resources::fxcol::v1::reflections::LightParameterIndexParameters; };
    template<> struct canonical<ucsl::resources::fxcol::v1::FxColCollisionShapeData::HeatHazeParameters> { using type = ucsl::resources::fxcol::v1::reflections::HeatHazeParameters; };
    template<> struct canonical<ucsl::resources::fxcol::v1::FxColCollisionShapeData::CameraParameters> { using type = ucsl::resources::fxcol::v1::reflections::CameraParameters; };
    template<> struct canonical<ucsl::resources::fxcol::v1::FxColCollisionShapeData::Parameters> { using type = ucsl::resources::fxcol::v1::reflections::Parameters; };
    template<> struct canonical<ucsl::resources::fxcol::v1::FxColCollisionShapeData> { using type = ucsl::resources::fxcol::v1::reflections::FxColCollisionShapeData; };
    template<> struct canonical<ucsl::resources::fxcol::v1::FxColBoundingVolumeData> { using type = ucsl::resources::fxcol::v1::reflections::FxColBoundingVolumeData; };
    template<> struct canonical<ucsl::resources::fxcol::v1::FxColUnk1Data> { using type = ucsl::resources::fxcol::v1::reflections::FxColUnk1Data; };
    template<> struct canonical<ucsl::resources::fxcol::v1::FxColData> { using type = ucsl::resources::fxcol::v1::reflections::FxColData; };
}
