// Class to properly do integer multiplication / division
// Copyright (C) 2015 David Stone
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

#include "operators.hpp"

#include <bounded/integer.hpp>

#include <limits>
#include <string>
#include <type_traits>

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
	using bounded::integer;
	using NLimits = std::numeric_limits<Numerator>;
	using DLimits = std::numeric_limits<Denominator>;
	using N = integer<static_cast<std::intmax_t>(NLimits::min()), static_cast<std::intmax_t>(NLimits::max())>;
	using D = integer<static_cast<std::intmax_t>(DLimits::min()), static_cast<std::intmax_t>(DLimits::max())>;
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

	template<auto minimum, auto maximum, typename overflow_policy>
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


	template<typename LN, typename LD, typename RN, typename RD>
	friend constexpr auto compare(bounded_rational<LN, LD> const lhs, bounded_rational<RN, RD> const rhs) {
		return bounded::compare(lhs.m_numerator * rhs.m_denominator, rhs.m_numerator * lhs.m_denominator);
	}
	
	template<typename Integer, BOUNDED_REQUIRES(bounded::is_bounded_integer<Integer>)>
	friend constexpr auto compare(bounded_rational const lhs, Integer const rhs) {
		return bounded::compare(lhs.m_numerator, rhs * lhs.m_denominator);
	}

	template<typename Integer, BOUNDED_REQUIRES(bounded::is_bounded_integer<Integer>)>
	friend constexpr auto compare(Integer const lhs, bounded_rational const rhs) {
		return bounded::compare(lhs * rhs.m_denominator, rhs.m_numerator);
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
	template<typename N, typename D, BOUNDED_REQUIRES(
		std::numeric_limits<N>::min() <= std::numeric_limits<numerator_type>::min() and
		std::numeric_limits<N>::max() >= std::numeric_limits<numerator_type>::max() and
		std::numeric_limits<D>::min() <= std::numeric_limits<denominator_type>::min() and
		std::numeric_limits<D>::max() >= std::numeric_limits<denominator_type>::max()
	)>
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



template<auto minimum, auto maximum, typename overflow_policy, typename Numerator, typename Denominator>
constexpr auto operator*(bounded::integer<minimum, maximum, overflow_policy> const number, bounded_rational<Numerator, Denominator> const rational) {
	return rational * number;
}

template<typename T, typename Numerator, typename Denominator, BOUNDED_REQUIRES(std::is_integral<T>::value)>
constexpr auto operator*(T const number, bounded_rational<Numerator, Denominator> const rational) {
	return static_cast<T>(bounded::integer(number) * rational);
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

