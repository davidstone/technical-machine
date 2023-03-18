// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>
#include <bounded/conditional.hpp>

export module tm.team_predictor.estimate;

import tm.move.move_name;

import tm.pokemon.species;

import tm.team_predictor.usage_stats_probabilities;
import tm.team_predictor.usage_stats;

import tm.ability;
import tm.item;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine {

struct PerSpecies {
	double usage;
	containers::flat_map<MoveName, double> moves;
	containers::flat_map<Item, double> items;
	containers::flat_map<Ability, double> abilities;
};

constexpr auto update_per_species(auto & data, auto const & per_species, auto const base_per_species) {
	for (auto & element : data) {
		auto const base = base_per_species(element.key);
		// If users do something very surprising, we don't want to lower the
		// odds of everything to 0.
		auto const multiplier = bounded::max(
			base == 0.0F ? 0.0 : static_cast<double>(per_species(element.key) / base),
			1.0 / 1'000'000.0
		);
		element.mapped *= multiplier;
	}
}

constexpr auto make_per_species(UsageStatsProbabilities::Inner const & probabilities) -> PerSpecies {
	auto const transformer = [](auto const element) {
		return containers::map_value_type{element.key, static_cast<double>(element.mapped)};
	};
	return PerSpecies{
		containers::sum(containers::transform(probabilities.moves.map(), [](auto const & element) { return static_cast<double>(element.mapped); })),
		containers::flat_map<MoveName, double>(containers::transform(probabilities.moves.map(), transformer)),
		containers::flat_map<Item, double>(containers::transform(probabilities.items.map(), transformer)),
		containers::flat_map<Ability, double>(containers::transform(probabilities.abilities.map(), transformer))
	};
}

constexpr auto get_most_likely(containers::range auto const & range) {
	auto const it = containers::max_element(range);
	return BOUNDED_CONDITIONAL(it == containers::end(range), tv::none, it.base()->key);
}

template<typename T, containers::range Range>
constexpr auto select_random_by_weight(Range const & range, std::mt19937 & random_engine, auto const transform) -> tv::optional<T> {
	if (containers::is_empty(range)) {
		return tv::none;
	}
	auto const total = containers::sum(containers::transform(range, transform));
	auto distribution = std::uniform_real_distribution(0.0, total);
	auto usage_threshold = distribution(random_engine);
	for (auto const & value : range) {
		usage_threshold -= transform(value);
		if (usage_threshold <= 0.0) {
			return value.key;
		}
	}
	// In case rounding leads to a minor error
	return containers::back(range).key;
}

constexpr auto per_species_to_usage = [](auto const & value) {
	return value.mapped.usage;
};

export struct Estimate {
	using Map = containers::flat_map<Species, PerSpecies>;

	explicit constexpr Estimate(UsageStats const & usage_stats):
		m_estimate(containers::transform(usage_stats.assuming().map(), [](auto const & value) {
			return containers::map_value_type{value.key, make_per_species(value.mapped)};
		}))
	{
		auto check = [](auto const & range) {
			for (auto const & element : range) {
				BOUNDED_ASSERT(element.mapped > 0.0);
			}
		};
		for (auto const & per_species : m_estimate) {
			BOUNDED_ASSERT(per_species.mapped.usage > 0.0);
			BOUNDED_ASSERT(!containers::is_empty(per_species.mapped.moves));
			check(per_species.mapped.moves);
			check(per_species.mapped.items);
			check(per_species.mapped.abilities);
		}
	}
	constexpr auto update(UsageStats const & usage_stats, Species const species) -> void {
		if (auto const probabilities = usage_stats.assuming(species)) {
			do_update(*probabilities, usage_stats.assuming());
		}
		if (auto const species_data = containers::lookup(m_estimate, species)) {
			species_data->usage = 0.0;
		}
	}
	constexpr auto update(UsageStats const & usage_stats, Species const species, MoveName const move) -> void {
		if (auto const probabilities = usage_stats.assuming(species, move)) {
			auto const base_probabilities = usage_stats.assuming(species);
			BOUNDED_ASSERT(base_probabilities);
			do_update(*probabilities, *base_probabilities);
		}
		if (auto const species_data = containers::lookup(m_estimate, species)) {
			containers::keyed_erase(species_data->moves, move);
		}
	}
	constexpr auto update(UsageStats const &, Species, Item) -> void {
	}
	constexpr auto update(UsageStats const &, Species, Ability) -> void {
	}

	constexpr auto most_likely_species() const -> tv::optional<Species> {
		return get_most_likely(containers::transform(m_estimate, per_species_to_usage));
	}
	constexpr auto random_species(std::mt19937 & random_engine) const -> tv::optional<Species> {
		return select_random_by_weight<Species>(m_estimate, random_engine, per_species_to_usage);
	}

	constexpr auto most_likely_move(Species const species) const -> tv::optional<MoveName> {
		auto const ptr = containers::lookup(m_estimate, species);
		if (!ptr) {
			return tv::none;
		}
		return get_most_likely(containers::transform(ptr->moves, containers::get_mapped));
	}
	constexpr auto random_move(std::mt19937 & random_engine, Species const species) const -> tv::optional<MoveName> {
		auto const element = containers::lookup(m_estimate, species);
		if (!element) {
			return tv::none;
		}
		return select_random_by_weight<MoveName>(element->moves, random_engine, containers::get_mapped);
	}

	constexpr auto most_likely_item(Species const species) const -> tv::optional<Item> {
		auto const ptr = containers::lookup(m_estimate, species);
		if (!ptr) {
			return tv::none;
		}
		return get_most_likely(containers::transform(ptr->items, containers::get_mapped));
	}
	constexpr auto random_item(std::mt19937 & random_engine, Species const species) const -> tv::optional<Item> {
		auto const element = containers::lookup(m_estimate, species);
		if (!element) {
			return tv::none;
		}
		return select_random_by_weight<Item>(element->items, random_engine, containers::get_mapped);
	}

	constexpr auto most_likely_ability(Species const species) const -> tv::optional<Ability> {
		auto const ptr = containers::lookup(m_estimate, species);
		if (!ptr) {
			return tv::none;
		}
		return get_most_likely(containers::transform(ptr->abilities, containers::get_mapped));
	}
	constexpr auto random_ability(std::mt19937 & random_engine, Species const species) const -> tv::optional<Ability> {
		auto const element = containers::lookup(m_estimate, species);
		if (!element) {
			return tv::none;
		}
		return select_random_by_weight<Ability>(element->abilities, random_engine, containers::get_mapped);
	}

	constexpr auto probability(Species const species) const -> double {
		auto const per_species = containers::lookup(m_estimate, species);
		return per_species ? per_species->usage : 0.0;
	}
	constexpr auto probability(Species const species, MoveName const move) const -> double {
		auto const per_species = containers::lookup(m_estimate, species);
		if (!per_species) {
			return 0.0;
		}
		auto per_move = containers::lookup(per_species->moves, move);
		return per_move ? *per_move : 0.0;
	}

private:
	constexpr auto do_update(UsageStatsProbabilities const & probability, UsageStatsProbabilities const & base_probability) & -> void {
		for (auto & per_species : m_estimate) {
			auto const species = per_species.key;
			auto const species_estimate = containers::lookup(probability.map(), species);
			if (!species_estimate) {
				per_species.mapped.usage = 0.0;
				continue;
			}
			auto const base_per_species = base_probability(species);
			update_per_species(
				per_species.mapped.moves,
				species_estimate->moves,
				[=](MoveName const move) { return base_per_species ? base_per_species->moves(move) : 0.0F; }
			);
			update_per_species(
				per_species.mapped.items,
				species_estimate->items,
				[=](Item const item) { return base_per_species ? base_per_species->items(item) : 0.0F; }
			);
			update_per_species(
				per_species.mapped.abilities,
				species_estimate->abilities,
				[=](Ability const ability) { return base_per_species ? base_per_species->abilities(ability) : 0.0F; }
			);
			per_species.mapped.usage =
				per_species.mapped.usage == 0.0 ?
				0.0 :
				containers::sum(containers::transform(per_species.mapped.moves, containers::get_mapped));
		}
	}

	Map m_estimate;
};

} // namespace technicalmachine
