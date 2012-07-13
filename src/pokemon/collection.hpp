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

#ifndef POKEMON__COLLECTION_HPP_
#define POKEMON__COLLECTION_HPP_

#include "../collection.hpp"

#include <cstdint>
#include <functional>
#include <string>

#include "pokemon.hpp"

namespace technicalmachine {
enum class Moves : uint16_t;
enum class Species : uint16_t;
class Move;
class SharedMoves;

class PokemonCollection : public detail::BaseCollection<Pokemon> {
		typedef detail::BaseCollection<Pokemon> Base;
	public:
		using Base::index_type;
		PokemonCollection ();
		// Need to rework my constructors or something so that this is not
		// needed. This should only be called once, in team intialization
		void initialize_size (index_type const new_size);
		void initialize_replacement ();
		index_type replacement() const;
		void set_replacement (index_type const new_index);
		Pokemon const & at_replacement () const;
		Pokemon & at_replacement ();
		void to_replacement();
		Moves replacement_to_switch () const;
		void replacement_from_switch ();
		bool is_switching_to_self () const;
		bool is_switching_to_self (Move const & move) const;
		index_type size () const;
		index_type real_size () const;
		index_type find_index(Species name) const;
		bool seen (Species const name);

		template<class... Args>
		void add(Args&&... args) {
			Base::add(std::forward<Args>(args)...);
			// Guaranteed to be a valid index
			current_replacement = static_cast<index_type>(container.size() - 1);
		}
		void remove_active ();
		void for_each_replacement (std::function<bool(void)> const & break_out, std::function<void(void)> const & f);
		void for_each_replacement (std::function<void(void)> const & f);
		unsigned count_if (std::function<bool(Pokemon const &)> const & f) const;
		typedef uint64_t hash_type;
		hash_type hash() const;
		hash_type max_hash() const;
	private:
		void decrement_real_size ();
		// If a Pokemon switches / faints, what Pokemon should replace it?
		index_type current_replacement;
		// The actual size of the foe's team, not just the Pokemon I've seen
		index_type true_size;
};

}	// namespace technicalmachine
#endif	// POKEMON__COLLECTION_HPP_
