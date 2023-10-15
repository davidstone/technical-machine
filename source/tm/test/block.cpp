// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.test.block;

import tm.block;

import tm.move.max_moves_per_pokemon;
import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.pokemon.faint;
import tm.pokemon.level;
import tm.pokemon.pokemon;
import tm.pokemon.species;

import tm.stat.default_evs;

import tm.string_conversions.move_name;

import tm.ability;
import tm.environment;
import tm.gender;
import tm.generation;
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
constexpr auto moves(auto... move_names) {
	return RegularMoves({Move(generation, move_names, 0_bi)...});
}

// All moves are legal moves
static_assert([]{
	auto user = Team<generation>({
		Pokemon<generation>(
			Species::Jolteon,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Volt_Absorb,
			default_combined_stats<generation>,
			moves(MoveName::Thunderbolt, MoveName::Charm, MoveName::Thunder, MoveName::Shadow_Ball)
		)
	});
	auto environment = Environment();
	user.pokemon().switch_in(environment);

	auto other = Team<generation>({
		Pokemon<generation>(
			Species::Gyarados,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Intimidate,
			default_combined_stats<generation>,
			moves(MoveName::Dragon_Dance, MoveName::Waterfall, MoveName::Stone_Edge, MoveName::Taunt)
		)
	});
	other.pokemon().switch_in(environment);

	user.reset_start_of_turn();

	auto const selections = legal_selections(user, other, environment);
	return selections == LegalSelections({MoveName::Thunderbolt, MoveName::Charm, MoveName::Thunder, MoveName::Shadow_Ball});
}());

constexpr auto remove_all_pp(Move & move) {
	auto remaining_pp = move.pp().remaining();
	if (remaining_pp) {
		move.reduce_pp(*remaining_pp);
	}
};

// Two moves with one out of pp
static_assert([]{
	auto environment = Environment();

	auto user_moves = moves(MoveName::Thunder, MoveName::Thunderbolt);
	remove_all_pp(containers::front(user_moves));
	auto user = Team<generation>({
		Pokemon<generation>(
			Species::Pikachu,
			Level(100_bi),
			Gender::female,
			Item::Leftovers,
			Ability::Intimidate,
			default_combined_stats<generation>,
			user_moves
		)
	});
	user.pokemon().switch_in(environment);

	auto other = Team<generation>({
		Pokemon<generation>(
			Species::Pikachu,
			Level(100_bi),
			Gender::female,
			Item::Leftovers,
			Ability::Intimidate,
			default_combined_stats<generation>,
			moves(MoveName::Thunder, MoveName::Thunderbolt)
		)
	});
	other.pokemon().switch_in(environment);
	
	user.reset_start_of_turn();

	auto const selections = legal_selections(user, other, environment);
	return selections == LegalSelections({MoveName::Thunderbolt});
}());

// Two moves with both out of pp
static_assert([]{
	auto environment = Environment();

	auto user_moves = moves(MoveName::Thunder, MoveName::Thunderbolt);
	for (auto & move : user_moves) {
		remove_all_pp(move);
	}
	auto user = Team<generation>({
		Pokemon<generation>(
			Species::Pikachu,
			Level(100_bi),
			Gender::female,
			Item::Leftovers,
			Ability::Intimidate,
			default_combined_stats<generation>,
			user_moves
		)
	});
	user.pokemon().switch_in(environment);

	auto other = Team<generation>({
		Pokemon<generation>(
			Species::Pikachu,
			Level(100_bi),
			Gender::female,
			Item::Leftovers,
			Ability::Intimidate,
			default_combined_stats<generation>,
			moves(MoveName::Thunder, MoveName::Thunderbolt)
		)
	});
	other.pokemon().switch_in(environment);
	
	user.reset_start_of_turn();

	auto const selections = legal_selections(user, other, environment);
	return selections == LegalSelections({MoveName::Struggle});
}());

// Replace fainted
static_assert([]{
	auto environment = Environment();

	auto team = Team<generation>({
		Pokemon<generation>(
			Species::Slugma,
			Level(100_bi),
			Gender::male,
			Item::Choice_Specs,
			Ability::Magma_Armor,
			default_combined_stats<generation>,
			moves(MoveName::Flamethrower)
		),
		Pokemon<generation>(
			Species::Zapdos,
			Level(100_bi),
			Gender::genderless,
			Item::Choice_Specs,
			Ability::Pressure,
			default_combined_stats<generation>,
			moves(MoveName::Thunderbolt)
		)
	});
	team.pokemon().switch_in(environment);
	team.reset_start_of_turn();

	auto other = Team<generation>({
		Pokemon<generation>(
			Species::Suicune,
			Level(100_bi),
			Gender::genderless,
			Item::Leftovers,
			Ability::Pressure,
			default_combined_stats<generation>,
			moves(MoveName::Surf)
		)
	});
	other.pokemon().switch_in(environment);

	other.reset_start_of_turn();

	faint(team.pokemon());

	auto const selections = legal_selections(team, other, environment);
	return selections == LegalSelections({MoveName::Switch1});
}());

} // namespace
} // namespace technicalmachine
