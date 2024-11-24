#pragma once
#include <ucsl/math.h>

namespace ucsl::resources::swif::v6 {
    // Basic types
    using Vector2 = math::Vector2;
    using Vector3 = math::Vector3;
    using Matrix34 = math::Matrix34;
    using Matrix44 = math::Matrix44;
    struct Rotation3 { int x; int y; int z; };

    struct Color {
        unsigned char r{};
        unsigned char g{};
        unsigned char b{};
        unsigned char a{};
    };

    struct Colorf {
        float a{};
        float r{};
        float g{};
        float b{};
    };

    enum class EPivotType {
        TOP_LEFT,
        TOP_CENTER,
        TOP_RIGHT,
        CENTER_LEFT,
        CENTER_CENTER,
        CENTER_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_CENTER,
        BOTTOM_RIGHT,
        CUSTOM,
    };

    enum class EOrientation {
        UP,
        LEFT,
        DOWN,
        RIGHT,
    };

    // Animations
    enum class ECurveType : unsigned short {
        TranslationX,
        TranslationY,
        TranslationZ,
        RotationX,
        RotationY,
        RotationZ,
        ScaleX,
        ScaleY,
        ScaleZ,
        MaterialColor,
        Display,
        Width,
        Height,
        VertexColorTopLeft,
        VertexColorTopRight,
        VertexColorBottomLeft,
        VertexColorBottomRight,
        CropIndex0,
        CropIndex1,
        Unknown,
        IlluminationColor,
        MaterialColorR,
        MaterialColorG,
        MaterialColorB,
        MaterialColorA,
        VertexColorTopLeftR,
        VertexColorTopLeftG,
        VertexColorTopLeftB,
        VertexColorTopLeftA,
        VertexColorTopRightR,
        VertexColorTopRightG,
        VertexColorTopRightB,
        VertexColorTopRightA,
        VertexColorBottomLeftR,
        VertexColorBottomLeftG,
        VertexColorBottomLeftB,
        VertexColorBottomLeftA,
        VertexColorBottomRightR,
        VertexColorBottomRightG,
        VertexColorBottomRightB,
        VertexColorBottomRightA,
        IlluminationColorR,
        IlluminationColorG,
        IlluminationColorB,
        IlluminationColorA,
    };

    enum class EInterpolationType : unsigned short {
        CONSTANT,
        LINEAR,
        HERMITE,
        INDIVIDUAL,
    };

    enum class ETrackDataType : unsigned short {
        UNKNOWN,
        FLOAT,
        INDEX,
        BOOL,
        INT,
        COLOR,

        // These are uncertain
        UNK_FLOAT,
        UNK_DOUBLE, // Types say double, but used for what? Won't interpolate linearly.
        UNK_CHAR, // Char? doubt
    };

    struct SRS_KEYFRAME {
        unsigned int frame{};
    };

    template<typename T>
    struct Key : public SRS_KEYFRAME {
        T value{};
    };

    template<typename T>
    struct KeyLinear : public Key<T> {
    };

    template<typename T>
    struct KeyHermite : public KeyLinear<T> {
        T derivativeIn{};
        T derivativeOut{};
    };

    template<typename T>
    struct KeyIndividual : public KeyHermite<T> {
        EInterpolationType interpolationType{};
    };

    union SRS_KEYFRAME_PTR {
        Key<float>* constantFloat;
        Key<int>* constantIndex;
        Key<int>* constantInt;
        Key<bool>* constantBool;
        Key<Color>* constantColor;

        KeyLinear<float>* linearFloat;
        KeyLinear<int>* linearIndex;
        KeyLinear<int>* linearInt;
        KeyLinear<bool>* linearBool;
        KeyLinear<Color>* linearColor;

        KeyHermite<float>* hermiteFloat;
        KeyHermite<int>* hermiteIndex;
        KeyHermite<int>* hermiteInt;
        KeyHermite<bool>* hermiteBool;
        KeyHermite<Color>* hermiteColor;

