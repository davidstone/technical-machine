// Stat stages
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

#ifndef STAT__STAGE_HPP_
#define STAT__STAGE_HPP_

#include <algorithm>
#include <cstdint>
#include <bounded_integer/array.hpp>
#include <bounded_integer/bounded_integer.hpp>
#include "stat_names.hpp"
#include "../rational.hpp"

namespace technicalmachine {

using namespace bounded_integer::literal;

class Stage {
public:
	using value_type = bounded_integer::clamped_integer<-6, 6>;
	using boost_type = bounded_integer::checked_integer<-3, 12>;
	static constexpr auto number_of_stats = bounded_integer::make_bounded<static_cast<intmax_t>(StatNames::END)>();
	using container_type = bounded_integer::array<value_type, number_of_stats.value()>;

	Stage();

	auto operator[](StatNames index) const -> value_type const &;
	auto operator[](StatNames index) -> value_type &;
	
	auto begin() const -> container_type::const_iterator;
	auto begin() -> container_type::iterator;
	auto end() const -> container_type::const_iterator;
	auto end() -> container_type::iterator;

	template<StatNames stat>
	auto modifier(bool const ch = false) const {
		return stage_modifier(operator[](stat), ch, std::integral_constant<StatNames, stat>{});
	}

	using hash_type = uint64_t;
	auto hash() const -> hash_type;
	static auto max_hash() -> hash_type;
private:

	container_type m_stages;


	static auto offensive_modifier(value_type const stage, bool const ch) {
		return BOUNDED_INTEGER_CONDITIONAL((stage >= 0_bi),
			make_bounded_rational(2_bi + bounded_integer::abs(stage), 2_bi),
			make_bounded_rational(2_bi, BOUNDED_INTEGER_CONDITIONAL(!ch, 2_bi + bounded_integer::abs(stage), 2_bi))
		);
	}
	static auto defensive_modifier(value_type const stage, bool const ch) {
		return BOUNDED_INTEGER_CONDITIONAL((stage <= 0_bi),
			make_bounded_rational(2_bi, 2_bi + bounded_integer::abs(stage)),
			make_bounded_rational(BOUNDED_INTEGER_CONDITIONAL(!ch, 2_bi + bounded_integer::abs(stage), 2_bi), 2_bi)
		);
	}
	template<typename Base>
	static auto neutral_modifier(value_type const stage, Base const base) {
		return BOUNDED_INTEGER_CONDITIONAL((stage >= 0_bi),
			make_bounded_rational(base + bounded_integer::abs(stage), base),
			make_bounded_rational(base, base + bounded_integer::abs(stage))
		);
	}

	static auto stage_modifier(value_type const stage, bool const ch, std::integral_constant<StatNames, StatNames::ATK>) {
		return offensive_modifier(stage, ch);
	}
	static auto stage_modifier(value_type const stage, bool const ch, std::integral_constant<StatNames, StatNames::SPA>) {
		return offensive_modifier(stage, ch);
	}

	static auto stage_modifier(value_type const stage, bool const ch, std::integral_constant<StatNames, StatNames::DEF>) {
		return defensive_modifier(stage, ch);
	}
	static auto stage_modifier(value_type const stage, bool const ch, std::integral_constant<StatNames, StatNames::SPD>) {
		return defensive_modifier(stage, ch);
	}

	static auto stage_modifier(value_type const stage, bool const ch, std::integral_constant<StatNames, StatNames::SPE>) {
		return neutral_modifier(stage, 2_bi);
	}
	static auto stage_modifier(value_type const stage, bool const ch, std::integral_constant<StatNames, StatNames::ACC>) {
		return neutral_modifier(stage, 3_bi);
	}
	static auto stage_modifier(value_type const stage, bool const ch, std::integral_constant<StatNames, StatNames::EVA>) {
		return neutral_modifier(stage, 3_bi);
	}
};

auto boost(Stage & stage, StatNames stat, Stage::boost_type number_of_stages) -> void;
auto boost_regular(Stage & stage, Stage::boost_type number_of_stages) -> void;
auto boost_physical(Stage & stage, Stage::boost_type number_of_stages) -> void;
auto boost_special(Stage & stage, Stage::boost_type number_of_stages) -> void;
auto boost_defensive(Stage & stage, Stage::boost_type number_of_stages) -> void;
auto boost_offensive(Stage & stage, Stage::boost_type number_of_stages) -> void;

template<typename Function>
auto accumulate(Stage const & stages, Function && f) {
	using sum_type = decltype(f(std::declval<Stage::value_type>()) * Stage::number_of_stats);
	return std::accumulate(stages.begin(), stages.end(), sum_type(0_bi), [& f](sum_type const initial, Stage::value_type const stage) {
		return initial + f(stage);
	});
}

auto swap_defensive(Stage & lhs, Stage & rhs) -> void;
auto swap_offensive(Stage & lhs, Stage & rhs) -> void;


auto operator==(Stage const & lhs, Stage const & rhs) -> bool;
auto operator!=(Stage const & lhs, Stage const & rhs) -> bool;

}	// namespace technicalmachine
#endif	// STAT__STAGE_HPP_
