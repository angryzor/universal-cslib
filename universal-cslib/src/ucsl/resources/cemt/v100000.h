#pragma once

namespace ucsl::resources::cemt::v100000 {
	template<typename T>
	struct PtrData {
		T* ptr;
	};

	struct JitteredValue {
		float value;
		float jitter;
	};

	struct Unk2Data {};

	struct Unk3Data {};

	struct Unk4Data {};

	struct Unk5Data {};

	struct Unk6Data {};

	struct Unk7Data {};

	struct Unk8Data {};

	struct Unk9Data {};

	struct Unk10Data {};

	struct Unk11Data {};

	struct Unk12Data {};

	struct Unk13Data {};

	struct Unk14Data {};

	struct Unk15Data {};

	struct Unk16Data {};

	struct Unk17Data {};

	struct AnimationKeyframeParam {
		enum class InterpolationType : unsigned short {
			CONSTANT,
			LINEAR,
			HERMITE,
		};

		unsigned short frame;
		InterpolationType interpolationType;
		float derivativeIn;
		float derivativeOut;
		JitteredValue value;
	};

	struct AnimationTrackParam {
		enum class RandomnessFlag : unsigned char {
			DISABLE_RANDOMNESS,
		};

		unsigned char index;
		ucsl::bits::Bitset<RandomnessFlag> randomnessFlag;
		short keyframeCount;
		AnimationKeyframeParam keyframes[2];
	};

	struct AnimationParam {
		char flags;
		char unk1;
		char unk2;
		bool durationEnabled;
		int loopCount;
		short totalFrameCount;
		unsigned short trackCount;
		AnimationTrackParam* tracks[4];
	};

	struct ChildEffect {
		uint8_t flags; // 0x01 = animate
		char unkType; // 0x01 = affected by fluctuation, 0x02 = related to color?
		float unk0;
		float unk1;
		float unk2;
		float unk3;
		float unk4;
		PtrData<AnimationParam> unkAnim;
		char name[128];
		void* param; // EffectParam
	};

	struct ChildEffect2 {
		unsigned int unk1;
		char name[128];
	};

	struct ModifierParam {
		enum class Type : unsigned char {
			GRAVITY,
			SPEED,
			MAGNET,
			NEWTON,
			VORTEX,
			SPIN,
			SPIN2,
			RANDOM,
			TAIL,
			FLUCTUATION,
			UNK0
		};

		enum class Origin : unsigned char {
			WORLD,
			EMISSION,
			INHERIT,
			UNK0 // uses option2
		};

		struct GravitySettings {
			float scale;
			csl::math::Position rotation;
			bool usesDirection;
		};

		struct SpeedSettings {
			float scale;
		};

		struct MagnetSettings {
			csl::math::Position magnetPoint;
			float scale;
		};

		struct NewtonSettings {
			csl::math::Position targetPoint;
			float scale;
			float maxDistance;
		};

		struct VortexSettings {
			float max;
			float min;
			float radius;
			csl::math::Position rotation;
			bool usesDirection;
		};

		struct SpinSettings {
			float scale;
			csl::math::Position rotation;
			bool usesDirection;
		};

		struct Spin2Settings {
			float baseStrength;
			float spinStrength;
			float rotationAngle;
			float axisFalloff;
			csl::math::Position axisVector;
			bool useEulerRotation;
		};

		struct RandomSettings {
			enum class Flags : unsigned short {
				RANDOMIZED_SCALE,
				X,
				Y,
				Z
			};

			float scale;
			float spreadScale;
			short updateInterval;
			ucsl::bits::Bitset<Flags> flags;
			bool normalizedSpreadVector;
			bool randomPerAxis;
		};

		struct TailSettings {
			float multiplier;
		};

		struct FluctuationSettings {
			enum class WaveformType : unsigned char {
				TRIANGLE,
				INVERSE_TRIANGLE,
				SAW,
				STEP,
				SINE
			};

			WaveformType waveformType;
			float baseOffset;
			int axisFlags;
		};

		struct UnkSettings {
			enum class UnkFlag : unsigned char {
				UNK0,
				UNK1,
				UNK2,
			};

