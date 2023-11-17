// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <std_module/prelude.hpp>
#include <catch2/catch_test_macros.hpp>

import tm.clients.ps.battle_interface;
import tm.clients.ps.battle_message_result;
import tm.clients.ps.battle_parser;
import tm.clients.ps.in_message;

import tm.clients.client_battle_inputs;
import tm.clients.party;
import tm.clients.teams;

import tm.evaluate.analysis_logger;
import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.evaluate_settings;

import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.pokemon.known_pokemon;
import tm.pokemon.level;
import tm.pokemon.seen_pokemon;
import tm.pokemon.species;

import tm.stat.default_evs;

import tm.team_predictor.usage_stats;

import tm.test.usage_bytes;

import tm.ability;
import tm.gender;
import tm.generation;
import tm.item;
import tm.team;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

using namespace std::string_view_literals;

template<Generation generation>
auto get_usage_stats() -> UsageStats const & {
	[[clang::no_destroy]] static auto const result = bytes_to_usage_stats(smallest_team_bytes(generation));
	return result;
}

constexpr auto evaluate_settings = EvaluateSettings{
	.hp = 1024_bi,
	.hidden = 80_bi,
	.spikes = -150_bi,
	.stealth_rock = -200_bi,
	.toxic_spikes = -100_bi
};

template<Generation generation>
auto make_parser(KnownTeam<generation> ai, SeenTeam<generation> foe) -> ps::BattleParser {
	return ps::BattleParser(
		AnalysisLogger(AnalysisLogger::none()),
		get_usage_stats<generation>(),
		GenerationGeneric<ClientBattleInputs>(ClientBattleInputs<generation>{
			Teams<generation>{
				std::move(ai),
				std::move(foe)
			},
			Evaluate<generation>(evaluate_settings),
		}),
		Party(0_bi),
		Depth(1_bi, 0_bi)
	);
}

struct AnyResponse{};
using ps::BattleContinues;
using ps::BattleResponseMove;
using ps::BattleResponseSwitch;
using ps::BattleStarted;
using ps::BattleFinished;
using Response = tv::variant<
	BattleContinues,
	AnyResponse,
	BattleResponseMove,
	BattleResponseSwitch,
	BattleFinished
>;
struct MessageResponse {
	std::string_view message;
	Response response = BattleContinues();
};

constexpr auto as_string(BattleResponseMove const index) -> containers::string {
	return containers::concatenate<containers::string>("Use "sv, containers::to_string(index));
}
constexpr auto as_string(BattleResponseSwitch const index) -> containers::string {
	return containers::concatenate<containers::string>("Switch to "sv, containers::to_string(index));
}

constexpr auto check_values(ps::BattleParser & parser, std::span<MessageResponse const> const values) {
	for (auto const value : values) {
		INFO(value.message);
		auto const result = parser.handle_message(ps::InMessage(value.message));
		tv::visit(result, value.response, tv::overload(
			[](BattleStarted, auto) {
				FAIL_CHECK("BattleParser should never return BattleStarted");
			},
			[](bounded::bounded_integer auto const received_index, BattleContinues) {
				FAIL_CHECK("Expected no response, got " << as_string(received_index));
			},
			[](bounded::bounded_integer auto const, AnyResponse) {
			},
			[](bounded::bounded_integer auto const received_index, bounded::bounded_integer auto const index) {
				CHECK(std::string_view(as_string(received_index)) == std::string_view(as_string(index)));
			},
			[](bounded::bounded_integer auto const received, BattleFinished) {
				FAIL_CHECK("Expected BattleFinished, got " << as_string(received));
			},
			[](BattleFinished, BattleFinished) {
			},
			[](BattleFinished, auto) {
				FAIL_CHECK("Unexpected BattleFinished");
			},
			[](BattleContinues, BattleContinues) {
			},
			[](BattleContinues, auto) {
				FAIL_CHECK("Received no response when one was expected");
			}
		));
	}
}

auto make_known_one_pokemon_team() {
	constexpr auto generation = Generation::three;
	return KnownTeam<generation>({
		KnownPokemon<generation>(
			Species::Smeargle,
			"Smeargle"sv,
			Level(100_bi),
			Gender::male,
			Item::None,
			Ability::Own_Tempo,
			default_combined_stats<generation>,
			RegularMoves({
				Move(generation, MoveName::Baton_Pass)
			})
		)
	});
}

