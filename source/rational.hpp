// Class to properly do integer multiplication / division
// Copyright (C) 2013 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#ifndef RATIONAL_HPP_
#define RATIONAL_HPP_

#include <limits>
#include <string>
#include <type_traits>

#include <ranged_integer/ranged_integer.hpp>

namespace technicalmachine {
class Pokemon;

class Rational {
public:
	using underlying_type = unsigned;
	constexpr explicit Rational(underlying_type const n = 1, underlying_type const d = 1):
		numerator(n),
		denominator(d) {
	}
	template<
		intmax_t lhs_min, intmax_t lhs_max, typename lhs_policy,
		intmax_t rhs_min, intmax_t rhs_max, typename rhs_policy,
		typename = std::enable_if<
			lhs_min >= std::numeric_limits<underlying_type>::min() and
			rhs_min >= std::numeric_limits<underlying_type>::min() and
			lhs_max >= std::numeric_limits<underlying_type>::max() and
			rhs_max >= std::numeric_limits<underlying_type>::max()
		>
	>
	constexpr Rational(ranged_integer<lhs_min, lhs_max, lhs_policy> const n, ranged_integer<rhs_min, rhs_max, rhs_policy> const d):
		numerator(static_cast<underlying_type>(n)),
		denominator(static_cast<underlying_type>(d)) {
	}
	template<typename T>
	friend T & operator*=(T & number, Rational const rational) {
		return compound_multiplication(number, rational, std::is_floating_point<T>{});
	}
	friend Rational operator*=(Rational & lhs, Rational const rhs) {
		lhs.numerator *= rhs.numerator;
		lhs.denominator *= rhs.denominator;
		return lhs;
	}
	friend constexpr Rational operator*(Rational const lhs, Rational const rhs) {
		return Rational(lhs.numerator * rhs.numerator, lhs.denominator * rhs.denominator);
	}
	friend Rational & operator+=(Rational & lhs, Rational const rhs) {
		lhs.numerator *= rhs.denominator;
		lhs.numerator += rhs.numerator * lhs.denominator;
		lhs.denominator *= rhs.denominator;
		return lhs;
	}
	friend constexpr Rational operator+(Rational const lhs, Rational const rhs) {
		return Rational(lhs.numerator * rhs.denominator + rhs.numerator * lhs.denominator, lhs.denominator * rhs.denominator);
	}
	friend Rational & operator-=(Rational & lhs, Rational const rhs) {
		lhs.numerator *= rhs.denominator;
		lhs.numerator -= rhs.numerator * lhs.denominator;
		lhs.denominator *= rhs.denominator;
		return lhs;
	}
	friend constexpr Rational operator-(Rational const lhs, Rational const rhs) {
		return Rational(lhs.numerator * rhs.denominator - rhs.numerator * lhs.denominator, lhs.denominator * rhs.denominator);
	}
	friend constexpr Rational complement(Rational const rational) {
		return Rational(rational.denominator - rational.numerator, rational.denominator);
	}
	// All these relational operators assume no overflow
	friend constexpr bool operator==(Rational const lhs, Rational const rhs) {
		return lhs.numerator * rhs.denominator == rhs.numerator * lhs.denominator;
	}
	friend constexpr bool operator<(Rational const lhs, Rational const rhs) {
		return lhs.numerator * rhs.denominator < rhs.numerator * lhs.denominator;
	}
	friend constexpr bool operator<=(Rational const lhs, Rational const rhs) {
		return lhs.numerator * rhs.denominator <= rhs.numerator * lhs.denominator;
	}
	friend std::string to_string(Rational const rational) {
		return std::to_string(rational.numerator) + " / " + std::to_string(rational.denominator);
	}
private:
	template<typename T>
	friend T & compound_multiplication(T & number, Rational const rational, std::false_type is_floating_point) {
		auto const temp = number * static_cast<typename Temp<T>::type>(rational.numerator);
		number = static_cast<T>(temp / static_cast<typename Temp<T>::type>(rational.denominator));
		return number;
	}
	template<typename T>
	friend T & compound_multiplication(T & number, Rational const rational, std::true_type is_floating_point) {
		number *= rational.numerator;
		number /= rational.denominator;
		return number;
	}
	template<typename T, typename Enable = void>
	class Temp;
	template<typename T>
	class Temp<T, typename std::enable_if<std::numeric_limits<T>::is_signed>::type> {
	public:
		using type = typename std::make_signed<underlying_type>::type;
	};
	template<typename T>
	class Temp<T, typename std::enable_if<!std::numeric_limits<T>::is_signed>::type> {
	public:
		using type = typename std::make_unsigned<underlying_type>::type;
	};
	underlying_type numerator;
	underlying_type denominator;
};

template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, T>::type
operator*(T number, Rational const rational) {
	return number *= rational;
}
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, T>::type
operator*(Rational const rational, T number) {
	return number *= rational;
}

constexpr bool operator!=(Rational const lhs, Rational const rhs) {
	return !(lhs == rhs);
}
constexpr bool operator>(Rational const lhs, Rational const rhs) {
	return rhs < lhs;
}
constexpr bool operator>=(Rational const lhs, Rational const rhs) {
	return rhs <= lhs;
}

}	// namespace technicalmachine

#endif	// RATIONAL_HPP_
