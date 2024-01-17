// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <std_module/prelude.hpp>
#include <catch2/catch_test_macros.hpp>

import tm.move.actual_damage;
import tm.move.call_move;
import tm.move.move;
import tm.move.move_name;
import tm.move.no_effect_function;
import tm.move.other_move;
import tm.move.side_effects;
import tm.move.used_move;

import tm.pokemon.level;
import tm.pokemon.pokemon;
import tm.pokemon.species;

import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.evs;
import tm.stat.iv;
import tm.stat.nature;

import tm.status.status_name;

import tm.test.pokemon_init;

import tm.ability;
import tm.contact_ability_effect;
import tm.environment;
import tm.gender;
import tm.generation;
import tm.item;
import tm.team;

import bounded;
import containers;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto damage = ActualDamage::Unknown{};

TEST_CASE("Sleep Talk", "[Sleep]") {
	constexpr auto generation = Generation::four;
	auto environment = Environment();
	auto attacker = make_team<generation>({
		{
			.species = Species::Zapdos,
			.moves = {{
				MoveName::Sleep_Talk,
				MoveName::Thunderbolt,
			}}
		},
	});
	attacker.pokemon().switch_in(environment);
	attacker.pokemon().set_status(StatusName::sleep, environment);

	auto defender = make_team<generation>({
		{
			.species = Species::Gyarados,
		},
	});
	defender.pokemon().switch_in(environment);

	call_move(
		attacker,
		UsedMove<Team<generation>>(
			MoveName::Sleep_Talk,
			MoveName::Thunderbolt,
			false,
			false,
			ContactAbilityEffect::nothing,
			no_effect_function
		),
		defender,
		FutureMove{false},
		environment,
		false,
		damage,
		false
	);
	CHECK(defender.pokemon().hp().current() == 0_bi);
}

template<Generation generation>
struct Sleeper {
	Sleeper():
		m_sleeper(make_sleeper_team(m_environment)),
		m_other(make_other_team(m_environment))
	{
	}

	void use_move(MoveName const selected, MoveName const executed) {
		auto pokemon = m_sleeper.pokemon();
		auto const probability_of_awakening = pokemon.status().probability_of_clearing(generation, pokemon.ability());
		CHECK((probability_of_awakening == 0.0 or probability_of_awakening == 1.0));
		auto const side_effects = possible_side_effects(executed, pokemon.as_const(), m_other, m_environment);
		auto const & side_effect = containers::front(side_effects);
		call_move(
			m_sleeper,
			UsedMove<Team<generation>>(
				selected,
				executed,
				false,
				false,
				ContactAbilityEffect::nothing,
				side_effect.function
			),
			m_other,
			FutureMove{false},
			m_environment,
			probability_of_awakening == 1.0,
			ActualDamage::Unknown(),
			false
		);
	}
	void use_move(MoveName const selected) {
		use_move(selected, selected);
	}

	void get_attacked() {
		call_move(
			m_other,
			UsedMove<Team<generation>>(
				MoveName::Seismic_Toss,
				no_effect_function
			),
			m_sleeper,
			FutureMove{false},
			m_environment,
			false,
			ActualDamage::Unknown(),
			false
		);
	}

	void end_turn() {
		m_sleeper.reset_end_of_turn();
		m_other.reset_end_of_turn();
	}

	auto at_max_hp() const {
		auto const hp = m_sleeper.pokemon().hp();
		return hp.current() == hp.max();
	}
	auto asleep() const {
		return m_sleeper.pokemon().status().name() == StatusName::rest;
	}

private:
	static auto make_sleeper_team(Environment & environment) -> Team<generation> {
		auto team = make_team<generation>({
			{
				.species = Species::Blissey,
				.moves = {{
					MoveName::Rest,
					MoveName::Sleep_Talk,
					MoveName::Wish,
				}}
			},
		});
		team.pokemon().switch_in(environment);
		return team;
	}
	static auto make_other_team(Environment & environment) -> Team<generation> {
		auto team = make_team<generation>({
			{
				.species = Species::Blissey,
				.moves = {{
					MoveName::Seismic_Toss,
				}}
			},
		});
		team.pokemon().switch_in(environment);
		return team;
	}

	Environment m_environment;
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

	sleeper.use_move(MoveName::Rest);
	CHECK(sleeper.asleep());
	CHECK(sleeper.at_max_hp());

	sleeper.end_turn();
	CHECK(sleeper.asleep());
	CHECK(sleeper.at_max_hp());


	sleeper.use_move(MoveName::Wish);
	CHECK(sleeper.asleep());
	CHECK(sleeper.at_max_hp());

	sleeper.get_attacked();
	CHECK(sleeper.asleep());
	CHECK(!sleeper.at_max_hp());

	sleeper.end_turn();
	CHECK(sleeper.asleep());
	CHECK(!sleeper.at_max_hp());


	sleeper.use_move(MoveName::Wish);
	CHECK(sleeper.asleep());
	CHECK(!sleeper.at_max_hp());

	sleeper.get_attacked();
	CHECK(sleeper.asleep());
	CHECK(!sleeper.at_max_hp());

	sleeper.end_turn();
	CHECK(sleeper.asleep());
	CHECK(!sleeper.at_max_hp());


	sleeper.use_move(MoveName::Wish);
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

	sleeper.use_move(MoveName::Rest);
	CHECK(sleeper.asleep());
	CHECK(sleeper.at_max_hp());

	sleeper.end_turn();
	CHECK(sleeper.asleep());
	CHECK(sleeper.at_max_hp());


	sleeper.use_move(MoveName::Sleep_Talk, MoveName::Rest);
	CHECK(sleeper.asleep());
	CHECK(sleeper.at_max_hp());

	sleeper.get_attacked();
	CHECK(sleeper.asleep());
	CHECK(!sleeper.at_max_hp());

	sleeper.end_turn();
	CHECK(sleeper.asleep());
	CHECK(!sleeper.at_max_hp());


	sleeper.use_move(MoveName::Sleep_Talk, MoveName::Rest);
	CHECK(sleeper.asleep());
	CHECK(!sleeper.at_max_hp());

	sleeper.get_attacked();
	CHECK(sleeper.asleep());
	CHECK(!sleeper.at_max_hp());

	sleeper.end_turn();
	CHECK(sleeper.asleep());
	CHECK(!sleeper.at_max_hp());


	sleeper.use_move(MoveName::Wish);
	CHECK(!sleeper.asleep());
	CHECK(!sleeper.at_max_hp());
}

} // namespace
} // namespace technicalmachine
