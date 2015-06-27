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

#pragma once

#include <limits>
#include <string>
#include <type_traits>

#include <bounded_integer/bounded_integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;
struct Pokemon;

template<typename Numerator, typename Denominator>
struct bounded_rational;

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
struct bounded_rational {
private:
	template<typename N, typename D>
	friend struct bounded_rational;
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


}	// namespace technicalmachine

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

