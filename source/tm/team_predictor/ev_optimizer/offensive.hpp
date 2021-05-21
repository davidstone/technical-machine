// Optimize offensive EVs and nature to remove waste
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/level.hpp>

#include <tm/stat/base_stats.hpp>
#include <tm/stat/calculate.hpp>
#include <tm/stat/initial_stat.hpp>
#include <tm/stat/iv.hpp>
#include <tm/stat/iv_and_ev.hpp>
#include <tm/stat/nature.hpp>
#include <tm/stat/stat_to_ev.hpp>

#include <bounded/integer.hpp>
#include <bounded/optional.hpp>

#include <containers/algorithms/filter_iterator.hpp>
#include <containers/algorithms/maybe_find.hpp>
#include <containers/algorithms/transform.hpp>
#include <containers/static_vector/static_vector.hpp>
#include <containers/size.hpp>

namespace technicalmachine {
using namespace bounded::literal;

template<Generation generation>
struct OffensiveEVInputs {
	IV iv;
	InitialStat<generation> stat;
	bool include_evs;
};

struct OffensiveStats {
	Nature nature;
	IVAndEV attack;
	IVAndEV special_attack;
};

namespace detail {

// If I don't have a physical move, prefer to lower that because it lowers
// confusion damage. If I do have a physical move but no special move, prefer
// to lower Special Attack because it is the only remaining stat guaranteed to
// be unused. This allows me to maximize Speed and the defensive stats.
constexpr auto useful_natures(bool const is_physical, bool const is_special) {
	return containers::filter(
		containers::enum_range<Nature>(),
		[=](Nature const nature) {
			if (!is_physical) {
				auto const lowers = lowers_stat(nature, RegularStat::atk);
				return !is_special ? lowers and !boosts_stat(nature, RegularStat::spa) : lowers;
			}
			if (!is_special) {
				return lowers_stat(nature, RegularStat::spa);
			}
			return true;
		}
	);
}

template<Generation generation>
constexpr auto target_stat(RegularStat const stat_name, auto const base_stat, Level const level, OffensiveEVInputs<generation> const input, Nature const harmful_nature) {
	return input.include_evs ? input.stat : initial_stat<generation>(stat_name, base_stat, level, harmful_nature, input.iv, EV(0_bi));
}

template<Generation generation>
auto evs_for_nature(BaseStats const base, Level const level, OffensiveEVInputs<generation> const atk, OffensiveEVInputs<generation> const spa) {
	auto const target_atk = target_stat(RegularStat::atk, base.atk(), level, atk, Nature::Modest);
	auto const target_spa = target_stat(RegularStat::spa, base.spa(), level, spa, Nature::Adamant);
	return [=](Nature const nature) {
		auto const atk_ev = stat_to_ev(target_atk, nature, RegularStat::atk, base.atk(), atk.iv, level);
		auto const spa_ev = stat_to_ev(target_spa, nature, RegularStat::spa, base.spa(), spa.iv, level);
		return BOUNDED_CONDITIONAL(atk_ev and spa_ev, (OffensiveStats{nature, {atk.iv, *atk_ev}, {spa.iv, *spa_ev}}), bounded::none);
	};
}

constexpr auto cat_optionals(containers::range auto && input) {
	return containers::transform(
		containers::filter(input, [](auto const & value) { return static_cast<bool>(value); }),
		[](auto && value) { return *OPERATORS_FORWARD(value); }
	);
}

} // namespace detail

struct OffensiveEVs {
	template<Generation generation>
	OffensiveEVs(BaseStats const base, Level const level, OffensiveEVInputs<generation> const atk, OffensiveEVInputs<generation> const spa):
		m_container(
			detail::cat_optionals(
				containers::transform(
					detail::useful_natures(atk.include_evs, spa.include_evs),
					detail::evs_for_nature(base, level, atk, spa)
				)
			)
		)
	{
	}

	auto find(Nature const nature) const -> OffensiveStats const * {
		return containers::maybe_find_if(m_container, [=](auto const value) { return value.nature == nature; });
	}

private:
	containers::static_vector<OffensiveStats, containers::size(containers::enum_range<Nature>()).value()> m_container;
};

}	// namespace technicalmachine
