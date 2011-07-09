// Analyze battle logs
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
#include <cstddef>
#include <iostream>
#include <map>
#include <string>
#include "analyze_logs.h"
#include "ability.h"
#include "endofturn.h"
#include "map.h"
#include "move.h"
#include "movefunction.h"
#include "pokemon.h"
#include "switch.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

Log::Log () :
	newline2 (-1),
	active (NULL),
	inactive (NULL),
	first (NULL),
	phaze (false),
	move_damage (false) {
}

bool analyze_turn (Team &ai, Team &foe, Weather &weather, Map const &map) {
	std::cout << "Enter the log for the turn, followed by a ~.\n";
	Log log;
	// Need to find a better way to signifiy end-of-turn. Currently the ~ with getline seems to remove the final line of a log, meaning I need to hit enter and then enter the ~.
	getline (std::cin, log.input, '~');
	std::cout << "======================\nAnalyzing...\n";
	bool won = false;
	if (log.input == "")
		won = true;
	else {
		initialize_turn (ai, foe, log);
		while (log.getline()) {
			if (log.line == "===============") {
				do_turn (*log.first, *log.last, weather);
				initialize_turn (ai, foe, log);
				std::cout << "\n";
			}
			else
				analyze_line (ai, foe, map, log);
		}
		if (ai.pokemon->hp.stat == 0) {
			ai.replacing = true;
			do_turn (ai, foe, weather);
		}
		// I have to check at_replacement(). instead of pokemon-> because when the AI switches to a Pokemon that then faints, the turn doesn't end, so there is never a ===... Therefore, that initial switch is never done, so if I don't check at_replacement()., I'm checking if the Pokemon that switched out fainted, which is not the behavior I want.
		else if (ai.at_replacement().fainted)
			do_turn (*log.first, *log.last, weather);
	}
	return won;
}

void analyze_line (Team &ai, Team &foe, Map const &map, Log &log) {
	if (log.line.find(": ") == std::string::npos) {		// Should ignore all comments, hopefully nobody puts : anywhere in their names
		log.search = "Begin turn #";
		if (log.search_is_first())
			return;
		log.search = "***";
		if (log.search_is_first())
			return;
		log.search = " sent out ";
		size_t found = log.line.find (log.search);
		if (found != std::string::npos) {
			log.search = ai.player + " sent out ";
			if (log.search_is_first())
				log_pokemon (ai, foe, map, log);
			else {
				if (foe.player == "") {	// If it's the very first turn
					foe.player = log.line.substr (0, found);
					ai.replacing = true;
					foe.replacing = true;
				}
				log.search = foe.player + " sent out ";
				log_pokemon (foe, ai, map, log);
			}
		}
		else {
			if (foe.player != "") {
				log.search = ai.player + " withdrew ";
				if (log.search_is_first())
					return;
				log.search = foe.player + " withdrew ";
				if (log.search_is_first())
					return;
			}
			if (log.line.substr (0, ai.at_replacement().nickname.length()) == ai.at_replacement().nickname) {
				log.active = &ai;
				log.inactive = &foe;
			}
			else if (log.line.substr (0, foe.at_replacement().nickname.length()) == foe.at_replacement().nickname) {
				log.active = &foe;
				log.inactive = &ai;
			}
			if (log.first == NULL) {
				log.first = log.active;
				log.last = log.inactive;
			}

			// It's best to include both nicknames in the search instead of just the invariant section. This prevents any combination of nicknames from causing an error. A Pokemon cannot have its own nickname plus something else in its nickname.
			if (log.move_damage) {
				log.search = log.active->at_replacement().nickname + " lost ";
				if (log.search_is_first()) {
					size_t division = log.line.find ("/", log.search.length());
					if (division != std::string::npos) {
						int numerator = boost::lexical_cast<int> (log.line.substr (log.search.length(), division - log.search.length()));
						++division;
						int denominator = boost::lexical_cast<int> (log.line.substr (division, log.line.find(" ", division) - division));
						log.active->damage = log.active->at_replacement().hp.max * numerator / denominator;
						log.move_damage = false;
					}
				}
				else if (log.line == "It had no effect.")
					log.move_damage = false;
			}
			log.search = log.active->at_replacement().nickname + "'s attack missed";
			if (log.search_is_first())
				log.active->miss = true;
			else if (log.line == "A critical hit!")
				log.active->ch = true;
			else if (log.line == log.active->at_replacement().nickname + " fainted!")
				log.active->at_replacement().fainted = true;
			else if (log.line == log.active->at_replacement().nickname + " is paralysed! It can't move!")
				log.active->fully_paralyzed = true;
			else if (log.line == "It hurt itself in confusion!")
				log.active->hitself = true;
			else if (log.line == log.active->at_replacement().nickname + " is paralysed! It may be unable to move!"
					or log.line == log.active->at_replacement().nickname + " was burned!"
					or log.line == log.active->at_replacement().nickname + " became confused!"
					or log.line == log.active->at_replacement().nickname + " was frozen solid!"
					or log.line == log.active->at_replacement().nickname + " was poisoned!"
					or log.line == log.active->at_replacement().nickname + " was badly poisoned!"
					or log.line == log.active->at_replacement().nickname + " flinched!"
					or log.line.find (" was sharply raised!", log.line.find (log.active->at_replacement().nickname + "'s ")) != std::string::npos
					or log.line.find (" was raised!", log.line.find (log.active->at_replacement().nickname + "'s ")) != std::string::npos
					or log.line.find (" was harshly lowered!", log.line.find (log.active->at_replacement().nickname + "'s ")) != std::string::npos
					or log.line.find (" was lowered!", log.line.find (log.active->at_replacement().nickname + "'s ")) != std::string::npos)
				log.inactive->at_replacement().move->variable.index = 1;
			else {
				log.search = log.active->at_replacement().nickname + " used ";
				if (log.search_is_first())
					log_move (log, map);
				else {
					bool shed_skin = false;
					log_misc (log, map, shed_skin);		// Fix
				}
			}
		}
	}
}

