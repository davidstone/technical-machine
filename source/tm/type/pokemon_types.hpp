// Type information for Pokemon
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

#include <tm/type/type.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <containers/array/array.hpp>

namespace technicalmachine {
struct ActivePokemon;
enum class Generation : std::uint8_t;
struct MutableActivePokemon;
struct Pokemon;
struct Weather;

struct PokemonTypes {
	// Can a Pokemon ever have 0 types?
	using size_type = bounded::integer<1, 2>;

	PokemonTypes(Generation, Species);

	constexpr explicit PokemonTypes(Type const type):
		m_types{type, Type::Typeless}
	{
	}

	friend constexpr auto begin(PokemonTypes const & types) {
		return begin(types.m_types);
	}
	friend constexpr auto end(PokemonTypes const & types) {
		return begin(types) + BOUNDED_CONDITIONAL(types.m_types[1_bi] == Type::Typeless, 1_bi, 2_bi);
	}

private:
	constexpr explicit PokemonTypes(Type const type1, Type const type2):
		m_types{type1, type2}
	{
	}

	containers::array<Type, 2> m_types;
};

auto is_type(Pokemon const &, Type, bool roosting) -> bool;

auto grounded(ActivePokemon pokemon, Weather weather) -> bool;
auto grounded(MutableActivePokemon pokemon, Weather weather) -> bool;
auto grounded(Pokemon const & pokemon, Weather weather) -> bool;

}	// namespace technicalmachine
