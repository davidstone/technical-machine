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

#include <nlohmann/json.hpp>

#include <cstdint>
#include <string>

namespace technicalmachine::ps_usage_stats {
namespace {

template<typename Key>
constexpr auto get_used(auto const get, auto predicate) {
	using Mapped = decltype(get(std::declval<Key>()));
	auto transformer = [&](Key const name) { return containers::map_value_type<Key, Mapped>{name, get(name)}; };
	return containers::static_vector<containers::map_value_type<Key, Mapped>, number_of<Key>>(
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

template<typename Enum>
auto serialize_simple_correlations(auto const & source, double const total) -> nlohmann::json {
	auto result = nlohmann::json(nlohmann::json::object());
	for (auto const index : containers::enum_range<Enum>()) {
		auto const value = containers::at(source, index);
		if (value != 0.0) {
			result[std::string(to_string(index))] = value / total;
		}
	}
	return result;
}

auto serialize_simple_correlations(auto const & source, double const total) -> nlohmann::json {
	auto result = nlohmann::json(nlohmann::json::object());
	for (auto const related : source) {
		result[std::string(to_string(related.key))] = related.mapped / total;
	}
	return result;
}

template<typename T>
auto serialize(UsageStats const & usage_stats, containers::map_value_type<Species, double> const species) -> nlohmann::json {
	auto const used = get_used<T>([&](T const value) { return usage_stats.get(species.key, value); });
	return serialize_simple_correlations(used, species.mapped);
}

auto serialize_speed(auto const & speed_distribution, double const total) -> nlohmann::json {
	auto result = nlohmann::json();
	for (auto const index : containers::integer_range(containers::size(speed_distribution))) {
		auto const speed = speed_distribution[index];
		if (speed == 0.0) {
			continue;
		}
		result[std::string(to_string(index))] = speed / total;
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

auto serialize_moves(Generation const generation, Correlations::TopMoves const & top_moves, double const species_total) -> nlohmann::json {
	auto result = nlohmann::json();
	for (auto const & top_move : top_moves) {
		auto & per_move = result[std::string(to_string(top_move.key))];

		auto const & data = top_move.mapped->unlocked();
		auto const total = containers::sum(data.abilities);
		per_move["Usage"] = total / species_total;

		per_move["Moves"] = serialize_simple_correlations<Moves>(data.moves, total);
		per_move["Teammates"] = serialize_teammates(data.teammates, total);
		if (generation >= Generation::two) {
			per_move["Items"] = serialize_simple_correlations<Item>(data.items, total);
		}
		if (generation >= Generation::three) {
			per_move["Abilities"] = serialize_simple_correlations<Ability>(data.abilities, total);
		}
		per_move["Speed"] = serialize_speed(data.speed, total);
	}
	return result;
}

} // namespace

// This implementation significantly reduces memory consumption compared to
// constructing a single giant json object and writing that.
auto serialize(std::ostream & stream, Generation const generation, UsageStats const & usage_stats, Correlations const & correlations) -> void {
	stream << R"({"Pokemon":{)";
	bool is_first = true;
	for (auto const usage : get_used<Species>([&](Species const species) { return usage_stats.get_total(species); })) {
		if (!is_first) {
			stream << ',';
		} else {
			is_first = false;
		}
		auto const species = usage.key;
		auto const total = usage.mapped;
		auto pokemon = nlohmann::json({
			{"Usage", total / usage_stats.total_teams()},
			{"Moves", serialize_moves(generation, correlations.top_moves(species), total)},
			{"Speed", serialize_speed(usage_stats.speed_distribution(species), total)}
		});
		if (generation >= Generation::two) {
			pokemon["Items"] = serialize<Item>(usage_stats, usage);
		}
		if (generation >= Generation::three) {
			pokemon["Abilities"] = serialize<Ability>(usage_stats, usage);
		}
		stream << '"' << to_string(species) << "\":" << pokemon;
	}
	stream << R"(},"Total teams":)" << usage_stats.total_teams() << "}";
}

} // namespace technicalmachine::ps_usage_stats
