// Stats data structures
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

#ifndef STAT_HPP_
#define STAT_HPP_

#include <cstdint>
#include <stdexcept>
#include <string>

namespace technicalmachine {
class Pokemon;
class Team;
class Weather;
enum class Species : uint16_t;

class Stat {
	public:
		// I set HP to -1 so it doesn't get in the way of indexing stat boosts in an array.
		enum Stats {
			HP = -1,
			ATK,
			DEF,
			SPA,
			SPD,
			SPE,
			ACC,
			EVA,
			END
		};
		typedef uint16_t stat_type;
		stat_type max;		// Max HP only
		stat_type stat;		// Current HP or last calculated value for other stats
		uint8_t base;
		uint8_t iv;			// 0 through 31
		uint8_t ev;			// 0 through 63
	
		Stat (Species name, Stats stat);
		void calculate_initial_hp (uint8_t const level);
		static void calculate_attack (Team & attacker, Weather const & weather);
		static void calculate_special_attack (Team & attacker, Weather const & weather);
		static void calculate_defense (Team & defender, bool ch = false, bool is_self_KO = false);
		static void calculate_special_defense (Team & defender, Weather const & weather, bool ch = false);
		static void calculate_speed (Team & team, Weather const & weather);
		static void chance_to_hit (Team & user, Team const & target, Weather const & weather);
	private:
		static uint8_t get_base_stat (Species name, Stats stat_name);
};

class InvalidStat : public std::runtime_error {
	public:
		explicit InvalidStat (std::string const & stat_string):
			std::runtime_error ("Invalid stat of " + stat_string + " requested.\n")
			{
		}
};

void calculate_attacking_stat (Team & attacker, Weather const & weather);
void calculate_defending_stat (Team const & attacker, Team & defender, Weather const & weather);

void order (Team & team1, Team & team2, Weather const & weather, Team* & faster, Team* & slower);
void faster_pokemon (Team & team1, Team & team2, Weather const & weather, Team* & faster, Team* & slower);

}	// namespace technicalmachine
#endif	// STAT_HPP_