        KeyIndividual<float>* individualFloat;
        KeyIndividual<int>* individualIndex;
        KeyIndividual<int>* individualInt;
        KeyIndividual<bool>* individualBool;
        KeyIndividual<Color>* individualColor;
    };

    struct SRS_TRACK {
        ECurveType trackType{};
        unsigned short keyCount{};
        unsigned short flags{};
        unsigned int firstFrame{};
        unsigned int lastFrame{};
        SRS_KEYFRAME* keyFrames{};

        inline EInterpolationType GetInterpolationType() const {
            return static_cast<EInterpolationType>(flags & 0x3);
        }

        inline ETrackDataType GetDataType() const {
            return static_cast<ETrackDataType>((flags >> 4) & 0xF);
        }
    };

    struct SRS_MOTION {
        unsigned short castId{};
        unsigned short trackCount{};
        SRS_TRACK* tracks{};
    };

    struct SRS_ANIMATION {
        const char* name{};
        unsigned int id{};
        unsigned int motionCount{};
        unsigned int frameCount{};
        SRS_MOTION* motions{};
        void* userData{};
        bool isLooping{};
    };

    // Crops
    struct SRS_CROP {
        float left{};
        float top{};
        float right{};
        float bottom{};
    };

    struct SRS_CROPREF {
        short textureListIndex{};
        short textureIndex{};
        short cropIndex{};
    };

    // User data
    union SRS_DATA_VALUE_PTR {
        bool* b;
        int* i32;
        unsigned int* u32;
        float* f32;
        const char* str;
    };

    struct SRS_DATA {
        enum class Type {
            BOOL,
            INT,
            UINT,
            FLOAT,
            UNK,
            STRING,
        };

        const char* name{};
        Type type{};
        SRS_DATA_VALUE_PTR value{};
    };

    struct SRS_USERDATA {
        unsigned int count{};
        SRS_DATA* items{};
    };

    // Textures
    struct SRS_TEXTURE {
        const char* name{};
        const char* filename{};
        unsigned int id{};
        unsigned short width{};
        unsigned short height{};
        unsigned int flags{};
        unsigned int cropCount{};
        SRS_CROP* crops{};
        SRS_USERDATA* userData{};
    };

    struct SRS_TEXTURELIST {
        const char* name{};
        unsigned int unk1{};
        unsigned int textureCount{};
        SRS_TEXTURE* textures{};
        SRS_USERDATA* userData{};
    };

    // Fonts
    struct SRS_CHARACTER_MAPPING {
        unsigned short character{};
        SRS_CROPREF cropRef{};
    };

    struct SRS_FONT {
        const char* name{};
        int id{};
        unsigned int unk1{};
        short unk2{};
        unsigned short characterCount{};
        unsigned short unk3{};
        SRS_CHARACTER_MAPPING* characters{};
        unsigned long long unk4{};
        SRS_USERDATA* userData{};
    };

    // Text
    struct SRS_TEXTDATA {
        enum class VerticalAlignment {
            TOP,
            CENTER,
            BOTTOM,
        };

        unsigned int flags{};
        unsigned int fontIndex{};
        const char* text{}; // UTF-8 encoded
        Vector2 scale{};
        short paddingLeft{};
        short paddingRight{};
        short paddingTop{};
        short paddingBottom{};
        short tracking{};
        short lineHeight{};
        SRS_FONT* font{};

        inline bool GetWrapFlag() const {
            return flags & 0x1;
        }

        inline void SetWrapFlag(bool state) {
            unsigned int mask = 0x1;
            if (state)
                flags |= mask;
            else
                flags &= ~mask;
        }

        inline bool GetDisableLineHeightFlag() const {
            return flags & 0x2;
        }

        inline void SetDisableLineHeightFlag(bool state) {
            unsigned int mask = 0x2;
            if (state)
                flags |= mask;
            else
                flags &= ~mask;
        }

