#pragma once

namespace ucsl::resources::rfl::v1 {
	struct Ref1RflData {};

	template<typename RflData = Ref1RflData>
	struct Ref1Data {
		alignas(16) RflData data{};
	};
}
