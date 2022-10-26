// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/battle.hpp>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto generation = Generation::four;
constexpr auto damage = FlaggedActualDamage{ActualDamage::Known{0_bi}, false};
constexpr auto nickname = std::string_view();
constexpr auto end_of_turn_flags = EndOfTurnFlags(false, false, false);

constexpr auto regular_moves(auto... moves) {
	return RegularMoves{Move(generation, moves)...};
}

TEST_CASE("Perish Song", "[battle]") {
	auto battle = Battle<generation>(
		KnownTeam<generation>({
			KnownPokemon<generation>(
				Species::Misdreavus,
				nickname,
				Level(100_bi),
				Gender::female,
				Item::None,
				Ability::Levitate,
				default_combined_stats<generation>,
				regular_moves(MoveName::Perish_Song)
			),
			KnownPokemon<generation>(
				Species::Regice,
				nickname,
				Level(100_bi),
				Gender::genderless,
				Item::None,
				Ability::Clear_Body,
				default_combined_stats<generation>,
				regular_moves(MoveName::Explosion)
			)
		}),
		SeenTeam<generation>(2_bi)
	);
	battle.find_or_add_foe_pokemon(Species::Starmie, nickname, Level(100_bi), Gender::genderless);
	battle.add_move(false, MoveName::Recover);

	auto const recover_side_effects = possible_side_effects(
		MoveName::Recover,
		battle.foe().pokemon(),
		battle.ai(),
		battle.weather()
	);
	CHECK(containers::size(recover_side_effects) == 1_bi);

	auto call_recover = [&] {
		battle.handle_use_move(
			UsedMove<SeenTeam<generation>>(
				MoveName::Recover,
				containers::front(recover_side_effects).function
			),
			false,
			damage
		);
	};

	auto const perish_song_side_effects = possible_side_effects(
		MoveName::Perish_Song,
		battle.ai().pokemon(),
		battle.foe(),
		battle.weather()
	);
	CHECK(containers::size(perish_song_side_effects) == 1_bi);

	auto call_perish_song = [&] {
		battle.handle_use_move(
			UsedMove<KnownTeam<generation>>(
				MoveName::Perish_Song,
				containers::front(perish_song_side_effects).function
			),
			false,
			damage
		);
	};

	for (auto const turn : containers::integer_range(4_bi)) {
		INFO(turn);
		battle.handle_begin_turn();

		CHECK(battle.ai().pokemon().hp().current() == battle.ai().pokemon().hp().max());
		CHECK(battle.foe().pokemon().hp().current() == battle.foe().pokemon().hp().max());

		call_recover();
		call_perish_song();

		CHECK(battle.ai().pokemon().hp().current() == battle.ai().pokemon().hp().max());
		CHECK(battle.foe().pokemon().hp().current() == battle.foe().pokemon().hp().max());

		battle.handle_end_turn(false, end_of_turn_flags, end_of_turn_flags);
	}

	CHECK(battle.ai().pokemon().hp().current() == 0_bi);
	CHECK(battle.foe().pokemon().hp().current() == 0_bi);
}

} // namespace
} // namespace technicalmachine
