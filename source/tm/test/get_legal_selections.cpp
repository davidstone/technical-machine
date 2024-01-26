// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.test.get_legal_selections;

import tm.move.legal_selections;
import tm.move.move_name;
import tm.move.switch_;

import tm.pokemon.initial_pokemon;
import tm.pokemon.species;

import tm.stat.stat_style;

import tm.environment;
import tm.generation;
import tm.get_legal_selections;
import tm.item;
import tm.team;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

template<Generation generation, std::size_t size>
constexpr auto make_team(Environment const environment, containers::c_array<InitialPokemon<special_style_for(generation)>, size> && pokemon) {
	auto team = Team<generation>({std::move(pokemon)});
	team.pokemon().switch_in(environment, true);
	return team;
}

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

namespace one_action {
	constexpr auto generation = Generation::four;

	constexpr auto environment = Environment();

	constexpr auto user = make_team<generation>(environment, {
		{
			.species = Species::Misdreavus,
			.moves = {{
				MoveName::Shadow_Ball,
			}}
		},
	});

	constexpr auto other = make_team<generation>(environment, {
		{
			.species = Species::Smeargle,
			.moves = {{
				MoveName::Tackle,
			}}
		},
	});

	static_assert(
		get_legal_selections(user, other, environment) ==
		LegalSelections({MoveName::Shadow_Ball})
	);
}

namespace one_action_with_choice_item {
	constexpr auto generation = Generation::four;

	constexpr auto environment = Environment();

	constexpr auto user = make_team<generation>(environment, {
		{
			.species = Species::Misdreavus,
			.item = Item::Choice_Specs,
			.moves = {{
				MoveName::Shadow_Ball,
			}}
		},
	});

	constexpr auto other = make_team<generation>(environment, {
		{
			.species = Species::Smeargle,
			.moves = {{
				MoveName::Tackle,
			}}
		},
	});

	static_assert(
		get_legal_selections(user, other, environment) ==
		LegalSelections({MoveName::Shadow_Ball})
	);
}

namespace moves_and_switch {
	constexpr auto generation = Generation::four;

	constexpr auto environment = Environment();

	constexpr auto user = make_team<generation>(environment, {
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
	});

	constexpr auto other = make_team<generation>(environment, {
		{
			.species = Species::Smeargle,
			.moves = {{
				MoveName::Tackle,
			}}
		},
	});

	static_assert(
		get_legal_selections(user, other, environment) ==
		LegalSelections({
			MoveName::Baton_Pass,
			MoveName::Belly_Drum,
			Switch(1_bi)
		})
	);
}

namespace already_acted {
	constexpr auto generation = Generation::four;

	constexpr auto environment = Environment();

	constexpr auto make = [] {
		auto team = make_team<generation>(environment, {
			{
				.species = Species::Magikarp,
				.moves = {{
					MoveName::Splash,
				}}
			},
		});
		team.pokemon().successfully_use_move(MoveName::Splash);
		return team;
	};
	constexpr auto user = make();
	constexpr auto other = user;

	static_assert(
		get_legal_selections(user, other, environment) ==
		LegalSelections({MoveName::Pass})
	);
}

namespace using_baton_pass {
	constexpr auto generation = Generation::four;

	constexpr auto environment = Environment();

	constexpr auto user = [] {
		auto team = make_team<generation>(environment, {
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
		});
		team.pokemon().successfully_use_move(MoveName::Baton_Pass);
		return team;
	}();

	constexpr auto other = make_team<generation>(environment, {
		{
			.species = Species::Smeargle,
			.moves = {{
				MoveName::Tackle,
			}}
		},
	});

	static_assert(
		get_legal_selections(user, other, environment) ==
		LegalSelections({Switch(1_bi)})
	);
	static_assert(
		get_legal_selections(other, user, environment) ==
		LegalSelections({MoveName::Pass})
	);
}

static_assert([]{
	constexpr auto generation = Generation::one;

	constexpr auto environment = Environment();

	auto user = make_team<generation>(environment, {
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
	});

	auto other = make_team<generation>(environment, {
		{
			.species = Species::Golem,
			.moves = {{
				MoveName::Earthquake,
			}}
		},
	});

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
		LegalSelections({Switch(1_bi)});
	auto const other_can_pass =
		get_legal_selections(other, user, environment) ==
		LegalSelections({MoveName::Pass});
	return user_can_switch and other_can_pass;
}());

static_assert([]{
	constexpr auto generation = Generation::one;

	auto environment = Environment();

	auto user = make_team<generation>(environment, {
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
	});

	auto other = make_team<generation>(environment, {
		{
			.species = Species::Golem,
			.moves = {{
				MoveName::Earthquake,
			}}
		},
	});

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
