#pragma once
#include <ucsl/rfl/ranges.h>

namespace ucsl::reflection::providers {
	enum class TypeKind {
		PRIMITIVE,
		ENUM,
		FLAGS,
		ARRAY,
		TARRAY,
		POINTER,
		CARRAY,
		UNION,
		STRUCTURE,
	};

	template<typename R> struct PrimitiveData {
		using repr = R;

		std::optional<R> constant_value{};
		const ucsl::rfl::ranges::range_t<R>* range{};
	};

	template<TypeKind K> struct Kind {

	};
}