auto make_seen_trap_team() {
	constexpr auto generation = Generation::three;
	auto team = SeenTeam<generation>(1_bi);
	auto & pokemon = team.add_pokemon(SeenPokemon<generation>(
		Species::Dugtrio,
		"Dugtrio"sv,
		Level(100_bi),
		Gender::male
	));
	pokemon.set_initial_ability(Ability::Arena_Trap);
	return team;
}

TEST_CASE("BattleParser Baton Pass no other Pokemon", "[Pokemon Showdown]") {
	auto parser = make_parser(
		make_known_one_pokemon_team(),
		make_seen_trap_team()
	);

	auto const values = containers::array{
		MessageResponse("|turn|1", BattleResponseMove(1_bi)),
		MessageResponse("|"),
		MessageResponse("|t:|1"),
		MessageResponse("|move|p1a: Smeargle|Baton Pass|p1a: Smeargle"),
	};
	check_values(parser, values);
}

auto make_known_two_pokemon_baton_pass_team() {
	constexpr auto generation = Generation::three;
	return KnownTeam<generation>({
		KnownPokemon<generation>(
			Species::Smeargle,
			"Smeargle"sv,
			Level(100_bi),
			Gender::male,
			Item::None,
			Ability::Own_Tempo,
			default_combined_stats<generation>,
			RegularMoves({
				Move(generation, MoveName::Baton_Pass)
			})
		),
		KnownPokemon<generation>(
			Species::Shedinja,
			"Shedinja"sv,
			Level(100_bi),
			Gender::genderless,
			Item::None,
			Ability::Wonder_Guard,
			default_combined_stats<generation>,
			RegularMoves({
				Move(generation, MoveName::Giga_Drain)
			})
		)
	});
}

TEST_CASE("BattleParser Baton Pass one other Pokemon moves first", "[Pokemon Showdown]") {
	auto parser = make_parser(
		make_known_two_pokemon_baton_pass_team(),
		make_seen_trap_team()
	);

	auto const values = containers::array{
		MessageResponse("|turn|1", BattleResponseMove(1_bi)),
		MessageResponse("|"),
		MessageResponse("|t:|1"),
		MessageResponse("|move|p1a: Smeargle|Baton Pass|p1a: Smeargle", BattleResponseSwitch(2_bi)),
	};
	check_values(parser, values);
}

TEST_CASE("BattleParser Baton Pass one other Pokemon moves second", "[Pokemon Showdown]") {
	auto parser = make_parser(
		make_known_two_pokemon_baton_pass_team(),
		make_seen_trap_team()
	);

	auto const values = containers::array{
		MessageResponse("|turn|1", BattleResponseMove(1_bi)),
		MessageResponse("|"),
		MessageResponse("|t:|1"),
		MessageResponse("|move|p2a: Dugtrio|Snore|p1a: Smeargle"),
		MessageResponse("|move|p1a: Smeargle|Baton Pass|p1a: Smeargle", BattleResponseSwitch(2_bi)),
		MessageResponse("|"),
		MessageResponse("|t:|2"),
		MessageResponse("|switch|p1a: Shedinja|Shedinja|1/1"),
		MessageResponse("|"),
		MessageResponse("|upkeep"),
		MessageResponse("|turn|2", BattleResponseMove(1_bi)),
	};
	check_values(parser, values);
}

TEST_CASE("BattleParser replace fainted in turn", "[Pokemon Showdown]") {
	auto parser = make_parser(
		make_known_two_pokemon_baton_pass_team(),
		make_seen_trap_team()
	);

	auto const values = containers::array{
		MessageResponse("|turn|1", BattleResponseMove(1_bi)),
		MessageResponse("|"),
		MessageResponse("|t:|1"),
		MessageResponse("|move|p2a: Dugtrio|Earthquake|p1a: Smeargle"),
		MessageResponse("|-damage|p1a: Smeargle|0 fnt"),
		MessageResponse("|faint|p1a: Smeargle", BattleResponseSwitch(2_bi)),
		MessageResponse("|"),
		MessageResponse("|t:|2"),
		MessageResponse("|switch|p1a: Shedinja|Shedinja|1/1"),
		MessageResponse("|"),
		MessageResponse("|upkeep"),
		MessageResponse("|turn|2", BattleResponseMove(1_bi)),
	};
	check_values(parser, values);
}

