// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.team_predictor.server.parse_predictor_inputs;

import tm.move.move_name;
import tm.move.move_names;

import tm.pokemon.level;
import tm.pokemon.species;

import tm.stat.ev;
import tm.stat.evs;
import tm.stat.nature;
import tm.stat.stat_style;

import tm.string_conversions.ability;
import tm.string_conversions.generation;
import tm.string_conversions.item;
import tm.string_conversions.nature;
import tm.string_conversions.move_name;
import tm.string_conversions.species;

import tm.team_predictor.server.predictor_inputs;
import tm.team_predictor.server.style;

import tm.ability;
import tm.gender;
import tm.generation;
import tm.item;
import tm.nlohmann_json;

import bounded;
import containers;
import std_module;
import tv;

namespace technicalmachine {

// Cannot use `containers::lookup` because `nlohmann::json` doesn't provide
// `key_type` and `mapped_type`, and `find` just returns the mapped element.
auto lookup(nlohmann::json const & map, std::string_view const key) {
	auto const it = map.find(key);
	return it != containers::end(map) ?
		std::addressof(*it) :
		nullptr;
}

auto get_str(nlohmann::json const & json, std::string_view const key) -> std::string_view {
	return json.at(key).get<std::string_view>();
}

template<typename T>
auto get_integer_wrapper(nlohmann::json const & json, std::string_view const key) -> T {
	return T(bounded::to_integer<typename T::value_type>(get_str(json, key)));
}

auto parse_evs(nlohmann::json const & pokemon) -> tv::optional<EVs> {
	auto const evs = lookup(pokemon, "evs");
	if (!evs) {
		return tv::none;
	}
	auto get = [&](std::string_view const key) -> EV {
		return get_integer_wrapper<EV>(*evs, key);
	};
	auto const result = EVs(
		get("HP"),
		get("Atk"),
		get("Def"),
		get("SpA"),
		get("SpD"),
		get("Spe")
	);
	if (ev_sum(result) > max_total_evs(SpecialStyle::split)) {
		throw std::runtime_error("Too many EVs");
	}
	return result;
}

auto parse_predictor_team(nlohmann::json const & team) -> PredictorTeam {
	return PredictorTeam(containers::transform(team, [](nlohmann::json const & pokemon) {
		auto get_opt = [&]<typename T>(bounded::type_t<T>, std::string_view const key) -> tv::optional<T> {
			auto const value = lookup(pokemon, key);
			if (!value) {
				return tv::none;
			}
			return from_string<T>(value->get<std::string_view>());
		};
		return PredictorPokemon(
			from_string<Species>(get_str(pokemon, "species")),
			get_integer_wrapper<Level>(pokemon, "level"),
			get_opt(bounded::type<Item>, "item"),
			get_opt(bounded::type<Ability>, "ability"),
			get_opt(bounded::type<Nature>, "nature"),
			parse_evs(pokemon),
			MoveNames(containers::transform(
				pokemon.at("moves"),
				[](nlohmann::json const & move) {
					return from_string<MoveName>(move.get<std::string_view>());
				}
			))
		);
	}));
}

export auto parse_predictor_inputs(std::string_view const str) -> PredictorInputs {
	auto const json = nlohmann::json::parse(str);
	return PredictorInputs(
		from_string<Generation>(get_str(json, "generation")),
		from_string<Style>(get_str(json, "style")),
		parse_predictor_team(json.at("team"))
	);
}

} // namespace technicalmachine
