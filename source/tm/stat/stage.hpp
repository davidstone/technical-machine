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

#include <tm/stat/stat_names.hpp>
#include <tm/operators.hpp>
#include <tm/rational.hpp>

#include <bounded/integer.hpp>
#include <containers/array/array.hpp>
#include <containers/array/make_array.hpp>

namespace technicalmachine {

using namespace bounded::literal;

struct Stage {
	using value_type = bounded::clamped_integer<-6, 6>;
	using boost_type = bounded::checked_integer<-3, 12>;
	static constexpr auto number_of_stats = bounded::max_value<decltype(containers::size(containers::enum_range<BoostableStat>()))>;
	using size_type = std::remove_const_t<decltype(number_of_stats)>;
	using container_type = containers::array<value_type, number_of_stats.value()>;

	constexpr Stage():
		m_stages(containers::make_array_n(number_of_stats, static_cast<value_type>(0_bi)))
	{
	}

	friend constexpr auto begin(Stage const & stage) {
		return begin(stage.m_stages);
	}
	friend constexpr auto begin(Stage & stage) {
		return begin(stage.m_stages);
	}
	friend constexpr auto end(Stage const & stage) {
		return end(stage.m_stages);
	}
	friend constexpr auto end(Stage & stage) {
		return end(stage.m_stages);
	}

	auto operator[](BoostableStat index) const -> value_type const & {
		return at(m_stages, index);
	}
	auto operator[](BoostableStat index) -> value_type & {
		return at(m_stages, index);
	}
private:
	container_type m_stages;
};

using ::containers::detail::common::operator<=>;
using ::containers::detail::common::operator==;

namespace detail {

template<BoostableStat stat>
constexpr auto base_stat_boost() {
	if constexpr (stat == BoostableStat::acc or stat == BoostableStat::eva) {
		return 3_bi;
	} else {
		return 2_bi;
	}
}

}	// namespace detail

template<BoostableStat stat> requires(stat == BoostableStat::atk or stat == BoostableStat::spa)
auto modifier(Stage const & stage, bool const ch) {
	constexpr auto base = detail::base_stat_boost<stat>();
	return BOUNDED_CONDITIONAL((stage[stat] >= 0_bi),
		rational(base + bounded::abs(stage[stat]), base),
		rational(base, BOUNDED_CONDITIONAL(!ch, base + bounded::abs(stage[stat]), base))
	);
}

template<BoostableStat stat> requires(stat == BoostableStat::def or stat == BoostableStat::spd)
auto modifier(Stage const & stage, bool const ch) {
	constexpr auto base = detail::base_stat_boost<stat>();
	return BOUNDED_CONDITIONAL((stage[stat] <= 0_bi),
		rational(base, base + bounded::abs(stage[stat])),
		rational(BOUNDED_CONDITIONAL(!ch, base + bounded::abs(stage[stat]), base), base)
	);
}

template<BoostableStat stat> requires(stat == BoostableStat::spe or stat == BoostableStat::acc or stat == BoostableStat::eva)
auto modifier(Stage const & stage) {
	constexpr auto base = detail::base_stat_boost<stat>();
	return BOUNDED_CONDITIONAL((stage[stat] >= 0_bi),
		rational(base + bounded::abs(stage[stat]), base),
		rational(base, base + bounded::abs(stage[stat]))
	);
}


auto boost_regular(Stage & stage, Stage::boost_type number_of_stages) -> void;
auto boost_physical(Stage & stage, Stage::boost_type number_of_stages) -> void;
auto boost_special(Stage & stage, Stage::boost_type number_of_stages) -> void;
auto boost_defensive(Stage & stage, Stage::boost_type number_of_stages) -> void;
auto boost_offensive(Stage & stage, Stage::boost_type number_of_stages) -> void;

auto swap_defensive(Stage & lhs, Stage & rhs) -> void;
auto swap_offensive(Stage & lhs, Stage & rhs) -> void;

}	// namespace technicalmachine
