// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/species.hpp>

#include <tm/generation_generic.hpp>
#include <tm/team.hpp>

#include <bounded/integer.hpp>

#include <containers/array.hpp>
#include <containers/at.hpp>
#include <containers/flat_map.hpp>
#include <containers/static_vector.hpp>

#include <cstdint>

namespace technicalmachine::ps_usage_stats {

template<typename T>
inline constexpr auto number_of = bounded::constant<numeric_traits::max_value<T>> + 1_bi;

// Getting the full data set of correlations would require free memory measured
// in TiB. Instead we restrict ourselves to correlations among the top N most
// used moves per Pokemon. However, it's impossible to know in advance what this
// set will be, which means we need to run two full passes over the data -- the
// first determines lower-cardinality data that informs what data to gather for
// the higher-cardinality second pass.
struct UsageStats {
	auto add(GenerationGeneric<Team> const & team, double weight) & -> void;

	constexpr auto get_total(Species const species) const {
		return containers::at(m_all, species).total;
	}
	constexpr auto get(Species const species, Ability const ability) const {
		auto const & per_species = containers::at(m_all, species);
		return containers::at(per_species.abilities, ability);
	}
	constexpr auto get(Species const species, Item const item) const {
		auto const & per_species = containers::at(m_all, species);
		return containers::at(per_species.items, item);
	}
	constexpr auto const & moves(Species const species) const {
		return containers::at(m_all, species).moves;
	}
	constexpr auto get(Species const species, Moves const move_name) const {
		auto const & per_species = containers::at(m_all, species);
		return containers::at(per_species.moves, move_name);
	}
	constexpr auto const & speed_distribution(Species const species) const {
		return containers::at(m_all, species).speed;
	}
	constexpr auto total_teams() const {
		return m_total_teams;
	}
	constexpr auto total_teams_unweighted() const {
		return m_total_teams_unweighted;
	}
private:
	// Regieleki with a Choice Scarf
	static constexpr auto max_initial_speed = 822_bi;

	struct PerSpecies {
		double total = 0.0;
		containers::array<double, number_of<Ability>.value()> abilities{};
		containers::array<double, number_of<Item>.value()> items{};
		containers::array<double, number_of<Moves>.value()> moves{};
		containers::array<double, max_initial_speed.value()> speed{};
	};
	containers::array<PerSpecies, number_of<Species>.value()> m_all;
	double m_total_teams = 0.0;
	bounded::integer<0, bounded::builtin_max_value<std::uint64_t>> m_total_teams_unweighted = 0_bi;
};

struct Correlations {
private:
	static constexpr auto top_n_cutoff = 20_bi;
	struct PairingKey {
		Species other_species;
		Moves other_move;
		friend constexpr auto operator<=>(PairingKey, PairingKey) = default;
		friend constexpr auto to_radix_sort_key(PairingKey const x) -> bounded::tuple<Species, Moves> {
			return bounded::tuple(x.other_species, x.other_move);
		}
	};
	struct Data {
		containers::flat_map<PairingKey, double> moves_and_species;
		containers::flat_map<Item, double> items;
		containers::flat_map<Ability, double> abilities;
	};
public:
	using TopMoves = containers::static_flat_map<Moves, Data, top_n_cutoff.value()>;

	Correlations(UsageStats const & general_usage_stats);
	auto add(GenerationGeneric<Team> const & team, double weight) & -> void;

	constexpr auto top_moves(Species const species) const -> TopMoves const & {
		return containers::at(m_top_moves, species);
	}
private:
	containers::array<TopMoves, number_of<Species>.value()> m_top_moves;
};

} // namespace technicalmachine::ps_usage_stats
