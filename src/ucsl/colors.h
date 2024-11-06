#pragma once

namespace ucsl::colors {
	template<typename T>
	struct alignas(4) Color {
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

	typedef Color<unsigned char> Color8;
	typedef Color<float> Colorf;
}
