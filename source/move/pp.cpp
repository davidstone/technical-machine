// PP class
// Copyright (C) 2013 David Stone
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

#include "moves_forward.hpp"

#include "../ability.hpp"
#include "../rational.hpp"

namespace technicalmachine {
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
		unlimited_pp,		// Switch0
		unlimited_pp,		// Switch1
		unlimited_pp,		// Switch2
		unlimited_pp,		// Switch3
		unlimited_pp,		// Switch4
		unlimited_pp,		// Switch5
		unlimited_pp,		// Hit_Self
		35,		// Pound
		25,		// Karate Chop
		10,		// DoubleSlap
		15,		// Comet Punch
		20,		// Mega Punch
		20,		// Pay Day
		15,		// Fire Punch
		15,		// Ice Punch
		15,		// ThunderPunch
		35,		// Scratch
		30,		// ViceGrip
		5,		// Guillotine
		10,		// Razor Wind
		30,		// Swords Dance
		30,		// Cut
		35,		// Gust
		35,		// Wing Attack
		20,		// Whirlwind
		15,		// Fly
		20,		// Bind
		20,		// Slam
		15,		// Vine Whip
		20,		// Stomp
		30,		// Double Kick
		5,		// Mega Kick
		10,		// Jump Kick
		15,		// Rolling Kick
		15,		// Sand-Attack
		15,		// Headbutt
		25,		// Horn Attack
		20,		// Fury Attack
		5,		// Horn Drill
		35,		// Tackle
		15,		// Body Slam
		20,		// Wrap
		20,		// Take Down
		10,		// Thrash
		15,		// Double-Edge
		30,		// Tail Whip
		35,		// Poison Sting
		20,		// Twineedle
		20,		// Pin Missile
		30,		// Leer
		25,		// Bite
		40,		// Growl
		20,		// Roar
		15,		// Sing
		20,		// Supersonic
		20,		// SonicBoom
		20,		// Disable
		30,		// Acid
		25,		// Ember
		15,		// Flamethrower
		30,		// Mist
		25,		// Water Gun
		5,		// Hydro Pump
		15,		// Surf
		10,		// Ice Beam
		5,		// Blizzard
		20,		// Psybeam
		20,		// BubbleBeam
		20,		// Aurora Beam
		5,		// Hyper Beam
		35,		// Peck
		20,		// Drill Peck
		25,		// Submission
		20,		// Low Kick
		20,		// Counter
		20,		// Seismic Toss
		15,		// Strength
		25,		// Absorb
		15,		// Mega Drain
		10,		// Leech Seed
		40,		// Growth
		25,		// Razor Leaf
		10,		// SolarBeam
		35,		// PoisonPowder
		30,		// Stun Spore
		15,		// Sleep Powder
		10,		// Petal Dance
		40,		// String Shot
		10,		// Dragon Rage
		15,		// Fire Spin
		30,		// ThunderShock
		15,		// Thunderbolt
		20,		// Thunder Wave
		10,		// Thunder
		15,		// Rock Throw
		10,		// Earthquake
		5,		// Fissure
		10,		// Dig
		10,		// Toxic
		25,		// Confusion
		10,		// Psychic
		20,		// Hypnosis
		40,		// Meditate
		30,		// Agility
		30,		// Quick Attack
		20,		// Rage
		20,		// Teleport
		15,		// Night Shade
		10,		// Mimic
		40,		// Screech
		15,		// Double Team
		10,		// Recover
		30,		// Harden
		20,		// Minimize
		20,		// SmokeScreen
		10,		// Confuse Ray
		40,		// Withdraw
		40,		// Defense Curl
		30,		// Barrier
		30,		// Light Screen
		30,		// Haze
		20,		// Reflect
		30,		// Focus Energy
		10,		// Bide
		10,		// Metronome
		20,		// Mirror Move
		5,		// Selfdestruct
		10,		// Egg Bomb
		30,		// Lick
		20,		// Smog
		20,		// Sludge
		20,		// Bone Club
		5,		// Fire Blast
		15,		// Waterfall
		15,		// Clamp
		20,		// Swift
		15,		// Skull Bash
		15,		// Spike Cannon
		35,		// Constrict
		20,		// Amnesia
		15,		// Kinesis
		10,		// Softboiled
		10,		// Hi Jump Kick
		30,		// Glare
		15,		// Dream Eater
		40,		// Poison Gas
		20,		// Barrage
		15,		// Leech Life
		10,		// Lovely Kiss
		5,		// Sky Attack
		10,		// Transform
		30,		// Bubble
		10,		// Dizzy Punch
		15,		// Spore
		20,		// Flash
		15,		// Psywave
		40,		// Splash
		40,		// Acid Armor
		10,		// Crabhammer
		5,		// Explosion
		15,		// Fury Swipes
		10,		// Bonemerang
		10,		// Rest
		10,		// Rock Slide
		15,		// Hyper Fang
		30,		// Sharpen
		30,		// Conversion
		10,		// Tri Attack
		10,		// Super Fang
		20,		// Slash
		10,		// Substitute
		unlimited_pp,		// Struggle
		1,		// Sketch
		10,		// Triple Kick
		10,		// Thief
		10,		// Spider Web
		5,		// Mind Reader
		15,		// Nightmare
		25,		// Flame Wheel
		15,		// Snore
		10,		// Curse
		15,		// Flail
		30,		// Conversion 2
		5,		// Aeroblast
		40,		// Cotton Spore
		15,		// Reversal
		10,		// Spite
		25,		// Powder Snow
		10,		// Protect
		30,		// Mach Punch
		10,		// Scary Face
		20,		// Faint Attack
		10,		// Sweet Kiss
		10,		// Belly Drum
		10,		// Sludge Bomb
		10,		// Mud-Slap
		10,		// Octazooka
		20,		// Spikes
		5,		// Zap Cannon
		40,		// Foresight
		5,		// Destiny Bond
		5,		// Perish Song
		15,		// Icy Wind
		5,		// Detect
		10,		// Bone Rush
		5,		// Lock-On
		10,		// Outrage
		10,		// Sandstorm
		10,		// Giga Drain
		10,		// Endure
		20,		// Charm
		20,		// Rollout
		40,		// False Swipe
		15,		// Swagger
		10,		// Milk Drink
		20,		// Spark
		20,		// Fury Cutter
		25,		// Steel Wing
		5,		// Mean Look
		15,		// Attract
		10,		// Sleep Talk
		5,		// Heal Bell
		20,		// Return
		15,		// Present
		20,		// Frustration
		25,		// Safeguard
		20,		// Pain Split
		5,		// Sacred Fire
		30,		// Magnitude
		5,		// DynamicPunch
		10,		// Megahorn
		20,		// DragonBreath
		40,		// Baton Pass
		5,		// Encore
		20,		// Pursuit
		40,		// Rapid Spin
		20,		// Sweet Scent
		15,		// Iron Tail
		35,		// Metal Claw
		10,		// Vital Throw
		5,		// Morning Sun
		5,		// Synthesis
		5,		// Moonlight
		15,		// Hidden Power
		5,		// Cross Chop
		20,		// Twister
		5,		// Rain Dance
		5,		// Sunny Day
		15,		// Crunch
		20,		// Mirror Coat
		10,		// Psych Up
		5,		// ExtremeSpeed
		5,		// AncientPower
		15,		// Shadow Ball
		10,		// Future Sight
		15,		// Rock Smash
		15,		// Whirlpool
		10,		// Beat Up
		10,		// Fake Out
		10,		// Uproar
		20,		// Stockpile
		10,		// Spit Up
		10,		// Swallow
		10,		// Heat Wave
		10,		// Hail
		15,		// Torment
		15,		// Flatter
		15,		// Will-O-Wisp
		10,		// Memento
		20,		// Facade
		20,		// Focus Punch
		10,		// SmellingSalt
		20,		// Follow Me
		20,		// Nature Power
		20,		// Charge
		20,		// Taunt
		20,		// Helping Hand
		10,		// Trick
		10,		// Role Play
		10,		// Wish
		20,		// Assist
		20,		// Ingrain
		5,		// Superpower
		15,		// Magic Coat
		10,		// Recycle
		10,		// Revenge
		15,		// Brick Break
		10,		// Yawn
		20,		// Knock Off
		5,		// Endeavor
		5,		// Eruption
		10,		// Skill Swap
		10,		// Imprison
		20,		// Refresh
		5,		// Grudge
		10,		// Snatch
		20,		// Secret Power
		10,		// Dive
		20,		// Arm Thrust
		20,		// Camouflage
		20,		// Tail Glow
		5,		// Luster Purge
		5,		// Mist Ball
		15,		// FeatherDance
		20,		// Teeter Dance
		10,		// Blaze Kick
		15,		// Mud Sport
		20,		// Ice Ball
		15,		// Needle Arm
		10,		// Slack Off
		10,		// Hyper Voice
		15,		// Poison Fang
		10,		// Crush Claw
		5,		// Blast Burn
		5,		// Hydro Cannon
		10,		// Meteor Mash
		15,		// Astonish
		10,		// Weather Ball
		5,		// Aromatherapy
		20,		// Fake Tears
		25,		// Air Cutter
		5,		// Overheat
		40,		// Odor Sleuth
		10,		// Rock Tomb
		5,		// Silver Wind
		40,		// Metal Sound
		15,		// GrassWhistle
		20,		// Tickle
		20,		// Cosmic Power
		5,		// Water Spout
		15,		// Signal Beam
		20,		// Shadow Punch
		30,		// Extrasensory
		15,		// Sky Uppercut
		15,		// Sand Tomb
		5,		// Sheer Cold
		10,		// Muddy Water
		30,		// Bullet Seed
		20,		// Aerial Ace
		30,		// Icicle Spear
		15,		// Iron Defense
		5,		// Block
		40,		// Howl
		15,		// Dragon Claw
		5,		// Frenzy Plant
		20,		// Bulk Up
		5,		// Bounce
		15,		// Mud Shot
		25,		// Poison Tail
		40,		// Covet
		15,		// Volt Tackle
		20,		// Magical Leaf
		15,		// Water Sport
		20,		// Calm Mind
		15,		// Leaf Blade
		20,		// Dragon Dance
		10,		// Rock Blast
		20,		// Shock Wave
		20,		// Water Pulse
		5,		// Doom Desire
		5,		// Psycho Boost
		10,		// Roost
		5,		// Gravity
		40,		// Miracle Eye
		10,		// Wake-Up Slap
		10,		// Hammer Arm
		5,		// Gyro Ball
		10,		// Healing Wish
		10,		// Brine
		15,		// Natural Gift
		10,		// Feint
		20,		// Pluck
		30,		// Tailwind
		30,		// Acupressure
		10,		// Metal Burst
		20,		// U-turn
		5,		// Close Combat
		10,		// Payback
		10,		// Assurance
		15,		// Embargo
		10,		// Fling
		10,		// Psycho Shift
		5,		// Trump Card
		15,		// Heal Block
		5,		// Wring Out
		10,		// Power Trick
		10,		// Gastro Acid
		30,		// Lucky Chant
		20,		// Me First
		20,		// Copycat
		10,		// Power Swap
		10,		// Guard Swap
		5,		// Punishment
		5,		// Last Resort
		10,		// Worry Seed
		5,		// Sucker Punch
		20,		// Toxic Spikes
		10,		// Heart Swap
		20,		// Aqua Ring
		10,		// Magnet Rise
		15,		// Flare Blitz
		10,		// Force Palm
		20,		// Aura Sphere
		20,		// Rock Polish
		20,		// Poison Jab
		15,		// Dark Pulse
		15,		// Night Slash
		10,		// Aqua Tail
		15,		// Seed Bomb
		20,		// Air Slash
		15,		// X-Scissor
		10,		// Bug Buzz
		10,		// Dragon Pulse
		10,		// Dragon Rush
		20,		// Power Gem
		10,		// Drain Punch
		30,		// Vacuum Wave
		5,		// Focus Blast
		10,		// Energy Ball
		15,		// Brave Bird
		10,		// Earth Power
		10,		// Switcheroo
		5,		// Giga Impact
		20,		// Nasty Plot
		30,		// Bullet Punch
		10,		// Avalanche
		30,		// Ice Shard
		15,		// Shadow Claw
		15,		// Thunder Fang
		15,		// Ice Fang
		15,		// Fire Fang
		30,		// Shadow Sneak
		10,		// Mud Bomb
		20,		// Psycho Cut
		15,		// Zen Headbutt
		10,		// Mirror Shot
		10,		// Flash Cannon
		20,		// Rock Climb
		15,		// Defog
		5,		// Trick Room
		5,		// Draco Meteor
		15,		// Discharge
		15,		// Lava Plume
		5,		// Leaf Storm
		10,		// Power Whip
		5,		// Rock Wrecker
		20,		// Cross Poison
		5,		// Gunk Shot
		15,		// Iron Head
		20,		// Magnet Bomb
		5,		// Stone Edge
		20,		// Captivate
		20,		// Stealth Rock
		20,		// Grass Knot
		20,		// Chatter
		10,		// Judgment
		20,		// Bug Bite
		10,		// Charge Beam
		15,		// Wood Hammer
		20,		// Aqua Jet
		15,		// Attack Order
		10,		// Defend Order
		10,		// Heal Order
		5,		// Head Smash
		10,		// Double Hit
		5,		// Roar of Time
		5,		// Spacial Rend
		10,		// Lunar Dance
		5,		// Crush Grip
		5,		// Magma Storm
		10,		// Dark Void
		5,		// Seed Flare
		5,		// Ominous Wind
		5,		// Shadow Force
		15,		// Hone Claws
		10,		// Wide Guard
		10,		// Guard Split
		10,		// Power Split
		10,		// Wonder Room
		10,		// Psyshock
		10,		// Venoshock
		15,		// Autotomize
		20,		// Rage Powder
		15,		// Telekinesis
		10,		// Magic Room
		15,		// Smack Down
		10,		// Storm Throw
		15,		// Flame Burst
		10,		// Sludge Wave
		20,		// Quiver Dance
		10,		// Heavy Slam
		15,		// Synchronoise
		10,		// Electro Ball
		20,		// Soak
		20,		// Flame Charge
		20,		// Coil
		20,		// Low Sweep
		20,		// Acid Spray
		15,		// Foul Play
		15,		// Simple Beam
		15,		// Entrainment
		15,		// After You
		15,		// Round
		15,		// Echoed Voice
		20,		// Chip Away
		15,		// Clear Smog
		10,		// Stored Power
		15,		// Quick Guard
		15,		// Ally Switch
		15,		// Scald
		15,		// Shell Smash
		10,		// Heal Pulse
		10,		// Hex
		10,		// Sky Drop
		10,		// Shift Gear
		10,		// Circle Throw
		15,		// Incinerate
		15,		// Quash
		15,		// Acrobatics
		15,		// Reflect Type
		5,		// Retaliate
		5,		// Final Gambit
		15,		// Bestow
		5,		// Inferno
		10,		// Water Pledge
		10,		// Fire Pledge
		10,		// Grass Pledge
		20,		// Volt Switch
		20,		// Struggle Bug
		20,		// Bulldoze
		10,		// Frost Breath
		10,		// Dragon Tail
		30,		// Work Up
		15,		// Electroweb
		15,		// Wild Charge
		10,		// Drill Run
		15,		// Dual Chop
		25,		// Heart Stamp
		10,		// Horn Leech
		20,		// Sacred Sword
		10,		// Razor Shell
		10,		// Heat Crash
		10,		// Leaf Tornado
		20,		// Steamroller
		10,		// Cotton Guard
		10,		// Night Daze
		10,		// Psystrike
		10,		// Tail Slap
		10,		// Hurricane
		15,		// Head Charge
		15,		// Gear Grind
		5,		// Searing Shot
		5,		// Techno Blast
		10,		// Relic Song
		10,		// Secret Sword
		10,		// Glaciate
		5,		// Bolt Strike
		5,		// Blue Flare
		10,		// Fiery Dance
		5,		// Freeze Shock
		5,		// Ice Burn
		15,		// Snarl
		10,		// Icicle Crash
		5,		// V-create
		5,		// Fusion Flare
		5		// Fusion Bolt
	};
	return get_pp[static_cast<unsigned>(move)];
}

}	// unnamed namespace
}	// namespace technicalmachine
