#pragma once

namespace ucsl::objectids {
	using ObjectIdV1 = unsigned int;
	class ObjectIdV2 {
		uint64_t groupId;
		uint64_t objectId;
	};
}
