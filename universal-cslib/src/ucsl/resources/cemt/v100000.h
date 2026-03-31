#pragma once
#include <ucsl/bitset.h>
#include <ucsl/math.h>

namespace ucsl::resources::cemt::v100000 {
	struct EffectParam;

	struct JitteredValue {
		float value;
		float jitter;
	};

	struct JitteredVector3 {
		JitteredValue x;
		JitteredValue y;
		JitteredValue z;
	};

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
		enum class LoopType : unsigned char { // completely guessed
			NORMAL,
			BOUNCE,
		};

		bool useGlobalTime;
		char unk1;
		LoopType loopType;
		bool timeScale;
		int loopCount;
		short totalFrameCount;
		unsigned short trackCount;
		AnimationTrackParam* tracks[4];
	};

	struct ChildEffect {
		enum class Flag : unsigned char {
			ENABLE_USING_ANIMATION,
		};

		ucsl::bits::Bitset<Flag> flags;
		char unkType; // 0x01 = affected by fluctuation, 0x02 = related to color?
		float unk0;
		float unk1;
		float unk2;
		float unk3;
		float unk4;
		AnimationParam* enableAnimation; // single track, child effect is on if inside track frame bounds
		char name[128];
		EffectParam* param;
	};

	struct FieldParam {
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
		enum class TextureFlag : unsigned int {
			TEXTURE_TYPE_0,
			TEXTURE_TYPE_1,
			TEXTURE_TYPE_2,
			TEXTURE_TYPE_3,
			TEXTURE_TYPE_4,
			TEXTURE_TYPE_5,
			TEXTURE_TYPE_6,
			TEXTURE_TYPE_7,
			TEXTURE_TYPE_8,
			TEXTURE_TYPE_9,
		};

		enum class AnimationFlag : unsigned int {
			ANIMATED, //0x01
			UNK1, //0x02
			UNK2, //0x04
			STEPWISE_PATTERN_ANIMATION, //0x08
			ENABLE_SECONDARY_SCROLL, //0x10
			UNK5, //0x20
			RANDOMIZE_STARTING_FRAMES, //0x40
			ENABLE_PATTERN_ANIMATION, //0x80
			UNK8, //0x100
			SCROLL_U, //0x200
			SCROLL_V, //0x400
			UNK11, //0x800
			USE_MATRICES_ON_TEXTURE_TYPE_0, //0x1000 - tested at 0x140FF4598 in frontiers 1.42
			USE_MATRICES_ON_TEXTURE_TYPE_1, //0x2000
			SCALE_U, //0x4000
			SCALE_V //0x8000
		};

		enum class DirectionFlag : unsigned char {
			INVERT_X,
			INVERT_Y,
		};

		enum class DirectionRandomizationFlag : unsigned char {
			RANDOMIZE_X,
			RANDOMIZE_Y,
		};

		struct ScrollColorParam {
			ucsl::math::Vector2 scrollColor;
			ucsl::math::Vector2 scrollStartJitter;
		};

		struct UVAnimationParam {
			ucsl::math::Vector2 scale;
			float rotation;
			ucsl::math::Vector2 translation;
			int gap15;
			AnimationParam* tilingAnimation;
			AnimationParam* rotationAnimation;
			AnimationParam* translationAnimation;
		};

		struct PatternAnimationParam {
			enum class Type : unsigned char {
				UNK0,
				UNK1,
				UNK2,
				UNK3,
				UNK4,
				UNK5,
				UNK6,
				UNK7,
				UNK8,
				UNK9,
				UNK10,
			};

			unsigned char startingFrame;
			unsigned char stepCountX;
			unsigned char stepCountY;
			Type type;
			unsigned char frameRangeStart;
			unsigned char frameRangeEnd;
			unsigned char frameCount;
			unsigned char repeating;
			char unk6;
			char unk6a;
			ucsl::bits::Bitset<DirectionFlag> scrollDirectionFlags;
			ucsl::bits::Bitset<DirectionRandomizationFlag> scrollDirectionRandomizationFlags;
			unsigned char unk9;
			AnimationParam* patternAnimation;
		};

		char name[128];
		ucsl::bits::Bitset<TextureFlag> textureFlags;
		ucsl::bits::Bitset<AnimationFlag> animationFlags;
		PatternAnimationParam patternAnimationParam;
		ScrollColorParam scrollColorParams[2];
		UVAnimationParam uvAnimationParams[2];
		ucsl::math::Vector2 uvScale;
		int64_t unk22;
	};

	struct ColorRandomSet {
		JitteredValue color1[3][32];
		JitteredValue color2[3][32];
		JitteredValue hardness1[32];
		JitteredValue hardness2[32];
		JitteredValue alpha1[32];
		JitteredValue alpha2[32];
		unsigned char color1Count;
		unsigned char color2Count;
		unsigned char hardness1Count;
		unsigned char hardness2Count;
		unsigned char alpha1Count;
		unsigned char alpha2Count;
	};

	struct ColorParam {
		JitteredValue color1[3];
		JitteredValue alpha1;
		JitteredValue color2[3];
		JitteredValue alpha2;
		JitteredValue hardness1;
		JitteredValue hardness2;
		AnimationParam* colorAnimation1;
		AnimationParam* hardnessAnimation1;
		AnimationParam* alphaAnimation1;
		AnimationParam* colorAnimation2;
		AnimationParam* hardnessAnimation2;
		AnimationParam* alphaAnimation2;
		ColorRandomSet colorRandomSet;
		float unk7;
		AnimationParam* unk7Animation;
	};

	struct LODEffectParam {
		char name[128];
		float threshold;
	};

	struct BasicLODParam {
		enum class Flag : unsigned char {
			USE_NEGATIVE_ONE_UNK1864,
		};

		unsigned char lodCount;
		ucsl::bits::Bitset<Flag> flags;
		LODEffectParam lods[16];
	};

	struct AnimatedLODParam {
		uint32_t gap9;
		math::Position unk20;
		math::Position unk21;
		AnimationParam* unk20Animation;
		AnimationParam* unk21Animation;
		uint32_t gap9a;
		math::Position unk22;
		math::Position unk23;
		AnimationParam* unk22Animation;
		AnimationParam* unk23Animation;
		uint64_t gap10;
		unsigned char lodCount;
		char gap11[7];
		LODEffectParam lods[16];
	};

	union LODParam {
		BasicLODParam basic;
		AnimatedLODParam animated;
	};

	struct UserParameter {
		enum class Type : unsigned int {
			UNK0,
			BOOL,
			INT,
			FLOAT,
			STRING,
			VECTOR,
		};

		union DataPtr {
			void* unk0;
			bool* boolean;
			int* integer;
			float* floating;
			ucsl::math::Position* vector;
		};

		Type type;
		unsigned int size;
		DataPtr data;
	};

	struct ElementParam {
		enum class LODFlag : unsigned int {
			BASIC_CHILD_EFFECTS,
			ANIMATED_CHILD_EFFECTS,
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
			unsigned char unk5;
			unsigned char unk6;
			bool unk2;
			char pad2[0x10];
		};

		struct HistoricalStripeParticleParam {
			unsigned int type;
			char pad[4];
			unsigned char unk1;
			char pad1[0x4];
			unsigned char unk3;
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
			UNK1,
			UNK2,
		};

		struct TransformParam {
			enum class RandomAngleFlag : unsigned char {
				CCW_90,
				CW_90,
				CW_180,
				CW_0
			};

			JitteredVector3 rotation;
			unsigned int rotationFlags;
			JitteredVector3 randomizedDirectionRotation;
			unsigned int randomizedDirectionRotationFlags;
			ucsl::bits::Bitset<RandomAngleFlag> rotationRandomInitialAngleFlags[3];
			JitteredVector3 angularVelocity;
			unsigned int angularVelocityFlags;
			JitteredVector3 size;
			unsigned int sizeFlags;
			JitteredVector3 scale;
			unsigned int scaleFlags;
		};

		struct UserParameterParam {
			unsigned int userParameterCount;
			UserParameter* userParameters;
		};

		enum class UpdateFlag : unsigned int {
			HAS_CHILDREN, // 0x1
			HAS_RANDOMIZED_ROTATION_DIRECTION, // 0x2
			HAS_FIXED_ANGULAR_VELOCITY, // 0x4
			USE_GLOBAL_TIME, // 0x8
			HAS_MODEL, // 0x10
			UNK5, // 0x20
			UNK6, // 0x40
			UNK7, // 0x80
			UNK8, // 0x100
			UNK9, // 0x200
			UNK10, // 0x400
			DISABLE_ANIMATIONS, // 0x800
			UNK12, // 0x1000
			DISABLE_FIELDS, // 0x2000
		};

		TransformParam transformParam;
		ColorParam colorParams[2];
		JitteredValue lifetime;
		AnimationParam* rotationAnimation;
		AnimationParam* scaleAnimation;
		AnimationParam* sizeAnimation;
		char gap6aa[0xC];
		bool hasModel;
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
		FieldParam fields[8];
		AnimationParam* fieldAnimations[8][5];
		ucsl::bits::Bitset<UpdateFlag> updateFlags; // 0x01 = has childeffects, 0x4 = use simple unkVec2 update (only x multiplier, no anim), 0x8 = use emitter global time for unkVec2 update, 0x1000 = use -1 or fps variables
		unsigned int gap7b;
		AnimationParam* uvAnimation;
		unsigned int elementAnimationCount; // 140FF3B3D
		char gap7bb[0x34];
		ucsl::bits::Bitset<GpuParticleFlag> gpuParticleFlags; // 0x1 = is gpu rendering?
		char vectorFieldName[128];
		math::Position vectorFieldSize;
		char gap7c2[0x30];
		math::Position unkVector2;
		math::Position vectorFieldParam;
		math::Position unkVector1;
		math::Position depthCollision;
		UserParameterParam userParameterParam;
		unsigned int emitterAnimationCount; // 140FE9F5B
		unsigned int unk7e;
		void* gpuElementParamTexture;
		void* gpuOtherTexture;
		ucsl::bits::Bitset<LODFlag> lodFlags;
		LODParam lodParam;
	};

	struct EmitterParam {
		struct TransformParam {
			enum class TransformType : unsigned int {
				UNK0,
				UNK1,
				ROTATED,
			};

			ucsl::math::Position position;
			ucsl::math::Position rotation;
			ucsl::math::Position scale;
			TransformType type;
			AnimationParam* positionAnimation;
			AnimationParam* rotationAnimation;
			AnimationParam* scaleAnimation;
		};

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
				FIXED
			};

			Shape shape;
			ucsl::math::Position positionJitter;
			float spread;
			float startAngle;
			float endAngle;
			bool useRadialDistribution;
			bool consistentAngle;
			bool useSubdivisions;
			unsigned char subdivisionCount;
			AnimationParam* positionJitterAnimation;
			AnimationParam* spreadAnimation;
			AnimationParam* startAngleAnimation;
			AnimationParam* endAngleAnimation;
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
			JitteredValue emitInterval;
			JitteredValue emissionCount;
			AttenuationMode attenuationMode;
			float minDistance;
			float maxDistance;
			float distanceAttenuation; // 0->1
			float duration;
			float startDelay;
			float fadeSpeed;
			AnimationParam* emissionCountAnimation;
		};

		struct DeathParam {
			enum class DeathMode {
				WAIT_FOR_ELEMENTS,
				KILL_AFTER_DELAY,
				WAIT_FOR_ELEMENTS_OR_KILL_AFTER_DELAY,
				IMMEDIATE,
			};

			DeathMode mode;
			float killDelay;
		};

		struct EmissionDynamicsParam {
			struct Settings {
				enum class SpreadDirectionType : unsigned int {
					DIRECTION,
					ANGLES,
				};

				float direction1Velocity;
				float direction2Velocity;
				float randomDirectionVelocity;
				float localSpreadVelocity;
				float localSpread;
				float worldSpreadVelocity;
				float worldSpread;
				SpreadDirectionType worldSpreadDirectionType;
				csl::math::Position worldSpreadAnglesOrDirection;
				float initialVelocityJitter;
				float velocityJitter;
			};

			Settings settings;
			float gap4d;
			AnimationParam* direction1VelocityAnimation;
			AnimationParam* direction2VelocityAnimation;
			AnimationParam* localSpreadVelocityAnimation;
			AnimationParam* localSpreadAnimation;
			AnimationParam* worldSpreadVelocityAnimation;
			AnimationParam* worldSpreadAnimation;
			AnimationParam* worldSpreadAnglesOrDirectionAnimation;
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
			USE_UNKFLOAT0_RANGE,
			USE_UNKFLOAT1_RANGE,
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

		TransformParam transformParam;
		EmissionParam emissionParam;
		EmitParam emitParam;
		DeathParam deathParam;
		EmissionDynamicsParam emissionDynamicsParam;
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
		float unkFloat0RangeStart;
		float unkFloat0RangeEnd;
		float unkFloat1RangeStart;
		float unkFloat1RangeEnd;
		float unkFloat2RangeStart;
		float unkFloat2RangeEnd;
		float unkFloat6RangeEnd; // 140FE4F74
		float unkFloat6RangeStart;
		unsigned int gap6b;
		unsigned int unkInt0;
		unsigned int unkInt1;
		unsigned int unkInt2;
		unsigned int randomSeed;
		char renderLayer;
		unsigned char unk6b2; // 140FE0540
		unsigned char unk6b3; // 140FE4E6C
		unsigned char unk6b4; // 140FE4F0A
		unsigned char unkType5;
		BillboardType billboardType; // handled in 0140FE9390
		bool enableUnkFloat2Range;
		char gap6b2;
		unsigned char unkChar0;
		char gap6b3[3];
		uint32_t unkRandomSeed;
		unsigned int gap6aa1[3];
		ucsl::math::Position unkFloat2342;
		unsigned int gap6aa2;
		MaskType1 unkType0;
		unsigned int unkType0a;
		unsigned int gap6aa1a;
		MaskType2 unkType0c;
		unsigned int unkType0d;
		MaskType3 unkType1; //has six values;
		MaskType1 unkType1a;
		unsigned int unkType1b;
		MaskType3 unkType2; //has six values;
		MaskType2 unkType3;
		unsigned int unkType4;
		unsigned int gap6aa1bc;
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
		unsigned int emitterCount;

		EmitterParam emitterParam;

	};
}
