// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.team_predictor.server.predicted_team_string;

import tm.move.move;

import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.seen_pokemon;

import tm.string_conversions.team;

import tm.team_predictor.server.predictor_inputs;
import tm.team_predictor.server.style;

import tm.team_predictor.all_usage_stats;
import tm.team_predictor.team_predictor;

import tm.constant_generation;
import tm.gender;
import tm.generation;
import tm.team;

import containers;
import std_module;

namespace technicalmachine {

template<Generation generation>
constexpr auto parsed_pokemon_to_seen_pokemon(PredictorPokemon const parsed) -> SeenPokemon<generation> {
	auto pokemon = SeenPokemon<generation>(
		parsed.species,
		parsed.level,
		Gender::genderless
	);
	if constexpr (generation >= Generation::two) {
		if (parsed.item) {
			pokemon.set_item(*parsed.item);
		}
	} else {
		if (parsed.item) {
			throw std::runtime_error("Received an item before items exist");
		}
	}
	if (parsed.ability) {
		pokemon.set_initial_ability(*parsed.ability);
	}
	for (auto const move : parsed.moves) {
		pokemon.add_move(Move(generation, move));
	}
	if constexpr (generation >= Generation::three) {
		if (!parsed.nature) {
			throw std::runtime_error("Did not receive Nature");
		}
		if (!parsed.evs) {
			throw std::runtime_error("Did not receive EVs");
		}
		pokemon.set_ivs_and_evs({
			.nature = *parsed.nature,
			.evs = *parsed.evs
		});
	} else {
		if (parsed.nature) {
			throw std::runtime_error("Received a nature before natures exist.");
		}
		if (parsed.evs) {
			throw std::runtime_error("Received EVs in an old generation.");
		}
	}
	return pokemon;
}

template<Generation generation>
constexpr auto parsed_team_to_seen_team(PredictorTeam const parsed) -> SeenTeam<generation> {
	auto team = SeenTeam<generation>(max_pokemon_per_team);
	for (auto const pokemon : parsed) {
		team.add_pokemon(parsed_pokemon_to_seen_pokemon<generation>(pokemon));
	}
	return team;
}

export auto predicted_team_string(
	PredictorInputs const inputs,
	AllUsageStats const & all_usage_stats,
	std::mt19937 & random_engine
) -> containers::string {
	try {
		auto const & usage_stats = all_usage_stats[inputs.generation];
		auto impl = [&]<Generation g>(constant_gen_t<g>) -> containers::string {
			auto seen_team = parsed_team_to_seen_team<g>(inputs.team);
			auto const predicted_team = inputs.style == Style::random ?
				random_team(usage_stats, random_engine, std::move(seen_team)) :
				most_likely_team(usage_stats, std::move(seen_team));
			return to_string(predicted_team);
		};
		return constant_generation(inputs.generation, impl);
	} catch (std::exception const & ex) {
		return containers::string(ex.what());
	}
}

} // namespace technicalmachine
