#pragma once

namespace ucsl::objectids {
    struct ObjectIdV1 {
        unsigned int id{};
        
        inline bool IsNonNull() const { return id != 0; }
        inline bool IsNull() const { return !IsNonNull(); }
        inline bool operator==(const ObjectIdV1& other) const { return id == other.id; }
        inline bool operator!=(const ObjectIdV1& other) const { return !operator==(other); }
    };

	struct ObjectIdV2 {
        unsigned long long objectId{};
		unsigned long long groupId{};
        
        inline bool IsNonNull() const { return objectId != 0 || groupId != 0; }
        inline bool IsNull() const { return !IsNonNull(); }
        inline bool operator==(const ObjectIdV2& other) const { return objectId == other.objectId && groupId == other.groupId; }
        inline bool operator!=(const ObjectIdV2& other) const { return !operator==(other); }

        //void Print(csl::ut::VariableString& str) const;
        //void PrintGUID(csl::ut::VariableString& str) const;
	};
}
