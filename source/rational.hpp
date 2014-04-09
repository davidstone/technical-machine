// Class to properly do integer multiplication / division
// Copyright (C) 2014 David Stone
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

#include <bounded_integer/bounded_integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;
class Pokemon;

template<typename Numerator, typename Denominator>
class bounded_rational;

template<typename T>
struct is_bounded_rational : std::false_type {};

template<typename Numerator, typename Denominator>
struct is_bounded_rational<bounded_rational<Numerator, Denominator>> : std::true_type {};

template<typename Numerator, typename Denominator>
constexpr auto make_rational(Numerator const & numerator, Denominator const & denominator) {
	// It has the null policy so that it can convert to any other policy with
	// arithmetic operations.
	using bounded::equivalent_type;
	using bounded::null_policy;
	using N = equivalent_type<Numerator, null_policy>;
	using D = equivalent_type<Denominator, null_policy>;
	return bounded_rational<N, D>(numerator, denominator);
}

template<typename Numerator, typename Denominator>
class bounded_rational {
private:
	template<typename N, typename D>
	friend class bounded_rational;
	friend class Rational;
	Numerator m_numerator;
	Denominator m_denominator;
public:
	using numerator_type = Numerator;
	using denominator_type = Denominator;
	constexpr bounded_rational(numerator_type const numerator, denominator_type const denominator):
		m_numerator(numerator),
		m_denominator(denominator) {
	}

	template<intmax_t minimum, intmax_t maximum, typename overflow_policy>
	constexpr auto operator*(bounded::integer<minimum, maximum, overflow_policy> const number) const {
		return number * m_numerator / m_denominator;
	}

	template<typename N, typename D>
	constexpr auto operator*(bounded_rational<N, D> const other) const {
		return make_rational(m_numerator * other.m_numerator, m_denominator * other.m_denominator);
	}
	template<typename N, typename D>
	constexpr auto operator+(bounded_rational<N, D> const other) const {
		return make_rational(m_numerator * other.m_denominator + other.m_numerator * m_denominator, m_denominator * other.m_denominator);
	}
	template<typename N, typename D>
	constexpr auto operator-(bounded_rational<N, D> const other) const {
		return make_rational(m_numerator * other.m_denominator - other.m_numerator * m_denominator, m_denominator * other.m_denominator);
	}

	template<typename N, typename D>
	constexpr bool operator==(bounded_rational<N, D> const other) const {
		return m_numerator * other.m_denominator == other.m_numerator * m_denominator;
	}
	template<typename N, typename D>
	constexpr bool operator<(bounded_rational<N, D> const other) const {
		return m_numerator * other.m_denominator < other.m_numerator * m_denominator;
	}

	friend std::string to_string(bounded_rational<numerator_type, denominator_type> const rational) {
		using bounded::to_string;
		return to_string(rational.m_numerator) + " / " + to_string(rational.m_denominator);
	}
	
	// Convert allows narrowing conversions, conversion operator does not
	template<typename T>
	constexpr T convert() const {
		static_assert(is_bounded_rational<T>::value, "Only usable with a bounded_rational type.");
		return T(
			static_cast<typename T::numerator_type>(m_numerator),
			static_cast<typename T::denominator_type>(m_denominator)
		);
	}
	template<typename N, typename D, enable_if_t<
		std::numeric_limits<N>::min() <= std::numeric_limits<numerator_type>::min() and
		std::numeric_limits<N>::max() >= std::numeric_limits<numerator_type>::max() and
		std::numeric_limits<D>::min() <= std::numeric_limits<denominator_type>::min() and
		std::numeric_limits<D>::max() >= std::numeric_limits<denominator_type>::max()
	> = clang_dummy>
	constexpr operator bounded_rational<N, D>() const {
		return convert<bounded_rational<N, D>>();
	}
	
	explicit constexpr operator double() const {
		return static_cast<double>(m_numerator.value()) / static_cast<double>(m_denominator.value());
	}
};

template<typename Numerator, typename Denominator>
constexpr auto complement(bounded_rational<Numerator, Denominator> const rational) {
	return make_rational(1_bi, 1_bi) - rational;
}



template<intmax_t minimum, intmax_t maximum, typename overflow_policy, typename Numerator, typename Denominator>
constexpr auto operator*(bounded::integer<minimum, maximum, overflow_policy> const number, bounded_rational<Numerator, Denominator> const rational) {
	return rational * number;
}

template<intmax_t minimum, intmax_t maximum, typename overflow_policy, typename N, typename D>
auto & operator*=(bounded::integer<minimum, maximum, overflow_policy> & number, bounded_rational<N, D> const rational) {
	return number = number * rational;
}

