#pragma once

// reversed from files

namespace ucsl::resources::scfnt::v1000 {
    struct alignas(8) SubCharacter {
        wchar_t character;
        short spacing;
    };

    struct Character {
        wchar_t character;
        short subCharCount;
        SubCharacter* subChars;
    };

    struct ScfntData {
        unsigned int magic;
        unsigned int version;
        const char* fontName;
        Character* chars;
        int charCount;
        long long ttfDataSize;
        char ttfData;
    };
}