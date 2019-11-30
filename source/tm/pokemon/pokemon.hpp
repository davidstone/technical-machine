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

#include <tm/pokemon/happiness.hpp>
#include <tm/pokemon/hidden_power.hpp>
#include <tm/pokemon/level.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <tm/ability.hpp>
#include <tm/gender.hpp>
#include <tm/item.hpp>
#include <tm/operators.hpp>
#include <tm/status.hpp>
#include <tm/weather.hpp>

#include <tm/move/container.hpp>

#include <tm/stat/nature.hpp>
#include <tm/stat/stats.hpp>

#include <tm/type/pokemon_types.hpp>

#include <bounded/assert.hpp>

#include <containers/string.hpp>

#include <cstdint>

namespace technicalmachine {

enum class Generation : std::uint8_t;

struct Pokemon {
	Pokemon(Generation, TeamSize my_team_size, Species species, Level level, Gender gender, Happiness happiness = Happiness{});
	Pokemon(Generation, TeamSize my_team_size, Species species, Level level, Gender gender, Item item, Ability ability, Nature nature, Happiness happiness = Happiness{});
	
	// These cannot be defined in the class because because I rely on a
	// conversion operator. Friend functions only declared in a class body are
	// not found by lookup rules in that case.

	friend auto all_moves(Pokemon const & pokemon) -> MoveContainer const &;
	friend Ability get_ability(Pokemon pokemon);
	friend Gender get_gender(Pokemon pokemon);
	friend Happiness get_happiness(Pokemon pokemon);
	friend HiddenPower get_hidden_power(Pokemon pokemon);
	friend Item get_item(Pokemon pokemon);
	friend Level get_level(Pokemon pokemon);
	friend Nature get_nature(Pokemon pokemon);
	friend Species get_species(Pokemon pokemon);
	friend HP get_hp(Pokemon pokemon);
	friend Stat get_stat(Pokemon pokemon, StatNames index_stat);
	friend Status get_status(Pokemon pokemon);
	friend PokemonTypes get_type(Pokemon pokemon);

	void set_status(Statuses const status) & {
		if (clears_status(m_item, status) and status != Statuses::clear) {
			m_item = Item::None;
		} else {
			m_status = status;
		}
	}
	friend void advance_status_from_move(Pokemon & pokemon, bool clear_status);
	friend void advance_status_end_of_turn(Pokemon & pokemon, bool is_having_a_nightmare, Pokemon other_pokemon, bool uproar);

	void switch_out() & {
		m_status.handle_switch(get_ability(*this));
	}

	void change_type(Type new_type);

	auto has_been_seen() const -> bool {
		return m_has_been_seen;
	}
	void mark_as_seen() & {
		m_has_been_seen = true;
	}

	friend auto all_moves(Pokemon & pokemon) -> MoveContainer &;
	friend void set_hp(Pokemon & pokemon, auto hp);

	friend void set_ability(Pokemon & pokemon, Ability ability);
	friend bool ability_is_known(Pokemon pokemon);
	friend void set_gender(Pokemon & pokemon, Gender gender);
	friend void set_item(Pokemon & pokemon, Item item);
	friend bool item_is_known(Pokemon pokemon);
	friend void set_nature(Pokemon & pokemon, Nature nature);
	friend bool nature_is_known(Pokemon pokemon);
	friend auto set_hp_ev(Generation, Pokemon &, EV, IV) -> void;
	friend auto set_stat_ev(Pokemon &, StatNames, EV, IV) -> void;
private:
	MoveContainer m_moves;
	
	PokemonTypes current_type;

	Stats stats;

	Species m_species;
	Item m_item;
	Ability m_ability;
	Gender m_gender;
	Status m_status;
	Nature m_nature;

	Level m_level;
	Happiness m_happiness;
	HiddenPower m_hidden_power;

	bool m_has_been_seen : 1;
	
