// Copyright (C) 2016 David Stone
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

#include "stat_names.hpp"
#include "../operators.hpp"
#include "../rational.hpp"

#include <bounded/integer.hpp>
#include <containers/array/array.hpp>
#include <containers/array/make_array.hpp>

namespace technicalmachine {

using namespace bounded::literal;

struct Stage {
	using value_type = bounded::clamped_integer<-6, 6>;
	using boost_type = bounded::checked_integer<-3, 12>;
	static constexpr auto number_of_stats = bounded::constant<static_cast<intmax_t>(StatNames::END)>;
	using container_type = containers::array<value_type, number_of_stats.value()>;

	constexpr Stage() noexcept:
		m_stages(containers::make_array_n<value_type>(number_of_stats, 0_bi))
	{
	}

	friend constexpr auto begin(Stage const & stage) noexcept {
		return begin(stage.m_stages);
	}
	friend constexpr auto begin(Stage & stage) noexcept {
		return begin(stage.m_stages);
	}
	friend constexpr auto end(Stage const & stage) noexcept {
		return end(stage.m_stages);
	}
	friend constexpr auto end(Stage & stage) noexcept {
		return end(stage.m_stages);
	}

	auto operator[](StatNames index) const -> value_type const & {
		return m_stages[index];
	}
	auto operator[](StatNames index) -> value_type & {
		return m_stages[index];
	}
private:
	container_type m_stages;
};

namespace detail {

template<StatNames stat>
constexpr auto base_stat_boost() {
	if constexpr (stat == StatNames::ACC or stat == StatNames::EVA) {
		return 3_bi;
	} else {
		return 2_bi;
	}
}

}	// namespace detail

template<StatNames stat, BOUNDED_REQUIRES(stat == StatNames::ATK or stat == StatNames::SPA)>
auto modifier(Stage const & stage, bool const ch) {
	constexpr auto base = detail::base_stat_boost<stat>();
	return BOUNDED_CONDITIONAL((stage[stat] >= 0_bi),
		make_rational(base + bounded::abs(stage[stat]), base),
		make_rational(base, BOUNDED_CONDITIONAL(!ch, base + bounded::abs(stage[stat]), base))
	);
}

template<StatNames stat, BOUNDED_REQUIRES(stat == StatNames::DEF or stat == StatNames::SPD)>
auto modifier(Stage const & stage, bool const ch) {
	constexpr auto base = detail::base_stat_boost<stat>();
	return BOUNDED_CONDITIONAL((stage[stat] <= 0_bi),
		make_rational(base, base + bounded::abs(stage[stat])),
		make_rational(BOUNDED_CONDITIONAL(!ch, base + bounded::abs(stage[stat]), base), base)
	);
}

template<StatNames stat, BOUNDED_REQUIRES(stat == StatNames::SPE or stat == StatNames::ACC or stat == StatNames::EVA)>
auto modifier(Stage const & stage) {
	constexpr auto base = detail::base_stat_boost<stat>();
	return BOUNDED_CONDITIONAL((stage[stat] >= 0_bi),
		make_rational(base + bounded::abs(stage[stat]), base),
		make_rational(base, base + bounded::abs(stage[stat]))
	);
}


auto boost(Stage & stage, StatNames stat, Stage::boost_type number_of_stages) -> void;
auto boost_regular(Stage & stage, Stage::boost_type number_of_stages) -> void;
auto boost_physical(Stage & stage, Stage::boost_type number_of_stages) -> void;
auto boost_special(Stage & stage, Stage::boost_type number_of_stages) -> void;
auto boost_defensive(Stage & stage, Stage::boost_type number_of_stages) -> void;
auto boost_offensive(Stage & stage, Stage::boost_type number_of_stages) -> void;

auto swap_defensive(Stage & lhs, Stage & rhs) -> void;
auto swap_offensive(Stage & lhs, Stage & rhs) -> void;

}	// namespace technicalmachine
