// Determine when transpositions occur
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include <map>
#include <string>
#include "evaluate.h"
#include "expectiminimax.h"
#include "move.h"
#include "pokemon.h"
#include "state.h"
#include "team.h"
#include "transposition.h"
#include "weather.h"

namespace technicalmachine {

// This currently generates no hits. I'm currently working on this to find out why.

long transposition (Team &ai, Team &foe, const Weather &weather, const int &depth, const score_variables &sv, moves_list &best_move, std::string &output, std::map<long, State> &transposition_table) {
	reset_iterators (ai);	
	reset_iterators (foe);
	State state;		// This causes possibly needless copying. Changing my code elsewhere to use the State data structure could solve this problem.

	if (depth == 0)
		state.score = evaluate (ai, foe, weather, sv);
	else {
		state.ai = ai;
		state.foe = foe;
		state.weather = weather;
		state.depth = depth;
		const long hash = hash_state (state, sv);
		std::map<long, State>::iterator it = transposition_table.find (hash);
		// If I can find the current state in my transposition table at a depth of at least the current depth, set the score to the stored score.
		if (it != transposition_table.end() and it->second.depth >= depth) {
//			std::cout << "Need for speed\n";
			state.score = it->second.score;
		}
		else {
			// If I can't find it, set the score to the evaluation of the state at depth - 1.
			state.score = tree1 (state.ai, state.foe, state.weather, state.depth, sv, best_move, output, transposition_table);
			
			// If I didn't find any stored value at the same hash as the current state, add it to my table. If the value I found was for a search at a shallower depth, replace it with the new value.
			if (it == transposition_table.end() or state.depth >= it->second.depth) {
				transposition_table [hash] = state;
			}
		}
	}
	return state.score;
}

long hash_state (const State &state, const score_variables &sv) {
	return (hash_team (state.ai) + 7 * hash_team (state.foe) + hash_weather (state.weather)) % sv.transposition_table;
}

long hash_team (const Team &team) {
	long hash = team.active->name + team.light_screen + team.lucky_chant + team.mist + team.reflect + team.safeguard + team.tailwind + team.wish + team.spikes + team.toxic_spikes + team.stealth_rock;
	for (std::vector<pokemon>::const_iterator it = team.member.begin(); it != team.member.end(); ++it)
		hash += hash_pokemon (*it);
	return hash;
}

long hash_pokemon (const Pokemon &member) {
	long hash = 1234 * member.name + member.status + member.vanish + member.confused + member.embargo + member.encore + member.heal_block + member.magnet_rise + member.partial_trap + member.perish_song + member.rampage + member.sleep + member.slow_start + member.stockpile + member.taunt + member.toxic + member.uproar + member.yawn + member.aqua_ring + member.attract + member.charge + member.curse + member.damaged + member.defense_curl + member.destiny_bond + member.	ff + member.flinch + member.focus_energy + member.identified + member.imprison + member.ingrain + member.item_recycle + member.item_unburden + member.leech_seed + member.loaf + member.lock_on + member.mf + member.minimize + member.moved + member.mud_sport + member.nightmare + member.roost + member.torment + member.trapped + member.water_sport + member.accuracy + member.evasion + member.hp.stat + member.atk.stage + member.def.stage + member.spa.stage + member.spd.stage + member.spe.stage + member.ability + member.item + member.nature;
	for (std::vector<Move>::const_iterator it = member.moveset.begin(); it != member.moveset.end(); ++it)
		hash += hash_move (*it);
	return hash;
}

long hash_move (const Move &move) {
	return move.name + move.disable + move.pp + move.times_used;
}

long hash_weather (const Weather &weather) {
	return weather.trick_room + weather.fog + weather.gravity + weather.uproar + weather.hail + weather.sun + weather.sand + weather.rain;
}

}
