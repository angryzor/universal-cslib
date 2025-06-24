#pragma once

// Credit to AdelQue for helping with reversing

namespace ucsl::resources::pba::v1 {
    struct RigidBody {
        const char* boneName;
        bool isStaticObject;
        bool isShapeBox;
        float shapeRadius;
        float shapeHeight;
        float shapeDepth;
        float mass;
        float friction;
        float restitution;
        float linearDamping;
        float angularDamping;
        ucsl::math::Vector3 offsetPosition;
        ucsl::math::Quaternion offsetRotation;
    };

    enum class LimitMode : unsigned char {
        NONE,
        ENABLED,
        DISABLED
    };

    struct Limit {
        LimitMode mode;
        bool springEnabled;
        float lowLimit;
        float highLimit;
        float springStiffness;
        float springDamping;
    };

    struct Constraint {
        const char* boneName;
        char unk0;
        char unk1;
        unsigned short iterationCount;
        short localParentBoneIndex;
        short localBoneIndex;
        short skeletonParentBoneIndex;
        Limit angularLimits[3];
        Limit linearLimits[3];
        ucsl::math::Vector3 offsetPositionA;
        ucsl::math::Quaternion offsetRotationA;
        ucsl::math::Vector3 offsetPositionB;
        ucsl::math::Quaternion offsetRotationB;
    };

    struct Node {
        const char* boneName;
        float mass;
        short unk0;
        bool isPinned;
        short childId;
        short parentId;
        short unk1;
        short unk2;
        short siblingLeftId;
        short siblingRightId;
    };

    struct Link {
        short verts[2];
        float restLength;
        float stiffness;
    };

    struct SoftBody {
        const char* name;
        float scale;
        float dampingCoeff;
        float dragCoeff;
        float liftCoeff;
        float dynamicFrictionCoeff;
        float poseMatchingCoeff;
        float rigidContactCoeff;
        float kineticContactsHardness;
        float softContactsHardness;
        float anchorsHardness;
        char positionIterationCount;
        char unk0;
        short unk1;
        unsigned int nodeCount;
        unsigned int linkCount;
        Node* nodes;
        Link* links;
    };

    struct PbaData {
        unsigned int magic;
        unsigned int version;
        const char* skeletonName;
        unsigned int rigidBodyCount;
        unsigned int constraintCount;
        RigidBody* rigidBodies;
        Constraint* constraints;
        unsigned int softBodyCount;
        SoftBody* softBodies;
    };
}