template<typename T, typename Numerator, typename Denominator, enable_if_t<std::is_integral<T>::value> = clang_dummy>
constexpr auto operator*(T const number, bounded_rational<Numerator, Denominator> const rational) {
	return static_cast<T>(bounded::make(number) * rational);
}
template<typename T, typename Numerator, typename Denominator, enable_if_t<std::is_integral<T>::value> = clang_dummy>
auto operator*=(T & number, bounded_rational<Numerator, Denominator> const rational) {
	return number = number * rational;
}


template<typename N1, typename D1, typename N2, typename D2>
auto & operator*=(bounded_rational<N1, D1> & lhs, bounded_rational<N2, D2> const rhs) {
	return lhs = lhs * rhs;
}

template<typename N1, typename D1, typename N2, typename D2>
auto & operator+=(bounded_rational<N1, D1> & lhs, bounded_rational<N2, D2> const rhs) {
	return lhs = lhs + rhs;
}

template<typename N1, typename D1, typename N2, typename D2>
auto & operator-=(bounded_rational<N1, D1> & lhs, bounded_rational<N2, D2> const rhs) {
	return lhs = lhs - rhs;
}



template<typename N1, typename D1, typename N2, typename D2>
constexpr bool operator!=(bounded_rational<N1, D1> const lhs, bounded_rational<N2, D2> const rhs) {
	return !(lhs == rhs);
}
template<typename N1, typename D1, typename N2, typename D2>
constexpr bool operator>(bounded_rational<N1, D1> const lhs, bounded_rational<N2, D2> const rhs) {
	return rhs < lhs;
}
template<typename N1, typename D1, typename N2, typename D2>
constexpr bool operator>=(bounded_rational<N1, D1> const lhs, bounded_rational<N2, D2> const rhs) {
	return !(lhs < rhs);
}
template<typename N1, typename D1, typename N2, typename D2>
constexpr bool operator<=(bounded_rational<N1, D1> const lhs, bounded_rational<N2, D2> const rhs) {
	return !(rhs < lhs);
}




class Rational {
public:
	using underlying_type = unsigned;
	explicit constexpr Rational(underlying_type const n = 1, underlying_type const d = 1):
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
	constexpr Rational(bounded::integer<lhs_min, lhs_max, lhs_policy> const n, bounded::integer<rhs_min, rhs_max, rhs_policy> const d):
		numerator(static_cast<underlying_type>(n)),
		denominator(static_cast<underlying_type>(d)) {
	}
	template<typename Numerator, typename Denominator>
	explicit constexpr Rational(bounded_rational<Numerator, Denominator> const & other):
		numerator(other.m_numerator),
		denominator(other.m_denominator) {
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
	friend T & compound_multiplication(T & number, Rational const rational, std::false_type /* is_floating_point */) {
		auto const temp = number * static_cast<typename Temp<T>::type>(rational.numerator);
		number = static_cast<T>(temp / static_cast<typename Temp<T>::type>(rational.denominator));
		return number;
	}
	template<typename T>
	friend T & compound_multiplication(T & number, Rational const rational, std::true_type /* is_floating_point */) {
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

// I briefly considered writing this in terms of scaling the two types to have
// the same numerator, but I decided against that because it requires more than
// just manipulating types; the value must be scaled as well. Instead, this
// takes the free-at-runtime but more conservative approach of just getting the
// common type of the numerator and denominator.

namespace std {

template<typename N1, typename D1, typename N2, typename D2>
struct common_type<technicalmachine::bounded_rational<N1, D1>, technicalmachine::bounded_rational<N2, D2>> {
private:
	using numerator = typename common_type<N1, N2>::type;
	using denominator = typename common_type<D1, D2>::type;
public:
	using type = technicalmachine::bounded_rational<numerator, denominator>;
};

template<typename N1, typename D1, typename N2, typename D2>
struct common_type<technicalmachine::bounded_rational<N1, D1> const, technicalmachine::bounded_rational<N2, D2>> {
private:
	using numerator = typename common_type<N1, N2>::type;
	using denominator = typename common_type<D1, D2>::type;
public:
	using type = technicalmachine::bounded_rational<numerator, denominator> const;
};

template<typename N1, typename D1, typename N2, typename D2>
struct common_type<technicalmachine::bounded_rational<N1, D1>, technicalmachine::bounded_rational<N2, D2> const> {
private:
	using numerator = typename common_type<N1, N2>::type;
	using denominator = typename common_type<D1, D2>::type;
public:
	using type = technicalmachine::bounded_rational<numerator, denominator> const;
};

template<typename N1, typename D1, typename N2, typename D2>
struct common_type<technicalmachine::bounded_rational<N1, D1> const, technicalmachine::bounded_rational<N2, D2> const> {
private:
	using numerator = typename common_type<N1, N2>::type;
	using denominator = typename common_type<D1, D2>::type;
public:
	using type = technicalmachine::bounded_rational<numerator, denominator> const;
};

}	// namespace std

#endif	// RATIONAL_HPP_
