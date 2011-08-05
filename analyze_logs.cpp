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
#include <vector>
#include "analyze_logs.h"
#include "ability.h"
#include "endofturn.h"
#include "map.h"
#include "move.h"
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

bool analyze_turn (Team & ai, Team & foe, Weather & weather, Map const & map) {
	std::cout << "Enter the log for the turn, followed by a ~.\n";
	Log log;
	// Need to find a better way to signifiy end-of-turn. Currently the ~ with getline seems to remove the final line of a log, meaning I need to hit enter and then enter the ~.
	getline (std::cin, log.input, '~');
	std::cout << "======================\nAnalyzing...\n";
	bool won = false;
	if (log.input == "")
		won = true;
	else {
		log.initialize_turn (ai, foe);
		while (log.getline()) {
			if (log.line == "===============") {
				do_turn (*log.first, *log.last, weather);
				log.initialize_turn (ai, foe);
				std::cout << "\n";
			}
			else
				log.analyze_line (ai, foe, map);
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

void Log::analyze_line (Team & ai, Team & foe, Map const & map) {
	if (line.find(": ") == std::string::npos) {		// Should ignore all comments, hopefully nobody puts : anywhere in their names
		search = "Begin turn #";
		if (search_is_first())
			return;
		search = "***";
		if (search_is_first())
			return;
		search = " sent out ";
		size_t found = line.find (search);
		if (found != std::string::npos) {
			search = ai.player + " sent out ";
			if (search_is_first())
				log_pokemon (ai, foe, map);
			else {
				if (foe.player == "") {	// If it's the very first turn
					foe.player = line.substr (0, found);
					ai.replacing = true;
					foe.replacing = true;
				}
				search = foe.player + " sent out ";
				log_pokemon (foe, ai, map);
			}
		}
		else {
			if (foe.player != "") {
				search = ai.player + " withdrew ";
				if (search_is_first())
					return;
				search = foe.player + " withdrew ";
				if (search_is_first())
					return;
			}
			if (line.substr (0, ai.at_replacement().nickname.length()) == ai.at_replacement().nickname) {
				active = &ai;
				inactive = &foe;
			}
			else if (line.substr (0, foe.at_replacement().nickname.length()) == foe.at_replacement().nickname) {
				active = &foe;
				inactive = &ai;
			}
			if (first == NULL) {
				first = active;
				last = inactive;
			}

			// It's best to include both nicknames in the search instead of just the invariant section. This prevents any combination of nicknames from causing an error. A Pokemon cannot have its own nickname plus something else in its nickname.
			if (move_damage) {
				search = active->at_replacement().nickname + " lost ";
				if (search_is_first()) {
					size_t division = line.find ("/", search.length());
					if (division != std::string::npos) {
						unsigned numerator = boost::lexical_cast<unsigned> (line.substr (search.length(), division - search.length()));
						++division;
						unsigned denominator = boost::lexical_cast<unsigned> (line.substr (division, line.find(" ", division) - division));
						active->damage = active->at_replacement().hp.max * numerator / denominator;
						move_damage = false;
					}
				}
				else if (line == "It had no effect.")
					move_damage = false;
			}
			search = active->at_replacement().nickname + "'s attack missed";
			if (search_is_first())
				active->miss = true;
			else if (line == "A critical hit!")
				active->ch = true;
			else if (line == active->at_replacement().nickname + " fainted!")
				active->at_replacement().fainted = true;
			else if (line == active->at_replacement().nickname + " is paralysed! It can't move!")
				active->fully_paralyzed = true;
			else if (line == "It hurt itself in confusion!")
				active->hitself = true;
			else if (line == active->at_replacement().nickname + " is paralysed! It may be unable to move!"
					or line == active->at_replacement().nickname + " was burned!"
					or line == active->at_replacement().nickname + " became confused!"
					or line == active->at_replacement().nickname + " was frozen solid!"
					or line == active->at_replacement().nickname + " was poisoned!"
					or line == active->at_replacement().nickname + " was badly poisoned!"
					or line == active->at_replacement().nickname + " flinched!"
					or line.find (" was sharply raised!", line.find (active->at_replacement().nickname + "'s ")) != std::string::npos
					or line.find (" was raised!", line.find (active->at_replacement().nickname + "'s ")) != std::string::npos
					or line.find (" was harshly lowered!", line.find (active->at_replacement().nickname + "'s ")) != std::string::npos
					or line.find (" was lowered!", line.find (active->at_replacement().nickname + "'s ")) != std::string::npos)
				inactive->at_replacement().move->variable.index = 1;
			else {
				search = active->at_replacement().nickname + " used ";
				if (search_is_first()) {
					moves_list move = find_move_name (map);
					log_move (move);
				}
				else {
					bool shed_skin = false;
					log_misc (map, shed_skin);		// Fix
				}
			}
		}
	}
}

void Log::log_pokemon (Team & team, Team & other, Map const & map) {
	species name;
	std::string nickname;
	int level;
	genders gender;
	get_pokemon_info (map, name, nickname, level, gender);
	pokemon_sent_out (map, name, nickname, level, gender, team, other);
}

void Log::get_pokemon_info (Map const & map, species & name, std::string & nickname, int & level, genders & gender) {
	std::string search2 = " (lvl ";
	size_t found2 = line.find (search2);
 	nickname = line.substr (search.length(), found2 - search.length());
 	std::string search3 = " ";
	size_t found3 = line.find (search3, found2 + search2.length());
	std::string search4 = " ?).";
	size_t found4 = line.find (search4);
	gender;
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
	name = map.specie.find (line.substr (found3 + search3.length(), found4 - found3 - search3.length()))->second;
	level = boost::lexical_cast<int> (line.substr (found2 + search2.length(), found3 - found2 - search2.length()));
}

void Log::pokemon_sent_out (Map const & map, species name, std::string const & nickname, int level, genders gender, Team & team, Team & other) {
	size_t replacement = team.replacement;		// This is needed to make sure I don't overwrite important information in a situation in which a team switches multiple times in one turn (due to replacing fainted Pokemon).
	
	bool found = seen_pokemon (team, name);
	
	// If it hasn't been seen already, add it to the team.
	if (!found)
		add_pokemon (team, name, nickname, level, gender);
	
	active = &team;
	inactive = &other;
	if (first == NULL) {
		first = &team;
		last = &other;
	}

	// Special analysis when a Pokemon is brought out due to a phazing move
	if (phaze) {
		other.at_replacement().move->variable.index = 0;
		while (team.pokemon.set [other.at_replacement().move->variable.index].name != name)
			++other.at_replacement().move->variable.index;
	}
	else if (!active->moved) {
		team.pokemon.set [replacement].move.index = 0;
		while (team.pokemon.set [replacement].move->name != SWITCH0)
			++team.pokemon.set [replacement].move.index;
		team.pokemon.set [replacement].move.index += team.replacement;
		active->moved = false;
	}
}

bool Log::seen_pokemon (Team & team, species name) {
	// Check if this Pokemon has been seen already. If it has, set team.replacement to its location.
	for (team.replacement = 0; team.replacement != team.pokemon.set.size(); ++team.replacement) {
		if (name == team.at_replacement().name)
			return true;
	}
	return false;
}

void Log::add_pokemon (Team & team, species name, std::string const & nickname, int level, genders gender) {
	Pokemon member (name, team.size);
	member.level = level;
	member.gender = gender;

	member.nickname = nickname;

	team.pokemon.set.push_back (member);
	team.replacement = team.pokemon.set.size() - 1;
	team.pokemon.set.back().load();
}

moves_list Log::find_move_name (Map const & map) const {
	// Account for Windows / Unix line endings
	size_t n = 1;
	if (line.find(".\r") != std::string::npos)
		n = 2;
	return map.move.find (line.substr (search.length(), line.length() - search.length() - n))->second;
}

void Log::log_move (moves_list name) {
	active->moved = true;
	bool isfound = false;
	for (active->at_replacement().move.index = 0; active->at_replacement().move.index != active->at_replacement().move.set.size(); ++active->at_replacement().move.index) {
		if (name == active->at_replacement().move->name) {
			isfound = true;
			break;
		}
	}
	if (!isfound) {
		Move move (name, 3, inactive->size);
		active->at_replacement().move.set.insert (active->at_replacement().move.set.begin(), move);
		active->at_replacement().move.index = 0;
	}
	if (active->at_replacement().move->name == ROAR or active->at_replacement().move->name == WHIRLWIND)
		phaze = true;
	active->at_replacement().move->variable.index = 0;
	if (active->at_replacement().move->basepower != 0)
		move_damage = true;
}


void Log::log_misc (Map const & map, bool & shed_skin) {
	if (active->at_replacement().ability == END_ABILITY) {
		if (active->at_replacement().nickname + "'s Anger Point raised its attack!" == line) {
			active->at_replacement().ability = ANGER_POINT;
			active->at_replacement().atk.stage = 6;
		}
		else if (active->at_replacement().nickname + "'s Anticipation made it shudder!" == line)
			active->at_replacement().ability = ANTICIPATION;
		else if (active->at_replacement().nickname + "'s Cute Charm infatuated " + inactive->at_replacement().nickname == line) {
			active->at_replacement().ability = CUTE_CHARM;
			inactive->attract = true;
		}
		else if (active->at_replacement().nickname + "'s Damp prevents explosions!" == line)
			active->at_replacement().ability = DAMP;
		else if (active->at_replacement().nickname + "'s Download raised its stats!" == line)
			active->at_replacement().ability = DOWNLOAD;
		else if (active->at_replacement().nickname + "'s Drizzle caused a storm!" == line)
			active->at_replacement().ability = DRIZZLE;
		else if (active->at_replacement().nickname + "'s Drought intensified the sun's rays!" == line)
			active->at_replacement().ability = DROUGHT;
		else if (active->at_replacement().nickname + " soaked up rain!" == line or active->at_replacement().nickname + "'s Dry Skin absorbed damage!" == line or active->at_replacement().nickname + "'s Dry Skin restored its health a little!" == line)	// Not sure which is correct
			active->at_replacement().ability = DRY_SKIN;
		else if (active->at_replacement().nickname + " was hurt by the sunlight!" == line)
			{/* Dry Skin and Solar Power. Need a way to distinguish them. */}
		else if (active->at_replacement().nickname + "'s Flame Body burned " + inactive->at_replacement().nickname == line)
			active->at_replacement().ability = FLAME_BODY;
		else if (active->at_replacement().nickname + "'s Flash Fire raised its fire power!" == line)
			active->at_replacement().ability = FLASH_FIRE;
		else if (line.find (active->at_replacement().nickname + "Forewarn alerted it to ") != std::string::npos)	// Fix to get the full information of Forewarn
			active->at_replacement().ability = FOREWARN;
		else if (line.find (active->at_replacement().nickname + " found " + inactive->at_replacement().nickname + "'s ") != std::string::npos)	// Fix to get full information of Frisk
			active->at_replacement().ability = FRISK;
		else if (active->at_replacement().nickname + "'s status was cured!" == line)
			active->at_replacement().ability = HYDRATION;
		else if (active->at_replacement().nickname + "absorbed the hail!" == line or active->at_replacement().nickname + "'s Ice Body restored its health a little!" == line)	// Not sure which is correct
			active->at_replacement().ability = ICE_BODY;
		else if (active->at_replacement().nickname + "'s Immunity prevents poisoning!" == line)
			active->at_replacement().ability = IMMUNITY;
		else if (active->at_replacement().nickname + "'s Insomnia kept it awake!" == line)
			active->at_replacement().ability = INSOMNIA;
		else if (active->at_replacement().nickname + "'s Intimidate cut " + inactive->at_replacement().nickname + "'s attack!" == line)
			active->at_replacement().ability = INTIMIDATE;
		else if (active->at_replacement().nickname + " makes ground moves miss with Levitate!" == line)
			active->at_replacement().ability = LEVITATE;
		else if (active->at_replacement().nickname + "'s Lightningrod drew the attack!" == line)
			active->at_replacement().ability = LIGHTNINGROD;
		else if (active->at_replacement().nickname + " has Mold Breaker!" == line)
			active->at_replacement().ability = MOLD_BREAKER;
		else if (active->at_replacement().nickname + "'s Motor Drive increased its speed!" == line)
			active->at_replacement().ability = MOTOR_DRIVE;
		else if (active->at_replacement().nickname + "'s Oblivious prevents attraction!" == line)
			active->at_replacement().ability = OBLIVIOUS;
		else if (active->at_replacement().nickname + "'s Poison Heal restored health!" == line)
			active->at_replacement().ability = POISON_HEAL;
		else if (active->at_replacement().nickname + " is exerting its pressure!" == line)
			active->at_replacement().ability = PRESSURE;
		else if (active->at_replacement().nickname + "'s Rain Dish restored its health a little!" == line)
			active->at_replacement().ability = RAIN_DISH;
		else if (active->at_replacement().nickname + "'s Sand Stream whipped up a sandstorm!" == line)
			active->at_replacement().ability = SAND_STREAM;
		else if (active->at_replacement().nickname + " shed its skin!" == line) {
			active->at_replacement().ability = SHED_SKIN;
			shed_skin = true;
		}
		else if (active->at_replacement().nickname + " can't get going due to its Slow Start!" == line)
			active->at_replacement().ability = SLOW_START;
		else if (active->at_replacement().nickname + "'s Snow Warning whipped up a hailstorm!" == line or active->at_replacement().nickname + "'s Snow Warning whipped up a hailstorm! " == line)
			active->at_replacement().ability = SNOW_WARNING;
		else if (line.find (active->at_replacement().nickname + "'s Soundproof blocks") != std::string::npos)
			active->at_replacement().ability = SOUNDPROOF;
		else if (active->at_replacement().nickname + "'s Speed Boost raised its speed!" == line)
			active->at_replacement().ability = SPEED_BOOST;
		else if (active->at_replacement().nickname + " held on with its Sticky Hold!" == line)
			active->at_replacement().ability = STICKY_HOLD;
		else if (active->at_replacement().nickname + "'s Storm Drain drew the attack!" == line)
			active->at_replacement().ability = STORM_DRAIN;
		else if (active->at_replacement().nickname + " held Sturdy!" == line)
			active->at_replacement().ability = STURDY;
		else if (active->at_replacement().nickname + "'s Synchronize activated!" == line)
			active->at_replacement().ability = SYNCHRONIZE;
		else if (active->at_replacement().nickname + "'s Tangled Feet raised its evasion!" == line)
			active->at_replacement().ability = TANGLED_FEET;
		else if (line.find (active->at_replacement().nickname + " traced " + inactive->at_replacement().nickname + "'s ") == 0)
			active->at_replacement().ability = TRACE;
		else if (active->at_replacement().nickname + " is loafing around!" == line)
			active->at_replacement().ability = TRUANT;
		else if (active->at_replacement().nickname + " lost its burden!" == line)
			active->at_replacement().ability = UNBURDEN;
		else if (active->at_replacement().nickname + "'s Water Veil prevents burns!" == line)
			active->at_replacement().ability = WATER_VEIL;
		else if (active->at_replacement().nickname + " avoided damage with Wonder Guard!" == line)
			active->at_replacement().ability = WONDER_GUARD;
	}
	if (inactive->at_replacement().ability == END_ABILITY) {
		if (active->at_replacement().nickname + " was hurt by " + inactive->at_replacement().nickname + "'s Aftermath!" == line)
			inactive->at_replacement().ability = AFTERMATH;
		else if (active->at_replacement().nickname + "is tormented by the foe " +inactive->at_replacement().nickname + "'s Bad Dreams!" == line)
			inactive->at_replacement().ability = BAD_DREAMS;
		else if (active->at_replacement().nickname + " sucked up liquid ooze!" == line)
			inactive->at_replacement().ability = LIQUID_OOZE;
		else if (active->at_replacement().nickname + " was hurt by " + inactive->at_replacement().nickname + "'s Rough Skin!" == line)
			inactive->at_replacement().ability = ROUGH_SKIN;
		std::string search = active->at_replacement().nickname + " traced " + inactive->at_replacement().nickname + "'s ";
		if (line.find (search) != std::string::npos) {
			size_t n = 1;
			if (line.find(".\r") != std::string::npos)
				n = 2;
			inactive->at_replacement().ability = map.ability.find (line.substr (search.length(), line.length() - search.length() - n))->second;
		}
	}
	if (active->at_replacement().item == END_ITEM) {
		if (active->at_replacement().nickname + "'s Black Sludge restored a little health!" == line)
			active->at_replacement().item = BLACK_SLUDGE;
		else if (active->at_replacement().nickname + "'s Leftovers restored its health a little!" == line)
			active->at_replacement().item = LEFTOVERS;
		else if (active->at_replacement().nickname + "'s Quick Claw activated!" == line)
			active->at_replacement().item = QUICK_CLAW;
		else if (active->at_replacement().nickname + " became fully charged due to its Power Herb!" == line)
			active->at_replacement().item = POWER_HERB;
	}
}

void do_turn (Team & first, Team & last, Weather & weather) {
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

void Log::initialize_turn (Team &ai, Team &foe) {
	initialize_team (ai);
	initialize_team (foe);
	phaze = false;
	move_damage = false;
	
	active = NULL;		// GCC reports a potential use of this unitialized only when compiling with full optimizations. Variables unnecessarily set to NULL to remove this warning.
	inactive = NULL;
	first = NULL;
	last = NULL;
}

void Log::initialize_team (Team & team) {
	for (std::vector<Pokemon>::iterator pokemon = team.pokemon.set.begin(); pokemon != team.pokemon.set.end(); ++pokemon)
		pokemon->move.index = 0;
	team.ch = false;
	team.fully_paralyzed = false;
	team.hitself = false;
	team.miss = false;
	team.replacement = team.pokemon.index;
	team.replacing = false;
}

void normalize_hp (Team & first, Team & last) {
	normalize_hp_team (first);
	normalize_hp_team (last);
}

void normalize_hp_team (Team & team) {
	// This is to correct for rounding issues caused by only seeing the foe's HP to the nearest 48th.
	if (team.pokemon->fainted)
		team.pokemon->hp.stat = 0;
	else if (team.pokemon->hp.stat == 0)
		team.pokemon->hp.stat = 1;
}

}
