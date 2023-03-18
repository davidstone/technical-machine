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
import tv;

namespace technicalmachine {

export struct StatsUser {
	explicit constexpr StatsUser(UsageStats const & usage_stats, tv::optional<std::mt19937 &> random = tv::none):
		m_usage_stats(usage_stats),
		m_random(random),
		m_estimate(m_usage_stats)
	{
	}

	constexpr auto update(Species const species, auto... args) -> void {
		m_estimate.update(m_usage_stats, species, args...);
	}

	constexpr auto species() const {
		return m_random ?
			m_estimate.random_species(*m_random) :
			m_estimate.most_likely_species();
	}
	constexpr auto move(Species const species) const {
		return m_random ?
			m_estimate.random_move(*m_random, species) :
			m_estimate.most_likely_move(species);
	}
	constexpr auto item(Species const species) const {
		return m_random ?
			m_estimate.random_item(*m_random, species) :
			m_estimate.most_likely_item(species);
	}
	constexpr auto ability(Species const species) const {
		return m_random ?
			m_estimate.random_ability(*m_random, species) :
			m_estimate.most_likely_ability(species);
	}

private:
	UsageStats const & m_usage_stats;
	tv::optional<std::mt19937 &> m_random;
	Estimate m_estimate;
};

export constexpr auto update_estimate(StatsUser & stats_user, any_seen_team auto const & team) -> void {
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
