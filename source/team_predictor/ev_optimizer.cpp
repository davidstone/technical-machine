// Optimize EVs and nature to remove waste
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

#include "ev_optimizer.hpp"
#include "../pokemon/pokemon.hpp"

namespace technicalmachine {
namespace {
void remove_unused_offensive_evs(Pokemon & pokemon, bool & lower_attack, bool & lower_special_attack);
bool has_physical_move(Pokemon const & pokemon);
bool has_special_move(Pokemon const & pokemon);
}	// unnamed namespace

void optimize_evs(Pokemon & pokemon) {
	bool lower_attack = false;
	bool lower_special_attack = false;
	remove_unused_offensive_evs(pokemon, lower_attack, lower_special_attack);
	unsigned const physical_product = pokemon.hp.points() * pokemon.def.points();
	unsigned const special_product = pokemon.hp.points() * pokemon.spd.points();
}

namespace {

void remove_unused_offensive_evs(Pokemon & pokemon, bool & lower_attack, bool & lower_special_attack) {
	if (!has_physical_move(pokemon)) {
		pokemon.atk.ev.set_value(0);
		lower_attack = true;
	}
	if (!has_special_move(pokemon)) {
		pokemon.spa.ev.set_value(0);
		lower_special_attack = true;
	}
}

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
