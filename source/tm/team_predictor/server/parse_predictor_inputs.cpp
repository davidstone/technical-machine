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
using namespace containers::string_literals;

// Cannot use `containers::lookup` because `nlohmann::json` doesn't provide
// `key_type` and `mapped_type`, and `find` just returns the mapped element.
auto lookup(nlohmann::json const & map, containers::string_view const key) {
	auto const it = map.find(key);
	return it != containers::end(map) ?
		std::addressof(*it) :
		nullptr;
}

auto get_str(nlohmann::json const & json, containers::string_view const key) -> containers::string_view {
	return json.at(key).get<std::string_view>();
}

template<typename T>
auto get_integer_wrapper(nlohmann::json const & json, containers::string_view const key) -> T {
	return T(bounded::to_integer<typename T::value_type>(get_str(json, key)));
}

auto parse_evs(nlohmann::json const & pokemon) -> tv::optional<EVs> {
	auto const evs = lookup(pokemon, "evs"_s);
	if (!evs) {
		return tv::none;
	}
	auto get = [&](containers::string_view const key) -> EV {
		return get_integer_wrapper<EV>(*evs, key);
	};
	auto const result = EVs(
		get("HP"_s),
		get("Atk"_s),
		get("Def"_s),
		get("SpA"_s),
		get("SpD"_s),
		get("Spe"_s)
	);
	if (ev_sum(result) > max_total_evs(SpecialInputStyle::split)) {
		throw std::runtime_error("Too many EVs");
	}
	return result;
}

auto parse_predictor_team(nlohmann::json const & team) -> PredictorTeam {
	return PredictorTeam(containers::transform(team, [](nlohmann::json const & pokemon) {
		auto get_opt = [&]<typename T>(bounded::type_t<T>, containers::string_view const key) -> tv::optional<T> {
			auto const value = lookup(pokemon, key);
			if (!value) {
				return tv::none;
			}
			return from_string<T>(value->get<std::string_view>());
		};
		return PredictorPokemon(
			from_string<Species>(get_str(pokemon, "species"_s)),
			get_integer_wrapper<Level>(pokemon, "level"_s),
			get_opt(bounded::type<Item>, "item"_s),
			get_opt(bounded::type<Ability>, "ability"_s),
			get_opt(bounded::type<Nature>, "nature"_s),
			parse_evs(pokemon),
			MoveNames(containers::transform(
				pokemon.at("moves"_s),
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
		from_string<Generation>(get_str(json, "generation"_s)),
		from_string<Style>(get_str(json, "style"_s)),
		parse_predictor_team(json.at("team"))
	);
}

} // namespace technicalmachine
