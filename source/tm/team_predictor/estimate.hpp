// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/moves.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/item.hpp>
#include <tm/ability.hpp>

#include <bounded/optional.hpp>

#include <containers/flat_map.hpp>

#include <random>

namespace technicalmachine {

struct UsageStats;

struct Estimate {
	struct PerSpecies {
		double usage;
		containers::flat_map<Moves, double> moves;
		containers::flat_map<Item, double> items;
		containers::flat_map<Ability, double> abilities;
	};
	using Map = containers::flat_map<Species, PerSpecies>;

	explicit Estimate(UsageStats const & usage_stats);
	auto update(UsageStats const &, Species) -> void;
	auto update(UsageStats const &, Species, Moves) -> void;
	auto update(UsageStats const &, Species, Item) -> void;
	auto update(UsageStats const &, Species, Ability) -> void;

	auto most_likely_species() const -> bounded::optional<Species>;
	auto random_species(std::mt19937 & random_engine) const -> bounded::optional<Species>;

	auto most_likely_move(Species) const -> bounded::optional<Moves>;
	auto random_move(std::mt19937 & random_engine, Species) const -> bounded::optional<Moves>;

	auto most_likely_item(Species) const -> bounded::optional<Item>;
	auto random_item(std::mt19937 & random_engine, Species) const -> bounded::optional<Item>;

	auto most_likely_ability(Species) const -> bounded::optional<Ability>;
	auto random_ability(std::mt19937 & random_engine, Species) const -> bounded::optional<Ability>;

	auto probability(Species const species) const -> double {
		auto const per_species = containers::lookup(m_estimate, species);
		return per_species ? per_species->usage : 0.0;
	}
	auto probability(Species const species, Moves const move) const -> double {
		auto const per_species = containers::lookup(m_estimate, species);
		if (!per_species) {
			return 0.0;
		}
		auto per_move = containers::lookup(per_species->moves, move);
		return per_move ? *per_move : 0.0;
	}

private:
	Map m_estimate;
};

} // namespace technicalmachine
