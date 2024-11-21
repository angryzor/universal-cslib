#pragma once
#include <ucsl/containers/arrays/array.h>
#include <ucsl/math.h>

namespace ucsl::resources::fxcol::v1 {
    struct FxColCollisionShapeData {
        enum class Shape : uint8_t {
            SPHERE,
            CYLINDER,
            ANISOTROPIC_OBB,
            ISOTROPIC_OBB,
        };

        enum class Type : uint8_t {
            SCENE_PARAMETER_INDEX,
            LIGHT_PARAMETER_INDEX,
            CAMERA,
            HEAT_HAZE,
        };

        struct SphereExtents {
            float radius;
            float borderThickness;
        };

        struct CylinderExtents {
            float radius;
            float halfHeight;
            float borderThickness;
        };

        struct AnisotropicObbExtents {
            float depth;  // Z
            float width;  // X
            float height; // Y
            float maybeWidthAndHeightBorderThickness;
            float positiveDepthBorderThickness;
            float negativeDepthBorderThickness;
        };

        struct IsotropicObbExtents {
            float depth;  // Z
            float width;  // X
            float height; // Y
            float borderThickness;
        };

        union Extents {
            SphereExtents sphere;
            CylinderExtents cylinder;
            AnisotropicObbExtents anisotropicObb;
            IsotropicObbExtents isotropicObb;
        };

        struct SceneParameterIndexParameters {
            unsigned int sceneParameterIndex;
            float interpolationTime;
        };

        struct LightParameterIndexParameters {
            unsigned int lightParameterIndex;
        };

        struct HeatHazeParameters {
            float heatHazeMaxHeight;
            float interpolationTime;
        };

        struct CameraParameters {
            uint32_t unk1;
            uint32_t unk2;
            float bloomScale;
            float autoExposureMiddleGray;
            uint32_t unk3;
            float luminance;
            float autoExposureAdaptedRatio;
            float interpolationTime;
        };

        union Parameters {
            SceneParameterIndexParameters sceneParameterIndex;
            LightParameterIndexParameters lightParameterIndex;
            HeatHazeParameters heatHaze;
            CameraParameters camera;
        };

        const char* name;
        Shape shape;
        Type type;
        uint8_t unk1;
        uint8_t priority;
        Extents extents;
        Parameters parameters;
        const char* unk2;
        math::Position position;
        math::Rotation rotation;
    };

    struct FxColBoundingVolumeData {
        unsigned int shapeCount;
        int shapeStartIdx;
        math::Position aabbMin;
        math::Position aabbMax;
    };

    struct FxColUnk1Data {
        int unk1;
        int unk2;
    };

    struct FxColData {
        unsigned int magic;
        unsigned int version;
        unsigned int collisionShapeCount;
        FxColCollisionShapeData* collisionShapes;
        unsigned int boundingVolumeCount;
        FxColBoundingVolumeData* boundingVolumes;
        unsigned int unk1Count;
        FxColUnk1Data* unk1s;
    };
}
