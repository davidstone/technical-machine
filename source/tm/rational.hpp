// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/operators.hpp>
#include <tm/round_up_divide.hpp>

#include <bounded/integer.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/string.hpp>

#include <numeric_traits/min_max_value.hpp>

#include <string_view>

namespace technicalmachine {
using namespace bounded::literal;

template<typename Numerator, typename Denominator>
struct rational;

// Since we cannot use class template argument deduction inside the class body
template<typename Numerator, typename Denominator>
constexpr auto make_rational(Numerator const numerator, Denominator const denominator) {
	return rational<Numerator, Denominator>(numerator, denominator);
}

template<typename Numerator, typename Denominator>
struct rational {
	constexpr rational(Numerator const numerator, Denominator const denominator):
		m_numerator(numerator),
		m_denominator(denominator) {
	}
	
	template<bounded::convertible_to<Numerator> N, bounded::convertible_to<Denominator> D>
	constexpr rational(rational<N, D> const other):
		m_numerator(other.m_numerator),
		m_denominator(other.m_denominator)
	{
	}

	friend constexpr auto to_string(rational const r) {
		using bounded::to_string;
		return containers::concatenate<containers::string>(to_string(r.m_numerator), std::string_view(" / "), to_string(r.m_denominator));
	}

	friend constexpr auto round_up_divide(bounded::bounded_integer auto const lhs, rational const rhs) {
		return round_up_divide(lhs * rhs.m_denominator, rhs.m_numerator);
	}

	explicit constexpr operator double() const {
		return static_cast<double>(m_numerator) / static_cast<double>(m_denominator);
	}

	template<typename N, typename D>
	friend constexpr auto operator+(rational const lhs, rational<N, D> const rhs) {
		return make_rational(
			lhs.m_numerator * rhs.m_denominator + rhs.m_numerator * lhs.m_denominator,
			lhs.m_denominator * rhs.m_denominator
		);
	}

	template<typename N, typename D>
	friend constexpr auto operator-(rational const lhs, rational<N, D> const rhs) {
		return make_rational(
			lhs.m_numerator * rhs.m_denominator - rhs.m_numerator * lhs.m_denominator,
			lhs.m_denominator * rhs.m_denominator
		);
	}

	template<typename N, typename D>
	friend constexpr auto operator*(rational const lhs, rational<N, D> const rhs) {
		return make_rational(lhs.m_numerator * rhs.m_numerator, lhs.m_denominator * rhs.m_denominator);
	}
	friend constexpr auto operator*(rational const lhs, bounded::bounded_integer auto const rhs) {
		return rhs * lhs.m_numerator / lhs.m_denominator;
	}
	friend constexpr auto operator*(bounded::bounded_integer auto const lhs, rational const rhs) {
		return rhs * lhs;
	}

	friend constexpr auto operator/(bounded::bounded_integer auto const lhs, rational const rhs) {
		return lhs * rhs.m_denominator / rhs.m_numerator;
	}

	friend constexpr auto operator%(bounded::bounded_integer auto const lhs, rational const rhs) {
		auto const quotient = lhs / rhs;
		return lhs - quotient * rhs;
	}

	template<typename N, typename D>
	friend constexpr auto operator<=>(rational const lhs, rational<N, D> const rhs) {
		return lhs.m_numerator * rhs.m_denominator <=> rhs.m_numerator * lhs.m_denominator;
	}

	friend constexpr auto operator<=>(rational const lhs, bounded::bounded_integer auto const rhs) {
		return lhs.m_numerator <=> rhs * lhs.m_denominator;
	}

	template<typename N, typename D>
	friend constexpr auto operator==(rational const lhs, rational<N, D> const rhs) -> bool {
		return lhs.m_numerator * rhs.m_denominator == rhs.m_numerator * lhs.m_denominator;
	}

	friend constexpr auto operator==(rational const lhs, bounded::bounded_integer auto const rhs) -> bool {
		return lhs.m_numerator == rhs * lhs.m_denominator;
	}

private:
	template<typename N, typename D>
	friend struct rational;

	Numerator m_numerator;
	Denominator m_denominator;
};

} // namespace technicalmachine

template<typename N1, typename D1, typename N2, typename D2>
struct std::common_type<technicalmachine::rational<N1, D1>, technicalmachine::rational<N2, D2>> {
	using type = technicalmachine::rational<
		common_type_t<N1, N2>,
		common_type_t<D1, D2>
	>;
};

template<typename N1, typename D1, typename N2, typename D2>
struct std::common_type<technicalmachine::rational<N1, D1> const, technicalmachine::rational<N2, D2>> {
	using type = technicalmachine::rational<
		common_type_t<N1, N2>,
		common_type_t<D1, D2>
	>;
};

template<typename N1, typename D1, typename N2, typename D2>
struct std::common_type<technicalmachine::rational<N1, D1>, technicalmachine::rational<N2, D2> const> {
	using type = technicalmachine::rational<
		common_type_t<N1, N2>,
		common_type_t<D1, D2>
	>;
};

template<typename N1, typename D1, typename N2, typename D2>
struct std::common_type<technicalmachine::rational<N1, D1> const, technicalmachine::rational<N2, D2> const> {
	using type = technicalmachine::rational<
		common_type_t<N1, N2>,
		common_type_t<D1, D2>
	>;
};
