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
private:
	// Regieleki with a Choice Scarf
	static constexpr auto max_initial_speed = 822_bi;

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

struct Correlations {
private:
	static constexpr auto top_n_cutoff = 20_bi;
	struct PerSpecies {
		double usage = 0.0;
		containers::flat_map<Moves, double> other_moves;
	};
	struct Data {
		Data():
			m_impl(std::make_unique<Impl>())
		{
		}

		auto lock() & {
			return std::scoped_lock(m_impl->mutex);
		}

		auto teammates() const -> auto const & {
			return m_impl->teammates;
		}
		auto teammates() -> auto & {
			return m_impl->teammates;
		}
		auto moves() const -> auto const & {
			return m_impl->moves;
		}
		auto moves() -> auto & {
			return m_impl->moves;
		}
		auto items() const -> auto const & {
			return m_impl->items;
		}
		auto items() -> auto & {
			return m_impl->items;
		}
		auto abilities() const -> auto const & {
			return m_impl->abilities;
		}
		auto abilities() -> auto & {
			return m_impl->abilities;
		}

	private:
		struct Impl {
			using Teammates = containers::array<PerSpecies, number_of<Species>>;
			mutable std::mutex mutex;
			Teammates teammates;
			containers::array<double, number_of<Moves>> moves = {};
			containers::array<double, number_of<Item>> items = {};
			containers::array<double, number_of<Ability>> abilities = {};
		};
		std::unique_ptr<Impl> m_impl;
	};
public:
	using TopMoves = containers::static_flat_map<Moves, Data, top_n_cutoff.value()>;

	Correlations(UsageStats const & general_usage_stats);
	auto add(GenerationGeneric<Team> const & team, double weight) & -> void;

	constexpr auto top_moves(Species const species) const -> TopMoves const & {
		return containers::at(m_top_moves, species);
	}
private:
	containers::array<TopMoves, number_of<Species>> m_top_moves;
};

} // namespace technicalmachine::ps_usage_stats
