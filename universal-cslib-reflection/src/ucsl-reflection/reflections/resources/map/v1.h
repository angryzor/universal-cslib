#pragma once
#include <ucsl/resources/map/v1.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::map::v1::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::map::v1;

	using SectorData = structure<impl::SectorData, "SectorData", void,
		field<const char*, "name">,
		field<unsigned int, "id">,
		field<unsigned int, "unk1">,
		field<ucsl::math::Position, "position">
	>;

	template<typename AllocatorSystem>
	using MapData = structure<impl::MapData<AllocatorSystem>, "MapData", void,
		field<containers::arrays::TArray<SectorData, AllocatorSystem>, "sectors">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::map::v1::SectorData> { using type = ucsl::resources::map::v1::reflections::SectorData; };
	template<typename AllocatorSystem> struct canonical<ucsl::resources::map::v1::MapData<AllocatorSystem>> { using type = ucsl::resources::map::v1::reflections::MapData<AllocatorSystem>; };
}
