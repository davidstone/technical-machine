// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/ps_usage_stats/parse_log.hpp>

#include <tm/clients/pokemon_showdown/parse_generation_from_format.hpp>

#include <tm/move/max_moves_per_pokemon.hpp>

#include <tm/stat/generic_stats.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/string_conversions/species.hpp>

#include <tm/constant_generation.hpp>

#include <containers/algorithms/transform.hpp>

#include <nlohmann/json.hpp>

#include <fstream>

namespace technicalmachine::ps_usage_stats {
namespace {

template<Generation generation>
auto parse_stats_impl(nlohmann::json const & stats, auto const convert) {
	auto read = [&](char const * const key) {
		return convert(stats.at(key).get<nlohmann::json::number_integer_t>());
	};
	return GenericStats(
		read("hp"),
		read("atk"),
		read("def"),
		read("spa"),
		read("spd"),
		read("spe")
	);
}

template<typename T>
constexpr auto parse_individual_stat = [](nlohmann::json::number_integer_t const value) {
	return T(bounded::check_in_range<typename T::value_type>(value));
};

template<Generation generation>
auto parse_evs(nlohmann::json const & stats) {
	auto const evs = parse_stats_impl<generation>(stats, parse_individual_stat<EV>);
	if constexpr (generation <= Generation::two) {
		return to_old_gen_evs(evs);
	} else {
		return evs;
	}
}

template<Generation generation>
auto parse_dvs_or_ivs(nlohmann::json const & stats) {
	if constexpr (generation <= Generation::two) {
		return to_dvs(parse_stats_impl<generation>(stats, [](nlohmann::json::number_integer_t const value) {
			return DV(parse_individual_stat<IV>(value));
		}));
	} else {
		return parse_stats_impl<generation>(stats, parse_individual_stat<IV>);
	}
}

template<Generation generation>
auto parse_team(nlohmann::json const & team_array) -> Team<generation> {
	return Team<generation>(PokemonContainer<generation>(containers::transform(team_array, [&](nlohmann::json const & pokemon) {
		auto const species = from_string<Species>(pokemon.at("species").get<std::string_view>());
		auto const item = from_string<Item>(pokemon.at("item").get<std::string_view>());
		auto const ability = from_string<Ability>(pokemon.at("ability").get<std::string_view>());
		auto const gender = [&] {
			auto const gender_str = pokemon.value("gender", "");
			return
				gender_str == "" or gender_str == "N" ? Gender::genderless :
				gender_str == "F" ? Gender::female :
				gender_str == "M" ? Gender::male :
				throw std::runtime_error("Invalid Gender string");
		}();
		auto const moves = RegularMoves(containers::transform(
			pokemon.at("moves"),
			[](nlohmann::json const & move) { return Move(generation, from_string<Moves>(move.get<std::string_view>())); }
		));
		auto const level = Level(bounded::check_in_range<Level::value_type>(pokemon.at("level").get<nlohmann::json::number_integer_t>()));
		auto const nature = from_string<Nature>(pokemon.at("nature").get<std::string_view>());
		auto const evs = parse_evs<generation>(pokemon.at("evs"));
		auto const dvs_or_ivs = parse_dvs_or_ivs<generation>(pokemon.at("ivs"));
		auto const happiness = Happiness(bounded::check_in_range<Happiness::value_type>(pokemon.value("happiness", 255)));
		return Pokemon<generation>(
			species,
			level,
			gender,
			item,
			ability,
			CombinedStats<generation>{
				nature,
				dvs_or_ivs,
				evs
			},
			moves,
			happiness
		);
	})));
}

auto parse_rating(nlohmann::json const & json, char const * const player_key) -> bounded::optional<Rating> {
	auto const & rating_json = json.at(player_key);
	if (rating_json.is_null()) {
		return bounded::none;
	}
	auto get = [&](char const * const key) {
		return rating_json.at(key).get<double>();
	};
	return Rating{get("rpr"), get("rprd")};
}

} // namespace

auto parse_log(std::filesystem::path const & path) -> BattleResult {
	auto file = std::ifstream(path);
	auto json = nlohmann::json();
	file >> json;
	auto const format = json.at("format").get<std::string_view>();
	auto const generation = ps::parse_generation_from_format(format);
	auto const p1 = json.at("p1").get<std::string_view>();
	auto const p2 = json.at("p2").get<std::string_view>();
	auto const winner = json.at("winner").get<std::string_view>();
	auto const side1_won =
		winner == p1 ? true :
		winner == p2 ? false :
		throw std::runtime_error("Invalid winner string");

	auto get_team = [&](char const * const key) {
		return constant_generation(generation, [&]<Generation g>(constant_gen_t<g>) {
			return GenerationGeneric<Team>(parse_team<g>(json.at(key)));
		});
	};
	auto const side1 = BattleResult::Side{
		get_team("p1team"),
		// Arr
		parse_rating(json, "p1rating")
	};
	auto const side2 = BattleResult::Side{
		get_team("p2team"),
		parse_rating(json, "p2rating")
	};
	return BattleResult{
		side1_won ? side1 : side2,
		side1_won ? side2 : side1
	};
}

} // namespace technicalmachine::ps_usage_stats
