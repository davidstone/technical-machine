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
#include <tm/stat/calculate.hpp>
#include <tm/stat/nature.hpp>
#include <tm/stat/stat.hpp>

#include <bounded/assert.hpp>
#include <bounded/optional.hpp>

namespace technicalmachine {
namespace {

using namespace bounded::literal;

template<StatNames stat_name, typename Initial>
auto find_least_stat(Species const species, Level const level, Nature const nature, Initial const initial) -> bounded::optional<EV::value_type> {
	EV::value_type ev = 0_bi;
	auto stat = Stat(species, stat_name, EV(ev));
	auto const test_stat = [&]() { return initial_stat(stat_name, stat, level, nature); };
	while (test_stat() < initial) {
		ev += 4_bi;
		stat = Stat(stat, EV(ev));
		if (ev == EV::max) {
			break;
		}
	}
	return (test_stat() < initial) ? bounded::none : bounded::optional<EV::value_type>(ev);
}

auto ideal_attack_stat(Stat const original_stat, Level const level, Nature const original_nature, bool const is_physical) {
	// All we care about on this nature is the boost to Attack
	auto const nature = is_physical ? original_nature : Nature::Modest;
	auto const stat = is_physical ? original_stat : Stat(original_stat, EV(0_bi));
	return initial_stat(StatNames::ATK, stat, level, nature);
}
auto ideal_special_attack_stat(Stat const original_stat, Level const level, Nature const original_nature, bool const is_special, bool const is_physical) {
	// All we care about on this nature is the boost to Special Attack
	auto const nature =
		is_special ? original_nature :
		is_physical ? Nature::Adamant :
		Nature::Hardy;
	auto const stat = is_special ? original_stat : Stat(original_stat, EV(0_bi));
	return initial_stat(StatNames::SPA, stat, level, nature);
}

template<typename Container>
auto remove_inferior_natures(Container & container, bool const is_physical, bool const is_special) {
	if (!is_physical) {
		containers::erase_if(container, [](auto candidate) { return !lowers_stat(candidate.nature, StatNames::ATK); });
	}
	if (!is_special) {
		if (is_physical) {
			containers::erase_if(container, [](auto candidate) { return !lowers_stat(candidate.nature, StatNames::SPA); });
		} else {
			containers::erase_if(container, [](auto candidate) { return boosts_stat(candidate.nature, StatNames::SPA); });
		}
	}
	BOUNDED_ASSERT(!empty(container));
}

}	// namespace

OffensiveEVs::OffensiveEVs(Species const species, Level const level, Nature const original_nature, Stat const attack, Stat const special_attack, bool const include_attack_evs, bool const include_special_attack_evs) {
	for (auto const nature : containers::enum_range<Nature>()) {
		m_container.emplace_back(nature);
	}
	// If I don't have a physical move, prefer to lower that because it lowers
	// confusion damage. If I do have a physical move but no special move,
	// prefer to lower Special Attack because it is the only remaining stat
	// guaranteed to be unused. This allows me to maximize Speed and the
	// defensive stats.
	remove_inferior_natures(m_container, include_attack_evs, include_special_attack_evs);

	auto const stats = OffensiveData{
		ideal_attack_stat(attack, level, original_nature, include_attack_evs),
		ideal_special_attack_stat(special_attack, level, original_nature, include_special_attack_evs, include_attack_evs)
	};

	equal_stats(stats, species, level);
}

void OffensiveEVs::equal_stats(OffensiveData const initial, Species const species, Level const level) {
	for (auto it = begin(m_container); it != end(m_container);) {
		auto const nature = it->nature;
		auto const atk_ev = find_least_stat<StatNames::ATK>(species, level, nature, initial.atk);
		auto const spa_ev = find_least_stat<StatNames::SPA>(species, level, nature, initial.spa);
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
