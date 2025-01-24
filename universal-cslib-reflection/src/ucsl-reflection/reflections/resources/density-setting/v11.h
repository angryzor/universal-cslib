#pragma once
#include <ucsl/resources/density-setting/v11.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::density_setting::v11::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::density_setting::v11;

	// see 0x140A0DA41 in rangers 1.41
	using ModelFlag = enumeration<impl::ModelFlag, "ModelFlag", unsigned int,
		option<"UNK0">,
		option<"UNK1">
	>;

	using Model = structure<impl::Model, "Model", void,
		field<const char*, "name">,
		field<unsigned int, "flags">
	>;

	using LodGroup = structure<impl::LodGroup, "LodGroup", void,
		field<unsigned int, "footPoint">,
		field<unsigned int, "flags">,
		field<unsigned int, "lodGroupReferenceOffset">,
		field<unsigned int, "lodGroupReferenceCount">,
		field<unsigned int, "collisionDataIndex1">,
		field<unsigned int, "collisionDataIndex2">,
		field<unsigned int, "shadowLodGroupReferenceOffset">,
		field<unsigned int, "shadowLodGroupReferenceCount">,
		field<float, "angle">,
		field<math::Position, "offset">,
		field<math::Position, "aabb">,
		field<unsigned int, "id">
	>;

	using LodGroupReference = structure<impl::LodGroupReference, "LodGroupReference", void,
		field<unsigned int, "index">,
		field<float, "length">,
		field<unsigned int, "flags">,
		field<constant<unsigned int, 0>, "padding1">
	>;

	using Biome = structure<impl::Biome, "Biome", void,
		field<unsigned int, "flags">,
		field<unsigned int, "biomeReferenceOffset">,
		field<unsigned int, "biomeReferenceCount">,
		field<unsigned int, "minBiomeReferenceOffset">
	>;

	using BiomeReference = structure<impl::BiomeReference, "BiomeReference", void,
		field<unsigned int, "index">,
		field<unsigned int, "flags">,
		field<unsigned int, "probability">,
		field<unsigned int, "copyFootPoint">,
		field<float, "scaleMin">,
		field<float, "scaleMax">,
		field<unsigned int, "hsvPh">,
		field<unsigned int, "hsvSv">,
		field<unsigned int, "upper">,
		field<constant<unsigned int, 0>, "padding1">,
		field<constant<unsigned int, 0>, "padding2">,
		field<constant<unsigned int, 0>, "padding3">
	>;

	// grass names
	using Unk6Data = structure<impl::Unk6Data, "Unk6Data", void,
		field<const char*, "name">,
		field<unsigned int, "unk1">,
		field<unsigned int, "unk2">,
		field<unsigned int, "unk3">,
		field<unsigned int, "unk4">
	>;

	// further to be reversed at 0x14C4D884F
	// see 0x1543216E0 and 0x14C4D8745
	using CollisionData = structure<impl::CollisionData, "CollisionData", void,
		field<math::Position, "offset">,
		field<math::Rotation, "rotation">,
		field<float, "radius">,
		field<unsigned int, "hitFlags">,
		field<unsigned int, "attri">,
		field<float[4], "param">,
		field<unsigned char, "idUpperType">,
		field<unsigned char, "idUpperUnk1">,
		field<unsigned short, "idUpperUnk2">,
		field<unsigned int, "idLower">,
		field<int, "soundEffectIndex">
	>;

	// unk9, 10, 11 and 12 seem unused
	using DensitySettingData = structure<impl::DensitySettingData, "DensitySettingData", void,
		field<constant<unsigned int, 0x43445347>, "magic">,
		field<constant<unsigned int, 11>, "version">,
		field<float, "width">,
		field<float, "height">,
		field<unsigned int, "unk1">,
		field<unsigned int, "unk2">,
		field<unsigned int[32], "lodUnks">,
		field<float[32], "lodRanges">,
		field<const char*, "areaTexture">,
		field<const char*, "layerTexture">,
		field<const char*, "colorTexture">,
		field<const char*, "scaleTexture">,
		field<dynamic_carray<Model, impl::DensitySettingData, [](const impl::DensitySettingData& parent) -> size_t { return parent.modelCount; }>*, "models">,
		field<int, "modelCount">,
		field<dynamic_carray<LodGroup, impl::DensitySettingData, [](const impl::DensitySettingData& parent) -> size_t { return parent.lodGroupCount; }>*, "lodGroups">,
		field<int, "lodGroupCount">,
		field<dynamic_carray<LodGroupReference, impl::DensitySettingData, [](const impl::DensitySettingData& parent) -> size_t { return parent.lodGroupReferenceCount; }>*, "lodGroupReferences">,
		field<int, "lodGroupReferenceCount">,
		field<dynamic_carray<Biome, impl::DensitySettingData, [](const impl::DensitySettingData& parent) -> size_t { return parent.biomeCount; }>*, "biomes">,
		field<int, "biomeCount">,
		field<dynamic_carray<BiomeReference, impl::DensitySettingData, [](const impl::DensitySettingData& parent) -> size_t { return parent.biomeReferenceCount; }>*, "biomeReferences">,
		field<int, "biomeReferenceCount">,
		field<dynamic_carray<Unk6Data, impl::DensitySettingData, [](const impl::DensitySettingData& parent) -> size_t { return parent.unk6Count; }>*, "unk6s">,
		field<int, "unk6Count">,
		field<dynamic_carray<CollisionData, impl::DensitySettingData, [](const impl::DensitySettingData& parent) -> size_t { return parent.collisionDataCount; }>*, "collisionDatas">,
		field<int, "collisionDataCount">,
		field<dynamic_carray<const char*, impl::DensitySettingData, [](const impl::DensitySettingData& parent) -> size_t { return parent.collisionResourceCount; }>*, "collisionResources">,
		field<int, "collisionResourceCount">,
		field<dynamic_carray<unsigned int, impl::DensitySettingData, [](const impl::DensitySettingData& parent) -> size_t { return parent.unk8Count; }>*, "unk8">,
		field<int, "unk8Count">,
		field<dynamic_carray<char, impl::DensitySettingData, [](const impl::DensitySettingData& parent) -> size_t { return parent.unk9Count; }>*, "unk9s">,
		field<int, "unk9Count">,
		field<dynamic_carray<char, impl::DensitySettingData, [](const impl::DensitySettingData& parent) -> size_t { return parent.unk10Count; }>*, "unk10s">,
		field<int, "unk10Count">,
		field<dynamic_carray<char, impl::DensitySettingData, [](const impl::DensitySettingData& parent) -> size_t { return parent.unk11Count; }>*, "unk11s">,
		field<int, "unk11Count">,
		field<dynamic_carray<char, impl::DensitySettingData, [](const impl::DensitySettingData& parent) -> size_t { return parent.unk12Count; }>*, "unk12s">,
		field<int, "unk12Count">,
		field<dynamic_carray<int, impl::DensitySettingData, [](const impl::DensitySettingData& parent) -> size_t { return parent.lodGroupSoundEffectIndexCount; }>*, "lodGroupSoundEffectIndices">,
		field<int, "lodGroupSoundEffectIndexCount">,
		field<dynamic_carray<const char*, impl::DensitySettingData, [](const impl::DensitySettingData& parent) -> size_t { return parent.soundEffectCount; }>*, "soundEffects">,
		field<int, "soundEffectCount">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::density_setting::v11::ModelFlag> { using type = ucsl::resources::density_setting::v11::reflections::ModelFlag; };
	template<> struct canonical<ucsl::resources::density_setting::v11::Model> { using type = ucsl::resources::density_setting::v11::reflections::Model; };
	template<> struct canonical<ucsl::resources::density_setting::v11::LodGroup> { using type = ucsl::resources::density_setting::v11::reflections::LodGroup; };
	template<> struct canonical<ucsl::resources::density_setting::v11::Biome> { using type = ucsl::resources::density_setting::v11::reflections::Biome; };
	template<> struct canonical<ucsl::resources::density_setting::v11::BiomeReference> { using type = ucsl::resources::density_setting::v11::reflections::BiomeReference; };
	template<> struct canonical<ucsl::resources::density_setting::v11::Unk6Data> { using type = ucsl::resources::density_setting::v11::reflections::Unk6Data; };
	template<> struct canonical<ucsl::resources::density_setting::v11::CollisionData> { using type = ucsl::resources::density_setting::v11::reflections::CollisionData; };
	template<> struct canonical<ucsl::resources::density_setting::v11::DensitySettingData> { using type = ucsl::resources::density_setting::v11::reflections::DensitySettingData; };
}
