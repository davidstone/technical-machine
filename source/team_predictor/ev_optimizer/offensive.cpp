// Optimize offensive EVs and nature to remove waste
// Copyright (C) 2015 David Stone
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

#include "offensive.hpp"

#include "../../enum_range.hpp"
#include "../../pokemon/pokemon.hpp"
#include "../../stat/calculate.hpp"
#include "../../stat/nature.hpp"
#include "../../stat/stat.hpp"

#include <bounded/optional.hpp>

#include <containers/algorithms/all_any_none.hpp>

#include <cassert>

namespace technicalmachine {
namespace {

using namespace bounded::literal;
bool has_physical_move(Pokemon const & pokemon);
bool has_special_move(Pokemon const & pokemon);

template<StatNames stat_name, typename Initial>
auto find_least_stat(Species const species, Level const level, Nature const nature, Initial const initial) -> bounded::optional<EV::value_type> {
	EV::value_type ev = 0_bi;
	Stat stat(species, stat_name, EV(ev));
	auto const test_stat = [&]() { return initial_stat<stat_name>(stat, level, nature); };
	while (test_stat() < initial) {
		ev += 4_bi;
		stat = Stat(stat, EV(ev));
		if (ev == EV::max) {
			break;
		}
	}
	return (test_stat() < initial) ? bounded::none : bounded::optional<EV::value_type>(ev);
}

}	// namespace

OffensiveEVs::OffensiveEVs(Pokemon const & pokemon) {
	for (auto const nature : enum_range<Nature>) {
		container.emplace(nature, OffensiveStats{});
	}
	optimize(pokemon);
}

namespace {

auto ideal_attack_stat(Pokemon const & pokemon, bool const is_physical) {
	// All we care about on this nature is the boost to Attack
	auto const nature = is_physical ? get_nature(pokemon) : Nature::Modest;
	Stat const stat(pokemon, StatNames::ATK, is_physical ? get_stat(pokemon, StatNames::ATK).ev() : EV(0_bi));
	return initial_stat<StatNames::ATK>(stat, get_level(pokemon), nature);
}
auto ideal_special_attack_stat(Pokemon const & pokemon, bool const is_special, bool const is_physical) {
	// All we care about on this nature is the boost to Special Attack
	auto const nature =
		is_special ? get_nature(pokemon) :
		is_physical ? Nature::Adamant :
		Nature::Hardy;
	Stat const stat(pokemon, StatNames::SPA, is_special ? get_stat(pokemon, StatNames::SPA).ev() : EV(0_bi));
	return initial_stat<StatNames::SPA>(stat, get_level(pokemon), nature);
}

template<typename Container, typename Condition>
void remove_individual_unused(Container & container, Condition const & condition) {
	for (auto it = begin(container); it != end(container);) {
		if (condition(it)) {
			it = container.erase(it);
		}
		else {
			++it;
		}
	}
	assert(!container.empty());
}

template<typename Container>
auto remove_inferior_natures(Container & container, bool const is_physical, bool const is_special) {
	auto const does_not_lower_attack = [](auto const iter) {
		return !lowers_stat<StatNames::ATK>(iter->first);
	};
	auto const does_not_lower_special_attack = [](auto const iter) {
		return !lowers_stat<StatNames::SPA>(iter->first);
	};
	auto const boosts_special_attack = [](auto const iter) {
		return boosts_stat<StatNames::SPA>(iter->first);
	};
	if (!is_physical) {
		remove_individual_unused(container, does_not_lower_attack);
	}
	if (!is_special) {
		if (is_physical) {
			remove_individual_unused(container, does_not_lower_special_attack);
		} else {
			remove_individual_unused(container, boosts_special_attack);
		}
	}
}

}	// namespace

void OffensiveEVs::optimize(Pokemon const & pokemon) {
	// If I don't have a physical move, prefer to lower that because it lowers
	// confusion damage. If I do have a physical move but no special move,
	// prefer to lower Special Attack because it is the only remaining stat
	// guaranteed to be unused. This allows me to maximize Speed and the
	// defensive stats.
	bool const is_physical = has_physical_move(pokemon);
	bool const is_special = has_special_move(pokemon);
	
	remove_inferior_natures(container, is_physical, is_special);
	
	OffensiveData const stats{ideal_attack_stat(pokemon, is_physical), ideal_special_attack_stat(pokemon, is_special, is_physical)};

	equal_stats(stats, pokemon, get_level(pokemon));
}

void OffensiveEVs::equal_stats(OffensiveData const initial, Species const species, Level const level) {
	for (auto it = begin(container); it != end(container);) {
		auto const nature = it->first;
		auto const atk_ev = find_least_stat<StatNames::ATK>(species, level, nature, initial.atk);
		auto const spa_ev = find_least_stat<StatNames::SPA>(species, level, nature, initial.spa);
		if (atk_ev and spa_ev) {
			OffensiveStats & stats = it->second;
			stats.attack = EV(*atk_ev);
			stats.special_attack = EV(*spa_ev);
			++it;
		}
		else {
			it = container.erase(it);
		}
	}
	assert(!container.empty());
}

namespace {

template<typename Predicate>
bool has_move(Pokemon const & pokemon, Predicate predicate) {
	auto const & moves = regular_moves(pokemon);
	return containers::any_of(begin(moves), end(moves), predicate);
}

bool has_physical_move(Pokemon const & pokemon) {
	return has_move(pokemon, is_physical);
}

bool has_special_move(Pokemon const & pokemon) {
	return has_move(pokemon, is_special);
}

}	// namespace
}	// namespace technicalmachine
