#pragma once

// reversed from files

namespace ucsl::resources::terrain_material::v1 {
    struct Material {
        const char* materialType;
        int id;
        int unk0;
        int unk1;
        const char* detailAbdTextureName;
        const char* detailNrmTextureName;
        const char* detailHgtTextureName;
        const char* abdTextureName;
        const char* nrmTextureName;
        const char* prmTextureName;
    };

    struct TerrainMaterialData {
        unsigned int magic;
        unsigned int version;
        Material* materials;
        int materialCount;
    };
}