// Transposition header
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef TRANSPOSITION_H_
#define TRANSPOSITION_H_

#include <map>
#include <string>
#include "evaluate.h"
#include "move.h"
#include "pokemon.h"
#include "state.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

long transposition (Team &ai, Team &foe, Weather const &weather, int const &depth, score_variables const &sv, moves_list &best_move, std::string &output, std::map<long, State> &transposition_table);
long hash_state (State const &state, score_variables const &sv);
long hash_team (Team const &team);
long hash_pokemon (Pokemon const &member);
long hash_move (Move const &move);
long hash_weather (Weather const &weather);

}
#endif
