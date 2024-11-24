#pragma once

namespace ucsl::reflection::util {
	template<typename T, typename E>
	class result {
		std::variant<T, E> data;

	public:
		constexpr result(const T& value) : data{ value } {}
		constexpr result(T&& value) : data{ std::move(value) } {}
		constexpr result(const E& err) : data{ err } {}
		constexpr result(T&& err) : data{ std::move(err) } {}

		constexpr bool has_value() const& { return std::holds_alternative<T>(data); }

		constexpr T& value()& { return std::get<T>(data); }
		constexpr const T& value() const& { return std::get<T>(data); }
		constexpr T&& value()&& { return std::get<T>(data); }
		constexpr const T&& value() const&& { return std::move(std::get<T>(data)); }
		constexpr E& error()& { return std::get<E>(data); }
		constexpr const E& error() const& { return std::get<E>(data); }
		constexpr E&& error()&& { return std::get<E>(data); }
		constexpr const E&& error() const&& { return std::move(std::get<E>(data)); }

		template<class U = T>
		constexpr T value_or(U&& default_value) const& { return has_value() ? std::get<T>(data) : std::forward<U>(default_value); }
		template<class U = T>
		constexpr T value_or(U&& default_value)&& { return has_value() ? std::move(std::get<T>(data)) : std::forward<U>(default_value); }
		template<class G = E>
		constexpr E error_or(G&& default_value) const& { return !has_value() ? std::get<E>(data) : std::forward<G>(default_value); }
		template<class G = E>
		constexpr E error_or(G&& default_value)&& { return !has_value() ? std::move(error()) : std::forward<G>(default_value); }
	};
}