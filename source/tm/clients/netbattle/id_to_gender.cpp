// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.nb.id_to_gender;

import tm.pokemon.is_genderless;
import tm.pokemon.species;

import tm.string_conversions.species;

import tm.gender;

import bounded;
import containers;
import std_module;

namespace technicalmachine::nb {
using namespace bounded::literal;
using namespace std::string_view_literals;

export constexpr auto id_to_gender(Species const species, bounded::integer<0, 1> const id) -> Gender {
	if (is_genderless(species)) {
		if (id == 1_bi) {
			throw std::runtime_error(containers::concatenate<std::string>("Tried to give a gender to "sv, to_string(species)));
		}
		return Gender::genderless;
	}
	switch (id.value()) {
		case 0: return Gender::male;
		case 1: return Gender::female;
		default: std::unreachable();
	}
}

} // namespace technicalmachine::nb