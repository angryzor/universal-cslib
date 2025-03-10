#pragma once

// reversed from files

namespace ucsl::resources::vibration::v21 {
    struct Sound { // unsure
        const char* name;
        int soundChannelId;
    };

    struct VibrationNode {
        float position[2];
        int flags;
        float unk1[2]; // maybe related to curve
        float unk2[2]; // maybe related to curve
    };

    struct VibrationData {
        int id;
        unsigned int nodeCount;
        VibrationNode* nodes;
    };

    struct Vibration {
        const char* name;
        char unk0;
        unsigned char dataCount;
        VibrationData* datas;
        Sound* sound;
        void* unk1;
    };

    struct VibData {
        unsigned int magic;
        unsigned int version;
        unsigned int vibrationCount;
        Vibration* vibrations;
    };
}