TEST_CASE("BattleParser replace fainted end of turn", "[Pokemon Showdown]") {
	auto parser = make_parser(
		make_known_two_pokemon_baton_pass_team(),
		make_seen_trap_team()
	);

	auto const values = containers::array{
		MessageResponse("|turn|1", BattleResponseMove(1_bi)),
		MessageResponse("|"),
		MessageResponse("|t:|1"),
		MessageResponse("|move|p1a: Smeargle|Baton Pass|p1a: Smeargle", BattleResponseSwitch(2_bi)),
		MessageResponse("|"),
		MessageResponse("|t:|2"),
		MessageResponse("|switch|p1a: Shedinja|Shedinja|1/1"),
		MessageResponse("|move|p2a: Dugtrio|Toxic|p1a: Shedinja"),
		MessageResponse("|-status|p1a: Shedinja|tox"),
		MessageResponse("|"),
		MessageResponse("|-damage|p1a: Shedinja|0 fnt|[from] psn"),
		MessageResponse("|faint|p1a: Shedinja"),
		MessageResponse("|upkeep", BattleResponseSwitch(2_bi)),
		MessageResponse("|"),
		MessageResponse("|t:|3"),
		MessageResponse("|switch|p1a: Smeargle|Smeargle|251/251"),
		MessageResponse("|turn|2", BattleResponseMove(1_bi)),
	};
	check_values(parser, values);
}

auto make_seen_spikes_team() {
	constexpr auto generation = Generation::three;
	auto team = SeenTeam<generation>(1_bi);
	auto & pokemon = team.add_pokemon(SeenPokemon<generation>(
		Species::Smeargle,
		"Smeargle"sv,
		Level(100_bi),
		Gender::male
	));
	pokemon.set_initial_ability(Ability::Own_Tempo);
	return team;
}

TEST_CASE("BattleParser replace multiple fainted", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::three;
	auto parser = make_parser(
		KnownTeam<generation>({
			KnownPokemon<generation>(
				Species::Smeargle,
				"Smeargle"sv,
				Level(100_bi),
				Gender::male,
				Item::None,
				Ability::Own_Tempo,
				default_combined_stats<generation>,
				RegularMoves({
					Move(generation, MoveName::Agility)
				})
			),
			KnownPokemon<generation>(
				Species::Shedinja,
				"Shedinja"sv,
				Level(100_bi),
				Gender::genderless,
				Item::None,
				Ability::Wonder_Guard,
				default_combined_stats<generation>,
				RegularMoves({
					Move(generation, MoveName::Giga_Drain)
				})
			),
			KnownPokemon<generation>(
				Species::Chansey,
				"Chansey"sv,
				Level(100_bi),
				Gender::female,
				Item::None,
				Ability::Natural_Cure,
				default_combined_stats<generation>,
				RegularMoves({
					Move(generation, MoveName::Aromatherapy)
				})
			)
		}),
		make_seen_spikes_team()
	);

	auto const values = containers::array{
		MessageResponse("|turn|1", AnyResponse()),
		MessageResponse("|"),
		MessageResponse("|t:|1"),
		MessageResponse("|move|p1a: Smeargle|Agility|p1a: Smeargle"),
		MessageResponse("|-boost|p1a: Smeargle|spe|2"),
		MessageResponse("|move|p2a: Smeargle|Spikes|p1a: Smeargle"),
		MessageResponse("|-sidestart|p1: Technical Machine|Spikes"),
		MessageResponse("|"),
		MessageResponse("|upkeep"),
		MessageResponse("|turn|2", AnyResponse()),
		MessageResponse("|"),
		MessageResponse("|t:|2"),
		MessageResponse("|-singleturn|p2a: Smeargle|move: Focus Punch"),
		MessageResponse("|move|p1a: Smeargle|Agility|p1a: Smeargle"),
		MessageResponse("|-boost|p1a: Smeargle|spe|2"),
		MessageResponse("|move|p2a: Smeargle|Focus Punch|p1a: Smeargle"),
		MessageResponse("|-supereffective|p1a: Smeargle"),
		MessageResponse("|-damage|p1a: Smeargle|0 fnt"),
		MessageResponse("|faint|p1a: Smeargle", AnyResponse()),
		MessageResponse("|"),
		MessageResponse("|t:|3"),
		MessageResponse("|switch|p1a: Shedinja|Shedinja|1/1"),
		MessageResponse("|-damage|p1a: Shedinja|0 fnt|[from] Spikes"),
		MessageResponse("|faint|p1a: Shedinja", BattleResponseSwitch(3_bi)),
		MessageResponse("|"),
		MessageResponse("|t:|4"),
		MessageResponse("|switch|p1a: Chansey|Chansey|641/641"),
		MessageResponse("|-damage|p1a: Chansey|561/641|[from] Spikes"),
		MessageResponse("|"),
		MessageResponse("|upkeep"),
		MessageResponse("|turn|3", BattleResponseMove(1_bi)),
	};
	check_values(parser, values);
}

