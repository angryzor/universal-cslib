#pragma once

namespace ucsl::resources::rfl::v2 {
	struct Ref2RflData {};

	template<typename RflData = Ref2RflData>
	struct Ref2Data {
		unsigned int magic{};
		char version[4]{};
		unsigned int hash{};
		alignas(16) RflData data{};
	};
}