			float unk1;
			float unk2;
			ucsl::bits::Bitset<UnkFlag> flags;
		};

		union Settings {
			GravitySettings gravity;
			SpeedSettings speed;
			MagnetSettings magnet;
			NewtonSettings newton;
			VortexSettings vortex;
			SpinSettings spin;
			Spin2Settings spin2;
			RandomSettings random;
			TailSettings tail;
			FluctuationSettings fluctuation;
			UnkSettings unk;
			unsigned int maxSize[8];
		};

		bool enabled;
		Type type;
		Origin origin;
		char option2;
		Settings settings;
	};

	struct TextureParam {
		enum class UVFlags {
			ANIMATED, //0x01
			UNK1, //0x02
			UNK2, //0x04
			UNK3, //0x08
			ENABLE_SECONDARY_SCROLL, //0x10
			UNK5, //0x20
			USE_RANDOM_UNK0, //0x40
			UNK7, //0x80
			UNK8, //0x100
			SCROLL_U, //0x200
			SCROLL_V, //0x400
			UNK11, //0x800
			UNK12, //0x1000 - tested at 0x140FF4598 in frontiers 1.42
			UNK13, //0x2000
			SCALE_U, //0x4000
			SCALE_V //0x8000
		};

		struct ScrollColorParam {
			float scrollColorX;
			float scrollColorY;
			float scrollStartJitterX;
			float scrollStartJitterY;
		};

		struct ScrollUVParam {
			float tilingX;
			float tilingY;
			float rotation;
			float offsetX;
			float offsetY;
			int unk15;
			PtrData<AnimationParam> tilingAnimation;
			PtrData<AnimationParam> rotationAnimation;
			PtrData<AnimationParam> offsetAnimation;
		};

		enum class ScrollDirectionFlag : unsigned char {
			INVERT_X,
			INVERT_Y,
		};

		enum class ScrollDirectionRandomizationFlag : unsigned char {
			RANDOMIZE_X,
			RANDOMIZE_Y,
		};

		char name[128];
		int flags0;
		csl::ut::Bitset<UVFlags> flags1;
		unsigned char unk0; // unk452434 on the live element
		unsigned char patternAnimXSteps;
		unsigned char patternAnimYSteps;
		unsigned char unk3;
		unsigned char unk0RandomRangeStart;
		unsigned char unk0RandomRangeEnd;
		unsigned char unk5;
		unsigned char unk5a;
		char unk6;
		char unk6a;
		ucsl::bits::Bitset<ScrollDirectionFlag> scrollDirectionFlags;
		ucsl::bits::Bitset<ScrollDirectionRandomizationFlag> scrollDirectionRandomizationFlags;
		unsigned char unk9;
		PtrData<AnimationParam> patternAnim;
		ScrollColorParam scrollColorParams[2];
		ScrollUVParam uvParams[2];
		float uvScale[2];
		int64_t unk22;
	};

	struct ColorRandomSet {
		JitteredValue color1[3][32];
		JitteredValue color2[3][32];
		JitteredValue unk1[32];
		JitteredValue unk2[32];
		JitteredValue alpha1[32];
		JitteredValue alpha2[32];
		unsigned char color1Count;
		unsigned char color2Count;
		unsigned char unk1Count;
		unsigned char unk2Count;
		unsigned char alpha1Count;
		unsigned char alpha2Count;
	};

	struct ColorParam {
		JitteredValue color1[3];
		JitteredValue alpha1;
		JitteredValue color2[3];
		JitteredValue alpha2;
		JitteredValue unk6_1;
		JitteredValue unk6_2;
		PtrData<AnimationParam> colorAnimation1;
		PtrData<AnimationParam> unk6Animation1;
		PtrData<AnimationParam> alphaAnimation1;
		PtrData<AnimationParam> colorAnimation2;
		PtrData<AnimationParam> unk6Animation2;
		PtrData<AnimationParam> alphaAnimation2;
		ColorRandomSet colorRandomSet;
		float unk7;
		PtrData<AnimationParam> unk7Animation;
	};

	struct ElementParam {
		enum class UnkChildEffectsFlag : unsigned int {
			BASIC_CHILD_EFFECTS,
			ANIMATED_CHILD_EFFECTS,
		};

