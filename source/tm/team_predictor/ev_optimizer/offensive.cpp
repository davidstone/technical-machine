// Optimize offensive EVs and nature to remove waste
// Copyright (C) 2018 David Stone
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

#include <tm/team_predictor/ev_optimizer/offensive.hpp>

#include <tm/pokemon/pokemon.hpp>

#include <tm/stat/base_stats.hpp>
#include <tm/stat/calculate.hpp>
#include <tm/stat/nature.hpp>
#include <tm/stat/stat.hpp>
#include <tm/stat/stat_to_ev.hpp>

#include <bounded/assert.hpp>
#include <bounded/integer.hpp>
#include <bounded/optional.hpp>

#include <containers/algorithms/filter_iterator.hpp>
#include <containers/algorithms/maybe_find.hpp>
#include <containers/algorithms/transform.hpp>

namespace technicalmachine {

auto OffensiveEVs::find(Nature const nature) const -> OffensiveStats const * {
	return containers::maybe_find_if(m_container, [=](auto const value) { return value.nature == nature; });
}

namespace {

using namespace bounded::literal;

// If I don't have a physical move, prefer to lower that because it lowers
// confusion damage. If I do have a physical move but no special move, prefer
// to lower Special Attack because it is the only remaining stat guaranteed to
// be unused. This allows me to maximize Speed and the defensive stats.
constexpr auto useful_natures(bool const is_physical, bool const is_special) {
	return containers::filter(
		containers::enum_range<Nature>(),
		[=](Nature const nature) {
			if (!is_physical) {
				auto const lowers = lowers_stat(nature, StatNames::ATK);
				return !is_special ? lowers and !boosts_stat(nature, StatNames::SPA) : lowers;
			}
			if (!is_special) {
				return lowers_stat(nature, StatNames::SPA);
			}
			return true;
		}
	);
}

constexpr auto target_stat(StatNames const stat_name, auto const base_stat, Level const level, OffensiveEVs::Input const input, Nature const harmful_nature) {
	return input.include_evs ? input.stat : initial_stat(stat_name, base_stat, input.iv, EV(0_bi), level, harmful_nature);
}

auto evs_for_nature(BaseStats const base, Level const level, OffensiveEVs::Input const atk, OffensiveEVs::Input const spa) {
	auto const target_atk = target_stat(StatNames::ATK, base.atk(), level, atk, Nature::Modest);
	auto const target_spa = target_stat(StatNames::SPA, base.spa(), level, spa, Nature::Adamant);
	return [=](Nature const nature) {
		auto const atk_ev = stat_to_ev(target_atk, nature, StatNames::ATK, base.atk(), atk.iv, level);
		auto const spa_ev = stat_to_ev(target_spa, nature, StatNames::SPA, base.spa(), spa.iv, level);
		return BOUNDED_CONDITIONAL(atk_ev and spa_ev, (OffensiveStats{nature, {atk.iv, *atk_ev}, {spa.iv, *spa_ev}}), bounded::none);
	};
}

constexpr auto cat_optionals(containers::range auto && input) {
	return containers::transform(
		containers::filter(input, [](auto const & value) { return static_cast<bool>(value); }),
		[](auto && value) { return *OPERATORS_FORWARD(value); }
	);
}

} // namespace

OffensiveEVs::OffensiveEVs(BaseStats const base, Level const level, Input const atk, Input const spa):
	m_container(
		cat_optionals(
			containers::transform(
				useful_natures(atk.include_evs, spa.include_evs),
				evs_for_nature(base, level, atk, spa)
			)
		)
	)
{
}

} // namespace technicalmachine
