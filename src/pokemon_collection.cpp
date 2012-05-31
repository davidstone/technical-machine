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

#include "pokemon_collection.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <string>

#include "pokemon.hpp"
#include "species.hpp"

#include "move/move.hpp"
#include "move/shared.hpp"

namespace technicalmachine {
enum class Moves : uint16_t;

PokemonCollection::PokemonCollection ():
	current_replacement(0),
	true_size(6) {
}

void PokemonCollection::initialize_size (uint8_t const new_size) {
	true_size = new_size;
}

void PokemonCollection::initialize_replacement () {
	// No need to bounds check because index() already is
	current_replacement = index();
}

uint8_t PokemonCollection::replacement() const {
	return current_replacement;
}

void PokemonCollection::set_replacement (uint8_t const new_index) {
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

uint8_t PokemonCollection::size () const {
	return container.size();
}
uint8_t PokemonCollection::real_size () const {
	return true_size;
}

bool PokemonCollection::seen (Species const name) {
	for (current_replacement = 0; current_replacement != size(); ++current_replacement) {
		if (name == at_replacement().name)
			return true;
	}
	return false;
}

void PokemonCollection::add (Pokemon const & pokemon) {
	BaseCollection<Pokemon>::add(pokemon);
}
void PokemonCollection::add (Species name, std::string const & nickname, unsigned level, Gender gender, uint8_t happiness, SharedMoves & shared_moves) {
	Pokemon pokemon (name, shared_moves, happiness);
	pokemon.level = level;
	pokemon.gender = gender;

	pokemon.set_nickname(nickname);

	add(pokemon);
	// Guaranteed to be a valid index
	current_replacement = container.size() - 1;
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

}	// namespace technicalmachine
