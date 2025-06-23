#pragma once

// Credit to AdelQue for helping with reversing

namespace ucsl::resources::pba::v1 {
    struct RigidBody {
        enum class Type : unsigned char {
            DYNAMIC,
            STATIC,
        };

        enum class Shape : unsigned char {
            SPHERE, // capsule if it has height
            BOX,
        };

        const char* boneName;
        Type type;
        Shape shape;
        uint8_t unk1;
        char group;
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
        bool disableCollisionsBetweenLinkedBodies;
        char unk1;
        unsigned short overrideSolverIterationCount;
        short parentRigidBodyIndex;
        short childRigidBodyIndex;
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
        short rigidBodyIndex;
        bool isPinned;
        short positiveXSiblingIndex;
        short negativeXSiblingIndex;
        short positiveYSiblingIndex;
        short negativeYSiblingIndex;
        short positiveZSiblingIndex;
        short negativeZSiblingIndex;
    };

    struct Link {
        short verts[2];
        float restLength;
        float stiffness;
    };

    struct SoftBody {
        const char* name;
        float margin;
        float dampingCoeff;
        float dragCoeff;
        float liftCoeff;
        float dynamicFrictionCoeff;
        float poseMatchingCoeff;
        float rigidContactsHardness;
        float kineticContactsHardness;
        float softContactsHardness;
        float anchorsHardness;
        unsigned char positionSolverIterationCount;
        char unk0;
        char group;
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
