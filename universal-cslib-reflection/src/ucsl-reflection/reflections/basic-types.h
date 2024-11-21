#pragma once
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl/memory/iallocator.h>
#include <ucsl/containers/arrays/array.h>
#include <ucsl/containers/arrays/tarray.h>
#include <ucsl/strings/variable-string.h>
#include <ucsl/math.h>
#include <ucsl/colors.h>
#include <ucsl/object-id.h>

namespace ucsl::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	using Vector2 = structure<math::Vector2, "Vector2", void,
		field<float, "x">,
		field<float, "y">
	>;

	using Vector3 = aligned<16, structure<math::Vector3, "Vector3", void,
		field<float, "x">,
		field<float, "y">,
		field<float, "z">
	>>;

	using Vector4 = aligned<16, structure<math::Vector4, "Vector4", void,
		field<float, "x">,
		field<float, "y">,
		field<float, "z">,
		field<float, "w">
	>>;

	using Quaternion = aligned<16, structure<math::Quaternion, "Quaternion", void,
		field<float, "x">,
		field<float, "y">,
		field<float, "z">,
		field<float, "w">
	>>;

	using Matrix34 = aligned<16, structure<math::Matrix34, "Matrix34", void,
		field<Vector4, "t">,
		field<Vector4, "u">,
		field<Vector4, "v">,
		field<Vector4, "w">
	>>;

	using Matrix44 = aligned<16, structure<math::Matrix44, "Matrix44", void,
		field<Vector4, "t">,
		field<Vector4, "u">,
		field<Vector4, "v">,
		field<Vector4, "w">
	>>;

	using Position = structure<math::Position, "Position", void,
		field<float, "x">,
		field<float, "y">,
		field<float, "z">
	>;

	using Rotation = structure<math::Rotation, "Rotation", void,
		field<float, "x">,
		field<float, "y">,
		field<float, "z">,
		field<float, "w">
	>;

	template<typename T> size_t get_array_size(const containers::arrays::Array<T>& arr) { return arr.size(); }
	template<typename T>
	using Array = structure<containers::arrays::Array<T>, "Array", void,
		field<dynamic_carray<T, containers::arrays::Array<T>, get_array_size<T>>*, "buffer">,
		field<size_t, "length">,
		field<size_t, "capacity">,
		field<erased<memory::IAllocator*>, "allocator">
	>;

	template<typename T, containers::arrays::AllocatorGetterFn* get_allocator> size_t get_tarray_size(const containers::arrays::TArray<T, get_allocator>& arr) { return arr.size(); }
	template<typename T, containers::arrays::AllocatorGetterFn* get_allocator>
	using TArray = structure<containers::arrays::TArray<T, get_allocator>, "TArray", void,
		field<dynamic_carray<T, containers::arrays::TArray<T, get_allocator>, get_tarray_size<T, get_allocator>>*, "buffer">,
		field<size_t, "length">,
		field<size_t, "capacity">
	>;

	using VariableString = structure<strings::VariableString, "VariableString", void,
		field<const char*, "buffer">,
		field<erased<memory::IAllocator*>, "allocator">
	>;

	using ObjectIdV1 = primitive<objectids::ObjectIdV1>;

	using ObjectIdV2 = structure<objectids::ObjectIdV2, "ObjectId", void,
		field<unsigned long long, "groupId">,
		field<unsigned long long, "objectId">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::memory::IAllocator*> { using type = primitive<size_t>; };
	template<> struct canonical<ucsl::math::Vector2> { using type = ucsl::reflections::Vector2; };
	template<> struct canonical<ucsl::math::Vector3> { using type = ucsl::reflections::Vector3; };
	template<> struct canonical<ucsl::math::Vector4> { using type = ucsl::reflections::Vector4; };
	template<> struct canonical<ucsl::math::Quaternion> { using type = ucsl::reflections::Quaternion; };
	template<> struct canonical<ucsl::math::Matrix34> { using type = ucsl::reflections::Matrix34; };
	template<> struct canonical<ucsl::math::Matrix44> { using type = ucsl::reflections::Matrix44; };
	template<> struct canonical<ucsl::math::Position> { using type = ucsl::reflections::Position; };
	template<> struct canonical<ucsl::math::Rotation> { using type = ucsl::reflections::Rotation; };
	template<typename T> struct canonical<ucsl::containers::arrays::Array<T>> { using type = ucsl::reflection::array<T>; };
	template<typename T, ucsl::containers::arrays::AllocatorGetterFn* get_allocator> struct canonical<ucsl::containers::arrays::TArray<T, get_allocator>> { using type = ucsl::reflection::tarray<T, get_allocator>; };
	template<> struct canonical<ucsl::strings::VariableString> { using type = primitive<ucsl::strings::VariableString>; };
	template<> struct canonical<ucsl::objectids::ObjectIdV1> { using type = primitive<ucsl::objectids::ObjectIdV1>; };
	template<> struct canonical<ucsl::objectids::ObjectIdV2> { using type = primitive<ucsl::objectids::ObjectIdV2>; };
}
