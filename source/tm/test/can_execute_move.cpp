// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.test.block;

import tm.can_execute_move;

import tm.move.initial_move;
import tm.move.legal_selections;
import tm.move.move;
import tm.move.move_name;
import tm.move.move_names;
import tm.move.regular_moves;

import tm.pokemon.faint;
import tm.pokemon.level;
import tm.pokemon.pokemon;
import tm.pokemon.species;

import tm.string_conversions.move_name;

import tm.ability;
import tm.environment;
import tm.gender;
import tm.generation;
import tm.get_legal_selections;
import tm.item;
import tm.team;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

using namespace std::string_view_literals;

constexpr auto generation = Generation::four;

// All moves are legal moves
static_assert([]{
	auto environment = Environment();

	auto user = Team<generation>({{
		{
			.species = Species::Jolteon,
			.moves = {{
				MoveName::Thunderbolt,
				MoveName::Charm,
				MoveName::Thunder,
				MoveName::Shadow_Ball,
			}}
		},
	}});
	user.pokemon().switch_in(environment);

	auto other = Team<generation>({{
		{
			.species = Species::Gyarados,
			.moves = {{
				MoveName::Dragon_Dance,
				MoveName::Waterfall,
				MoveName::Stone_Edge,
				MoveName::Taunt,
			}}
		},
	}});
	other.pokemon().switch_in(environment);

	auto const selections = get_legal_selections(user, other, environment);
	return selections == LegalSelections({
		MoveName::Thunderbolt,
		MoveName::Charm,
		MoveName::Thunder,
		MoveName::Shadow_Ball,
	});
}());

constexpr auto remove_all_pp(auto const pokemon, MoveName const move, Environment const environment) {
	pokemon.reduce_pp(
		move,
		environment,
		*containers::find(pokemon.regular_moves(), move)->pp().remaining()
	);
};

// Two moves with one out of pp
static_assert([]{
	auto environment = Environment();

	auto user = Team<generation>({{
		{
			.species = Species::Pikachu,
			.moves = {{
				MoveName::Thunder,
				MoveName::Thunderbolt,
			}}
		},
	}});
	user.pokemon().switch_in(environment);
	remove_all_pp(user.pokemon(), MoveName::Thunder, environment);

	auto other = Team<generation>({{
		{
			.species = Species::Pikachu,
			.moves = {{
				MoveName::Tackle,
			}}
		},
	}});
	other.pokemon().switch_in(environment);

	auto const selections = get_legal_selections(user, other, environment);
	return selections == LegalSelections({MoveName::Thunderbolt});
}());

// Two moves with both out of pp
static_assert([]{
	auto environment = Environment();

	constexpr auto moves = InitialMoves({MoveName::Thunder, MoveName::Thunderbolt});
	auto user = Team<generation>({{
		{
			.species = Species::Pikachu,
			.moves = moves
		},
	}});
	user.pokemon().switch_in(environment);
	for (auto const move : moves) {
		remove_all_pp(user.pokemon(), move.name, environment);
	}

	auto other = Team<generation>({{
		{
			.species = Species::Pikachu,
			.moves = {{
				MoveName::Tackle,
			}}
		},
	}});
	other.pokemon().switch_in(environment);

	auto const selections = get_legal_selections(user, other, environment);
	return selections == LegalSelections({MoveName::Struggle});
}());

// Replace fainted
static_assert([]{
	auto environment = Environment();

	auto team = Team<generation>({{
		{
			.species = Species::Slugma,
			.moves = {{
				MoveName::Tackle,
			}}
		},
		{
			.species = Species::Zapdos,
			.moves = {{
				MoveName::Tackle,
			}}
		},
	}});
	team.pokemon().switch_in(environment);

	auto other = Team<generation>({{
		{
			.species = Species::Suicune,
			.moves = {{
				MoveName::Tackle,
			}}
		},
	}});
	other.pokemon().switch_in(environment);

	faint(team.pokemon());

	auto const selections = get_legal_selections(team, other, environment);
	return selections == LegalSelections({MoveName::Switch1});
}());

} // namespace
} // namespace technicalmachine
