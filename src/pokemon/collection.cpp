// Collection of Pokemon with index indicating current Pokemon
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

#include "collection.hpp"

#include <algorithm>
#include <cassert>
#include <functional>

#include "pokemon.hpp"
#include "pokemon_not_found.hpp"

#include "../move/move.hpp"

namespace technicalmachine {
enum class Moves : uint16_t;
enum class Species : uint16_t;

PokemonCollection::PokemonCollection ():
	current_replacement(0),
	true_size(6) {
}

void PokemonCollection::initialize_size (index_type const new_size) {
	true_size = new_size;
}

void PokemonCollection::initialize_replacement () {
	// No need to bounds check because index() already is
	current_replacement = index();
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

void PokemonCollection::to_replacement() {
	set_index (replacement());
}

Moves PokemonCollection::replacement_to_switch () const {
	return Move::from_replacement (replacement());
}
void PokemonCollection::replacement_from_switch () {
	set_replacement(operator()().move().to_replacement());
}

bool PokemonCollection::is_switching_to_self () const {
	return replacement() == index();
}
bool PokemonCollection::is_switching_to_self (Move const & move) const {
	return move.to_replacement() == index();
}

PokemonCollection::index_type PokemonCollection::size () const {
	return container.size();
}
PokemonCollection::index_type PokemonCollection::real_size () const {
	return true_size;
}

PokemonCollection::index_type PokemonCollection::find_index(Species const name) const {
	for (index_type found_index = 0; found_index != size(); ++found_index) {
		if (operator()(found_index).name == name)
			return found_index;
	}
	throw PokemonNotFound(name);
}

bool PokemonCollection::seen (Species const name) {
	for (current_replacement = 0; current_replacement != size(); ++current_replacement) {
		if (name == at_replacement().name)
			return true;
	}
	return false;
}

void PokemonCollection::remove_active () {
	assert(index() != replacement());
	container.erase (container.begin() + index());
	decrement_real_size();
	set_index((index() > replacement()) ? replacement() : replacement() - 1);
}

void PokemonCollection::for_each_replacement (std::function<bool(void)> const & break_out, std::function<void(void)> const & f) {
	assert(container.size() == size());
	for (current_replacement = 0; current_replacement != container.size(); ++current_replacement) {
		if (is_switching_to_self() and container.size() > 1)
			continue;
		f ();
		if (break_out())
			break;
	}
}
void PokemonCollection::for_each_replacement (std::function<void(void)> const & f) {
	// Most versions of the loop do not require the ability to break out early.
	// This passes in a function that always returns false for when to break out
	for_each_replacement([]() { return false; }, f);
}

unsigned PokemonCollection::count_if (std::function<bool(Pokemon const &)> const & f) const {
	return std::count_if(container.begin(), container.end(), f);
}

void PokemonCollection::decrement_real_size () {
	--true_size;
}

PokemonCollection::hash_type PokemonCollection::hash() const {
	hash_type current_hash = 0;
	for_each([& current_hash](Pokemon const & next_pokemon) {
		current_hash *= next_pokemon.max_hash();
		current_hash += next_pokemon.hash();
	});
	return current_hash;
}

PokemonCollection::hash_type PokemonCollection::max_hash() const {
	hash_type current_max = 0;
	for_each([& current_max](Pokemon const & next_pokemon) {
		current_max += next_pokemon.max_hash();
	});
	return current_max;
}

}	// namespace technicalmachine
