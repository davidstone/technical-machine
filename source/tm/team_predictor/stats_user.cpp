// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.team_predictor.stats_user;

import tm.team_predictor.estimate;
import tm.team_predictor.usage_stats;

import tm.pokemon.species;

import tm.any_team;

import std_module;

namespace technicalmachine {

export struct StatsUser {
	StatsUser(UsageStats const & usage_stats, bool const use_most_likely):
		m_usage_stats(usage_stats),
		m_estimate(std::make_unique<Estimate>(m_usage_stats)),
		m_use_most_likely(use_most_likely)
	{
	}

	auto update(Species const species, auto... args) -> void {
		m_estimate->update(m_usage_stats, species, args...);
	}

	auto species(std::mt19937 & random_engine) const {
		return m_use_most_likely ?
			m_estimate->most_likely_species() :
			m_estimate->random_species(random_engine);
	}
	auto move(std::mt19937 & random_engine, Species const species) const {
		return m_use_most_likely ?
			m_estimate->most_likely_move(species) :
			m_estimate->random_move(random_engine, species);
	}
	auto item(std::mt19937 & random_engine, Species const species) const {
		return m_use_most_likely ?
			m_estimate->most_likely_item(species) :
			m_estimate->random_item(random_engine, species);
	}
	auto ability(std::mt19937 & random_engine, Species const species) const {
		return m_use_most_likely ?
			m_estimate->most_likely_ability(species) :
			m_estimate->random_ability(random_engine, species);
	}

private:
	UsageStats const & m_usage_stats;
	std::unique_ptr<Estimate> m_estimate;
	bool m_use_most_likely;
};

export auto update_estimate(StatsUser & stats_user, any_seen_team auto const & team) -> void {
	for (auto const & pokemon : team.all_pokemon()) {
		stats_user.update(pokemon.species());
		constexpr auto embargo = false;
		constexpr auto magic_room = false;
		// TODO: We want to keep track of this even if the item is removed
		stats_user.update(pokemon.species(), pokemon.item(embargo, magic_room));
		for (auto const move : pokemon.regular_moves()) {
			stats_user.update(pokemon.species(), move.name());
		}
	}
}

} // namespace technicalmachine
