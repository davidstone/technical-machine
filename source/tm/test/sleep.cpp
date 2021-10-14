// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/move/call_move.hpp>
#include <tm/move/moves.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>

#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <bounded/assert.hpp>

#include <containers/front_back.hpp>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto damage = ActualDamage::Unknown{};	

constexpr auto regular_moves(Generation const generation, auto... moves) {
	return RegularMoves{Move(generation, moves)...};
}

TEST_CASE("Sleep Talk", "[Sleep]") {
	constexpr auto generation = Generation::four;
	auto weather = Weather{};
	auto attacker = Team<generation>(1_bi, true);
	{
		auto jolteon = Pokemon<generation>(
			Species::Jolteon,
			Level(100_bi),
			Gender::female,
			Item::Leftovers,
			Ability::Volt_Absorb,
			CombinedStats<generation>{
				Nature::Hardy,
				max_dvs_or_ivs<generation>,
				EVs(
					EV(0_bi),
					EV(0_bi),
					EV(0_bi),
					EV(252_bi),
					EV(0_bi),
					EV(0_bi)
				)
			},
			regular_moves(generation, Moves::Sleep_Talk, Moves::Thunderbolt)
		);
		jolteon.set_status(Statuses::sleep);
		
		attacker.add_pokemon(jolteon);
		attacker.pokemon().switch_in(weather);
	}

	auto defender = Team<generation>(1_bi);
	defender.add_pokemon(Pokemon<generation>(
		Species::Gyarados,
		Level(100_bi),
		Gender::male,
		Item::Life_Orb,
		Ability::Intimidate,
		CombinedStats<generation>{
			Nature::Adamant,
			max_dvs_or_ivs<generation>,
			EVs(
				EV(0_bi),
				EV(252_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi)
			)
		},
		RegularMoves({Move(generation, Moves::Earthquake)})
	));
	defender.pokemon().switch_in(weather);

	call_move(
		attacker,
		UsedMove<generation>(
			Moves::Sleep_Talk,
			Moves::Thunderbolt,
			false,
			false,
			no_effect_function
		),
		defender,
		FutureMove{false},
		weather,
		false,
		damage
	);
	CHECK(defender.pokemon().hp().current() == 0_bi);
}

template<Generation generation>
struct Sleeper {
	Sleeper():
		m_sleeper(make_sleeper_team(m_weather)),
		m_other(make_other_team(m_weather))
	{
	}

	void use_move(Moves const selected, Moves const executed) {
		auto pokemon = m_sleeper.pokemon();
		auto const probability_of_awakening = pokemon.status().probability_of_clearing(generation, pokemon.ability());
		CHECK((probability_of_awakening == 0.0 or probability_of_awakening == 1.0));
		auto const side_effects = possible_side_effects(executed, pokemon.as_const(), m_other, m_weather);
		auto const & side_effect = containers::front(side_effects);
		call_move(
			m_sleeper,
			UsedMove<generation>(
				selected,
				executed,
				false,
				false,
				side_effect.function
			),
			m_other,
			FutureMove{false},
			m_weather,
			probability_of_awakening == 1.0,
			ActualDamage::Unknown()
		);
	}
	void use_move(Moves const selected) {
		use_move(selected, selected);
	}

	void get_attacked() {
		call_move(
			m_other,
			UsedMove<generation>(
				Moves::Seismic_Toss,
				no_effect_function
			),
			m_sleeper,
			FutureMove{false},
			m_weather,
			false,
			ActualDamage::Unknown()
		);
	}

	void end_turn() {
		m_sleeper.reset_start_of_turn();
		m_other.reset_start_of_turn();
	}

