// Team data structure
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

#ifndef TEAM_HPP_
#define TEAM_HPP_

#include <cstdint>
#include <random>
#include <string>

#include "active.hpp"
#include "gender.hpp"
#include "pokemon.hpp"
#include "species.hpp"
#include "stat.hpp"

namespace technicalmachine {

class Team {
	public:
		Active<Pokemon> pokemon;

		Vanish vanish;
		// How much damage will this Pokemon do
		int16_t damage;
		int16_t bide_damage;
		int16_t chance_to_hit;
		// -6 though 6
		int8_t stage [Stat::END];
		int8_t bide;
		int8_t confused;
		int8_t embargo;
		int8_t encore;
		int8_t heal_block;
		int8_t magnet_rise;
		// Number of turns remaining on Bind, Clamp, Fire Spin, Magma Storm,
		// Sand Tomb, Whirlpool, and Wrap
		int8_t partial_trap;
		int8_t perish_song;
		// Number of turns remaining on Outrage, Petal Dance, and Thrash
		int8_t rampage;
		int8_t slow_start;
		int8_t stockpile;
		// How much HP does the substitute have left?
		int8_t substitute;
		int8_t taunt;
		// Number of turns this Pokemon has already taken Toxic damage (or
		// would have if Magic Guard / Poison Heal weren't in play)
		int8_t toxic;
		// Number of turns remaining on Uproar
		int8_t uproar;
		int8_t yawn;
		bool aqua_ring;
		bool attract;
		// Will it wake up
		bool awaken;
		bool ch;
		bool charge;
		bool curse;
		bool damaged;
		bool defense_curl;
		bool destiny_bond;
		bool endure;
		bool flash_fire;
		bool flinch;
		bool focus_energy;
		bool fully_paralyzed;
		bool gastro_acid;
		// Will this Pokemon hit itself in its confusion this turn?
		bool hitself;
		bool identified;
		// Has this Pokemon used Imprison?
		bool imprison;
		bool ingrain;
		bool leech_seed;
		bool loaf;
		bool lock_on;
		bool me_first;
		bool minimize;
		bool miss;
		bool moved;
		bool mud_sport;
		bool nightmare;
		bool pass;
		bool power_trick;
		bool protect;
		bool recharging;
		// Replacing a fainted Pokemon. Also used for initial switch-in at
		// start of battle.
		bool replacing;
		bool roost;
		bool shed_skin;
		bool torment;
		// Block, Mean Look, Spider Web
		bool trapped;
		bool u_turning;
		bool water_sport;
		
		// Attacker that used Doom Desire / Future Sight
		// Pokemon ddfs;
		// Set to 3 initially, 1 = delayed attack hits at the end of this turn,
		// 0 = not active
		int8_t counter;

		// Number of turns remaining
		int8_t light_screen;
		int8_t lucky_chant;
		int8_t mist;
		int8_t reflect;
		int8_t safeguard;
		int8_t tailwind;

		// 2 = Wish will heal next turn, 1 = Wish will heal this turn, 0 = Wish
		// is not in play
		int8_t wish;
			
		// The entry hazards represent those down on this team's side of the
		// field. Using an entry hazard puts them down on the opponent's side
		// of the field.
		int8_t spikes;
		int8_t toxic_spikes;
		bool stealth_rock;

		// If a Pokemon switches / faints, what Pokemon should replace it?
		uint8_t replacement;

		uint8_t called_move;
		
		// How big is the team?
		uint8_t size;
		
		// Is this my team?
		bool me;
		
		Team ();
		Team (unsigned foe_size, std::mt19937 & random_engine, std::string const & team_file_name);
		bool operator== (Team const & other) const;
		Pokemon& at_replacement ();
		Pokemon const & at_replacement () const;
		bool is_switching_to_self () const;
		// Has this Pokemon been seen already? If it has, set replacement.
		bool seen_pokemon (Species name);
		void add_pokemon (Species name, std::string const & nickname, int level, Gender gender);
		uint64_t hash () const;
		std::string to_string () const;
	private:
		void load (std::string const & name, unsigned other_size);
};

}	// namespace technicalmachine
#endif	// TEAM_HPP_
