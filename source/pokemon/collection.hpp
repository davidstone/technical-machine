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

#include "../collection.hpp"
#include "../hash.hpp"

#include <bounded/integer.hpp>

#include <utility>

namespace technicalmachine {
using namespace bounded::literal;

struct PokemonCollection : detail::Collection<PokemonContainer> {
	explicit PokemonCollection(TeamSize const initial_size):
		current_replacement(0_bi),
		true_size(initial_size)
	{
	}

	// TODO: Need to rework my constructors or something so that this is not
	// needed. This should only be called once, in team intialization
	auto initialize_size(TeamSize const new_size) {
		true_size = new_size;
	}
	auto initialize_replacement() {
		current_replacement = index();
	}

	using detail::Collection<PokemonContainer>::operator();
	template<typename... Args>
	Pokemon & operator()(Args && ... args) {
		auto const self = const_cast<PokemonCollection const *>(this);
		return const_cast<Pokemon &>(self->operator()(std::forward<Args>(args)...));
	}

	auto replacement() const {
		return current_replacement;
	}

	auto set_replacement(containers::index_type<PokemonCollection> const new_index) {
		current_replacement = check_range(new_index);
	}

	auto real_size() const {
		return true_size;
	}

	template<typename... Args>
	void add(Args&&... args) {
		emplace_back(true_size, std::forward<Args>(args)...);
		current_replacement = size(*this) - 1_bi;
	}

	void remove_active(containers::index_type<PokemonCollection> index_of_replacement);

private:
	// If a Pokemon switches / faints, what Pokemon should replace it?
	containers::index_type<PokemonCollection> current_replacement;
	// The actual size of the foe's team, not just the Pokemon I've seen
	TeamSize true_size;
};

inline auto hash(PokemonCollection const & collection) noexcept {
	return hash_combine(hash(collection.real_size()), hash_range<TeamSize>(begin(collection), end(collection)));
}



}	// namespace technicalmachine