		struct BasicUnkChildEffectsParam {
			enum class Flag : unsigned char {
				USE_NEGATIVE_ONE_UNK1864,
			};

			unsigned char childEffectCount;
			ucsl::bits::Bitset<Flag> flags;
			ChildEffect2 childEffects[16];
		};

		struct AnimatedChildEffectsParam {
			uint32_t gap9;
			math::Position unk20;
			math::Position unk21;
			PtrData<AnimationParam> unk20Animation;
			PtrData<AnimationParam> unk21Animation;
			uint32_t gap9a;
			math::Position unk22;
			math::Position unk23;
			PtrData<AnimationParam> unk22Animation;
			PtrData<AnimationParam> unk23Animation;
			uint64_t gap10;
			unsigned char childEffectCount;
			ChildEffect2 childEffects[16];
		};

		union UnkChildEffectsParam {
			BasicUnkChildEffectsParam basic;
			AnimatedChildEffectsParam animated;
		};

		enum class ParticleType : unsigned char {
			UNK0,
			UNK1,
			UNK2,
			UNK3,
			UNK4,
			UNK5_CPU,
			HISTORICAL_STRIPE,
			POINT_LIGHT,
		};

		struct Unk0ParticleParam {
			unsigned int unk0;
			float unk1;
		};

		struct Unk1ParticleParam {
			unsigned int unk0;
			float unk1;
		};

		struct Unk2ParticleParam {
			unsigned int type;
			char pad[4];
			unsigned int unk0;
			float unk1;
		};

		struct Unk3ParticleParam {
			unsigned int type;
			bool unk2;
			char pad[3];
			unsigned int unk0;
			float unk1;
		};

		struct Unk4ParticleParam {
			unsigned int type;
			char pad[5];
			unsigned char unk2;
			unsigned char gap3;
			unsigned int unk0;
			float unk1;
		};

		struct Unk5ParticleParam {
			unsigned int type;
			unsigned char unk3;
			char pad[3];
			unsigned int unk1;
			unsigned char unk4;
			char pad1[0x2];
			bool unk2;
			char pad2[0x10];
		};

		struct HistoricalStripeParticleParam {
			unsigned int type;
			char pad[4];
			unsigned char unk1;
			char pad1[0x5];
			bool unk2;
			char pad2[0x11];
		};

		struct PointLightParticleParam {
			char pad[0x20];
		};

		union ParticleParam {
			Unk0ParticleParam unk0;
			Unk1ParticleParam unk1;
			Unk2ParticleParam unk2;
			Unk3ParticleParam unk3;
			Unk4ParticleParam unk4;
			Unk5ParticleParam unk5;
			HistoricalStripeParticleParam historicalStripe;
			PointLightParticleParam pointLight;
		};

		enum class GpuParticleFlag : unsigned int {
			IS_GPU,
		};

		float initialRotation[6];
		unsigned int unk18a0;
		char gap6aa2[24];
		unsigned int unkVec2Flags;
		unsigned char unkVec2OffsetXRandomFlags;
		unsigned char unkVec2OffsetYRandomFlags;
		unsigned char unkVec2OffsetZRandomFlags;
		float angularVelocity[6];
		unsigned int unk18a01;
		JitteredValue sizeX;
		JitteredValue sizeY;
		JitteredValue sizeZ;
		unsigned int sizeFlags; //controls whetever jiggle should be used
		JitteredValue scaleX;
		JitteredValue scaleY;
		JitteredValue scaleZ;
		unsigned int scaleFlags; //controls whetever jiggle should be used
		ColorParam colorParams[2];
		JitteredValue fps;
		PtrData<AnimationParam> unkVec2Anim;
		PtrData<AnimationParam> scaleAnimation;
		PtrData<AnimationParam> sizeAnimation;
		char gap6aa[0xC];
		bool useModel;
		unsigned char modelSetting1;
		char gap6b[128];
		char modelName[128];
		char skeletonName[128];
		char nodeAnimName[128];
		ParticleType particleType;
		unsigned char particleType2; // 140FE7E43
		ParticleParam particleParam;
		TextureParam textures[4];
		unsigned int textureCount;
		ChildEffect childEffects[16];
		ModifierParam modifiers[8];
		PtrData<AnimationParam> modifierAnimations[5][8];
		unsigned int fieldFlags; // 0x01 = has childeffects, 0x4 = use simple unkVec2 update (only x multiplier, no anim), 0x8 = use emitter global time for unkVec2 update, 0x1000 = use -1 or fps variables
		char gap7b[0x4];
		PtrData<AnimationParam> unkAnim7bc;
		unsigned int someAnimationCount; // 140FF3B3D
		char gap7bb[0x34];
		ucsl::bits::Bitset<GpuParticleFlag> gpuParticleFlags; // 0x1 = is gpu rendering?
		char vectorFieldName[128];
		math::Position vectorFieldSize;
		char gap7c2[0x3C];
		math::Position vectorFieldParam;
		char gap7c1[0xC];
		math::Position depthCollision;
		char gap7c[0x10];
		unsigned int unk7d; // 140FE9F5B
		unsigned int unk7e;
		void* gpuElementParamTexture;
		void* gpuOtherTexture;
		ucsl::bits::Bitset<UnkChildEffectsFlag> unkChildEffectFlags;
		UnkChildEffectsParam childEffectsParam;
		//unsigned char unkCount;
		//unsigned char childElementAnimationFlags;

