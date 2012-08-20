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

#include "entry_hazards.hpp"
#include "screens.hpp"
#include "substitute.hpp"
#include "taunt.hpp"
#include "vanish.hpp"
#include "wish.hpp"

#include "bide/bide.hpp"

#include "move/shared.hpp"

#include "pokemon/collection.hpp"

#include "stat/chance_to_hit.hpp"
#include "stat/stage.hpp"

namespace technicalmachine {
enum class Species : uint16_t;
class Ability;
class Weather;

class Team {
	public:
		Team ();
		Team (unsigned foe_size, std::mt19937 & random_engine, std::string const & team_file_name);
		Team (Team const & other);
		Team (Team && other);
		Team & operator= (Team const & other);
		Team & operator= (Team && other);
		template<class... Args>
		void add_pokemon(Args&&... args) {
			pokemon.add(shared_moves, std::forward<Args>(args)...);
		}
		void remove_pokemon ();
		
		// Not for variables that give a message at the end of the turn, this is
		// just for some book-keeping variables.
		void reset_end_of_turn();
		void reset_switch();
		void substitute();
		void lower_pp(Ability const & target);
		void taunt();
		bool is_taunted() const;
		void increment_taunt();
		void use_bide(Pokemon & target);
		bool is_locked_in_to_bide() const;
		bool can_be_phazed () const;
		unsigned damaged() const;
		void do_damage(unsigned damage);
		bool has_switched() const;
		void update_chance_to_hit(Team const & target, Weather const & weather);
		ChanceToHit::value_type chance_to_hit() const;
		ChanceToHit::value_type chance_to_miss() const;
		bool can_miss() const;

		friend bool operator== (Team const & lhs, Team const & rhs);
		uint64_t hash () const;
		std::string to_string () const;

	public:
		PokemonCollection pokemon;
	private:
		friend class Score;
		void load (std::string const & name, unsigned other_size);

		SharedMoves shared_moves;
		uint16_t damage_done_to_active = 0;
		Bide bide;
	public:
		Stage stage;
		Vanish vanish;
	private:
		ChanceToHit cached_chance_to_hit;
	public:
		uint8_t confused = 0;
		uint8_t embargo = 0;
		uint8_t encore = 0;
		uint8_t heal_block = 0;
		uint8_t magnet_rise = 0;
		// Number of turns remaining on Bind, Clamp, Fire Spin, Magma Storm,
		// Sand Tomb, Whirlpool, and Wrap
		uint8_t partial_trap = 0;
		uint8_t perish_song = 0;
		// Number of turns remaining on Outrage, Petal Dance, and Thrash
		uint8_t rampage = 0;
		uint8_t slow_start = 0;
		uint8_t stockpile = 0;
	private:
		Substitute active_substitute;
		Taunt m_taunt;
	public:
		// Number of turns this Pokemon has already taken Toxic damage (or
		// would have if Magic Guard / Poison Heal weren't in play)
		uint8_t toxic = 0;
		// Number of turns remaining on Uproar
		uint8_t uproar = 0;
		uint8_t yawn = 0;
		bool aqua_ring = false;
		bool attract = false;
		// Will it wake up
		bool awaken = false;
		bool ch = false;
		bool charge = false;
		bool curse = false;
		bool defense_curl = false;
		bool destiny_bond = false;
		bool endure = false;
		bool flash_fire = false;
		bool flinch = false;
		bool focus_energy = false;
		bool fully_paralyzed = false;
		bool gastro_acid = false;
		// Will this Pokemon hit itself in its confusion this turn?
		bool hitself = false;
		bool identified = false;
		// Has this Pokemon used Imprison?
		bool imprison = false;
		bool ingrain = false;
		bool leech_seed = false;
		bool loaf = false;
		bool lock_on = false;
		bool me_first = false;
		bool minimize = false;
		bool miss = false;
		bool moved = false;
		bool mud_sport = false;
		bool nightmare = false;
		bool pass = false;
		bool power_trick = false;
		bool protect = false;
		bool recharging = false;
		// Replacing a fainted Pokemon. Also used for initial switch-in at
		// start of battle.
		bool replacing = false;
		bool roost = false;
		bool shed_skin = false;
		bool torment = false;
		// Block, Mean Look, Spider Web
		bool trapped = false;
		bool u_turning = false;
		bool water_sport = false;
		
		// Attacker that used Doom Desire / Future Sight
		// Pokemon ddfs;
		// Set to 3 initially, 1 = delayed attack hits at the end of this turn,
		// 0 = not active
		uint8_t counter = 0;

		Screens screens;
		Wish wish;
		
		EntryHazards entry_hazards;

		uint8_t called_move = 0;
		
		// Is this my team?
		bool me;
};
bool operator!= (Team const & lhs, Team const & rhs);

}	// namespace technicalmachine
#endif	// TEAM_HPP_
