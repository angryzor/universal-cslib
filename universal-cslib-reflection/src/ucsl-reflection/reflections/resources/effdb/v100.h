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
        field<float, "delay">,
        field<const char*, "particleName">,
        field<const char*, "boneName">,
        field<math::Vector3, "position">,
        field<math::Quaternion, "rotation">,
        field<math::Vector3, "scale">
    >;

    using Binding = structure<impl::Binding, "Binding", void,
        field<const char*, "clipName">,
        field<unsigned int, "particleCount">,
        field<dynamic_carray<Particle, field_resolver<unsigned int, "particleCount">>*, "particles">,
        field<unsigned int, "soundNameCount">,
        field<dynamic_carray<const char*, field_resolver<unsigned int, "soundNameCount">>*, "soundNames">
    >;

    using EffdbData = structure<impl::EffdbData, "EffdbData", void,
        field<unsigned int, "magic">,
        field<unsigned int, "version">,
        field<unsigned int, "bindingCount">,
        field<dynamic_carray<Binding, field_resolver<unsigned int, "bindingCount">>*, "bindings">
    >;
}

namespace simplerfl {
    template<> struct canonical<ucsl::resources::effdb::v100::Particle> { using type = ucsl::resources::effdb::v100::reflections::Particle; };
    template<> struct canonical<ucsl::resources::effdb::v100::Binding> { using type = ucsl::resources::effdb::v100::reflections::Binding; };
    template<> struct canonical<ucsl::resources::effdb::v100::EffdbData> { using type = ucsl::resources::effdb::v100::reflections::EffdbData; };
}