TEST_CASE("BattleParser switch faints from entry hazards before other moves", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::three;
	auto parser = make_parser(
		KnownTeam<generation>({
			KnownPokemon<generation>(
				Species::Smeargle,
				"Smeargle"sv,
				Level(100_bi),
				Gender::male,
				Item::None,
				Ability::Own_Tempo,
				default_combined_stats<generation>,
				RegularMoves({
					Move(generation, MoveName::Agility)
				})
			),
			KnownPokemon<generation>(
				Species::Shedinja,
				"Shedinja"sv,
				Level(100_bi),
				Gender::genderless,
				Item::None,
				Ability::Wonder_Guard,
				default_combined_stats<generation>,
				RegularMoves({
					Move(generation, MoveName::Giga_Drain)
				})
			)
		}),
		make_seen_spikes_team()
	);

	auto const values = containers::array{
		MessageResponse("|turn|1", AnyResponse()),
		MessageResponse("|"),
		MessageResponse("|t:|1"),
		MessageResponse("|move|p1a: Smeargle|Agility|p1a: Smeargle"),
		MessageResponse("|-boost|p1a: Smeargle|spe|2"),
		MessageResponse("|move|p2a: Smeargle|Spikes|p1a: Smeargle"),
		MessageResponse("|-sidestart|p1: Technical Machine|Spikes"),
		MessageResponse("|"),
		MessageResponse("|upkeep"),
		MessageResponse("|turn|2", AnyResponse()),
		MessageResponse("|"),
		MessageResponse("|t:|2"),
		MessageResponse("|switch|p1a: Shedinja|Shedinja|1/1"),
		MessageResponse("|-damage|p1a: Shedinja|0 fnt|[from] Spikes"),
		MessageResponse("|faint|p1a: Shedinja", BattleResponseSwitch(2_bi)),
		MessageResponse("|"),
		MessageResponse("|t:|3"),
		MessageResponse("|switch|p1a: Smeargle|Smeargle|251/251"),
		MessageResponse("|-damage|p1a: Smeargle|220/251|[from] Spikes"),
		MessageResponse("|"),
		MessageResponse("|upkeep"),
		MessageResponse("|turn|3", BattleResponseMove(1_bi)),
	};
	check_values(parser, values);
}

TEST_CASE("BattleParser lose", "[Pokemon Showdown]") {
	auto parser = make_parser(
		make_known_one_pokemon_team(),
		make_seen_trap_team()
	);

	auto const values = containers::array{
		MessageResponse("|turn|1", BattleResponseMove(1_bi)),
		MessageResponse("|"),
		MessageResponse("|t:|1"),
		MessageResponse("|move|p2a: Dugtrio|Earthquake|p1a: Smeargle"),
		MessageResponse("|-damage|p1a: Smeargle|0 fnt"),
		MessageResponse("|faint|p1a: Smeargle"),
		MessageResponse("|"),
		MessageResponse("|win|obi", BattleFinished()),
	};
	check_values(parser, values);
}

