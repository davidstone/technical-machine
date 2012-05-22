// PP class
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

#include "pp.hpp"
#include <algorithm>
#include <cassert>
#include "../ability.hpp"
#include "../rational.hpp"

namespace technicalmachine {
enum class Moves : uint16_t;
namespace {

constexpr uint8_t unlimited_pp = 0xFF;
Rational pp_ups_multiplier(unsigned pp_ups);
uint8_t get_pp (Moves move);

}	// unnamed namespace

Pp::Pp (Moves const move, unsigned const pp_ups) :
	max(get_pp(move) * pp_ups_multiplier(pp_ups)),
	current(max)
	{
}

bool Pp::is_empty() const {
	return current == 0;
}

bool Pp::has_unlimited_pp() const {
	return max == unlimited_pp;
}

void Pp::decrement(Ability const & foe_ability) {
	if (has_unlimited_pp())
		return;
	assert(current != 0);
	unsigned const reductions = std::min(foe_ability.uses_extra_pp() ? 2u : 1u, static_cast<unsigned>(current));
	current -= reductions;
}

unsigned Pp::trump_card_power() const {
	switch (current) {
		case 0:
			return 200;
		case 1:
			return 80;
		case 2:
			return 60;
		case 3:
			return 50;
		default:
			return 40;
	}
}

uint64_t Pp::hash() const {
	return current;
}

uint64_t Pp::max_hash() const {
	return max;
}

bool operator== (Pp const & lhs, Pp const & rhs) {
	return lhs.current == rhs.current;
}

bool operator!= (Pp const & lhs, Pp const & rhs) {
	return !(lhs == rhs);
}

namespace {

Rational pp_ups_multiplier(unsigned const pp_ups) {
	return Rational(5 + pp_ups, 5);
}

uint8_t get_pp (Moves const move) {
	static constexpr uint8_t get_pp [] = {
		25,		// Absorb
		30,		// Acid
		40,		// Acid Armor
		30,		// Acupressure
		20,		// Aerial Ace
		5,		// Aeroblast
		30,		// Agility
		25,		// Air Cutter
		20,		// Air Slash
		20,		// Amnesia
		5,		// AncientPower
		20,		// Aqua Jet
		20,		// Aqua Ring
		10,		// Aqua Tail
		20,		// Arm Thrust
		5,		// Aromatherapy
		20,		// Assist
		10,		// Assurance
		15,		// Astonish
		15,		// Attack Order
		15,		// Attract
		20,		// Aura Sphere
		20,		// Aurora Beam
		10,		// Avalanche
		20,		// Barrage
		30,		// Barrier
		40,		// Baton Pass
		10,		// Beat Up
		10,		// Belly Drum
		10,		// Bide
		20,		// Bind
		25,		// Bite
		5,		// Blast Burn
		10,		// Blaze Kick
		5,		// Blizzard
		5,		// Block
		15,		// Body Slam
		20,		// Bone Club
		10,		// Bone Rush
		10,		// Bonemerang
		5,		// Bounce
		15,		// Brave Bird
		15,		// Brick Break
		10,		// Brine
		30,		// Bubble
		20,		// BubbleBeam
		20,		// Bug Bite
		10,		// Bug Buzz
		20,		// Bulk Up
		30,		// Bullet Punch
		30,		// Bullet Seed
		20,		// Calm Mind
		20,		// Camouflage
		20,		// Captivate
		20,		// Charge
		10,		// Charge Beam
		20,		// Charm
		20,		// Chatter
		10,		// Clamp
		5,		// Close Combat
		15,		// Comet Punch
		10,		// Confuse Ray
		25,		// Confusion
		35,		// Constrict
		30,		// Conversion
		30,		// Conversion2
		20,		// Copycat
		20,		// Cosmic Power
		40,		// Cotton Spore
		20,		// Counter
		40,		// Covet
		10,		// Crabhammer
		5,		// Cross Chop
		20,		// Cross Poison
		15,		// Crunch
		10,		// Crush Claw
		5,		// Crush Grip
		10,		// Curse
		30,		// Cut
		15,		// Dark Pulse
		10,		// Dark Void
		10,		// Defend Order
		40,		// Defense Curl
		15,		// Defog
		5,		// Destiny Bond
		5,		// Detect
		10,		// Dig
		20,		// Disable
		15,		// Discharge
		10,		// Dive
		10,		// Dizzy Punch
		5,		// Doom Desire
		10,		// Double Hit
		30,		// Double Kick
		15,		// Double Team
		15,		// Double-Edge
		10,		// DoubleSlap
		5,		// Draco Meteor
		15,		// Dragon Claw
		20,		// Dragon Dance
		10,		// Dragon Pulse
		10,		// Dragon Rage
		10,		// Dragon Rush
		20,		// DragonBreath
		5,		// Drain Punch
		15,		// Dream Eater
		20,		// Drill Peck
		5,		// DynamicPunch
		10,		// Earth Power
		10,		// Earthquake
		10,		// Egg Bomb
		15,		// Embargo
		25,		// Ember
		5,		// Encore
		5,		// Endeavor
		10,		// Endure
		10,		// Energy Ball
		5,		// Eruption
		5,		// Explosion
		30,		// Extrasensory
		5,		// ExtremeSpeed
		20,		// Facade
		20,		// Faint Attack
		10,		// Fake Out
		20,		// Fake Tears
		40,		// False Swipe
		15,		// FeatherDance
		10,		// Feint
		5,		// Fire Blast
		15,		// Fire Fang
		15,		// Fire Punch
		15,		// Fire Spin
		5,		// Fissure
		15,		// Flail
		25,		// Flame Wheel
		15,		// Flamethrower
		15,		// Flare Blitz
		20,		// Flash
		10,		// Flash Cannon
		15,		// Flatter
		10,		// Fling
		15,		// Fly
		5,		// Focus Blast
		30,		// Focus Energy
		20,		// Focus Punch
		20,		// Follow Me
		10,		// Force Palm
		5,		// Foresight
		5,		// Frenzy Plant
		20,		// Frustration
		20,		// Fury Attack
		20,		// Fury Cutter
		15,		// Fury Swipes
		15,		// Future Sight
		10,		// Gastro Acid
		10,		// Giga Drain
		5,		// Giga Impact
		30,		// Glare
		20,		// Grass Knot
		15,		// GrassWhistle
		5,		// Gravity
		40,		// Growl
		40,		// Growth
		5,		// Grudge
		10,		// Guard Swap
		5,		// Guillotine
		5,		// Gunk Shot
		35,		// Gust
		5,		// Gyro Ball
		10,		// Hail
		10,		// Hammer Arm
		30,		// Harden
		30,		// Haze
		5,		// Head Smash
		15,		// Headbutt
		5,		// Heal Bell
		15,		// Heal Block
		10,		// Heal Order
		10,		// Healing Wish
		10,		// Heart Swap
		10,		// Heat Wave
		20,		// Helping Hand
		20,		// Hi Jump Kick
		15,		// Hidden Power
		25,		// Horn Attack
		5,		// Horn Drill
		40,		// Howl
		5,		// Hydro Cannon
		5,		// Hydro Pump
		5,		// Hyper Beam
		15,		// Hyper Fang
		10,		// Hyper Voice
		20,		// Hypnosis
		20,		// Ice Ball
		10,		// Ice Beam
		15,		// Ice Fang
		15,		// Ice Punch
		30,		// Ice Shard
		30,		// Icicle Spear
		15,		// Icy Wind
		10,		// Imprison
		20,		// Ingrain
		15,		// Iron Defense
		15,		// Iron Head
		15,		// Iron Tail
		10,		// Judgment
		25,		// Jump Kick
		25,		// Karate Chop
		15,		// Kinesis
		20,		// Knock Off
		5,		// Last Resort
		15,		// Lava Plume
		15,		// Leaf Blade
		5,		// Leaf Storm
		15,		// Leech Life
		10,		// Leech Seed
		30,		// Leer
		30,		// Lick
		30,		// Light Screen
		5,		// Lock-On
		10,		// Lovely Kiss
		20,		// Low Kick
		30,		// Lucky Chant
		10,		// Lunar Dance
		5,		// Luster Purge
		30,		// Mach Punch
		15,		// Magic Coat
		20,		// Magical Leaf
		5,		// Magma Storm
		20,		// Magnet Bomb
		10,		// Magnet Rise
		30,		// Magnitude
		20,		// Me First
		5,		// Mean Look
		40,		// Meditate
		15,		// Mega Drain
		5,		// Mega Kick
		20,		// Mega Punch
		10,		// Megahorn
		10,		// Memento
		10,		// Metal Burst
		35,		// Metal Claw
		40,		// Metal Sound
		10,		// Meteor Mash
		10,		// Metronome
		10,		// Milk Drink
		10,		// Mimic
		40,		// Mind Reader
		20,		// Minimize
		40,		// Miracle Eye
		20,		// Mirror Coat
		20,		// Mirror Move
		10,		// Mirror Shot
		30,		// Mist
		5,		// Mist Ball
		5,		// Moonlight
		5,		// Morning Sun
		10,		// Mud Bomb
		15,		// Mud Shot
		15,		// Mud Sport
		10,		// Mud-Slap
		10,		// Muddy Water
		20,		// Nasty Plot
		15,		// Natural Gift
		20,		// Nature Power
		15,		// Needle Arm
		15,		// Night Shade
		15,		// Night Slash
		15,		// Nightmare
		10,		// Octazooka
		40,		// Odor Sleuth
		5,		// Ominous Wind
		15,		// Outrage
		5,		// Overheat
		20,		// Pain Split
		20,		// Pay Day
		10,		// Payback
		35,		// Peck
		5,		// Perish Song
		20,		// Petal Dance
		20,		// Pin Missile
		20,		// Pluck
		15,		// Poison Fang
		40,		// Poison Gas
		20,		// Poison Jab
		35,		// Poison Sting
		25,		// Poison Tail
		35,		// PoisonPowder
		35,		// Pound
		25,		// Powder Snow
		20,		// Power Gem
		10,		// Power Swap
		10,		// Power Trick
		10,		// Power Whip
		15,		// Present
		10,		// Protect
		20,		// Psybeam
		10,		// Psych Up
		10,		// Psychic
		5,		// Psycho Boost
		20,		// Psycho Cut
		10,		// Psycho Shift
		15,		// Psywave
		5,		// Punishment
		20,		// Pursuit
		30,		// Quick Attack
		20,		// Rage
		5,		// Rain Dance
		40,		// Rapid Spin
		25,		// Razor Leaf
		10,		// Razor Wind
		10,		// Recover
		10,		// Recycle
		20,		// Reflect
		20,		// Refresh
		10,		// Rest
		20,		// Return
		10,		// Revenge
		15,		// Reversal
		20,		// Roar
		5,		// Roar Of Time
		10,		// Rock Blast
		20,		// Rock Climb
		20,		// Rock Polish
		10,		// Rock Slide
		15,		// Rock Smash
		15,		// Rock Throw
		10,		// Rock Tomb
		5,		// Rock Wrecker
		10,		// Role Play
		15,		// Rolling Kick
		20,		// Rollout
		10,		// Roost
		5,		// Sacred Fire
		25,		// Safeguard
		15,		// Sand Tomb
		15,		// Sand-Attack
		10,		// Sandstorm
		10,		// Scary Face
		35,		// Scratch
		40,		// Screech
		20,		// Secret Power
		15,		// Seed Bomb
		5,		// Seed Flare
		20,		// Seismic Toss
		5,		// Selfdestruct
		15,		// Shadow Ball
		15,		// Shadow Claw
		5,		// Shadow Force
		20,		// Shadow Punch
		30,		// Shadow Sneak
		30,		// Sharpen
		5,		// Sheer Cold
		20,		// Shock Wave
		15,		// Signal Beam
		5,		// Silver Wind
		15,		// Sing
		1,		// Sketch
		10,		// Skill Swap
		15,		// Skull Bash
		5,		// Sky Attack
		15,		// Sky Uppercut
		10,		// Slack Off
		20,		// Slam
		20,		// Slash
		15,		// Sleep Powder
		10,		// Sleep Talk
		20,		// Sludge
		10,		// Sludge Bomb
		10,		// SmellingSalt
		20,		// Smog
		20,		// SmokeScreen
		10,		// Snatch
		15,		// Snore
		10,		// Softboiled
		10,		// SolarBeam
		20,		// SonicBoom
		5,		// Spacial Rend
		20,		// Spark
		10,		// Spider Web
		15,		// Spike Cannon
		20,		// Spikes
		10,		// Spit Up
		10,		// Spite
		40,		// Splash
		15,		// Spore
		20,		// Stealth Rock
		25,		// Steel Wing
		20,		// Stockpile
		20,		// Stomp
		5,		// Stone Edge
		15,		// Strength
		40,		// String Shot
		unlimited_pp,		// Struggle
		30,		// Stun Spore
		25,		// Submission
		10,		// Substitute
		5,		// Sucker Punch
		5,		// Sunny Day
		10,		// Super Fang
		5,		// Superpower
		20,		// Supersonic
		15,		// Surf
		15,		// Swagger
		10,		// Swallow
		10,		// Sweet Kiss
		20,		// Sweet Scent
		20,		// Swift
		unlimited_pp,		// Switch0
		unlimited_pp,		// Switch1
		unlimited_pp,		// Switch2
		unlimited_pp,		// Switch3
		unlimited_pp,		// Switch4
		unlimited_pp,		// Switch5
		10,		// Switcheroo
		30,		// Swords Dance
		5,		// Synthesis
		35,		// Tackle
		20,		// Tail Glow
		30,		// Tail Whip
		30,		// Tailwind
		20,		// Take Down
		20,		// Taunt
		20,		// Teeter Dance
		20,		// Teleport
		10,		// Thief
		20,		// Thrash
		10,		// Thunder
		15,		// Thunder Fang
		20,		// Thunder Wave
		15,		// Thunderbolt
		15,		// ThunderPunch
		30,		// ThunderShock
		20,		// Tickle
		15,		// Torment
		10,		// Toxic
		20,		// Toxic Spikes
		10,		// Transform
		10,		// Tri Attack
		10,		// Trick
		5,		// Trick Room
		10,		// Triple Kick
		5,		// Trump Card
		20,		// Twineedle
		20,		// Twister
		20,		// U-turn
		10,		// Uproar
		30,		// Vacuum Wave
		30,		// ViceGrip
		15,		// Vine Whip
		10,		// Vital Throw
		15,		// Volt Tackle
		10,		// Wake-Up Slap
		25,		// Water Gun
		20,		// Water Pulse
		15,		// Water Sport
		5,		// Water Spout
		15,		// Waterfall
		10,		// Weather Ball
		15,		// Whirlpool
		20,		// Whirlwind
		15,		// Will-O-Wisp
		35,		// Wing Attack
		10,		// Wish
		40,		// Withdraw
		15,		// Wood Hammer
		10,		// Worry Seed
		20,		// Wrap
		5,		// Wring Out
		15,		// X-Scissor
		10,		// Yawn
		5,		// Zap Cannon
		15		// Zen Headbutt
	};
	return get_pp[static_cast<unsigned>(move)];
}

}	// unnamed namespace
}	// namespace technicalmachine
