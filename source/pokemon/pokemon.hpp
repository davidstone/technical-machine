// Pokemon data structure
// Copyright (C) 2014 David Stone
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

#include "happiness.hpp"
#include "level.hpp"
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

// I use a macro here because I rely on a conversion operator. Friend functions
// only declared in a class body are not found by lookup rules in that case. The
// macro solution seemed better than duplicating all of this by hand.
//
// qualifier should be `friend` or nothing
#define TECHNICALMACHINE_FRIEND_DECLARATIONS(qualifier) \
qualifier Ability const & get_ability(Pokemon const & pokemon); \
qualifier Ability & get_ability(Pokemon & pokemon); \
qualifier Gender const & get_gender(Pokemon const & pokemon); \
qualifier Gender & get_gender(Pokemon & pokemon); \
qualifier Happiness get_happiness(Pokemon const & pokemon); \
qualifier Item const & get_item(Pokemon const & pokemon); \
qualifier Item & get_item(Pokemon & pokemon); \
qualifier Level get_level(Pokemon const & pokemon); \
qualifier Nature const & get_nature(Pokemon const & pokemon); \
qualifier Nature & get_nature(Pokemon & pokemon); \
qualifier HP const & get_hp(Pokemon const & pokemon); \
qualifier HP & get_hp(Pokemon & pokemon); \
qualifier Stat const & get_stat(Pokemon const & pokemon, StatNames index_stat); \
qualifier Stat & get_stat(Pokemon & pokemon, StatNames index_stat); \
qualifier Status const & get_status(Pokemon const & pokemon); \
qualifier Status & get_status(Pokemon & pokemon); \
qualifier TypeCollection const & get_type(Pokemon const & pokemon); \
qualifier void switch_in(Pokemon & pokemon)

TECHNICALMACHINE_FRIEND_DECLARATIONS(/*empty*/);


class Pokemon {
public:
	Pokemon(TeamSize my_team_size, Species species, Level level, Gender gender, std::string const & nickname = std::string(), Happiness happiness = Happiness{});
	Pokemon(TeamSize my_team_size, Species species, Level level, Gender gender, Item const & item, Ability const & ability, Nature const & nature, std::string const & nickname = std::string(), Happiness happiness = Happiness{});
	operator Species() const;
	void remove_switch();
	MoveCollection::index_type index_of_first_switch () const;
	std::string get_nickname () const;
	
	TECHNICALMACHINE_FRIEND_DECLARATIONS(friend);
	void change_type(Type new_type);
	auto has_been_seen() const -> bool;
	typedef uint64_t hash_type;
	hash_type hash () const;
	hash_type max_hash() const;
	friend bool operator== (Pokemon const & lhs, Pokemon const & rhs);

	MoveCollection move;
	
private:
	friend bool illegal_inequality_check(Pokemon const & lhs, Pokemon const & rhs);
	#if defined TECHNICALMACHINE_POKEMON_USE_NICKNAMES
	std::string nickname;
	#endif
	TypeCollection current_type;

	Stats stats;

	Species m_species;
	Item m_item;
	Ability m_ability;
	Gender m_gender;
	Status m_status;
	Nature m_nature;

	Level m_level;
	Happiness m_happiness;

	bool m_has_been_seen = false;
};
bool operator!= (Pokemon const & lhs, Pokemon const & rhs);

#undef TECHNICALMACHINE_FRIEND_DECLARATIONS


std::string to_string(Pokemon const & pokemon, bool include_nickname = false);

inline auto hp_ratio(Pokemon const & pokemon) {
	auto const & hp = get_hp(pokemon);
	return make_rational(hp.current(), hp.max());
}

}	// namespace technicalmachine
#endif	// POKEMON__POKEMON_HPP_
