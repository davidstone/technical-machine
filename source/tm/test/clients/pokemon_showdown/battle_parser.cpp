// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <compare>
#include <catch2/catch_test_macros.hpp>

import tm.clients.ps.battle_interface;
import tm.clients.ps.battle_parser;
import tm.clients.ps.inmessage;

import tm.clients.make_battle_manager_inputs;
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

import tm.stat.combined_stats;

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

constexpr auto write_team = tv::none;

constexpr auto battle_id = "battle-id"sv;

constexpr auto evaluate_settings = EvaluateSettings{
	.hp = 1024_bi,
	.hidden = 80_bi,
	.spikes = -150_bi,
	.stealth_rock = -200_bi,
	.toxic_spikes = -100_bi
};

template<Generation generation>
auto make_parser(KnownTeam<generation> ai, SeenTeam<generation> foe) -> ps::BattleParser {
	auto random_device = std::random_device();
	return ps::BattleParser(
		AnalysisLogger(AnalysisLogger::none()),
		write_team,
		battle_id,
		"Technical Machine",
		get_usage_stats<generation>(),
		GenerationGeneric<BattleManagerInputs>(BattleManagerInputs<generation>{
			Teams<generation>{
				std::move(ai),
				std::move(foe)
			},
			Evaluate<generation>(evaluate_settings),
		}),
		Party(0_bi),
		Depth(1_bi, 0_bi),
		std::mt19937(random_device())
	);
}

struct NoResponse{};
struct AnyResponse{};
using Response = tv::variant<NoResponse, AnyResponse, containers::string>;
struct MessageResponse {
	std::string_view message;
	Response response;
};

constexpr auto make_message_response(std::string_view const message, std::string_view const response) {
	return MessageResponse{
		message,
		Response(containers::concatenate<containers::string>(battle_id, "|"sv, response))
	};
}
constexpr auto make_message_response(std::string_view const message, AnyResponse) {
	return MessageResponse{
		message,
		Response(AnyResponse())
	};
}
constexpr auto make_message_response(std::string_view const message) {
	return MessageResponse{
		message,
		Response(NoResponse())
	};
}

