module;

#include <std_module/prelude.hpp>
#include <catch2/catch_test_macros.hpp>

export module tm.test.clients.client_battle;

import tm.clients.client_battle;
import tm.clients.make_client_battle;
import tm.clients.teams;

import tm.move.move_name;
import tm.move.move_result;
import tm.move.switch_;

import tm.pokemon.initial_pokemon;
import tm.pokemon.level;
import tm.pokemon.species;

import tm.stat.stat_style;

import tm.test.make_seen_team;
import tm.test.usage_bytes;

import tm.end_of_turn_flags;
import tm.gender;
import tm.generation;
import tm.team;
import tm.visible_hp;

import bounded;
import containers;
import std_module;
import tv;

namespace technicalmachine {
namespace {
using namespace std::string_view_literals;
using namespace bounded::literal;

template<Generation generation, std::size_t known_size>
auto make_battle(
	containers::c_array<InitialPokemon<special_style_for(generation)>, known_size> && known,
	SeenPokemonInit const seen
) {
	auto battle = make_client_battle(
		Teams<generation>{
			KnownTeam<generation>({std::move(known)}),
			make_seen_team<generation>(seen)
		}
	);
	return battle;
}

constexpr auto visible_hp(auto const min, auto const max) -> VisibleHP {
	return VisibleHP(CurrentVisibleHP(min), MaxVisibleHP(max));
}

constexpr auto damaging_move(MoveName const move, VisibleHP const remaining) -> Used {
	auto result = Used(move);
	result.damage = remaining;
	return result;
}

TEST_CASE("Accurate HP after move", "[ClientBattle]") {
	constexpr auto generation = Generation::one;
	auto battle = make_battle<generation>(
		{
			{
				.species = Species::Bulbasaur,
				.moves = {{MoveName::Tackle}}
			},
		},
		{
			.team_size = 1_bi,
			.species = Species::Bulbasaur,
		}
	);

	battle->use_move(
		true,
		damaging_move(MoveName::Tackle, visible_hp(99_bi, 100_bi)),
		false
	);
	battle->correct_hp(
		false,
		visible_hp(99_bi, 100_bi)
	);

	auto const hp = tv::visit(battle->state(), [](auto const & state) {
		return state.foe.pokemon().hp();
	});
	CHECK(hp.max() == 293_bi);
	CHECK(hp.current() == 290_bi);
}

TEST_CASE("Report end of turn after both Pokemon move", "[ClientBattle]") {
	constexpr auto generation = Generation::one;
	auto battle = make_battle<generation>(
		{
			{
				.species = Species::Magikarp,
				.moves = {{MoveName::Splash}}
			},
		},
		{
			.team_size = 1_bi,
			.species = Species::Bulbasaur,
		}
	);

	CHECK(!battle->is_end_of_turn());

	battle->use_move(
		true,
		Used(MoveName::Splash),
		false
	);

	CHECK(!battle->is_end_of_turn());

	battle->use_move(
		false,
		Used(MoveName::Splash),
		false
	);

	CHECK(battle->is_end_of_turn());

	constexpr auto end_of_turn_flags = EndOfTurnFlags(false, false, false);
	battle->end_turn(true, end_of_turn_flags, end_of_turn_flags);

	CHECK(!battle->is_end_of_turn());
}

TEST_CASE("Handle replacing two fainted Pokemon old generation", "[ClientBattle]") {
	constexpr auto generation = Generation::one;
	auto battle = make_battle<generation>(
		{
			{
				.species = Species::Bulbasaur,
				.moves = {{
					MoveName::Tackle,
				}}
			},
			{
				.species = Species::Jynx,
				.moves = {{
					MoveName::Tackle,
				}}
			},
		},
		{
			.team_size = 2_bi,
			.species = Species::Gengar,
		}
	);

	CHECK(!battle->is_end_of_turn());

	battle->use_move(
		false,
		damaging_move(MoveName::Explosion, visible_hp(0_bi, 1_bi)),
		false
	);

	CHECK(!battle->is_end_of_turn());

	battle->use_switch(true, Switch(1_bi));
	battle->foe_has(
		Species::Pikachu,
		"Pikachu"sv,
		Level(100_bi),
		Gender::genderless,
		MaxVisibleHP(100_bi)
	);
	battle->use_switch(false, Switch(1_bi));

	CHECK(battle->is_end_of_turn());

	constexpr auto end_of_turn_flags = EndOfTurnFlags(false, false, false);
	battle->end_turn(true, end_of_turn_flags, end_of_turn_flags);

	CHECK(!battle->is_end_of_turn());
}

TEST_CASE("Handle replacing two fainted Pokemon new generation", "[ClientBattle]") {
	constexpr auto generation = Generation::four;
	auto battle = make_battle<generation>(
		{
			{
				.species = Species::Bulbasaur,
				.moves = {{
					MoveName::Tackle,
				}}
			},
			{
				.species = Species::Jynx,
				.moves = {{
					MoveName::Tackle,
				}}
			},
		},
		{
			.team_size = 2_bi,
			.species = Species::Gengar,
		}
	);

	CHECK(!battle->is_end_of_turn());

	battle->use_move(
		false,
		damaging_move(MoveName::Explosion, visible_hp(0_bi, 1_bi)),
		false
	);

	CHECK(battle->is_end_of_turn());

	constexpr auto end_of_turn_flags = EndOfTurnFlags(false, false, false);
	battle->end_turn(true, end_of_turn_flags, end_of_turn_flags);

	CHECK(!battle->is_end_of_turn());

	battle->use_switch(true, Switch(1_bi));
	battle->foe_has(
		Species::Pikachu,
		"Pikachu"sv,
		Level(100_bi),
		Gender::genderless,
		MaxVisibleHP(100_bi)
	);
	battle->use_switch(false, Switch(1_bi));

	CHECK(!battle->is_end_of_turn());
}

} // namespace
} // namespace technicalmachine