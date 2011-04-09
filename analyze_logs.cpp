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
#include "team.h"
#include "weather.h"

namespace technicalmachine {

void analyze_turn (teams &ai, teams &foe, teams* &first, teams* &last, weathers &weather, const Map &map) {
	first = NULL;		// Only check if first == NULL, so no need to set last = NULL
	std::cout << "Enter the log for the turn, followed by a ~.\n";
	std::string input;
	getline (std::cin, input, '~');		// Need to find a better way to signifiy end-of-turn. This works for now.
	std::cout << "======================\n";
	size_t newline1 = 0;
	size_t newline2 = input.find ('\n', newline1 + 1);
	while (newline2 != std::string::npos) {
		std::string line = input.substr (newline1, newline2);
		analyze_line (ai, foe, first, weather, line, map);
		newline1 = newline2;
		newline2 = input.find ('\n', newline1 + 1);
		while (newline1 + 1 == newline2) {
			newline1 = newline2;
			newline2 = input.find ('\n', newline1 + 1);
		}
	}
	if (first->me)
		last = &foe;
	else
		last = &ai;
}

void analyze_line (teams &ai, teams &foe, teams* &ordering, weathers &weather, const std::string &line, const Map &map) {
	if (line.find(": ") == std::string::npos) {		// Should ignore all comments, hopefully nobody puts : anywhere in their names
		// name sent out nickname (lvl x species ?).
		std::string search = " sent out ";
		size_t found = line.find (search);
		if (found != std::string::npos) {
//			if (ai.player == "")
//				ai.player = line.substr (0, found);
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

			teams* active;
			teams* inactive;
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
				log_move (*active->active, line, map, search);
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

void log_pokemon  (teams &team, pokemon &target, weathers &weather, const std::string &line, const Map &map, std::string &search1) {
	std::string search2 = " (lvl ";
	size_t found2 = line.find (search2);
	std::string nickname = line.substr (search1.length(), found2 - search1.length());
	bool found = false;
	for (unsigned char replacement = 0; replacement != team.member.size(); ++replacement) {
		if (nickname == team.member.at (replacement).nickname) {
			found = true;
			team.replacement = replacement;
			break;
		}
	}
	if (!found) {
			// Insertion to a vector invalidates all iterators if the insertion causes a reallocation. This corrects for that.
		unsigned n = 0;
		if (team.member.size() > 0)
			n = team.active - team.member.begin();
		pokemon member;
		team.member.push_back (member);

		team.active = team.member.begin() + n;

		team.replacement = team.member.size() - 1;
		
		team.member.back().nickname = nickname;
		team.member.back().happiness = 255;
		team.member.back().item = END_ITEM;
		team.member.back().ability = END_ABILITY;
		search1 = " ";
		size_t found1 = line.find (search1, found2 + search2.length());
		team.member.back().level = boost::lexical_cast<int> (line.substr (found2 + search2.length(), found1 - found2 - search2.length()));

		search2 = " ?).";
		found2 = line.find (search2);
		if (found2 == std::string::npos) {
			search2 = " ?)!";
			found2 = line.find (search2);
			if (found2 == std::string::npos) {
				search2 = " ♂).";
				found2 = line.find (search2);
				if (found2 == std::string::npos) {
					search2 = " ♂)!";
					found2 = line.find (search2);
				}
			}
		}
		if (found2 != std::string::npos)
			team.member.back().gender = MALE;		// No sexism here!
		else {
			search2 = " ♀).";
			found2 = line.find (search2);
			if (found2 == std::string::npos) {
				search2 = " ♀)!";
				found2 = line.find (search2);
			}
			if (found2 != std::string::npos)
				team.member.back().gender = FEMALE;
		}
		if (found2 == std::string::npos) {
			search2 = ").";
			found2 = line.find (search2);
			if (found2 == std::string::npos) {
				search2 = " )!";
				found2 = line.find (search2);
			}
			team.member.back().gender = GENDERLESS;
		}
		team.member.back().name = map.specie.find (line.substr (found1 + search1.length(), found2 - found1 - search1.length()))->second;
		
		team.member.back().nature = HARDY;
		team.member.back().hp.iv = 31;
		team.member.back().hp.ev = 0;
		team.member.back().atk.iv = 31;
		team.member.back().atk.ev = 0;
		team.member.back().def.iv = 31;
		team.member.back().def.ev = 0;
		team.member.back().spe.iv = 31;
		team.member.back().spe.ev = 0;
		team.member.back().spa.iv = 31;
		team.member.back().spa.ev = 0;
		team.member.back().spd.iv = 31;
		team.member.back().spd.ev = 0;
		
		loadpokemon (team, team.member.back());
	}
	switchpokemon (team, target, weather);
	std::cout << pokemon_name [team.member.back().name] + '\n';
}

void log_move (pokemon &member, const std::string &line, const Map &map, const std::string &search) {
	// Account for Windows / Unix line endings
	size_t n = 1;
	if (line.find(".\r") != std::string::npos)
		n = 2;
	moves_list move_name = map.move.find (line.substr (search.length(), line.length() - search.length() - n))->second;
	bool isfound = false;
	for (std::vector<moves>::iterator it = member.moveset.begin(); it != member.moveset.end(); ++it) {
		if (move_name == it->name) {
			isfound = true;
			member.move = it;
			break;
		}
	}
	if (!isfound) {
		moves move;
		member.moveset.push_back (move);
		member.move = member.moveset.end() - 1;
		member.move->name = move_name;
	}
}

void log_misc (pokemon &active, pokemon &inactive, const std::string &line, const Map &map) {
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

void isme (teams &team) {		// Top secret information. Can't let this leak out. Make sure to remove this function before uploading.
	if (team.player == "graviton" or team.player == "Graviton" or team.player == "king of the king" or team.player == "Morlock" or team.player == "obi" or team.player == "Obi" or team.player == "reziarfg" or team.player == "Reziarfg" or team.player == "Specter" or team.player == "Sylar" or team.player == "Tracer Tong")
		team.me = true;
	else
		team.me = false;
}

void output (std::string &output, const teams &team) {
	output += team.player + ":\n";
	for (std::vector<pokemon>::const_iterator active = team.member.begin(); active != team.member.end(); ++active) {
		output += pokemon_name [active->name];
		output += " @ " + item_name [active->item];
		output += " ** " + active->nickname + '\n';
		if (active->ability != END_ABILITY)
			output += "\tAbility: " + ability_name [active->ability] + '\n';
		for (std::vector<moves>::const_iterator move = active->moveset.begin(); move != active->moveset.end(); ++move)
			output += "\t- " + move_name [move->name] + '\n';
	}
	output += '\n';
}

}
