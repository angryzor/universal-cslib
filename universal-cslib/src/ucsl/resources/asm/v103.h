#pragma once
#include <ucsl/bitset.h>
#include <ucsl/math.h>

namespace ucsl::resources::animation_state_machine::v103 {
	struct ClipDataAnimationSettings{
		const char* resourceName;
		float start;
		float end;
		float speed;
		unsigned char flags;
		bool loops;
	};

	struct ClipData {
		const char* name;
		ClipDataAnimationSettings animationSettings;
		unsigned short triggerCount;
		short triggerOffset;
		short blendMaskIndex;
		unsigned short childClipIndexCount;
		short childClipIndexOffset;
	};

	enum class TransitionType : unsigned char {
		IMMEDIATE,
		FROZEN,
		SMOOTH,
		SYNCHRONIZE,
		HOLD_TO,
		HOLD_BOTH,
		WAIT_FROM,
		WAIT_FROM_HOLD_TO,
		USER_0,
		USER_1,
		USER_2,
	};

	enum class TransitionEasingType : unsigned char {
		LINEAR,
		CUBIC,
	};

	struct TransitionDataTransitionInfo {
		TransitionType type;
		TransitionEasingType easingType;
		short targetStateIndex;
		float transitionTime;
	};

	struct TransitionData {
		TransitionDataTransitionInfo transitionInfo;
		short transitionTimeVariableIndex;
	};

	enum class StateType : char {
		NULL_STATE = -1,
		CLIP,
		BLEND_TREE,
		NONE,
	};

	struct StateData {
		enum class Flag : unsigned char {
			LOOPS,
			UNK1,
			DISABLE_PBA,
			USE_PBA_BLEND_FACTOR,
		};

		const char* name;
		char type;
		bool transitImmediately;
		ucsl::bits::Bitset<Flag> flags;
		char defaultLayerIndex;
		short rootBlendNodeOrClipIndex;
		short maxCycles;
		float speed;
		short speedVariableIndex;
		unsigned short eventCount;
		short eventOffset;
		short transitionArrayIndex;
		TransitionData stateEndTransition;
		unsigned short flagIndexCount;
		short flagIndexOffset;
		float pbaBlendMaskOpacity;
	};

	enum class BlendNodeType : unsigned char {
		LERP,
		ADDITIVE,
		CLIP,
		OVERRIDE,
		LAYER,
		MULTIPLY,
		BLEND_SPACE,
		TWO_POINT_LERP,
	};

	struct BlendNodeData {
		BlendNodeType type;
		short blendSpaceIndex;
		short blendFactorVariableIndex;
		float blendFactorTarget;
		unsigned short childNodeArraySize;
		short childNodeArrayOffset;
	};

	struct TransitionArrayData {
		int size;
		int offset;
	};

	struct EventData {
		const char* name;
		TransitionData transition;
	};

	struct LayerData {
		const char* name;
		unsigned short layerId;
		short blendMaskIndex;
	};

	struct BlendMaskData {
		const char* name;
		unsigned short maskBoneCount;
		short maskBoneOffset;
	};

	enum class TriggerType : uint8_t {
		HIT,
		ENTER_LEAVE,
	};

	struct TriggerData {
		TriggerType type;
		float unknown2;
		float unknown3;
		unsigned short triggerTypeIndex;
		short colliderIndex;
		const char* name;
	};

	struct BlendSpaceDataTriangle {
		short nodeIndex0;
		short nodeIndex1;
		short nodeIndex2;
		short unused;
	};

	struct BlendSpaceData {
		short xVariableIndex;
		short yVariableIndex;
		float xMin;
		float xMax;
		float yMin;
		float yMax;
		unsigned short nodeCount;
		unsigned short triangleCount;
		ucsl::math::Vector2* nodes;
		short* clipIndices;
		BlendSpaceDataTriangle* triangles;
	};

	struct AsmData {
		unsigned int magic;
		int version;
		int clipCount;
		ClipData* clips;
		int stateCount;
		StateData* states;
		int blendNodeCount;
		BlendNodeData* blendNodes;
		int eventCount;
		EventData* events;
		int transitionArrayCount;
		TransitionArrayData* transitionArrays;
		int transitionCount;
		TransitionData* transitions;
		TransitionData nullTransition;
		int flagIndexCount;
		short* flagIndices;
		int flagCount;
		const char** flags;
		int variableCount;
		const char** variables;
		int layerCount;
		LayerData* layers;
		int blendMaskCount;
		BlendMaskData* blendMasks;
		int maskBoneCount;
		const char** maskBones;
		int triggerCount;
		TriggerData* triggers;
		int triggerTypeCount;
		const char** triggerTypes;
		int colliderCount;
		const char** colliders;
		short blendTreeRootNodeId;
		int blendSpaceCount;
		BlendSpaceData* blendSpaces;
		int childClipIndexCount;
		short* childClipIndices;
	};
}
