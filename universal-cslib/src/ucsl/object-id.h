#pragma once

namespace ucsl::objectids {
	using ObjectIdV1 = unsigned int;
	struct ObjectIdV2 {
		uint64_t groupId;
		uint64_t objectId;
	};
}