TEST_CASE("BattleParser generation 2 thaw", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::two;
	auto parser = make_parser(
		KnownTeam<generation>({
			KnownPokemon<generation>(
				Species::Gengar,
				"Gengar"sv,
				Level(100_bi),
				Gender::male,
				Item::None,
				Ability::Honey_Gather,
				default_combined_stats<generation>,
				RegularMoves({
					Move(generation, MoveName::Ice_Punch)
				})
			)
		}),
		[=] {
			auto team = SeenTeam<generation>(1_bi);
			team.add_pokemon(SeenPokemon<generation>(
				Species::Zapdos,
				"Zapdos"sv,
				Level(100_bi),
				Gender::genderless
			));
			return team;
		}()
	);

	auto const values = containers::array{
		MessageResponse("|turn|1", BattleResponseMove(1_bi)),
		MessageResponse("|"),
		MessageResponse("|t:|1"),
		MessageResponse("|move|p1a: Gengar|Ice Punch|p2a: Zapdos"),
		MessageResponse("|-supereffective|p2a: Zapdos"),
		MessageResponse("|-damage|p2a: Zapdos|57/100"),
		MessageResponse("|-status|p2a: Zapdos|frz"),
		MessageResponse("|cant|p2a: Zapdos|frz"),
		MessageResponse("|"),
		MessageResponse("|-curestatus|p2a: Zapdos|frz|[msg]"),
		MessageResponse("|upkeep|"),
	};
	check_values(parser, values);
}

TEST_CASE("BattleParser generation 2 explosion double faint", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::two;
	auto parser = make_parser(
		KnownTeam<generation>({
			KnownPokemon<generation>(
				Species::Jynx,
				"Jynx"sv,
				Level(100_bi),
				Gender::female,
				Item::None,
				Ability::Honey_Gather,
				default_combined_stats<generation>,
				RegularMoves({
					Move(generation, MoveName::Ice_Beam)
				})
			),
			KnownPokemon<generation>(
				Species::Pikachu,
				"Pikachu"sv,
				Level(100_bi),
				Gender::female,
				Item::None,
				Ability::Honey_Gather,
				default_combined_stats<generation>,
				RegularMoves({
					Move(generation, MoveName::Thunder)
				})
			)
		}),
		[=] {
			auto team = SeenTeam<generation>(2_bi);
			team.add_pokemon(SeenPokemon<generation>(
				Species::Gengar,
				"Gengar"sv,
				Level(100_bi),
				Gender::male
			));
			return team;
		}()
	);

	auto const values = containers::array{
		MessageResponse("|turn|1", AnyResponse()),
		MessageResponse("|"),
		MessageResponse("|t:|1"),
		MessageResponse("|move|p2a: Gengar|Explosion|p1a: Jynx"),
		MessageResponse("|-damage|p1a: Jynx|0 fnt"),
		MessageResponse("|faint|p2a: Gengar"),
		MessageResponse("|faint|p1a: Jynx", BattleResponseSwitch(2_bi)),
	};
	check_values(parser, values);
}

TEST_CASE("BattleParser shiny genderless Pokemon", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::three;
	auto parser = make_parser(
		make_known_one_pokemon_team(),
		[=] {
			auto team = SeenTeam<generation>(2_bi);
			team.add_pokemon(SeenPokemon<generation>(
				Species::Gengar,
				"Gengar"sv,
				Level(100_bi),
				Gender::male
			));
			return team;
		}()
	);

	auto const values = containers::array{
		MessageResponse("|turn|1", AnyResponse()),
		MessageResponse("|"),
		MessageResponse("|t:|1"),
		MessageResponse("|switch|p2a: Raikou|Raikou, shiny|100/100"),
	};
	check_values(parser, values);
}

