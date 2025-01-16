#pragma once

namespace ucsl::resources::density_setting::v11 {
	struct Unk1Data { // size 0x68

	};

	struct Unk2Data {
		unsigned int unk1;
		unsigned int unk2;

		const char* texture1;
		const char* texture2;
		const char* texture3;
		const char* texture4;
		uint64_t unk3;
		unsigned int unkCount;
	};
	struct DensitySettingData {
		unsigned int magic;
		unsigned int version;

		Unk2Data* unk2; // 0x78

		Unk1Data* unks;
		unsigned int unkCount; // 0x1A0
	};
}