        inline VerticalAlignment GetVerticalAlignment() const {
            return static_cast<VerticalAlignment>((flags >> 4) & 0xF);
        }

        inline void SetVerticalAlignment(VerticalAlignment alignment) {
            flags = (flags & ~0xF0) | ((static_cast<unsigned int>(alignment) & 0xF) << 4);
        }
    };

    // Effects
    enum EEffectType : unsigned int {
        NONE = 0,
        BLUR = 1,
        REFLECT = 2,
    };

    enum EBlendMode : unsigned int {
        DEFAULT,
        ADD,
        MODE2,
        MODE3,
    };

    struct SRS_EFFECT {};

    struct SRS_BLUR : public SRS_EFFECT {
        unsigned int unk1{};
        unsigned int unk2{};
        unsigned int cropCount{};
        unsigned int steps{};
        int duration{};
        unsigned int flags{};
        Color color{};

        inline EBlendMode GetBlendMode() const {
            return static_cast<EBlendMode>(flags & 0xF);
        }

        inline void SetBlendMode(EBlendMode type) {
            flags = (flags & ~0xF) | (static_cast<unsigned int>(type) & 0xF);
        }

        inline bool Hides() const {
            return flags & 0x1000;
        }

        inline void SetHides(bool type) {
            if (type)
                flags |= 0x1000;
            else
                flags &= ~0x1000;
        }
    };

    struct SRS_BLUR3D : public SRS_BLUR {};

    struct SRS_REFLECT : public SRS_EFFECT {
        unsigned int unk1{};
        unsigned int unk2{};
        unsigned int unk3{};
        float unk4{};
        float unk5{};
        float unk6{};
        float unk7{};
        unsigned int flags{};
        Color color{};

        inline EBlendMode GetBlendMode() const {
            return static_cast<EBlendMode>(flags & 0xF);
        }

        inline void SetBlendMode(EBlendMode type) {
            flags = (flags & ~0xF) | (static_cast<unsigned int>(type) & 0xF);
        }

        inline bool Hides() const {
            return flags & 0x1000;
        }

        inline void SetHides(bool type) {
            if (type)
                flags |= 0x1000;
            else
                flags &= ~0x1000;
        }
    };

    struct SRS_REFLECT3D : public SRS_REFLECT {};

    // Transforms
    struct SRS_TRS_BASE {
        Color materialColor{};
        Color illuminationColor{};
        bool display{};
    };

    struct SRS_TRS2D : public SRS_TRS_BASE {
        Vector2 position{};
        int rotation{};
        Vector2 scale{ 1.0f, 1.0f };
        unsigned int unkX{};
        unsigned int unkY{};
    };

    struct SRS_TRS3D : public SRS_TRS_BASE {
        Vector3 position{};
        Rotation3 rotation{};
        Vector3 scale{ 1.0f, 1.0f, 1.0f };
    };

    // Casts
    union SRS_EFFECT_PTR {
        SRS_BLUR3D* blur;
        SRS_REFLECT3D* reflect;
    };

    struct SRS_IMAGECAST {
        unsigned int flags{};
        Vector2 size{};
        Vector2 pivot{};
        Color vertexColorTopLeft{};
        Color vertexColorBottomLeft{};
        Color vertexColorTopRight{};
        Color vertexColorBottomRight{};
        short cropIndex0{};
        short cropIndex1{};
        short cropRef0Count{};
        short cropRef1Count{};
        SRS_CROPREF* cropRefs0{};
        SRS_CROPREF* cropRefs1{};
        SRS_TEXTDATA* textData{};
        unsigned int effectType{};
        SRS_EFFECT_PTR effectData{};

        inline EEffectType GetEffectType() const {
            return static_cast<EEffectType>(effectType & 0xF);
        }

        inline void SetEffectType(EEffectType type) {
            effectType = (effectType & ~0xF) | static_cast<unsigned int>(type & 0xF);
        }

