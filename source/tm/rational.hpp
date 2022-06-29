// Class to properly do integer multiplication / division
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/operators.hpp>

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
private:
	Numerator m_numerator;
	Denominator m_denominator;
	using numerator_type = Numerator;
	using denominator_type = Denominator;

public:
	constexpr rational(Numerator const numerator, Denominator const denominator):
		m_numerator(numerator),
		m_denominator(denominator) {
	}
	
	template<typename N, typename D> requires(
		numeric_traits::min_value<N> >= numeric_traits::min_value<Numerator> and
		numeric_traits::max_value<N> <= numeric_traits::max_value<Numerator> and
		numeric_traits::min_value<D> >= numeric_traits::min_value<Denominator> and
		numeric_traits::max_value<D> <= numeric_traits::max_value<Denominator>
	)
	constexpr rational(rational<N, D> const other):
		m_numerator(other.numerator()),
		m_denominator(other.denominator())
	{
	}

	constexpr auto numerator() const {
		return m_numerator;
	}
	constexpr auto denominator() const {
		return m_denominator;
	}

	friend constexpr auto to_string(rational const r) {
		using bounded::to_string;
		return containers::concatenate<containers::string>(to_string(r.numerator()), std::string_view(" / "), to_string(r.denominator()));
	}

	explicit constexpr operator double() const {
		return static_cast<double>(m_numerator) / static_cast<double>(m_denominator);
	}

	template<typename N, typename D>
	friend constexpr auto operator+(rational const lhs, rational<N, D> const rhs) {
		return make_rational(
			lhs.numerator() * rhs.denominator() + rhs.numerator() * lhs.denominator(),
			lhs.denominator() * rhs.denominator()
		);
	}

	template<typename N, typename D>
	friend constexpr auto operator-(rational const lhs, rational<N, D> const rhs) {
		return make_rational(
			lhs.numerator() * rhs.denominator() - rhs.numerator() * lhs.denominator(),
			lhs.denominator() * rhs.denominator()
		);
	}

	template<typename N, typename D>
	friend constexpr auto operator*(rational const lhs, rational<N, D> const rhs) {
		return make_rational(lhs.numerator() * rhs.numerator(), lhs.denominator() * rhs.denominator());
	}
	friend constexpr auto operator*(rational const lhs, bounded::bounded_integer auto const rhs) {
		return rhs * lhs.numerator() / lhs.denominator();
	}
	friend constexpr auto operator*(bounded::bounded_integer auto const lhs, rational const rhs) {
		return rhs * lhs;
	}

	friend constexpr auto operator/(bounded::bounded_integer auto const lhs, rational const rhs) {
		return lhs * rhs.denominator() / rhs.numerator();
	}

	friend constexpr auto operator%(bounded::bounded_integer auto const lhs, rational const rhs) {
		auto const quotient = lhs / rhs;
		return lhs - quotient * rhs;
	}

	template<typename N, typename D>
	friend constexpr auto operator<=>(rational const lhs, rational<N, D> const rhs) {
		return lhs.numerator() * rhs.denominator() <=> rhs.numerator() * lhs.denominator();
	}

	friend constexpr auto operator<=>(rational const lhs, bounded::bounded_integer auto const rhs) {
		return lhs.numerator() <=> rhs * lhs.denominator();
	}

	template<typename N, typename D>
	friend constexpr auto operator==(rational const lhs, rational<N, D> const rhs) -> bool {
		return lhs.numerator() * rhs.denominator() == rhs.numerator() * lhs.denominator();
	}

	friend constexpr auto operator==(rational const lhs, bounded::bounded_integer auto const rhs) -> bool {
		return lhs.numerator() == rhs * lhs.denominator();
	}
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
