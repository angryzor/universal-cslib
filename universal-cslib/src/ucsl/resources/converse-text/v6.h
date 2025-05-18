#pragma once

// reversed from files

namespace ucsl::resources::converse_text::v6 {
    struct AttributeEntry {
        const char* type;
        int unk0;
        const char* value;
    };

    struct Attributes {
        unsigned int attributeAmount;
        AttributeEntry* attributes;
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
        float* defaultSize;
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

    struct FontLayout {
        const char* entryKey;
        Font* font;
        Layout* layout;
    };

    struct Entry {
        int hash;
        const char* key;
        FontLayout* fontLayout;
        wchar_t* text;
        unsigned int textLength;
        Attributes* attributes;
    };

    struct CnvrsTextData {
        unsigned char version;
        char unk0;
        unsigned short entryAmount;
        Entry* entries;
        const char* languageName;
    };
}