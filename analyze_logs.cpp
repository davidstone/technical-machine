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
#include <iostream>
#include <map>
#include <string>
#include "analyze_logs.h"
#include "ability.h"
#include "endofturn.h"
#include "move.h"
#include "movefunction.h"
#include "pokemon.h"
#include "switch.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

bool analyze_turn (Team &ai, Team &foe, Team* &first, Team* &last, Weather &weather, const Map &map) {
	first = NULL;
	std::cout << "Enter the log for the turn, followed by a ~.\n";
	std::string input;
	getline (std::cin, input, '~');		// Need to find a better way to signifiy end-of-turn. This works for now.
	std::cout << "======================\nAnalyzing...\n";
	bool won = false;
	if (input == "")
		won = true;
	else {
		ai.replacement = ai.active.index;
		foe.replacement = foe.active.index;
		bool first_replacing = false;
		bool last_replacing = false;
		bool phaze = false;
		bool move_damage = false;
		
		// active and inactive keep track of the Pokemon that are the "major" Pokemon of that line. This helps keep track of information on future lines so I can do things like assign critical hits to the right move.
		Team* active = NULL;		// GCC reports a potential use of this unitialized only when compiling with full optimizations. Variables unnecessarily set to NULL to remove this warning.
		Team* inactive = NULL;
		size_t newline1;
		size_t newline2 = -1;
		while (true) {
			newline1 = newline2 + 1;
			while (newline1 < input.length() and input.at (newline1) == '\n')
				++newline1;
			newline2 = input.find ('\n', newline1 + 1);
			if (newline2 == std::string::npos)
				break;
			std::string line = input.substr (newline1, newline2 - newline1);
			analyze_line (ai, foe, active, inactive, first, last, weather, map, phaze, first_replacing, last_replacing, move_damage, line);
		}
		do_turn (*first, *last, weather, first_replacing, last_replacing);
	}
	return won;
}

void analyze_line (Team &ai, Team &foe, Team* &active, Team* &inactive, Team* &first, Team* &last, Weather &weather, Map const &map, bool &phaze, bool &first_replacing, bool &last_replacing, bool &move_damage, std::string const &line) {
	if (line.find(": ") == std::string::npos) {		// Should ignore all comments, hopefully nobody puts : anywhere in their names
		std::cout << line << '\n';
		std::string search = "Begin turn #";
		if (line.substr (0, search.length()) == search)
			return;
		search = " sent out ";
		size_t found = line.find (search);
		if (found != std::string::npos) {
			search = ai.player + " sent out ";
			if (line.substr (0, search.length()) == search) {
				log_pokemon (ai, foe, weather, line, map, search, phaze);
				active = &ai;
				inactive = &foe;
				if (first == NULL) {
					first = &ai;
					last = &foe;
				}
			}
			else {
				if (foe.player == "") {
					foe.player = line.substr (0, found);
					first_replacing = true;
					last_replacing = true;
				}
				search = foe.player + " sent out ";
				log_pokemon (foe, ai, weather, line, map, search, phaze);
				active = &foe;
				inactive = &ai;
				if (first == NULL) {
					first = &foe;
					last = &ai;
				}
			}
		}
		else {
			if (foe.player != "") {
				search = ai.player + " withdrew ";
				if (line.substr (0, search.length()) == search)
					return;
				search = foe.player + " withdrew ";
				if (line.substr (0, search.length()) == search)
					return;
			}
			if (line.substr (0, ai.active.set [ai.replacement].nickname.length()) == ai.active.set [ai.replacement].nickname) {
				active = &ai;
				inactive = &foe;
			}
			else if (line.substr (0, foe.active.set [foe.replacement].nickname.length()) == foe.active.set [foe.replacement].nickname) {
				active = &foe;
				inactive = &ai;
			}
			if (first == NULL) {
				first = active;
				last = inactive;
			}

			// It's best to include both nicknames in the search instead of just the invariant section. This prevents any combination of nicknames from causing an error. A Pokemon cannot have its own nickname plus something else in its nickname.
			if (move_damage) {
				search = active->active.set [active->replacement].nickname + " lost ";
				if (line.substr (0, search.length()) == search) {
					size_t division = line.find ("/", search.length());
					if (division != std::string::npos) {
						int numerator = boost::lexical_cast<int> (line.substr (search.length(), division - search.length()));
						++division;
						int denominator = boost::lexical_cast<int> (line.substr (division, line.find(" ", division) - division));
						active->damage = active->active.set [active->replacement].hp.max * numerator / denominator;
						move_damage = false;
					}
				}
				else if (line == "It had no effect.")
					move_damage = false;
			}
			if (line == "A critical hit!")
				active->active.set [active->replacement].move->ch = true;
			else if (line == active->active.set [active->replacement].nickname + " flinched!")
				active->flinch = true;
			else if (line == first->active.set [first->replacement].nickname + " fainted!")
				first_replacing = true;
			else if (line == last->active.set [last->replacement].nickname + " fainted!")
				last_replacing = true;
			else {
				search = active->active.set [active->replacement].nickname + " used ";
				if (line.substr (0, search.length()) == search)
					log_move (*active, *inactive, weather, line, map, search, phaze, move_damage);
				// else if (line != "===============") {
				else {
					bool shed_skin = false;
					log_misc (*active, inactive->active.set [inactive->replacement], line, map, shed_skin);		// Fix
				}
			}
		}
	}
}

