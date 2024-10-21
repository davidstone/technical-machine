// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <catch2/catch_test_macros.hpp>

export module tm.test.clients.ps.battle_message_handler;

import tm.clients.ps.action_required;
import tm.clients.ps.battle_message_handler;
import tm.clients.ps.battle_response_switch;
import tm.clients.ps.event_block;
import tm.clients.ps.parsed_message;
import tm.clients.ps.start_of_turn;
import tm.clients.ps.switch_message;

import tm.clients.battle_finished;
import tm.clients.client_battle;
import tm.clients.make_client_battle;
import tm.clients.party;
import tm.clients.teams;
import tm.clients.turn_count;

import tm.move.actual_damage;
import tm.move.max_moves_per_pokemon;
import tm.move.move_name;
import tm.move.move_result;
import tm.move.switch_;

import tm.pokemon.initial_pokemon;
import tm.pokemon.level;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.species;

import tm.stat.stat_style;

import tm.status.status_name;

import tm.test.make_seen_team;

import tm.ability;
import tm.gender;
import tm.generation;
import tm.generation_generic;
import tm.item;
import tm.team;
import tm.visible_hp;
import tm.visible_state;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine {
namespace {
using namespace bounded::literal;
using namespace std::string_view_literals;

template<Generation generation, std::size_t known_size>
auto make_init(
	containers::c_array<InitialPokemon<special_style_for(generation)>, known_size> && known,
	SeenPokemonInit const seen
) {
	auto teams = [&] {
		return Teams<generation>{
			KnownTeam<generation>({std::move(known)}),
			make_seen_team<generation>(seen)
		};
	};
	return std::pair(
		make_client_battle(teams()),
		ps::BattleMessageHandler(Party(0_bi), teams())
	);
}

constexpr auto visible_hp(auto const min, auto const max) -> VisibleHP {
	return VisibleHP(CurrentVisibleHP(min), MaxVisibleHP(max));
}

auto handle_end_turn(ClientBattle & battle) -> void {
	battle.end_turn(true, {false, false, false}, {false, false, false});
}

constexpr auto damaging_move(MoveName const move, VisibleHP const remaining) -> Used {
	auto result = Used(move);
	result.damage = remaining;
	return result;
}

template<std::size_t size>
auto check_switch_options(ps::BattleMessageHandler const & handler, containers::c_array<ps::BattleResponseSwitch, size> && memory) {
	for (auto const index : containers::integer_range(size)) {
		auto const result = handler.slot_memory().reverse_lookup(memory[index]);
		CHECK(result == index);
	}
}

constexpr auto compare_state = []<Generation generation>(
	VisibleState<generation> const & actual,
	VisibleState<generation> const & expected
) -> void {
	CHECK(actual.ai == expected.ai);
	CHECK(actual.foe == expected.foe);
	CHECK(actual.environment == expected.environment);
};

auto check_state(
	GenerationGeneric<VisibleState> const & actual,
	GenerationGeneric<VisibleState> const & expected
) -> void {
	tv::visit(actual, expected, tv::overload(
		compare_state,
		[](auto const &, auto const &) {
			FAIL_CHECK("Generation mismatch");
		}
	));
}

auto check_state(
	ps::BattleMessageHandler::Result const & actual,
	GenerationGeneric<VisibleState> const & expected
) -> void {
	tv::visit(actual, tv::overload(
		[&](ps::ActionRequired const & result) {
			tv::visit(result.state, expected, tv::overload(
				compare_state,
				[](auto const &, auto const &) {
					FAIL_CHECK("Generation mismatch");
				}
			));
		},
		[](auto const &) {
			FAIL_CHECK("Incorrect handler result");
		}
	));
}

auto check_state(
	ps::BattleMessageHandler::Result const & actual,
	GenerationGeneric<VisibleState> const & expected,
	TurnCount const expected_turn_count
) -> void {
	tv::visit(actual, tv::overload(
		[&](ps::StartOfTurn const & result) {
			CHECK(result.turn_count == expected_turn_count);
			tv::visit(result.state, expected, tv::overload(
				compare_state,
				[](auto const &, auto const &) {
					FAIL_CHECK("Generation mismatch");
				}
			));
		},
		[](auto const &) {
			FAIL_CHECK("Incorrect handler result");
		}
	));
}

constexpr auto did_not_miss = false;

TEST_CASE("BattleMessageHandler constructor has correct initial state", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::one;
	auto [expected, handler] = make_init<generation>(
		{
			{.species = Species::Pikachu, .moves = {{MoveName::Tackle}}},
			{.species = Species::Eevee, .moves = {{MoveName::Tackle}}},
		},
		{
			.team_size = 1_bi,
			.species = Species::Bulbasaur,
		}
	);

	check_state(handler.state(), expected->state());
	check_switch_options(handler, {1_bi});
}

TEST_CASE("BattleMessageHandler partial turn", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::one;
	auto [expected, handler] = make_init<generation>(
		{
			{
				.species = Species::Bulbasaur,
				.moves = {{MoveName::Tackle}}
			},
		},
		{
			.team_size = 1_bi,
			.species = Species::Bulbasaur
		}
	);

	// Just verify it doesn't assert or execute undefined behavior
	handler.handle_message(ps::EventBlock({
		ps::SeparatorMessage(),
		ps::MoveMessage(Party(0_bi), MoveName::Tackle, did_not_miss),
		ps::DamageMessage(
			Party(1_bi),
			StatusName::clear,
			visible_hp(90_bi, 100_bi)
		),
	}));
}

TEST_CASE("BattleMessageHandler full turn", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::one;
	auto [expected, handler] = make_init<generation>(
		{
			{
				.species = Species::Bulbasaur,
				.moves = {{MoveName::Tackle}}
			},
		},
		{
			.team_size = 1_bi,
			.species = Species::Bulbasaur
		}
	);

	auto const result = handler.handle_message(ps::EventBlock({
		ps::SeparatorMessage(),
		ps::MoveMessage(Party(0_bi), MoveName::Tackle, did_not_miss),
		ps::DamageMessage(
			Party(1_bi),
			StatusName::clear,
			visible_hp(90_bi, 100_bi)
		),
		ps::MoveMessage(Party(1_bi), MoveName::Tackle, did_not_miss),
		ps::DamageMessage(
			Party(0_bi),
			StatusName::clear,
			visible_hp(263_bi, 293_bi)
		),
		ps::SeparatorMessage(),
		ps::TurnMessage(2_bi)
	}));

	expected->use_move(
		true,
		damaging_move(MoveName::Tackle, visible_hp(90_bi, 100_bi)),
		false
	);

	expected->use_move(
		false,
		damaging_move(MoveName::Tackle, visible_hp(263_bi, 293_bi)),
		false
	);
	expected->correct_hp(
		false,
		visible_hp(90_bi, 100_bi)
	);

	handle_end_turn(*expected);

	check_state(result, expected->state(), TurnCount(2_bi));
}

TEST_CASE("BattleMessageHandler can switch into entry hazards", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::four;
	auto [expected, handler] = make_init<generation>(
		{
			{
				.species = Species::Smeargle,
				.moves = {{MoveName::Splash}}
			},
			{
				.species = Species::Bulbasaur,
				.moves = {{MoveName::Tackle}}
			},
		},
		{
			.team_size = 1_bi,
			.species = Species::Smeargle
		}
	);

	{
		auto const result = handler.handle_message(ps::EventBlock({
			ps::SeparatorMessage(),
			ps::MoveMessage(Party(0_bi), MoveName::Splash, did_not_miss),
			ps::MoveMessage(Party(1_bi), MoveName::Spikes, did_not_miss),
			ps::SeparatorMessage(),
			ps::TurnMessage(2_bi),
		}));

		expected->use_move(
			true,
			Used(MoveName::Splash),
			false
		);

		expected->use_move(
			false,
			Used(MoveName::Spikes),
			false
		);

		handle_end_turn(*expected);

		check_state(result, expected->state(), TurnCount(2_bi));
	}

	{
		auto const result = handler.handle_message(ps::EventBlock({
			ps::SeparatorMessage(),
			ps::SwitchMessage(
				Party(0_bi),
				Species::Bulbasaur,
				"Bulbasaur"sv,
				Level(100_bi),
				Gender::genderless,
				visible_hp(231_bi, 231_bi),
				StatusName::clear
			),
			ps::HPMessage(Party(0_bi), StatusName::clear, visible_hp(203_bi, 231_bi)),
			ps::MoveMessage(Party(1_bi), MoveName::Spikes, did_not_miss),
			ps::SeparatorMessage(),
			ps::TurnMessage(3_bi),
		}));

		expected->use_switch(true, Switch(1_bi));

		expected->use_move(
			false,
			Used(MoveName::Spikes),
			false
		);

		handle_end_turn(*expected);

		check_state(result, expected->state(), TurnCount(3_bi));
	}

}

TEST_CASE("BattleMessageHandler can Baton Pass", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::two;
	auto [expected, handler] = make_init<generation>(
		{
			{
				.species = Species::Smeargle,
				.moves = {{MoveName::Baton_Pass}}
			},
			{
				.species = Species::Eevee,
				.moves = {{MoveName::Tackle}}
			},
		},
		{
			.team_size = 1_bi,
			.species = Species::Bulbasaur
		}
	);

	auto const result = handler.handle_message(ps::EventBlock({
		ps::SeparatorMessage(),
		ps::MoveMessage(Party(0_bi), MoveName::Baton_Pass, did_not_miss),
	}));

	expected->use_move(
		true,
		Used(MoveName::Baton_Pass),
		false
	);

	check_state(result, expected->state());
	check_switch_options(handler, {1_bi, 2_bi});
}

TEST_CASE("BattleMessageHandler can Baton Pass with no other Pokemon", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::three;
	auto [expected, handler] = make_init<generation>(
		{
			{
				.species = Species::Smeargle,
				.moves = {{MoveName::Baton_Pass}}
			},
		},
		{
			.team_size = 1_bi,
			.species = Species::Magikarp
		}
	);

	auto const result = handler.handle_message(ps::EventBlock({
		ps::SeparatorMessage(),
		ps::MoveMessage(Party(0_bi), MoveName::Baton_Pass, did_not_miss),
		ps::MoveMessage(Party(1_bi), MoveName::Splash, did_not_miss),
		ps::SeparatorMessage(),
		ps::TurnMessage(2_bi)
	}));

	expected->use_move(
		true,
		Used(MoveName::Baton_Pass),
		false
	);
	expected->use_move(
		false,
		Used(MoveName::Splash),
		false
	);

	handle_end_turn(*expected);

	check_state(result, expected->state(), TurnCount(2_bi));
}

TEST_CASE("BattleMessageHandler can replace fainted from middle of turn", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::one;
	auto [expected, handler] = make_init<generation>(
		{
			{
				.species = Species::Pikachu,
				.moves = {{MoveName::Thunderbolt}}
			},
			{
				.species = Species::Tauros,
				.moves = {{MoveName::Tackle}}
			},
		},
		{
			.team_size = 1_bi,
			.species = Species::Golem
		}
	);

	{
		auto const result = handler.handle_message(ps::EventBlock({
			ps::SeparatorMessage(),
			ps::MoveMessage(Party(0_bi), MoveName::Thunderbolt, did_not_miss),
			ps::EffectivenessMessage(Party(1_bi), ps::EffectivenessMessage::immune),
			ps::MoveMessage(Party(1_bi), MoveName::Earthquake, did_not_miss),
			ps::EffectivenessMessage(Party(0_bi), ps::EffectivenessMessage::super),
			ps::DamageMessage(
				Party(0_bi),
				StatusName::clear,
				visible_hp(0_bi, 100_bi)
			)
		}));

		expected->use_move(
			true,
			Used(MoveName::Thunderbolt),
			false
		);

		expected->use_move(
			false,
			damaging_move(MoveName::Earthquake, visible_hp(0_bi, 293_bi)),
			false
		);

		handle_end_turn(*expected);

		check_state(result, expected->state());
	}

	{
		auto const result = handler.handle_message(ps::EventBlock({
			ps::SeparatorMessage(),
			ps::SwitchMessage(
				Party(0_bi),
				Species::Tauros,
				"Tauros"sv,
				Level(100_bi),
				Gender::genderless,
				visible_hp(353_bi, 353_bi),
				StatusName::clear
			),
			ps::TurnMessage(2_bi),
		}));

		expected->use_switch(true, Switch(1_bi));

		check_state(result, expected->state(), TurnCount(2_bi));
	}
}

TEST_CASE("BattleMessageHandler can replace fainted from end of turn", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::three;
	auto [expected, handler] = make_init<generation>(
		{
			{
				.species = Species::Shedinja,
				.moves = {{MoveName::Mind_Reader}}
			},
			{
				.species = Species::Tauros,
				.moves = {{MoveName::Tackle}}
			},
		},
		{
			.team_size = 1_bi,
			.species = Species::Dugtrio
		}
	);

	{
		auto const result = handler.handle_message(ps::EventBlock({
			ps::SeparatorMessage(),
			ps::MoveMessage(Party(1_bi), MoveName::Toxic, did_not_miss),
			ps::MoveStatus(Party(0_bi), StatusName::toxic),
			ps::MoveMessage(Party(0_bi), MoveName::Mind_Reader, did_not_miss),
			ps::SeparatorMessage(),
			ps::HPMessage(Party(0_bi), StatusName::clear, visible_hp(0_bi, 100_bi)),
		}));

		expected->use_move(
			false,
			Used(MoveName::Toxic),
			false
		);

		expected->use_move(
			true,
			Used(MoveName::Mind_Reader),
			false
		);

		handle_end_turn(*expected);

		check_state(result, expected->state());
	}

	{
		auto const result = handler.handle_message(ps::EventBlock({
			ps::SeparatorMessage(),
			ps::SwitchMessage(
				Party(0_bi),
				Species::Tauros,
				"Tauros"sv,
				Level(100_bi),
				Gender::genderless,
				visible_hp(291_bi, 291_bi),
				StatusName::clear
			),
			ps::TurnMessage(2_bi),
		}));
		expected->use_switch(true, Switch(1_bi));

		check_state(result, expected->state(), TurnCount(2_bi));
	}
}

// TODO: Same test for generation 3
TEST_CASE("BattleMessageHandler can replace multiple Pokemon", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::four;
	auto [expected, handler] = make_init<generation>(
		{
			{
				.species = Species::Smeargle,
				.moves = {{MoveName::Splash}}
			},
			{
				.species = Species::Shedinja,
				.moves = {{MoveName::Tackle}}
			},
			{
				.species = Species::Bulbasaur,
				.moves = {{MoveName::Tackle}}
			},
		},
		{
			.team_size = 1_bi,
			.species = Species::Smeargle
		}
	);

	{
		auto const result = handler.handle_message(ps::EventBlock({
			ps::SeparatorMessage(),
			ps::MoveMessage(Party(0_bi), MoveName::Splash, did_not_miss),
			ps::MoveMessage(Party(1_bi), MoveName::Spikes, did_not_miss),
			ps::SeparatorMessage(),
			ps::TurnMessage(2_bi),
		}));

		expected->use_move(
			true,
			Used(MoveName::Splash),
			false
		);

		expected->use_move(
			false,
			Used(MoveName::Spikes),
			false
		);

		handle_end_turn(*expected);

		check_state(result, expected->state(), TurnCount(2_bi));
	}

	{
		auto const result = handler.handle_message(ps::EventBlock({
			ps::SeparatorMessage(),
			ps::MoveMessage(Party(0_bi), MoveName::Splash, did_not_miss),
			ps::MoveMessage(Party(1_bi), MoveName::Focus_Punch, did_not_miss),
			ps::EffectivenessMessage(Party(0_bi), ps::EffectivenessMessage::super),
			ps::DamageMessage(
				Party(0_bi),
				StatusName::clear,
				visible_hp(0_bi, 100_bi)
			),
			ps::SeparatorMessage(),
		}));

		expected->use_move(
			true,
			Used(MoveName::Splash),
			false
		);

		expected->use_move(
			false,
			damaging_move(MoveName::Focus_Punch, visible_hp(0_bi, 251_bi)),
			false
		);

		handle_end_turn(*expected);

		check_state(result, expected->state());
	}

	{
		auto const result = handler.handle_message(ps::EventBlock({
			ps::SeparatorMessage(),
			ps::SwitchMessage(
				Party(0_bi),
				Species::Shedinja,
				"Shedinja"sv,
				Level(100_bi),
				Gender::genderless,
				visible_hp(1_bi, 1_bi),
				StatusName::clear
			),
			ps::HPMessage(Party(0_bi), StatusName::clear, visible_hp(0_bi, 1_bi)),
		}));

		expected->use_switch(true, Switch(1_bi));

		check_state(result, expected->state());
	}

	{
		auto const result = handler.handle_message(ps::EventBlock({
			ps::SeparatorMessage(),
			ps::SwitchMessage(
				Party(0_bi),
				Species::Bulbasaur,
				"Bulbasaur"sv,
				Level(100_bi),
				Gender::genderless,
				visible_hp(231_bi, 231_bi),
				StatusName::clear
			),
			ps::HPMessage(Party(0_bi), StatusName::clear, visible_hp(203_bi, 231_bi)),
			ps::TurnMessage(3_bi),
		}));

		expected->use_switch(true, Switch(1_bi));

		check_state(result, expected->state(), TurnCount(3_bi));
	}
}

TEST_CASE("BattleMessageHandler lose", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::four;
	auto [expected, handler] = make_init<generation>(
		{
			{
				.species = Species::Smeargle,
				.moves = {{MoveName::Tackle}}
			},
		},
		{
			.team_size = 1_bi,
			.species = Species::Dugtrio
		}
	);

	auto const result = handler.handle_message(ps::EventBlock({
		ps::SeparatorMessage(),
		ps::MoveMessage(Party(1_bi), MoveName::Earthquake, did_not_miss),
		ps::DamageMessage(
			Party(0_bi),
			StatusName::clear,
			visible_hp(0_bi, 251_bi)
		),
		ps::SeparatorMessage(),
		ps::BattleFinishedMessage(),
	}));
	CHECK(result == BattleFinished());
}

TEST_CASE("BattleMessageHandler generation 1 explosion single faint", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::one;

	auto [expected, handler] = make_init<generation>(
		{
			{
				.species = Species::Tauros,
				.moves = {{MoveName::Tackle}}
			},
		},
		{
			.team_size = 2_bi,
			.species = Species::Electrode
		}
	);

	{
		auto const result = handler.handle_message(ps::EventBlock({
			ps::SeparatorMessage(),
			ps::MoveMessage(Party(1_bi), MoveName::Self_Destruct, did_not_miss),
			ps::DamageMessage(
				Party(0_bi),
				StatusName::clear,
				visible_hp(201_bi, 353_bi)
			),
		}));

		expected->use_move(
			false,
			damaging_move(MoveName::Self_Destruct, visible_hp(201_bi, 353_bi)),
			false
		);

		handle_end_turn(*expected);

		check_state(result, expected->state());
	}

	{
		auto const result = handler.handle_message(ps::EventBlock({
			ps::SeparatorMessage(),
			ps::SwitchMessage(
				Party(1_bi),
				Species::Pikachu,
				"Pikachu"sv,
				Level(100_bi),
				Gender::genderless,
				visible_hp(100_bi, 100_bi),
				StatusName::clear
			),
			ps::TurnMessage(2_bi),
		}));

		expected->foe_has(Species::Pikachu, "Pikachu"sv, Level(100_bi), Gender::genderless, MaxVisibleHP(100_bi));
		expected->use_switch(false, Switch(1_bi));

		check_state(result, expected->state(), TurnCount(2_bi));
	}
}

TEST_CASE("BattleMessageHandler generation 1 explosion double faint", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::one;

	auto [expected, handler] = make_init<generation>(
		{
			{
				.species = Species::Bulbasaur,
				.moves = {{MoveName::Tackle}}
			},
			{
				.species = Species::Jynx,
				.moves = {{MoveName::Tackle}}
			},
		},
		{
			.team_size = 2_bi,
			.species = Species::Gengar
		}
	);

	{
		auto const result = handler.handle_message(ps::EventBlock({
			ps::SeparatorMessage(),
			ps::MoveMessage(Party(1_bi), MoveName::Explosion, did_not_miss),
			ps::DamageMessage(
				Party(0_bi),
				StatusName::clear,
				visible_hp(0_bi, 273_bi)
			),
		}));

		expected->use_move(
			false,
			damaging_move(MoveName::Explosion, visible_hp(0_bi, 1_bi)),
			false
		);

		handle_end_turn(*expected);

		check_state(result, expected->state());
	}

	{
		auto const result = handler.handle_message(ps::EventBlock({
			ps::SeparatorMessage(),
			ps::SwitchMessage(
				Party(0_bi),
				Species::Jynx,
				"Jynx"sv,
				Level(100_bi),
				Gender::genderless,
				visible_hp(333_bi, 333_bi),
				StatusName::clear
			),
			ps::SwitchMessage(
				Party(1_bi),
				Species::Pikachu,
				"Pikachu"sv,
				Level(100_bi),
				Gender::genderless,
				visible_hp(100_bi, 100_bi),
				StatusName::clear
			),
			ps::TurnMessage(2_bi),
		}));

		expected->use_switch(true, Switch(1_bi));
		expected->foe_has(Species::Pikachu, "Pikachu"sv, Level(100_bi), Gender::genderless, MaxVisibleHP(100_bi));
		expected->use_switch(false, Switch(1_bi));

		check_state(result, expected->state(), TurnCount(2_bi));
	}
}

TEST_CASE("BattleMessageHandler Pain Split", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::two;

	auto [expected, handler] = make_init<generation>(
		{
			{
				.species = Species::Misdreavus,
				.level = Level(68_bi),
				.moves = {{MoveName::Pain_Split}}
			},
		},
		{
			.team_size = 1_bi,
			.species = Species::Blissey,
			.level = Level(68_bi),
		}
	);

	auto const result = handler.handle_message(ps::EventBlock({
		ps::SeparatorMessage(),
		ps::MoveMessage(Party(1_bi), MoveName::Ice_Beam, did_not_miss),
		ps::DamageMessage(
			Party(0_bi),
			StatusName::clear,
			visible_hp(176_bi, 222_bi)
		),
		ps::MoveMessage(Party(0_bi), MoveName::Pain_Split, did_not_miss),
		ps::HPMessage(Party(1_bi), StatusName::clear, visible_hp(69_bi, 100_bi)),
		ps::HPMessage(Party(0_bi), StatusName::clear, visible_hp(222_bi, 222_bi)),
		ps::SeparatorMessage(),
		ps::TurnMessage(2_bi),
	}));

	expected->use_move(
		false,
		damaging_move(MoveName::Ice_Beam, visible_hp(176_bi, 222_bi)),
		false
	);
	expected->use_move(
		true,
		Used(MoveName::Pain_Split),
		false
	);
	expected->correct_hp(
		false,
		visible_hp(69_bi, 100_bi)
	);

	handle_end_turn(*expected);

	check_state(result, expected->state(), TurnCount(2_bi));
}

TEST_CASE("BattleMessageHandler full paralysis", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::two;

	auto [expected, handler] = make_init<generation>(
		{
			{
				.species = Species::Magikarp,
				.moves = {{MoveName::Splash}}
			},
		},
		{
			.team_size = 1_bi,
			.species = Species::Blissey
		}
	);

	{
		auto const result = handler.handle_message(ps::EventBlock({
			ps::SeparatorMessage(),
			ps::MoveMessage(Party(0_bi), MoveName::Splash, did_not_miss),
			ps::MoveMessage(Party(1_bi), MoveName::Thunder_Wave, did_not_miss),
			ps::MoveStatus(Party(0_bi), StatusName::paralysis),
			ps::SeparatorMessage(),
			ps::TurnMessage(2_bi),
		}));

		expected->use_move(
			true,
			Used(MoveName::Splash),
			false
		);
		auto thunder_wave = Used(MoveName::Thunder_Wave);
		thunder_wave.status = StatusName::paralysis;
		expected->use_move(
			false,
			thunder_wave,
			false
		);

		handle_end_turn(*expected);
	
		check_state(result, expected->state(), TurnCount(2_bi));
	}
	{
		auto const result = handler.handle_message(ps::EventBlock({
			ps::SeparatorMessage(),
			ps::MoveMessage(Party(1_bi), MoveName::Thunder_Wave, did_not_miss),
			ps::FullyParalyzedMessage(Party(0_bi)),
			ps::SeparatorMessage(),
			ps::TurnMessage(3_bi),
		}));

		expected->use_move(
			false,
			Used(MoveName::Thunder_Wave),
			false
		);
		expected->use_move(
			true,
			FullyParalyzed(),
			false
		);

		handle_end_turn(*expected);

		check_state(result, expected->state(), TurnCount(3_bi));
	}
}

TEST_CASE("BattleMessageHandler random freeze", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::one;

	auto [expected, handler] = make_init<generation>(
		{
			{
				.species = Species::Jynx,
				.moves = {{MoveName::Blizzard}}
			},
		},
		{
			.team_size = 1_bi,
			.species = Species::Starmie
		}
	);

	auto const result = handler.handle_message(ps::EventBlock({
		ps::SeparatorMessage(),
		ps::MoveMessage(Party(1_bi), MoveName::Hydro_Pump, did_not_miss),
		ps::DamageMessage(
			Party(0_bi),
			StatusName::clear,
			visible_hp(188_bi, 333_bi)
		),
		ps::MoveMessage(Party(0_bi), MoveName::Blizzard, did_not_miss),
		ps::EffectivenessMessage(Party(1_bi), ps::EffectivenessMessage::not_very),
		ps::DamageMessage(
			Party(1_bi),
			StatusName::clear,
			visible_hp(77_bi, 100_bi)
		),
		ps::MoveStatus(Party(1_bi), StatusName::freeze),
		ps::SeparatorMessage(),
		ps::TurnMessage(2_bi),
	}));

	expected->use_move(
		false,
		damaging_move(MoveName::Hydro_Pump, visible_hp(188_bi, 333_bi)),
		false
	);
	{
		auto move = Used(MoveName::Blizzard);
		move.damage = visible_hp(77_bi, 100_bi);
		move.status = StatusName::freeze;
		expected->use_move(
			true,
			move,
			false
		);
		expected->correct_hp(
			false,
			visible_hp(77_bi, 100_bi)
		);
	}

	expected->end_turn(
		true,
		{false, false, true},
		{false, false, false}
	);

	check_state(result, expected->state(), TurnCount(2_bi));
}

TEST_CASE("BattleMessageHandler generation 2 thaw", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::two;

	auto [expected, handler] = make_init<generation>(
		{
			{
				.species = Species::Blissey,
				.moves = {{MoveName::Ice_Beam}}
			},
		},
		{
			.team_size = 1_bi,
			.species = Species::Blissey
		}
	);

	auto const result = handler.handle_message(ps::EventBlock({
		ps::SeparatorMessage(),
		ps::MoveMessage(Party(1_bi), MoveName::Ice_Beam, did_not_miss),
		ps::DamageMessage(
			Party(0_bi),
			StatusName::clear,
			visible_hp(662_bi, 713_bi)
		),
		ps::MoveStatus(Party(0_bi), StatusName::freeze),
		ps::FrozenSolidMessage(Party(0_bi)),
		ps::SeparatorMessage(),
		ps::StatusClearMessage(Party(0_bi), StatusName::freeze),
		ps::TurnMessage(2_bi),
	}));

	auto move = Used(MoveName::Ice_Beam);
	move.damage = visible_hp(662_bi, 713_bi);
	move.status = StatusName::freeze;
	expected->use_move(
		false,
		move,
		false
	);
	expected->use_move(
		true,
		FrozenSolid(),
		false
	);

	expected->end_turn(
		true,
		{false, false, true},
		{false, false, false}
	);

	check_state(result, expected->state(), TurnCount(2_bi));
}

TEST_CASE("BattleMessageHandler Struggle", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::three;
	auto [expected, handler] = make_init<generation>(
		{
			{
				.species = Species::Smeargle,
				.moves = {{MoveName::Sketch}}
			},
		},
		{
			.team_size = 1_bi,
			.species = Species::Smeargle
		}
	);

	{
		auto const result = handler.handle_message(ps::EventBlock({
			ps::SeparatorMessage(),
			ps::MoveMessage(Party(0_bi), MoveName::Sketch, did_not_miss),
			ps::MoveMessage(Party(1_bi), MoveName::Splash, did_not_miss),
			ps::SeparatorMessage(),
			ps::TurnMessage(2_bi),
		}));

		expected->use_move(
			true,
			Used(MoveName::Sketch),
			false
		);
		expected->use_move(
			false,
			Used(MoveName::Splash),
			false
		);
		handle_end_turn(*expected);

		check_state(result, expected->state(), TurnCount(2_bi));
	}

	{
		auto const result = handler.handle_message(ps::EventBlock({
			ps::SeparatorMessage(),
			ps::MoveMessage(Party(0_bi), MoveName::Struggle, did_not_miss),
			ps::DamageMessage(Party(1_bi), StatusName::clear, visible_hp(92_bi, 100_bi)),
			ps::RecoilMessage(Party(0_bi), StatusName::clear, visible_hp(246_bi, 251_bi)),
			ps::MoveMessage(Party(1_bi), MoveName::Splash, did_not_miss),
			ps::SeparatorMessage(),
			ps::TurnMessage(3_bi),
		}));

		expected->use_move(
			true,
			damaging_move(MoveName::Struggle, visible_hp(92_bi, 100_bi)),
			false
		);
		expected->correct_hp(
			false,
			visible_hp(92_bi, 100_bi)
		);
		expected->use_move(
			false,
			Used(MoveName::Splash),
			false
		);

		handle_end_turn(*expected);

		check_state(result, expected->state(), TurnCount(3_bi));
	}
}

TEST_CASE("BattleMessageHandler hit self", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::one;
	auto [expected, handler] = make_init<generation>(
		{
			{
				.species = Species::Exeggutor,
				.moves = {{MoveName::Psychic}}
			},
		},
		{
			.team_size = 1_bi,
			.species = Species::Gengar
		}
	);

	auto const result = handler.handle_message(ps::EventBlock({
		ps::SeparatorMessage(),
		ps::MoveMessage(Party(1_bi), MoveName::Confuse_Ray, did_not_miss),
		ps::StartConfusionMessage(Party(0_bi)),
		ps::HitSelfMessage(Party(0_bi), StatusName::clear, visible_hp(355_bi, 393_bi)),
		ps::SeparatorMessage(),
		ps::TurnMessage(2_bi),
	}));

	expected->use_move(
		false,
		Used(MoveName::Confuse_Ray),
		false
	);
	expected->hit_self_in_confusion(
		true,
		visible_hp(355_bi, 393_bi)
	);
	handle_end_turn(*expected);

	check_state(result, expected->state(), TurnCount(2_bi));
}

#if 0

// TODO: Figure out how to handle this case
TEST_CASE("BattleMessageHandler switch faints from entry hazards before other moves in Generation 3", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::three;
	auto [expected, handler] = make_init<generation>(
		{
			{
				.species = Species::Smeargle,
				.moves = {{MoveName::Splash}}
			},
			{
				.species = Species::Shedinja,
				.moves = {{MoveName::Tackle}}
			},
		},
		{
			.team_size = 1_bi,
			.species = Species::Smeargle
		}
	);

	{
		auto const result = handler.handle_message(ps::EventBlock({
			ps::SeparatorMessage(),
			ps::MoveMessage(Party(0_bi), MoveName::Splash, did_not_miss),
			ps::MoveMessage(Party(1_bi), MoveName::Spikes, did_not_miss),
			ps::SeparatorMessage(),
			ps::TurnMessage(2_bi),
		}));

		expected->use_move(
			true,
			Used(MoveName::Splash),
			false
		);
		expected->use_move(
			false,
			Used(MoveName::Spikes),
			false
		);

		handle_end_turn(*expected);

		check_state(result, expected->state(), TurnCount(2_bi));
	}
	{
		auto const result = handler.handle_message(ps::EventBlock({
			ps::SeparatorMessage(),
			ps::SwitchMessage(
				Party(0_bi),
				Species::Shedinja,
				"Shedinja"sv,
				Level(100_bi),
				Gender::genderless,
				visible_hp(1_bi, 1_bi),
				StatusName::clear
			),
			ps::HPMessage(Party(0_bi), StatusName::clear, visible_hp(0_bi, 1_bi)),
		}));

		expected->use_switch(true, Switch(1_bi));

		check_state(result, expected->state());
	}

	{
		auto const result = handler.handle_message(ps::EventBlock({
			ps::SeparatorMessage(),
			ps::SwitchMessage(
				Party(0_bi),
				Species::Smeargle,
				"Smeargle"sv,
				Level(100_bi),
				Gender::genderless,
				visible_hp(251_bi, 251_bi),
				StatusName::clear
			),
			ps::HPMessage(Party(0_bi), StatusName::clear, visible_hp(220_bi, 251_bi)),
			ps::SeparatorMessage(),
			ps::TurnMessage(3_bi),
		}));

		expected->use_switch(true, Switch(0_bi));

		handle_end_turn(*expected);

		check_state(result, expected->state(), TurnCount(3_bi));
	}
}

#endif

} // namespace
} // namespace technicalmachine