void log_pokemon (Team &team, Team &target, Map const &map, Log &log) {
	log.active = &team;
	log.inactive = &target;
	if (log.first == NULL) {
		log.first = &team;
		log.last = &target;
	}
	std::string search2 = " (lvl ";
	size_t found2 = log.line.find (search2);
	std::string nickname = log.line.substr (log.search.length(), found2 - log.search.length());

	size_t replacement = team.replacement;		// This is needed to make sure I don't overwrite important information in a situation in which a team switches multiple times in one turn (due to replacing fainted Pokemon).
	
	// Check if this Pokemon has been seen already.
	bool found = false;
	for (team.replacement = 0; team.replacement != team.pokemon.set.size(); ++team.replacement) {
		if (nickname == team.at_replacement().nickname) {
			found = true;
			break;
		}
	}
	
	// If it hasn't been seen already, get information like gender and level and add it to the team.
	if (!found) {
		std::string search3 = " ";
		size_t found3 = log.line.find (search3, found2 + search2.length());
		std::string search4 = " ?).";
		size_t found4 = log.line.find (search4);
		genders gender;
		if (found4 == std::string::npos) {
			search4 = " ?)!";
			found4 = log.line.find (search4);
			if (found4 == std::string::npos) {
				search4 = " ♂).";
				found4 = log.line.find (search4);
				if (found4 == std::string::npos) {
					search4 = " ♂)!";
					found4 = log.line.find (search4);
				}
			}
		}
		if (found4 != std::string::npos)
			gender = MALE;		// No sexism here!
		else {
			search4 = " ♀).";
			found4 = log.line.find (search4);
			if (found4 == std::string::npos) {
				search4 = " ♀)!";
				found4 = log.line.find (search4);
			}
			if (found4 != std::string::npos)
				gender = FEMALE;
		}
		if (found4 == std::string::npos) {
			search4 = ").";
			found4 = log.line.find (search4);
			if (found4 == std::string::npos) {
				search4 = ")!";
				found4 = log.line.find (search4);
			}
			gender = GENDERLESS;
		}
		species name = map.specie.find (log.line.substr (found3 + search3.length(), found4 - found3 - search3.length()))->second;
		Pokemon member (name, team.size);
		member.gender = gender;

		member.nickname = nickname;

		member.level = boost::lexical_cast<int> (log.line.substr (found2 + search2.length(), found3 - found2 - search2.length()));

		member.nature = HARDY;
		team.pokemon.set.push_back (member);
		team.replacement = team.pokemon.set.size() - 1;
		team.pokemon.set.back().load();
	}

	// Special analysis when a Pokemon is brought out due to a phazing move
	if (log.phaze) {
		target.at_replacement().move->variable.index = 0;
		while (team.pokemon.set [target.at_replacement().move->variable.index].nickname != nickname)
			++target.at_replacement().move->variable.index;
	}
	else if (!log.active->moved) {
		team.pokemon.set [replacement].move.index = 0;
		while (team.pokemon.set [replacement].move->name != SWITCH0)
			++team.pokemon.set [replacement].move.index;
		team.pokemon.set [replacement].move.index += team.replacement;
		log.active->moved = false;
	}
}


