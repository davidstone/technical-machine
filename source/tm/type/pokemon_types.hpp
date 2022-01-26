// Type information for Pokemon
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/type/type.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <containers/array.hpp>
#include <containers/begin_end.hpp>

namespace technicalmachine {
using namespace bounded::literal;

enum class Generation : std::uint8_t;

struct PokemonTypes {
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

	friend auto operator==(PokemonTypes, PokemonTypes) -> bool = default;

private:
	constexpr explicit PokemonTypes(Type const type1, Type const type2):
		m_types{type1, type2}
	{
	}

	containers::array<Type, 2_bi> m_types;
};

}	// namespace technicalmachine
