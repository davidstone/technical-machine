// Random effects of moves
// Copyright (C) 2014 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "variable.hpp"
#include "phazing_in_same_pokemon.hpp"
#include "team.hpp"
#include "move/moves.hpp"
#include "pokemon/level.hpp"

#include <bounded_integer/integer_range.hpp>

#include <algorithm>
#include <cassert>

namespace technicalmachine {
using namespace bounded::literal;

Variable::Variable():
	m_value(0_bi),
	m_probability(1_bi, 1_bi) {
}

Variable::Variable(value_type const v, Probability set_probability):
	m_value(v),
	m_probability(set_probability) {
}

auto Variable::set_phaze_index(Team const & team, Species const species) -> void {
	assert(team.size() > 1_bi);
	// This is required to work with my current battle implementation
	auto const pokemon_index = team.pokemon().has_switched() ? team.all_pokemon().replacement() : team.all_pokemon().index();
	auto const new_index = team.all_pokemon().find_index(species);
	if (new_index == pokemon_index) {
		throw PhazingInSamePokemon(new_index);
	}
	m_value = BOUNDED_CONDITIONAL(new_index < pokemon_index, new_index, new_index - 1_bi);
}

auto Variable::set_flinch(bool const set) -> void {
	m_value = BOUNDED_CONDITIONAL(set, 1_bi, 0_bi);
}

auto Variable::value() const -> value_type {
	return m_value;
}

auto Variable::probability() const -> Probability {
	return m_probability;
}

auto Variable::effect_activates() const -> bool {
	return value() != 0_bi;
}

auto Variable::phaze_index(PokemonCollection::index_type const foe_index) const -> PokemonCollection::index_type {
	return (value() < foe_index) ? PokemonCollection::index_type(value()) : PokemonCollection::index_type(value() + 1_bi);
}

auto Variable::present_heals() const -> bool {
	return value() != 0_bi;
}

auto Variable::set_magnitude(Magnitude const magnitude) -> void {
	switch (magnitude.value()) {
		case 4:
			m_value = 10_bi;
			break;
		case 5:
			m_value = 30_bi;
			break;
		case 6:
			m_value = 50_bi;
			break;
		case 7:
			m_value = 70_bi;
			break;
		case 8:
			m_value = 90_bi;
			break;
		case 9:
			m_value = 110_bi;
			break;
		default:
			// The type system ensures this is 10
			m_value = 150_bi;
			break;
	}
}

namespace {
using Probability = Variable::Probability;
Probabilities magnitude_variables();
Probabilities present_variables();
Probabilities psywave_variables();

#ifndef NDEBUG
template<typename T, unsigned depth>
class RecursiveSum {
public:
	static_assert(depth != 0, "Depth must be at least 1.");
	using type = decltype(std::declval<T>() + std::declval<typename RecursiveSum<T, depth - 1>::type>());
};
template<typename T>
class RecursiveSum<T, 1> {
public:
	using type = T;
};

constexpr auto max_different_probabilities = 7_bi;	// Magnitude
using ProbabilitySum = RecursiveSum<Probability, max_different_probabilities.value()>::type;
auto probability_sum(Probabilities const & value) -> ProbabilitySum {
	using std::begin;
	using std::end;
	using std::accumulate;
	assert(value.size() <= max_different_probabilities);
	return accumulate(begin(value), end(value), ProbabilitySum(0_bi, 1_bi), [](ProbabilitySum const probability, Variable const & variable) {
		return (probability + variable.probability()).convert<ProbabilitySum>();
	});
}
#endif


auto initial_probabilities() {
	bounded::array<Probabilities, number_of_moves.value()> activate_probability {{
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Switch0
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Switch1
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Switch2
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Switch3
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Switch4
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Switch5
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Hit self in confusion
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Pound
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Karate Chop
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// DoubleSlap
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Comet Punch
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Mega Punch
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Pay Day
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Fire Punch
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Ice Punch
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// ThunderPunch
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Scratch
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// ViceGrip
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Guillotine
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Razor Wind
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Swords Dance
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Cut
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Gust
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Wing Attack
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Whirlwind
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Fly
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Bind
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Slam
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Vine Whip
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Stomp
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Double Kick
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Mega Kick
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Jump Kick
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Rolling Kick
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Sand-Attack
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Headbutt
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Horn Attack
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Fury Attack
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Horn Drill
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Tackle
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Body Slam
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Wrap
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Take Down
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Thrash
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Double-Edge
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Tail Whip
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Poison Sting
		{ Variable(0_bi, Probability(4_bi, 5_bi)), Variable(1_bi, Probability(1_bi, 5_bi)) },		// Twineedle
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Pin Missile
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Leer
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Bite
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Growl
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Roar
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Sing
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Supersonic
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// SonicBoom
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Disable
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Acid
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Ember
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Flamethrower
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Mist
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Water Gun
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Hydro Pump
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Surf
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Ice Beam
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Blizzard
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Psybeam
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// BubbleBeam
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Aurora Beam
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Hyper Beam
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Peck
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Drill Peck
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Submission
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Low Kick
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Counter
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Seismic Toss
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Strength
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Absorb
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Mega Drain
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Leech Seed
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Growth
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Razor Leaf
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// SolarBeam
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// PoisonPowder
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Stun Spore
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Sleep Powder
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Petal Dance
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// String Shot
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Dragon Rage
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Fire Spin
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// ThunderShock
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Thunderbolt
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Thunder Wave
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Thunder
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Rock Throw
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Earthquake
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Fissure
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Dig
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Toxic
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Confusion
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Psychic
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Hypnosis
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Meditate
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Agility
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Quick Attack
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Rage
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Teleport
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Night Shade
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Mimic
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Screech
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Double Team
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Recover
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Harden
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Minimize
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// SmokeScreen
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Confuse Ray
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Withdraw
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Defense Curl
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Barrier
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Light Screen
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Haze
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Reflect
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Focus Energy
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Bide
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Metronome
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Mirror Move
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Selfdestruct
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Egg Bomb
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Lick
		{ Variable(0_bi, Probability(3_bi, 5_bi)), Variable(1_bi, Probability(2_bi, 5_bi)) },		// Smog
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Sludge
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Bone Club
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Fire Blast
		{ Variable(0_bi, Probability(4_bi, 5_bi)), Variable(1_bi, Probability(1_bi, 5_bi)) },		// Waterfall
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Clamp
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Swift
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Skull Bash
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Spike Cannon
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Constrict
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Amnesia
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Kinesis
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Softboiled
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Hi Jump Kick
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Glare
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Dream Eater
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Poison Gas
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Barrage
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Leech Life
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Lovely Kiss
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Sky Attack
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Transform
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Bubble
		{ Variable(0_bi, Probability(4_bi, 5_bi)), Variable(1_bi, Probability(1_bi, 5_bi)) },		// Dizzy Punch
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Spore
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Flash
		{ psywave_variables() },				// Psywave
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Splash
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Acid Armor
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Crabhammer
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Explosion
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Fury Swipes
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Bonemerang
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Rest
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Rock Slide
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Hyper Fang
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Sharpen
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Conversion
		{ Variable(0_bi, Probability(4_bi, 5_bi)), Variable(1_bi, Probability(1_bi, 15_bi)), Variable(2_bi, Probability(1_bi, 15_bi)), Variable(3_bi, Probability(1_bi, 15_bi)) },		// Tri Attack
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Super Fang
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Slash
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Substitute
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Struggle
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Sketch
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Triple Kick
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Thief
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Spider Web
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Mind Reader
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Nightmare
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Flame Wheel
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Snore
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Curse
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Flail
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Conversion 2
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Aeroblast
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Cotton Spore
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Reversal
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Spite
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Powder Snow
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Protect
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Mach Punch
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Scary Face
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Faint Attack
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Sweet Kiss
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Belly Drum
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Sludge Bomb
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Mud-Slap
		{ Variable(0_bi, Probability(1_bi, 2_bi)), Variable(1_bi, Probability(1_bi, 2_bi)) },		// Octazooka
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Spikes
		{ Variable(0_bi, Probability(0_bi, 1_bi)), Variable(1_bi, Probability(1_bi, 1_bi)) },		// Zap Cannon
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Foresight
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Destiny Bond
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Perish Song
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Icy Wind
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Detect
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Bone Rush
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Lock-On
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Outrage
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Sandstorm
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Giga Drain
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Endure
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Charm
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Rollout
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// False Swipe
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Swagger
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Milk Drink
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Spark
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Fury Cutter
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Steel Wing
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Mean Look
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Attract
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Sleep Talk
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Heal Bell
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Return
		{ present_variables() },				// Present
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Frustration
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Safeguard
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Pain Split
		{ Variable(0_bi, Probability(1_bi, 2_bi)), Variable(1_bi, Probability(1_bi, 2_bi)) },		// Sacred Fire
		{ magnitude_variables() },			// Magnitude
		{ Variable(0_bi, Probability(0_bi, 1_bi)), Variable(1_bi, Probability(1_bi, 1_bi)) },		// DynamicPunch
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Megahorn
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// DragonBreath
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Baton Pass
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Encore
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Pursuit
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Rapid Spin
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Sweet Scent
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Iron Tail
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Metal Claw
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Vital Throw
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Morning Sun
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Synthesis
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Moonlight
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Hidden Power
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Cross Chop
		{ Variable(0_bi, Probability(4_bi, 5_bi)), Variable(1_bi, Probability(1_bi, 5_bi)) },		// Twister
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Rain Dance
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Sunny Day
		{ Variable(0_bi, Probability(4_bi, 5_bi)), Variable(1_bi, Probability(1_bi, 5_bi)) },		// Crunch
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Mirror Coat
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Psych Up
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// ExtremeSpeed
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// AncientPower
		{ Variable(0_bi, Probability(4_bi, 5_bi)), Variable(1_bi, Probability(1_bi, 5_bi)) },		// Shadow Ball
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Future Sight
		{ Variable(0_bi, Probability(1_bi, 2_bi)), Variable(1_bi, Probability(1_bi, 2_bi)) },		// Rock Smash
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Whirlpool
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Beat Up
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Fake Out
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Uproar
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Stockpile
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Spit Up
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Swallow
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Heat Wave
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Hail
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Torment
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Flatter
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Will-O-Wisp
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Memento
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Facade
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Focus Punch
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// SmellingSalt
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Follow Me
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Nature Power
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Charge
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Taunt
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Helping Hand
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Trick
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Role Play
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Wish
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Assist
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Ingrain
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Superpower
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Magic Coat
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Recycle
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Revenge
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Brick Break
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Yawn
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Knock Off
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Endeavor
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Eruption
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Skill Swap
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Imprison
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Refresh
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Grudge
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Snatch
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Secret Power
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Dive
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Arm Thrust
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Camouflage
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Tail Glow
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Luster Purge
		{ Variable(0_bi, Probability(1_bi, 2_bi)), Variable(1_bi, Probability(1_bi, 2_bi)) },		// Mist Ball
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// FeatherDance
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Teeter Dance
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Blaze Kick
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Mud Sport
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Ice Ball
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Needle Arm
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Slack Off
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Hyper Voice
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Poison Fang
		{ Variable(0_bi, Probability(1_bi, 2_bi)), Variable(1_bi, Probability(1_bi, 2_bi)) },		// Crush Claw
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Blast Burn
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Hydro Cannon
		{ Variable(0_bi, Probability(4_bi, 5_bi)), Variable(1_bi, Probability(1_bi, 5_bi)) },		// Meteor Mash
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Astonish
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Weather Ball
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Aromatherapy
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Fake Tears
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Air Cutter
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Overheat
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Odor Sleuth
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Rock Tomb
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Silver Wind
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Metal Sound
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// GrassWhistle
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Tickle
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Cosmic Power
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Water Spout
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Signal Beam
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Shadow Punch
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Extrasensory
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Sky Uppercut
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Sand Tomb
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Sheer Cold
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Muddy Water
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Bullet Seed
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Aerial Ace
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Icicle Spear
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Iron Defense
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Block
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Howl
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Dragon Claw
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Frenzy Plant
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Bulk Up
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Bounce
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Mud Shot
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Poison Tail
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Covet
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Volt Tackle
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Magical Leaf
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Water Sport
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Calm Mind
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Leaf Blade
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Dragon Dance
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Rock Blast
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Shock Wave
		{ Variable(0_bi, Probability(4_bi, 5_bi)), Variable(1_bi, Probability(1_bi, 5_bi)) },		// Water Pulse
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Doom Desire
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Psycho Boost
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Roost
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Gravity
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Miracle Eye
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Wake-Up Slap
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Hammer Arm
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Gyro Ball
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Healing Wish
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Brine
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Natural Gift
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Feint
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Pluck
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Tailwind
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Acupressure
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Metal Burst
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// U-turn
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Close Combat
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Payback
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Assurance
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Embargo
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Fling
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Psycho Shift
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Trump Card
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Heal Block
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Wring Out
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Power Trick
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Gastro Acid
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Lucky Chant
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Me First
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Copycat
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Power Swap
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Guard Swap
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Punishment
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Last Resort
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Worry Seed
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Sucker Punch
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Toxic Spikes
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Heart Swap
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Aqua Ring
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Magnet Rise
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Flare Blitz
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Force Palm
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Aura Sphere
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Rock Polish
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Poison Jab
		{ Variable(0_bi, Probability(4_bi, 5_bi)), Variable(1_bi, Probability(1_bi, 5_bi)) },		// Dark Pulse
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Night Slash
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Aqua Tail
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Seed Bomb
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Air Slash
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// X-Scissor
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Bug Buzz
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Dragon Pulse
		{ Variable(0_bi, Probability(4_bi, 5_bi)), Variable(1_bi, Probability(1_bi, 5_bi)) },		// Dragon Rush
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Power Gem
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Drain Punch
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Vacuum Wave
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Focus Blast
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Energy Ball
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Brave Bird
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Earth Power
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Switcheroo
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Giga Impact
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Nasty Plot
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Bullet Punch
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Avalanche
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Ice Shard
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Shadow Claw
		{ Variable(0_bi, Probability(79_bi, 100_bi)), Variable(1_bi, Probability(1_bi, 10_bi)), Variable(2_bi, Probability(1_bi, 10_bi)), Variable(3_bi, Probability(1_bi, 100_bi)) },		// Thunder Fang
		{ Variable(0_bi, Probability(79_bi, 100_bi)), Variable(1_bi, Probability(1_bi, 10_bi)), Variable(2_bi, Probability(1_bi, 10_bi)), Variable(3_bi, Probability(1_bi, 100_bi)) },		// Ice Fang
		{ Variable(0_bi, Probability(79_bi, 100_bi)), Variable(1_bi, Probability(1_bi, 10_bi)), Variable(2_bi, Probability(1_bi, 10_bi)), Variable(3_bi, Probability(1_bi, 100_bi)) },		// Fire Fang
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Shadow Sneak
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Mud Bomb
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Psycho Cut
		{ Variable(0_bi, Probability(4_bi, 5_bi)), Variable(1_bi, Probability(1_bi, 5_bi)) },		// Zen Headbutt
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Mirror Shot
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Flash Cannon
		{ Variable(0_bi, Probability(4_bi, 5_bi)), Variable(1_bi, Probability(1_bi, 5_bi)) },		// Rock Climb
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Defog
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Trick Room
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Draco Meteor
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Discharge
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Lava Plume
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Leaf Storm
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Power Whip
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Rock Wrecker
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Cross Poison
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Gunk Shot
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Iron Head
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Magnet Bomb
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Stone Edge
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Captivate
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Stealth Rock
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Grass Knot
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Chatter
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Judgment
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Bug Bite
		{ Variable(0_bi, Probability(3_bi, 10_bi)), Variable(1_bi, Probability(7_bi, 10_bi)) },		// Charge Beam
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Wood Hammer
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Aqua Jet
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Attack Order
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Defend Order
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Heal Order
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Head Smash
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Double Hit
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Roar of Time
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Spacial Rend
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Lunar Dance
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Crush Grip
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Magma Storm
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Dark Void
		{ Variable(0_bi, Probability(3_bi, 5_bi)), Variable(1_bi, Probability(2_bi, 5_bi)) },		// Seed Flare
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Ominous Wind
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Shadow Force
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Hone Claws
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Wide Guard
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Guard Split
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Power Split
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Wonder Room
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Psyshock
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Venoshock
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Autotomize
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Rage Powder
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Telekinesis
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Magic Room
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Smack Down
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Storm Throw
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Flame Burst
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Sludge Wave
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Quiver Dance
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Heavy Slam
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Synchronoise
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Electro Ball
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Soak
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Flame Charge
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Coil
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Low Sweep
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Acid Spray
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Foul Play
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Simple Beam
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Entrainment
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// After You
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Round
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Echoed Voice
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Chip Away
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Clear Smog
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Stored Power
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Quick Guard
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Ally Switch
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Scald
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Shell Smash
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Heal Pulse
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Hex
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Sky Drop
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Shift Gear
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Circle Throw
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Incinerate
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Quash
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Acrobatics
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Reflect Type
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Retaliate
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Final Gambit
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Bestow
		{ Variable(0_bi, Probability(0_bi, 1_bi)), Variable(1_bi, Probability(1_bi, 1_bi)) },		// Inferno
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Water Pledge
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Fire Pledge
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Grass Pledge
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Volt Switch
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Struggle Bug
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Bulldoze
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Frost Breath
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Dragon Tail
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Work Up
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Electroweb
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Wild Charge
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Drill Run
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Dual Chop
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Heart Stamp
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Horn Leech
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Sacred Sword
		{ Variable(0_bi, Probability(1_bi, 2_bi)), Variable(1_bi, Probability(1_bi, 2_bi)) },		// Razor Shell
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Heat Crash
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Leaf Tornado
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Steamroller
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Cotton Guard
		{ Variable(0_bi, Probability(3_bi, 5_bi)), Variable(1_bi, Probability(2_bi, 5_bi)) },		// Night Daze
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Psystrike
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Tail Slap
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Hurricane
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Head Charge
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Gear Grind
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Searing Shot
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Techno Blast
		{ Variable(0_bi, Probability(9_bi, 10_bi)), Variable(1_bi, Probability(1_bi, 10_bi)) },		// Relic Song
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Secret Sword
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Glaciate
		{ Variable(0_bi, Probability(4_bi, 5_bi)), Variable(1_bi, Probability(1_bi, 5_bi)) },		// Bolt Strike
		{ Variable(0_bi, Probability(4_bi, 5_bi)), Variable(1_bi, Probability(1_bi, 5_bi)) },		// Blue Flare
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Fiery Dance
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Freeze Shock
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Ice Burn
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Snarl
		{ Variable(0_bi, Probability(7_bi, 10_bi)), Variable(1_bi, Probability(3_bi, 10_bi)) },		// Icicle Crash
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// V-create
		{ Variable(0_bi, Probability(1_bi, 1_bi)) },				// Fusion Flare
		{ Variable(0_bi, Probability(1_bi, 1_bi)) }		// Fusion Bolt
	}};
	for (auto move = static_cast<Moves>(0); move != Moves::END; move = static_cast<Moves>(static_cast<unsigned>(move) + 1)) {
		// Psywave overflows the integer counter
		if (move != Moves::Psywave) {
			assert(probability_sum(activate_probability.at(move)) == Probability(1_bi, 1_bi));
		}
	}
	return activate_probability;
}

auto magnitude_variables() -> Probabilities {
	static Probabilities const variables {
		Variable(10_bi, Probability(1_bi, 20_bi)),
		Variable(30_bi, Probability(1_bi, 10_bi)),
		Variable(50_bi, Probability(1_bi, 5_bi)),
		Variable(70_bi, Probability(3_bi, 10_bi)),
		Variable(90_bi, Probability(1_bi, 5_bi)),
		Variable(110_bi, Probability(1_bi, 10_bi)),
		Variable(150_bi, Probability(1_bi, 20_bi))
	};
	assert(probability_sum(variables) == Probability(1_bi, 1_bi));
	return variables;
}

auto present_variables() -> Probabilities {
	static constexpr auto present = bounded::make_array(
		0_bi, 40_bi, 80_bi, 120_bi
	);
	Probabilities probabilities;
	for (auto const n : present) {
		probabilities.emplace_back(n, Probability(1_bi, present.size()));
	}
	return probabilities;
}

auto psywave_variables() -> Probabilities {
	Probabilities probabilities;
	constexpr auto min = 50_bi;
	constexpr auto max = 150_bi + 1_bi;
	for (auto const n : bounded::integer_range(min, max)) {
		probabilities.emplace_back(n, Probability(1_bi, max - min));
	}
	return probabilities;
}

auto phaze_probability(TeamSize const foe_size) -> Probabilities {
	Probabilities probabilities;
	static constexpr auto large_cutoff = 2_bi;
	using LargeFoeSize = bounded::integer<large_cutoff.value(), std::numeric_limits<TeamSize>::max().value()>;
	auto const possible_replacements = BOUNDED_CONDITIONAL(foe_size > large_cutoff, LargeFoeSize(foe_size - 1_bi, bounded::non_check), 1_bi);
	for (auto const n : bounded::integer_range(possible_replacements)) {
		probabilities.emplace_back(n, Probability(1_bi, possible_replacements));
	}
	return probabilities;
}

auto acupressure_probability(ActivePokemon const & pokemon) -> Probabilities {
	static constexpr auto boostable_stats = bounded::make_array(
		StatNames::ATK,
		StatNames::DEF,
		StatNames::SPA,
		StatNames::SPD,
		StatNames::SPE,
		StatNames::ACC,
		StatNames::EVA
	);
	auto const non_maxed_stats = bounded::count_if(std::begin(boostable_stats), std::end(boostable_stats), [&](StatNames const stat) {
		return pokemon.stage()[stat] != 6_bi;
	});
	if (non_maxed_stats == 0_bi) {
		// Arbitrary value so the code doesn't break when Acupressure can no
		// longer work
		return Probabilities({ Variable(0_bi, Probability(1_bi, 1_bi)) });
	}
	Probability const probability(1_bi, bounded::max(1_bi, non_maxed_stats));
	Probabilities probabilities;
	for (auto const stat : boostable_stats) {
		probabilities.emplace_back(static_cast<Variable::value_type>(stat), probability);
	}
	return probabilities;
}

}	// namespace

auto all_probabilities(ActivePokemon const & pokemon, TeamSize const foe_size) -> Probabilities {
	static auto const probability = initial_probabilities();
	Moves const move = current_move(pokemon);
	if (is_phaze(move)) {
		return phaze_probability(foe_size);
	}
	else if (move == Moves::Acupressure) {
		return acupressure_probability(pokemon);
	}
	else {
		return probability.at(move);
	}
}
}	// namespace technicalmachine
