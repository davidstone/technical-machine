// Collection of Pokemon with index indicating current Pokemon
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

#include "collection.hpp"

#include "max_pokemon_per_team.hpp"
#include "pokemon.hpp"
#include "pokemon_not_found.hpp"
#include "species.hpp"

#include "../move/is_switch.hpp"
#include "../move/move.hpp"

#include <bounded_integer/integer_range.hpp>

#include <cassert>

namespace technicalmachine {

PokemonCollection::PokemonCollection():
	current_replacement(0_bi),
	true_size(max_pokemon_per_team) {
}

void PokemonCollection::initialize_size(TeamSize const new_size) {
	true_size = new_size;
}

void PokemonCollection::initialize_replacement () {
	// No need to bounds check because index() already is
	current_replacement = index();
}

PokemonCollection::const_iterator PokemonCollection::begin() const {
	return container.begin();
}
PokemonCollection::iterator PokemonCollection::begin() {
	return container.begin();
}
PokemonCollection::const_iterator PokemonCollection::end() const {
	return container.end();
}
PokemonCollection::iterator PokemonCollection::end() {
	return container.end();
}


PokemonCollection::index_type PokemonCollection::replacement() const {
	return current_replacement;
}

void PokemonCollection::set_replacement (index_type const new_index) {
	current_replacement = check_range (new_index);
}

Pokemon const & PokemonCollection::at_replacement () const {
	return operator() (replacement());
}
Pokemon & PokemonCollection::at_replacement () {
	return operator() (replacement());
}

Moves PokemonCollection::replacement_to_switch () const {
	return from_replacement(replacement());
}
void PokemonCollection::replacement_from_switch () {
	Move const & move = operator()().move();
	set_replacement(to_replacement(move));
}

bool PokemonCollection::is_switching_to_self () const {
	return replacement() == index();
}
bool PokemonCollection::is_switching_to_self (Moves const move) const {
	return to_replacement(move) == index();
}

TeamSize PokemonCollection::size() const {
	return static_cast<TeamSize>(container.size());
}
TeamSize PokemonCollection::real_size() const {
	return true_size;
}

PokemonCollection::index_type PokemonCollection::find_index(Species const name) const {
	for (index_type const found_index : bounded::integer_range(size())) {
		if (operator()(found_index) == name)
			return found_index;
	}
	throw PokemonNotFound(name);
}

bool PokemonCollection::seen (Species const name) {
	// In the event of current_replacement == size(), a new Pokemon is added
	// immediately, increasing size() by 1, making this safe.
	for (current_replacement = 0_bi; current_replacement != size(); ++current_replacement) {
		if (name == at_replacement())
			return true;
	}
	return false;
}

void PokemonCollection::remove_active () {
	assert(index() != replacement());
	container.erase(container.begin() + index().value());
	decrement_real_size();
	// We don't need any bounds checking here because we've already established
	// that replacement() is greater than index(), so it cannot be 0, which is
	// the only value that could get this out of bounds.
	set_index((index() > replacement()) ? replacement() : index_type(replacement() - 1_bi, bounded::non_check));
	for (auto & pokemon : container) {
		pokemon.remove_switch();
	}
}

void PokemonCollection::decrement_real_size () {
	--true_size;
}

PokemonCollection::hash_type PokemonCollection::hash() const {
	hash_type current_hash = 0;
	for (auto const & pokemon : container) {
		current_hash *= pokemon.max_hash();
		current_hash += pokemon.hash();
	}
	current_hash *= max_pokemon_per_team;
	current_hash += static_cast<hash_type>(true_size - 1_bi);
	current_hash *= static_cast<hash_type>(true_size);
	current_hash += index();
	return current_hash;
}

PokemonCollection::hash_type PokemonCollection::max_hash() const {
	hash_type current_max = static_cast<hash_type>(max_pokemon_per_team);
	current_max *= true_size;
	for (auto const & pokemon : container) {
		current_max *= pokemon.max_hash();
	}
	return current_max;
}

}	// namespace technicalmachine
