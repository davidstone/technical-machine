// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/team_predictor/usage_stats_probabilities.hpp>

#include <tm/move/moves.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/stat/ev.hpp>
#include <tm/stat/nature.hpp>

#include <tm/ability.hpp>
#include <tm/generation.hpp>
#include <tm/item.hpp>

#include <bounded/number_of.hpp>

#include <containers/array.hpp>
#include <containers/flat_map.hpp>

#include <istream>
#include <utility>

namespace technicalmachine {

struct UsageStats {
	static auto make(std::istream && stream) -> UsageStats;

	auto assuming() const -> UsageStatsProbabilities const & {
		return m_probabilities;
	}
	auto assuming(Species const species) const -> UsageStatsProbabilities const * {
		auto const per_species = containers::lookup(m_data, species);
		return per_species ? std::addressof(per_species->probabilities) : nullptr;
	}
	auto assuming(Species const species, Moves const moves) const -> UsageStatsProbabilities const * {
		auto const per_species = containers::lookup(m_data, species);
		return per_species ?
			containers::lookup(per_species->used_moves, moves) :
			nullptr;
	}

private:
	using UsedMoves = containers::flat_map<Moves, UsageStatsProbabilities>;
	struct PerSpecies {
		UsedMoves used_moves;
		UsageStatsProbabilities probabilities;
	};
	using Data = containers::flat_map<Species, PerSpecies>;

	explicit UsageStats(Data data, UsageStatsProbabilities probabilities):
		m_data(std::move(data)),
		m_probabilities(std::move(probabilities))
	{
	}

	Data m_data;
	UsageStatsProbabilities m_probabilities;
};

struct AllUsageStats {
	AllUsageStats();
	auto operator[](Generation const generation) const -> UsageStats const & {
		return m_all_stats[bounded::integer(generation) - 1_bi];
	}

private:
	containers::array<UsageStats, bounded::number_of<Generation>> m_all_stats;
};

} // namespace technicalmachine
