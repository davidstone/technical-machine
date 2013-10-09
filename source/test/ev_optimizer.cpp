// Test EV optimizer
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
#include <iostream>
#include <random>
#include "../team.hpp"
#include "../pokemon/species.hpp"
#include "../team_predictor/ev_optimizer/ev_optimizer.hpp"
#include "../move/moves.hpp"

namespace technicalmachine {

void ev_optimizer_tests() {
	std::cout << "\tEV optimizer tests.\n";

	Team team;
	constexpr unsigned level = 100;
	Gender const gender(Gender::MALE);
	team.add_pokemon(Species::Snorlax, level, gender);
	Pokemon & pokemon = team.pokemon();
	pokemon.stat(Stat::HP).ev.set_value(128);
	for (auto const stat : { Stat::ATK, Stat::DEF, Stat::SPA, Stat::SPD, Stat::SPE }) {
		pokemon.stat(stat).ev.set_value(76);
	}
	pokemon.nature() = Nature::HARDY;
	calculate_initial_hp(pokemon);
	pokemon.move.add(Moves::Psychic, 3);
	pokemon.move.add(Moves::Earthquake, 3);
//	std::cerr << "Before: " << '\n' << team.to_string(false) << '\n';
	minimize_evs(team.pokemon());
//	std::cerr << "After: " << '\n' << team.to_string(false) << '\n';
	std::random_device rd;
	std::mt19937 random_engine(rd());
	pad_random_evs(team.pokemon(), random_engine);
//	std::cerr << "Final: " << '\n' << team.to_string(false) << '\n';
}

}	// namespace technicalmachine
