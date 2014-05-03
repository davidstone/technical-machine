// Optimize offensive EVs and nature to remove waste
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

#include "offensive.hpp"

#include "../../pokemon/pokemon.hpp"
#include "../../stat/calculate.hpp"
#include "../../stat/nature.hpp"
#include "../../stat/stat.hpp"

#include <bounded_integer/optional.hpp>

#include <cassert>
#include <vector>

namespace technicalmachine {
namespace {

using namespace bounded::literal;
bool has_physical_move(Pokemon const & pokemon);
bool has_special_move(Pokemon const & pokemon);

template<StatNames stat_name, typename Initial>
bounded::optional<EV::value_type> reset_stat(Stat & stat, Level const level, Nature const nature, Initial const initial) {
	EV::value_type ev_estimate = 0_bi;
	EV & ev = stat.ev;
	ev = EV(ev_estimate);
	while (initial_stat<stat_name>(stat, level, nature) < initial) {
		ev_estimate += 4_bi;
		ev = EV(ev_estimate);
		if (ev_estimate == bounded::make<EV::max>()) {
			return bounded::none;
		}
	}
	return bounded::optional<EV::value_type>(ev_estimate);
}

}	// namespace

OffensiveEVs::OffensiveEVs(Pokemon pokemon) {
	for (Nature::Natures nature = static_cast<Nature::Natures>(0); nature != Nature::END; nature = static_cast<Nature::Natures>(nature + 1)) {
		container.insert(Container::value_type(nature, OffensiveStats{}));
	}
	optimize(pokemon);
	assert(!container.empty());
}

void OffensiveEVs::optimize(Pokemon & pokemon) {
	remove_unused(pokemon);
	equal_stats(pokemon);
}

namespace {

template<typename Container, typename Condition>
void remove_individual_unused(Container & container, Condition const & condition) {
	for (auto it = std::begin(container); it != std::end(container);) {
		if (condition(it)) {
			it = container.erase(it);
		}
		else {
			++it;
		}
	}
}

}	// namespace

void OffensiveEVs::remove_unused(Pokemon & pokemon) {
	// If I don't have a physical move, prefer to lower that because it lowers
	// confusion damage. If I do have a physical move but no special move,
	// prefer to lower Special Attack because it is the only remaining stat
	// guaranteed to be unused. This allows me to maximize Speed and the
	// defensive stats.
	bool const is_physical = has_physical_move(pokemon);
	if (!is_physical) {
		remove_individual_unused(container, [](Container::const_iterator it) {
			return !Nature(it->first).lowers_stat<StatNames::ATK>();
		});
		get_stat(pokemon, StatNames::ATK).ev = EV(0_bi);
	}
	bool const is_special = has_special_move(pokemon);
	if (!is_special) {
		if (is_physical) {
			remove_individual_unused(container, [](Container::const_iterator it) {
				return !Nature(it->first).lowers_stat<StatNames::SPA>();
			});
		}
		else {
			remove_individual_unused(container, [](Container::const_iterator it) {
				return Nature(it->first).boosts_stat<StatNames::SPA>();
			});
		}
		get_stat(pokemon, StatNames::SPA).ev = EV(0_bi);
	}
	if (!is_physical and !is_special) {
		get_nature(pokemon).name = Nature::CALM;
	}
	else if (!is_physical and get_nature(pokemon).boosts_stat<StatNames::SPA>()) {
		get_nature(pokemon).name = Nature::MODEST;
	}
	else if (!get_nature(pokemon).boosts_stat<StatNames::ATK>() and !is_special) {
		get_nature(pokemon).name = Nature::IMPISH;
	}
}

void OffensiveEVs::equal_stats(Pokemon & pokemon) {
	Stat & attack = get_stat(pokemon, StatNames::ATK);
	Stat & special_attack = get_stat(pokemon, StatNames::SPA);
	Level const level = get_level(pokemon);
	Nature & nature = get_nature(pokemon);
	auto const initial_atk = initial_stat<StatNames::ATK>(attack, level, nature);
	auto const initial_spa = initial_stat<StatNames::SPA>(special_attack, level, nature);
	for (auto it = std::begin(container); it != std::end(container);) {
		OffensiveStats & stats = it->second;
		nature = it->first;
		auto const atk_ev = reset_stat<StatNames::ATK>(attack, level, nature, initial_atk);
		auto const spa_ev = reset_stat<StatNames::SPA>(special_attack, level, nature, initial_spa);
		if (atk_ev and spa_ev) {
			stats.attack = EV(*atk_ev);
			stats.special_attack = EV(*spa_ev);
			++it;
		}
		else {
			it = container.erase(it);
		}
	}
}

namespace {

template<typename Predicate>
bool has_move(Pokemon const & pokemon, Predicate predicate) {
	auto const & moves = pokemon.move;
	return std::find_if(moves.regular().begin(), moves.regular().end(), predicate) != moves.regular().end();
}

bool has_physical_move(Pokemon const & pokemon) {
	return has_move(pokemon, is_physical);
}

bool has_special_move(Pokemon const & pokemon) {
	return has_move(pokemon, is_special);
}

}	// unnamed namespace
}	// namespace technicalmachine
