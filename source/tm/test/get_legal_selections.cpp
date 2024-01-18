// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.test.get_legal_selections;

import tm.move.legal_selections;
import tm.move.move_name;

import tm.pokemon.species;

import tm.environment;
import tm.generation;
import tm.get_legal_selections;
import tm.item;
import tm.team;

import bounded;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

static_assert([]{
	constexpr auto generation = Generation::four;

	auto environment = Environment();

	auto user = Team<generation>({{
		{
			.species = Species::Misdreavus,
			.item = Item::Choice_Specs,
			.moves = {{
				MoveName::Shadow_Ball,
			}}
		},
	}});
	user.pokemon().switch_in(environment);

	auto other = Team<generation>({{
		{
			.species = Species::Smeargle,
			.moves = {{
				MoveName::Tackle,
			}}
		},
	}});
	other.pokemon().switch_in(environment);

	return
		get_legal_selections(user, other, environment) ==
		LegalSelections({MoveName::Shadow_Ball});
}());

static_assert([]{
	constexpr auto generation = Generation::four;

	auto environment = Environment();

	auto user = Team<generation>({{
		{
			.species = Species::Smeargle,
			.moves = {{
				MoveName::Baton_Pass,
				MoveName::Belly_Drum,
			}}
		},
		{
			.species = Species::Alakazam,
			.moves = {{
				MoveName::Tackle,
			}}
		},
	}});
	user.pokemon().switch_in(environment);

	auto other = Team<generation>({{
		{
			.species = Species::Smeargle,
			.moves = {{
				MoveName::Tackle,
			}}
		},
	}});
	other.pokemon().switch_in(environment);

	return
		get_legal_selections(user, other, environment) ==
		LegalSelections({
			MoveName::Baton_Pass,
			MoveName::Belly_Drum,
			MoveName::Switch1
		});
}());

static_assert([]{
	constexpr auto generation = Generation::four;

	auto environment = Environment();

	auto user = Team<generation>({{
		{
			.species = Species::Magikarp,
			.moves = {{
				MoveName::Splash,
			}}
		},
	}});
	user.pokemon().switch_in(environment);

	auto other = Team<generation>({{
		{
			.species = Species::Magikarp,
			.moves = {{
				MoveName::Splash,
			}}
		},
	}});
	other.pokemon().switch_in(environment);

	user.pokemon().successfully_use_move(MoveName::Splash);
	other.pokemon().successfully_use_move(MoveName::Splash);

	auto const can_pass = [&](auto const & attacker, auto const & defender) {
		return
			get_legal_selections(attacker, defender, environment) ==
			LegalSelections({MoveName::Pass});
	};
	return can_pass(user, other) and can_pass(other, user);
}());

static_assert([]{
	constexpr auto generation = Generation::four;

	auto environment = Environment();

	auto user = Team<generation>({{
		{
			.species = Species::Smeargle,
			.moves = {{
				MoveName::Baton_Pass,
			}}
		},
		{
			.species = Species::Alakazam,
			.moves = {{
				MoveName::Tackle,
			}}
		},
	}});
	user.pokemon().switch_in(environment);

	auto other = Team<generation>({{
		{
			.species = Species::Smeargle,
			.moves = {{
				MoveName::Tackle,
			}}
		},
	}});
	other.pokemon().switch_in(environment);

	user.pokemon().successfully_use_move(MoveName::Baton_Pass);

	auto const user_can_switch =
		get_legal_selections(user, other, environment) ==
		LegalSelections({MoveName::Switch1});
	auto const other_can_pass =
		get_legal_selections(other, user, environment) ==
		LegalSelections({MoveName::Pass});
	return user_can_switch and other_can_pass;
}());

constexpr auto use_move_that_kos(
	auto const user,
	auto const other,
	Environment const environment,
	MoveName const move_name
) {
	other.direct_damage(
		move_name,
		user,
		environment,
		other.hp().current().value()
	);
	user.successfully_use_move(move_name);
}

static_assert([]{
	constexpr auto generation = Generation::one;

	auto environment = Environment();

	auto user = Team<generation>({{
		{
			.species = Species::Pikachu,
			.moves = {{
				MoveName::Thunderbolt,
			}}
		},
		{
			.species = Species::Tauros,
			.moves = {{
				MoveName::Tackle,
			}}
		},
	}});
	user.pokemon().switch_in(environment);

	auto other = Team<generation>({{
		{
			.species = Species::Golem,
			.moves = {{
				MoveName::Earthquake,
			}}
		},
	}});
	other.pokemon().switch_in(environment);

	user.pokemon().successfully_use_move(MoveName::Thunderbolt);
	use_move_that_kos(
		other.pokemon(),
		user.pokemon(),
		environment,
		MoveName::Earthquake
	);

	user.reset_end_of_turn();
	other.reset_end_of_turn();

	auto const user_can_switch =
		get_legal_selections(user, other, environment) ==
		LegalSelections({MoveName::Switch1});
	auto const other_can_pass =
		get_legal_selections(other, user, environment) ==
		LegalSelections({MoveName::Pass});
	return user_can_switch and other_can_pass;
}());

static_assert([]{
	constexpr auto generation = Generation::one;

	auto environment = Environment();

	auto user = Team<generation>({{
		{
			.species = Species::Pikachu,
			.moves = {{
				MoveName::Thunderbolt,
			}}
		},
		{
			.species = Species::Tauros,
			.moves = {{
				MoveName::Hyper_Beam,
			}}
		},
	}});
	user.pokemon().switch_in(environment);

	auto other = Team<generation>({{
		{
			.species = Species::Golem,
			.moves = {{
				MoveName::Earthquake,
			}}
		},
	}});
	other.pokemon().switch_in(environment);

	user.pokemon().successfully_use_move(MoveName::Thunderbolt);
	use_move_that_kos(
		other.pokemon(),
		user.pokemon(),
		environment,
		MoveName::Earthquake
	);

	user.reset_end_of_turn();
	other.reset_end_of_turn();

	user.switch_pokemon(other.pokemon(), environment, 1_bi);

	auto const user_must_attack =
		get_legal_selections(user, other, environment) ==
		LegalSelections({MoveName::Hyper_Beam});
	auto const other_must_attack =
		get_legal_selections(other, user, environment) ==
		LegalSelections({MoveName::Earthquake});
	return user_must_attack and other_must_attack;
}());

} // namespace
} // namespace technicalmachine
