// Optimize offensive EVs and nature to remove waste
// Copyright (C) 2013 David Stone
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

#include <vector>

#include <bounded_integer/optional.hpp>

#include "../../pokemon/pokemon.hpp"
#include "../../stat/nature.hpp"
#include "../../stat/stat.hpp"

namespace technicalmachine {
namespace {

using namespace bounded_integer::literal;
bool has_physical_move(Pokemon const & pokemon);
bool has_special_move(Pokemon const & pokemon);

template<StatNames stat>
bounded_integer::optional<EV::value_type> reset_stat(Pokemon const & pokemon, EV & ev, unsigned const initial) {
	EV::value_type ev_estimate = 0_bi;
	ev = EV(ev_estimate);
	while (initial_stat<stat>(pokemon) < initial) {
		ev_estimate += 4_bi;
		ev = EV(ev_estimate);
		if (ev_estimate == bounded_integer::make_bounded<EV::max>()) {
			return bounded_integer::none;
		}
	}
	return bounded_integer::optional<EV::value_type>(ev_estimate);
}

}	// namespace

OffensiveEVs::OffensiveEVs(Pokemon pokemon) {
	for (Nature::Natures nature = static_cast<Nature::Natures>(0); nature != Nature::END; nature = static_cast<Nature::Natures>(nature + 1)) {
		container.insert(Container::value_type(nature, OffensiveStats{}));
	}
	optimize(pokemon);
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
	unsigned const initial_atk = initial_stat<StatNames::ATK>(pokemon);
	unsigned const initial_spa = initial_stat<StatNames::SPA>(pokemon);
	for (auto it = std::begin(container); it != std::end(container);) {
		OffensiveStats & stats = it->second;
		get_nature(pokemon) = it->first;
		auto const atk = reset_stat<StatNames::ATK>(pokemon, get_stat(pokemon, StatNames::ATK).ev, initial_atk);
		auto const spa = reset_stat<StatNames::SPA>(pokemon, get_stat(pokemon, StatNames::SPA).ev, initial_spa);
		if (atk and spa) {
			stats.attack = EV(*atk);
			stats.special_attack = EV(*spa);
			++it;
		}
		else {
			it = container.erase(it);
		}
	}
}

namespace {

bool has_physical_move(Pokemon const & pokemon) {
	return pokemon.move.regular_move_exists([](Move const & move) {
		return is_physical(move);
	});
}

bool has_special_move(Pokemon const & pokemon) {
	return pokemon.move.regular_move_exists([](Move const & move) {
		return is_special(move);
	});
}

}	// unnamed namespace
}	// namespace technicalmachine
