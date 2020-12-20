// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/stat_names.hpp>
#include <tm/operators.hpp>
#include <tm/rational.hpp>

#include <bounded/integer.hpp>
#include <containers/array/array.hpp>
#include <containers/array/make_array.hpp>
#include <containers/begin_end.hpp>
#include <containers/size.hpp>

namespace technicalmachine {

using namespace bounded::literal;

struct Stage {
	using value_type = bounded::integer<-6, 6>;
	using boost_type = bounded::integer<-3, 12>;
	static constexpr auto number_of_stats =
		bounded::constant<static_cast<int>(bounded::max_value<BoostableStat>) - static_cast<int>(bounded::min_value<BoostableStat>) + 1>;
	using size_type = std::remove_const_t<decltype(number_of_stats)>;
	using container_type = containers::array<value_type, number_of_stats.value()>;

	constexpr Stage():
		m_stages(containers::make_array_n(number_of_stats, static_cast<value_type>(0_bi)))
	{
	}

	constexpr auto begin() const & {
		return containers::begin(m_stages);
	}
	constexpr auto begin() & {
		return containers::begin(m_stages);
	}
	constexpr auto size() const {
		return containers::size(m_stages);
	}

	constexpr auto operator[](BoostableStat const index) const -> value_type const & {
		return m_stages[bounded::integer(index)];
	}
	constexpr auto operator[](BoostableStat index) -> value_type & {
		return m_stages[bounded::integer(index)];
	}

	friend auto operator==(Stage, Stage) -> bool = default;
private:
	container_type m_stages;
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

}	// namespace detail

template<BoostableStat stat> requires(stat == BoostableStat::atk or stat == BoostableStat::spa)
constexpr auto modifier(Stage const & stage, bool const ch) {
	constexpr auto base = detail::base_stat_boost<stat>();
	return BOUNDED_CONDITIONAL((stage[stat] >= 0_bi),
		rational(base + bounded::abs(stage[stat]), base),
		rational(base, BOUNDED_CONDITIONAL(!ch, base + bounded::abs(stage[stat]), base))
	);
}

template<BoostableStat stat> requires(stat == BoostableStat::def or stat == BoostableStat::spd)
constexpr auto modifier(Stage const & stage, bool const ch) {
	constexpr auto base = detail::base_stat_boost<stat>();
	return BOUNDED_CONDITIONAL((stage[stat] <= 0_bi),
		rational(base, base + bounded::abs(stage[stat])),
		rational(BOUNDED_CONDITIONAL(!ch, base + bounded::abs(stage[stat]), base), base)
	);
}

template<BoostableStat stat> requires(stat == BoostableStat::spe or stat == BoostableStat::acc or stat == BoostableStat::eva)
constexpr auto modifier(Stage const & stage) {
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