void log_pokemon  (Team &team, Team &target, Weather &weather, const std::string &line, const Map &map, const std::string &search1, bool &phaze) {
	std::string search2 = " (lvl ";
	size_t found2 = line.find (search2);
	std::string nickname = line.substr (search1.length(), found2 - search1.length());

	bool found = false;
	for (team.replacement = 0; team.replacement != team.active.set.size(); ++team.replacement) {
		if (nickname == team.active.set [team.replacement].nickname) {
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

		member.level = boost::lexical_cast<int> (line.substr (found2 + search2.length(), found3 - found2 - search2.length()));

		member.nature = HARDY;
		team.active.set.push_back (member);
		team.replacement = team.active.set.size() - 1;
		
		for (std::vector<Pokemon>::iterator active = team.active.set.begin(); active != team.active.set.end(); ++active)
			loadpokemon (team, *active);
	}
	if (phaze) {
		target.active->move->variable.index = 0;
		while (team.active.set [target.active->move->variable.index].nickname != nickname)
			++target.active->move->variable.index;
	}
	else {
		team.active->move.index = 0;
		if (team.active->move.set.size() > 1) {
			while (team.active->move->name != SWITCH0)
				++team.active->move.index;
		}
	}
	team.active->move.index += team.replacement;
}


void log_move (Team &user, Team &target, Weather &weather, const std::string &line, const Map &map, const std::string &search, bool &phaze, bool &move_damage) {
	// Account for Windows / Unix line endings
	size_t n = 1;
	if (line.find(".\r") != std::string::npos)
		n = 2;
	moves_list name = map.move.find (line.substr (search.length(), line.length() - search.length() - n))->second;
	bool isfound = false;
	for (user.active->move.index = 0; user.active->move.index != user.active->move.set.size(); ++user.active->move.index) {
		if (name == user.active->move->name) {
			isfound = true;
			break;
		}
	}
	if (!isfound) {
		Move move (name, 3);
		user.active->move.set.insert (user.active->move.set.begin(), move);
		user.active->move.index = 0;
	}
	if (user.active->move->name == ROAR or user.active->move->name == WHIRLWIND)
		phaze = true;
	user.active->move->ch = false;
	user.active->move->effect = false;
	if (user.active->move->basepower != 0)
		move_damage = true;
}


void log_misc (Team &user, Pokemon &inactive, const std::string &line, const Map &map, bool &shed_skin) {
	if (user.active->ability == END_ABILITY) {
		if (user.active->nickname + "'s Anger Point raised its attack!" == line) {
			user.active->ability = ANGER_POINT;
			user.active->atk.stage = 6;
		}
		else if (user.active->nickname + "'s Anticipation made it shudder!" == line)
			user.active->ability = ANTICIPATION;
		else if (user.active->nickname + "'s Cute Charm infatuated " + inactive.nickname == line) {
			inactive.ability = CUTE_CHARM;
			user.attract = true;
		}
		else if (user.active->nickname + "'s Damp prevents explosions!" == line)
			user.active->ability = DAMP;
		else if (user.active->nickname + "'s Download raised its stats!" == line)
			user.active->ability = DOWNLOAD;
		else if (user.active->nickname + "'s Drizzle caused a storm!" == line)
			user.active->ability = DRIZZLE;
		else if (user.active->nickname + "'s Drought intensified the sun's rays!" == line)
			user.active->ability = DROUGHT;
		else if (user.active->nickname + " soaked up rain!" == line or user.active->nickname + "'s Dry Skin absorbed damage!" == line or user.active->nickname + "'s Dry Skin restored its health a little!" == line)	// Not sure which is correct
			user.active->ability = DRY_SKIN;
		else if (user.active->nickname + " was hurt by the sunlight!" == line)
			{/* Dry Skin and Solar Power. Need a way to distinguish them. */}
		else if (user.active->nickname + "'s Flame Body burned " + inactive.nickname == line)
			user.active->ability = FLAME_BODY;
		else if (user.active->nickname + "'s Flash Fire raised its fire power!" == line)
			user.active->ability = FLASH_FIRE;
		else if (line.find (user.active->nickname + "Forewarn alerted it to ") != std::string::npos)	// Fix to get the full information of Forewarn
			user.active->ability = FOREWARN;
		else if (line.find (user.active->nickname + " found " + inactive.nickname + "'s ") != std::string::npos)	// Fix to get full information of Frisk
			user.active->ability = FRISK;
		else if (user.active->nickname + "'s status was cured!" == line)
			user.active->ability = HYDRATION;
		else if (user.active->nickname + "absorbed the hail!" == line or user.active->nickname + "'s Ice Body restored its health a little!" == line)	// Not sure which is correct
			user.active->ability = ICE_BODY;
		else if (user.active->nickname + "'s Immunity prevents poisoning!" == line)
			user.active->ability = IMMUNITY;
		else if (user.active->nickname + "'s Insomnia kept it awake!" == line)
			user.active->ability = INSOMNIA;
		else if (user.active->nickname + "'s Intimidate cut " + inactive.nickname + "'s attack!" == line)
			user.active->ability = INTIMIDATE;
		else if (user.active->nickname + " makes ground moves miss with Levitate!" == line)
			user.active->ability = LEVITATE;
		else if (user.active->nickname + "'s Lightningrod drew the attack!" == line)
			user.active->ability = LIGHTNINGROD;
		else if (user.active->nickname + " has Mold Breaker!" == line)
			user.active->ability = MOLD_BREAKER;
		else if (user.active->nickname + "'s Motor Drive increased its speed!" == line)
			user.active->ability = MOTOR_DRIVE;
		else if (user.active->nickname + "'s Oblivious prevents attraction!" == line)
			user.active->ability = OBLIVIOUS;
		else if (user.active->nickname + "'s Poison Heal restored health!" == line)
			user.active->ability = POISON_HEAL;
		else if (user.active->nickname + " is exerting its pressure!" == line)
			user.active->ability = PRESSURE;
		else if (user.active->nickname + "'s Rain Dish restored its health a little!" == line)
			user.active->ability = RAIN_DISH;
		else if (user.active->nickname + "'s Sand Stream whipped up a sandstorm!" == line)
			user.active->ability = SAND_STREAM;
		else if (user.active->nickname + " shed its skin!" == line) {
			user.active->ability = SHED_SKIN;
			shed_skin = true;
		}
		else if (user.active->nickname + " can't get going due to its Slow Start!" == line)
			user.active->ability = SLOW_START;
		else if (user.active->nickname + "'s Snow Warning whipped up a hailstorm!" == line or user.active->nickname + "'s Snow Warning whipped up a hailstorm! " == line)
			user.active->ability = SNOW_WARNING;
		else if (line.find (user.active->nickname + "'s Soundproof blocks") != std::string::npos)
			user.active->ability = SOUNDPROOF;
		else if (user.active->nickname + "'s Speed Boost raised its speed!" == line)
			user.active->ability = SPEED_BOOST;
		else if (user.active->nickname + " held on with its Sticky Hold!" == line)
			user.active->ability = STICKY_HOLD;
		else if (user.active->nickname + "'s Storm Drain drew the attack!" == line)
			user.active->ability = STORM_DRAIN;
		else if (user.active->nickname + " held Sturdy!" == line)
			user.active->ability = STURDY;
		else if (user.active->nickname + "'s Synchronize activated!" == line)
			user.active->ability = SYNCHRONIZE;
		else if (user.active->nickname + "'s Tangled Feet raised its evasion!" == line)
			user.active->ability = TANGLED_FEET;
		else if (line.find (user.active->nickname + " traced " + inactive.nickname + "'s ") == 0)
			user.active->ability = TRACE;
		else if (user.active->nickname + " is loafing around!" == line)
			user.active->ability = TRUANT;
		else if (user.active->nickname + " lost its burden!" == line)
			user.active->ability = UNBURDEN;
		else if (user.active->nickname + "'s Water Veil prevents burns!" == line)
			user.active->ability = WATER_VEIL;
		else if (user.active->nickname + " avoided damage with Wonder Guard!" == line)
			user.active->ability = WONDER_GUARD;
	}
	if (inactive.ability == END_ABILITY) {
		if (user.active->nickname + " was hurt by  " + inactive.nickname + "'s Aftermath!" == line)
			inactive.ability = AFTERMATH;
		else if (user.active->nickname + "is tormented by the foe " +inactive.nickname + "'s Bad Dreams!" == line)
			inactive.ability = BAD_DREAMS;
		else if (user.active->nickname + " sucked up liquid ooze!" == line)
			inactive.ability = LIQUID_OOZE;
		else if (user.active->nickname + " was hurt by " + inactive.nickname + "'s Rough Skin!" == line)
			inactive.ability = ROUGH_SKIN;
		std::string search = user.active->nickname + " traced " + inactive.nickname + "'s ";
		if (line.find (search) != std::string::npos) {
			size_t n = 1;
			if (line.find(".\r") != std::string::npos)
				n = 2;
			inactive.ability = map.ability.find (line.substr (search.length(), line.length() - search.length() - n))->second;
		}
	}
	if (user.active->item == END_ITEM) {
		if (user.active->nickname + "'s Black Sludge restored a little health!" == line)
			user.active->item = BLACK_SLUDGE;
		else if (user.active->nickname + "'s leftovers restored its health a little!" == line)
			user.active->item = LEFTOVERS;
		else if (user.active->nickname + "'s Quick Claw activated!" == line)
			user.active->item = QUICK_CLAW;
		else if (user.active->nickname + " became fully charged due to its Power Herb!" == line)
			user.active->item = NO_ITEM;
	}
}

void do_turn (Team &first, Team &last, Weather &weather, bool first_replacing, bool last_replacing) {
	if (first_replacing or last_replacing) {
		if (first_replacing) {
			// Hopefully this will catch minor rounding errors and nothing else.
			if (first.active->hp.stat != first.active->hp.max)
				first.active->hp.stat = 0;
			switchpokemon (first, last, weather);
			first.moved = false;
		}
		if (last_replacing) {
			if (last.active->hp.stat != last.active->hp.max)
				last.active->hp.stat = 0;
			switchpokemon (last, first, weather);
			last.moved = false;
		}
	}
	else {
		if (first.replacement != first.active.index) {
			first.active->move.index = 0;
			while (first.active->move->name - SWITCH0 != first.replacement)
				++first.active->move.index;
		}
		if (last.replacement != last.active.index) {
			last.active->move.index = 0;
			while (last.active->move->name - SWITCH0 != last.replacement)
				++last.active->move.index;
		}
		usemove (first, last, weather, last.damage);
		usemove (last, first, weather, first.damage);
		std::cout << static_cast<int> (first.spikes) << '\n';
		std::cout << static_cast<int> (last.spikes) << '\n';
		endofturn (first, last, weather);
	}
	std::string out;
	output (first, out);
	std::cout << out;
	output (last, out);
	std::cout << out;
}

}
