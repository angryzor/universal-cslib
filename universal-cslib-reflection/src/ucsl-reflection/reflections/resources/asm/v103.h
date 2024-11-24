#pragma once
#include <ucsl/math.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::animation_state_machine::v103::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ::ucsl::resources::animation_state_machine::v103;

	using ClipDataAnimationSettings = structure<impl::ClipDataAnimationSettings, "ClipDataAnimationSettings", void,
		field<const char*, "resourceName">,
		field<float, "start">,
		field<float, "end">,
		field<float, "speed">,
		field<unsigned char, "flags">,
		field<bool, "loops">
	>;

	using ClipData = structure<impl::ClipData, "ClipData", void,
		field<const char*, "name">,
		field<ClipDataAnimationSettings, "animationSettings">,
		field<unsigned short, "triggerCount">,
		field<short, "triggerOffset">,
		field<short, "blendMaskIndex">,
		field<unsigned short, "childClipIndexCount">,
		field<short, "childClipIndexOffset">
	>;

	using TransitionType = enumeration<impl::TransitionType, "TransitionType", unsigned char,
		option<"IMMEDIATE">,
		option<"FROZEN">,
		option<"SMOOTH">,
		option<"SYNCHRONIZE">,
		option<"HOLD_TO">,
		option<"HOLD_BOTH">,
		option<"WAIT_FROM">,
		option<"WAIT_FROM_HOLD_TO">,
		option<"USER_0">,
		option<"USER_1">,
		option<"USER_2">
	>;

	using TransitionEasingType = enumeration<impl::TransitionEasingType, "TransitionEasingType", unsigned char,
		option<"LINEAR">,
		option<"CUBIC">
	>;

	using TransitionDataTransitionInfo = structure<impl::TransitionDataTransitionInfo, "TransitionDataTransitionInfo", void,
		field<TransitionType, "type">,
		field<TransitionEasingType, "easingType">,
		field<short, "targetStateIndex">,
		field<float, "transitionTime">
	>;

	using TransitionData = structure<impl::TransitionData, "TransitionData", void,
		field<TransitionDataTransitionInfo, "transitionInfo">,
		field<short, "transitionTimeVariableIndex">
	>;

	using StateType = enumeration<impl::StateType, "StateType", char,
		fixed_option<"NULL_STATE", -1>,
		option<"CLIP">,
		option<"BLEND_TREE">,
		option<"NONE">
	>;

	using StateData = structure<impl::StateData, "StateData", void,
		field<const char*, "name">,
		field<StateType, "type">,
		field<bool, "transitImmediately">,
		field<unsigned char, "flags">,
		field<char, "defaultLayerIndex">,
		field<short, "rootBlendNodeOrClipIndex">,
		field<short, "maxCycles">,
		field<float, "speed">,
		field<short, "speedVariableIndex">,
		field<unsigned short, "eventCount">,
		field<short, "eventOffset">,
		field<short, "transitionArrayIndex">,
		field<TransitionData, "stateEndTransition">,
		field<unsigned short, "flagIndexCount">,
		field<short, "flagIndexOffset">,
		field<float, "pbaBlendMaskOpacity">
	>;

	using BlendNodeType = enumeration< impl::BlendNodeType, "BlendNodeType", unsigned char,
		option<"LERP">,
		option<"ADDITIVE">,
		option<"CLIP">,
		option<"OVERRIDE">,
		option<"LAYER">,
		option<"MULTIPLY">,
		option<"BLEND_SPACE">,
		option<"TWO_POINT_LERP">
	>;

	using BlendNodeData = structure<impl::BlendNodeData, "BlendNodeData", void,
		field<BlendNodeType, "type">,
		field<short, "blendSpaceIndex">,
		field<short, "blendFactorVariableIndex">,
		field<float, "blendFactorTarget">,
		field<unsigned short, "childNodeArraySize">,
		field<short, "childNodeArrayOffset">
	>;

	using TransitionArrayData = structure<impl::TransitionArrayData, "TransitionArrayData", void,
		field<int, "size">,
		field<int, "offset">
	>;

	using EventData = structure<impl::EventData, "EventData", void,
		field<const char*, "name">,
		field<TransitionData, "transition">
	>;

	using LayerData = structure<impl::LayerData, "LayerData", void,
		field<const char*, "name">,
		field<unsigned short, "layerId">,
		field<short, "blendMaskIndex">
	>;

	using BlendMaskData = structure<impl::BlendMaskData, "BlendMaskData", void,
		field<const char*, "name">,
		field<unsigned short, "maskBoneCount">,
		field<short, "maskBoneOffset">
	>;

	using TriggerType = enumeration<impl::TriggerType, "TriggerType", unsigned char,
		option<"HIT">,
		option<"ENTER_LEAVE">
	>;

	using TriggerData = structure<impl::TriggerData, "TriggerData", void,
		field<TriggerType, "type">,
		field<float, "unknown2">,
		field<float, "unknown3">,
		field<unsigned short, "triggerTypeIndex">,
		field<short, "colliderIndex">,
		field<const char*, "name">
	>;

	using BlendSpaceDataTriangle = structure<impl::BlendSpaceDataTriangle, "BlendSpaceDataTriangle", void,
		field<short, "nodeIndex0">,
		field<short, "nodeIndex1">,
		field<short, "nodeIndex2">,
		field<short, "unused">
	>;

	using BlendSpaceData = structure<impl::BlendSpaceData, "BlendSpaceData", void,
		field<short, "xVariableIndex">,
		field<short, "yVariableIndex">,
		field<float, "xMin">,
		field<float, "xMax">,
		field<float, "yMin">,
		field<float, "yMax">,
		field<unsigned short, "nodeCount">,
		field<unsigned short, "triangleCount">,
		field<dynamic_carray<ucsl::math::Vector2, impl::BlendSpaceData, [](const impl::BlendSpaceData& parent) -> size_t { return parent.nodeCount; }>*, "nodes">,
		field<dynamic_carray<short, impl::BlendSpaceData, [](const impl::BlendSpaceData& parent) -> size_t { return parent.nodeCount; }>*, "clipIndices">,
		field<dynamic_carray<BlendSpaceDataTriangle, impl::BlendSpaceData, [](const impl::BlendSpaceData& parent) -> size_t { return parent.triangleCount; }>*, "triangles">
	>;

	using AsmData = structure<impl::AsmData, "AsmData", void,
		field<constant<unsigned int, 0x41534D46>, "magic">,
		field<constant<int, 0x103>, "version">,
		field<int, "clipCount">,
		field<dynamic_carray<ClipData, impl::AsmData, [](const impl::AsmData& parent) -> size_t { return parent.clipCount; }>*, "clips">,
		field<int, "stateCount">,
		field<dynamic_carray<StateData, impl::AsmData, [](const impl::AsmData& parent) -> size_t { return parent.stateCount; }>*, "states">,
		field<int, "blendNodeCount">,
		field<dynamic_carray<BlendNodeData, impl::AsmData, [](const impl::AsmData& parent) -> size_t { return parent.blendNodeCount; }>*, "blendNodes">,
		field<int, "eventCount">,
		field<dynamic_carray<EventData, impl::AsmData, [](const impl::AsmData& parent) -> size_t { return parent.eventCount; }>*, "events">,
		field<int, "transitionArrayCount">,
		field<dynamic_carray<TransitionArrayData, impl::AsmData, [](const impl::AsmData& parent) -> size_t { return parent.transitionArrayCount; }>*, "transitionArrays">,
		field<int, "transitionCount">,
		field<dynamic_carray<TransitionData, impl::AsmData, [](const impl::AsmData& parent) -> size_t { return parent.transitionCount; }>*, "transitions">,
		field<TransitionData, "nullTransition">,
		field<int, "flagIndexCount">,
		field<dynamic_carray<short, impl::AsmData, [](const impl::AsmData& parent) -> size_t { return parent.flagIndexCount; }>*, "flagIndices">,
		field<int, "flagCount">,
		field<dynamic_carray<const char*, impl::AsmData, [](const impl::AsmData& parent) -> size_t { return parent.flagCount; }>*, "flags">,
		field<int, "variableCount">,
		field<dynamic_carray<const char*, impl::AsmData, [](const impl::AsmData& parent) -> size_t { return parent.variableCount; }>*, "variables">,
		field<int, "layerCount">,
		field<dynamic_carray<LayerData, impl::AsmData, [](const impl::AsmData& parent) -> size_t { return parent.layerCount; }>*, "layers">,
		field<int, "blendMaskCount">,
		field<dynamic_carray<BlendMaskData, impl::AsmData, [](const impl::AsmData& parent) -> size_t { return parent.blendMaskCount; }>*, "blendMasks">,
		field<int, "maskBoneCount">,
		field<dynamic_carray<const char*, impl::AsmData, [](const impl::AsmData& parent) -> size_t { return parent.maskBoneCount; }>*, "maskBones">,
		field<int, "triggerCount">,
		field<dynamic_carray<TriggerData, impl::AsmData, [](const impl::AsmData& parent) -> size_t { return parent.triggerCount; }>*, "triggers">,
		field<int, "triggerTypeCount">,
		field<dynamic_carray<const char*, impl::AsmData, [](const impl::AsmData& parent) -> size_t { return parent.triggerTypeCount; }>*, "triggerTypes">,
		field<int, "colliderCount">,
		field<dynamic_carray<const char*, impl::AsmData, [](const impl::AsmData& parent) -> size_t { return parent.colliderCount; }>*, "colliders">,
		field<short, "blendTreeRootNodeId">,
		field<int, "blendSpaceCount">,
		field<dynamic_carray<BlendSpaceData, impl::AsmData, [](const impl::AsmData& parent) -> size_t { return parent.blendSpaceCount; }>*, "blendSpaces">,
		field<int, "childClipIndexCount">,
		field<dynamic_carray<short, impl::AsmData, [](const impl::AsmData& parent) -> size_t { return parent.childClipIndexCount; }>*, "childClipIndices">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::animation_state_machine::v103::ClipDataAnimationSettings> { using type = ucsl::resources::animation_state_machine::v103::reflections::ClipDataAnimationSettings; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103::ClipData> { using type = ucsl::resources::animation_state_machine::v103::reflections::ClipData; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103::TransitionType> { using type = ucsl::resources::animation_state_machine::v103::reflections::TransitionType; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103::TransitionEasingType> { using type = ucsl::resources::animation_state_machine::v103::reflections::TransitionEasingType; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103::TransitionDataTransitionInfo> { using type = ucsl::resources::animation_state_machine::v103::reflections::TransitionDataTransitionInfo; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103::TransitionData> { using type = ucsl::resources::animation_state_machine::v103::reflections::TransitionData; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103::StateType> { using type = ucsl::resources::animation_state_machine::v103::reflections::StateType; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103::StateData> { using type = ucsl::resources::animation_state_machine::v103::reflections::StateData; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103::BlendNodeType> { using type = ucsl::resources::animation_state_machine::v103::reflections::BlendNodeType; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103::BlendNodeData> { using type = ucsl::resources::animation_state_machine::v103::reflections::BlendNodeData; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103::TransitionArrayData> { using type = ucsl::resources::animation_state_machine::v103::reflections::TransitionArrayData; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103::EventData> { using type = ucsl::resources::animation_state_machine::v103::reflections::EventData; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103::LayerData> { using type = ucsl::resources::animation_state_machine::v103::reflections::LayerData; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103::BlendMaskData> { using type = ucsl::resources::animation_state_machine::v103::reflections::BlendMaskData; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103::TriggerType> { using type = ucsl::resources::animation_state_machine::v103::reflections::TriggerType; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103::TriggerData> { using type = ucsl::resources::animation_state_machine::v103::reflections::TriggerData; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103::BlendSpaceDataTriangle> { using type = ucsl::resources::animation_state_machine::v103::reflections::BlendSpaceDataTriangle; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103::BlendSpaceData> { using type = ucsl::resources::animation_state_machine::v103::reflections::BlendSpaceData; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103::AsmData> { using type = ucsl::resources::animation_state_machine::v103::reflections::AsmData; };
}