	bool m_ability_is_known : 1;
	bool m_item_is_known : 1;
	bool m_nature_is_known : 1;
};

inline auto operator==(Species const lhs, Pokemon const rhs) {
	return lhs == get_species(rhs);
}
inline auto operator==(Pokemon const lhs, Species const rhs) {
	return get_species(lhs) == rhs;
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

inline Ability get_ability(Pokemon const pokemon) {
	return pokemon.m_ability;
}
inline void set_ability(Pokemon & pokemon, Ability const ability) {
	pokemon.m_ability = ability;
	pokemon.m_ability_is_known = true;
}
inline bool ability_is_known(Pokemon const pokemon) {
	return pokemon.m_ability_is_known;
}

inline Gender get_gender(Pokemon const pokemon) {
	return pokemon.m_gender;
}
inline void set_gender(Pokemon & pokemon, Gender const gender) {
	pokemon.m_gender = gender;
}

inline Item get_item(Pokemon const pokemon) {
	return pokemon.m_item;
}
inline void set_item(Pokemon & pokemon, Item const item) {
	pokemon.m_item = item;
	pokemon.m_item_is_known = true;
}
inline bool item_is_known(Pokemon const pokemon) {
	return pokemon.m_item_is_known;
}

inline Nature get_nature(Pokemon const pokemon) {
	return pokemon.m_nature;
}
inline void set_nature(Pokemon & pokemon, Nature const nature) {
	pokemon.m_nature = nature;
	pokemon.m_nature_is_known = true;
}
inline bool nature_is_known(Pokemon const pokemon) {
	return pokemon.m_nature_is_known;
}

inline Species get_species(Pokemon const pokemon) {
	return pokemon.m_species;
}


void activate_pinch_item(Pokemon & pokemon);

inline HP get_hp(Pokemon const pokemon) {
	return pokemon.stats.hp();
}
inline void set_hp(Pokemon & pokemon, auto const hp) {
	pokemon.stats.hp() = hp;
	activate_pinch_item(pokemon);
}
inline void change_hp(Pokemon & pokemon, auto const change) {
	set_hp(pokemon, get_hp(pokemon).current() + change);
}
inline auto set_hp_ev(Generation const generation, Pokemon & pokemon, EV const ev, IV const iv) -> void {
	pokemon.stats.hp() = HP(generation, get_species(pokemon), get_level(pokemon), ev, iv);
}
inline auto set_hp_ev(Generation const generation, Pokemon & pokemon, EV const ev) -> void {
	set_hp_ev(generation, pokemon, ev, get_hp(pokemon).iv());
}


inline Stat get_stat(Pokemon const pokemon, StatNames const index_stat) {
	return pokemon.stats[index_stat];
}
inline auto set_stat_ev(Pokemon & pokemon, StatNames const stat_name, EV const ev, IV const iv) -> void {
	auto & stat = pokemon.stats[stat_name];
	stat = Stat(stat, ev, iv);
}
inline auto set_stat_ev(Pokemon & pokemon, StatNames const stat_name, EV const ev) -> void {
	set_stat_ev(pokemon, stat_name, ev, get_stat(pokemon, stat_name).iv());
}


inline Status get_status(Pokemon const pokemon) {
	return pokemon.m_status;
}

inline auto clear_status(Pokemon & pokemon) -> void {
	pokemon.set_status(Statuses::clear);
}


inline void advance_status_from_move(Pokemon & pokemon, bool const clear_status) {
	pokemon.m_status.advance_from_move(get_ability(pokemon), clear_status);
}

inline void advance_status_end_of_turn(Pokemon & pokemon, bool const is_having_a_nightmare, Pokemon const other, bool const uproar) {
	pokemon.m_status.end_of_turn(pokemon, is_having_a_nightmare, other, uproar);
}

inline PokemonTypes get_type(Pokemon const pokemon) {
	return pokemon.current_type;
}

inline Level get_level(Pokemon const pokemon) {
	return pokemon.m_level;
}

inline Happiness get_happiness(Pokemon const pokemon) {
	return pokemon.m_happiness;
}

inline HiddenPower get_hidden_power(Pokemon const pokemon) {
	return pokemon.m_hidden_power;
}


containers::string to_string(Pokemon pokemon);

inline auto hp_ratio(Pokemon const pokemon) {
	auto const hp = get_hp(pokemon);
	return rational(hp.current(), hp.max());
}

}	// namespace technicalmachine
