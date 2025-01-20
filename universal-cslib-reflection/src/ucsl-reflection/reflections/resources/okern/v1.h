#pragma once
#include <ucsl/math.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

// reversed from files

namespace ucsl::resources::okern::v1::reflections {
    using namespace simplerfl;
    using namespace ucsl::reflection;

    namespace impl = ::ucsl::resources::okern::v1::reflections;

    using SubCharacter = structure<impl::SubCharacter, "SubCharacter", void,
        field<wchar_t, "character">,
        field<short, "spacing">
    >;

    using Character = structure<impl::Character, "Character", void,
        field<wchar_t, "character">,
        field<short, "subCharCount">,
        field<dynamic_carray<SubCharacter, impl::Character, [](const impl::Character& parent) -> size_t { return parent.subCharCount; }>*, "subChars">
    >;

    using OkernData = structure<impl::OkernData, "OkernData", void,
        field<unsigned int, "version">,
        field<int, "charCount">,
        field<dynamic_carray<Character, impl::OkernData, [](const impl::OkernData& parent) -> size_t { return parent.charCount; }>*, "chars">
        field<const char*, "fontName">
    >;
}

namespace simplerfl {
    template<> struct canonical<ucsl::resources::okern::v1::SubCharacter> { using type = ucsl::resources::okern::v1::reflections::SubCharacter; };
    template<> struct canonical<ucsl::resources::okern::v1::Character> { using type = ucsl::resources::okern::v1::reflections::Character; };
    template<> struct canonical<ucsl::resources::okern::v1::OkernData> { using type = ucsl::resources::okern::v1::reflections::OkernData; };
}
