// Copyright (C) 2020 David Stone
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

#include <tm/string_conversions/pokemon.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/string_conversions/species.hpp>
#include <tm/string_conversions/status.hpp>

#include <tm/pokemon/pokemon.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/algorithms/find.hpp>
#include <containers/range_view.hpp>

namespace technicalmachine {

auto to_string(Pokemon const pokemon) -> containers::string {
	// Boost.Format fails to compile with C++20, so we have to do this instead
	auto hp_string = [&] {
		auto const buffer = std::to_string(100.0 * static_cast<double>(hp_ratio(pokemon)));
		auto const it = containers::find(buffer, '.');
		auto const last = (end(buffer) - it <= 2) ? end(buffer) : it + 2;
		return containers::string(containers::range_view(begin(buffer), last));
	};

	auto const output_status = !is_clear(get_status(pokemon));
	
	#define TECHNICALMACHINE_STAT(stat, str) \
		std::string_view(" / "), std::string_view(bounded::to_string(get_stat(pokemon, stat).ev().value())), std::string_view(" " str)
	
	auto moves_to_string = [&]{
		containers::string output;
		for (auto const & move : regular_moves(pokemon)) {
			output = containers::concatenate<containers::string>(std::move(output), std::string_view("\n\t- "), to_string(move.name()));
		}
		return output;
	};

	return containers::concatenate<containers::string>(
		to_string(get_species(pokemon)),
		std::string_view(" ("), hp_string(), std::string_view("% HP) @ "),
		to_string(pokemon.unmodified_item()),
		std::string_view("\n\tAbility: "), to_string(pokemon.initial_ability()), std::string_view("\n"),
		(output_status ? containers::concatenate<containers::string>(std::string_view("\tStatus: "), to_string(get_status(pokemon).name()), std::string_view("\n")) : containers::string("")),
		std::string_view("\tNature: "), to_string(get_nature(pokemon)),
		std::string_view("\n\t"),
		std::string_view(bounded::to_string(get_hp(pokemon).ev().value())), std::string_view(" HP"),
		TECHNICALMACHINE_STAT(StatNames::ATK, "Atk"),
		TECHNICALMACHINE_STAT(StatNames::DEF, "Def"),
		TECHNICALMACHINE_STAT(StatNames::SPA, "SpA"),
		TECHNICALMACHINE_STAT(StatNames::SPD, "SpD"),
		TECHNICALMACHINE_STAT(StatNames::SPE, "Spe"),
		moves_to_string()
	);
	#undef TECHNICALMACHINE_STAT
}


}	// namespace technicalmachine
