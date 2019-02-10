// Copyright (C) 2019 David Stone
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

#include <tm/clients/pokemon_showdown/packed_team.hpp>

#include <tm/team.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/gender.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/string_conversions/pokemon.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/integer_range.hpp>

namespace technicalmachine {
namespace ps {

// TODO: Write a PS-specific to_string and from_string
auto to_packed_format(Team const & team) -> containers::string {
	auto result = containers::string{};
	for (auto pokemon_it = begin(team.all_pokemon()); pokemon_it != end(team.all_pokemon()); ++pokemon_it) {
		if (pokemon_it != begin(team.all_pokemon())) {
			result += ']';
		}
		Pokemon const & pokemon = *pokemon_it;
		result += to_string(get_species(pokemon));
		result += "||";
		result += to_string(get_item(pokemon));
		result += '|';
		result += to_string(get_ability(pokemon));
		result += '|';
		auto const moves = regular_moves(pokemon);
		for (auto it = begin(moves); it != end(moves); ++it) {
			if (it != begin(moves)) {
				result += ',';
			}
			result += to_string(it->name());
		}
		result += '|';
		result += to_string(get_nature(pokemon));
		result += '|';
		result += std::string_view(to_string(get_hp(pokemon).ev().value()));
		for (auto const stat : containers::enum_range(StatNames::NORMAL_END)) {
			result += ',';
			result += std::string_view(to_string(get_stat(pokemon, stat).ev().value()));
		}
		result += '|';
		result += to_string(get_gender(pokemon));
		result += '|';
		result += std::string_view(to_string(get_hp(pokemon).iv().value()));
		for (auto const stat : containers::enum_range(StatNames::NORMAL_END)) {
			result += ',';
			result += std::string_view(to_string(get_stat(pokemon, stat).iv().value()));
		}
		result += '|';
		// Assume non-shiny
		result += '|';
		result += std::string_view(to_string(get_level(pokemon)()));
		result += '|';
		// Assume max happiness
		// Assume regular Poke Ball
		// Generations with Hyper Training are not yet supported
	}
	return result;
}

}	// namespace ps
}	// namespace technicalmachine