void log_move (Log &log, Map const &map) {
	log.active->moved = true;
	// Account for Windows / Unix line endings
	size_t n = 1;
	if (log.line.find(".\r") != std::string::npos)
		n = 2;
	moves_list name = map.move.find (log.line.substr (log.search.length(), log.line.length() - log.search.length() - n))->second;
	bool isfound = false;
	for (log.active->at_replacement().move.index = 0; log.active->at_replacement().move.index != log.active->at_replacement().move.set.size(); ++log.active->at_replacement().move.index) {
		if (name == log.active->at_replacement().move->name) {
			isfound = true;
			break;
		}
	}
	if (!isfound) {
		Move move (name, 3, log.inactive->size);
		log.active->at_replacement().move.set.insert (log.active->at_replacement().move.set.begin(), move);
		log.active->at_replacement().move.index = 0;
	}
	if (log.active->at_replacement().move->name == ROAR or log.active->at_replacement().move->name == WHIRLWIND)
		log.phaze = true;
	log.active->at_replacement().move->variable.index = 0;
	if (log.active->at_replacement().move->basepower != 0)
		log.move_damage = true;
}


void log_misc (Log &log, Map const &map, bool &shed_skin) {
	if (log.active->at_replacement().ability == END_ABILITY) {
		if (log.active->at_replacement().nickname + "'s Anger Point raised its attack!" == log.line) {
			log.active->at_replacement().ability = ANGER_POINT;
			log.active->at_replacement().atk.stage = 6;
		}
		else if (log.active->at_replacement().nickname + "'s Anticipation made it shudder!" == log.line)
			log.active->at_replacement().ability = ANTICIPATION;
		else if (log.active->at_replacement().nickname + "'s Cute Charm infatuated " + log.inactive->at_replacement().nickname == log.line) {
			log.active->at_replacement().ability = CUTE_CHARM;
			log.inactive->attract = true;
		}
		else if (log.active->at_replacement().nickname + "'s Damp prevents explosions!" == log.line)
			log.active->at_replacement().ability = DAMP;
		else if (log.active->at_replacement().nickname + "'s Download raised its stats!" == log.line)
			log.active->at_replacement().ability = DOWNLOAD;
		else if (log.active->at_replacement().nickname + "'s Drizzle caused a storm!" == log.line)
			log.active->at_replacement().ability = DRIZZLE;
		else if (log.active->at_replacement().nickname + "'s Drought intensified the sun's rays!" == log.line)
			log.active->at_replacement().ability = DROUGHT;
		else if (log.active->at_replacement().nickname + " soaked up rain!" == log.line or log.active->at_replacement().nickname + "'s Dry Skin absorbed damage!" == log.line or log.active->at_replacement().nickname + "'s Dry Skin restored its health a little!" == log.line)	// Not sure which is correct
			log.active->at_replacement().ability = DRY_SKIN;
		else if (log.active->at_replacement().nickname + " was hurt by the sunlight!" == log.line)
			{/* Dry Skin and Solar Power. Need a way to distinguish them. */}
		else if (log.active->at_replacement().nickname + "'s Flame Body burned " + log.inactive->at_replacement().nickname == log.line)
			log.active->at_replacement().ability = FLAME_BODY;
		else if (log.active->at_replacement().nickname + "'s Flash Fire raised its fire power!" == log.line)
			log.active->at_replacement().ability = FLASH_FIRE;
		else if (log.line.find (log.active->at_replacement().nickname + "Forewarn alerted it to ") != std::string::npos)	// Fix to get the full information of Forewarn
			log.active->at_replacement().ability = FOREWARN;
		else if (log.line.find (log.active->at_replacement().nickname + " found " + log.inactive->at_replacement().nickname + "'s ") != std::string::npos)	// Fix to get full information of Frisk
			log.active->at_replacement().ability = FRISK;
		else if (log.active->at_replacement().nickname + "'s status was cured!" == log.line)
			log.active->at_replacement().ability = HYDRATION;
		else if (log.active->at_replacement().nickname + "absorbed the hail!" == log.line or log.active->at_replacement().nickname + "'s Ice Body restored its health a little!" == log.line)	// Not sure which is correct
			log.active->at_replacement().ability = ICE_BODY;
		else if (log.active->at_replacement().nickname + "'s Immunity prevents poisoning!" == log.line)
			log.active->at_replacement().ability = IMMUNITY;
		else if (log.active->at_replacement().nickname + "'s Insomnia kept it awake!" == log.line)
			log.active->at_replacement().ability = INSOMNIA;
		else if (log.active->at_replacement().nickname + "'s Intimidate cut " + log.inactive->at_replacement().nickname + "'s attack!" == log.line)
			log.active->at_replacement().ability = INTIMIDATE;
		else if (log.active->at_replacement().nickname + " makes ground moves miss with Levitate!" == log.line)
			log.active->at_replacement().ability = LEVITATE;
		else if (log.active->at_replacement().nickname + "'s Lightningrod drew the attack!" == log.line)
			log.active->at_replacement().ability = LIGHTNINGROD;
		else if (log.active->at_replacement().nickname + " has Mold Breaker!" == log.line)
			log.active->at_replacement().ability = MOLD_BREAKER;
		else if (log.active->at_replacement().nickname + "'s Motor Drive increased its speed!" == log.line)
			log.active->at_replacement().ability = MOTOR_DRIVE;
		else if (log.active->at_replacement().nickname + "'s Oblivious prevents attraction!" == log.line)
			log.active->at_replacement().ability = OBLIVIOUS;
		else if (log.active->at_replacement().nickname + "'s Poison Heal restored health!" == log.line)
			log.active->at_replacement().ability = POISON_HEAL;
		else if (log.active->at_replacement().nickname + " is exerting its pressure!" == log.line)
			log.active->at_replacement().ability = PRESSURE;
		else if (log.active->at_replacement().nickname + "'s Rain Dish restored its health a little!" == log.line)
			log.active->at_replacement().ability = RAIN_DISH;
		else if (log.active->at_replacement().nickname + "'s Sand Stream whipped up a sandstorm!" == log.line)
			log.active->at_replacement().ability = SAND_STREAM;
		else if (log.active->at_replacement().nickname + " shed its skin!" == log.line) {
			log.active->at_replacement().ability = SHED_SKIN;
			shed_skin = true;
		}
		else if (log.active->at_replacement().nickname + " can't get going due to its Slow Start!" == log.line)
			log.active->at_replacement().ability = SLOW_START;
		else if (log.active->at_replacement().nickname + "'s Snow Warning whipped up a hailstorm!" == log.line or log.active->at_replacement().nickname + "'s Snow Warning whipped up a hailstorm! " == log.line)
			log.active->at_replacement().ability = SNOW_WARNING;
		else if (log.line.find (log.active->at_replacement().nickname + "'s Soundproof blocks") != std::string::npos)
			log.active->at_replacement().ability = SOUNDPROOF;
		else if (log.active->at_replacement().nickname + "'s Speed Boost raised its speed!" == log.line)
			log.active->at_replacement().ability = SPEED_BOOST;
		else if (log.active->at_replacement().nickname + " held on with its Sticky Hold!" == log.line)
			log.active->at_replacement().ability = STICKY_HOLD;
		else if (log.active->at_replacement().nickname + "'s Storm Drain drew the attack!" == log.line)
			log.active->at_replacement().ability = STORM_DRAIN;
		else if (log.active->at_replacement().nickname + " held Sturdy!" == log.line)
			log.active->at_replacement().ability = STURDY;
		else if (log.active->at_replacement().nickname + "'s Synchronize activated!" == log.line)
			log.active->at_replacement().ability = SYNCHRONIZE;
		else if (log.active->at_replacement().nickname + "'s Tangled Feet raised its evasion!" == log.line)
			log.active->at_replacement().ability = TANGLED_FEET;
		else if (log.line.find (log.active->at_replacement().nickname + " traced " + log.inactive->at_replacement().nickname + "'s ") == 0)
			log.active->at_replacement().ability = TRACE;
		else if (log.active->at_replacement().nickname + " is loafing around!" == log.line)
			log.active->at_replacement().ability = TRUANT;
		else if (log.active->at_replacement().nickname + " lost its burden!" == log.line)
			log.active->at_replacement().ability = UNBURDEN;
		else if (log.active->at_replacement().nickname + "'s Water Veil prevents burns!" == log.line)
			log.active->at_replacement().ability = WATER_VEIL;
		else if (log.active->at_replacement().nickname + " avoided damage with Wonder Guard!" == log.line)
			log.active->at_replacement().ability = WONDER_GUARD;
	}
	if (log.inactive->at_replacement().ability == END_ABILITY) {
		if (log.active->at_replacement().nickname + " was hurt by " + log.inactive->at_replacement().nickname + "'s Aftermath!" == log.line)
			log.inactive->at_replacement().ability = AFTERMATH;
		else if (log.active->at_replacement().nickname + "is tormented by the foe " +log.inactive->at_replacement().nickname + "'s Bad Dreams!" == log.line)
			log.inactive->at_replacement().ability = BAD_DREAMS;
		else if (log.active->at_replacement().nickname + " sucked up liquid ooze!" == log.line)
			log.inactive->at_replacement().ability = LIQUID_OOZE;
		else if (log.active->at_replacement().nickname + " was hurt by " + log.inactive->at_replacement().nickname + "'s Rough Skin!" == log.line)
			log.inactive->at_replacement().ability = ROUGH_SKIN;
		std::string search = log.active->at_replacement().nickname + " traced " + log.inactive->at_replacement().nickname + "'s ";
		if (log.line.find (search) != std::string::npos) {
			size_t n = 1;
			if (log.line.find(".\r") != std::string::npos)
				n = 2;
			log.inactive->at_replacement().ability = map.ability.find (log.line.substr (search.length(), log.line.length() - search.length() - n))->second;
		}
	}
	if (log.active->at_replacement().item == END_ITEM) {
		if (log.active->at_replacement().nickname + "'s Black Sludge restored a little health!" == log.line)
			log.active->at_replacement().item = BLACK_SLUDGE;
		else if (log.active->at_replacement().nickname + "'s Leftovers restored its health a little!" == log.line)
			log.active->at_replacement().item = LEFTOVERS;
		else if (log.active->at_replacement().nickname + "'s Quick Claw activated!" == log.line)
			log.active->at_replacement().item = QUICK_CLAW;
		else if (log.active->at_replacement().nickname + " became fully charged due to its Power Herb!" == log.line)
			log.active->at_replacement().item = POWER_HERB;
	}
}

