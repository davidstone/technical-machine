// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/ps_usage_stats/serialize.hpp>

#include <tm/ps_usage_stats/usage_stats.hpp>

#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/species.hpp>

#include <containers/algorithms/accumulate.hpp>
#include <containers/algorithms/filter_iterator.hpp>
#include <containers/algorithms/transform.hpp>
#include <containers/integer_range.hpp>
#include <containers/size.hpp>
#include <containers/static_vector.hpp>

#include <cstdint>
#include <string>

namespace technicalmachine::ps_usage_stats {
namespace {

template<typename Key, typename Mapped = double>
using NameValue = containers::map_value_type<Key, Mapped>;

template<typename Key>
constexpr auto get_used(auto const get, auto predicate) {
	using Mapped = decltype(get(std::declval<Key>()));
	auto transformer = [&](Key const name) { return NameValue<Key, Mapped>{name, get(name)}; };
	return containers::static_vector<NameValue<Key, Mapped>, static_cast<std::size_t>(number_of<Key>)>(
		containers::filter(
			containers::transform(containers::enum_range<Key>(), transformer),
			predicate
		)
	);
}

template<typename Key>
constexpr auto get_used(auto const get) {
	return get_used<Key>(get, [](auto const x) { return x.mapped != 0.0; });
}

auto serialize_simple_correlations(auto const & source, double const total) -> nlohmann::json {
	auto result = nlohmann::json(nlohmann::json::object());
	for (auto const related : source) {
		result[std::string(to_string(related.key))] = related.mapped / total;
	}
	return result;
}

template<typename T>
auto serialize(UsageStats const & usage_stats, NameValue<Species> const species) -> nlohmann::json {
	auto const used = get_used<T>([&](T const value) { return usage_stats.get(species.key, value); });
	return serialize_simple_correlations(used, species.mapped);
}

auto serialize_speed(UsageStats const & usage_stats, NameValue<Species> const species) -> nlohmann::json {
	auto result = nlohmann::json();
	auto const & speed_distribution = usage_stats.speed_distribution(species.key);
	for (auto const index : containers::integer_range(containers::size(speed_distribution))) {
		auto const speed = speed_distribution[index];
		if (speed == 0.0) {
			continue;
		}
		result[std::string(to_string(index))] = speed / species.mapped;
	}
	return result;
};

auto serialize_teammates(auto const & source, double const total) -> nlohmann::json {
	auto result = nlohmann::json(nlohmann::json::object());
	auto transform = [&](Species const species) { return containers::at(source, species); };
	auto filter = [&](auto const & x) { return x.mapped.usage != 0.0; };
	for (auto const & related : get_used<Species>(transform, filter)) {
		auto & teammate = result[std::string(to_string(related.key))];
		teammate["Usage"] = related.mapped.usage / total;
		teammate["Moves"] = serialize_simple_correlations(related.mapped.other_moves, related.mapped.usage);
	}
	return result;
}

auto serialize_moves(Generation const generation, Correlations::TopMoves const & top_moves) -> nlohmann::json {
	auto result = nlohmann::json();
	for (auto const & top_move : top_moves) {
		auto & per_move = result[std::string(to_string(top_move.key))];

		auto const total = containers::sum(containers::transform(top_move.mapped.abilities(), [](auto const value) { return value.mapped; }));
		per_move["Usage"] = total;

		per_move["Moves"] = serialize_simple_correlations(top_move.mapped.moves(), total);
		per_move["Teammates"] = serialize_teammates(top_move.mapped.teammates(), total);
		if (generation >= Generation::two) {
			per_move["Items"] = serialize_simple_correlations(top_move.mapped.items(), total);
		}
		if (generation >= Generation::three) {
			per_move["Abilities"] = serialize_simple_correlations(top_move.mapped.abilities(), total);
		}
	}
	return result;
}

} // namespace

auto serialize(Generation const generation, UsageStats const & usage_stats, Correlations const & correlations) -> nlohmann::json {
	auto output = nlohmann::json();
	auto & all_pokemon = output["Pokemon"];
	for (auto const species : get_used<Species>([&](Species const species) { return usage_stats.get_total(species); })) {
		auto & species_data = all_pokemon[std::string(to_string(species.key))];
		species_data = {
			{"Usage", species.mapped},
			{"Moves", serialize_moves(generation, correlations.top_moves(species.key))},
			{"Speed", serialize_speed(usage_stats, species)}
		};
		if (generation >= Generation::two) {
			species_data["Items"] = serialize<Item>(usage_stats, species);
		}
		if (generation >= Generation::three) {
			species_data["Abilities"] = serialize<Ability>(usage_stats, species);
		}
	}
	output["Total teams"] = usage_stats.total_teams();

	return output;
}

} // namespace technicalmachine::ps_usage_stats
