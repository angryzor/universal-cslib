#pragma once

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
	};

	template<TypeKind K> struct Kind {

	};
}
