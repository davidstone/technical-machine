// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/species.hpp>

#include <tm/generation_generic.hpp>
#include <tm/number_of.hpp>
#include <tm/team.hpp>

#include <bounded/integer.hpp>

#include <containers/array.hpp>
#include <containers/at.hpp>
#include <containers/flat_map.hpp>

#include <cstdint>
#include <mutex>

namespace technicalmachine::ps_usage_stats {

// Regieleki with a Choice Scarf
inline constexpr auto max_initial_speed = 822_bi;

// Getting the full data set of correlations would require free memory measured
// in TiB. Instead we restrict ourselves to correlations among the top N most
// used moves per Pokemon. However, it's impossible to know in advance what this
// set will be, which means we need to run two full passes over the data -- the
// first determines lower-cardinality data that informs what data to gather for
// the higher-cardinality second pass.
struct UsageStats {
	auto add(GenerationGeneric<Team> const & team, double weight) & -> void;

	constexpr auto get_total(Species const species) const {
		return m_all[bounded::integer(species)].total;
	}
	constexpr auto get(Species const species, Ability const ability) const {
		auto const & per_species = m_all[bounded::integer(species)];
		return per_species.abilities[bounded::integer(ability)];
	}
	constexpr auto get(Species const species, Item const item) const {
		auto const & per_species = m_all[bounded::integer(species)];
		return per_species.items[bounded::integer(item)];
	}
	constexpr auto const & moves(Species const species) const {
		return m_all[bounded::integer(species)].moves;
	}
	constexpr auto get(Species const species, Moves const move_name) const {
		auto const & per_species = m_all[bounded::integer(species)];
		return per_species.moves[bounded::integer(move_name)];
	}
	constexpr auto const & speed_distribution(Species const species) const {
		return m_all[bounded::integer(species)].speed;
	}
	constexpr auto total_teams() const {
		return m_total_teams;
	}
private:

	struct PerSpecies {
		double total = 0.0;
		containers::array<double, number_of<Ability>> abilities{};
		containers::array<double, number_of<Item>> items{};
		containers::array<double, number_of<Moves>> moves{};
		containers::array<double, max_initial_speed> speed{};
	};
	containers::array<PerSpecies, number_of<Species>> m_all;
	double m_total_teams = 0.0;
};

template<typename T>
struct LockedAccess {
	auto locked() & {
		struct Result {
			std::scoped_lock<std::mutex> lock;
			T & data;
		};
		return Result{
			std::scoped_lock(m_mutex),
			m_data
		};
	}
	constexpr auto unlocked() const -> T const & {
		return m_data;
	}
private:
	mutable std::mutex m_mutex;
	T m_data;
};

struct Correlations {
private:
	struct PerSpecies {
		double usage = 0.0;
		containers::flat_map<Moves, double> other_moves;
	};
	struct MoveData {
		containers::array<PerSpecies, number_of<Species>> teammates = {};
		containers::array<double, number_of<Moves>> moves = {};
		containers::array<double, number_of<Item>> items = {};
		containers::array<double, number_of<Ability>> abilities = {};
		containers::array<double, max_initial_speed> speed = {};
	};
	static constexpr auto top_n_cutoff = 20_bi;
public:
	using TopMoves = containers::static_flat_map<Moves, std::unique_ptr<LockedAccess<MoveData>>, top_n_cutoff>;

	Correlations(UsageStats const & general_usage_stats);
	auto add(GenerationGeneric<Team> const & team, double weight) & -> void;

	constexpr auto top_moves(Species const species) const -> TopMoves const & {
		return m_data[bounded::integer(species)];
	}
private:
	containers::array<TopMoves, number_of<Species>> m_data;
};

} // namespace technicalmachine::ps_usage_stats
