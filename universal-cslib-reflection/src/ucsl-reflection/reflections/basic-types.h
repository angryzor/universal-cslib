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

	template<typename T, typename AllocatorSystem> size_t get_array_size(const containers::arrays::Array<T, AllocatorSystem>& arr) { return arr.size(); }
	template<typename T, typename AllocatorSystem>
	using Array = structure<containers::arrays::Array<T, AllocatorSystem>, "Array", void,
		field<dynamic_carray<T, containers::arrays::Array<T, AllocatorSystem>, get_array_size<T, AllocatorSystem>>*, "buffer">,
		field<size_t, "length">,
		field<size_t, "capacity">,
		field<erased<memory::IAllocator*>, "allocator">
	>;

	template<typename T, typename AllocatorSystem> size_t get_tarray_size(const containers::arrays::TArray<T, AllocatorSystem>& arr) { return arr.size(); }
	template<typename T, typename AllocatorSystem>
	using TArray = structure<containers::arrays::TArray<T, AllocatorSystem>, "TArray", void,
		field<dynamic_carray<T, containers::arrays::TArray<T, AllocatorSystem>, get_tarray_size<T, AllocatorSystem>>*, "buffer">,
		field<size_t, "length">,
		field<size_t, "capacity">
	>;

	using VariableString = structure<strings::VariableString, "VariableString", void,
		field<const char*, "buffer">,
		field<erased<memory::IAllocator*>, "allocator">
	>;

	using ObjectIdV1 = structure<objectids::ObjectIdV1, "ObjectId", void,
		field<unsigned int, "id">
	>;

	using ObjectIdV2 = structure<objectids::ObjectIdV2, "ObjectId", void,
		field<unsigned long long, "groupId">,
		field<unsigned long long, "objectId">
	>;

	template<typename T>
	using Color = aligned<4, structure<colors::Color<T, ucsl::colors::ChannelOrder::RGBA>, "Color", void,
		field<T, "r">,
		field<T, "g">,
		field<T, "b">,
		field<T, "a">
	>>;

	template<typename T>
	using Color = aligned<4, structure<colors::Color<T, ucsl::colors::ChannelOrder::ABGR>, "Color", void,
		field<T, "a">,
		field<T, "b">,
		field<T, "g">,
		field<T, "r">
	>>;

	template<ucsl::colors::ChannelOrder order>
	using Color8 = Color<unsigned char, order>;

	template<ucsl::colors::ChannelOrder order>
	using Colorf = Color<float, order>;

	typedef Color8<ChannelOrder::RGBA> Color8RGBA;
	typedef Colorf<ChannelOrder::RGBA> ColorfRGBA;
	typedef Color8<ChannelOrder::ABGR> Color8ABGR;
	typedef Colorf<ChannelOrder::ABGR> ColorfABGR;
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
	template<typename T, typename AllocatorSystem> struct canonical<ucsl::containers::arrays::Array<T, AllocatorSystem>> { using type = ucsl::reflection::array<T, AllocatorSystem>; };
	template<typename T, typename AllocatorSystem> struct canonical<ucsl::containers::arrays::TArray<T, AllocatorSystem>> { using type = ucsl::reflection::tarray<T, AllocatorSystem>; };
	template<> struct canonical<ucsl::strings::VariableString> { using type = primitive<ucsl::strings::VariableString>; };
	template<> struct canonical<ucsl::objectids::ObjectIdV1> { using type = primitive<ucsl::objectids::ObjectIdV1>; };
	template<> struct canonical<ucsl::objectids::ObjectIdV2> { using type = primitive<ucsl::objectids::ObjectIdV2>; };
	template<typename T> struct canonical<ucsl::colors::Color<T, ucsl::colors::ChannelOrder::RGBA>> { using type = ucsl::reflection::colors::Color<T, ucsl::colors::ChannelOrder::RGBA>; };
	template<typename T> struct canonical<ucsl::colors::Color<T, ucsl::colors::ChannelOrder::ABGR>> { using type = ucsl::reflection::colors::Color<T, ucsl::colors::ChannelOrder::ABGR>; };
	template<ucsl::colors::ChannelOrder order> struct canonical<ucsl::colors::Color8, order> { using type = ucsl::reflection::colors::Color8<order>; };
	template<ucsl::colors::ChannelOrder order> struct canonical<ucsl::colors::Colorf, order> { using type = ucsl::reflection::colors::Colorf<order>; };
}
