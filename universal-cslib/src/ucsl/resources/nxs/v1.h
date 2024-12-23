#pragma once

namespace ucsl::resources::nxs::v1 {
	struct NXSData {
		unsigned int magic;
		unsigned int version;
		unsigned int vertexCount;
		unsigned int triangleCount;
		size_t unk2Count;
		size_t mesh1Size;
		size_t mesh2Size;
		size_t mesh3Size;
		size_t* unk2;
		uint8_t* mesh1; // binary data: nVidia PhysX cooked meshes
		uint8_t* mesh2;
		uint8_t* mesh3;
	};
}