        inline EPivotType GetPivotType() const {
            return static_cast<EPivotType>((flags >> 19) & 0xF);
        }

        inline void SetPivotType(EPivotType pivotType) {
            flags = (flags & ~(0xF << 19)) | ((static_cast<unsigned int>(pivotType) & 0xF) << 19);
        }

        inline bool GetMaterialColorFlag() const {
            return flags & 0x10;
        }

        inline void SetMaterialColorFlag(bool enabled) {
            if (enabled)
                flags |= 0x10;
            else
                flags &= ~0x10;
        }

        inline bool GetIlluminationColorFlag() const {
            return flags & 0x20;
        }

        inline void SetIlluminationColorFlag(bool enabled) {
            if (enabled)
                flags |= 0x20;
            else
                flags &= ~0x20;
        }

        inline EOrientation GetOrientation() const {
            return static_cast<EOrientation>((flags >> 6) & 0x3);
        }

        inline void SetOrientation(EOrientation orientation) {
            flags = (flags & ~(0x3 << 6)) | ((static_cast<unsigned int>(orientation) & 0x3) << 6);
        }

        inline bool GetDisableTextureFlag() const {
            return flags & 0x100;
        }

        inline void SetDisableTextureFlag(bool enabled) {
            if (enabled)
                flags |= 0x100;
            else
                flags &= ~0x100;
        }

        inline bool GetUseCropSurface0Flag() const {
            return flags & 0x20000;
        }

        inline void SetUseCropSurface0Flag(bool enabled) {
            if (enabled)
                flags |= 0x20000;
            else
                flags &= ~0x20000;
        }
    };

    struct SRS_SLICE {
        unsigned int flags{};
        Vector2 fixedSize{};
        Color materialColor{};
        Color illuminationColor{};
        Color vertexColorTopLeft{};
        Color vertexColorBottomLeft{};
        Color vertexColorTopRight{};
        Color vertexColorBottomRight{};
        short cropIndex0{};
        short cropIndex1{};
    };

    struct SRS_SLICECAST {
        unsigned int flags{};
        Vector2 size{};
        Vector2 pivot{};
        Color vertexColorTopLeft{};
        Color vertexColorBottomLeft{};
        Color vertexColorTopRight{};
        Color vertexColorBottomRight{};
        Vector2 fixedSize{};
        short sliceHorizontalCount{};
        short sliceVerticalCount{};
        short horizontalFixedCount{};
        short verticalFixedCount{};
        short cropRef0Count{};
        short cropRef1Count{};
        SRS_CROPREF* cropRefs0{};
        unsigned int effectType{};
        SRS_EFFECT_PTR effectData{};
        SRS_SLICE slices[1]{};

        inline EEffectType GetEffectType() const {
            return static_cast<EEffectType>(effectType & 0xF);
        }

        inline void SetEffectType(EEffectType type) {
            effectType = (effectType & ~0xF) | static_cast<unsigned int>(type & 0xF);
        }

        inline EPivotType GetPivotType() const {
            return static_cast<EPivotType>((flags >> 19) & 0xF);
        }

        inline void SetPivotType(EPivotType pivotType) {
            flags = (flags & ~(0xF << 19)) | ((static_cast<unsigned int>(pivotType) & 0xF) << 19);
        }

        inline EOrientation GetOrientation() const {
            return static_cast<EOrientation>((flags >> 6) & 0x3);
        }

        inline void SetOrientation(EOrientation orientation) {
            flags = (flags & ~(0x3 << 6)) | ((static_cast<unsigned int>(orientation) & 0x3) << 6);
        }

        inline bool GetMaterialColorFlag() const {
            return flags & 0x10;
        }

        inline void SetMaterialColorFlag(bool enabled) {
            if (enabled)
                flags |= 0x10;
            else
                flags &= ~0x10;
        }

        inline bool GetIlluminationColorFlag() const {
            return flags & 0x20;
        }

