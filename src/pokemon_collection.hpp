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

#ifndef POKEMON_COLLECTION_HPP_
#define POKEMON_COLLECTION_HPP_

#include "collection.hpp"
#include <cassert>
#include <functional>
#include <string>
#include "pokemon.hpp"
#include "species.hpp"

namespace technicalmachine {

class PokemonCollection : public detail::BaseCollection<Pokemon> {
	public:
		PokemonCollection ():
			current_replacement(0),
			true_size(6) {
		}
		PokemonCollection (detail::BaseCollection<Pokemon>::container_type const & pre_set):
			detail::BaseCollection<Pokemon>(pre_set),
			current_replacement(0),
			true_size(6) {
		}
		void initialize_size (uint8_t const new_size) {
			// Need to rework my constructors or something so that this is not
			// needed. This should only be called once, in team intialization
			true_size = new_size;
		}
		void initialize_replacement () {
			// No need to bounds check because index() already is
			current_replacement = index();
		}
		uint8_t replacement() const {
			return current_replacement;
		}
		void set_replacement (uint8_t const new_index) {
			current_replacement = check_range (new_index);
		}
		Pokemon const & at_replacement () const {
			return operator() (replacement());
		}
		Pokemon & at_replacement () {
			return operator() (replacement());
		}
		void to_replacement() {
			set_index (replacement());
		}
		Move::Moves replacement_to_switch () const {
			return Move::from_replacement (replacement());
		}
		void replacement_from_switch () {
			set_replacement(operator()().move().to_replacement());
		}
		bool is_switching_to_self () const {
			return replacement() == index();
		}
		bool is_switching_to_self (Move const & move) const {
			return move.to_replacement() == index();
		}
		uint8_t size () const {
			return container.size();
		}
		uint8_t real_size () const {
			return true_size;
		}
		bool seen (Species const name) {
			for (current_replacement = 0; current_replacement != size(); ++current_replacement) {
				if (name == at_replacement().name)
					return true;
			}
			return false;
		}
		void add (Pokemon const & pokemon) {
			BaseCollection<Pokemon>::add(pokemon);
		}
		void add (Species name, std::string const & nickname, unsigned level, Gender gender) {
			Pokemon pokemon (name, real_size());
			pokemon.level = level;
			pokemon.gender = gender;

			pokemon.set_nickname(nickname);

			add(pokemon);
			// Guaranteed to be a valid index
			current_replacement = container.size() - 1;
		}
		void remove_active () {
			assert(index() != replacement());
			container.erase (container.begin() + index());
			decrement_real_size();
			set_index((index() > replacement()) ? replacement() : replacement() - 1);
		}
		void for_each_replacement (std::function<bool(void)> const & break_out, std::function<void(void)> const & f) {
			assert(container.size() == size());
			for (current_replacement = 0; current_replacement != container.size(); ++current_replacement) {
				if (is_switching_to_self() and container.size() > 1)
					continue;
				f ();
				if (break_out())
					break;
			}
		}
		void for_each_replacement (std::function<void(void)> const & f) {
			for_each_replacement([]() { return false; }, f);
		}
	private:
		void decrement_real_size () {
			--true_size;
		}
		// If a Pokemon switches / faints, what Pokemon should replace it?
		uint8_t current_replacement;
		// The actual size of the foe's team, not just the Pokemon I've seen
		uint8_t true_size;
};

}	// namespace technicalmachine
#endif	// POKEMON_COLLECTION_HPP_