		//math::Position unk20;
		//math::Position unk21;
		//PtrData<AnimationParam> unk20Animation;
		//PtrData<AnimationParam> unk21Animation;
		//uint32_t gap9a;
		//math::Position unk22;
		//math::Position unk23;
		//PtrData<AnimationParam> unk22Animation;
		//PtrData<AnimationParam> unk23Animation;
		//
		//uint64_t gap10;
		//unsigned char childEffect2Count;
		//ChildEffect2 childEffect2s[16];
	};

	struct EmitterParam {
		struct EmissionParam {
			enum class Shape : unsigned int {
				POINT,
				SPHERE,
				DISC,
				CYLINDER,
				LINE,
				TORUS,
				CUBE,
				MESH,
				FIXED = 8
			};

			Shape shape;
			csl::math::Position randomTransform;
			float spread;
			float startAngle;
			float endAngle;
			bool useRadialDistribution;
			bool consistentAngle;
			bool useAngularSubdivisions;
			unsigned char emissionParticleCount;
			PtrData<AnimationParam> randomPositionAnimation;
			PtrData<AnimationParam> spreadAnimation;
			PtrData<AnimationParam> startAngleAnimation;
			PtrData<AnimationParam> endAngleAnimation;
		};

		struct EmitParam {
			enum class EmitMode : unsigned int {
				UNK0,
				UNK1,
			};

			enum class AttenuationMode : unsigned char {
				FIXED,
				POSE_POSITION,
			};

			EmitMode emissionMode;
			JitteredValue frequency;
			JitteredValue emissionCount;
			AttenuationMode attenuationMode;
			float minDistance;
			float maxDistance;
			float distanceAttenuation; // 0->1
			float duration;
			float startDelay;
			float fadeSpeed;
			PtrData<AnimationParam> emissionCountAnimation;
		};

		enum class InheritFlag : unsigned int {
			TRANSLATION_RAW, // ignore world scale
			ROTATION, // parent rotation
			TRANSLATION, // add parent translation scaled by inheritRatio
			TRANSLATION_RELATIVE, // apply relative translation
			INHERIT_DIRECT // sets the matrix to parents
		};

		enum class UnkFlags : unsigned int {
			UNK0, // sets Emitter::flags to |= 0x20
			UNK1,
			UNK2,
			UNK3,
			USE_TEXTURES,
			USE_RENDER_PRIORITY,
			UNK6,
			UNK7,
			UNK8,
			UNK9,
			UNK10,
			UNK11, // sets emitter flag 0x40
		};

		enum class CullMode : unsigned int {
			NONE,
			FRONT,
			BACK,
		};

		enum class MaskType1 : unsigned int {
			UNK0,
			UNK1,
			UNK2,
			UNK3,
		};

		enum class MaskType2 : unsigned int {
			UNK0,
			UNK1,
			UNK2,
			UNK3,
			UNK4,
		};

