// Optimize offensive EVs and nature to remove waste
// Copyright (C) 2012 David Stone
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

#include <boost/optional.hpp>

#include "../../pokemon/pokemon.hpp"
#include "../../stat/nature.hpp"
#include "../../stat/stat.hpp"

namespace technicalmachine {

namespace {
enum NatureBoost { Penalty, Neutral, Boost };

template<Stat::Stats stat>
Nature::Natures nature_effect(NatureBoost nature);

bool has_physical_move(Pokemon const & pokemon);
bool has_special_move(Pokemon const & pokemon);


template<Stat::Stats stat>
boost::optional<unsigned> reset_stat(Pokemon & pokemon, EV & ev, unsigned initial) {
	constexpr unsigned max_ev = 252;
	unsigned ev_estimate = 0;
	ev.set_value(ev_estimate);
	while (initial_stat<stat>(pokemon) < initial) {
		ev_estimate += 4;
		if (ev_estimate > max_ev)
			break;
		ev.set_value(ev_estimate);
	}
	return (ev_estimate <= max_ev) ? boost::optional<unsigned>(ev_estimate) : boost::optional<unsigned>();
}

}	// unnamed namespace

OffensiveEVs::OffensiveEVs(Pokemon pokemon):
	container({
		{ Nature::BRAVE, OffensiveStats() },
		{ Nature::ADAMANT, OffensiveStats() },
		{ Nature::QUIET, OffensiveStats() },
		{ Nature::HARDY, OffensiveStats() },
		{ Nature::IMPISH, OffensiveStats() },
		{ Nature::MODEST, OffensiveStats() },
		{ Nature::CALM, OffensiveStats() }
	}) {
	optimize(pokemon);
}

void OffensiveEVs::optimize(Pokemon & pokemon) {
	remove_unused(pokemon);
	equal_stats(pokemon);
}

void OffensiveEVs::remove_unused(Pokemon const & pokemon) {
	// If I don't have a physical move, prefer to lower that because it lowers
	// confusion damage.
	//
	// If I don't have a physical or special move, then I want
	// Impish (-Atk, =SpA).
	//
	// If I have a physical move but no special move, then I want
	// Adamant (+Atk, -SpA) or Impish (=Atk, -SpA).
	//
	// If I have a special move but no physical move, then I want
	// Modest (-Atk, +SpA) or Calm (-Atk, =SpA).
	bool const is_physical = has_physical_move(pokemon);
	if (!is_physical) {
		for (auto const nature : { Nature::BRAVE, Nature::ADAMANT, Nature::QUIET, Nature::HARDY, Nature::IMPISH })
			container.erase(nature);
		for (auto & remainder : container)
			remainder.second.attack = 0;
	}
	if (!has_special_move(pokemon)) {
		for (auto const nature : { Nature::BRAVE, Nature::QUIET, Nature::HARDY, Nature::MODEST })
			container.erase(nature);
		if (is_physical) {
			container.erase(Nature::CALM);
		}
		for (auto & remainder : container)
			remainder.second.special_attack = 0;
	}
}

void OffensiveEVs::equal_stats(Pokemon & pokemon) {
	unsigned const initial_atk = initial_stat<Stat::ATK>(pokemon);
	unsigned const initial_spa = initial_stat<Stat::SPA>(pokemon);
	for (auto it = std::begin(container); it != std::end(container);) {
		OffensiveStats & stats = it->second;
		pokemon.nature() = it->first;
		boost::optional<unsigned> const atk = reset_stat<Stat::ATK>(pokemon, pokemon.atk().ev, initial_atk);
		boost::optional<unsigned> const spa = reset_stat<Stat::SPA>(pokemon, pokemon.spa().ev, initial_spa);
		if (atk and spa) {
			stats.attack = *atk;
			stats.special_attack = *spa;
			++it;
		}
		else {
			container.erase(it++);
		}
	}
}

namespace {

bool has_physical_move(Pokemon const & pokemon) {
	return pokemon.move.regular_move_exists([](Move const & move) {
		return move.is_physical();
	});
}

bool has_special_move(Pokemon const & pokemon) {
	return pokemon.move.regular_move_exists([](Move const & move) {
		return move.is_special();
	});
}

}	// unnamed namespace
}	// namespace technicalmachine