	auto at_max_hp() const {
		auto const hp = m_sleeper.pokemon().hp();
		return hp.current() == hp.max();
	}
	auto asleep() const {
		return m_sleeper.pokemon().status().name() == Statuses::rest;
	}

private:
	static auto make_team(bool const is_me, RegularMoves const moves, Weather & weather) {
		auto sleeper = Team<generation>(1_bi, is_me);
		sleeper.add_pokemon(Pokemon<generation>(
			Species::Blissey,
			Level(100_bi),
			Gender::female,
			Item::None,
			Ability::Natural_Cure,
			default_combined_stats<generation>,
			moves
		));
		sleeper.pokemon().switch_in(weather);
		return sleeper;
	}
	static auto make_sleeper_team(Weather & weather) -> Team<generation> {
		return make_team(true, regular_moves(generation, Moves::Rest, Moves::Sleep_Talk, Moves::Wish), weather);
	}
	static auto make_other_team(Weather & weather) -> Team<generation> {
		return make_team(false, regular_moves(generation, Moves::Seismic_Toss), weather);
	}

	Weather m_weather;
	Team<generation> m_sleeper;
	Team<generation> m_other;
};

TEST_CASE("Rest", "[Sleep]") {
	auto sleeper = Sleeper<Generation::three>();
	CHECK(!sleeper.asleep());
	CHECK(sleeper.at_max_hp());


	sleeper.get_attacked();
	CHECK(!sleeper.asleep());
	CHECK(!sleeper.at_max_hp());

	sleeper.use_move(Moves::Rest);
	CHECK(sleeper.asleep());
	CHECK(sleeper.at_max_hp());

	sleeper.end_turn();
	CHECK(sleeper.asleep());
	CHECK(sleeper.at_max_hp());


	sleeper.use_move(Moves::Wish);
	CHECK(sleeper.asleep());
	CHECK(sleeper.at_max_hp());

	sleeper.get_attacked();
	CHECK(sleeper.asleep());
	CHECK(!sleeper.at_max_hp());

	sleeper.end_turn();
	CHECK(sleeper.asleep());
	CHECK(!sleeper.at_max_hp());


	sleeper.use_move(Moves::Wish);
	CHECK(sleeper.asleep());
	CHECK(!sleeper.at_max_hp());

	sleeper.get_attacked();
	CHECK(sleeper.asleep());
	CHECK(!sleeper.at_max_hp());

	sleeper.end_turn();
	CHECK(sleeper.asleep());
	CHECK(!sleeper.at_max_hp());


	sleeper.use_move(Moves::Wish);
	CHECK(!sleeper.asleep());
	CHECK(!sleeper.at_max_hp());
}

TEST_CASE("Sleep Talk Rest generation 3", "[Sleep]") {
	auto sleeper = Sleeper<Generation::three>();
	CHECK(!sleeper.asleep());
	CHECK(sleeper.at_max_hp());


	sleeper.get_attacked();
	CHECK(!sleeper.asleep());
	CHECK(!sleeper.at_max_hp());

	sleeper.use_move(Moves::Rest);
	CHECK(sleeper.asleep());
	CHECK(sleeper.at_max_hp());

	sleeper.end_turn();
	CHECK(sleeper.asleep());
	CHECK(sleeper.at_max_hp());


	sleeper.use_move(Moves::Sleep_Talk, Moves::Rest);
	CHECK(sleeper.asleep());
	CHECK(sleeper.at_max_hp());

	sleeper.get_attacked();
	CHECK(sleeper.asleep());
	CHECK(!sleeper.at_max_hp());

	sleeper.end_turn();
	CHECK(sleeper.asleep());
	CHECK(!sleeper.at_max_hp());


	sleeper.use_move(Moves::Sleep_Talk, Moves::Rest);
	CHECK(sleeper.asleep());
	CHECK(!sleeper.at_max_hp());

	sleeper.get_attacked();
	CHECK(sleeper.asleep());
	CHECK(!sleeper.at_max_hp());

	sleeper.end_turn();
	CHECK(sleeper.asleep());
	CHECK(!sleeper.at_max_hp());


	sleeper.use_move(Moves::Wish);
	CHECK(!sleeper.asleep());
	CHECK(!sleeper.at_max_hp());
}

} // namespace
} // namespace technicalmachine
