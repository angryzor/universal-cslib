#pragma once

// reversed from files

namespace ucsl::resources::okern::v1 {
    struct SubCharacter {
        wchar_t character;
        short spacing;
    };

    struct Character {
        wchar_t character;
        short subCharCount;
        SubCharacter* subChars;
    };

    struct OkernData {
        unsigned int version;
        int charCount;
        Character* chars;
        const char* fontName;
    };
}