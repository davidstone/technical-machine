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
#include <bounded/optional.hpp>

#include <containers/algorithms/filter_iterator.hpp>
#include <containers/algorithms/maybe_find.hpp>
#include <containers/emplace_back.hpp>

namespace technicalmachine {

auto OffensiveEVs::find(Nature const nature) const -> OffensiveStats const * {
	return containers::maybe_find_if(m_container, [=](auto const value) { return value.nature == nature; });
}

struct OffensiveEVs::OffensiveData {
private:
	using StatType = decltype(initial_stat(std::declval<StatNames>(), std::declval<Stat>(), std::declval<Level>(), std::declval<Nature>()));
public:
	StatType atk;
	StatType spa;
};

namespace {

using namespace bounded::literal;

auto ideal_attack_stat(Stat const original_stat, Level const level, Nature const original_nature, bool const is_physical) {
	// All we care about on this nature is the boost to Attack
	auto const nature = is_physical ? original_nature : Nature::Modest;
	auto const stat = is_physical ? original_stat : Stat(original_stat, original_stat.iv(), EV(0_bi));
	return initial_stat(StatNames::ATK, stat, level, nature);
}
auto ideal_special_attack_stat(Stat const original_stat, Level const level, Nature const original_nature, bool const is_special, bool const is_physical) {
	// All we care about on this nature is the boost to Special Attack
	auto const nature =
		is_special ? original_nature :
		is_physical ? Nature::Adamant :
		Nature::Hardy;
	auto const stat = is_special ? original_stat : Stat(original_stat, original_stat.iv(), EV(0_bi));
	return initial_stat(StatNames::SPA, stat, level, nature);
}

// If I don't have a physical move, prefer to lower that because it lowers
// confusion damage. If I do have a physical move but no special move, prefer
// to lower Special Attack because it is the only remaining stat guaranteed to
// be unused. This allows me to maximize Speed and the defensive stats.
auto is_useful_nature(bool const is_physical, bool const is_special) {
	return [=](Nature const nature) {
		if (!is_physical) {
			auto const lowers = lowers_stat(nature, StatNames::ATK);
			return !is_special ? lowers and !boosts_stat(nature, StatNames::SPA) : lowers;
		}
		if (!is_special) {
			return lowers_stat(nature, StatNames::SPA);
		}
		return true;
	};
}

} // namespace

OffensiveEVs::OffensiveEVs(Generation const generation, Species const species, Level const level, Nature const original_nature, Stat const attack, Stat const special_attack, bool const include_attack_evs, bool const include_special_attack_evs):
	m_container(containers::transform(
		containers::filter(containers::enum_range<Nature>(), is_useful_nature(include_attack_evs, include_special_attack_evs)),
		[](Nature const nature) { return OffensiveStats(nature); }
	))
{
	BOUNDED_ASSERT(!empty(m_container));

	auto const stats = OffensiveData{
		ideal_attack_stat(attack, level, original_nature, include_attack_evs),
		ideal_special_attack_stat(special_attack, level, original_nature, include_special_attack_evs, include_attack_evs)
	};

	equal_stats(generation, stats, species, level);
}

void OffensiveEVs::equal_stats(Generation const generation, OffensiveData const initial, Species const species, Level const level) {
	auto const base_stats = BaseStats(generation, species);
	for (auto it = begin(m_container); it != end(m_container);) {
		auto const nature = it->nature;
		auto const atk_ev = stat_to_ev(initial.atk, nature, StatNames::ATK, base_stats.atk(), default_iv(generation), level);
		auto const spa_ev = stat_to_ev(initial.spa, nature, StatNames::SPA, base_stats.spa(), default_iv(generation), level);
		if (atk_ev and spa_ev) {
			it->attack = EV(*atk_ev);
			it->special_attack = EV(*spa_ev);
			++it;
		} else {
			it = erase(m_container, it);
		}
	}
	BOUNDED_ASSERT(!empty(m_container));
}

}	// namespace technicalmachine