		enum class MaskType3 : unsigned int {
			UNK0,
			UNK1,
			UNK2,
		};

		enum class DepthMode : unsigned int {
			BLENDING_ENABLED_WITH_DEPTH_TEST,
			BLENDING_ENABLED,
			BLENDING_DISABLED_WITH_DEPTH_TEST_AND_DEPTH_WRITE,
			BLENDING_DISABLED_WITH_DEPTH_TEST_AND_DEPTH_WRITE_2,
			BLENDING_ENABLED_WITH_DEPTH_TEST_AND_DEPTH_WRITE,
		};

		enum class BlendMode : unsigned int {
			MIX,
			ADD,
			MULTIPLY,
			SUBTRACT,
		};

		enum class BillboardType : unsigned char {
			UNK0,
			UNK1,
			UNK2,
		};

		csl::math::Position position;
		csl::math::Position rotation;
		csl::math::Position scale;
		unsigned int cameraFlags; //changes how the emitter faces the camera (billboard style, using the values etc)
		PtrData<AnimationParam> unk1Animation;
		PtrData<AnimationParam> unk2Animation;
		PtrData<AnimationParam> unk3Animation;
		EmissionParam emissionParam;
		EmitParam emitParam;
		unsigned int unkType;
		float lifeEndTime;
		float accelerationMultiplier;
		float accelerationNormalMultiplier;
		float initialSpeed;
		float velocityMultiplier;
		float shapeRadius;
		float velocityScale;
		float emitSize;
		bool useEmitVector;
		csl::math::Position emitVector;
		float emitVectorJitter;
		float directionJitter;
		float unk4d;
		PtrData<AnimationParam> accelerationMultiplierAnimation;
		PtrData<AnimationParam> accelerationNormalMultiplierAnimation;
		PtrData<AnimationParam> velocityMultiplierAnimation;
		PtrData<AnimationParam> shapeRadiusAnimation;
		PtrData<AnimationParam> velocityScaleAnimation;
		PtrData<AnimationParam> emitSizeAnimation;
		PtrData<AnimationParam> emitVectorAnimation;
		BlendMode blendMode;
		DepthMode depthMode;
		CullMode cullMode;
		ucsl::bits::Bitset<InheritFlag> inheritTransformFlags;
		float inheritTransformRatio;
		ucsl::bits::Bitset<InheritFlag> inheritPoseFlags;
		float inheritPoseRatio;
		ucsl::bits::Bitset<UnkFlags> unkFlags;
		float unkFloat4;
		float unkFloat5;
		float unkFloat0;
		float unkFloat0Jitter;
		float unkFloat1;
		float unkFloat1Jitter;
		float unkFloat2;
		float unkFloat3;
		float unkFloat6RangeEnd; // 140FE4F74
		float unkFloat6RangeStart;
		char gap6b[0x4];
		unsigned int unkInt0;
		unsigned int unkInt1;
		unsigned int unkInt2;
		unsigned int randomSeed;
		char renderLayer;
		unsigned char unk6b2;
		unsigned char unk6b3; // 140FE4E6C
		unsigned char unk6b4; // 140FE4F0A
		unsigned char unkType5;
		BillboardType billboardType; // handled in 0140FE9390
		bool unkBool0;
		char gap6b2;
		unsigned char unkChar0;
		char gap6b3[3];
		uint32_t unkRandomSeed;
		char gap6aa1[28];
		MaskType1 unkType0;
		unsigned int unkType0a;
		char gap6aa1a[4];
		MaskType2 unkType0c;
		unsigned int unkType0d;
		MaskType3 unkType1; //has six values;
		MaskType1 unkType1a;
		unsigned int unkType1b;
		MaskType3 unkType2; //has six values;
		MaskType2 unkType3;
		unsigned int unkType4;
		char gap6aa1bc[4];
		ElementParam elementParam;
	};

	struct EffectParam {
		char magic[3];
		bool resolved;
		unsigned char majorVersion;
		unsigned char minorVersion;
		unsigned char patchVersion;
		unsigned char buildVersion;
		char name[0x80];
		unsigned int animationBufferSize;
		unsigned int numEffects; // 8C

		EmitterParam emitterParam;

	};
}
