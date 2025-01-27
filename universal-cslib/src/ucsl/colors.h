#pragma once

namespace ucsl::colors {
	enum class ChannelOrder {
		RGBA,
		ABGR,
	};

	template<typename T, ChannelOrder order>
	struct alignas(4) Color;

	template<typename T>
	struct alignas(4) Color<T, ChannelOrder::RGBA> {
		T r{};
		T g{};
		T b{};
		T a{};

		Color() {}
		Color(T r, T g, T b, T a) : r{ r }, g{ g }, b{ b }, a{ a } {}

		inline bool operator==(const Color<T>& other) const {
			return r == other.r && g == other.g && b == other.b && a == other.a;
		}

		inline bool operator!=(const Color<T>& other) const {
			return !operator==(other);
		}
	};

	template<typename T>
	struct alignas(4) Color<T, ChannelOrder::ABGR> {
		T a{};
		T b{};
		T g{};
		T r{};

		Color() {}
		Color(T r, T g, T b, T a) : r{ r }, g{ g }, b{ b }, a{ a } {}

		inline bool operator==(const Color<T>& other) const {
			return r == other.r && g == other.g && b == other.b && a == other.a;
		}

		inline bool operator!=(const Color<T>& other) const {
			return !operator==(other);
		}
	};

	template<ChannelOrder order>
	using Color8 = Color<unsigned char, order>;

	template<ChannelOrder order>
	using Colorf = Color<float, order>;

	typedef Color8<ChannelOrder::RGBA> Color8RGBA;
	typedef Colorf<ChannelOrder::RGBA> ColorfRGBA;
	typedef Color8<ChannelOrder::ABGR> Color8ABGR;
	typedef Colorf<ChannelOrder::ABGR> ColorfABGR;
}
