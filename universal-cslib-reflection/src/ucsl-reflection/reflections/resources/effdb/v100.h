#pragma once
#include <ucsl/math.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

// reversed from files

namespace ucsl::resources::effdb::v100::reflections {
    using namespace simplerfl;
    using namespace ucsl::reflection;

    namespace impl = ::ucsl::resources::effdb::v100;

    using Particle = structure<impl::Particle, "Particle", void,
        field<bool, "attachedToBone">,
        field<unsigned char, "flags">,
        field<const char*, "particleName">,
        field<const char*, "boneName">,
        field<Vector3, "position">,
        field<Quaternion, "rotation">,
        field<Vector3, "scale">
    >;

    using State = structure<impl::State, "State", void,
        field<const char*, "stateName">,
        field<unsigned int, "particleCount">,
        field<dynamic_carray<Particle, impl::State, [](const impl::State& parent) -> size_t { return parent.particleCount; }>*, "particles">,
        field<unsigned int, "soundNameCount">,
        field<dynamic_carray<const char*, impl::State, [](const impl::State& parent) -> size_t { return parent.soundNameCount; }>*, "soundNames">,
    >;

    using EffdbData = structure<impl::EffdbData, "EffdbData", void,
        field<unsigned int, "magic">,
        field<unsigned int, "version">,
        field<unsigned int, "stateCount">,
        field<dynamic_carray<State, impl::EffdbData, [](const impl::EffdbData& parent) -> size_t { return parent.stateCount; }>*, "states">
    >;
}

namespace simplerfl {
    template<> struct canonical<ucsl::resources::effdb::v100::Particle> { using type = ucsl::resources::effdb::v100::reflections::Particle; };
    template<> struct canonical<ucsl::resources::effdb::v100::State> { using type = ucsl::resources::effdb::v100::reflections::State; };
    template<> struct canonical<ucsl::resources::effdb::v100::EffdbData> { using type = ucsl::resources::effdb::v100::reflections::EffdbData; };
}
