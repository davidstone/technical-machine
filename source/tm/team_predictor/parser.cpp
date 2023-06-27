// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string>

#include <bounded/conditional.hpp>

export module tm.team_predictor.parser;

import tm.team_predictor.all_usage_stats;
import tm.team_predictor.team_predictor;

import tm.move.max_moves_per_pokemon;
import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.pokemon.level;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.seen_pokemon;
import tm.pokemon.species;

import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.evs;
import tm.stat.iv;
import tm.stat.nature;

import tm.string_conversions.ability;
import tm.string_conversions.gender;
import tm.string_conversions.item;
import tm.string_conversions.nature;
import tm.string_conversions.move_name;
import tm.string_conversions.species;
import tm.string_conversions.team;

import tm.ability;
import tm.buffer_view;
import tm.constant_generation;
import tm.gender;
import tm.generation;
import tm.item;
import tm.team;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine {

using namespace std::string_view_literals;

constexpr auto get_expected_base(std::string_view const input, std::string_view const expected_key) -> std::string_view {
	auto [key, value] = split_view(input, '=');
	if (key != expected_key) {
		throw std::runtime_error(containers::concatenate<std::string>("Expected "sv, expected_key, " but got "sv, key));
	}
	return value;
}

template<typename T>
constexpr auto get_expected(std::string_view const input, std::string_view const key, std::string_view const index) -> tv::optional<T> {
	auto const value = get_expected_base(input, containers::concatenate<containers::string>(key, index));
	return BOUNDED_CONDITIONAL(value == containers::concatenate<containers::string>("Select+"sv, key), tv::none, from_string<T>(value));
}

template<typename T>
constexpr auto get_expected_integer_wrapper(std::string_view const input, std::string_view const key) -> T {
	return T(bounded::to_integer<typename T::value_type>(get_expected_base(input, key)));
}

using ParsedMoves = containers::static_vector<MoveName, max_moves_per_pokemon>;
struct ParsedPokemon {
	Species species;
	Level level;
	Gender gender;
	tv::optional<Item> item;
	tv::optional<Ability> ability;
	Nature nature;
	EVs evs;
	ParsedMoves moves;
};
using ParsedTeam = containers::static_vector<ParsedPokemon, max_pokemon_per_team>;

constexpr auto parse_html_team(DelimitedBufferView<std::string_view> buffer, Generation const generation) -> ParsedTeam {
	auto get_pokemon = [&](TeamIndex const index) -> tv::optional<ParsedPokemon> {
		auto const index_str = containers::to_string(index);
		auto get_integer_wrapper = [&]<typename T>(bounded::type_t<T>, std::string_view const key) -> T {
			return get_expected_integer_wrapper<T>(buffer.pop(), containers::concatenate<containers::string>(key, index_str));
		};
		auto get = [&]<typename T>(bounded::type_t<T>, std::string_view const key) -> tv::optional<T> {
			return get_expected<T>(buffer.pop(), key, index_str);
		};
		auto get_ev = [&](std::string_view const key) -> EV {
			return get_integer_wrapper(bounded::type<EV>, key);
		};
		auto get_move = [&](containers::index_type<RegularMoves> const move_index) -> tv::optional<MoveName> {
			return get_expected<MoveName>(
				buffer.pop(),
				"move",
				containers::concatenate<containers::string>(containers::to_string(index), "_"sv, containers::to_string(move_index))
			);
		};

		auto const species = get(bounded::type<Species>, "species");
		auto const level = get_integer_wrapper(bounded::type<Level>, "level");
		auto const gender = get(bounded::type<Gender>, "gender");
		auto const item = get(bounded::type<Item>, "item");
		auto const ability = get(bounded::type<Ability>, "ability");
		auto const nature = get(bounded::type<Nature>, "nature");
		auto const evs = EVs{
			get_ev("hp"),
			get_ev("atk"),
			get_ev("def"),
			get_ev("spa"),
			get_ev("spd"),
			get_ev("spe")
		};
		if (ev_sum(evs) > max_total_evs(generation)) {
			throw std::runtime_error("Too many EVs");
		}
		auto const moves = ParsedMoves(
			containers::remove_none(
				containers::transform(
					containers::integer_range(max_moves_per_pokemon),
					get_move
				)
			)
		);

		if (!species) {
			return tv::none;
		}

		return ParsedPokemon{
			*species,
			level,
			gender ? *gender : Gender::genderless,
			item,
			ability,
			nature ? *nature : Nature::Hardy,
			evs,
			moves
		};
	};

	return ParsedTeam(
		containers::remove_none(
			containers::transform(
				containers::integer_range(max_pokemon_per_team),
				get_pokemon
			)
		)
	);
}

template<Generation generation>
auto parsed_pokemon_to_seen_pokemon(ParsedPokemon const parsed) -> SeenPokemon<generation> {
	auto pokemon = SeenPokemon<generation>(
		parsed.species,
		parsed.level,
		parsed.gender
	);
	if (parsed.item) {
		pokemon.set_item(*parsed.item);
	}
	if (parsed.ability) {
		pokemon.set_initial_ability(*parsed.ability);
	}
	for (auto const move : parsed.moves) {
		pokemon.add_move(Move(generation, move));
	}
	auto const evs = [&] {
		if constexpr (generation <= Generation::two) {
			return to_old_gen_evs(parsed.evs);
		} else {
			return parsed.evs;
		}
	}();
	pokemon.set_ivs_and_evs(CombinedStats<generation>{
		parsed.nature,
		max_dvs_or_ivs<generation>,
		evs
	});
	return pokemon;
}

template<Generation generation>
auto parsed_team_to_seen_team(ParsedTeam const parsed) -> SeenTeam<generation> {
	auto team = SeenTeam<generation>(max_pokemon_per_team);
	for (auto const pokemon : parsed) {
		team.add_pokemon(SeenPokemon<generation>(
			parsed_pokemon_to_seen_pokemon<generation>(pokemon)
		));
	}
	return team;
}

export struct Parser {
	Parser(AllUsageStats const & all_usage_stats, std::mt19937 & random_engine):
		m_all_usage_stats(all_usage_stats),
		m_random_engine(random_engine)
	{
	}

	auto team_string(std::string_view const input_data) const -> containers::string {
		try {
			auto buffer = DelimitedBufferView(input_data, '&');
			auto const generation = from_string<Generation>(get_expected_base(buffer.pop(), "generation"));
			auto const parsed_team = parse_html_team(buffer, generation);
			auto & usage_stats = m_all_usage_stats[generation];
			auto impl = [&]<Generation generation>(constant_gen_t<generation>) -> containers::string {
				return to_string(
					random_team(
						usage_stats,
						m_random_engine,
						parsed_team_to_seen_team<generation>(parsed_team)
					)
				);
			};
			return constant_generation(generation, impl);
		} catch (std::exception const & ex) {
			return containers::string(ex.what());
		}
	}

private:
	AllUsageStats const & m_all_usage_stats;
	std::mt19937 & m_random_engine;
};

} // namespace technicalmachine
