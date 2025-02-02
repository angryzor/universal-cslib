#pragma once
#include <cassert>
#include <ucsl/resources/swif/v6.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::swif::v6::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ::ucsl::resources::swif::v6;

	using Vector2 = canonical_t<impl::Vector2>;
	using Vector3 = canonical_t<impl::Vector3>;
	using Matrix34 = canonical_t<impl::Matrix34>;
	using Matrix44 = canonical_t<impl::Matrix44>;
	using Rotation3 = structure<impl::Rotation3, "Rotation3", void,
		field<int, "x">,
		field<int, "y">,
		field<int, "z">
	>;

	using Color = structure<impl::Color, "Color", void,
		field<unsigned char, "r">,
		field<unsigned char, "g">,
		field<unsigned char, "b">,
		field<unsigned char, "a">
	>;

	using ECurveType = enumeration<impl::ECurveType, "ECurveType", unsigned short,
		option<"TranslationX">,
		option<"TranslationY">,
		option<"TranslationZ">,
		option<"RotationX">,
		option<"RotationY">,
		option<"RotationZ">,
		option<"ScaleX">,
		option<"ScaleY">,
		option<"ScaleZ">,
		option<"MaterialColor">,
		option<"Display">,
		option<"Width">,
		option<"Height">,
		option<"VertexColorTopLeft">,
		option<"VertexColorTopRight">,
		option<"VertexColorBottomLeft">,
		option<"VertexColorBottomRight">,
		option<"CropIndex0">,
		option<"CropIndex1">,
		option<"Unknown">,
		option<"IlluminationColor">,
		option<"MaterialColorR">,
		option<"MaterialColorG">,
		option<"MaterialColorB">,
		option<"MaterialColorA">,
		option<"VertexColorTopLeftR">,
		option<"VertexColorTopLeftG">,
		option<"VertexColorTopLeftB">,
		option<"VertexColorTopLeftA">,
		option<"VertexColorTopRightR">,
		option<"VertexColorTopRightG">,
		option<"VertexColorTopRightB">,
		option<"VertexColorTopRightA">,
		option<"VertexColorBottomLeftR">,
		option<"VertexColorBottomLeftG">,
		option<"VertexColorBottomLeftB">,
		option<"VertexColorBottomLeftA">,
		option<"VertexColorBottomRightR">,
		option<"VertexColorBottomRightG">,
		option<"VertexColorBottomRightB">,
		option<"VertexColorBottomRightA">,
		option<"IlluminationColorR">,
		option<"IlluminationColorG">,
		option<"IlluminationColorB">,
		option<"IlluminationColorA">
	>;

	using EInterpolationType = enumeration<impl::EInterpolationType, "EInterpolationType", unsigned short,
		option<"CONSTANT">,
		option<"LINEAR">,
		option<"HERMITE">,
		option<"INDIVIDUAL">
	>;

	using SRS_KEYFRAME = structure<impl::SRS_KEYFRAME, "SRS_KEYFRAME", void,
		field<unsigned int, "frame">
	>;

	template<typename T>
	using Key = structure<impl::Key<T>, "Key", SRS_KEYFRAME,
		field<T, "value">
	>;

	template<typename T>
	using KeyLinear = structure<impl::KeyLinear<T>, "KeyLinear", Key<T>>;

	template<typename T>
	using KeyHermite = structure<impl::KeyHermite<T>, "KeyHermite", KeyLinear<T>,
		field<T, "derivativeIn">,
		field<T, "derivativeOut">
	>;

	template<typename T>
	using KeyIndividual = structure<impl::KeyIndividual<T>, "KeyIndividual", KeyHermite<T>,
		field<EInterpolationType, "interpolationType">
	>;

	using SRS_KEYFRAME_PTR = unionof<impl::SRS_KEYFRAME_PTR, "SRS_KEYFRAME_PTR", impl::SRS_TRACK, [](const impl::SRS_TRACK& track) -> size_t {
		switch (track.GetInterpolationType()) {
		case impl::EInterpolationType::CONSTANT:
			switch (track.GetDataType()) {
			case impl::ETrackDataType::FLOAT: return 0;
			case impl::ETrackDataType::INDEX: return 1;
			case impl::ETrackDataType::INT: return 2;
			case impl::ETrackDataType::BOOL: return 3;
			case impl::ETrackDataType::COLOR: return 4;
			default: assert(false && "Invalid track flags"); return 0;
			}
		case impl::EInterpolationType::LINEAR:
			switch (track.GetDataType()) {
			case impl::ETrackDataType::FLOAT: return 5;
			case impl::ETrackDataType::INDEX: return 6;
			case impl::ETrackDataType::INT: return 7;
			case impl::ETrackDataType::BOOL: return 8;
			case impl::ETrackDataType::COLOR: return 9;
			default: assert(false && "Invalid track flags"); return 0;
			}
		case impl::EInterpolationType::HERMITE:
			switch (track.GetDataType()) {
			case impl::ETrackDataType::FLOAT: return 10;
			case impl::ETrackDataType::INDEX: return 11;
			case impl::ETrackDataType::INT: return 12;
			case impl::ETrackDataType::BOOL: return 13;
			case impl::ETrackDataType::COLOR: return 14;
			default: assert(false && "Invalid track flags"); return 0;
			}
		case impl::EInterpolationType::INDIVIDUAL:
			switch (track.GetDataType()) {
			case impl::ETrackDataType::FLOAT: return 15;
			case impl::ETrackDataType::INDEX: return 16;
			case impl::ETrackDataType::INT: return 17;
			case impl::ETrackDataType::BOOL: return 18;
			case impl::ETrackDataType::COLOR: return 19;
			default: assert(false && "Invalid track flags"); return 0;
			}
		default: assert(false && "Invalid track flags"); return 0;
		}
	},
        field<dynamic_carray<Key<float>, impl::SRS_TRACK, [](const impl::SRS_TRACK& track) -> size_t { return track.keyCount; }>*, "constantFloat">,
        field<dynamic_carray<Key<int>, impl::SRS_TRACK, [](const impl::SRS_TRACK& track) -> size_t { return track.keyCount; }>*, "constantIndex">,
        field<dynamic_carray<Key<int>, impl::SRS_TRACK, [](const impl::SRS_TRACK& track) -> size_t { return track.keyCount; }>*, "constantInt">,
        field<dynamic_carray<Key<bool>, impl::SRS_TRACK, [](const impl::SRS_TRACK& track) -> size_t { return track.keyCount; }>*, "constantBool">,
        field<dynamic_carray<Key<Color>, impl::SRS_TRACK, [](const impl::SRS_TRACK& track) -> size_t { return track.keyCount; }>*, "constantColor">,

        field<dynamic_carray<KeyLinear<float>, impl::SRS_TRACK, [](const impl::SRS_TRACK& track) -> size_t { return track.keyCount; }>*, "linearFloat">,
        field<dynamic_carray<KeyLinear<int>, impl::SRS_TRACK, [](const impl::SRS_TRACK& track) -> size_t { return track.keyCount; }>*, "linearIndex">,
        field<dynamic_carray<KeyLinear<int>, impl::SRS_TRACK, [](const impl::SRS_TRACK& track) -> size_t { return track.keyCount; }>*, "linearInt">,
        field<dynamic_carray<KeyLinear<bool>, impl::SRS_TRACK, [](const impl::SRS_TRACK& track) -> size_t { return track.keyCount; }>*, "linearBool">,
        field<dynamic_carray<KeyLinear<Color>, impl::SRS_TRACK, [](const impl::SRS_TRACK& track) -> size_t { return track.keyCount; }>*, "linearColor">,

        field<dynamic_carray<KeyHermite<float>, impl::SRS_TRACK, [](const impl::SRS_TRACK& track) -> size_t { return track.keyCount; }>*, "hermiteFloat">,
        field<dynamic_carray<KeyHermite<int>, impl::SRS_TRACK, [](const impl::SRS_TRACK& track) -> size_t { return track.keyCount; }>*, "hermiteIndex">,
        field<dynamic_carray<KeyHermite<int>, impl::SRS_TRACK, [](const impl::SRS_TRACK& track) -> size_t { return track.keyCount; }>*, "hermiteInt">,
        field<dynamic_carray<KeyHermite<bool>, impl::SRS_TRACK, [](const impl::SRS_TRACK& track) -> size_t { return track.keyCount; }>*, "hermiteBool">,
        field<dynamic_carray<KeyHermite<Color>, impl::SRS_TRACK, [](const impl::SRS_TRACK& track) -> size_t { return track.keyCount; }>*, "hermiteColor">,

        field<dynamic_carray<KeyIndividual<float>, impl::SRS_TRACK, [](const impl::SRS_TRACK& track) -> size_t { return track.keyCount; }>*, "individualFloat">,
        field<dynamic_carray<KeyIndividual<int>, impl::SRS_TRACK, [](const impl::SRS_TRACK& track) -> size_t { return track.keyCount; }>*, "individualIndex">,
        field<dynamic_carray<KeyIndividual<int>, impl::SRS_TRACK, [](const impl::SRS_TRACK& track) -> size_t { return track.keyCount; }>*, "individualInt">,
        field<dynamic_carray<KeyIndividual<bool>, impl::SRS_TRACK, [](const impl::SRS_TRACK& track) -> size_t { return track.keyCount; }>*, "individualBool">,
        field<dynamic_carray<KeyIndividual<Color>, impl::SRS_TRACK, [](const impl::SRS_TRACK& track) -> size_t { return track.keyCount; }>*, "individualColor">
	>;

	using SRS_TRACK = structure<impl::SRS_TRACK, "SRS_TRACK", void,
		field<ECurveType, "trackType">,
		field<unsigned short, "keyCount">,
		field<unsigned short, "flags">,
		field<unsigned int, "firstFrame">,
		field<unsigned int, "lastFrame">,
		field<SRS_KEYFRAME_PTR, "keyFrames">
	>;

	using SRS_MOTION = structure<impl::SRS_MOTION, "SRS_MOTION", void,
		field<unsigned short, "castId">,
		field<unsigned short, "trackCount">,
		field<dynamic_carray<SRS_TRACK, impl::SRS_MOTION, [](const impl::SRS_MOTION& motion) -> size_t { return motion.trackCount; }>*, "tracks">
	>;

	using SRS_ANIMATION = structure<impl::SRS_ANIMATION, "SRS_ANIMATION", void,
		field<const char*, "name">,
		field<unsigned int, "id">,
		field<unsigned int, "motionCount">,
		field<unsigned int, "frameCount">,
		field<dynamic_carray<SRS_MOTION, impl::SRS_ANIMATION, [](const impl::SRS_ANIMATION& animation) -> size_t { return animation.motionCount; }>*, "motions">,
		field<void*, "userData">,
		field<bool, "isLooping">
	>;

	// Crops
	using SRS_CROP = structure<impl::SRS_CROP, "SRS_CROP", void,
		field<float, "left">,
		field<float, "top">,
		field<float, "right">,
		field<float, "bottom">
	>;

	using SRS_CROPREF = structure<impl::SRS_CROPREF, "SRS_CROPREF", void,
		field<short, "textureListIndex">,
		field<short, "textureIndex">,
		field<short, "cropIndex">
	>;

	// User data
	using EDataType = enumeration<impl::SRS_DATA::Type, "EDataType", unsigned int,
		option<"BOOL">,
		option<"INT">,
		option<"UINT">,
		option<"FLOAT">,
		option<"UNK">,
		option<"STRING">
	>;

	using SRS_DATA_VALUE_PTR = unionof<impl::SRS_DATA_VALUE_PTR, "SRS_CASTNODE_PTR", impl::SRS_DATA, [](const impl::SRS_DATA& data) -> size_t {
		switch (data.type) {
		case impl::SRS_DATA::Type::BOOL: return 0;
		case impl::SRS_DATA::Type::INT: return 1;
		case impl::SRS_DATA::Type::UINT: return 2;
		case impl::SRS_DATA::Type::FLOAT: return 3;
		case impl::SRS_DATA::Type::STRING: return 4;
		default: assert(false && "Unknown data type"); return 0;
		}
	},
		field<bool*, "b">,
		field<int*, "i32">,
		field<unsigned int*, "u32">,
		field<float*, "float">,
		field<const char*, "u32">
	>;

	using SRS_DATA = structure<impl::SRS_DATA, "SRS_DATA", void,
		field<const char*, "name">,
		field<EDataType, "type">,
		field<SRS_DATA_VALUE_PTR, "value">
	>;

	using SRS_USERDATA = structure<impl::SRS_USERDATA, "SRS_USERDATA", void,
		field<unsigned int, "count">,
		field<dynamic_carray<SRS_DATA, impl::SRS_USERDATA, [](const impl::SRS_USERDATA& userData) -> size_t { return userData.count; }>*, "items">
	>;

	// Textures
	using SRS_TEXTURE = structure<impl::SRS_TEXTURE, "SRS_TEXTURE", void,
		field<const char*, "name">,
		field<const char*, "filename">,
		field<unsigned int, "id">,
		field<unsigned short, "width">,
		field<unsigned short, "height">,
		field<unsigned int, "flags">,
		field<unsigned int, "cropCount">,
		field<dynamic_carray<SRS_CROP, impl::SRS_TEXTURE, [](const impl::SRS_TEXTURE& texture) -> size_t { return texture.cropCount; }>*, "crops">,
		field<SRS_USERDATA*, "userData">
	>;

	using SRS_TEXTURELIST = structure<impl::SRS_TEXTURELIST, "SRS_TEXTURELIST", void,
		field<const char*, "name">,
		field<unsigned int, "unk1">,
		field<unsigned int, "textureCount">,
		field<dynamic_carray<SRS_TEXTURE, impl::SRS_TEXTURELIST, [](const impl::SRS_TEXTURELIST& textureList) -> size_t { return textureList.textureCount; }>*, "textures">,
		field<SRS_USERDATA*, "userData">
	>;

	// Fonts
	using SRS_CHARACTER_MAPPING = structure<impl::SRS_CHARACTER_MAPPING, "SRS_CHARACTER_MAPPING", void,
		field<unsigned short, "character">,
		field<SRS_CROPREF, "cropRef">
	>;

	using SRS_FONT = structure<impl::SRS_FONT, "SRS_FONT", void,
		field<const char*, "name">,
		field<unsigned int, "id">,
		field<unsigned int, "unk1">,
		field<short, "unk2">,
		field<unsigned short, "characterCount">,
		field<unsigned short, "unk3">,
		field<dynamic_carray< SRS_CHARACTER_MAPPING, impl::SRS_FONT, [](const impl::SRS_FONT& font) -> size_t { return font.characterCount; }>*, "characters">,
		field<unsigned long long, "unk4">,
		field<SRS_USERDATA*, "userData">
	>;

	// Text
	using SRS_TEXTDATA = structure<impl::SRS_TEXTDATA, "SRS_TEXTDATA", void,
		field<unsigned int, "flags">,
		field<unsigned int, "fontIndex">,
		field<const char*, "text">,
		field<Vector2, "scale">,
		field<short, "paddingLeft">,
		field<short, "paddingRight">,
		field<short, "paddingTop">,
		field<short, "paddingBottom">,
		field<short, "tracking">,
		field<short, "lineHeight">,
		field<SRS_FONT*, "font">
	>;

	// Effects
	using SRS_EFFECT = structure<impl::SRS_EFFECT, "SRS_EFFECT", void>;

	using SRS_BLUR = structure<impl::SRS_BLUR, "SRS_BLUR", SRS_EFFECT,
		field<unsigned int, "unk1">,
		field<unsigned int, "unk2">,
		field<unsigned int, "cropCount">,
		field<unsigned int, "steps">,
		field<int, "duration">,
		field<unsigned int, "flags">,
		field<Color, "color">
	>;

	using SRS_BLUR3D = structure<impl::SRS_BLUR3D, "SRS_BLUR3D", SRS_BLUR>;

	using SRS_REFLECT = structure<impl::SRS_REFLECT, "SRS_REFLECT", SRS_EFFECT,
		field<unsigned int, "unk1">,
		field<unsigned int, "unk2">,
		field<unsigned int, "unk3">,
		field<float, "unk4">,
		field<float, "unk5">,
		field<float, "unk6">,
		field<float, "unk7">,
		field<unsigned int, "flags">,
		field<Color, "color">
	>;

	using SRS_REFLECT3D = structure<impl::SRS_REFLECT3D, "SRS_REFLECT3D", SRS_REFLECT>;

	// Transforms
	using SRS_TRS_BASE = structure<impl::SRS_TRS_BASE, "SRS_TRS_BASE", void,
		field<Color, "materialColor">,
		field<Color, "illuminationColor">,
		field<bool, "display">
	>;

	using SRS_TRS2D = structure<impl::SRS_TRS2D, "SRS_TRS2D", SRS_TRS_BASE,
		field<Vector2, "position">,
		field<int, "rotation">,
		field<Vector2, "scale">,
		field<unsigned int, "unkX">,
		field<unsigned int, "unkY">
	>;

	using SRS_TRS3D = structure<impl::SRS_TRS3D, "SRS_TRS3D", SRS_TRS_BASE,
		field<Vector3, "position">,
		field<Rotation3, "rotation">,
		field<Vector3, "scale">
	>;

	// Casts
	template<typename Parent>
	using SRS_EFFECT_PTR = unionof<impl::SRS_EFFECT_PTR, "SRS_EFFECT_PTR", Parent, [](const Parent& cast) -> size_t {
		switch (cast.GetEffectType()) {
		case impl::EEffectType::NONE: return 0;
		case impl::EEffectType::BLUR: return 1;
		case impl::EEffectType::REFLECT: return 2;
		default: assert(false && "invalid type"); return 0;
		}
	},
		field<void*, "none">,
		field<SRS_BLUR3D*, "blur">,
		field<SRS_REFLECT3D*, "reflect">
	>;

	using SRS_IMAGECAST = structure<impl::SRS_IMAGECAST, "SRS_IMAGECAST", void,
		field<unsigned int, "flags">,
		field<Vector2, "size">,
		field<Vector2, "pivot">,
		field<Color, "vertexColorTopLeft">,
		field<Color, "vertexColorBottomLeft">,
		field<Color, "vertexColorTopRight">,
		field<Color, "vertexColorBottomRight">,
		field<short, "cropIndex0">,
		field<short, "cropIndex1">,
		field<short, "cropRef0Count">,
		field<short, "cropRef1Count">,
		field<dynamic_carray<SRS_CROPREF, impl::SRS_IMAGECAST, [](const impl::SRS_IMAGECAST& imageCast) -> size_t { return imageCast.cropRef0Count; }>*, "cropRefs0">,
		field<dynamic_carray<SRS_CROPREF, impl::SRS_IMAGECAST, [](const impl::SRS_IMAGECAST& imageCast) -> size_t { return imageCast.cropRef1Count; }>*, "cropRefs1">,
		field<SRS_TEXTDATA*, "textData">,
		field<unsigned int, "effectType">,
		field<SRS_EFFECT_PTR<impl::SRS_IMAGECAST>, "effectData">
	>;

	using SRS_SLICE = structure<impl::SRS_SLICE, "SRS_SLICE", void,
		field<unsigned int, "flags">,
		field<Vector2, "fixedSize">,
		field<Color, "materialColor">,
		field<Color, "illuminationColor">,
		field<Color, "vertexColorTopLeft">,
		field<Color, "vertexColorBottomLeft">,
		field<Color, "vertexColorTopRight">,
		field<Color, "vertexColorBottomRight">,
		field<short, "cropIndex0">
	>;

	using SRS_SLICECAST = structure<impl::SRS_SLICECAST, "SRS_SLICECAST", void,
		field<unsigned int, "flags">,
		field<Vector2, "size">,
		field<Vector2, "pivot">,
		field<Color, "vertexColorTopLeft">,
		field<Color, "vertexColorBottomLeft">,
		field<Color, "vertexColorTopRight">,
		field<Color, "vertexColorBottomRight">,
		field<Vector2, "fixedSize">,
		field<short, "sliceHorizontalCount">,
		field<short, "sliceVerticalCount">,
		field<short, "horizontalFixedCount">,
		field<short, "verticalFixedCount">,
		field<short, "cropRef0Count">,
		field<dynamic_carray<SRS_CROPREF, impl::SRS_SLICECAST, [](const impl::SRS_SLICECAST& imageCast) -> size_t { return imageCast.cropRef0Count; }>*, "cropRefs0">,
		field<unsigned int, "effectType">,
		field<SRS_EFFECT_PTR<impl::SRS_SLICECAST>, "effectData">,
		field<dynamic_carray<SRS_SLICE, impl::SRS_SLICECAST, [](const impl::SRS_SLICECAST& sliceCast) -> size_t { return sliceCast.sliceHorizontalCount * sliceCast.sliceVerticalCount; }>, "slices">
	>;

	using SRS_REFERENCECAST = structure<impl::SRS_REFERENCECAST, "SRS_REFERENCECAST", void,
		field<void*, "layer">,
		field<unsigned int, "flags">,
		field<unsigned int, "animationId">,
		field<unsigned int, "animationFrame">,
		field<int, "unk2">
	>;

	using SRS_CAST_PTR = unionof<impl::SRS_CAST_PTR, "SRS_CASTNODE_PTR", impl::SRS_CASTNODE, [](const impl::SRS_CASTNODE& castNode) -> size_t {
		switch (castNode.GetType()) {
		case impl::SRS_CASTNODE::Type::NORMAL: return 0;
		case impl::SRS_CASTNODE::Type::IMAGE: return 1;
		case impl::SRS_CASTNODE::Type::SLICE: return 2;
		case impl::SRS_CASTNODE::Type::REFERENCE: return 3;
		default: assert(false && "invalid type"); return 0;
		}
	},
		field<void*, "none">,
		field<SRS_IMAGECAST*, "image">,
		field<SRS_SLICECAST*, "slice">,
		field<SRS_REFERENCECAST*, "reference">
	>;

	using SRS_CASTNODE = structure<impl::SRS_CASTNODE, "SRS_CASTNODE", void,
		field<const char*, "name">,
		field<unsigned int, "id">,
		field<unsigned int, "flags">,
		field<SRS_CAST_PTR, "data">,
		field<short, "childIndex">,
		field<short, "siblingIndex">,
		field<SRS_USERDATA*, "userData">
	>;

	// Layers
	using SRS_TRS_PTR = unionof<impl::SRS_TRS_PTR, "SRS_TRS_PTR", impl::SRS_LAYER, [](const impl::SRS_LAYER& layer) -> size_t { return layer.Is3D() ? 1 : 0; },
		field<dynamic_carray<SRS_TRS2D, impl::SRS_LAYER, [](const impl::SRS_LAYER& layer) -> size_t { return layer.castCount; }>*, "transforms2d">,
		field<dynamic_carray<SRS_TRS3D, impl::SRS_LAYER, [](const impl::SRS_LAYER& layer) -> size_t { return layer.castCount; }>*, "transforms3d">
	>;

	using SRS_LAYER = structure<impl::SRS_LAYER, "SRS_LAYER", void,
		field<const char*, "name">,
		field<unsigned int, "id">,
		field<unsigned int, "flags">,
		field<int, "castCount">,
		field<dynamic_carray<SRS_CASTNODE, impl::SRS_LAYER, [](const impl::SRS_LAYER& layer) -> size_t { return layer.castCount; }>*, "casts">,
		field<SRS_TRS_PTR, "transforms">,
		field<int, "animationCount">,
		field<dynamic_carray<SRS_ANIMATION, impl::SRS_LAYER, [](const impl::SRS_LAYER& layer) -> size_t { return layer.animationCount; }>*, "animations">,
		field<int, "currentAnimationIndex">,
		field<SRS_USERDATA*, "userData">
	>;

	// Cameras
	using SRS_CAMERA = structure<impl::SRS_CAMERA, "SRS_CAMERA", void,
		field<const char*, "name">,
		field<unsigned int, "id">,
		field<Vector3, "position">,
		field<Vector3, "target">,
		field<bool, "isOrthogonal">,
		field<int, "fov">,
		field<float, "nearPlane">,
		field<float, "farPlane">,
		field<float, "unk1">,
		field<unsigned long long, "unk2">
	>;

	// Scenes
	using SRS_SCENE = structure<impl::SRS_SCENE, "SRS_SCENE", void,
		field<const char*, "name">,
		field<unsigned int, "id">,
		field<unsigned int, "flags">,
		field<bool, "loaded">,
		field<int, "layerCount">,
		field<dynamic_carray<SRS_LAYER, impl::SRS_SCENE, [](const impl::SRS_SCENE& scene) -> size_t { return scene.layerCount; }>*, "layers">,
		field<unsigned short, "cameraCount">,
		field<unsigned short, "currentCameraIndex">,
		field<dynamic_carray<SRS_CAMERA, impl::SRS_SCENE, [](const impl::SRS_SCENE& scene) -> size_t { return scene.cameraCount; }>*, "cameras">,
		field<Color, "backgroundColor">,
		field<Vector2, "resolution">,
		field<SRS_USERDATA*, "userData">
	>;

	// Projects
	using SRS_PROJECT = structure<impl::SRS_PROJECT, "SRS_PROJECT", void,
		field<const char*, "name">,
		field<unsigned short, "sceneCount">,
		field<unsigned short, "unk1">,
		field<unsigned short, "textureListCount">,
		field<unsigned short, "fontCount">,
		field<dynamic_carray<SRS_SCENE, impl::SRS_PROJECT, [](const impl::SRS_PROJECT& project) -> size_t { return project.sceneCount; }>*, "scenes">,
		field<dynamic_carray<SRS_TEXTURELIST, impl::SRS_PROJECT, [](const impl::SRS_PROJECT& project) -> size_t { return project.textureListCount; }>*, "textureLists">,
		field<dynamic_carray<SRS_FONT, impl::SRS_PROJECT, [](const impl::SRS_PROJECT& project) -> size_t { return project.fontCount; }>*, "fonts">,
		field<SRS_CAMERA, "camera">,
		field<int, "startFrame">,
		field<int, "endFrame">,
		field<float, "frameRate">,
		field<SRS_USERDATA*, "userData">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::swif::v6::Rotation3> { using type = ucsl::resources::swif::v6::reflections::Rotation3; };
	template<> struct canonical<ucsl::resources::swif::v6::Color> { using type = ucsl::resources::swif::v6::reflections::Color; };
	template<> struct canonical<ucsl::resources::swif::v6::ECurveType> { using type = ucsl::resources::swif::v6::reflections::ECurveType; };
	template<> struct canonical<ucsl::resources::swif::v6::EInterpolationType> { using type = ucsl::resources::swif::v6::reflections::EInterpolationType; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_KEYFRAME> { using type = ucsl::resources::swif::v6::reflections::SRS_KEYFRAME; };
	template<> struct canonical<ucsl::resources::swif::v6::Key<float>> { using type = ucsl::resources::swif::v6::reflections::Key<float>; };
	template<> struct canonical<ucsl::resources::swif::v6::Key<int>> { using type = ucsl::resources::swif::v6::reflections::Key<int>; };
	template<> struct canonical<ucsl::resources::swif::v6::Key<bool>> { using type = ucsl::resources::swif::v6::reflections::Key<bool>; };
	template<> struct canonical<ucsl::resources::swif::v6::Key<ucsl::resources::swif::v6::Color>> { using type = ucsl::resources::swif::v6::reflections::Key<ucsl::resources::swif::v6::Color>; };
	template<> struct canonical<ucsl::resources::swif::v6::KeyLinear<float>> { using type = ucsl::resources::swif::v6::reflections::KeyLinear<float>; };
	template<> struct canonical<ucsl::resources::swif::v6::KeyLinear<int>> { using type = ucsl::resources::swif::v6::reflections::KeyLinear<int>; };
	template<> struct canonical<ucsl::resources::swif::v6::KeyLinear<bool>> { using type = ucsl::resources::swif::v6::reflections::KeyLinear<bool>; };
	template<> struct canonical<ucsl::resources::swif::v6::KeyLinear<ucsl::resources::swif::v6::Color>> { using type = ucsl::resources::swif::v6::reflections::KeyLinear<ucsl::resources::swif::v6::Color>; };
	template<> struct canonical<ucsl::resources::swif::v6::KeyHermite<float>> { using type = ucsl::resources::swif::v6::reflections::KeyHermite<float>; };
	template<> struct canonical<ucsl::resources::swif::v6::KeyHermite<int>> { using type = ucsl::resources::swif::v6::reflections::KeyHermite<int>; };
	template<> struct canonical<ucsl::resources::swif::v6::KeyHermite<bool>> { using type = ucsl::resources::swif::v6::reflections::KeyHermite<bool>; };
	template<> struct canonical<ucsl::resources::swif::v6::KeyHermite<ucsl::resources::swif::v6::Color>> { using type = ucsl::resources::swif::v6::reflections::KeyHermite<ucsl::resources::swif::v6::Color>; };
	template<> struct canonical<ucsl::resources::swif::v6::KeyIndividual<float>> { using type = ucsl::resources::swif::v6::reflections::KeyIndividual<float>; };
	template<> struct canonical<ucsl::resources::swif::v6::KeyIndividual<int>> { using type = ucsl::resources::swif::v6::reflections::KeyIndividual<int>; };
	template<> struct canonical<ucsl::resources::swif::v6::KeyIndividual<bool>> { using type = ucsl::resources::swif::v6::reflections::KeyIndividual<bool>; };
	template<> struct canonical<ucsl::resources::swif::v6::KeyIndividual<ucsl::resources::swif::v6::Color>> { using type = ucsl::resources::swif::v6::reflections::KeyIndividual<ucsl::resources::swif::v6::Color>; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_KEYFRAME_PTR> { using type = ucsl::resources::swif::v6::reflections::SRS_KEYFRAME_PTR; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_TRACK> { using type = ucsl::resources::swif::v6::reflections::SRS_TRACK; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_MOTION> { using type = ucsl::resources::swif::v6::reflections::SRS_MOTION; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_ANIMATION> { using type = ucsl::resources::swif::v6::reflections::SRS_ANIMATION; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_CROP> { using type = ucsl::resources::swif::v6::reflections::SRS_CROP; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_CROPREF> { using type = ucsl::resources::swif::v6::reflections::SRS_CROPREF; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_DATA::Type> { using type = ucsl::resources::swif::v6::reflections::EDataType; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_DATA_VALUE_PTR> { using type = ucsl::resources::swif::v6::reflections::SRS_DATA_VALUE_PTR; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_DATA> { using type = ucsl::resources::swif::v6::reflections::SRS_DATA; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_USERDATA> { using type = ucsl::resources::swif::v6::reflections::SRS_USERDATA; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_TEXTURE> { using type = ucsl::resources::swif::v6::reflections::SRS_TEXTURE; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_TEXTURELIST> { using type = ucsl::resources::swif::v6::reflections::SRS_TEXTURELIST; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_CHARACTER_MAPPING> { using type = ucsl::resources::swif::v6::reflections::SRS_CHARACTER_MAPPING; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_FONT> { using type = ucsl::resources::swif::v6::reflections::SRS_FONT; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_TEXTDATA> { using type = ucsl::resources::swif::v6::reflections::SRS_TEXTDATA; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_EFFECT> { using type = ucsl::resources::swif::v6::reflections::SRS_EFFECT; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_BLUR> { using type = ucsl::resources::swif::v6::reflections::SRS_BLUR; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_BLUR3D> { using type = ucsl::resources::swif::v6::reflections::SRS_BLUR3D; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_REFLECT> { using type = ucsl::resources::swif::v6::reflections::SRS_REFLECT; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_REFLECT3D> { using type = ucsl::resources::swif::v6::reflections::SRS_REFLECT3D; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_TRS_BASE> { using type = ucsl::resources::swif::v6::reflections::SRS_TRS_BASE; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_TRS2D> { using type = ucsl::resources::swif::v6::reflections::SRS_TRS2D; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_TRS3D> { using type = ucsl::resources::swif::v6::reflections::SRS_TRS3D; };
	//template<> struct canonical<ucsl::resources::swif::v6::SRS_EFFECT_PTR> { using type = ucsl::resources::swif::v6::reflections::SRS_EFFECT_PTR; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_IMAGECAST> { using type = ucsl::resources::swif::v6::reflections::SRS_IMAGECAST; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_SLICE> { using type = ucsl::resources::swif::v6::reflections::SRS_SLICE; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_SLICECAST> { using type = ucsl::resources::swif::v6::reflections::SRS_SLICECAST; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_REFERENCECAST> { using type = ucsl::resources::swif::v6::reflections::SRS_REFERENCECAST; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_CAST_PTR> { using type = ucsl::resources::swif::v6::reflections::SRS_CAST_PTR; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_CASTNODE> { using type = ucsl::resources::swif::v6::reflections::SRS_CASTNODE; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_TRS_PTR> { using type = ucsl::resources::swif::v6::reflections::SRS_TRS_PTR; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_LAYER> { using type = ucsl::resources::swif::v6::reflections::SRS_LAYER; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_CAMERA> { using type = ucsl::resources::swif::v6::reflections::SRS_CAMERA; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_SCENE> { using type = ucsl::resources::swif::v6::reflections::SRS_SCENE; };
	template<> struct canonical<ucsl::resources::swif::v6::SRS_PROJECT> { using type = ucsl::resources::swif::v6::reflections::SRS_PROJECT; };
}