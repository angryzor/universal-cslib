#pragma once
#include <ucsl/colors.h>

// reversed from files

namespace ucsl::resources::converse_project::v4 {
    struct Language {
        const char* name;
        const char* shortName;
        long long id;
        long long unk0;
    };

    struct LanguageInfo {
        int languageCount;
        Language** languages;
    };

    struct Color {
        const char* name;
        ucsl::colors::Color8RGBA color;
        long long unk0;
    };

    struct ColorInfo {
        int colorCount;
        Color** colors;
    };

    struct ProjectSettings {
        ColorInfo* colorInfo;
        LanguageInfo* langInfo;
    };

    struct Layout {
        const char* idName;
        int* unk0;
        float* unk1;
        float* unk2;
        int* unk3;
        int* unk4;
        int* unk5;
        int* unk6;
        int* unk7;
        int* unk8;
        int* unk9;
    };

    struct Font {
        const char* idName;
        const char* fontName;
        float* unk0;
        float* unk1;
        float* unk2;
        int* unk3;
        int* unk4;
        int* unk5;
        int* unk6;
        int* unk7;
        int* unk8;
        int* unk9;
        int* unk10;
        int* unk11;
    };

    template<typename T>
    struct LanguageItem {
        const char* langName;
        int itemCount;
        T** items;
    };

    struct LanguageSettings {
        int count;
        LanguageItem<Font>** fonts;
        LanguageItem<Layout>** layouts;
    };

    struct CnvrsProjData {
        unsigned int version;
        ProjectSettings* projSettings;
        LanguageSettings* langSettings;
    };
}