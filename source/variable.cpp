// Random effects of moves
// Copyright (C) 2015 David Stone
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

#include <containers/algorithms/count.hpp>
#include <containers/array/make_array.hpp>

#include <algorithm>
#include <cassert>

namespace technicalmachine {
using namespace bounded::literal;

Variable::Variable():
	m_value(0_bi),
	m_probability(1.0) {
}

Variable::Variable(value_type const v, Probability const set_probability):
	m_value(v),
	m_probability(set_probability) {
}

auto Variable::set_phaze_index(Team const & team, Species const species) -> void {
	assert(team.size() > 1_bi);
	// This is required to work with my current battle implementation
	auto const pokemon_index = has_switched(team.pokemon()) ? team.all_pokemon().replacement() : team.all_pokemon().index();
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

constexpr bool almost_equal(double const lhs, double const rhs) {
	return lhs - std::numeric_limits<double>::epsilon() <= rhs or rhs <= lhs + std::numeric_limits<double>::epsilon();
}

auto probability_sum(Probabilities const & value) -> double {
	return std::accumulate(value.begin(), value.end(), 0.0, [](auto const probability, Variable const & variable) {
		return probability + variable.probability();
	});
}

auto magnitude_variables() {
	static Probabilities const variables {
		Variable(10_bi, 0.05),
		Variable(30_bi, 0.1),
		Variable(50_bi, 0.2),
		Variable(70_bi, 0.3),
		Variable(90_bi, 0.2),
		Variable(110_bi, 0.1),
		Variable(150_bi, 0.05)
	};
	assert(almost_equal(probability_sum(variables), 1.0));
	return variables;
}

auto present_variables() {
	static constexpr auto present = containers::make_array(
		0_bi, 40_bi, 80_bi, 120_bi
	);
	Probabilities probabilities;
	for (auto const n : present) {
		probabilities.emplace_back(n, 1.0 / static_cast<double>(size(present)));
	}
	return probabilities;
}

auto psywave_variables() {
	Probabilities probabilities;
	constexpr auto min = 50_bi;
	constexpr auto max = 150_bi + 1_bi;
	for (auto const n : bounded::integer_range(min, max)) {
		probabilities.emplace_back(n, 1.0 / static_cast<double>(max - min));
	}
	return probabilities;
}

auto initial_probabilities() {
	containers::array<Probabilities, static_cast<std::size_t>(Moves::END)> activate_probability {{
		{ Variable(0_bi, 1.0) },				// Switch0
		{ Variable(0_bi, 1.0) },				// Switch1
		{ Variable(0_bi, 1.0) },				// Switch2
		{ Variable(0_bi, 1.0) },				// Switch3
		{ Variable(0_bi, 1.0) },				// Switch4
		{ Variable(0_bi, 1.0) },				// Switch5
		{ Variable(0_bi, 1.0) },				// Hit self in confusion
		{ Variable(0_bi, 1.0) },				// Pound
		{ Variable(0_bi, 1.0) },				// Karate Chop
		{ Variable(0_bi, 1.0) },				// DoubleSlap
		{ Variable(0_bi, 1.0) },				// Comet Punch
		{ Variable(0_bi, 1.0) },				// Mega Punch
		{ Variable(0_bi, 1.0) },				// Pay Day
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Fire Punch
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Ice Punch
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// ThunderPunch
		{ Variable(0_bi, 1.0) },				// Scratch
		{ Variable(0_bi, 1.0) },				// ViceGrip
		{ Variable(0_bi, 1.0) },				// Guillotine
		{ Variable(0_bi, 1.0) },				// Razor Wind
		{ Variable(0_bi, 1.0) },				// Swords Dance
		{ Variable(0_bi, 1.0) },				// Cut
		{ Variable(0_bi, 1.0) },				// Gust
		{ Variable(0_bi, 1.0) },				// Wing Attack
		{ Variable(0_bi, 1.0) },				// Whirlwind
		{ Variable(0_bi, 1.0) },				// Fly
		{ Variable(0_bi, 1.0) },				// Bind
		{ Variable(0_bi, 1.0) },				// Slam
		{ Variable(0_bi, 1.0) },				// Vine Whip
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Stomp
		{ Variable(0_bi, 1.0) },				// Double Kick
		{ Variable(0_bi, 1.0) },				// Mega Kick
		{ Variable(0_bi, 1.0) },				// Jump Kick
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Rolling Kick
		{ Variable(0_bi, 1.0) },				// Sand-Attack
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Headbutt
		{ Variable(0_bi, 1.0) },				// Horn Attack
		{ Variable(0_bi, 1.0) },				// Fury Attack
		{ Variable(0_bi, 1.0) },				// Horn Drill
		{ Variable(0_bi, 1.0) },				// Tackle
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Body Slam
		{ Variable(0_bi, 1.0) },				// Wrap
		{ Variable(0_bi, 1.0) },				// Take Down
		{ Variable(0_bi, 1.0) },				// Thrash
		{ Variable(0_bi, 1.0) },				// Double-Edge
		{ Variable(0_bi, 1.0) },				// Tail Whip
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Poison Sting
		{ Variable(0_bi, 0.8), Variable(1_bi, 0.2) },		// Twineedle
		{ Variable(0_bi, 1.0) },				// Pin Missile
		{ Variable(0_bi, 1.0) },				// Leer
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Bite
		{ Variable(0_bi, 1.0) },				// Growl
		{ Variable(0_bi, 1.0) },				// Roar
		{ Variable(0_bi, 1.0) },				// Sing
		{ Variable(0_bi, 1.0) },				// Supersonic
		{ Variable(0_bi, 1.0) },				// SonicBoom
		{ Variable(0_bi, 1.0) },				// Disable
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Acid
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Ember
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Flamethrower
		{ Variable(0_bi, 1.0) },				// Mist
		{ Variable(0_bi, 1.0) },				// Water Gun
		{ Variable(0_bi, 1.0) },				// Hydro Pump
		{ Variable(0_bi, 1.0) },				// Surf
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Ice Beam
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Blizzard
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Psybeam
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// BubbleBeam
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Aurora Beam
		{ Variable(0_bi, 1.0) },				// Hyper Beam
		{ Variable(0_bi, 1.0) },				// Peck
		{ Variable(0_bi, 1.0) },				// Drill Peck
		{ Variable(0_bi, 1.0) },				// Submission
		{ Variable(0_bi, 1.0) },				// Low Kick
		{ Variable(0_bi, 1.0) },				// Counter
		{ Variable(0_bi, 1.0) },				// Seismic Toss
		{ Variable(0_bi, 1.0) },				// Strength
		{ Variable(0_bi, 1.0) },				// Absorb
		{ Variable(0_bi, 1.0) },				// Mega Drain
		{ Variable(0_bi, 1.0) },				// Leech Seed
		{ Variable(0_bi, 1.0) },				// Growth
		{ Variable(0_bi, 1.0) },				// Razor Leaf
		{ Variable(0_bi, 1.0) },				// SolarBeam
		{ Variable(0_bi, 1.0) },				// PoisonPowder
		{ Variable(0_bi, 1.0) },				// Stun Spore
		{ Variable(0_bi, 1.0) },				// Sleep Powder
		{ Variable(0_bi, 1.0) },				// Petal Dance
		{ Variable(0_bi, 1.0) },				// String Shot
		{ Variable(0_bi, 1.0) },				// Dragon Rage
		{ Variable(0_bi, 1.0) },				// Fire Spin
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// ThunderShock
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Thunderbolt
		{ Variable(0_bi, 1.0) },				// Thunder Wave
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Thunder
		{ Variable(0_bi, 1.0) },				// Rock Throw
		{ Variable(0_bi, 1.0) },				// Earthquake
		{ Variable(0_bi, 1.0) },				// Fissure
		{ Variable(0_bi, 1.0) },				// Dig
		{ Variable(0_bi, 1.0) },				// Toxic
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Confusion
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Psychic
		{ Variable(0_bi, 1.0) },				// Hypnosis
		{ Variable(0_bi, 1.0) },				// Meditate
		{ Variable(0_bi, 1.0) },				// Agility
		{ Variable(0_bi, 1.0) },				// Quick Attack
		{ Variable(0_bi, 1.0) },				// Rage
		{ Variable(0_bi, 1.0) },				// Teleport
		{ Variable(0_bi, 1.0) },				// Night Shade
		{ Variable(0_bi, 1.0) },				// Mimic
		{ Variable(0_bi, 1.0) },				// Screech
		{ Variable(0_bi, 1.0) },				// Double Team
		{ Variable(0_bi, 1.0) },				// Recover
		{ Variable(0_bi, 1.0) },				// Harden
		{ Variable(0_bi, 1.0) },				// Minimize
		{ Variable(0_bi, 1.0) },				// SmokeScreen
		{ Variable(0_bi, 1.0) },				// Confuse Ray
		{ Variable(0_bi, 1.0) },				// Withdraw
		{ Variable(0_bi, 1.0) },				// Defense Curl
		{ Variable(0_bi, 1.0) },				// Barrier
		{ Variable(0_bi, 1.0) },				// Light Screen
		{ Variable(0_bi, 1.0) },				// Haze
		{ Variable(0_bi, 1.0) },				// Reflect
		{ Variable(0_bi, 1.0) },				// Focus Energy
		{ Variable(0_bi, 1.0) },				// Bide
		{ Variable(0_bi, 1.0) },				// Metronome
		{ Variable(0_bi, 1.0) },				// Mirror Move
		{ Variable(0_bi, 1.0) },				// Selfdestruct
		{ Variable(0_bi, 1.0) },				// Egg Bomb
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Lick
		{ Variable(0_bi, 0.6), Variable(1_bi, 0.4) },		// Smog
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Sludge
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Bone Club
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Fire Blast
		{ Variable(0_bi, 0.8), Variable(1_bi, 0.2) },		// Waterfall
		{ Variable(0_bi, 1.0) },				// Clamp
		{ Variable(0_bi, 1.0) },				// Swift
		{ Variable(0_bi, 1.0) },				// Skull Bash
		{ Variable(0_bi, 1.0) },				// Spike Cannon
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Constrict
		{ Variable(0_bi, 1.0) },				// Amnesia
		{ Variable(0_bi, 1.0) },				// Kinesis
		{ Variable(0_bi, 1.0) },				// Softboiled
		{ Variable(0_bi, 1.0) },				// Hi Jump Kick
		{ Variable(0_bi, 1.0) },				// Glare
		{ Variable(0_bi, 1.0) },				// Dream Eater
		{ Variable(0_bi, 1.0) },				// Poison Gas
		{ Variable(0_bi, 1.0) },				// Barrage
		{ Variable(0_bi, 1.0) },				// Leech Life
		{ Variable(0_bi, 1.0) },				// Lovely Kiss
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Sky Attack
		{ Variable(0_bi, 1.0) },				// Transform
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Bubble
		{ Variable(0_bi, 0.8), Variable(1_bi, 0.2) },		// Dizzy Punch
		{ Variable(0_bi, 1.0) },				// Spore
		{ Variable(0_bi, 1.0) },				// Flash
		{ psywave_variables() },				// Psywave
		{ Variable(0_bi, 1.0) },				// Splash
		{ Variable(0_bi, 1.0) },				// Acid Armor
		{ Variable(0_bi, 1.0) },				// Crabhammer
		{ Variable(0_bi, 1.0) },				// Explosion
		{ Variable(0_bi, 1.0) },				// Fury Swipes
		{ Variable(0_bi, 1.0) },				// Bonemerang
		{ Variable(0_bi, 1.0) },				// Rest
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Rock Slide
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Hyper Fang
		{ Variable(0_bi, 1.0) },				// Sharpen
		{ Variable(0_bi, 1.0) },				// Conversion
		{ Variable(0_bi, 0.8), Variable(1_bi, 1.0 / 15.0), Variable(2_bi, 1.0 / 15.0), Variable(3_bi, 1.0 / 15.0) },		// Tri Attack
		{ Variable(0_bi, 1.0) },				// Super Fang
		{ Variable(0_bi, 1.0) },				// Slash
		{ Variable(0_bi, 1.0) },				// Substitute
		{ Variable(0_bi, 1.0) },				// Struggle
		{ Variable(0_bi, 1.0) },				// Sketch
		{ Variable(0_bi, 1.0) },				// Triple Kick
		{ Variable(0_bi, 1.0) },				// Thief
		{ Variable(0_bi, 1.0) },				// Spider Web
		{ Variable(0_bi, 1.0) },				// Mind Reader
		{ Variable(0_bi, 1.0) },				// Nightmare
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Flame Wheel
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Snore
		{ Variable(0_bi, 1.0) },				// Curse
		{ Variable(0_bi, 1.0) },				// Flail
		{ Variable(0_bi, 1.0) },				// Conversion 2
		{ Variable(0_bi, 1.0) },				// Aeroblast
		{ Variable(0_bi, 1.0) },				// Cotton Spore
		{ Variable(0_bi, 1.0) },				// Reversal
		{ Variable(0_bi, 1.0) },				// Spite
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Powder Snow
		{ Variable(0_bi, 1.0) },				// Protect
		{ Variable(0_bi, 1.0) },				// Mach Punch
		{ Variable(0_bi, 1.0) },				// Scary Face
		{ Variable(0_bi, 1.0) },				// Faint Attack
		{ Variable(0_bi, 1.0) },				// Sweet Kiss
		{ Variable(0_bi, 1.0) },				// Belly Drum
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Sludge Bomb
		{ Variable(0_bi, 1.0) },				// Mud-Slap
		{ Variable(0_bi, 0.5), Variable(1_bi, 0.5) },		// Octazooka
		{ Variable(0_bi, 1.0) },				// Spikes
		{ Variable(0_bi, 0.0), Variable(1_bi, 1.0) },		// Zap Cannon
		{ Variable(0_bi, 1.0) },				// Foresight
		{ Variable(0_bi, 1.0) },				// Destiny Bond
		{ Variable(0_bi, 1.0) },				// Perish Song
		{ Variable(0_bi, 1.0) },				// Icy Wind
		{ Variable(0_bi, 1.0) },				// Detect
		{ Variable(0_bi, 1.0) },				// Bone Rush
		{ Variable(0_bi, 1.0) },				// Lock-On
		{ Variable(0_bi, 1.0) },				// Outrage
		{ Variable(0_bi, 1.0) },				// Sandstorm
		{ Variable(0_bi, 1.0) },				// Giga Drain
		{ Variable(0_bi, 1.0) },				// Endure
		{ Variable(0_bi, 1.0) },				// Charm
		{ Variable(0_bi, 1.0) },				// Rollout
		{ Variable(0_bi, 1.0) },				// False Swipe
		{ Variable(0_bi, 1.0) },				// Swagger
		{ Variable(0_bi, 1.0) },				// Milk Drink
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Spark
		{ Variable(0_bi, 1.0) },				// Fury Cutter
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Steel Wing
		{ Variable(0_bi, 1.0) },				// Mean Look
		{ Variable(0_bi, 1.0) },				// Attract
		{ Variable(0_bi, 1.0) },				// Sleep Talk
		{ Variable(0_bi, 1.0) },				// Heal Bell
		{ Variable(0_bi, 1.0) },				// Return
		{ present_variables() },				// Present
		{ Variable(0_bi, 1.0) },				// Frustration
		{ Variable(0_bi, 1.0) },				// Safeguard
		{ Variable(0_bi, 1.0) },				// Pain Split
		{ Variable(0_bi, 0.5), Variable(1_bi, 0.5) },		// Sacred Fire
		{ magnitude_variables() },			// Magnitude
		{ Variable(0_bi, 0.0), Variable(1_bi, 1.0) },		// DynamicPunch
		{ Variable(0_bi, 1.0) },				// Megahorn
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// DragonBreath
		{ Variable(0_bi, 1.0) },				// Baton Pass
		{ Variable(0_bi, 1.0) },				// Encore
		{ Variable(0_bi, 1.0) },				// Pursuit
		{ Variable(0_bi, 1.0) },				// Rapid Spin
		{ Variable(0_bi, 1.0) },				// Sweet Scent
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Iron Tail
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Metal Claw
		{ Variable(0_bi, 1.0) },				// Vital Throw
		{ Variable(0_bi, 1.0) },				// Morning Sun
		{ Variable(0_bi, 1.0) },				// Synthesis
		{ Variable(0_bi, 1.0) },				// Moonlight
		{ Variable(0_bi, 1.0) },				// Hidden Power
		{ Variable(0_bi, 1.0) },				// Cross Chop
		{ Variable(0_bi, 0.8), Variable(1_bi, 0.2) },		// Twister
		{ Variable(0_bi, 1.0) },				// Rain Dance
		{ Variable(0_bi, 1.0) },				// Sunny Day
		{ Variable(0_bi, 0.8), Variable(1_bi, 0.2) },		// Crunch
		{ Variable(0_bi, 1.0) },				// Mirror Coat
		{ Variable(0_bi, 1.0) },				// Psych Up
		{ Variable(0_bi, 1.0) },				// ExtremeSpeed
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// AncientPower
		{ Variable(0_bi, 0.8), Variable(1_bi, 0.2) },		// Shadow Ball
		{ Variable(0_bi, 1.0) },				// Future Sight
		{ Variable(0_bi, 0.5), Variable(1_bi, 0.5) },		// Rock Smash
		{ Variable(0_bi, 1.0) },				// Whirlpool
		{ Variable(0_bi, 1.0) },				// Beat Up
		{ Variable(0_bi, 1.0) },				// Fake Out
		{ Variable(0_bi, 1.0) },				// Uproar
		{ Variable(0_bi, 1.0) },				// Stockpile
		{ Variable(0_bi, 1.0) },				// Spit Up
		{ Variable(0_bi, 1.0) },				// Swallow
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Heat Wave
		{ Variable(0_bi, 1.0) },				// Hail
		{ Variable(0_bi, 1.0) },				// Torment
		{ Variable(0_bi, 1.0) },				// Flatter
		{ Variable(0_bi, 1.0) },				// Will-O-Wisp
		{ Variable(0_bi, 1.0) },				// Memento
		{ Variable(0_bi, 1.0) },				// Facade
		{ Variable(0_bi, 1.0) },				// Focus Punch
		{ Variable(0_bi, 1.0) },				// SmellingSalt
		{ Variable(0_bi, 1.0) },				// Follow Me
		{ Variable(0_bi, 1.0) },				// Nature Power
		{ Variable(0_bi, 1.0) },				// Charge
		{ Variable(0_bi, 1.0) },				// Taunt
		{ Variable(0_bi, 1.0) },				// Helping Hand
		{ Variable(0_bi, 1.0) },				// Trick
		{ Variable(0_bi, 1.0) },				// Role Play
		{ Variable(0_bi, 1.0) },				// Wish
		{ Variable(0_bi, 1.0) },				// Assist
		{ Variable(0_bi, 1.0) },				// Ingrain
		{ Variable(0_bi, 1.0) },				// Superpower
		{ Variable(0_bi, 1.0) },				// Magic Coat
		{ Variable(0_bi, 1.0) },				// Recycle
		{ Variable(0_bi, 1.0) },				// Revenge
		{ Variable(0_bi, 1.0) },				// Brick Break
		{ Variable(0_bi, 1.0) },				// Yawn
		{ Variable(0_bi, 1.0) },				// Knock Off
		{ Variable(0_bi, 1.0) },				// Endeavor
		{ Variable(0_bi, 1.0) },				// Eruption
		{ Variable(0_bi, 1.0) },				// Skill Swap
		{ Variable(0_bi, 1.0) },				// Imprison
		{ Variable(0_bi, 1.0) },				// Refresh
		{ Variable(0_bi, 1.0) },				// Grudge
		{ Variable(0_bi, 1.0) },				// Snatch
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Secret Power
		{ Variable(0_bi, 1.0) },				// Dive
		{ Variable(0_bi, 1.0) },				// Arm Thrust
		{ Variable(0_bi, 1.0) },				// Camouflage
		{ Variable(0_bi, 1.0) },				// Tail Glow
		{ Variable(0_bi, 1.0) },				// Luster Purge
		{ Variable(0_bi, 0.5), Variable(1_bi, 0.5) },		// Mist Ball
		{ Variable(0_bi, 1.0) },				// FeatherDance
		{ Variable(0_bi, 1.0) },				// Teeter Dance
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Blaze Kick
		{ Variable(0_bi, 1.0) },				// Mud Sport
		{ Variable(0_bi, 1.0) },				// Ice Ball
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Needle Arm
		{ Variable(0_bi, 1.0) },				// Slack Off
		{ Variable(0_bi, 1.0) },				// Hyper Voice
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Poison Fang
		{ Variable(0_bi, 0.5), Variable(1_bi, 0.5) },		// Crush Claw
		{ Variable(0_bi, 1.0) },				// Blast Burn
		{ Variable(0_bi, 1.0) },				// Hydro Cannon
		{ Variable(0_bi, 0.8), Variable(1_bi, 0.2) },		// Meteor Mash
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Astonish
		{ Variable(0_bi, 1.0) },				// Weather Ball
		{ Variable(0_bi, 1.0) },				// Aromatherapy
		{ Variable(0_bi, 1.0) },				// Fake Tears
		{ Variable(0_bi, 1.0) },				// Air Cutter
		{ Variable(0_bi, 1.0) },				// Overheat
		{ Variable(0_bi, 1.0) },				// Odor Sleuth
		{ Variable(0_bi, 1.0) },				// Rock Tomb
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Silver Wind
		{ Variable(0_bi, 1.0) },				// Metal Sound
		{ Variable(0_bi, 1.0) },				// GrassWhistle
		{ Variable(0_bi, 1.0) },				// Tickle
		{ Variable(0_bi, 1.0) },				// Cosmic Power
		{ Variable(0_bi, 1.0) },				// Water Spout
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Signal Beam
		{ Variable(0_bi, 1.0) },				// Shadow Punch
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Extrasensory
		{ Variable(0_bi, 1.0) },				// Sky Uppercut
		{ Variable(0_bi, 1.0) },				// Sand Tomb
		{ Variable(0_bi, 1.0) },				// Sheer Cold
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Muddy Water
		{ Variable(0_bi, 1.0) },				// Bullet Seed
		{ Variable(0_bi, 1.0) },				// Aerial Ace
		{ Variable(0_bi, 1.0) },				// Icicle Spear
		{ Variable(0_bi, 1.0) },				// Iron Defense
		{ Variable(0_bi, 1.0) },				// Block
		{ Variable(0_bi, 1.0) },				// Howl
		{ Variable(0_bi, 1.0) },				// Dragon Claw
		{ Variable(0_bi, 1.0) },				// Frenzy Plant
		{ Variable(0_bi, 1.0) },				// Bulk Up
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Bounce
		{ Variable(0_bi, 1.0) },				// Mud Shot
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Poison Tail
		{ Variable(0_bi, 1.0) },				// Covet
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Volt Tackle
		{ Variable(0_bi, 1.0) },				// Magical Leaf
		{ Variable(0_bi, 1.0) },				// Water Sport
		{ Variable(0_bi, 1.0) },				// Calm Mind
		{ Variable(0_bi, 1.0) },				// Leaf Blade
		{ Variable(0_bi, 1.0) },				// Dragon Dance
		{ Variable(0_bi, 1.0) },				// Rock Blast
		{ Variable(0_bi, 1.0) },				// Shock Wave
		{ Variable(0_bi, 0.8), Variable(1_bi, 0.2) },		// Water Pulse
		{ Variable(0_bi, 1.0) },				// Doom Desire
		{ Variable(0_bi, 1.0) },				// Psycho Boost
		{ Variable(0_bi, 1.0) },				// Roost
		{ Variable(0_bi, 1.0) },				// Gravity
		{ Variable(0_bi, 1.0) },				// Miracle Eye
		{ Variable(0_bi, 1.0) },				// Wake-Up Slap
		{ Variable(0_bi, 1.0) },				// Hammer Arm
		{ Variable(0_bi, 1.0) },				// Gyro Ball
		{ Variable(0_bi, 1.0) },				// Healing Wish
		{ Variable(0_bi, 1.0) },				// Brine
		{ Variable(0_bi, 1.0) },				// Natural Gift
		{ Variable(0_bi, 1.0) },				// Feint
		{ Variable(0_bi, 1.0) },				// Pluck
		{ Variable(0_bi, 1.0) },				// Tailwind
		{ Variable(0_bi, 1.0) },				// Acupressure
		{ Variable(0_bi, 1.0) },				// Metal Burst
		{ Variable(0_bi, 1.0) },				// U-turn
		{ Variable(0_bi, 1.0) },				// Close Combat
		{ Variable(0_bi, 1.0) },				// Payback
		{ Variable(0_bi, 1.0) },				// Assurance
		{ Variable(0_bi, 1.0) },				// Embargo
		{ Variable(0_bi, 1.0) },				// Fling
		{ Variable(0_bi, 1.0) },				// Psycho Shift
		{ Variable(0_bi, 1.0) },				// Trump Card
		{ Variable(0_bi, 1.0) },				// Heal Block
		{ Variable(0_bi, 1.0) },				// Wring Out
		{ Variable(0_bi, 1.0) },				// Power Trick
		{ Variable(0_bi, 1.0) },				// Gastro Acid
		{ Variable(0_bi, 1.0) },				// Lucky Chant
		{ Variable(0_bi, 1.0) },				// Me First
		{ Variable(0_bi, 1.0) },				// Copycat
		{ Variable(0_bi, 1.0) },				// Power Swap
		{ Variable(0_bi, 1.0) },				// Guard Swap
		{ Variable(0_bi, 1.0) },				// Punishment
		{ Variable(0_bi, 1.0) },				// Last Resort
		{ Variable(0_bi, 1.0) },				// Worry Seed
		{ Variable(0_bi, 1.0) },				// Sucker Punch
		{ Variable(0_bi, 1.0) },				// Toxic Spikes
		{ Variable(0_bi, 1.0) },				// Heart Swap
		{ Variable(0_bi, 1.0) },				// Aqua Ring
		{ Variable(0_bi, 1.0) },				// Magnet Rise
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Flare Blitz
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Force Palm
		{ Variable(0_bi, 1.0) },				// Aura Sphere
		{ Variable(0_bi, 1.0) },				// Rock Polish
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Poison Jab
		{ Variable(0_bi, 0.8), Variable(1_bi, 0.2) },		// Dark Pulse
		{ Variable(0_bi, 1.0) },				// Night Slash
		{ Variable(0_bi, 1.0) },				// Aqua Tail
		{ Variable(0_bi, 1.0) },				// Seed Bomb
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Air Slash
		{ Variable(0_bi, 1.0) },				// X-Scissor
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Bug Buzz
		{ Variable(0_bi, 1.0) },				// Dragon Pulse
		{ Variable(0_bi, 0.8), Variable(1_bi, 0.2) },		// Dragon Rush
		{ Variable(0_bi, 1.0) },				// Power Gem
		{ Variable(0_bi, 1.0) },				// Drain Punch
		{ Variable(0_bi, 1.0) },				// Vacuum Wave
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Focus Blast
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Energy Ball
		{ Variable(0_bi, 1.0) },				// Brave Bird
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Earth Power
		{ Variable(0_bi, 1.0) },				// Switcheroo
		{ Variable(0_bi, 1.0) },				// Giga Impact
		{ Variable(0_bi, 1.0) },				// Nasty Plot
		{ Variable(0_bi, 1.0) },				// Bullet Punch
		{ Variable(0_bi, 1.0) },				// Avalanche
		{ Variable(0_bi, 1.0) },				// Ice Shard
		{ Variable(0_bi, 1.0) },				// Shadow Claw
		{ Variable(0_bi, 0.79), Variable(1_bi, 0.1), Variable(2_bi, 0.1), Variable(3_bi, 0.01) },		// Thunder Fang
		{ Variable(0_bi, 0.79), Variable(1_bi, 0.1), Variable(2_bi, 0.1), Variable(3_bi, 0.01) },		// Ice Fang
		{ Variable(0_bi, 0.79), Variable(1_bi, 0.1), Variable(2_bi, 0.1), Variable(3_bi, 0.01) },		// Fire Fang
		{ Variable(0_bi, 1.0) },				// Shadow Sneak
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Mud Bomb
		{ Variable(0_bi, 1.0) },				// Psycho Cut
		{ Variable(0_bi, 0.8), Variable(1_bi, 0.2) },		// Zen Headbutt
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Mirror Shot
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Flash Cannon
		{ Variable(0_bi, 0.8), Variable(1_bi, 0.2) },		// Rock Climb
		{ Variable(0_bi, 1.0) },				// Defog
		{ Variable(0_bi, 1.0) },				// Trick Room
		{ Variable(0_bi, 1.0) },				// Draco Meteor
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Discharge
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Lava Plume
		{ Variable(0_bi, 1.0) },				// Leaf Storm
		{ Variable(0_bi, 1.0) },				// Power Whip
		{ Variable(0_bi, 1.0) },				// Rock Wrecker
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Cross Poison
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Gunk Shot
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Iron Head
		{ Variable(0_bi, 1.0) },				// Magnet Bomb
		{ Variable(0_bi, 1.0) },				// Stone Edge
		{ Variable(0_bi, 1.0) },				// Captivate
		{ Variable(0_bi, 1.0) },				// Stealth Rock
		{ Variable(0_bi, 1.0) },				// Grass Knot
		{ Variable(0_bi, 1.0) },				// Chatter
		{ Variable(0_bi, 1.0) },				// Judgment
		{ Variable(0_bi, 1.0) },				// Bug Bite
		{ Variable(0_bi, 0.3), Variable(1_bi, 0.7) },		// Charge Beam
		{ Variable(0_bi, 1.0) },				// Wood Hammer
		{ Variable(0_bi, 1.0) },				// Aqua Jet
		{ Variable(0_bi, 1.0) },				// Attack Order
		{ Variable(0_bi, 1.0) },				// Defend Order
		{ Variable(0_bi, 1.0) },				// Heal Order
		{ Variable(0_bi, 1.0) },				// Head Smash
		{ Variable(0_bi, 1.0) },				// Double Hit
		{ Variable(0_bi, 1.0) },				// Roar of Time
		{ Variable(0_bi, 1.0) },				// Spacial Rend
		{ Variable(0_bi, 1.0) },				// Lunar Dance
		{ Variable(0_bi, 1.0) },				// Crush Grip
		{ Variable(0_bi, 1.0) },				// Magma Storm
		{ Variable(0_bi, 1.0) },				// Dark Void
		{ Variable(0_bi, 0.6), Variable(1_bi, 0.4) },		// Seed Flare
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Ominous Wind
		{ Variable(0_bi, 1.0) },				// Shadow Force
		{ Variable(0_bi, 1.0) },				// Hone Claws
		{ Variable(0_bi, 1.0) },				// Wide Guard
		{ Variable(0_bi, 1.0) },				// Guard Split
		{ Variable(0_bi, 1.0) },				// Power Split
		{ Variable(0_bi, 1.0) },				// Wonder Room
		{ Variable(0_bi, 1.0) },				// Psyshock
		{ Variable(0_bi, 1.0) },				// Venoshock
		{ Variable(0_bi, 1.0) },				// Autotomize
		{ Variable(0_bi, 1.0) },				// Rage Powder
		{ Variable(0_bi, 1.0) },				// Telekinesis
		{ Variable(0_bi, 1.0) },				// Magic Room
		{ Variable(0_bi, 1.0) },				// Smack Down
		{ Variable(0_bi, 1.0) },				// Storm Throw
		{ Variable(0_bi, 1.0) },				// Flame Burst
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Sludge Wave
		{ Variable(0_bi, 1.0) },				// Quiver Dance
		{ Variable(0_bi, 1.0) },				// Heavy Slam
		{ Variable(0_bi, 1.0) },				// Synchronoise
		{ Variable(0_bi, 1.0) },				// Electro Ball
		{ Variable(0_bi, 1.0) },				// Soak
		{ Variable(0_bi, 1.0) },				// Flame Charge
		{ Variable(0_bi, 1.0) },				// Coil
		{ Variable(0_bi, 1.0) },				// Low Sweep
		{ Variable(0_bi, 1.0) },				// Acid Spray
		{ Variable(0_bi, 1.0) },				// Foul Play
		{ Variable(0_bi, 1.0) },				// Simple Beam
		{ Variable(0_bi, 1.0) },				// Entrainment
		{ Variable(0_bi, 1.0) },				// After You
		{ Variable(0_bi, 1.0) },				// Round
		{ Variable(0_bi, 1.0) },				// Echoed Voice
		{ Variable(0_bi, 1.0) },				// Chip Away
		{ Variable(0_bi, 1.0) },				// Clear Smog
		{ Variable(0_bi, 1.0) },				// Stored Power
		{ Variable(0_bi, 1.0) },				// Quick Guard
		{ Variable(0_bi, 1.0) },				// Ally Switch
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Scald
		{ Variable(0_bi, 1.0) },				// Shell Smash
		{ Variable(0_bi, 1.0) },				// Heal Pulse
		{ Variable(0_bi, 1.0) },				// Hex
		{ Variable(0_bi, 1.0) },				// Sky Drop
		{ Variable(0_bi, 1.0) },				// Shift Gear
		{ Variable(0_bi, 1.0) },				// Circle Throw
		{ Variable(0_bi, 1.0) },				// Incinerate
		{ Variable(0_bi, 1.0) },				// Quash
		{ Variable(0_bi, 1.0) },				// Acrobatics
		{ Variable(0_bi, 1.0) },				// Reflect Type
		{ Variable(0_bi, 1.0) },				// Retaliate
		{ Variable(0_bi, 1.0) },				// Final Gambit
		{ Variable(0_bi, 1.0) },				// Bestow
		{ Variable(0_bi, 0.0), Variable(1_bi, 1.0) },		// Inferno
		{ Variable(0_bi, 1.0) },				// Water Pledge
		{ Variable(0_bi, 1.0) },				// Fire Pledge
		{ Variable(0_bi, 1.0) },				// Grass Pledge
		{ Variable(0_bi, 1.0) },				// Volt Switch
		{ Variable(0_bi, 1.0) },				// Struggle Bug
		{ Variable(0_bi, 1.0) },				// Bulldoze
		{ Variable(0_bi, 1.0) },				// Frost Breath
		{ Variable(0_bi, 1.0) },				// Dragon Tail
		{ Variable(0_bi, 1.0) },				// Work Up
		{ Variable(0_bi, 1.0) },				// Electroweb
		{ Variable(0_bi, 1.0) },				// Wild Charge
		{ Variable(0_bi, 1.0) },				// Drill Run
		{ Variable(0_bi, 1.0) },				// Dual Chop
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Heart Stamp
		{ Variable(0_bi, 1.0) },				// Horn Leech
		{ Variable(0_bi, 1.0) },				// Sacred Sword
		{ Variable(0_bi, 0.5), Variable(1_bi, 0.5) },		// Razor Shell
		{ Variable(0_bi, 1.0) },				// Heat Crash
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Leaf Tornado
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Steamroller
		{ Variable(0_bi, 1.0) },				// Cotton Guard
		{ Variable(0_bi, 0.6), Variable(1_bi, 0.4) },		// Night Daze
		{ Variable(0_bi, 1.0) },				// Psystrike
		{ Variable(0_bi, 1.0) },				// Tail Slap
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Hurricane
		{ Variable(0_bi, 1.0) },				// Head Charge
		{ Variable(0_bi, 1.0) },				// Gear Grind
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Searing Shot
		{ Variable(0_bi, 1.0) },				// Techno Blast
		{ Variable(0_bi, 0.9), Variable(1_bi, 0.1) },		// Relic Song
		{ Variable(0_bi, 1.0) },				// Secret Sword
		{ Variable(0_bi, 1.0) },				// Glaciate
		{ Variable(0_bi, 0.8), Variable(1_bi, 0.2) },		// Bolt Strike
		{ Variable(0_bi, 0.8), Variable(1_bi, 0.2) },		// Blue Flare
		{ Variable(0_bi, 1.0) },				// Fiery Dance
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Freeze Shock
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Ice Burn
		{ Variable(0_bi, 1.0) },				// Snarl
		{ Variable(0_bi, 0.7), Variable(1_bi, 0.3) },		// Icicle Crash
		{ Variable(0_bi, 1.0) },				// V-create
		{ Variable(0_bi, 1.0) },				// Fusion Flare
		{ Variable(0_bi, 1.0) }		// Fusion Bolt
	}};
	for (auto const & probabilities : activate_probability) {
		assert(almost_equal(probability_sum(probabilities), 1.0));
	}
	return activate_probability;
}

auto phaze_probability(TeamSize const foe_size) -> Probabilities {
	Probabilities probabilities;
	static constexpr auto large_cutoff = 2_bi;
	using LargeFoeSize = bounded::integer<large_cutoff.value(), std::numeric_limits<TeamSize>::max().value()>;
	auto const possible_replacements = BOUNDED_CONDITIONAL(foe_size > large_cutoff, LargeFoeSize(foe_size - 1_bi, bounded::non_check), 1_bi);
	for (auto const n : bounded::integer_range(possible_replacements)) {
		probabilities.emplace_back(n, 1.0 / static_cast<double>(possible_replacements));
	}
	return probabilities;
}

auto acupressure_probability(ActivePokemon const pokemon) -> Probabilities {
	static constexpr auto boostable_stats = containers::make_array(
		StatNames::ATK,
		StatNames::DEF,
		StatNames::SPA,
		StatNames::SPD,
		StatNames::SPE,
		StatNames::ACC,
		StatNames::EVA
	);
	auto const non_maxed_stats = containers::count_if(begin(boostable_stats), end(boostable_stats), [&](StatNames const stat) {
		return stage(pokemon)[stat] != 6_bi;
	});
	if (non_maxed_stats == 0_bi) {
		// Arbitrary value so the code doesn't break when Acupressure can no
		// longer work
		return Probabilities({ Variable(0_bi, 1.0) });
	}
	auto const probability = 1.0 / static_cast<double>(bounded::max(1_bi, non_maxed_stats));
	Probabilities probabilities;
	for (auto const stat : boostable_stats) {
		probabilities.emplace_back(stat, probability);
	}
	return probabilities;
}

}	// namespace

auto all_probabilities(ActivePokemon const pokemon, TeamSize const foe_size) -> Probabilities {
	static auto const probability = initial_probabilities();
	Moves const move = current_move(pokemon);
	if (is_phaze(move)) {
		return phaze_probability(foe_size);
	} else if (move == Moves::Acupressure) {
		return acupressure_probability(pokemon);
	} else {
		return probability[move];
	}
}
}	// namespace technicalmachine
