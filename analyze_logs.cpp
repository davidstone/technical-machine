// Analyze Shoddy Battle logs
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <boost/lexical_cast.hpp>
#include <iostream>
#include <map>
#include <string>
#include "analyze_logs.h"
#include "ability.h"
#include "move.h"
#include "movefunction.h"
#include "pokemon.h"
#include "switch.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

void analyze_turn (Team &ai, Team &foe, Team* &first, Team* &last, Weather &weather, const Map &map) {
	first = NULL;		// Only check if first == NULL, so no need to set last = NULL
	std::cout << "Enter the log for the turn, followed by a ~.\n";
	std::string input;
	getline (std::cin, input, '~');		// Need to find a better way to signifiy end-of-turn. This works for now.
	std::cout << "======================\nAnalyzing...\n";
	size_t newline1 = 0;
	size_t newline2 = input.find ('\n', newline1 + 1);
	while (newline2 != std::string::npos) {
		std::string line = input.substr (newline1, newline2 - newline1);
		analyze_line (ai, foe, first, weather, line, map);
		newline1 = newline2 + 1;
		newline2 = input.find ('\n', newline1 + 1);
	}
	if (first->me)
		last = &foe;
	else
		last = &ai;
}

void analyze_line (Team &ai, Team &foe, Team* &ordering, Weather &weather, const std::string &line, const Map &map) {
	if (line.find(": ") == std::string::npos) {		// Should ignore all comments, hopefully nobody puts : anywhere in their names
		std::string search = " sent out ";
		size_t found = line.find (search);
		if (found != std::string::npos) {
			search = ai.player + " sent out ";
			if (line.substr (0, search.length()) == search) {
				log_pokemon (ai, *foe.active, weather, line, map, search);
				if (ordering == NULL)
					ordering = &ai;
			}
			else {
				if (foe.player == "")
					foe.player = line.substr (0, found);
				search = foe.player + " sent out ";
				log_pokemon (foe, *ai.active, weather, line, map, search);
				if (ordering == NULL)
					ordering = &foe;
			}
		}
		else {
			// name switched in nickname (lvl x species ?).
			search = ai.player + " switched in ";
			if (line.substr (0, search.length()) == search) {
				log_pokemon (ai, *foe.active, weather, line, map, search);
				if (ordering == NULL)
					ordering = &ai;
			}
			else {
				search = foe.player + " switched in ";
				if (line.substr (0, search.length()) == search)
					log_pokemon (foe, *ai.active, weather, line, map, search);
				if (ordering == NULL)
					ordering = &foe;
			}

			Team* active;
			Team* inactive;
			if (line.substr (0, ai.active->nickname.length()) == ai.active->nickname) {
				active = &ai;
				inactive = &foe;
			}
			else {
				active = &foe;
				inactive = &ai;
			}

			// It's best to include both nicknames in the search instead of just the invariant section. This prevents any combination of nicknames from causing an error. A Pokemon cannot have its own nickname plus something else in its nickname.
			
			// nickname used move.
			search = active->active->nickname + " used ";
			if (line.find (search) == 0) {		// If the beginning of the line is this
				log_move (*active, *inactive, weather, line, map, search);
				if (ordering == NULL)
					ordering = active;
			}
			else {
				log_misc (*active->active, *inactive->active, line, map);
				if (ordering == NULL)
					ordering = active;
			}
		}
	}
}

void log_pokemon  (Team &team, Pokemon &target, Weather &weather, const std::string &line, const Map &map, const std::string &search1) {
	std::string search2 = " (lvl ";
	size_t found2 = line.find (search2);
	std::string nickname = line.substr (search1.length(), found2 - search1.length());

	bool found = false;
	for (team.replacement = 0; team.replacement != team.active.member.size(); ++team.replacement) {
		if (nickname == team.active.member [team.replacement].nickname) {
			found = true;
			break;
		}
	}
	if (!found) {
		std::string search3 = " ";
		size_t found3 = line.find (search3, found2 + search2.length());
		std::string search4 = " ?).";
		size_t found4 = line.find (search4);
		genders gender;
		if (found4 == std::string::npos) {
			search4 = " ?)!";
			found4 = line.find (search4);
			if (found4 == std::string::npos) {
				search4 = " ♂).";
				found4 = line.find (search4);
				if (found4 == std::string::npos) {
					search4 = " ♂)!";
					found4 = line.find (search4);
				}
			}
		}
		if (found4 != std::string::npos)
			gender = MALE;		// No sexism here!
		else {
			search4 = " ♀).";
			found4 = line.find (search4);
			if (found4 == std::string::npos) {
				search4 = " ♀)!";
				found4 = line.find (search4);
			}
			if (found4 != std::string::npos)
				gender = FEMALE;
		}
		if (found4 == std::string::npos) {
			search4 = ").";
			found4 = line.find (search4);
			if (found4 == std::string::npos) {
				search4 = ")!";
				found4 = line.find (search4);
			}
			gender = GENDERLESS;
		}
		species name = map.specie.find (line.substr (found3 + search3.length(), found4 - found3 - search3.length()))->second;
		Pokemon member (name);
		member.gender = gender;

		member.nickname = nickname;
		member.ability = END_ABILITY;
		member.item = END_ITEM;

		member.level = boost::lexical_cast<int> (line.substr (found2 + search2.length(), found3 - found2 - search2.length()));

		member.nature = HARDY;
		team.active.member.push_back (member);
		team.replacement = team.active.member.size() - 1;
		
		loadpokemon (team, team.active.member.back());
	}
	switchpokemon (team, target, weather);
}

