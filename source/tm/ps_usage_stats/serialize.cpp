// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/ps_usage_stats/serialize.hpp>

#include <tm/ps_usage_stats/header.hpp>
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

template<typename T>
constexpr auto write_bytes(std::ostream & stream, T const & value, auto const expected_size) {
	static_assert(!std::is_empty_v<T>);
	static_assert(std::is_trivial_v<T>);
	static_assert(sizeof(value) == expected_size);
	stream.write(reinterpret_cast<char const *>(std::addressof(value)), sizeof(value));
}

constexpr auto write_string(std::ostream & stream, std::string_view const value) {
	stream.write(value.data(), static_cast<std::streamsize>(value.size()));
}

template<typename Key>
struct HasUsage {
	constexpr auto operator()(containers::map_value_type<Key, double> const & x) const {
		return x.mapped != 0.0;
	}
	constexpr auto operator()(containers::map_value_type<Key, Correlations::PerTeammate> const & x) const {
		return x.mapped.usage != 0.0;
	}
};

template<typename Key>
constexpr auto get_used_impl(auto && range, auto const get) {
	auto transformer = [=](Key const name) { return containers::map_value_type{name, get(name)}; };
	return containers::filter(
		containers::transform(OPERATORS_FORWARD(range), transformer),
		HasUsage<Key>()
	);
}

template<typename Key>
constexpr auto get_used(auto const get) {
	return get_used_impl<Key>(containers::enum_range<Key>(), get);
}

template<typename Source>
constexpr auto used_to_map(Source const & source) {
	using Key = containers::index_type<Source>;
	return get_used_impl<Key>(
		containers::integer_range(containers::size(source)),
		[&](Key const key) { return source[key]; }
	);
}

auto serialize_simple_correlations(std::ostream & stream, auto const & source, double const total) -> void {
	write_bytes(stream, containers::linear_size(source), 2_bi);
	for (auto const related : source) {
		write_bytes(stream, related.key, 2_bi);
		write_bytes(stream, related.mapped / total, 8_bi);
	}
}

template<typename T>
auto serialize(std::ostream & stream, UsageStats const & usage_stats, containers::map_value_type<Species, double> const species) -> void {
	auto const used = get_used<T>([&](T const value) { return usage_stats.get(species.key, value); });
	return serialize_simple_correlations(stream, used, species.mapped);
}

auto serialize_speed(std::ostream & stream, SpeedDistribution const & distribution, double const total) -> void {
	auto const used_speeds = used_to_map(distribution);
	write_bytes(stream, containers::linear_size(used_speeds), 2_bi);
	for (auto const speed : used_speeds) {
		write_bytes(stream, speed.key, 2_bi);
		write_bytes(stream, speed.mapped / total, 8_bi);
	}
};

auto serialize_teammates(std::ostream & stream, Correlations::Teammates const & source, double const total) -> void {
	auto const used = used_to_map(source);
	write_bytes(stream, containers::linear_size(used), 2_bi);
	for (auto const & related : used) {
		write_bytes(stream, related.key, 2_bi);
		write_bytes(stream, related.mapped.usage / total, 8_bi);
		serialize_simple_correlations(stream, related.mapped.other_moves, related.mapped.usage);
	}
}

[[clang::no_destroy]] constexpr auto empty_move_data = Correlations::MoveData();

auto serialize_moves(std::ostream & stream, Generation const generation, UsageStats const & usage_stats, Species const species, Correlations::TopMoves const & top_moves, double const species_total) -> void {
	auto const used_moves = get_used<Moves>([&](Moves const name) { return usage_stats.get(species, name); });
	write_bytes(stream, containers::linear_size(used_moves), 2_bi);
	for (auto const move : used_moves) {
		write_bytes(stream, move.key, 2_bi);
		write_bytes(stream, move.mapped / species_total, 8_bi);

		auto serialize_all = [&](Correlations::MoveData const & data) {
			serialize_speed(stream, data.speed, move.mapped);
			serialize_teammates(stream, data.teammates, move.mapped);
			serialize_simple_correlations(stream, used_to_map(data.moves), move.mapped);
			if (generation >= Generation::two) {
				serialize_simple_correlations(stream, used_to_map(data.items), move.mapped);
			}
			if (generation >= Generation::three) {
				serialize_simple_correlations(stream, used_to_map(data.abilities), move.mapped);
			}
		};
		if (auto const top_move = containers::lookup(top_moves, move.key)) {
			serialize_all((*top_move)->unlocked());
		} else {
			serialize_all(empty_move_data);
		}
	}
}

} // namespace

auto serialize(std::ostream & stream, Generation const generation, UsageStats const & usage_stats, Correlations const & correlations) -> void {
	using Version = bounded::integer<0, 65535>;
	write_string(stream, usage_stats_magic_string);
	constexpr auto version = Version(0_bi);
	write_bytes(stream, version, 2_bi);
	write_bytes(stream, generation, 1_bi);
	auto const all_species = get_used<Species>([&](Species const species) { return usage_stats.get_total(species); });
	write_bytes(stream, containers::linear_size(all_species), 2_bi);
	for (auto const usage : all_species) {
		auto const species = usage.key;
		auto const total = usage.mapped;
		write_bytes(stream, species, 2_bi);
		write_bytes(stream, total / usage_stats.total_teams(), 8_bi);
		serialize_speed(stream, usage_stats.speed_distribution(species), total);
		serialize_teammates(stream, correlations.teammates(species), total);
		serialize_moves(stream, generation, usage_stats, species, correlations.top_moves(species), total);
		if (generation >= Generation::two) {
			serialize<Item>(stream, usage_stats, usage);
		}
		if (generation >= Generation::three) {
			serialize<Ability>(stream, usage_stats, usage);
		}
	}
}

} // namespace technicalmachine::ps_usage_stats
