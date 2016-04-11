// Collection of Pokemon with index indicating current Pokemon
// Copyright (C) 2016 David Stone
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

#include "container.hpp"
#include "max_pokemon_per_team.hpp"
#include "pokemon.hpp"
#include "species_forward.hpp"

#include "../collection.hpp"
#include "../hash.hpp"

#include "../move/moves.hpp"

#include <bounded/integer.hpp>
#include <bounded/integer_range.hpp>

#include <cstddef>

namespace technicalmachine {
using namespace bounded::literal;
struct Move;

struct PokemonCollection : detail::Collection<PokemonContainer> {
	explicit PokemonCollection(TeamSize initial_size);
	// TODO: Need to rework my constructors or something so that this is not
	// needed. This should only be called once, in team intialization
	void initialize_size(TeamSize const new_size);
	void initialize_replacement ();
	using detail::Collection<PokemonContainer>::operator();
	template<typename... Args>
	Pokemon & operator()(Args && ... args) {
		auto const self = const_cast<PokemonCollection const *>(this);
		return const_cast<Pokemon &>(self->operator()(std::forward<Args>(args)...));
	}

	auto replacement() const {
		return current_replacement;
	}

	void set_replacement(containers::index_type<PokemonCollection> const new_index);
	TeamSize real_size() const;

	template<typename... Args>
	void add(Args&&... args) {
		emplace_back(true_size, std::forward<Args>(args)...);
		current_replacement = size(*this) - 1_bi;
	}

	void remove_active();
	template<typename Function1, typename Function2>
	void for_each_replacement (Function1 const & break_out, Function2 const & f) {
		ResetIndex reset(*this);
		for (auto const test_replacement : bounded::integer_range(size(*this))) {
			auto const would_switch_to_self = replacement() == index();
			if (would_switch_to_self and size(*this) > 1_bi) {
				continue;
			}
			current_replacement = test_replacement;
			f();
			if (break_out()) {
				break;
			}
		}
	}
	template<typename Function>
	void for_each_replacement (Function const & f) {
		// Most versions of the loop do not require the ability to break out early.
		// This passes in a function that always returns false for when to break out
		for_each_replacement(std::false_type{}, f);
	}
private:
	struct ResetIndex {
		explicit ResetIndex(PokemonCollection & collection):
			copy(collection),
			index(collection.current_replacement)
			{
		}
		~ResetIndex() {
			copy.current_replacement = index;
		}
	private:
		PokemonCollection & copy;
		containers::index_type<PokemonCollection> const index;
	};

	void decrement_real_size();
	// If a Pokemon switches / faints, what Pokemon should replace it?
	containers::index_type<PokemonCollection> current_replacement;
	// The actual size of the foe's team, not just the Pokemon I've seen
	TeamSize true_size;
};

inline auto hash(PokemonCollection const & collection) noexcept {
	return hash_combine(hash(collection.real_size()), hash_range<TeamSize>(collection.begin(), collection.end()));
}



}	// namespace technicalmachine