        inline void SetIlluminationColorFlag(bool enabled) {
            if (enabled)
                flags |= 0x20;
            else
                flags &= ~0x20;
        }
    };

    struct SRS_LAYER;
    struct SRS_REFERENCECAST {
        SRS_LAYER* layer{};
        int flags{};
        unsigned int animationId{};
        unsigned int animationFrame{};
        int unk2{};
    };

    union SRS_CAST_PTR {
        void* none;
        SRS_IMAGECAST* image;
        SRS_SLICECAST* slice;
        SRS_REFERENCECAST* reference;
    };

    struct SRS_CASTNODE {
        enum class Type {
            NORMAL,
            IMAGE,
            SLICE,
            REFERENCE,
        };

        const char* name{};
        int id{};
        unsigned int flags{};
        SRS_CAST_PTR data{};
        short childIndex{ -1 };
        short siblingIndex{ -1 };
        SRS_USERDATA* userData{};

        inline Type GetType() const {
            return static_cast<Type>(flags & 0xF);
        }

        inline void SetType(Type type) {
            flags = (flags & ~0xF) | (static_cast<unsigned int>(type) & 0xF);
        }
    };

    // Layers
    union SRS_TRS_PTR {
        SRS_TRS2D* transforms2d;
        SRS_TRS3D* transforms3d;
    };

    struct SRS_LAYER {
        const char* name{};
        int id{};
        int flags{};
        int castCount{};
        SRS_CASTNODE* casts{};
        SRS_TRS_PTR transforms{};
        int animationCount{};
        SRS_ANIMATION* animations{};
        int currentAnimationIndex{};
        SRS_USERDATA* userData{};

        inline bool Is3D() const {
            return flags & 0x1;
        }

        inline void SetIs3D(bool enabled) {
            if (enabled)
                flags |= 0x1;
            else
                flags &= ~0x1;
        }
    };

    // Cameras
    struct SRS_CAMERA {
        const char* name{};
        int id{};
        Vector3 position{};
        Vector3 target{};
        bool isOrthogonal{};
        int fov{};
        float nearPlane{};
        float farPlane{};
        float unk1{};
        unsigned long long unk2{};
    };

    // Scenes
    struct SRS_SCENE {
        const char* name{};
        int id{};
        int flags{};
        bool loaded{};
        int layerCount{};
        SRS_LAYER* layers{};
        unsigned short cameraCount;
        unsigned short currentCameraIndex{};
        SRS_CAMERA* cameras{};
        Color backgroundColor{};
        Vector2 resolution{};
        SRS_USERDATA* userData{};
    };

    // Projects
    struct SRS_PROJECT {
        const char* name{};
        unsigned short sceneCount{};
        unsigned short unk1{};
        unsigned short textureListCount{};
        unsigned short fontCount{};
        SRS_SCENE* scenes{};
        SRS_TEXTURELIST* textureLists{};
        SRS_FONT* fonts{};
        SRS_CAMERA camera{};
        int startFrame{};
        int endFrame{};
        float frameRate{};
        SRS_USERDATA* userData{};
    };

    // Binary data / container
    struct SRS_CHUNK_HEADER {
        unsigned int magic;
        unsigned int chunkSize;
    };

    struct SRS_BINARY_FILE_HEADER_CHUNK_HEADER {
        unsigned int chunkCount;
        unsigned int chunksStart;
        unsigned int chunksSize;
        unsigned int addressResolutionHeaderOffset;
        unsigned int revision;
    };

    struct SRS_TEXTURELIST_CHUNK_HEADER {
        unsigned int startOffset;
        unsigned int textureListCount;
    };

    struct SRS_PROJECT_CHUNK_HEADER {
        unsigned int startOffset;
    };

    struct SRS_ADDRESS_RESOLUTION_CHUNK_HEADER {
        unsigned int addressToResolveCount;
        unsigned int isResolved; // 0 if not, 1 if yes
    };
}
