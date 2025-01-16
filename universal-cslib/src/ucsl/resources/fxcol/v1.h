#pragma once
#include <ucsl/math.h>

namespace ucsl::resources::fxcol::v1 {
    struct ShapeData {
        enum class Shape : unsigned char {
            SPHERE,
            CYLINDER,
            ANISOTROPIC_OBB,
            ISOTROPIC_OBB,
        };

        enum class Type : unsigned char {
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

    enum class KdTreeNodeType {
        SPLIT_X,
        SPLIT_Y,
        SPLIT_Z,
        LEAF,
    };

    // This is very similar to the node list in ResKdTree, with the single difference that an invalid node
    // has the deadZoneStartCoord set to -1 instead of 0.
    struct KdTreeNodeData {
        int deadZoneStartCoordOrLeafIndexAndNodeType; // Type is in lower 2 bits for intermediate nodes, and deadzone start is in the other bits. For leaf nodes the other bits contain the leaf index (the index is shifted left 2 bits).
        float deadZoneEndCoord;
    };

    struct KdTreeLeafData {
        unsigned int shapeCount;
        int shapeOffset;
        math::Position aabbMin;
        math::Position aabbMax;
    };

    // The K/D tree nodes are not used by the engine, it builds its own ResKdTree from the leaf AABBs, so it can be omitted.
    // NOTE: IT DOES USE THE LEAVES, THESE _MUST_ BE PRESENT.
    struct FxColData {
        unsigned int magic;
        unsigned int version;
        unsigned int shapeCount;
        ShapeData* shapes;
        unsigned int kdTreeLeafCount;
        KdTreeLeafData* kdTreeLeaves;
        unsigned int kdTreeNodeCount;
        KdTreeNodeData* kdTreeNodes;
    };
}
