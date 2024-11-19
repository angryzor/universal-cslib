#pragma once

namespace ucsl::rfl::ranges {
	namespace internal {
		struct RangeVector2Value { float x; float y; };
		struct RangeVector3Value { float x; float y; float z; };
		struct RangeVector4Value { float x; float y; float z; float w; };
	}

	template<typename T>
	struct Range {
		using value_type = T;

		T min;
		T max;
		T step;
	};

	struct RangeSint32 : Range<int32_t> {
		static constexpr const char* name = "RangeSint32";
	};

	struct RangeUint32 : Range<uint32_t> {
		static constexpr const char* name = "RangeUint32";
	};

	struct RangeSint64 : Range<int64_t> {
		static constexpr const char* name = "RangeSint64";
	};

	struct RangeUint64 : Range<uint64_t> {
		static constexpr const char* name = "RangeUint64";
	};

	struct RangeFloat : Range<float> {
		static constexpr const char* name = "RangeFloat";
	};

	struct RangeVector2 : Range<internal::RangeVector2Value> {
		static constexpr const char* name = "RangeVector2";
	};

	struct RangeVector3 : Range<internal::RangeVector3Value> {
		static constexpr const char* name = "RangeVector3";
	};

	struct RangeVector4 : Range<internal::RangeVector4Value> {
		static constexpr const char* name = "RangeVector4";
	};

	template<typename T> using range_value_t = typename T::value_type;
	template<typename T> static constexpr const char* range_name_v = T::name;

	template<typename T> struct range {};
	template<> struct range<int8_t> { using type = RangeSint32; };
	template<> struct range<uint8_t> { using type = RangeUint32; };
	template<> struct range<int16_t> { using type = RangeSint32; };
	template<> struct range<uint16_t> { using type = RangeUint32; };
	template<> struct range<int32_t> { using type = RangeSint32; };
	template<> struct range<uint32_t> { using type = RangeUint32; };
	template<> struct range<int64_t> { using type = RangeSint64; };
	template<> struct range<uint64_t> { using type = RangeUint64; };
	template<> struct range<float> { using type = RangeFloat; };
	template<> struct range<ucsl::math::Vector2> { using type = RangeVector2; };
	template<> struct range<ucsl::math::Vector3> { using type = RangeVector3; };
	template<> struct range<ucsl::math::Vector4> { using type = RangeVector4; };

	template<typename T> using range_t = typename range<T>::type;
}
