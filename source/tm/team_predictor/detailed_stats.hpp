// Detailed Pokemon stats
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

#include <tm/ability.hpp>
#include <tm/item.hpp>

#include <tm/move/max_moves_per_pokemon.hpp>
#include <tm/move/moves.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/stat/nature.hpp>

#include <containers/array/array.hpp>
#include <containers/static_vector/static_vector.hpp>

#include <cstdint>
#include <filesystem>

namespace technicalmachine {

struct DetailedStats {
	using UsedMoves = containers::static_vector<Moves, max_moves_per_pokemon.value()>;
	
	explicit DetailedStats(std::filesystem::path const & path);

	template<typename T, BOUNDED_REQUIRES(std::is_same<T, Ability>::value)>
	auto get(Species const species) const {
		return ability[species];
	}
	template<typename T, BOUNDED_REQUIRES(std::is_same<T, Item>::value)>
	auto get(Species const species) const {
		return item[species];
	}
	template<typename T, BOUNDED_REQUIRES(std::is_same<T, Nature>::value)>
	auto get(Species const species) const {
		return nature[species];
	}
	template<typename T, BOUNDED_REQUIRES(std::is_same<T, UsedMoves>::value)>
	auto get(Species const species) const {
		return move[species];
	}
private:
	containers::array<UsedMoves, number_of_species> move;
	containers::array<Item, number_of_species> item;
	containers::array<Ability, number_of_species> ability;
	containers::array<Nature, number_of_species> nature;
};

}	// namespace technicalmachine
