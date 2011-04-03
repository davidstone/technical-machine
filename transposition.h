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

#include "evaluate.h"
#include "state.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

long transposition (teams &ai, teams &foe, const weathers &weather, const int &depth, const score_variables &sv, moves_list &best_move, std::string &output, std::map<long, State> &transposition_table);
long hash_state (const State &state, const score_variables &sv);
long hash_team (const teams &team);
long hash_pokemon (const pokemon &member);
long hash_move (const moves &move);
long hash_weather (const weathers &weather);

}
#endif
