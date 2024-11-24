#pragma once
#include <optional>

namespace ucsl::reflection::traversals {
	struct RootInfo {
		size_t alignment{};
		size_t size{};
	};

	struct StructureInfo {
		const char* name{};
		size_t alignment{};
	};

	struct UnionInfo {
		size_t size{};
	};

	struct FieldInfo {
		const char* name{};
		bool erased{};
	};

	struct ArrayFieldInfo : FieldInfo {
		size_t size{};
	};

	struct ArrayFieldItemInfo : ArrayFieldInfo {
		size_t index{};
	};

	struct TypeInfo {
		size_t alignment{};
		size_t size{};
	};

	struct PointerInfo {
		size_t targetAlignment{};
		size_t targetSize{};
	};

	struct CArrayInfo {
		size_t size{};
		size_t stride{};
	};

	struct ArrayInfo {
		size_t itemSize{};
		size_t itemAlignment{};
	};

	template<typename T>
	struct PrimitiveInfo {
		bool erased{};
		std::optional<T> constantValue{};
	};

	template<typename T>
	struct EnumInfo {
		const T& options{};
	};

	template<typename T>
	struct FlagsInfo {
		const T* flags{};
	};
}