constexpr auto check_values(ps::BattleParser & parser, auto const & values) {
	for (auto const value : values) {
		CHECK(parser.completed() == ps::BattleInterface::Complete::none);
		INFO(value.message);
		auto const response = parser.handle_message(ps::InMessage(parser.id(), value.message));
		if (response) {
			UNSCOPED_INFO("*response: " << *response);
		}
		tv::visit(value.response, tv::overload(
			[&](NoResponse) { CHECK(!response); },
			[&](AnyResponse) { CHECK(static_cast<bool>(response)); },
			[&](std::string_view const str) {
				REQUIRE(static_cast<bool>(response));
				CHECK(std::string_view(*response) == str);
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

TEST_CASE("BattleParser has right ID", "[Pokemon Showdown]") {
	auto parser = make_parser(
		make_known_one_pokemon_team(),
		make_seen_trap_team()
	);

	CHECK(parser.id() == battle_id);
}

TEST_CASE("BattleParser Baton Pass no other Pokemon", "[Pokemon Showdown]") {
	auto parser = make_parser(
		make_known_one_pokemon_team(),
		make_seen_trap_team()
	);

	auto const values = containers::array{
		make_message_response("|turn|1", "/choose move 1"),
		make_message_response("|"),
		make_message_response("|t:|1"),
		make_message_response("|move|p1a: Smeargle|Baton Pass|p1a: Smeargle"),
	};
	check_values(parser, values);
	CHECK(parser.completed() == ps::BattleInterface::Complete::none);
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
		make_message_response("|turn|1", "/choose move 1"),
		make_message_response("|"),
		make_message_response("|t:|1"),
		make_message_response("|move|p1a: Smeargle|Baton Pass|p1a: Smeargle", "/choose switch 2"),
	};
	check_values(parser, values);
	CHECK(parser.completed() == ps::BattleInterface::Complete::none);
}

TEST_CASE("BattleParser Baton Pass one other Pokemon moves second", "[Pokemon Showdown]") {
	auto parser = make_parser(
		make_known_two_pokemon_baton_pass_team(),
		make_seen_trap_team()
	);

	auto const values = containers::array{
		make_message_response("|turn|1", "/choose move 1"),
		make_message_response("|"),
		make_message_response("|t:|1"),
		make_message_response("|move|p2a: Dugtrio|Snore|p1a: Smeargle"),
		make_message_response("|move|p1a: Smeargle|Baton Pass|p1a: Smeargle", "/choose switch 2"),
		make_message_response("|"),
		make_message_response("|t:|2"),
		make_message_response("|switch|p1a: Shedinja|Shedinja|1/1"),
		make_message_response("|"),
		make_message_response("|upkeep"),
		make_message_response("|turn|2", "/choose move 1"),
	};
	check_values(parser, values);
	CHECK(parser.completed() == ps::BattleInterface::Complete::none);
}

TEST_CASE("BattleParser replace fainted in turn", "[Pokemon Showdown]") {
	auto parser = make_parser(
		make_known_two_pokemon_baton_pass_team(),
		make_seen_trap_team()
	);

	auto const values = containers::array{
		make_message_response("|turn|1", "/choose move 1"),
		make_message_response("|"),
		make_message_response("|t:|1"),
		make_message_response("|move|p2a: Dugtrio|Earthquake|p1a: Smeargle"),
		make_message_response("|-damage|p1a: Smeargle|0 fnt"),
		make_message_response("|faint|p1a: Smeargle", "/choose switch 2"),
		make_message_response("|"),
		make_message_response("|t:|2"),
		make_message_response("|switch|p1a: Shedinja|Shedinja|1/1"),
		make_message_response("|"),
		make_message_response("|upkeep"),
		make_message_response("|turn|2", "/choose move 1"),
	};
	check_values(parser, values);
	CHECK(parser.completed() == ps::BattleInterface::Complete::none);
}

TEST_CASE("BattleParser replace fainted end of turn", "[Pokemon Showdown]") {
	auto parser = make_parser(
		make_known_two_pokemon_baton_pass_team(),
		make_seen_trap_team()
	);

	auto const values = containers::array{
		make_message_response("|turn|1", "/choose move 1"),
		make_message_response("|"),
		make_message_response("|t:|1"),
		make_message_response("|move|p1a: Smeargle|Baton Pass|p1a: Smeargle", "/choose switch 2"),
		make_message_response("|"),
		make_message_response("|t:|2"),
		make_message_response("|switch|p1a: Shedinja|Shedinja|1/1"),
		make_message_response("|move|p2a: Dugtrio|Toxic|p1a: Shedinja"),
		make_message_response("|-status|p1a: Shedinja|tox"),
		make_message_response("|"),
		make_message_response("|-damage|p1a: Shedinja|0 fnt|[from] psn"),
		make_message_response("|faint|p1a: Shedinja"),
		make_message_response("|upkeep", "/choose switch 2"),
		make_message_response("|"),
		make_message_response("|t:|3"),
		make_message_response("|switch|p1a: Smeargle|Smeargle|251/251"),
		make_message_response("|turn|2", "/choose move 1"),
	};
	check_values(parser, values);
	CHECK(parser.completed() == ps::BattleInterface::Complete::none);
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
		make_message_response("|turn|1", AnyResponse()),
		make_message_response("|"),
		make_message_response("|t:|1"),
		make_message_response("|move|p1a: Smeargle|Agility|p1a: Smeargle"),
		make_message_response("|-boost|p1a: Smeargle|spe|2"),
		make_message_response("|move|p2a: Smeargle|Spikes|p1a: Smeargle"),
		make_message_response("|-sidestart|p1: Technical Machine|Spikes"),
		make_message_response("|"),
		make_message_response("|upkeep"),
		make_message_response("|turn|2", AnyResponse()),
		make_message_response("|"),
		make_message_response("|t:|2"),
		make_message_response("|-singleturn|p2a: Smeargle|move: Focus Punch"),
		make_message_response("|move|p1a: Smeargle|Agility|p1a: Smeargle"),
		make_message_response("|-boost|p1a: Smeargle|spe|2"),
		make_message_response("|move|p2a: Smeargle|Focus Punch|p1a: Smeargle"),
		make_message_response("|-supereffective|p1a: Smeargle"),
		make_message_response("|-damage|p1a: Smeargle|0 fnt"),
		make_message_response("|faint|p1a: Smeargle", AnyResponse()),
		make_message_response("|"),
		make_message_response("|t:|3"),
		make_message_response("|switch|p1a: Shedinja|Shedinja|1/1"),
		make_message_response("|-damage|p1a: Shedinja|0 fnt|[from] Spikes"),
		make_message_response("|faint|p1a: Shedinja", "/choose switch 3"),
		make_message_response("|"),
		make_message_response("|t:|4"),
		make_message_response("|switch|p1a: Chansey|Chansey|641/641"),
		make_message_response("|-damage|p1a: Chansey|561/641|[from] Spikes"),
		make_message_response("|"),
		make_message_response("|upkeep"),
		make_message_response("|turn|3", "/choose move 1"),
	};
	check_values(parser, values);
	CHECK(parser.completed() == ps::BattleInterface::Complete::none);
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
		make_message_response("|turn|1", AnyResponse()),
		make_message_response("|"),
		make_message_response("|t:|1"),
		make_message_response("|move|p1a: Smeargle|Agility|p1a: Smeargle"),
		make_message_response("|-boost|p1a: Smeargle|spe|2"),
		make_message_response("|move|p2a: Smeargle|Spikes|p1a: Smeargle"),
		make_message_response("|-sidestart|p1: Technical Machine|Spikes"),
		make_message_response("|"),
		make_message_response("|upkeep"),
		make_message_response("|turn|2", AnyResponse()),
		make_message_response("|"),
		make_message_response("|t:|2"),
		make_message_response("|switch|p1a: Shedinja|Shedinja|1/1"),
		make_message_response("|-damage|p1a: Shedinja|0 fnt|[from] Spikes"),
		make_message_response("|faint|p1a: Shedinja", "/choose switch 2"),
		make_message_response("|"),
		make_message_response("|t:|3"),
		make_message_response("|switch|p1a: Smeargle|Smeargle|251/251"),
		make_message_response("|-damage|p1a: Smeargle|220/251|[from] Spikes"),
		make_message_response("|"),
		make_message_response("|upkeep"),
		make_message_response("|turn|3", "/choose move 1"),
	};
	check_values(parser, values);
	CHECK(parser.completed() == ps::BattleInterface::Complete::none);
}

TEST_CASE("BattleParser lose", "[Pokemon Showdown]") {
	auto parser = make_parser(
		make_known_one_pokemon_team(),
		make_seen_trap_team()
	);

	auto const values = containers::array{
		make_message_response("|turn|1", "/choose move 1"),
		make_message_response("|"),
		make_message_response("|t:|1"),
		make_message_response("|move|p2a: Dugtrio|Earthquake|p1a: Smeargle"),
		make_message_response("|-damage|p1a: Smeargle|0 fnt"),
		make_message_response("|faint|p1a: Smeargle"),
		make_message_response("|"),
		make_message_response("|win|obi"),
	};
	check_values(parser, values);
	CHECK(parser.completed() == ps::BattleInterface::Complete::finish);
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
		make_message_response("|turn|1", "/choose move 1"),
		make_message_response("|"),
		make_message_response("|t:|1"),
		make_message_response("|move|p1a: Gengar|Ice Punch|p2a: Zapdos"),
		make_message_response("|-supereffective|p2a: Zapdos"),
		make_message_response("|-damage|p2a: Zapdos|57/100"),
		make_message_response("|-status|p2a: Zapdos|frz"),
		make_message_response("|cant|p2a: Zapdos|frz"),
		make_message_response("|"),
		make_message_response("|-curestatus|p2a: Zapdos|frz|[msg]"),
		make_message_response("|upkeep|"),
	};
	check_values(parser, values);
	CHECK(parser.completed() == ps::BattleInterface::Complete::none);
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
		make_message_response("|turn|1", AnyResponse()),
		make_message_response("|"),
		make_message_response("|t:|1"),
		make_message_response("|move|p2a: Gengar|Explosion|p1a: Jynx"),
		make_message_response("|-damage|p1a: Jynx|0 fnt"),
		make_message_response("|faint|p2a: Gengar"),
		make_message_response("|faint|p1a: Jynx", "/choose switch 2"),
	};
	check_values(parser, values);
	CHECK(parser.completed() == ps::BattleInterface::Complete::none);
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
		make_message_response("|turn|1", AnyResponse()),
		make_message_response("|"),
		make_message_response("|t:|1"),
		make_message_response("|switch|p2a: Raikou|Raikou, shiny|100/100"),
	};
	check_values(parser, values);
	CHECK(parser.completed() == ps::BattleInterface::Complete::none);
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
		make_message_response("|turn|1", "/choose move 1"),
		make_message_response("|"),
		make_message_response("|t:|1"),
		make_message_response("|move|p1a: Smeargle|Sketch||[still]"),
		make_message_response("|-fail|p1a: Smeargle"),
		make_message_response("|move|p2a: Smeargle|Splash|p2a: Smeargle"),
		make_message_response("|-nothing"),
		make_message_response("||"),
		make_message_response("|upkeep|"),
		make_message_response("|turn|2", "/choose move 1"),
		make_message_response("|-activate|p1a: Smeargle|move: Struggle"),
		make_message_response("||"),
		make_message_response("|t:|1"),
		make_message_response("|move|p1a: Smeargle|Struggle|p2a: Smeargle"),
		make_message_response("|-damage|p2a: Smeargle|92/100"),
		make_message_response("|-damage|p1a: Smeargle|246/251|[from] Recoil|[of] p2a: Smeargle"),
		make_message_response("|move|p2a: Smeargle|Splash|p2a: Smeargle"),
		make_message_response("|-nothing"),
		make_message_response("||"),
		make_message_response("|upkeep|"),
	};
	check_values(parser, values);
	CHECK(parser.completed() == ps::BattleInterface::Complete::none);
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
		make_message_response("|turn|1", "/choose move 1"),
		make_message_response("|move|p1a: Hypno|Curse|p1a: Hypno"),
		make_message_response("|move|p2a: Blissey|Thunder Wave|p1a: Hypno"),
		make_message_response("|-status|p1a: Hypno|par"),
		make_message_response("|upkeep|"),
		make_message_response("|turn|2", "/choose move 1"),
		make_message_response("|move|p2a: Blissey|Ice Beam|p1a: Hypno"),
		make_message_response("|-damage|p1a: Hypno|124/278 par"),
		make_message_response("|cant|p1a: Hypno|par"),
		make_message_response("||"),
		make_message_response("|-heal|p1a: Hypno|141/278 par|[from] item: Leftovers"),
		make_message_response("|upkeep|"),
	};
	check_values(parser, values);
	CHECK(parser.completed() == ps::BattleInterface::Complete::none);
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
		make_message_response("|turn|1", "/choose move 1"),
		make_message_response("|move|p2a: Blissey|Ice Beam|p1a: Misdreavus"),
		make_message_response("|-damage|p1a: Misdreavus|85/222"),
		make_message_response("|move|p1a: Misdreavus|Pain Split|p2a: Blissey"),
		make_message_response("|-sethp|p2a: Blissey|46/100|[from] move: Pain Split|[silent]"),
	};
	check_values(parser, values);
	CHECK(parser.completed() == ps::BattleInterface::Complete::none);
}

} // namespace
} // namespace technicalmachine
