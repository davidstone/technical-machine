// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/ps_usage_stats/battle_result_reader.hpp>
#include <tm/ps_usage_stats/glicko1.hpp>
#include <tm/ps_usage_stats/rating.hpp>
#include <tm/ps_usage_stats/serialize.hpp>
#include <tm/ps_usage_stats/usage_stats.hpp>

#include <tm/string_conversions/generation.hpp>

#include <tm/load_json_from_file.hpp>

#include <bounded/to_integer.hpp>

#include <containers/append.hpp>
#include <containers/range_view.hpp>
#include <containers/vector.hpp>

#include <compare>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string_view>

#include <iostream>
#include <memory>
#include <utility>

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
	auto file = std::ofstream(directory / ("stats_" + element.key() + ".json"));
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
		auto file = std::ofstream(directory / "pokemon_usage_stats.json");
		auto json = nlohmann::json::array_t();
		for (auto const & value : m_data) {
			json.push_back({{std::string(to_string(value.species)), value.proportion}});
		}
		file << nlohmann::json(json);
	}

private:
	struct PokemonUsageStat {
		double proportion;
		Species species;

		friend auto operator<=>(PokemonUsageStat, PokemonUsageStat) = default;
	};
	containers::static_vector<PokemonUsageStat, number_of_species> m_data;
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
	for (auto const & element : original_json.at("Pokemon").items()) {
		write_per_pokemon_detailed_stats(element, args.output_directory);
		pokemon_usage_stats.add(element);
	}
	pokemon_usage_stats.write(args.output_directory);
	return 0;
}
