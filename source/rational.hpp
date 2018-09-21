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
struct rational;

// Since we cannot use class template argument deduction inside the class body
template<typename Numerator, typename Denominator>
constexpr auto make_rational(Numerator const numerator, Denominator const denominator) {
	return rational<Numerator, Denominator>(numerator, denominator);
}

template<typename Numerator, typename Denominator>
struct rational {
private:
	template<typename N, typename D>
	friend struct rational;
	Numerator m_numerator;
	Denominator m_denominator;
	using numerator_type = Numerator;
	using denominator_type = Denominator;
public:
	constexpr rational(Numerator const numerator, Denominator const denominator):
		m_numerator(numerator),
		m_denominator(denominator) {
	}
	
	template<typename N, typename D, BOUNDED_REQUIRES(
		std::numeric_limits<N>::min() >= std::numeric_limits<Numerator>::min() and
		std::numeric_limits<N>::max() <= std::numeric_limits<Numerator>::max() and
		std::numeric_limits<D>::min() >= std::numeric_limits<Denominator>::min() and
		std::numeric_limits<D>::max() <= std::numeric_limits<Denominator>::max()
	)>
	constexpr rational(rational<N, D> const other):
		m_numerator(other.m_numerator),
		m_denominator(other.m_denominator)
	{
	}

	template<typename Integer, BOUNDED_REQUIRES(bounded::is_bounded_integer<Integer>)>
	friend constexpr auto operator*(rational<Numerator, Denominator> const lhs, Integer const rhs) {
		return rhs * lhs.m_numerator / lhs.m_denominator;
	}
	
	template<typename N1, typename D1, typename N2, typename D2>
	friend constexpr auto operator*(rational<N1, D1> const lhs, rational<N2, D2> const rhs) {
		return make_rational(lhs.m_numerator * rhs.m_numerator, lhs.m_denominator * rhs.m_denominator);
	}
	template<typename N1, typename D1, typename N2, typename D2>
	friend constexpr auto operator+(rational<N1, D1> const lhs, rational<N2, D2> const rhs) {
		return make_rational(
			lhs.m_numerator * rhs.m_denominator + rhs.m_numerator * lhs.m_denominator,
			lhs.m_denominator * rhs.m_denominator
		);
	}
	template<typename N1, typename D1, typename N2, typename D2>
	friend constexpr auto operator-(rational<N1, D1> const lhs, rational<N2, D2> const rhs) {
		return make_rational(
			lhs.m_numerator * rhs.m_denominator - rhs.m_numerator * lhs.m_denominator,
			lhs.m_denominator * rhs.m_denominator
		);
	}


	template<typename LN, typename LD, typename RN, typename RD>
	friend constexpr auto compare(rational<LN, LD> const lhs, rational<RN, RD> const rhs) {
		return bounded::compare(lhs.m_numerator * rhs.m_denominator, rhs.m_numerator * lhs.m_denominator);
	}
	
	template<typename Integer, BOUNDED_REQUIRES(bounded::is_bounded_integer<Integer>)>
	friend constexpr auto compare(rational const lhs, Integer const rhs) {
		return bounded::compare(lhs.m_numerator, rhs * lhs.m_denominator);
	}

	template<typename Integer, BOUNDED_REQUIRES(bounded::is_bounded_integer<Integer>)>
	friend constexpr auto compare(Integer const lhs, rational const rhs) {
		return bounded::compare(lhs * rhs.m_denominator, rhs.m_numerator);
	}

	template<typename LN, typename LD, typename RN, typename RD>
	friend constexpr auto operator==(rational<LN, LD> const lhs, rational<RN, RD> const rhs) {
		return lhs.m_numerator * rhs.m_denominator == rhs.m_numerator * lhs.m_denominator;
	}
	
	template<typename Integer, BOUNDED_REQUIRES(bounded::is_bounded_integer<Integer>)>
	friend constexpr auto operator==(rational const lhs, Integer const rhs) {
		return lhs.m_numerator == rhs * lhs.m_denominator;
	}

	template<typename Integer, BOUNDED_REQUIRES(bounded::is_bounded_integer<Integer>)>
	friend constexpr auto operator==(Integer const lhs, rational const rhs) {
		return lhs * rhs.m_denominator == rhs.m_numerator;
	}


	friend std::string to_string(rational<Numerator, Denominator> const r) {
		using bounded::to_string;
		return to_string(r.m_numerator) + " / " + to_string(r.m_denominator);
	}
	
	// Convert allows narrowing conversions, constructor does not
	template<typename T>
	constexpr T convert() const {
		return T(
			static_cast<typename T::numerator_type>(m_numerator),
			static_cast<typename T::denominator_type>(m_denominator)
		);
	}
	
	explicit constexpr operator double() const {
		return static_cast<double>(m_numerator) / static_cast<double>(m_denominator);
	}
};

template<typename Numerator, typename Denominator>
constexpr auto complement(rational<Numerator, Denominator> const r) {
	return rational(1_bi, 1_bi) - r;
}

template<typename T, typename Numerator, typename Denominator, BOUNDED_REQUIRES(bounded::is_bounded_integer<T>)>
constexpr auto operator*(T const lhs, rational<Numerator, Denominator> const rhs) {
	return rhs * lhs;
}


}	// namespace technicalmachine

namespace std {

template<typename N1, typename D1, typename N2, typename D2>
struct common_type<technicalmachine::rational<N1, D1>, technicalmachine::rational<N2, D2>> {
private:
	using numerator = typename common_type<N1, N2>::type;
	using denominator = typename common_type<D1, D2>::type;
public:
	using type = technicalmachine::rational<numerator, denominator>;
};

template<typename N1, typename D1, typename N2, typename D2>
struct common_type<technicalmachine::rational<N1, D1> const, technicalmachine::rational<N2, D2>> {
private:
	using numerator = typename common_type<N1, N2>::type;
	using denominator = typename common_type<D1, D2>::type;
public:
	using type = technicalmachine::rational<numerator, denominator> const;
};

template<typename N1, typename D1, typename N2, typename D2>
struct common_type<technicalmachine::rational<N1, D1>, technicalmachine::rational<N2, D2> const> {
private:
	using numerator = typename common_type<N1, N2>::type;
	using denominator = typename common_type<D1, D2>::type;
public:
	using type = technicalmachine::rational<numerator, denominator> const;
};

template<typename N1, typename D1, typename N2, typename D2>
struct common_type<technicalmachine::rational<N1, D1> const, technicalmachine::rational<N2, D2> const> {
private:
	using numerator = typename common_type<N1, N2>::type;
	using denominator = typename common_type<D1, D2>::type;
public:
	using type = technicalmachine::rational<numerator, denominator> const;
};

}	// namespace std

