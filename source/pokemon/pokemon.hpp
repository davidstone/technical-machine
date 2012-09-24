// Pokemon data structure
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

#ifndef POKEMON__POKEMON_HPP_
#define POKEMON__POKEMON_HPP_

#include <cstdint>
#include <string>

#include "level.hpp"
#include "seen.hpp"

#include "../ability.hpp"
#include "../gender.hpp"
#include "../item.hpp"
#include "../status.hpp"

#include "../move/collection.hpp"

#include "../stat/nature.hpp"
#include "../stat/stat.hpp"

#include "../type/collection.hpp"

namespace technicalmachine {
enum class Species : uint16_t;
class SharedMoves;
// #define TECHNICALMACHINE_POKEMON_USE_NICKNAMES

class Pokemon {
	public:
		Pokemon(SharedMoves & shared, Species species, uint8_t set_level, Gender set_gender, std::string const & set_nickname = std::string(), uint8_t set_happiness = 255);
		Pokemon(SharedMoves & shared, Species species, uint8_t set_level, Gender set_gender, Item const & set_item, Ability const & set_ability, Nature const & set_nature, std::string const & set_nickname = std::string(), uint8_t set_happiness = 255);
		void switch_in();
		void switch_out();
		void calculate_initial_hp ();
		uint8_t index_of_first_switch () const;
		// Fix any rounding issues caused by not seeing the foe's exact HP.
		void normalize_hp ();
		uint8_t power_of_mass_based_moves() const;
		std::string to_string () const;
		std::string get_nickname () const;
		bool can_use_chatter() const;
		bool can_use_substitute() const;
		bool is_boosted_by_adamant_orb() const;
		bool is_boosted_by_deepseascale() const;
		bool is_boosted_by_deepseatooth() const;
		bool is_boosted_by_griseous_orb() const;
		bool is_boosted_by_light_ball() const;
		bool is_boosted_by_lustrous_orb() const;
		bool is_boosted_by_metal_powder() const;
		bool is_boosted_by_quick_powder() const;
		bool is_boosted_by_soul_dew() const;
		bool is_boosted_by_thick_club() const;
		void set_hidden_power_type ();
		Species name() const;
		Ability const & ability() const;
		Ability & ability();
		Gender const & gender() const;
		Gender & gender();
		Item const & item() const;
		Item & item();
		Nature const & nature() const;
		Nature & nature();
		Status const & status() const;
		Status & status();
		TypeCollection const & type() const;
		void change_type(Type::Types new_type);
		unsigned level() const;
		unsigned happiness() const;
		bool has_been_seen() const;
		// This function should be used instead of checking if hp == 0 to handle
		// messages being sent about multiple Pokemon fainting in one turn.
		// Using this function will allow TM to correctly update an entire turn
		// from a message.
		bool will_be_replaced() const;
		void faint();
		typedef uint64_t hash_type;
		hash_type hash () const;
		hash_type max_hash() const;
		friend bool operator== (Pokemon const & lhs, Pokemon const & rhs);
		friend bool illegal_inequality_check(Pokemon const & lhs, Pokemon const & rhs);

		MoveCollection move;
		TypeCollection current_type;
//	private:
		#if defined TECHNICALMACHINE_POKEMON_USE_NICKNAMES
		std::string nickname;
		#endif

		Stat hp;
		Stat atk;
		Stat def;
		Stat spa;
		Stat spd;
		Stat spe;

		// 0 through 48 for foes, used to keep the HP learned from the log on track with reality
		uint16_t new_hp;

		Species m_name;
		Item m_item;
		Ability m_ability;
		Gender m_gender;
		Status m_status;
		Nature m_nature;

		bool m_will_be_replaced;
		Seen seen;
		Level m_level;
		uint8_t m_happiness;
		Type::Types calculate_hidden_power_type () const;
		friend class Score;
};
bool operator!= (Pokemon const & lhs, Pokemon const & rhs);

bool is_alternate_form(Species first, Species second);

}	// namespace technicalmachine
#endif	// POKEMON__POKEMON_HPP_
