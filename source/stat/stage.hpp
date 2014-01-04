// Stat stages
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

#ifndef STAT__STAGE_HPP_
#define STAT__STAGE_HPP_

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <bounded_integer/array.hpp>
#include <bounded_integer/bounded_integer.hpp>
#include "stat_names.hpp"
#include "../rational.hpp"

namespace technicalmachine {
class Rational;

// Work around internal compiler error in gcc
#define RETURNS(x) -> decltype((x)) { return (x); }

using namespace bounded_integer::literal;

class Stage {
public:
	using value_type = bounded_integer::clamped_integer<-6, 6>;
	using boost_type = bounded_integer::checked_integer<-3, 12>;

private:
	class array {
	public:
		static constexpr auto size = static_cast<std::size_t>(StatNames::END);
		using container_type = bounded_integer::array<value_type, size>;
		constexpr container_type::const_iterator begin() const {
			return m_value.begin();
		}
		container_type::iterator begin() {
			return m_value.begin();
		}
		constexpr container_type::const_iterator end() const {
			return m_value.end();
		}
		container_type::iterator end() {
			return m_value.end();
		}
		value_type const & operator[](StatNames index) const;
		value_type & operator[](StatNames index);
		friend bool operator==(array const & lhs, array const & rhs);
	private:
		container_type m_value;
	};
	friend bool operator==(array const & lhs, array const & rhs);
	array m_stages;


	static auto offensive_modifier(value_type const stage, bool const ch)
		RETURNS(
			BOUNDED_INTEGER_CONDITIONAL((stage >= 0_bi),
				make_bounded_rational(2_bi + bounded_integer::abs(stage), 2_bi),
				make_bounded_rational(2_bi, BOUNDED_INTEGER_CONDITIONAL(!ch, 2_bi + bounded_integer::abs(stage), 2_bi))
			)
		)
	static auto defensive_modifier(value_type const stage, bool const ch)
		RETURNS(
			BOUNDED_INTEGER_CONDITIONAL((stage <= 0_bi),
				make_bounded_rational(2_bi, 2_bi + bounded_integer::abs(stage)),
				make_bounded_rational(BOUNDED_INTEGER_CONDITIONAL(!ch, 2_bi + bounded_integer::abs(stage), 2_bi), 2_bi)
			)
		)
	template<typename Base>
	static auto neutral_modifier(value_type const stage, Base const base)
		RETURNS(
			BOUNDED_INTEGER_CONDITIONAL((stage >= 0_bi),
				make_bounded_rational(base + bounded_integer::abs(stage), base),
				make_bounded_rational(base, base + bounded_integer::abs(stage))
			)
		)

	static auto stage_modifier(value_type const stage, bool const ch, std::integral_constant<StatNames, StatNames::ATK>)
		RETURNS(offensive_modifier(stage, ch))
	static auto stage_modifier(value_type const stage, bool const ch, std::integral_constant<StatNames, StatNames::SPA>)
		RETURNS(offensive_modifier(stage, ch))

	static auto stage_modifier(value_type const stage, bool const ch, std::integral_constant<StatNames, StatNames::DEF>)
		RETURNS(defensive_modifier(stage, ch))
	static auto stage_modifier(value_type const stage, bool const ch, std::integral_constant<StatNames, StatNames::SPD>)
		RETURNS(defensive_modifier(stage, ch))

	static auto stage_modifier(value_type const stage, bool const ch, std::integral_constant<StatNames, StatNames::SPE>)
		RETURNS(neutral_modifier(stage, 2_bi))
	static auto stage_modifier(value_type const stage, bool const ch, std::integral_constant<StatNames, StatNames::ACC>)
		RETURNS(neutral_modifier(stage, 3_bi))
	static auto stage_modifier(value_type const stage, bool const ch, std::integral_constant<StatNames, StatNames::EVA>)
		RETURNS(neutral_modifier(stage, 3_bi))

public:

	Stage();
	void reset();

	template<StatNames stat>
	auto modifier(bool const ch = false) const
		RETURNS(stage_modifier(m_stages[stat], ch, std::integral_constant<StatNames, stat>{}))

	void boost(StatNames stat, boost_type number_of_stages);
	template<typename Function>
	auto accumulate(Function const & f) const {
		using sum_type = decltype(f(std::declval<value_type>()) * bounded_integer::make_bounded<m_stages.size>());
		return std::accumulate(m_stages.begin(), m_stages.end(), sum_type(0_bi), [& f](sum_type const initial, value_type const stage) {
			return initial + f(stage);
		});
	}
	friend int dot_product(Stage const & stage, std::array<int, static_cast<std::size_t>(StatNames::END)> const & multiplier);
	void boost_regular(boost_type number_of_stages);
	void boost_physical(boost_type number_of_stages);
	void boost_special(boost_type number_of_stages);
	void boost_defensive(boost_type number_of_stages);
	void boost_offensive(boost_type number_of_stages);
	static void swap_defensive(Stage & lhs, Stage & rhs);
	static void swap_offensive(Stage & lhs, Stage & rhs);
	uint64_t hash() const;
	static uint64_t max_hash();
	friend bool operator==(Stage const & lhs, Stage const & rhs);
private:
	friend class Stat;
	friend class ActivePokemon;
	static void swap_specified(Stage & lhs, Stage & rhs, std::initializer_list<StatNames> const & stats);
};

bool operator!=(Stage const & lhs, Stage const & rhs);

}	// namespace technicalmachine
#endif	// STAT__STAGE_HPP_