void log_move (Team &user, Team &target, Weather &weather, const std::string &line, const Map &map, const std::string &search) {
	// Account for Windows / Unix line endings
	size_t n = 1;
	if (line.find(".\r") != std::string::npos)
		n = 2;
	moves_list move_name = map.move.find (line.substr (search.length(), line.length() - search.length() - n))->second;
	bool isfound = false;
	for (user.active->move = user.active->moveset.begin(); user.active->move != user.active->moveset.end(); ++user.active->move) {
		if (move_name == user.active->move->name) {
			isfound = true;
			break;
		}
	}
	if (!isfound) {
		Move move (move_name, 3);
		user.active->moveset.push_back (move);
		user.active->move = user.active->moveset.end() - 1;
	}
	bool hitself = false;
	int old_damage = 0;
 	usemove (user, target, weather, hitself, old_damage);
}

void log_misc (Pokemon &active, Pokemon &inactive, const std::string &line, const Map &map) {
	if (active.ability == END_ABILITY) {
		if (active.nickname + "'s Anger Point raised its attack!" == line)
			active.ability = ANGER_POINT;
		else if (active.nickname + "'s Anticipation made it shudder!" == line)
			active.ability = ANTICIPATION;
		else if (active.nickname + "'s Cute Charm infatuated " + inactive.nickname == line)
			inactive.ability = CUTE_CHARM;
		else if (active.nickname + "'s Damp prevents explosions!" == line)
			active.ability = DAMP;
		else if (active.nickname + "'s Download raised its stats!" == line)
			active.ability = DOWNLOAD;
		else if (active.nickname + "'s Drizzle caused a storm!" == line)
			active.ability = DRIZZLE;
		else if (active.nickname + "'s Drought intensified the sun's rays!" == line)
			active.ability = DROUGHT;
		else if (active.nickname + " soaked up rain!" == line or active.nickname + "'s Dry Skin absorbed damage!" == line or active.nickname + "'s Dry Skin restored its health a little!" == line)	// Not sure which is correct
			active.ability = DRY_SKIN;
		else if (active.nickname + " was hurt by the sunlight!" == line)
			{/* Dry Skin and Solar Power. Need a way to distinguish them. */}
		else if (active.nickname + "'s Flame Body burned " + inactive.nickname == line)
			active.ability = FLAME_BODY;
		else if (active.nickname + "'s Flash Fire raised its fire power!" == line)
			active.ability = FLASH_FIRE;
		else if (line.find (active.nickname + "Forewarn alerted it to ") != std::string::npos)	// Fix to get the full information of Forewarn
			active.ability = FOREWARN;
		else if (line.find (active.nickname + " found " + inactive.nickname + "'s ") != std::string::npos)	// Fix to get full information of Frisk
			active.ability = FRISK;
		else if (active.nickname + "'s status was cured!" == line)
			active.ability = HYDRATION;
		else if (active.nickname + "absorbed the hail!" == line or active.nickname + "'s Ice Body restored its health a little!" == line)	// Not sure which is correct
			active.ability = ICE_BODY;
		else if (active.nickname + "'s Immunity prevents poisoning!" == line)
			active.ability = IMMUNITY;
		else if (active.nickname + "'s Insomnia kept it awake!" == line)
			active.ability = INSOMNIA;
		else if (active.nickname + "'s Intimidate cut " + inactive.nickname + "'s attack!" == line)
			active.ability = INTIMIDATE;
		else if (active.nickname + " makes ground moves miss with Levitate!" == line)
			active.ability = LEVITATE;
		else if (active.nickname + "'s Lightningrod drew the attack!" == line)
			active.ability = LIGHTNINGROD;
		else if (active.nickname + " has Mold Breaker!" == line)
			active.ability = MOLD_BREAKER;
		else if (active.nickname + "'s Motor Drive increased its speed!" == line)
			active.ability = MOTOR_DRIVE;
		else if (active.nickname + "'s Oblivious prevents attraction!" == line)
			active.ability = OBLIVIOUS;
		else if (active.nickname + "'s Poison Heal restored health!" == line)
			active.ability = POISON_HEAL;
		else if (active.nickname + " is exerting its pressure!" == line)
			active.ability = PRESSURE;
		else if (active.nickname + "'s Rain Dish restored its health a little!" == line)
			active.ability = RAIN_DISH;
		else if (active.nickname + "'s Sand Stream whipped up a sandstorm!" == line)
			active.ability = SAND_STREAM;
		else if (active.nickname + " shed its skin!" == line)
			active.ability = SHED_SKIN;
		else if (active.nickname + " can't get going due to its Slow Start!" == line)
			active.ability = SLOW_START;
		else if (active.nickname + "'s Snow Warning whipped up a hailstorm!" == line or active.nickname + "'s Snow Warning whipped up a hailstorm! " == line)
			active.ability = SNOW_WARNING;
		else if (line.find (active.nickname + "'s Soundproof blocks") != std::string::npos)
			active.ability = SOUNDPROOF;
		else if (active.nickname + "'s Speed Boost raised its speed!" == line)
			active.ability = SPEED_BOOST;
		else if (active.nickname + " held on with its Sticky Hold!" == line)
			active.ability = STICKY_HOLD;
		else if (active.nickname + "'s Storm Drain drew the attack!" == line)
			active.ability = STORM_DRAIN;
		else if (active.nickname + " held Sturdy!" == line)
			active.ability = STURDY;
		else if (active.nickname + "'s Synchronize activated!" == line)
			active.ability = SYNCHRONIZE;
		else if (active.nickname + "'s Tangled Feet raised its evasion!" == line)
			active.ability = TANGLED_FEET;
		else if (line.find (active.nickname + " traced " + inactive.nickname + "'s ") == 0)
			active.ability = TRACE;
		else if (active.nickname + " is loafing around!" == line)
			active.ability = TRUANT;
		else if (active.nickname + " lost its burden!" == line)
			active.ability = UNBURDEN;
		else if (active.nickname + "'s Water Veil prevents burns!" == line)
			active.ability = WATER_VEIL;
		else if (active.nickname + " avoided damage with Wonder Guard!" == line)
			active.ability = WONDER_GUARD;
	}
	if (inactive.ability == END_ABILITY) {
		if (active.nickname + " was hurt by  " + inactive.nickname + "'s Aftermath!" == line)
			inactive.ability = AFTERMATH;
		else if (active.nickname + "is tormented by the foe " +inactive.nickname + "'s Bad Dreams!" == line)
			inactive.ability = BAD_DREAMS;
		else if (active.nickname + " sucked up liquid ooze!" == line)
			inactive.ability = LIQUID_OOZE;
		else if (active.nickname + " was hurt by " + inactive.nickname + "'s Rough Skin!" == line)
			inactive.ability = ROUGH_SKIN;
		std::string search = active.nickname + " traced " + inactive.nickname + "'s ";
		if (line.find (search) != std::string::npos) {
			size_t n = 1;
			if (line.find(".\r") != std::string::npos)
				n = 2;
			inactive.ability = map.ability.find (line.substr (search.length(), line.length() - search.length() - n))->second;
		}
	}
	if (active.item == END_ITEM) {
		if (active.nickname + "'s Black Sludge restored a little health!" == line)
			active.item = BLACK_SLUDGE;
		else if (active.nickname + "'s leftovers restored its health a little!" == line)
			active.item = LEFTOVERS;
		else if (active.nickname + "'s Quick Claw activated!" == line)
			active.item = QUICK_CLAW;
		else if (active.nickname + " became fully charged due to its Power Herb!" == line)
			active.item = NO_ITEM;
	}
}

void output (std::string &output, const Team &team) {
	output += team.player + ":\n";
	for (std::vector<Pokemon>::const_iterator active = team.active.member.begin(); active != team.active.member.end(); ++active) {
		output += pokemon_name [active->name];
		output += " @ " + item_name [active->item];
		output += " ** " + active->nickname + '\n';
		if (active->ability != END_ABILITY)
			output += "\tAbility: " + ability_name [active->ability] + '\n';
		for (std::vector<Move>::const_iterator move = active->moveset.begin(); move != active->moveset.end(); ++move)
			output += "\t- " + move_name [move->name] + '\n';
	}
	output += '\n';
}

}
