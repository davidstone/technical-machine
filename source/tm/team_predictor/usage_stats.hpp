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

#include <tm/move/max_moves_per_pokemon.hpp>
#include <tm/pokemon/species.hpp>
#include <tm/stat/combined_stats.hpp>

#include <tm/ability.hpp>
#include <tm/generation.hpp>
#include <tm/item.hpp>

#include <containers/array/array.hpp>
#include <containers/legacy_iterator.hpp>
#include <containers/static_vector/static_vector.hpp>

#include <algorithm>
#include <filesystem>

namespace technicalmachine {

struct UsageStats {
	struct PerPokemon {
		float weighted_usage = 0.0F;
		containers::array<float, number_of_species> teammates = {};
		containers::static_vector<Moves, max_moves_per_pokemon.value()> moves;
		Ability ability = Ability::Honey_Gather;
		Item item = Item::None;	
		CombinedStats<EV> stats = {
			Nature::Hardy,
			EV(EV::max),
			EV(EV::max),
			EV(EV::max),
			EV(EV::max),
			EV(EV::max),
			EV(EV::max)
		};
	};

	explicit UsageStats(std::filesystem::path const & usage_stats_directory);

	auto total_weighted_usage() const {
		return m_total_weighted_usage;
	}

	auto const & get(Species const species) const {
		return m_all_per_pokemon[bounded::integer(species)];
	}
private:
	float m_total_weighted_usage = 0.0F;
	containers::array<PerPokemon, number_of_species> m_all_per_pokemon;
};

struct AllUsageStats {
	AllUsageStats();
	auto operator[](Generation const generation) const -> UsageStats const & {
		if (generation == Generation::eight) {
			throw std::runtime_error("Generation 8 usage stats not supported yet.");
		}
		return m_all_stats[bounded::integer(generation) - 1_bi];
	}
private:
	// static_cast<unsigned>(bounded::max_value<Generation>)
	containers::array<UsageStats, 7> m_all_stats;
};

}	// namespace technicalmachine
