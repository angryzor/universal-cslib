//#pragma once
//#include "../../reflection/declaration.h"
//
//namespace reflection {
//	using Rotation3 = structure<"SurfRideRotation3", void,
//		field<int, "x">,
//		field<int, "y">,
//		field<int, "z">
//	>;
//	
//	using SRS_PROJECT = structure<"SRS_PROJECT", void,
//		field<const char*, "name",>,
//		field<unsigned short, "sceneCount",>,
//		field<unsigned short, "unk1",>,
//		field<unsigned short, "textureListCount",>,
//		field<unsigned short, "fontCount",>,
//		field<RflDynamicPointerArray < SRS_SCENE, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_PROJECT*>(parent)->sceneCount; } > , "scenes",>,
//		field<RflDynamicPointerArray < SRS_TEXTURELIST, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_PROJECT*>(parent)->textureListCount; } > , "textureLists",>,
//		field<RflDynamicPointerArray < SRS_FONT, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_PROJECT*>(parent)->fontCount; } > , "fonts",>,
//		field<SRS_CAMERA, "camera",>,
//		field<int, "startFrame",>,
//		field<int, "endFrame",>,
//		field<float, "frameRate",>,
//		field<SRS_USERDATA*, "userData">
//	>;
//
//auto srsSceneMembers = CreateRflClassMembers<void,
//	const char*,
//	int,
//	int,
//	int,
//	unsigned int,
//	RflDynamicPointerArray < SRS_LAYER, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_SCENE*>(parent)->layerCount; } > ,
//	unsigned short,
//	unsigned short,
//	RflDynamicPointerArray < SRS_CAMERA, [](const void* parent) -> size_t {  return static_cast<const SurfRide::SRS_SCENE*>(parent)->cameraCount; } > ,
//	int,
//	Vector2,
//	SRS_USERDATA*
//> (
//	"name",
//	"id",
//	"flags",
//	"unk1",
//	"layerCount",
//	"layers",
//	"cameraCount",
//	"currentCameraIndex",
//	"cameras",
//	"backgroundColor",
//	"resolution",
//	"userData"
//);
//
//auto srsCameraMembers = CreateRflClassMembers<void,
//	const char*,
//	int,
//	csl::math::Vector3,
//	csl::math::Vector3,
//	bool,
//	int,
//	float,
//	float,
//	float,
//	uint64_t
//>(
//	"name",
//	"id",
//	"position",
//	"target",
//	"isOrthogonal",
//	"fov",
//	"nearClip",
//	"farClip",
//	"unk1",
//	"unk2"
//);
//
//auto srsLayerMembers = CreateRflClassMembers<void,
//	const char*,
//	int,
//	int,
//	unsigned int,
//	RflDynamicPointerArray < SRS_CASTNODE, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_LAYER*>(parent)->castCount; } > ,
//	RflDynamicStructPointerArray <
//	[](const void* parent) { return static_cast<const SurfRide::SRS_LAYER*>(parent)->flags & 0x1 ? &RflClassTraits<SurfRide::SRS_TRS3D>::rflClass : &RflClassTraits<SurfRide::SRS_TRS2D>::rflClass; },
//	[](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_LAYER*>(parent)->castCount; }
//	> ,
//	int,
//	RflDynamicPointerArray < SRS_ANIMATION, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_LAYER*>(parent)->animationCount; } > ,
//	int,
//	SRS_USERDATA*
//> (
//	"name",
//	"id",
//	"flags",
//	"castCount",
//	"casts",
//	"transforms",
//	"animationCount",
//	"animations",
//	"currentAnimationIndex",
//	"userData"
//);
//
//auto srsTrsBaseMembers = CreateRflClassMembers<void,
//	SurfRide::Color,
//	SurfRide::Color,
//	bool
//>(
//	"materialColor",
//	"illuminationColor",
//	"display"
//);
//
//auto srsTrs2DMembers = CreateRflClassMembers<SurfRide::SRS_TRS_BASE,
//	Vector2,
//	uint32_t,
//	Vector2,
//	uint32_t,
//	uint32_t
//>(
//	"position",
//	"rotation",
//	"scale",
//	"unkX",
//	"unkY"
//);
//
//auto srsTrs3DMembers = CreateRflClassMembers<SurfRide::SRS_TRS_BASE,
//	Vector3,
//	Rotation3,
//	Vector3
//>(
//	"position",
//	"rotation",
//	"scale"
//);
//
//auto srsSliceMembers = CreateRflClassMembers<void,
//	uint32_t,
//	Vector2,
//	SurfRide::Color,
//	SurfRide::Color,
//	SurfRide::Color,
//	SurfRide::Color,
//	SurfRide::Color,
//	SurfRide::Color,
//	short,
//	short
//>(
//	"flags",
//	"fixedSize",
//	"materialColor",
//	"illuminationColor",
//	"vertexColorTopLeft",
//	"vertexColorBottomLeft",
//	"vertexColorTopRight",
//	"vertexColorBottomRight",
//	"cropIndex0",
//	"cropIndex1"
//);
//
//auto srsCastNodeMembers = CreateRflClassMembers<void,
//	const char*,
//	int,
//	int,
//	RflDynamicStructPointer < [](const void* parent) -> const hh::fnd::RflClass* {
//	switch (static_cast<const SurfRide::SRS_CASTNODE*>(parent)->GetType()) {
//	case SurfRide::SRS_CASTNODE::Type::NORMAL: return nullptr;
//	case SurfRide::SRS_CASTNODE::Type::IMAGE: return &RflClassTraits<SurfRide::SRS_IMAGECAST>::rflClass;
//	case SurfRide::SRS_CASTNODE::Type::SLICE: return &RflClassTraits<SurfRide::SRS_SLICECAST>::rflClass;
//	case SurfRide::SRS_CASTNODE::Type::REFERENCE: return &RflClassTraits<SurfRide::SRS_REFERENCECAST>::rflClass;
//	default: assert(false); return nullptr;
//	};
//	} > ,
//	short,
//		short,
//		SRS_USERDATA*
//> (
//	"name",
//	"id",
//	"flags",
//	"data",
//	"childIndex",
//	"siblingIndex",
//	"userData"
//);
//
//	auto srsTextDataMembers = CreateRflClassMembers<void,
//		unsigned int,
//		unsigned int,
//		const char*,
//		Vector2,
//		short,
//		short,
//		short,
//		short,
//		short,
//		short,
//		SRS_FONT*
//	>(
//		"flags",
//		"fontIndex",
//		"str",
//		"scale",
//		"paddingLeft",
//		"paddingRight",
//		"paddingTop",
//		"paddingBottom",
//		"tracking",
//		"lineHeight",
//		"font"
//	);
//
//	auto srsImageCastMembers = CreateRflClassMembers<void,
//		uint32_t,
//		Vector2,
//		Vector2,
//		SurfRide::Color,
//		SurfRide::Color,
//		SurfRide::Color,
//		SurfRide::Color,
//		short,
//		short,
//		short,
//		short,
//		RflDynamicPointerArray < SRS_CROPREF, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_IMAGECAST*>(parent)->cropRef0Count; } > ,
//		RflDynamicPointerArray < SRS_CROPREF, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_IMAGECAST*>(parent)->cropRef1Count; } > ,
//		SRS_TEXTDATA*,
//		uint32_t,
//		RflDynamicStructPointer < [](const void* parent) -> const hh::fnd::RflClass* {
//		switch (static_cast<const SurfRide::SRS_IMAGECAST*>(parent)->effectType) {
//		case SurfRide::SRE_EFFECT_TYPE::BLUR: return &RflClassTraits<SurfRide::SRS_BLUR3D>::rflClass;
//		case SurfRide::SRE_EFFECT_TYPE::REFLECT: return &RflClassTraits<SurfRide::SRS_REFLECT3D>::rflClass;
//		default: assert(false); return nullptr;
//		}
//		} >
//	> (
//		"flags",
//		"size",
//		"pivot",
//		"vertexColorTopLeft",
//		"vertexColorBottomLeft",
//		"vertexColorTopRight",
//		"vertexColorBottomRight",
//		"cropIndex0",
//		"cropIndex1",
//		"cropRef0Count",
//		"cropRef1Count",
//		"cropRef0Offset",
//		"cropRef1Offset",
//		"textData",
//		"effectType",
//		"effectData"
//	);
//
//		auto srsReferenceCastMembers = CreateRflClassMembers<void,
//			SRS_LAYER*,
//			int,
//			uint32_t,
//			uint32_t,
//			int
//		>(
//			"layer",
//			"flags",
//			"animationId",
//			"animationFrame",
//			"unk2"
//		);
//
//		auto srsSliceCastMembers = CreateRflClassMembers<void,
//			uint32_t,
//			Vector2,
//			Vector2,
//			SurfRide::Color,
//			SurfRide::Color,
//			SurfRide::Color,
//			SurfRide::Color,
//			Vector2,
//			short,
//			short,
//			short,
//			short,
//			short,
//			short,
//			RflDynamicPointerArray < SRS_CROPREF, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_SLICECAST*>(parent)->cropRef0Count; } > ,
//			uint32_t,
//			RflDynamicStructPointer < [](const void* parent) -> const hh::fnd::RflClass* {
//			switch (static_cast<const SurfRide::SRS_SLICECAST*>(parent)->effectType) {
//			case SurfRide::SRE_EFFECT_TYPE::BLUR: return &RflClassTraits<SurfRide::SRS_BLUR3D>::rflClass;
//			case SurfRide::SRE_EFFECT_TYPE::REFLECT: return &RflClassTraits<SurfRide::SRS_REFLECT3D>::rflClass;
//			default: assert(false); return nullptr;
//			}
//			} > ,
//			RflDynamicInlineArray < SRS_SLICE, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_SLICECAST*>(parent)->sliceHorizontalCount* static_cast<const SurfRide::SRS_SLICECAST*>(parent)->sliceVerticalCount; } >
//		> (
//			"flags",
//			"size",
//			"pivot",
//			"vertexColorTopLeft",
//			"vertexColorBottomLeft",
//			"vertexColorTopRight",
//			"vertexColorBottomRight",
//			"fixedSize",
//			"sliceHorizontalCount",
//			"sliceVerticalCount",
//			"horizontalFixedCount",
//			"verticalFixedCount",
//			"cropRef0Count",
//			"cropRef1Count",
//			"cropRefs0",
//			"effectType",
//			"effectData",
//			"slices"
//		);
//
//			auto srsCropMembers = CreateRflClassMembers<void,
//				float,
//				float,
//				float,
//				float
//			>(
//				"left",
//				"top",
//				"right",
//				"bottom"
//			);
//
//			auto srsCroprefMembers = CreateRflClassMembers<void,
//				short,
//				short,
//				short
//			>(
//				"textureListIndex",
//				"textureIndex",
//				"cropIndex"
//			);
//
//			auto srsTextureMembers = CreateRflClassMembers<void,
//#ifndef DEVTOOLS_TARGET_SDK_wars
//				const char*,
//#endif
//				const char*,
//				unsigned int,
//				unsigned short,
//				unsigned short,
//				unsigned int,
//				unsigned int,
//				RflDynamicPointerArray < SRS_CROP, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_TEXTURE*>(parent)->cropCount; } > ,
//				SRS_USERDATA*
//			> (
//#ifndef DEVTOOLS_TARGET_SDK_wars
//				"name",
//#endif
//				"fileName",
//				"id",
//				"width",
//				"height",
//				"flags",
//				"cropCount",
//				"crops",
//				"userData"
//			);
//
//			auto srsTextureListMembers = CreateRflClassMembers<void,
//				const char*,
//				unsigned int,
//				unsigned int,
//				RflDynamicPointerArray < SRS_TEXTURE, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_TEXTURELIST*>(parent)->textureCount; } > ,
//				SRS_USERDATA*
//			> (
//				"name",
//				"unk1",
//				"textureCount",
//				"textures",
//				"userData"
//			);
//
//			auto srsKeyframeMembers = CreateRflClassMembers<void,
//				unsigned int
//			>(
//				"frame"
//			);
//
//			auto srsKeyframeKeyLinearFloatMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, float>("value");
//			//auto srsKeyframeKeyLinearDoubleMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, double>("value");
//			auto srsKeyframeKeyLinearIntMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, int>("value");
//			auto srsKeyframeKeyLinearBoolMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, bool>("value");
//			auto srsKeyframeKeyLinearColorMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, SurfRide::Color>("value");
//			auto srsKeyframeKeyHermiteFloatMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, float, float, float>("value", "derivativeIn", "derivativeOut");
//			//auto srsKeyframeKeyHermiteDoubleMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, double, double, double>("value", "derivativeIn", "derivativeOut");
//			auto srsKeyframeKeyHermiteIntMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, int, int, int>("value", "derivativeIn", "derivativeOut");
//			auto srsKeyframeKeyHermiteBoolMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, bool, bool, bool>("value", "derivativeIn", "derivativeOut");
//			auto srsKeyframeKeyHermiteColorMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, SurfRide::Color, SurfRide::Color, SurfRide::Color>("value", "derivativeIn", "derivativeOut");
//			auto srsKeyframeKeyIndividualFloatMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, float, float, float, SRE_INTERPOLATION_TYPE>("value", "derivativeIn", "derivativeOut", "interpolationType");
//			//auto srsKeyframeKeyIndividualDoubleMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, double, double, double, SRE_INTERPOLATION_TYPE>("value", "derivativeIn", "derivativeOut", "interpolationType");
//			auto srsKeyframeKeyIndividualIntMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, int, int, int, SRE_INTERPOLATION_TYPE>("value", "derivativeIn", "derivativeOut", "interpolationType");
//			auto srsKeyframeKeyIndividualBoolMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, bool, bool, bool, SRE_INTERPOLATION_TYPE>("value", "derivativeIn", "derivativeOut", "interpolationType");
//			auto srsKeyframeKeyIndividualColorMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, SurfRide::Color, SurfRide::Color, SurfRide::Color, SRE_INTERPOLATION_TYPE>("value", "derivativeIn", "derivativeOut", "interpolationType");
//
//			auto srsTrackMembers = CreateRflClassMembers<void,
//				SRE_CURVE_TYPE,
//				uint16_t,
//				uint16_t,
//				uint32_t,
//				uint32_t,
//				RflDynamicStructPointerArray <
//				[](const void* parent) -> const hh::fnd::RflClass* {
//				auto* track = static_cast<const SurfRide::SRS_TRACK*>(parent);
//
//				switch (track->GetInterpolationType()) {
//				case SurfRide::SRE_TRACK_INTERPOLATION_TYPE::CONSTANT:
//				case SurfRide::SRE_TRACK_INTERPOLATION_TYPE::LINEAR:
//					switch (track->GetDataType()) {
//					case SurfRide::SRE_TRACK_DATA_TYPE::FLOAT: return &RflClassTraits<SurfRide::KeyLinear<float>>::rflClass;
//					case SurfRide::SRE_TRACK_DATA_TYPE::INDEX: return &RflClassTraits<SurfRide::KeyLinear<int>>::rflClass;
//					case SurfRide::SRE_TRACK_DATA_TYPE::INT: return &RflClassTraits<SurfRide::KeyLinear<int>>::rflClass;
//					case SurfRide::SRE_TRACK_DATA_TYPE::BOOL: return &RflClassTraits<SurfRide::KeyLinear<bool>>::rflClass;
//					case SurfRide::SRE_TRACK_DATA_TYPE::COLOR: return &RflClassTraits<SurfRide::KeyLinear<SurfRide::Color>>::rflClass;
//					default: assert(false); return nullptr;
//					}
//				case SurfRide::SRE_TRACK_INTERPOLATION_TYPE::HERMITE:
//					switch (track->GetDataType()) {
//					case SurfRide::SRE_TRACK_DATA_TYPE::FLOAT: return &RflClassTraits<SurfRide::KeyHermite<float>>::rflClass;
//					case SurfRide::SRE_TRACK_DATA_TYPE::INDEX: return &RflClassTraits<SurfRide::KeyHermite<int>>::rflClass;
//					case SurfRide::SRE_TRACK_DATA_TYPE::INT: return &RflClassTraits<SurfRide::KeyHermite<int>>::rflClass;
//					case SurfRide::SRE_TRACK_DATA_TYPE::BOOL: return &RflClassTraits<SurfRide::KeyHermite<bool>>::rflClass;
//					case SurfRide::SRE_TRACK_DATA_TYPE::COLOR: return &RflClassTraits<SurfRide::KeyHermite<SurfRide::Color>>::rflClass;
//					default: assert(false); return nullptr;
//					}
//				case SurfRide::SRE_TRACK_INTERPOLATION_TYPE::INDIVIDUAL:
//					switch (track->GetDataType()) {
//					case SurfRide::SRE_TRACK_DATA_TYPE::FLOAT: return &RflClassTraits<SurfRide::KeyIndividual<float>>::rflClass;
//					case SurfRide::SRE_TRACK_DATA_TYPE::INDEX: return &RflClassTraits<SurfRide::KeyIndividual<int>>::rflClass;
//					case SurfRide::SRE_TRACK_DATA_TYPE::INT: return &RflClassTraits<SurfRide::KeyIndividual<int>>::rflClass;
//					case SurfRide::SRE_TRACK_DATA_TYPE::BOOL: return &RflClassTraits<SurfRide::KeyIndividual<bool>>::rflClass;
//					case SurfRide::SRE_TRACK_DATA_TYPE::COLOR: return &RflClassTraits<SurfRide::KeyIndividual<SurfRide::Color>>::rflClass;
//					default: assert(false); return nullptr;
//					}
//				default: assert(false); return nullptr;
//				}
//				},
//				[](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_TRACK*>(parent)->keyCount; }
//				>
//			> (
//				"trackType",
//				"keyCount",
//				"flags",
//				"firstFrame",
//				"lastFrame",
//				"keyFrame"
//			);
//
//			auto srsMotionMembers = CreateRflClassMembers<void,
//				uint16_t,
//				uint16_t,
//				RflDynamicPointerArray < SRS_TRACK, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_MOTION*>(parent)->trackCount; } >
//			> (
//				"castId",
//				"trackCount",
//				"tracks"
//			);
//
//			auto srsAnimationMembers = CreateRflClassMembers<void,
//				const char*,
//				uint32_t,
//				uint32_t,
//				uint32_t,
//				RflDynamicPointerArray < SRS_MOTION, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_ANIMATION*>(parent)->motionCount; } > ,
//				SRS_USERDATA*,
//				bool
//			> (
//				"name",
//				"id",
//				"motionCount",
//				"frameCount",
//				"motions",
//				"userData",
//				"isLooping"
//			);
//
//			auto srsFontMembers = CreateRflClassMembers<void,
//				const char*,
//				int,
//				unsigned int,
//				short,
//				unsigned short,
//				unsigned short,
//				RflDynamicPointerArray < SRS_CHARACTER_MAPPING, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_FONT*>(parent)->characterCount; } > ,
//				uint64_t,
//				SRS_USERDATA*
//			> (
//				"name",
//				"id",
//				"unk1",
//				"unk2",
//				"characterCount",
//				"unk3",
//				"characters",
//				"unk4",
//				"userData"
//			);
//
//			auto srsCharacterMappingMembers = CreateRflClassMembers<void,
//				unsigned short,
//				short,
//				short,
//				short
//			>(
//				"character",
//				"textureListIndex",
//				"textureIndex",
//				"cropIndex"
//			);
//
//			auto srsDataBoolMembers = CreateRflClassMembers<void,
//				const char*,
//				SRS_DATA::Type,
//				bool*
//			>(
//				"name",
//				"type",
//				"value"
//			);
//
//			auto srsDataIntMembers = CreateRflClassMembers<void,
//				const char*,
//				SRS_DATA::Type,
//				int*
//			>(
//				"name",
//				"type",
//				"value"
//			);
//
//			auto srsDataUintMembers = CreateRflClassMembers<void,
//				const char*,
//				SRS_DATA::Type,
//				unsigned int*
//			>(
//				"name",
//				"type",
//				"value"
//			);
//
//			auto srsDataFloatMembers = CreateRflClassMembers<void,
//				const char*,
//				SRS_DATA::Type,
//				float*
//			>(
//				"name",
//				"type",
//				"value"
//			);
//
//			auto srsDataStringMembers = CreateRflClassMembers<void,
//				const char*,
//				SRS_DATA::Type,
//				const char*
//			>(
//				"name",
//				"type",
//				"value"
//			);
//
//			auto srsUserDataMembers = CreateRflClassMembers<void,
//				unsigned int,
//				RflDynamicStructSelfPointerArray <
//				[](const void* self) -> const hh::fnd::RflClass* {
//				switch (static_cast<const SurfRide::SRS_DATA*>(self)->type) {
//				case SurfRide::SRS_DATA::Type::BOOL: return &RflClassTraits<SurfRide::SRS_DATA_BOOL>::rflClass;
//				case SurfRide::SRS_DATA::Type::INT: return &RflClassTraits<SurfRide::SRS_DATA_INT>::rflClass;
//				case SurfRide::SRS_DATA::Type::UINT: return &RflClassTraits<SurfRide::SRS_DATA_UINT>::rflClass;
//				case SurfRide::SRS_DATA::Type::FLOAT: return &RflClassTraits<SurfRide::SRS_DATA_FLOAT>::rflClass;
//				case SurfRide::SRS_DATA::Type::STRING: return &RflClassTraits<SurfRide::SRS_DATA_STRING>::rflClass;
//				default: assert(false); return nullptr;
//				}
//				},
//				[](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_USERDATA*>(parent)->count; }
//				>
//			> (
//				"itemCount",
//				"items"
//			);
//
//			auto srsBlurMembers = CreateRflClassMembers<void,
//				uint32_t,
//				uint32_t,
//				uint32_t,
//				uint32_t,
//				uint32_t,
//				uint32_t
//			>(
//				"field00",
//				"field04",
//				"cropCount",
//				"steps",
//				"unk1",
//				"flags"
//			);
//
//			auto srsReflectMembers = CreateRflClassMembers<void,
//				uint32_t,
//				uint32_t,
//				uint32_t,
//				float,
//				float,
//				float,
//				float,
//				uint32_t,
//				SurfRide::Color
//			>(
//				"unk1",
//				"unk2",
//				"unk3",
//				"unk4",
//				"unk5",
//				"unk6",
//				"unk7",
//				"flags",
//				"color"
//			);
//}