void do_turn (Team &first, Team &last, Weather &weather) {
	first.moved = false;
	last.moved = false;
	if (first.replacing) {
		normalize_hp (first, last);
		switchpokemon (first, last, weather);
		first.moved = false;
		normalize_hp (first, last);
		if (last.replacing) {
			switchpokemon (last, first, weather);
			last.moved = false;
			normalize_hp (first, last);
		}
	}
	else {
		// Anything with recoil will mess this up
		usemove (first, last, weather, last.damage);
		normalize_hp_team (last);
		usemove (last, first, weather, first.damage);
		normalize_hp_team (first);

		endofturn (first, last, weather);
		normalize_hp (first, last);
		Team* ai;
		Team* foe;
		if (first.me) {
			ai = &first;
			foe = &last;
		}
		else {
			foe = &first;
			ai = &last;
		}
		while (foe->pokemon->fainted) {
			if (!foe->pokemon->move->is_switch()) {
				foe->pokemon->move.index = 0;
				while (foe->pokemon->move->name != SWITCH0)
					++foe->pokemon->move.index;
				foe->pokemon->move.index += foe->replacement;
			}
			usemove (*foe, *ai, weather, first.damage);
		}
	}
	std::string out;
	first.output (out);
	std::cout << out;
	last.output (out);
	std::cout << out;
}

