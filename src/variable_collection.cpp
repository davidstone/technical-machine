// Collection of move random effects with index indicating current effect
// Copyright (C) 2012 David Stone
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

#include "variable_collection.hpp"

#include <cassert>
#include <cstdint>

#include "empty_team.hpp"
#include "phazing_in_same_pokemon.hpp"
#include "variable.hpp"

#include "move/move.hpp"

namespace technicalmachine {
namespace {

typedef detail::BaseCollection<Variable>::container_type container_type;
container_type create_container (Move::Moves move, unsigned foe_size);
container_type simple_range (uint16_t first, uint16_t last, Move::Moves move);
container_type simple_range (uint16_t first, uint16_t last, uint16_t probability);
container_type default_effects (Move::Moves move);
uint16_t get_probability (Move::Moves move);

}	// unnamed namespace

VariableCollection::VariableCollection (uint16_t const move, unsigned const foe_size):
	detail::BaseCollection<Variable>(create_container(static_cast<Move::Moves>(move), foe_size)) {
}

void VariableCollection::set_phaze_index (uint8_t const pokemon_index, uint8_t const new_index) {
	if (container.size() == 1) {
		assert (new_index == 0);
		reset_index();
	}
	else {
		if (new_index == pokemon_index)
			throw PhazingInSamePokemon(new_index);
		set_index ((new_index < pokemon_index) ? new_index : new_index - 1);
	}
}

uint8_t VariableCollection::phaze_index (uint8_t const pokemon_index) const {
	if (container.size() <= 1)
		throw InvalidCollectionIndex (pokemon_index, container.size(), "phaze variable");
	return (operator()().value() < pokemon_index) ? operator()().value() : operator()().value() + 1;
}

void VariableCollection::for_each_index (std::function<void(void)> const & f) {
	for (current_index = 0; current_index != container.size(); ++current_index) {
		f ();
	}
}

void VariableCollection::remove_phazing (uint8_t const foe_size) {
	container.pop_back();
	for_each([foe_size](Variable & variable) {
		variable.reset_phaze_probabilities(foe_size);
	});
}

void VariableCollection::set_magnitude (unsigned const magnitude) {
	set_index (magnitude - 4);
}

namespace {

container_type create_container (Move::Moves const move, unsigned const foe_size) {
	if (foe_size == 0)
		throw EmptyTeam(__FILE__, __LINE__);
	switch (move) {
		case Move::ACUPRESSURE:
			return simple_range(0, 6, move);
		case Move::BIND:
		case Move::CLAMP:
		case Move::FIRE_SPIN:
		case Move::MAGMA_STORM:
		case Move::SAND_TOMB:
		case Move::WHIRLPOOL:
		case Move::WRAP: {
			uint16_t const probability = get_probability (move);
			return container_type ({
				Variable(2, probability),
				Variable(2, probability),
				Variable(4, probability / 3),
				Variable(5, probability / 3)
			});
		}
		case Move::ENCORE:
			return simple_range(4, 8, move);
		case Move::FIRE_FANG:
		case Move::ICE_FANG:
		case Move::THUNDER_FANG: {
			constexpr uint16_t flinch_and_status = Variable::max_probability / (10u * 10u);
			constexpr uint16_t only_flinch = Variable::max_probability / 10u - flinch_and_status;
			constexpr uint16_t only_status = Variable::max_probability / 10u - flinch_and_status;
			constexpr uint16_t no_effect = Variable::max_probability - only_flinch - only_status - flinch_and_status;
			return container_type ({
				Variable(0, no_effect),
				Variable(1, only_status),
				Variable(2, only_flinch),
				Variable(3, flinch_and_status)
			});
		}
		case Move::MAGNITUDE:
			return container_type ({
				Variable(10, Variable::max_probability * 5u / 100u),
				Variable(30, Variable::max_probability * 10u / 100u),
				Variable(50, Variable::max_probability * 20u / 100u),
				Variable(70, Variable::max_probability * 30u / 100u),
				Variable(90, Variable::max_probability * 20u / 100u),
				Variable(110, Variable::max_probability * 10u / 100u),
				Variable(150, Variable::max_probability * 5u / 100u)
			});
		case Move::OUTRAGE:
		case Move::PETAL_DANCE:
		case Move::THRASH:
			return simple_range(2, 3, move);
		case Move::PRESENT: {
			uint16_t const probability = get_probability (move);
			return container_type ({
				Variable(0, probability),
				Variable(40, probability),
				Variable(80, probability),
				Variable(120, probability)
			});
		}
		case Move::PSYWAVE:
			return simple_range(5, 15, move);
		case Move::ROAR:
		case Move::WHIRLWIND: {
			if (foe_size > 2) {
				uint16_t const probability = Variable::max_probability / (foe_size - 1);
				return simple_range(0, foe_size - 2, probability);
			}
			else {
				return container_type ({
					Variable(0, Variable::max_probability)
				});
			}
		}
		case Move::TAUNT:
			return simple_range(2, 3, move);
		case Move::TRI_ATTACK:
			return simple_range(0, 3, move);
		default:
			return default_effects(move);
	}
}

container_type simple_range (uint16_t const first, uint16_t const last, Move::Moves const move) {
	return simple_range(first, last, get_probability (move));
}

container_type simple_range (uint16_t const first, uint16_t const last, uint16_t const probability) {
	container_type collection;
	for (uint16_t value = first; value <= last; ++value)
		collection.push_back(Variable(value, probability));
	return collection;
}

container_type default_effects (Move::Moves const move) {
	container_type collection;
	uint16_t const probability = get_probability(move);
	if (probability != Variable::max_probability)
		collection.push_back(Variable(0, Variable::max_probability - probability));
	if (probability != 0)
		collection.push_back(Variable(0, probability));
	return collection;
}

uint16_t get_probability (Move::Moves const move) {
	// Chance (out of max_probability) to activate side-effect
	static constexpr uint16_t get_probability [] = {
		0,		// Absorb
		84,		// Acid
		0,		// Acid Armor
		140,		// Acupressure
		0,		// Aerial Ace
		0,		// Aeroblast
		0,		// Agility
		0,		// Air Cutter
		252,		// Air Slash
		0,		// Amnesia
		84,		// AncientPower
		0,		// Aqua Jet
		0,		// Aqua Ring
		0,		// Aqua Tail
		0,		// Arm Thrust
		0,		// Aromatherapy
		0,		// Assist
		0,		// Assurance
		252,		// Astonish
		0,		// Attack Order
		0,		// Attract
		0,		// Aura Sphere
		84,		// Aurora Beam
		0,		// Avalanche
		0,		// Barrage
		0,		// Barrier
		0,		// Baton Pass
		0,		// Beat Up
		0,		// Belly Drum
		0,		// Bide
		315,		// Bind
		252,		// Bite
		0,		// Blast Burn
		84,		// Blaze Kick
		84,		// Blizzard
		0,		// Block
		252,		// Body Slam
		84,		// Bone Club
		0,		// Bone Rush
		0,		// Bonemerang
		252,		// Bounce
		0,		// Brave Bird
		0,		// Brick Break
		0,		// Brine
		84,		// Bubble
		84,		// BubbleBeam
		0,		// Bug Bite
		84,		// Bug Buzz
		0,		// Bulk Up
		0,		// Bullet Punch
		0,		// Bullet Seed
		0,		// Calm Mind
		0,		// Camouflage
		0,		// Captivate
		0,		// Charge
		588,		// Charge Beam
		0,		// Charm
		0,		// Chatter
		315,		// Clamp
		0,		// Close Combat
		0,		// Comet Punch
		0,		// Confuse Ray
		84,		// Confusion
		84,		// Constrict
		0,		// Conversion
		0,		// Conversion2
		0,		// Copycat
		0,		// Cosmic Power
		0,		// Cotton Spore
		0,		// Counter
		0,		// Covet
		0,		// Crabhammer
		0,		// Cross Chop
		84,		// Cross Poison
		168,		// Crunch
		420,		// Crush Claw
		0,		// Crush Grip
		0,		// Curse
		0,		// Cut
		168,		// Dark Pulse
		0,		// Dark Void
		0,		// Defend Order
		0,		// Defense Curl
		0,		// Defog
		0,		// Destiny Bond
		0,		// Detect
		0,		// Dig
		0,		// Disable
		252,		// Discharge
		0,		// Dive
		168,		// Dizzy Punch
		0,		// Doom Desire
		0,		// Double Hit
		0,		// Double Kick
		0,		// Double Team
		0,		// Double-Edge
		0,		// DoubleSlap
		0,		// Draco Meteor
		0,		// Dragon Claw
		0,		// Dragon Dance
		0,		// Dragon Pulse
		0,		// Dragon Rage
		168,		// Dragon Rush
		252,		// DragonBreath
		0,		// Drain Punch
		0,		// Dream Eater
		0,		// Drill Peck
		840,		// DynamicPunch
		84,		// Earth Power
		0,		// Earthquake
		0,		// Egg Bomb
		0,		// Embargo
		84,		// Ember
		168,		// Encore
		0,		// Endeavor
		0,		// Endure
		84,		// Energy Ball
		0,		// Eruption
		0,		// Explosion
		84,		// Extrasensory
		0,		// ExtremeSpeed
		0,		// Facade
		0,		// Faint Attack
		840,		// Fake Out
		0,		// Fake Tears
		0,		// False Swipe
		0,		// FeatherDance
		0,		// Feint
		84,		// Fire Blast
		84,		// Fire Fang
		84,		// Fire Punch
		315,		// Fire Spin
		0,		// Fissure
		0,		// Flail
		84,		// Flame Wheel
		84,		// Flamethrower
		84,		// Flare Blitz
		0,		// Flash
		84,		// Flash Cannon
		0,		// Flatter
		0,		// Fling
		0,		// Fly
		84,		// Focus Blast
		0,		// Focus Energy
		0,		// Focus Punch
		0,		// Follow Me
		252,		// Force Palm
		0,		// Foresight
		0,		// Frenzy Plant
		0,		// Frustration
		0,		// Fury Attack
		0,		// Fury Cutter
		0,		// Fury Swipes
		0,		// Future Sight
		0,		// Gastro Acid
		0,		// Giga Drain
		0,		// Giga Impact
		0,		// Glare
		0,		// Grass Knot
		0,		// GrassWhistle
		0,		// Gravity
		0,		// Growl
		0,		// Growth
		0,		// Grudge
		0,		// Guard Swap
		0,		// Guillotine
		252,		// Gunk Shot
		0,		// Gust
		0,		// Gyro Ball
		0,		// Hail
		0,		// Hammer Arm
		0,		// Harden
		0,		// Haze
		0,		// Head Smash
		252,		// Headbutt
		0,		// Heal Bell
		0,		// Heal Block
		0,		// Heal Order
		0,		// Healing Wish
		0,		// Heart Swap
		84,		// Heat Wave
		0,		// Helping Hand
		0,		// Hi Jump Kick
		0,		// Hidden Power
		0,		// Horn Attack
		0,		// Horn Drill
		0,		// Howl
		0,		// Hydro Cannon
		0,		// Hydro Pump
		0,		// Hyper Beam
		84,		// Hyper Fang
		0,		// Hyper Voice
		0,		// Hypnosis
		0,		// Ice Ball
		84,		// Ice Beam
		84,		// Ice Fang
		84,		// Ice Punch
		0,		// Ice Shard
		0,		// Icicle Spear
		0,		// Icy Wind
		0,		// Imprison
		0,		// Ingrain
		0,		// Iron Defense
		252,		// Iron Head
		252,		// Iron Tail
		0,		// Judgment
		0,		// Jump Kick
		0,		// Karate Chop
		0,		// Kinesis
		0,		// Knock Off
		0,		// Last Resort
		252,		// Lava Plume
		0,		// Leaf Blade
		0,		// Leaf Storm
		0,		// Leech Life
		0,		// Leech Seed
		0,		// Leer
		252,		// Lick
		0,		// Light Screen
		0,		// Lock-On
		0,		// Lovely Kiss
		0,		// Low Kick
		0,		// Lucky Chant
		0,		// Lunar Dance
		420,		// Luster Purge
		0,		// Mach Punch
		0,		// Magic Coat
		0,		// Magical Leaf
		315,		// Magma Storm
		0,		// Magnet Bomb
		0,		// Magnet Rise
		0,		// Magnitude
		0,		// Me First
		0,		// Mean Look
		0,		// Meditate
		0,		// Mega Drain
		0,		// Mega Kick
		0,		// Mega Punch
		0,		// Megahorn
		0,		// Memento
		0,		// Metal Burst
		84,		// Metal Claw
		0,		// Metal Sound
		168,		// Meteor Mash
		0,		// Metronome
		0,		// Milk Drink
		0,		// Mimic
		0,		// Mind Reader
		0,		// Minimize
		0,		// Miracle Eye
		0,		// Mirror Coat
		0,		// Mirror Move
		252,		// Mirror Shot
		0,		// Mist
		420,		// Mist Ball
		0,		// Moonlight
		0,		// Morning Sun
		252,		// Mud Bomb
		0,		// Mud Shot
		0,		// Mud Sport
		0,		// Mud-Slap
		252,		// Muddy Water
		0,		// Nasty Plot
		0,		// Natural Gift
		0,		// Nature Power
		252,		// Needle Arm
		0,		// Night Shade
		0,		// Night Slash
		0,		// Nightmare
		420,		// Octazooka
		0,		// Odor Sleuth
		84,		// Ominous Wind
		420,		// Outrage
		0,		// Overheat
		0,		// Pain Split
		0,		// Pay Day
		0,		// Payback
		0,		// Peck
		0,		// Perish Song
		420,		// Petal Dance
		0,		// Pin Missile
		0,		// Pluck
		252,		// Poison Fang
		0,		// Poison Gas
		252,		// Poison Jab
		252,		// Poison Sting
		84,		// Poison Tail
		0,		// PoisonPowder
		0,		// Pound
		84,		// Powder Snow
		0,		// Power Gem
		0,		// Power Swap
		0,		// Power Trick
		0,		// Power Whip
		210,		// Present
		0,		// Protect
		84,		// Psybeam
		0,		// Psych Up
		84,		// Psychic
		0,		// Psycho Boost
		0,		// Psycho Cut
		0,		// Psycho Shift
		0,		// Psywave
		0,		// Punishment
		0,		// Pursuit
		0,		// Quick Attack
		0,		// Rage
		0,		// Rain Dance
		0,		// Rapid Spin
		0,		// Razor Leaf
		0,		// Razor Wind
		0,		// Recover
		0,		// Recycle
		0,		// Reflect
		0,		// Refresh
		0,		// Rest
		0,		// Return
		0,		// Revenge
		0,		// Reversal
		0,		// Roar
		0,		// Roar Of Time
		0,		// Rock Blast
		168,		// Rock Climb
		0,		// Rock Polish
		252,		// Rock Slide
		420,		// Rock Smash
		0,		// Rock Throw
		0,		// Rock Tomb
		0,		// Rock Wrecker
		0,		// Role Play
		252,		// Rolling Kick
		0,		// Rollout
		0,		// Roost
		420,		// Sacred Fire
		0,		// Safeguard
		315,		// Sand Tomb
		0,		// Sand-Attack
		0,		// Sandstorm
		0,		// Scary Face
		0,		// Scratch
		0,		// Screech
		0,		// Secret Power
		0,		// Seed Bomb
		336,		// Seed Flare
		0,		// Seismic Toss
		0,		// Selfdestruct
		168,		// Shadow Ball
		0,		// Shadow Claw
		0,		// Shadow Force
		0,		// Shadow Punch
		0,		// Shadow Sneak
		0,		// Sharpen
		0,		// Sheer Cold
		0,		// Shock Wave
		84,		// Signal Beam
		84,		// Silver Wind
		0,		// Sing
		0,		// Sketch
		0,		// Skill Swap
		0,		// Skull Bash
		252,		// Sky Attack
		0,		// Sky Uppercut
		0,		// Slack Off
		0,		// Slam
		0,		// Slash
		0,		// Sleep Powder
		0,		// Sleep Talk
		252,		// Sludge
		252,		// Sludge Bomb
		0,		// SmellingSalt
		336,		// Smog
		0,		// SmokeScreen
		0,		// Snatch
		252,		// Snore
		0,		// Softboiled
		0,		// SolarBeam
		0,		// SonicBoom
		0,		// Spacial Rend
		252,		// Spark
		0,		// Spider Web
		0,		// Spike Cannon
		0,		// Spikes
		0,		// Spit Up
		0,		// Spite
		0,		// Splash
		0,		// Spore
		0,		// Stealth Rock
		84,		// Steel Wing
		0,		// Stockpile
		252,		// Stomp
		0,		// Stone Edge
		0,		// Strength
		0,		// String Shot
		0,		// Struggle
		0,		// Stun Spore
		0,		// Submission
		0,		// Substitute
		0,		// Sucker Punch
		0,		// Sunny Day
		0,		// Super Fang
		0,		// Superpower
		0,		// Supersonic
		0,		// Surf
		0,		// Swagger
		0,		// Swallow
		0,		// Sweet Kiss
		0,		// Sweet Scent
		0,		// Swift
		0,		// Switch0
		0,		// Switch1
		0,		// Switch2
		0,		// Switch3
		0,		// Switch4
		0,		// Switch5
		0,		// Switcheroo
		0,		// Swords Dance
		0,		// Synthesis
		0,		// Tackle
		0,		// Tail Glow
		0,		// Tail Whip
		0,		// Tailwind
		0,		// Take Down
		420,		// Taunt
		0,		// Teeter Dance
		0,		// Teleport
		0,		// Thief
		420,		// Thrash
		252,		// Thunder
		84,		// Thunder Fang
		0,		// Thunder Wave
		84,		// Thunderbolt
		84,		// ThunderPunch
		84,		// ThunderShock
		0,		// Tickle
		0,		// Torment
		0,		// Toxic
		0,		// Toxic Spikes
		0,		// Transform
		168,		// Tri Attack
		0,		// Trick
		0,		// Trick Room
		0,		// Triple Kick
		0,		// Trump Card
		168,		// Twineedle
		168,		// Twister
		0,		// U-turn
		0,		// Uproar
		0,		// Vacuum Wave
		0,		// ViceGrip
		0,		// Vine Whip
		0,		// Vital Throw
		84,		// Volt Tackle
		0,		// Wake-Up Slap
		0,		// Water Gun
		168,		// Water Pulse
		0,		// Water Sport
		0,		// Water Spout
		168,		// Waterfall
		0,		// Weather Ball
		315,		// Whirlpool
		0,		// Whirlwind
		0,		// Will-O-Wisp
		0,		// Wing Attack
		0,		// Wish
		0,		// Withdraw
		0,		// Wood Hammer
		0,		// Worry Seed
		315,		// Wrap
		0,		// Wring Out
		0,		// X-Scissor
		0,		// Yawn
		0,		// Zap Cannon
		168		// Zen Headbut
	};
	return get_probability [move];
}
}	// unnamed namespace
}	// namespace technicalmachine
