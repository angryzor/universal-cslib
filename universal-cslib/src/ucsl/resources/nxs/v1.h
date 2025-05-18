#pragma once

namespace ucsl::resources::nxs::v1 {
	struct MaterialData {
		unsigned int unk1;
		unsigned int flags;
	};

	struct NXSData {
		unsigned int magic;
		unsigned int version;
		unsigned int mesh1VertexCount;
		unsigned int mesh1TriangleCount;
		size_t materialCount;
		size_t mesh1Size;
		size_t mesh2Size;
		size_t mesh3Size;
		MaterialData* material;
		uint8_t* mesh1; // binary data: nVidia PhysX cooked meshes
		uint8_t* mesh2;
		uint8_t* mesh3;
	};
}