bool Log::getline () {
	// Returns false if there are no more non-empty newline delimited strings remaining, otherwise, gives the next non-empty newline delimited string from a multi-line string and returns true.
	newline1 = newline2 + 1;
	while (newline1 < input.length() and input.at (newline1) == '\n')
		++newline1;
	newline2 = input.find ('\n', newline1 + 1);
	if (newline2 == std::string::npos)
		return false;
	else {
		line = input.substr (newline1, newline2 - newline1);
		return true;
	}
}

bool Log::search_is_first () {
	return line.substr (0, search.length()) == search;
}

void initialize_turn (Team &ai, Team &foe, Log &log) {
	initialize_team (ai);
	initialize_team (foe);
	log.phaze = false;
	log.move_damage = false;
	
	log.active = NULL;		// GCC reports a potential use of this unitialized only when compiling with full optimizations. Variables unnecessarily set to NULL to remove this warning.
	log.inactive = NULL;
	log.first = NULL;
	log.last = NULL;
}

void initialize_team (Team &team) {
	for (std::vector<Pokemon>::iterator pokemon = team.pokemon.set.begin(); pokemon != team.pokemon.set.end(); ++pokemon)
		pokemon->move.index = 0;
	team.ch = false;
	team.fully_paralyzed = false;
	team.hitself = false;
	team.miss = false;
	team.replacement = team.pokemon.index;
	team.replacing = false;
}

void normalize_hp (Team &first, Team &last) {
	normalize_hp_team (first);
	normalize_hp_team (last);
}

void normalize_hp_team (Team &team) {
	// This is to correct for rounding issues caused by only seeing the foe's HP to the nearest 48th.
	if (team.pokemon->fainted)
		team.pokemon->hp.stat = 0;
	else if (team.pokemon->hp.stat == 0)
		team.pokemon->hp.stat = 1;
}

}
