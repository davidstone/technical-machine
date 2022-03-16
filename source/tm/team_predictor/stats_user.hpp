// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/team_predictor/estimate.hpp>
#include <tm/team_predictor/usage_stats.hpp>

#include <tm/any_team.hpp>

#include <memory>
#include <random>

namespace technicalmachine {

struct StatsUser {
	StatsUser(UsageStats const & usage_stats, bool const use_most_likely):
		m_usage_stats(usage_stats),
		m_estimate(std::make_unique<Estimate>(m_usage_stats)),
		m_use_most_likely(use_most_likely)
	{
	}

	auto update(Species const species) -> void {
		m_estimate->update(m_usage_stats, species);
	}

	auto species(std::mt19937 & random_engine) const {
		return m_use_most_likely ?
			m_estimate->most_likely() :
			m_estimate->random(random_engine);
	}
	auto moves(Species const species) const {
		return m_usage_stats.get(species).moves;
	}
	auto ability(Species const species) const {
		return m_usage_stats.get(species).ability;
	}
	auto item(Species const species) const {
		return m_usage_stats.get(species).item;
	}

private:
	UsageStats const & m_usage_stats;
	std::unique_ptr<Estimate> m_estimate;
	bool m_use_most_likely;
};

auto update_estimate(StatsUser & stats_user, any_seen_team auto const & team) -> void {
	for (auto const & pokemon : team.all_pokemon()) {
		stats_user.update(pokemon.species());
	}
}

} // namespace technicalmachine
