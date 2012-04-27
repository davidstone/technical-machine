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
#include <functional>
#include <string>
#include "pokemon.hpp"
#include "species.hpp"

namespace technicalmachine {

class PokemonCollection : public detail::BaseCollection<Pokemon> {
	public:
		PokemonCollection ();
		PokemonCollection (detail::BaseCollection<Pokemon>::container_type const & pre_set);
		// Need to rework my constructors or something so that this is not
		// needed. This should only be called once, in team intialization
		void initialize_size (uint8_t const new_size);
		void initialize_replacement ();
		uint8_t replacement() const;
		void set_replacement (uint8_t const new_index);
		Pokemon const & at_replacement () const;
		Pokemon & at_replacement ();
		void to_replacement();
		Move::Moves replacement_to_switch () const;
		void replacement_from_switch ();
		bool is_switching_to_self () const;
		bool is_switching_to_self (Move const & move) const;
		uint8_t size () const;
		uint8_t real_size () const;
		bool seen (Species const name);
		void add (Pokemon const & pokemon);
		void add (Species name, std::string const & nickname, unsigned level, Gender gender);
		void remove_active ();
		void for_each_replacement (std::function<bool(void)> const & break_out, std::function<void(void)> const & f);
		void for_each_replacement (std::function<void(void)> const & f);
	private:
		void decrement_real_size ();
		// If a Pokemon switches / faints, what Pokemon should replace it?
		uint8_t current_replacement;
		// The actual size of the foe's team, not just the Pokemon I've seen
		uint8_t true_size;
};

}	// namespace technicalmachine
#endif	// POKEMON_COLLECTION_HPP_
