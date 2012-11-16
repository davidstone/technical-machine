// Class to properly do integer multiplication / division
// Copyright (C) 2012 David Stone
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

#include <string>
#include <type_traits>

namespace technicalmachine {
class Pokemon;

class Rational {
	public:
		constexpr explicit Rational(unsigned const n = 1, unsigned const d = 1):
			numerator(n),
			denominator(d) {
		}
		template<typename T>
		friend typename std::enable_if<std::is_integral<T>::value, T>::type
		operator*=(T & number, Rational const rational) {
			auto const temp = number * static_cast<typename Temp<T>::type>(rational.numerator);
			number = static_cast<T>(temp / static_cast<typename Temp<T>::type>(rational.denominator));
			return number;
		}
		template<typename T>
		friend typename std::enable_if<std::is_floating_point<T>::value, T>::type
		operator*=(T & number, Rational const rational) {
			number *= rational.numerator;
			number /= rational.denominator;
			return number;
		}
		friend Rational operator*=(Rational & lhs, Rational const rhs) {
			lhs.numerator *= rhs.numerator;
			lhs.denominator *= rhs.denominator;
			return lhs;
		}
		friend constexpr Rational operator*(Rational const lhs, Rational const rhs) {
			return Rational(lhs.numerator * rhs.numerator, lhs.denominator * rhs.denominator);
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
		template<typename T, typename Enable = void>
		class Temp;
		template<typename T>
		class Temp<T, typename std::enable_if<std::is_signed<T>::value>::type> {
			public:
				typedef int type;
		};
		template<typename T>
		class Temp<T, typename std::enable_if<std::is_unsigned<T>::value>::type> {
			public:
				typedef unsigned type;
		};
		unsigned numerator;
		unsigned denominator;
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

}	// namespace technicalmachine

#endif	// RATIONAL_HPP_
