// Pokemon data structure
// Copyright (C) 2013 David Stone
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
#include "species_forward.hpp"

#include "../ability.hpp"
#include "../gender.hpp"
#include "../item.hpp"
#include "../status.hpp"

#include "../move/collection.hpp"

#include "../stat/nature.hpp"
#include "../stat/stats.hpp"

#include "../type/collection.hpp"

namespace technicalmachine {
class Rational;
// #define TECHNICALMACHINE_POKEMON_USE_NICKNAMES

class Pokemon {
public:
	Pokemon(unsigned my_team_size, Species species, uint8_t set_level, Gender set_gender, std::string const & set_nickname = std::string(), uint8_t set_happiness = 255);
	Pokemon(unsigned my_team_size, Species species, uint8_t set_level, Gender set_gender, Item const & set_item, Ability const & set_ability, Nature const & set_nature, std::string const & set_nickname = std::string(), uint8_t set_happiness = 255);
	operator Species() const;
	void switch_in();
	void remove_switch();
	uint8_t index_of_first_switch () const;
	std::string get_nickname () const;
	Species name() const;
	Ability const & ability() const;
	Ability & ability();
	Gender const & gender() const;
	Gender & gender();
	Item const & item() const;
	Item & item();
	Nature const & nature() const;
	Nature & nature();
	Stat const & stat(Stat::Stats index_stat) const;
	Stat & stat(Stat::Stats index_stat);
	Status const & status() const;
	Status & status();
	TypeCollection const & type() const;
	void change_type(Type::Types new_type);
	unsigned level() const;
	unsigned happiness() const;
	// This function should be used instead of checking if hp == 0 to handle
	// messages being sent about multiple Pokemon fainting in one turn.
	// Using this function will allow TM to correctly update an entire turn
	// from a message.
	bool is_fainted() const;
	bool will_be_replaced() const;
	void faint();
	void reset_replacing();
	typedef uint64_t hash_type;
	hash_type hash () const;
	hash_type max_hash() const;
	friend bool operator== (Pokemon const & lhs, Pokemon const & rhs);
	friend bool illegal_inequality_check(Pokemon const & lhs, Pokemon const & rhs);

	MoveCollection move;
	
private:
	#if defined TECHNICALMACHINE_POKEMON_USE_NICKNAMES
	std::string nickname;
	#endif
	TypeCollection current_type;

	Stats stats;

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
	friend class Evaluate;
};
bool operator!= (Pokemon const & lhs, Pokemon const & rhs);

bool is_alternate_form(Species first, Species second);
std::string to_string(Pokemon const & pokemon, bool include_nickname = false);

void switch_out(Pokemon & pokemon);

void calculate_initial_hp(Pokemon & pokemon);
// The server reports Technical Machine's HP tracking is wrong
void correct_error_in_hp(Pokemon & pokemon, unsigned correct_hp_stat);
Rational current_hp(Pokemon const & pokemon);

bool can_confuse_with_chatter(Species pokemon);
bool can_use_substitute(Pokemon const & pokemon);

bool is_boosted_by_adamant_orb(Species species);
bool is_boosted_by_deepseascale(Species species);
bool is_boosted_by_deepseatooth(Species species);
bool is_boosted_by_griseous_orb(Species species);
bool is_boosted_by_light_ball(Species species);
bool is_boosted_by_lustrous_orb(Species species);
bool is_boosted_by_metal_powder(Species species);
bool is_boosted_by_quick_powder(Species species);
bool is_boosted_by_soul_dew(Species species);
bool is_boosted_by_thick_club(Species species);

unsigned power_of_mass_based_moves(Species species);

}	// namespace technicalmachine
#endif	// POKEMON__POKEMON_HPP_
