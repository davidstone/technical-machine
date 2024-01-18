// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.test.critical_hit_probability;

import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.pokemon.level;
import tm.pokemon.species;

import tm.ability;
import tm.critical_hit_probability;
import tm.environment;
import tm.gender;
import tm.generation;
import tm.item;
import tm.team;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

template<Generation generation>
constexpr auto individual_test(Species const species, MoveName const move_name, Item const item, bool const focus_energy, double const rate) -> bool {
	auto environment = Environment();
	auto attacker = Team<generation>({{
		{
			.species = species,
			.item = item,
			.moves = {{
				move_name,
			}}
		},
	}});
	attacker.pokemon().switch_in(environment);
	if (focus_energy) {
		attacker.pokemon().focus_energy();
	}

	auto const ch_rate = critical_hit_probability(
		attacker.pokemon().as_const(),
		move_name,
		Ability::Honey_Gather,
		environment
	);
	return ch_rate == rate;
}

static_assert(individual_test<Generation::one>(Species::Slowpoke, MoveName::Water_Gun, Item::None, true, 1.0 / 256.0));
static_assert(individual_test<Generation::one>(Species::Tauros, MoveName::Body_Slam, Item::None, false, 55.0 / 256.0));
static_assert(individual_test<Generation::one>(Species::Electrode, MoveName::Thunderbolt, Item::None, false, 70.0 / 256.0));
static_assert(individual_test<Generation::one>(Species::Electrode, MoveName::Slash, Item::None, false, 255.0 / 256.0));
static_assert(individual_test<Generation::one>(Species::Sandslash, MoveName::Slash, Item::None, false, 255.0 / 256.0));
static_assert(individual_test<Generation::one>(Species::Goldeen, MoveName::Slash, Item::None, false, 248.0 / 256.0));
static_assert(individual_test<Generation::two>(Species::Pikachu, MoveName::Thunder, Item::None, false, 1.0 / 16.0));
static_assert(individual_test<Generation::two>(Species::Smeargle, MoveName::Slash, Item::None, false, 1.0 / 4.0));
static_assert(individual_test<Generation::two>(Species::Sandshrew, MoveName::Slash, Item::None, true, 1.0 / 3.0));
static_assert(individual_test<Generation::two>(Species::Farfetchd, MoveName::Slash, Item::Stick, true, 1.0 / 2.0));
static_assert(individual_test<Generation::three>(Species::Smeargle, MoveName::Slash, Item::None, false, 1.0 / 8.0));
static_assert(individual_test<Generation::three>(Species::Sandshrew, MoveName::Slash, Item::None, true, 1.0 / 3.0));
static_assert(individual_test<Generation::three>(Species::Farfetchd, MoveName::Slash, Item::Stick, true, 1.0 / 2.0));

} // namespace technicalmachine