TEST_CASE("BattleParser Struggle", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::three;
	auto parser = make_parser(
		KnownTeam<generation>({
			KnownPokemon<generation>(
				Species::Smeargle,
				"Smeargle"sv,
				Level(100_bi),
				Gender::female,
				Item::None,
				Ability::Own_Tempo,
				default_combined_stats<generation>,
				RegularMoves({
					Move(generation, MoveName::Sketch)
				})
			)
		}),
		[=] {
			auto team = SeenTeam<generation>(1_bi);
			team.add_pokemon(SeenPokemon<generation>(
				Species::Smeargle,
				"Smeargle"sv,
				Level(100_bi),
				Gender::male
			));
			return team;
		}()
	);

	auto const values = containers::array{
		MessageResponse("|turn|1", BattleResponseMove(1_bi)),
		MessageResponse("|"),
		MessageResponse("|t:|1"),
		MessageResponse("|move|p1a: Smeargle|Sketch||[still]"),
		MessageResponse("|-fail|p1a: Smeargle"),
		MessageResponse("|move|p2a: Smeargle|Splash|p2a: Smeargle"),
		MessageResponse("|-nothing"),
		MessageResponse("||"),
		MessageResponse("|upkeep|"),
		MessageResponse("|turn|2", BattleResponseMove(1_bi)),
		MessageResponse("|-activate|p1a: Smeargle|move: Struggle"),
		MessageResponse("||"),
		MessageResponse("|t:|1"),
		MessageResponse("|move|p1a: Smeargle|Struggle|p2a: Smeargle"),
		MessageResponse("|-damage|p2a: Smeargle|92/100"),
		MessageResponse("|-damage|p1a: Smeargle|246/251|[from] Recoil|[of] p2a: Smeargle"),
		MessageResponse("|move|p2a: Smeargle|Splash|p2a: Smeargle"),
		MessageResponse("|-nothing"),
		MessageResponse("||"),
		MessageResponse("|upkeep|"),
	};
	check_values(parser, values);
}

TEST_CASE("BattleParser full paralysis", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::two;
	auto parser = make_parser(
		KnownTeam<generation>({
			KnownPokemon<generation>(
				Species::Hypno,
				"Hypno"sv,
				Level(74_bi),
				Gender::male,
				Item::Leftovers,
				Ability::Honey_Gather,
				default_combined_stats<generation>,
				RegularMoves({Move(generation, MoveName::Curse)})
			)
		}),
		[=] {
			auto team = SeenTeam<generation>(1_bi);
			team.add_pokemon(SeenPokemon<generation>(
				Species::Blissey,
				"Blissey"sv,
				Level(68_bi),
				Gender::female
			));
			return team;
		}()
	);

	auto const values = containers::array{
		MessageResponse("|turn|1", BattleResponseMove(1_bi)),
		MessageResponse("|move|p1a: Hypno|Curse|p1a: Hypno"),
		MessageResponse("|move|p2a: Blissey|Thunder Wave|p1a: Hypno"),
		MessageResponse("|-status|p1a: Hypno|par"),
		MessageResponse("|upkeep|"),
		MessageResponse("|turn|2", BattleResponseMove(1_bi)),
		MessageResponse("|move|p2a: Blissey|Ice Beam|p1a: Hypno"),
		MessageResponse("|-damage|p1a: Hypno|124/278 par"),
		MessageResponse("|cant|p1a: Hypno|par"),
		MessageResponse("||"),
		MessageResponse("|-heal|p1a: Hypno|141/278 par|[from] item: Leftovers"),
		MessageResponse("|upkeep|"),
	};
	check_values(parser, values);
}

TEST_CASE("BattleParser Pain Split", "[Pokemon Showdown]") {
	constexpr auto generation = Generation::two;
	auto parser = make_parser(
		KnownTeam<generation>({
			KnownPokemon<generation>(
				Species::Misdreavus,
				"Misdreavus"sv,
				Level(68_bi),
				Gender::male,
				Item::None,
				Ability::Honey_Gather,
				default_combined_stats<generation>,
				RegularMoves({Move(generation, MoveName::Pain_Split)})
			)
		}),
		[=] {
			auto team = SeenTeam<generation>(1_bi);
			team.add_pokemon(SeenPokemon<generation>(
				Species::Blissey,
				"Blissey"sv,
				Level(68_bi),
				Gender::female
			));
			return team;
		}()
	);

	auto const values = containers::array{
		MessageResponse("|turn|1", BattleResponseMove(1_bi)),
		MessageResponse("|move|p2a: Blissey|Ice Beam|p1a: Misdreavus"),
		MessageResponse("|-damage|p1a: Misdreavus|85/222"),
		MessageResponse("|move|p1a: Misdreavus|Pain Split|p2a: Blissey"),
		MessageResponse("|-sethp|p2a: Blissey|46/100|[from] move: Pain Split|[silent]"),
	};
	check_values(parser, values);
}

} // namespace
} // namespace technicalmachine
