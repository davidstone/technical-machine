// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/pokemon_showdown/battle_parser.hpp>

#include <tm/team_predictor/usage_stats.hpp>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {

using namespace std::string_view_literals;

constexpr auto generation = Generation::three;

auto make_seen_trap_team() {
	auto team = SeenTeam<generation>(1_bi);
	auto & pokemon = team.add_pokemon(SeenPokemon<generation>(
		Species::Dugtrio,
		"Dugtrio",
		Level(100_bi),
		Gender::male
	));
	pokemon.set_initial_ability(Ability::Arena_Trap);
	return team;
}

auto make_seen_spikes_team() {
	auto team = SeenTeam<generation>(1_bi);
	auto & pokemon = team.add_pokemon(SeenPokemon<generation>(
		Species::Smeargle,
		"Smeargle",
		Level(100_bi),
		Gender::male
	));
	pokemon.set_initial_ability(Ability::Own_Tempo);
	return team;
}

auto get_usage_stats() -> UsageStats const & {
	[[clang::no_destroy]] static auto const result = stats_for_generation(generation);
	return result;
}

constexpr auto log_foe_teams = false;
constexpr auto id = "battle-gen3ou-1"sv;

auto get_evaluate() -> Evaluate<generation> {
	static auto const result = Evaluate<generation>();
	return result;
}

auto make_known_one_pokemon_team() {
	return KnownTeam<generation>({
		KnownPokemon<generation>(
			Species::Smeargle,
			"Smeargle",
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

auto make_known_two_pokemon_team() {
	return KnownTeam<generation>({
		KnownPokemon<generation>(
			Species::Smeargle,
			"Smeargle",
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
			"Shedinja",
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

auto make_known_two_pokemon_baton_pass_team() {
	return KnownTeam<generation>({
		KnownPokemon<generation>(
			Species::Smeargle,
			"Smeargle",
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
			"Shedinja",
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

auto make_known_three_pokemon_team() {
	return KnownTeam<generation>({
		KnownPokemon<generation>(
			Species::Smeargle,
			"Smeargle",
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
			"Shedinja",
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
			"Chansey",
			Level(100_bi),
			Gender::female,
			Item::None,
			Ability::Natural_Cure,
			default_combined_stats<generation>,
			RegularMoves({
				Move(generation, MoveName::Aromatherapy)
			})
		)
	});
}

auto make_parser(KnownTeam<generation> ai, SeenTeam<generation> foe) -> ps::BattleParser {
	auto random_device = std::random_device();
	return ps::BattleParser(
		AnalysisLogger(AnalysisLogger::none()),
		containers::string(id),
		"Technical Machine",
		get_usage_stats(),
		GenerationGeneric<BattleManagerInputs>(BattleManagerInputs<generation>{
			Teams<generation>{
				std::move(ai),
				std::move(foe)
			},
			get_evaluate(),
		}),
		Party(0_bi),
		DepthValues{0_bi, 0_bi},
		std::mt19937(random_device()),
		log_foe_teams
	);
}

struct NoResponse{};
struct AnyResponse{};
using Response = bounded::variant<NoResponse, AnyResponse, containers::string>;
struct MessageResponse {
	std::string_view message;
	Response response;
};

constexpr auto make_message_response(std::string_view const message, std::string_view const response) {
	return MessageResponse{
		message,
		Response(containers::concatenate<containers::string>(id, "|"sv, response))
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
		auto const response = parser.handle_message(ps::InMessage(id, value.message));
		INFO(value.message);
		if (response) {
			UNSCOPED_INFO("*response: " << *response);
		}
		bounded::visit(value.response, bounded::overload(
			[&](NoResponse) { CHECK(!response); },
			[&](AnyResponse) { CHECK(static_cast<bool>(response)); },
			[&](std::string_view const str) {
				CHECK(static_cast<bool>(response));
				CHECK(std::string_view(*response) == str);
			}
		));
	}
}

TEST_CASE("BattleParser has right ID", "[Pokemon Showdown]") {
	auto parser = make_parser(
		make_known_one_pokemon_team(),
		make_seen_trap_team()
	);

	CHECK(parser.id() == id);
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

TEST_CASE("BattleParser replace multiple fainted", "[Pokemon Showdown]") {
	auto parser = make_parser(
		make_known_three_pokemon_team(),
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
	auto parser = make_parser(
		make_known_two_pokemon_team(),
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

} // namespace
} // namespace technicalmachine
