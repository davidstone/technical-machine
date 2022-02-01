// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/max_moves_per_pokemon.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/stat/ev.hpp>
#include <tm/stat/nature.hpp>

#include <tm/ability.hpp>
#include <tm/generation.hpp>
#include <tm/item.hpp>
#include <tm/number_of.hpp>

#include <containers/array.hpp>
#include <containers/legacy_iterator.hpp>
#include <containers/static_vector.hpp>

#include <numeric_traits/min_max_value.hpp>

#include <filesystem>
#include <stdexcept>

namespace technicalmachine {

struct UsageStats {
	struct PerPokemon {
		float weight = 0.0F;
		containers::array<float, number_of<Species>> teammates = {};
		containers::static_vector<Moves, max_moves_per_pokemon> moves;
		Ability ability = Ability::Honey_Gather;
		Item item = Item::None;
	};

	explicit UsageStats(std::filesystem::path const & usage_stats_directory);

	auto const & get(Species const species) const {
		return m_all_per_pokemon[bounded::integer(species)];
	}
private:
	containers::array<PerPokemon, number_of<Species>> m_all_per_pokemon;
};

struct AllUsageStats {
	AllUsageStats();
	auto operator[](Generation const generation) const -> UsageStats const & {
		return m_all_stats[bounded::integer(generation) - 1_bi];
	}
private:
	containers::array<UsageStats, number_of<Generation>> m_all_stats;
};

}	// namespace technicalmachine
