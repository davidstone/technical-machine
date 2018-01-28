// Copyright (C) 2018 David Stone
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

#pragma once

#include <cstdint>
#include <string>

#include "happiness.hpp"
#include "level.hpp"
#include "species.hpp"

#include "../ability.hpp"
#include "../gender.hpp"
#include "../item.hpp"
#include "../operators.hpp"
#include "../status.hpp"

#include "../move/container.hpp"

#include "../stat/nature.hpp"
#include "../stat/stats.hpp"

#include "../type/collection.hpp"

namespace technicalmachine {
// #define TECHNICALMACHINE_POKEMON_USE_NICKNAMES

struct Pokemon {
	static constexpr auto class_name = "Pokemon";
	Pokemon(TeamSize my_team_size, Species species, Level level, Gender gender, std::string const & nickname = std::string(), Happiness happiness = Happiness{});
	Pokemon(TeamSize my_team_size, Species species, Level level, Gender gender, Item const & item, Ability const & ability, Nature const & nature, std::string const & nickname = std::string(), Happiness happiness = Happiness{});
	operator Species() const;
	
	std::string const & get_nickname() const;

	// These cannot be defined in the class because because I rely on a
	// conversion operator. Friend functions only declared in a class body are
	// not found by lookup rules in that case.

	friend auto all_moves(Pokemon const & pokemon) -> MoveContainer const &;
	friend auto all_moves(Pokemon & pokemon) -> MoveContainer &;
	friend Ability const & get_ability(Pokemon const & pokemon);
	friend Ability & get_ability(Pokemon & pokemon);
	friend Gender const & get_gender(Pokemon const & pokemon);
	friend Gender & get_gender(Pokemon & pokemon);
	friend Happiness get_happiness(Pokemon const & pokemon);
	friend Item const & get_item(Pokemon const & pokemon);
	friend Item & get_item(Pokemon & pokemon);
	friend Level get_level(Pokemon const & pokemon);
	friend Nature const & get_nature(Pokemon const & pokemon);
	friend Nature & get_nature(Pokemon & pokemon);
	friend HP const & get_hp(Pokemon const & pokemon);
	friend HP & get_hp(Pokemon & pokemon);
	friend Stat const & get_stat(Pokemon const & pokemon, StatNames index_stat);
	friend Stat & get_stat(Pokemon & pokemon, StatNames index_stat);
	friend Status const & get_status(Pokemon const & pokemon);
	friend Status & get_status(Pokemon & pokemon);
	friend TypeCollection const & get_type(Pokemon const & pokemon);
	friend void switch_in(Pokemon & pokemon);

	void change_type(Type new_type);
	auto has_been_seen() const -> bool;

	// Species clause is assumed, and Pokemon will only be compared for equality
	// on the same team, so the same species implies many other things are the
	// same
	friend auto compare(Pokemon const & lhs, Pokemon const & rhs) {
		assert(illegal_inequality_check(lhs, rhs));
		auto as_tuple = [](auto const & value) {
			return containers::make_tuple(
				value.m_moves,
				value.m_species,
				value.m_status,
				get_hp(value).current(),
				value.m_item,
				value.has_been_seen()
			);
		};
		return compare(as_tuple(lhs), as_tuple(rhs));
	}


private:
	friend bool illegal_inequality_check(Pokemon const & lhs, Pokemon const & rhs);

	MoveContainer m_moves;
	
	#if defined TECHNICALMACHINE_POKEMON_USE_NICKNAMES
	std::string nickname;
	#endif
	TypeCollection current_type;

	Stats stats;

	Species m_species;
	Item m_item = Item::END;
	Ability m_ability;
	Gender m_gender;
	Status m_status;
	Nature m_nature = Nature::END;

	Level m_level;
	Happiness m_happiness;

	bool m_has_been_seen = false;
};

inline auto compare(Pokemon const & lhs, Species const rhs) {
	return bounded::compare(static_cast<Species>(lhs), rhs);
}
inline auto compare(Species const & lhs, Pokemon const & rhs) {
	return bounded::compare(lhs, static_cast<Species>(rhs));
}

inline auto all_moves(Pokemon const & pokemon) -> MoveContainer const & {
	return pokemon.m_moves;
}
inline auto all_moves(Pokemon & pokemon) -> MoveContainer & {
	return pokemon.m_moves;
}
inline decltype(auto) regular_moves(Pokemon const & pokemon) {
	return all_moves(pokemon).regular();
}
inline decltype(auto) regular_moves(Pokemon & pokemon) {
	return all_moves(pokemon).regular();
}

inline Ability const & get_ability(Pokemon const & pokemon) {
	return pokemon.m_ability;
}
inline Ability & get_ability(Pokemon & pokemon) {
	return pokemon.m_ability;
}

inline Gender const & get_gender(Pokemon const & pokemon) {
	return pokemon.m_gender;
}
inline Gender & get_gender(Pokemon & pokemon) {
	return pokemon.m_gender;
}

inline Item const & get_item(Pokemon const & pokemon) {
	return pokemon.m_item;
}
inline Item & get_item(Pokemon & pokemon) {
	return pokemon.m_item;
}

inline Nature const & get_nature(Pokemon const & pokemon) {
	return pokemon.m_nature;
}
inline Nature & get_nature(Pokemon & pokemon) {
	return pokemon.m_nature;
}

inline HP const & get_hp(Pokemon const & pokemon) {
	return pokemon.stats.hp();
}
inline HP & get_hp(Pokemon & pokemon) {
	return pokemon.stats.hp();
}

inline Stat const & get_stat(Pokemon const & pokemon, StatNames const index_stat) {
	return pokemon.stats[index_stat];
}
inline Stat & get_stat(Pokemon & pokemon, StatNames const index_stat) {
	return pokemon.stats[index_stat];
}

inline Status const & get_status(Pokemon const & pokemon) {
	return pokemon.m_status;
}
inline Status & get_status(Pokemon & pokemon) {
	return pokemon.m_status;
}

inline TypeCollection const & get_type(Pokemon const & pokemon) {
	return pokemon.current_type;
}

inline Level get_level(Pokemon const & pokemon) {
	return pokemon.m_level;
}

inline Happiness get_happiness(Pokemon const & pokemon) {
	return pokemon.m_happiness;
}

inline void switch_in(Pokemon & pokemon) {
	pokemon.m_has_been_seen = true;
}


std::string to_string(Pokemon const & pokemon, bool include_nickname = false);

inline auto hp_ratio(Pokemon const & pokemon) {
	auto const & hp = get_hp(pokemon);
	return make_rational(hp.current(), hp.max());
}

}	// namespace technicalmachine
