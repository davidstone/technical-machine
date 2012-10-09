// Generate a random team, weighted by usage
// Copyright (C) 2012 David Stone
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

#include "random_team.hpp"

#include <algorithm>
#include <numeric>
#include <random>
#include <stdexcept>

#include "load_stats.hpp"

#include "../team.hpp"

#include "../pokemon/species.hpp"
#include "../string_conversions/conversion.hpp"

namespace technicalmachine {
namespace {
class Usages {
	public:
		Usages():
			usages(overall_stats()),
			remaining_total(std::accumulate(std::begin(usages), std::end(usages), 0u))
			{
		}
		void reduce(Species const species) {
			remaining_total -= usages[static_cast<size_t>(species)];
		}
		unsigned const & operator[] (size_t const index) const {
			return usages[index];
		}
		unsigned total() const {
			return remaining_total;
		}
	private:
		std::array<unsigned, number_of_species> usages;
		unsigned remaining_total;
};

class InvalidRandomSpecies : public std::logic_error {
	public:
		InvalidRandomSpecies():
			std::logic_error("Random species selection is invalid.")
			{
		}
};
std::vector<Species> random_species(std::mt19937 & random_engine, unsigned max_random_pokemon);
unsigned pokemon_to_generate(std::mt19937 & random_engine, unsigned max_random_pokemon);
Species find_random_species(Usages const & usages, unsigned usage_threshold, std::vector<Species> const & current);
bool species_already_on_team(std::vector<Species> const & current, Species const species);
}	// unnamed namespace


void random_team(Team & team, std::mt19937 & random_engine, unsigned const max_random_pokemon) {
	for (Species const species : random_species(random_engine, max_random_pokemon)) {
		constexpr uint8_t level = 100;
		team.add_pokemon(species, level, Gender());
	}
}

Team random_team(std::mt19937 & random_engine, unsigned const max_random_pokemon) {
	Team team;
	random_team(team, random_engine, max_random_pokemon);
	return team;
}

namespace {

std::vector<Species> random_species(std::mt19937 & random_engine, unsigned const max_random_pokemon) {
	Usages usages;
	std::vector<Species> current;
	auto const random_pokemon = pokemon_to_generate(random_engine, max_random_pokemon);
	for (unsigned generated = 0; generated != random_pokemon; ++generated) {
		std::uniform_int_distribution<unsigned> pokemon_distribution(0, usages.total() - 1);
		Species const species = find_random_species(usages, pokemon_distribution(random_engine), current);
		usages.reduce(species);
		current.emplace_back(species);
	}
	return current;
}

unsigned pokemon_to_generate(std::mt19937 & random_engine, unsigned const max_random_pokemon) {
	std::uniform_int_distribution<unsigned> team_distribution(0, max_random_pokemon);
	return team_distribution(random_engine);
}

Species find_random_species(Usages const & usages, unsigned usage_threshold, std::vector<Species> const & current) {
	for (unsigned species = 0; species != number_of_species; ++species) {
		if (species_already_on_team(current, static_cast<Species>(species)))
			continue;
		if (usage_threshold >= usages[species])
			usage_threshold -= usages[species];
		else
			return static_cast<Species>(species);
	}
	throw InvalidRandomSpecies();
}

bool species_already_on_team(std::vector<Species> const & current, Species const species) {
	return std::find(current.begin(), current.end(), species) != current.end();
}
}	// unnamed namespace

}	// namespace technicalmachine
