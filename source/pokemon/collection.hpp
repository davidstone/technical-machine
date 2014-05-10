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

#ifndef POKEMON__COLLECTION_HPP_
#define POKEMON__COLLECTION_HPP_

#include "container.hpp"
#include "max_pokemon_per_team.hpp"
#include "pokemon.hpp"
#include "species_forward.hpp"

#include "../collection.hpp"

#include "../move/moves_forward.hpp"

#include <bounded_integer/bounded_integer.hpp>
#include <bounded_integer/integer_range.hpp>

#include <cstddef>
#include <vector>

namespace technicalmachine {
using namespace bounded::literal;
class Move;

class PokemonCollection : public detail::Collection<PokemonContainer> {
	using Base = detail::Collection<PokemonContainer>;
public:
	using Base::index_type;
	using Base::size_type;
	using const_iterator = Base::container_type::const_iterator;
	using iterator = Base::container_type::iterator;
	PokemonCollection();
	// Need to rework my constructors or something so that this is not
	// needed. This should only be called once, in team intialization
	void initialize_size(TeamSize const new_size);
	void initialize_replacement ();
	using Base::operator();
	template<typename... Args>
	Pokemon & operator()(Args && ... args) {
		auto const self = const_cast<PokemonCollection const *>(this);
		return const_cast<Pokemon &>(self->operator()(std::forward<Args>(args)...));
	}
	const_iterator begin() const;
	iterator begin();
	const_iterator end() const;
	iterator end();

	index_type replacement() const;
	void set_replacement (index_type const new_index);
	Pokemon const & at_replacement () const;
	Pokemon & at_replacement ();
	Moves replacement_to_switch () const;
	void replacement_from_switch ();
	bool is_switching_to_self () const;
	bool is_switching_to_self(Moves move) const;
	TeamSize size() const;
	TeamSize real_size() const;
	index_type find_index(Species name) const;

	template<class... Args>
	void add(Args&&... args) {
		container.emplace_back(true_size, std::forward<Args>(args)...);
		// Guaranteed to be a valid index
		current_replacement = static_cast<index_type>(size() - 1_bi);
	}
	template<class...Args>
	bool add_if_not_present(Species name, Args&&... args) {
		bool const add_new_pokemon = !seen(name);
		if (add_new_pokemon) {
			add(name, std::forward<Args>(args)...);
		}
		return add_new_pokemon;
	}
	void remove_active();
	template<typename Function1, typename Function2>
	void for_each_replacement (Function1 const & break_out, Function2 const & f) {
		ResetIndex reset(*this);
		for (auto const test_replacement : bounded::integer_range(size())) {
			current_replacement = test_replacement;
			if (is_switching_to_self() and size() > 1_bi)
				continue;
			f();
			if (break_out())
				break;
		}
	}
	template<typename Function>
	void for_each_replacement (Function const & f) {
		// Most versions of the loop do not require the ability to break out early.
		// This passes in a function that always returns false for when to break out
		for_each_replacement([]() { return false; }, f);
	}
	typedef uint64_t hash_type;
	hash_type hash() const;
	hash_type max_hash() const;
private:
	class ResetIndex {
	public:
		ResetIndex(PokemonCollection & collection):
			copy(collection),
			index(collection.current_replacement)
			{
		}
		~ResetIndex() {
			copy.current_replacement = index;
		}
	private:
		PokemonCollection & copy;
		index_type const index;
	};
	using Base::unchecked_value;
	Pokemon & unchecked_value(index_type const specified_index) {
		auto const self = const_cast<PokemonCollection const *>(this);
		return const_cast<Pokemon &>(self->unchecked_value(specified_index));
	}
	bool seen(Species name);
	void decrement_real_size ();
	// If a Pokemon switches / faints, what Pokemon should replace it?
	index_type current_replacement;
	// The actual size of the foe's team, not just the Pokemon I've seen
	TeamSize true_size;
};

}	// namespace technicalmachine
#endif	// POKEMON__COLLECTION_HPP_
