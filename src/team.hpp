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

#include <array>
#include <cstdint>
#include <random>
#include <string>

#include "entry_hazards.hpp"
#include "gender.hpp"
#include "pokemon.hpp"
#include "pokemon_collection.hpp"
#include "screens.hpp"
#include "species.hpp"
#include "stage.hpp"
#include "stat.hpp"
#include "vanish.hpp"
#include "wish.hpp"

#include "move/shared.hpp"

namespace technicalmachine {

class Team {
	public:
		Team ();
		Team (unsigned foe_size, std::mt19937 & random_engine, std::string const & team_file_name);
		Team (Team const & other);
		Team (Team && other);
		void add_pokemon (Species name);
		void add_pokemon (Species name, std::string const & nickname, unsigned level, Gender gender, uint8_t happiness = 255);
		void remove_pokemon ();
		bool can_be_phazed () const;
		bool operator== (Team const & other) const;
		uint64_t hash () const;
		std::string to_string () const;

		PokemonCollection pokemon;
	private:
		void load (std::string const & name, unsigned other_size);
		SharedMoves shared_moves;
	public:
		// How much damage has been done to this Pokemon this turn
		uint16_t damage;
		uint16_t bide_damage;
		uint16_t chance_to_hit;
		Stage stage;
		Vanish vanish;
		uint8_t bide;
		uint8_t confused;
		uint8_t embargo;
		uint8_t encore;
		uint8_t heal_block;
		uint8_t magnet_rise;
		// Number of turns remaining on Bind, Clamp, Fire Spin, Magma Storm,
		// Sand Tomb, Whirlpool, and Wrap
		uint8_t partial_trap;
		uint8_t perish_song;
		// Number of turns remaining on Outrage, Petal Dance, and Thrash
		uint8_t rampage;
		uint8_t slow_start;
		uint8_t stockpile;
		// How much HP does the substitute have left?
		uint8_t substitute;
		uint8_t taunt;
		// Number of turns this Pokemon has already taken Toxic damage (or
		// would have if Magic Guard / Poison Heal weren't in play)
		uint8_t toxic;
		// Number of turns remaining on Uproar
		uint8_t uproar;
		uint8_t yawn;
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
		uint8_t counter;

		Screens screens;
		Wish wish;
		
		EntryHazards entry_hazards;

		uint8_t called_move;
		
		// Is this my team?
		bool me;
};

}	// namespace technicalmachine
#endif	// TEAM_HPP_
