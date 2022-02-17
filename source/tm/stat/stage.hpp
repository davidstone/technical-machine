// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/stat_names.hpp>
#include <tm/compress.hpp>
#include <tm/operators.hpp>
#include <tm/rational.hpp>

#include <bounded/integer.hpp>

#include <containers/array.hpp>
#include <containers/begin_end.hpp>
#include <containers/size.hpp>

#include <numeric_traits/min_max_value.hpp>

namespace technicalmachine {

using namespace bounded::literal;

struct Stage {
	using value_type = bounded::integer<-6, 6>;
	using boost_type = bounded::integer<-3, 12>;

	Stage() = default;
	constexpr explicit Stage(value_type const value):
		m_value(value)
	{
	}

	constexpr auto value() const -> value_type {
		return m_value;
	}

	friend auto operator<=>(Stage, Stage) = default;
	friend auto operator==(Stage, Stage) -> bool = default;
	friend constexpr auto operator<=>(Stage const lhs, value_type const rhs) {
		return lhs.value() <=> rhs;
	}
	friend constexpr auto operator==(Stage const lhs, value_type const rhs) {
		return lhs.value() == rhs;
	}

	friend constexpr auto compress(Stage const stage) {
		return compress(stage.m_value);
	}

	friend constexpr auto operator+(Stage const lhs, boost_type const rhs) -> Stage {
		return Stage(bounded::clamp(lhs.value() + rhs, numeric_traits::min_value<value_type>, numeric_traits::max_value<value_type>));
	}
	friend constexpr auto operator+(boost_type const lhs, Stage const rhs) -> Stage {
		return rhs + lhs;
	}

private:
	value_type m_value = 0_bi;
};

struct Stages {
	Stages() = default;

	constexpr auto begin() const {
		return containers::begin(m_stages);
	}
	constexpr auto begin() {
		return containers::begin(m_stages);
	}
	constexpr auto size() const {
		return containers::size(m_stages);
	}

	constexpr auto operator[](BoostableStat const index) const -> Stage const & {
		return m_stages[bounded::integer(index)];
	}
	constexpr auto operator[](BoostableStat index) -> Stage & {
		return m_stages[bounded::integer(index)];
	}

	friend auto operator==(Stages, Stages) -> bool = default;

private:
	static constexpr auto number_of_stats = bounded::constant<numeric_traits::max_value<BoostableStat>> - bounded::constant<numeric_traits::min_value<BoostableStat>> + 1_bi;
	containers::array<Stage, number_of_stats> m_stages;
};

namespace detail {

template<BoostableStat stat>
constexpr auto base_stat_boost() {
	if constexpr (stat == BoostableStat::acc or stat == BoostableStat::eva) {
		return 3_bi;
	} else {
		return 2_bi;
	}
}

} // namespace detail

template<BoostableStat stat> requires(stat == BoostableStat::atk or stat == BoostableStat::spa)
constexpr auto modifier(Stages const & stages, bool const ch) {
	constexpr auto base = detail::base_stat_boost<stat>();
	return BOUNDED_CONDITIONAL((stages[stat] >= 0_bi),
		rational(base + bounded::abs(stages[stat].value()), base),
		rational(base, BOUNDED_CONDITIONAL(!ch, base + bounded::abs(stages[stat].value()), base))
	);
}

template<BoostableStat stat> requires(stat == BoostableStat::def or stat == BoostableStat::spd)
constexpr auto modifier(Stages const & stages, bool const ch) {
	constexpr auto base = detail::base_stat_boost<stat>();
	return BOUNDED_CONDITIONAL((stages[stat] <= 0_bi),
		rational(base, base + bounded::abs(stages[stat].value())),
		rational(BOUNDED_CONDITIONAL(!ch, base + bounded::abs(stages[stat].value()), base), base)
	);
}

template<BoostableStat stat> requires(stat == BoostableStat::spe or stat == BoostableStat::acc or stat == BoostableStat::eva)
constexpr auto modifier(Stages const & stages) {
	constexpr auto base = detail::base_stat_boost<stat>();
	return BOUNDED_CONDITIONAL((stages[stat] >= 0_bi),
		rational(base + bounded::abs(stages[stat].value()), base),
		rational(base, base + bounded::abs(stages[stat].value()))
	);
}


auto boost_regular(Stages & stages, Stage::boost_type number_of_stages) -> void;
auto boost_physical(Stages & stages, Stage::boost_type number_of_stages) -> void;
auto boost_special(Stages & stages, Stage::boost_type number_of_stages) -> void;
auto boost_defensive(Stages & stages, Stage::boost_type number_of_stages) -> void;
auto boost_offensive(Stages & stages, Stage::boost_type number_of_stages) -> void;

auto swap_defensive(Stages & lhs, Stages & rhs) -> void;
auto swap_offensive(Stages & lhs, Stages & rhs) -> void;

} // namespace technicalmachine
