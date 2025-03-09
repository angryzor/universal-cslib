#pragma once

// reversed from files

namespace ucsl::resources::converse_meta::v2 {
    struct Crop {
        float left{};
        float top{};
        float right{};
        float bottom{};
    };

    struct Icon {
        const char* iconName;
        const char* imageName;
        float unk0;
        float unk1;
        float unk2;
        float unk3;
        Crop crop;
        long long unk4;
        long long unk5;
    };

    struct IconInfo {
        unsigned int iconCount;
        Icon** icons;
        unsigned int resourceCount;
        const char** resourceNames;
    };

    struct TypeFace {
        unsigned int parentCount;
        const char** parents;
        const char* name0;
        const char* name1;
        float unk0;
        float unk1;
        float unk2;
        float unk3;
    };

    struct TypeFaceInfo {
        unsigned int typeFaceCount;
        TypeFace** typeFaces;
    };

    struct CnvrsMetaData {
        unsigned int version;
        TypeFaceInfo* typeFaceInfo;
        IconInfo* iconInfo;
    };
}