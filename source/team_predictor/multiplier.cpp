// Load stats such as Pokemon usage stats
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

#include "multiplier.hpp"

#include "../invalid_settings_file.hpp"

#include "../pokemon/max_pokemon_per_team.hpp"
#include "../pokemon/pokemon.hpp"
#include "../pokemon/species.hpp"

#include "../string_conversions/move.hpp"
#include "../string_conversions/pokemon.hpp"

#include <containers/algorithms/accumulate.hpp>
#include <containers/integer_range.hpp>

#include <boost/lexical_cast.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <string>

namespace technicalmachine {
namespace {

using namespace bounded::literal;

constexpr auto other_pokemon_per_team = max_pokemon_per_team - 1_bi;
constexpr Multiplier::value_type not_set = -1.0F;
bool is_alternate_form(Species first, Species second);

}	// namespace

Multiplier::Multiplier(OverallStats const & overall, std::filesystem::path const & teammate_stats):
	multiplier(species_clause()) {
	// There are 5 other Pokemon on a team for each individual Pokemon.
	// Therefore, if I've seen a Pokemon with n usages, there are 5 * n other
	// Pokemon on the team. Subtract the known number of usages from this
	// number until all known usages are gone. Then, assume the distribution of
	// Pokemon not on the team mate stats is equal to the relative overall
	// distribution and divide up all remaining usages proportionally.
	OverallStats unaccounted;
	std::transform(begin(overall), end(overall), begin(unaccounted), [](unsigned const element) {
		return element * static_cast<unsigned>(other_pokemon_per_team);
	});

	load_listed_multipliers(overall, teammate_stats, unaccounted);
	estimate_remaining(overall, unaccounted);
}

Multiplier::value_type Multiplier::operator() (Species const species1, Species const species2) const {
	return multiplier[species1][species2];
}

Multiplier::Container Multiplier::species_clause() {
	Container multiplier;
	for (auto & array : multiplier) {
		std::fill(begin(array), end(array), not_set);
	}
	for (auto const a : containers::enum_range<Species>()) {
		for (auto const b : containers::enum_range<Species>()) {
			if (is_alternate_form(a, b)) {
				multiplier[a][b] = 0.0F;
			}
		}
	}
	return multiplier;
}

void Multiplier::load_listed_multipliers(OverallStats const & overall, std::filesystem::path const & teammate_stats, OverallStats & unaccounted) {
	// I may not need to calculate this...
	auto const total = static_cast<value_type>(containers::accumulate(overall));

	std::ifstream file(teammate_stats);
	std::string line;
	while (getline(file, line)) {
		constexpr char delimiter = '\t';
		size_t const x = line.find (delimiter);
		auto const member = from_string<Species>(line.substr (0, x));
		size_t const y = line.find (delimiter, x + 1);
		auto const ally = from_string<Species>(line.substr(x + 1, y - x - 1));

		auto const number_used_with = boost::lexical_cast<unsigned>(line.substr(y + 1));
		assert(unaccounted[member] >= number_used_with);
		unaccounted[member] -= number_used_with;
		auto const per_cent_used_with = static_cast<value_type>(number_used_with) / static_cast<value_type>(overall[member]);
		auto const per_cent_used = static_cast<value_type>(overall[ally]) / total;
		multiplier[member][ally] = per_cent_used_with / per_cent_used;
	}
}

void Multiplier::estimate_remaining(OverallStats const & overall, OverallStats const & unaccounted) {
	// Take the total number of remaining Pokemon not accounted for and assume
	// they are distributed the same as their overall distribution among all
	// Pokemon not on the list of team mate stats. This is the same as giving
	// all those Pokemon the same multiplier. Reality probably has very low
	// usage Pokemon more evenly distributed and high usage Pokemon that do not
	// appear with a much lower multiplier.
	//
	// There are probably cases where I can prove that certain Pokemon are lower
	// than my current method would suggest, so I should look into limiting the
	// maximum multiplier.
	//
	// This is because the teammate stats only give the top Pokemon used with
	// this Pokemon. If a Pokemon that is used a lot does not show up on this
	// list, then we can be sure that it is used less than the current method
	// suggests.
	for (auto const a : containers::enum_range<Species>()) {
		if (overall[a] != 0_bi) {
			for (value_type & value : multiplier[a]) {
				if (value == not_set) {
					value = (unaccounted[a] != 0_bi) ?
						(static_cast<value_type>(unaccounted[a]) / static_cast<value_type>(overall[a] * static_cast<unsigned>(other_pokemon_per_team))) :
						0.0F;
				}
			}
		} else {
			// 1 is superior to 0 because if they use an unused Pokemon, this
			// will have no effect instead of making everything equally 0
			auto & m = multiplier[a];
			std::fill(begin(m), end(m), 1.0F);
		}
	}
}

namespace {

bool is_deoxys(Species species);
bool is_giratina(Species species);
bool is_rotom(Species species);
bool is_shaymin(Species species);
bool is_wormadam(Species species);

bool is_alternate_form(Species const first, Species const second) {
	return
		first == second or
		(is_deoxys(first) and is_deoxys(second)) or
		(is_giratina(first) and is_giratina(second)) or
		(is_rotom(first) and is_rotom(second)) or
		(is_shaymin(first) and is_shaymin(second)) or
		(is_wormadam(first) and is_wormadam(second));
}

bool is_deoxys(Species const species) {
	switch (species) {
		case Species::Deoxys_Attack:
		case Species::Deoxys_Defense:
		case Species::Deoxys_Mediocre:
		case Species::Deoxys_Speed:
			return true;
		default:
			return false;
	}
}

bool is_giratina(Species const species) {
	switch (species) {
		case Species::Giratina_Altered:
		case Species::Giratina_Origin:
			return true;
		default:
			return false;
	}
}

bool is_rotom(Species const species) {
	switch (species) {
		case Species::Rotom:
		case Species::Rotom_Mow:
		case Species::Rotom_Frost:
		case Species::Rotom_Heat:
		case Species::Rotom_Fan:
		case Species::Rotom_Wash:
			return true;
		default:
			return false;
	}
}

bool is_shaymin(Species const species) {
	switch (species) {
		case Species::Shaymin_Land:
		case Species::Shaymin_Sky:
			return true;
		default:
			return false;
	}
}

bool is_wormadam(Species const species) {
	switch (species) {
		case Species::Wormadam_Plant:
		case Species::Wormadam_Sandy:
		case Species::Wormadam_Trash:
			return true;
		default:
			return false;
	}
}

}	// namespace
}	// namespace technicalmachine
