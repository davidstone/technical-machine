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
#include <containers/begin_end.hpp>

namespace technicalmachine {
using namespace bounded::literal;

enum class Generation : std::uint8_t;

struct PokemonTypes {
	// Can a Pokemon ever have 0 types?
	using size_type = bounded::integer<1, 2>;

	PokemonTypes(Generation, Species);

	constexpr explicit PokemonTypes(Type const type):
		m_types{type, Type::Typeless}
	{
	}

	constexpr auto begin() const {
		return containers::begin(m_types);
	}
	constexpr auto size() const {
		return BOUNDED_CONDITIONAL(m_types[1_bi] == Type::Typeless, 1_bi, 2_bi);
	}

	friend auto operator==(PokemonTypes const &, PokemonTypes const &) -> bool = default;

private:
	constexpr explicit PokemonTypes(Type const type1, Type const type2):
		m_types{type1, type2}
	{
	}

	containers::array<Type, 2> m_types;
};

}	// namespace technicalmachine
