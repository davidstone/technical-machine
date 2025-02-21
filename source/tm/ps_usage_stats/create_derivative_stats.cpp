// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <fstream>
#include <memory>

import tm.ps_usage_stats.battle_result_reader;
import tm.ps_usage_stats.glicko1;
import tm.ps_usage_stats.rating;
import tm.ps_usage_stats.usage_stats;

import tm.pokemon.species;

import tm.string_conversions.generation;
import tm.string_conversions.species;

import tm.load_json_from_file;
import tm.nlohmann_json;
import tm.open_file;

import bounded;
import containers;
import std_module;

namespace technicalmachine::ps_usage_stats {
namespace {

using namespace std::string_view_literals;

struct ParsedArgs {
	std::filesystem::path output_directory;
	std::filesystem::path full_stats_path;
};

auto parse_args(int argc, char const * const * argv) -> ParsedArgs {
	if (argc < 2) {
		throw std::runtime_error(
			"Usage is create_derivative_stats output_directory full_stats_path\n"
			"If full_stats_path is not given, it is assumed to be output_directory/stats.json\n"
		);
	}
	auto output_directory = std::filesystem::path(argv[1]);
	auto full_stats_path = argc == 2 ? output_directory / "stats.json" : std::filesystem::path(argv[2]);
	if (!std::filesystem::exists(full_stats_path)) {
		throw std::runtime_error(containers::concatenate<std::string>(full_stats_path.string(), " does not exist"sv));
	}
	auto const general_usage_stats_path = output_directory / "pokemon_usage_stats.json";
	if (std::filesystem::exists(general_usage_stats_path)) {
		throw std::runtime_error(containers::concatenate<std::string>(general_usage_stats_path.string(), " already exists"sv));
	}
	std::filesystem::create_directories(output_directory);
	return ParsedArgs{
		std::move(output_directory),
		std::move(full_stats_path)
	};
}

auto write_per_pokemon_detailed_stats(auto const & element, std::filesystem::path const & directory) -> void {
	auto file = open_text_file_for_writing(directory / ("stats_" + element.key() + ".json"));
	file << element.value();
}

struct PokemonUsageStats {
	auto add(auto const & element) & -> void {
		containers::emplace_back(
			m_data,
			element.value().at("Usage").template get<double>(),
			from_string<Species>(element.key())
		);
	}

	auto write(std::filesystem::path const & directory) -> void {
		containers::sort(m_data, std::greater());
		auto file = open_text_file_for_writing(directory / "pokemon_usage_stats.json");
		auto json = nlohmann::json::array_t();
		for (auto const & value : m_data) {
			json.push_back({{to_string(value.species), value.proportion}});
		}
		file << nlohmann::json(json);
	}

private:
	struct PokemonUsageStat {
		double proportion;
		Species species;

		friend auto operator<=>(PokemonUsageStat, PokemonUsageStat) = default;
	};
	containers::static_vector<PokemonUsageStat, bounded::number_of<Species>> m_data;
};

struct SpeedStats {
	auto add(nlohmann::json const & speeds) & -> void {
		for (auto const speed : speeds.items()) {
			m_data[bounded::to_integer<containers::index_type<SpeedDistribution>>(speed.key())] += speed.value().template get<double>();
		}
	}

	auto write(std::filesystem::path const & directory) -> void {
		auto file = open_text_file_for_writing(directory / "speed.json");
		auto const total = containers::sum(m_data);
		auto json = nlohmann::json::array_t();
		for (auto const index : containers::integer_range(containers::size(m_data))) {
			auto const value = m_data[index];
			if (value != 0.0) {
				json.push_back({{containers::to_string(index), value / total}});
			}
		}
		file << nlohmann::json(json);
	}

private:
	SpeedDistribution m_data{};
};

} // namespace
} // namespace technicalmachine::ps_usage_stats

auto main(int argc, char ** argv) -> int {
	using namespace technicalmachine;
	using namespace technicalmachine::ps_usage_stats;

	auto const args = parse_args(argc, argv);
	auto const original_json = load_json_from_file(args.full_stats_path);
	std::filesystem::create_directories(args.output_directory);
	auto pokemon_usage_stats = PokemonUsageStats();
	auto speed_stats = SpeedStats();
	for (auto const & element : original_json.at("Pokemon").items()) {
		write_per_pokemon_detailed_stats(element, args.output_directory);
		pokemon_usage_stats.add(element);
		speed_stats.add(element.value().at("Speed"));
	}
	pokemon_usage_stats.write(args.output_directory);
	speed_stats.write(args.output_directory);
	return 0;
}
