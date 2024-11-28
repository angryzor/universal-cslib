#pragma once

namespace ucsl::objectids {
    struct ObjectIdV1 {
        unsigned int id{};
    };

	struct ObjectIdV2 {
        uint64_t objectId{};
		uint64_t groupId{};

        //inline ObjectIdV2() = default;
        //inline ObjectIdV2(uint64_t objectId, uint64_t groupId) : objectId{ objectId }, groupId{ groupId } {}
        //inline ObjectIdV2(const ObjectIdV2& other) = default;

        //inline void CopyTo(ObjectIdV2& target) {
        //    target.objectId = objectId;
        //    target.groupId = groupId;
        //}

        //inline bool IsNonNull() const { return objectId != 0 || groupId != 0; }
        ////void Print(csl::ut::VariableString& str) const;
        ////void PrintGUID(csl::ut::VariableString& str) const;
        //inline bool operator==(const ObjectIdV2& other) const { return objectId == other.objectId && groupId == other.groupId; }
        //inline bool operator!=(const ObjectIdV2& other) const { return !operator==(other); }
	};
}
