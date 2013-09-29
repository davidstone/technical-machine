// Base Power lookup
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

#include "base_power.hpp"
#include <cstddef>

namespace technicalmachine {
namespace {

constexpr uint8_t fixed_damage = 0xFF;
constexpr uint8_t variable_power = fixed_damage - 1;
uint8_t get_base_power (Moves move);

}	// unnamed namespace

BasePower::BasePower(Moves const move) :
	base_power(get_base_power(move))
	{
}

unsigned BasePower::operator()() const {
	return base_power;
}

bool BasePower::does_fixed_damage() const {
	return base_power == fixed_damage;
}


namespace {

uint8_t get_base_power(Moves const move) {
	static constexpr uint8_t get_power [] = {
		0,		// Switch0
		0,		// Switch1
		0,		// Switch2
		0,		// Switch3
		0,		// Switch4
		0,		// Switch5
		40,		// Hit Self
		40,		// Pound
		50,		// Karate Chop
		15,		// DoubleSlap
		18,		// Comet Punch
		80,		// Mega Punch
		40,		// Pay Day
		75,		// Fire Punch
		75,		// Ice Punch
		75,		// ThunderPunch
		40,		// Scratch
		55,		// ViceGrip
		fixed_damage,		// Guillotine
		80,		// Razor Wind
		0,		// Swords Dance
		50,		// Cut
		40,		// Gust
		60,		// Wing Attack
		0,		// Whirlwind
		90,		// Fly
		15,		// Bind
		80,		// Slam
		35,		// Vine Whip
		65,		// Stomp
		30,		// Double Kick
		120,		// Mega Kick
		100,		// Jump Kick
		60,		// Rolling Kick
		0,		// Sand-Attack
		70,		// Headbutt
		65,		// Horn Attack
		15,		// Fury Attack
		fixed_damage,		// Horn Drill
		50,		// Tackle
		85,		// Body Slam
		15,		// Wrap
		90,		// Take Down
		120,		// Thrash
		120,		// Double-Edge
		0,		// Tail Whip
		15,		// Poison Sting
		25,		// Twineedle
		14,		// Pin Missile
		0,		// Leer
		60,		// Bite
		0,		// Growl
		0,		// Roar
		0,		// Sing
		0,		// Supersonic
		fixed_damage,		// SonicBoom
		0,		// Disable
		40,		// Acid
		40,		// Ember
		95,		// Flamethrower
		0,		// Mist
		40,		// Water Gun
		120,		// Hydro Pump
		95,		// Surf
		95,		// Ice Beam
		120,		// Blizzard
		65,		// Psybeam
		65,		// BubbleBeam
		65,		// Aurora Beam
		150,		// Hyper Beam
		35,		// Peck
		80,		// Drill Peck
		80,		// Submission
		variable_power,		// Low Kick
		fixed_damage,		// Counter
		fixed_damage,		// Seismic Toss
		80,		// Strength
		20,		// Absorb
		40,		// Mega Drain
		0,		// Leech Seed
		0,		// Growth
		55,		// Razor Leaf
		120,		// SolarBeam
		0,		// PoisonPowder
		0,		// Stun Spore
		0,		// Sleep Powder
		120,		// Petal Dance
		0,		// String Shot
		fixed_damage,		// Dragon Rage
		35,		// Fire Spin
		40,		// ThunderShock
		95,		// Thunderbolt
		0,		// Thunder Wave
		120,		// Thunder
		50,		// Rock Throw
		100,		// Earthquake
		fixed_damage,		// Fissure
		80,		// Dig
		0,		// Toxic
		50,		// Confusion
		90,		// Psychic
		0,		// Hypnosis
		0,		// Meditate
		0,		// Agility
		40,		// Quick Attack
		20,		// Rage
		0,		// Teleport
		fixed_damage,		// Night Shade
		0,		// Mimic
		0,		// Screech
		0,		// Double Team
		0,		// Recover
		0,		// Harden
		0,		// Minimize
		0,		// SmokeScreen
		0,		// Confuse Ray
		0,		// Withdraw
		0,		// Defense Curl
		0,		// Barrier
		0,		// Light Screen
		0,		// Haze
		0,		// Reflect
		0,		// Focus Energy
		variable_power,		// Bide
		0,		// Metronome
		0,		// Mirror Move
		200,		// Selfdestruct
		100,		// Egg Bomb
		20,		// Lick
		20,		// Smog
		65,		// Sludge
		65,		// Bone Club
		120,		// Fire Blast
		80,		// Waterfall
		35,		// Clamp
		60,		// Swift
		100,		// Skull Bash
		20,		// Spike Cannon
		10,		// Constrict
		0,		// Amnesia
		0,		// Kinesis
		0,		// Softboiled
		130,		// Hi Jump Kick
		0,		// Glare
		100,		// Dream Eater
		0,		// Poison Gas
		15,		// Barrage
		20,		// Leech Life
		0,		// Lovely Kiss
		140,		// Sky Attack
		0,		// Transform
		20,		// Bubble
		70,		// Dizzy Punch
		0,		// Spore
		0,		// Flash
		fixed_damage,		// Psywave
		0,		// Splash
		0,		// Acid Armor
		90,		// Crabhammer
		250,		// Explosion
		18,		// Fury Swipes
		50,		// Bonemerang
		0,		// Rest
		75,		// Rock Slide
		80,		// Hyper Fang
		0,		// Sharpen
		0,		// Conversion
		80,		// Tri Attack
		fixed_damage,		// Super Fang
		70,		// Slash
		0,		// Substitute
		50,		// Struggle
		0,		// Sketch
		10,		// Triple Kick
		40,		// Thief
		0,		// Spider Web
		0,		// Mind Reader
		0,		// Nightmare
		60,		// Flame Wheel
		40,		// Snore
		0,		// Curse
		variable_power,		// Flail
		0,		// Conversion 2
		100,		// Aeroblast
		0,		// Cotton Spore
		variable_power,		// Reversal
		0,		// Spite
		40,		// Powder Snow
		0,		// Protect
		40,		// Mach Punch
		0,		// Scary Face
		60,		// Faint Attack
		0,		// Sweet Kiss
		0,		// Belly Drum
		90,		// Sludge Bomb
		20,		// Mud-Slap
		65,		// Octazooka
		0,		// Spikes
		120,		// Zap Cannon
		0,		// Foresight
		0,		// Destiny Bond
		0,		// Perish Song
		55,		// Icy Wind
		0,		// Detect
		25,		// Bone Rush
		0,		// Lock-On
		120,		// Outrage
		0,		// Sandstorm
		75,		// Giga Drain
		0,		// Endure
		0,		// Charm
		30,		// Rollout
		40,		// False Swipe
		0,		// Swagger
		0,		// Milk Drink
		65,		// Spark
		20,		// Fury Cutter
		70,		// Steel Wing
		0,		// Mean Look
		0,		// Attract
		0,		// Sleep Talk
		0,		// Heal Bell
		variable_power,		// Return
		variable_power,		// Present
		variable_power,		// Frustration
		0,		// Safeguard
		0,		// Pain Split
		100,		// Sacred Fire
		variable_power,		// Magnitude
		100,		// DynamicPunch
		120,		// Megahorn
		60,		// DragonBreath
		0,		// Baton Pass
		0,		// Encore
		40,		// Pursuit
		20,		// Rapid Spin
		0,		// Sweet Scent
		100,		// Iron Tail
		50,		// Metal Claw
		70,		// Vital Throw
		0,		// Morning Sun
		0,		// Synthesis
		0,		// Moonlight
		variable_power,		// Hidden Power
		100,		// Cross Chop
		40,		// Twister
		0,		// Rain Dance
		0,		// Sunny Day
		80,		// Crunch
		fixed_damage,		// Mirror Coat
		0,		// Psych Up
		80,		// ExtremeSpeed
		60,		// AncientPower
		80,		// Shadow Ball
		100,		// Future Sight
		40,		// Rock Smash
		35,		// Whirlpool
		variable_power,		// Beat Up
		40,		// Fake Out
		90,		// Uproar
		0,		// Stockpile
		variable_power,		// Spit Up
		0,		// Swallow
		100,		// Heat Wave
		0,		// Hail
		0,		// Torment
		0,		// Flatter
		0,		// Will-O-Wisp
		0,		// Memento
		70,		// Facade
		150,		// Focus Punch
		60,		// SmellingSalt
		0,		// Follow Me
		0,		// Nature Power
		0,		// Charge
		0,		// Taunt
		0,		// Helping Hand
		0,		// Trick
		0,		// Role Play
		0,		// Wish
		0,		// Assist
		0,		// Ingrain
		120,		// Superpower
		0,		// Magic Coat
		0,		// Recycle
		60,		// Revenge
		75,		// Brick Break
		0,		// Yawn
		20,		// Knock Off
		fixed_damage,		// Endeavor
		150,		// Eruption
		0,		// Skill Swap
		0,		// Imprison
		0,		// Refresh
		0,		// Grudge
		0,		// Snatch
		70,		// Secret Power
		80,		// Dive
		15,		// Arm Thrust
		0,		// Camouflage
		0,		// Tail Glow
		70,		// Luster Purge
		70,		// Mist Ball
		0,		// FeatherDance
		0,		// Teeter Dance
		85,		// Blaze Kick
		0,		// Mud Sport
		30,		// Ice Ball
		60,		// Needle Arm
		0,		// Slack Off
		90,		// Hyper Voice
		50,		// Poison Fang
		75,		// Crush Claw
		150,		// Blast Burn
		150,		// Hydro Cannon
		100,		// Meteor Mash
		30,		// Astonish
		50,		// Weather Ball
		0,		// Aromatherapy
		0,		// Fake Tears
		55,		// Air Cutter
		140,		// Overheat
		0,		// Odor Sleuth
		50,		// Rock Tomb
		60,		// Silver Wind
		0,		// Metal Sound
		0,		// GrassWhistle
		0,		// Tickle
		0,		// Cosmic Power
		150,		// Water Spout
		75,		// Signal Beam
		60,		// Shadow Punch
		80,		// Extrasensory
		85,		// Sky Uppercut
		35,		// Sand Tomb
		fixed_damage,		// Sheer Cold
		95,		// Muddy Water
		25,		// Bullet Seed
		60,		// Aerial Ace
		25,		// Icicle Spear
		0,		// Iron Defense
		0,		// Block
		0,		// Howl
		80,		// Dragon Claw
		150,		// Frenzy Plant
		0,		// Bulk Up
		85,		// Bounce
		55,		// Mud Shot
		50,		// Poison Tail
		60,		// Covet
		120,		// Volt Tackle
		60,		// Magical Leaf
		0,		// Water Sport
		0,		// Calm Mind
		90,		// Leaf Blade
		0,		// Dragon Dance
		25,		// Rock Blast
		60,		// Shock Wave
		60,		// Water Pulse
		140,		// Doom Desire
		140,		// Psycho Boost
		0,		// Roost
		0,		// Gravity
		0,		// Miracle Eye
		60,		// Wake-Up Slap
		100,		// Hammer Arm
		variable_power,		// Gyro Ball
		0,		// Healing Wish
		65,		// Brine
		variable_power,		// Natural Gift
		30,		// Feint
		60,		// Pluck
		0,		// Tailwind
		0,		// Acupressure
		fixed_damage,		// Metal Burst
		70,		// U-turn
		120,		// Close Combat
		50,		// Payback
		50,		// Assurance
		0,		// Embargo
		variable_power,		// Fling
		0,		// Psycho Shift
		variable_power,		// Trump Card
		0,		// Heal Block
		variable_power,		// Wring Out
		0,		// Power Trick
		0,		// Gastro Acid
		0,		// Lucky Chant
		variable_power,		// Me First
		0,		// Copycat
		0,		// Power Swap
		0,		// Guard Swap
		variable_power,		// Punishment
		140,		// Last Resort
		0,		// Worry Seed
		80,		// Sucker Punch
		0,		// Toxic Spikes
		0,		// Heart Swap
		0,		// Aqua Ring
		0,		// Magnet Rise
		120,		// Flare Blitz
		60,		// Force Palm
		90,		// Aura Sphere
		0,		// Rock Polish
		80,		// Poison Jab
		80,		// Dark Pulse
		70,		// Night Slash
		90,		// Aqua Tail
		80,		// Seed Bomb
		75,		// Air Slash
		80,		// X-Scissor
		90,		// Bug Buzz
		90,		// Dragon Pulse
		100,		// Dragon Rush
		70,		// Power Gem
		75,		// Drain Punch
		40,		// Vacuum Wave
		120,		// Focus Blast
		80,		// Energy Ball
		120,		// Brave Bird
		90,		// Earth Power
		0,		// Switcheroo
		150,		// Giga Impact
		0,		// Nasty Plot
		40,		// Bullet Punch
		60,		// Avalanche
		40,		// Ice Shard
		70,		// Shadow Claw
		65,		// Thunder Fang
		65,		// Ice Fang
		65,		// Fire Fang
		40,		// Shadow Sneak
		65,		// Mud Bomb
		70,		// Psycho Cut
		80,		// Zen Headbutt
		65,		// Mirror Shot
		80,		// Flash Cannon
		90,		// Rock Climb
		0,		// Defog
		0,		// Trick Room
		140,		// Draco Meteor
		80,		// Discharge
		80,		// Lava Plume
		140,		// Leaf Storm
		120,		// Power Whip
		150,		// Rock Wrecker
		70,		// Cross Poison
		120,		// Gunk Shot
		80,		// Iron Head
		60,		// Magnet Bomb
		100,		// Stone Edge
		0,		// Captivate
		0,		// Stealth Rock
		variable_power,		// Grass Knot
		60,		// Chatter
		100,		// Judgment
		60,		// Bug Bite
		50,		// Charge Beam
		120,		// Wood Hammer
		40,		// Aqua Jet
		90,		// Attack Order
		0,		// Defend Order
		0,		// Heal Order
		150,		// Head Smash
		35,		// Double Hit
		150,		// Roar of Time
		100,		// Spacial Rend
		0,		// Lunar Dance
		variable_power,		// Crush Grip
		120,		// Magma Storm
		0,		// Dark Void
		120,		// Seed Flare
		60,		// Ominous Wind
		120,		// Shadow Force
		0,		// Hone Claws
		0,		// Wide Guard
		0,		// Guard Split
		0,		// Power Split
		0,		// Wonder Room
		80,		// Psyshock
		65,		// Venoshock
		0,		// Autotomize
		0,		// Rage Powder
		0,		// Telekinesis
		0,		// Magic Room
		50,		// Smack Down
		40,		// Storm Throw
		70,		// Flame Burst
		95,		// Sludge Wave
		0,		// Quiver Dance
		variable_power,		// Heavy Slam
		70,		// Synchronoise
		variable_power,		// Electro Ball
		0,		// Soak
		50,		// Flame Charge
		0,		// Coil
		60,		// Low Sweep
		40,		// Acid Spray
		95,		// Foul Play
		0,		// Simple Beam
		0,		// Entrainment
		0,		// After You
		60,		// Round
		40,		// Echoed Voice
		70,		// Chip Away
		50,		// Clear Smog
		20,		// Stored Power
		0,		// Quick Guard
		0,		// Ally Switch
		80,		// Scald
		0,		// Shell Smash
		0,		// Heal Pulse
		50,		// Hex
		60,		// Sky Drop
		0,		// Shift Gear
		60,		// Circle Throw
		30,		// Incinerate
		0,		// Quash
		55,		// Acrobatics
		0,		// Reflect Type
		70,		// Retaliate
		fixed_damage,		// Final Gambit
		0,		// Bestow
		100,		// Inferno
		50,		// Water Pledge
		50,		// Fire Pledge
		50,		// Grass Pledge
		70,		// Volt Switch
		30,		// Struggle Bug
		60,		// Bulldoze
		40,		// Frost Breath
		60,		// Dragon Tail
		0,		// Work Up
		55,		// Electroweb
		90,		// Wild Charge
		80,		// Drill Run
		40,		// Dual Chop
		60,		// Heart Stamp
		75,		// Horn Leech
		90,		// Sacred Sword
		75,		// Razor Shell
		variable_power,		// Heat Crash
		65,		// Leaf Tornado
		65,		// Steamroller
		0,		// Cotton Guard
		85,		// Night Daze
		100,		// Psystrike
		25,		// Tail Slap
		120,		// Hurricane
		120,		// Head Charge
		50,		// Gear Grind
		100,		// Searing Shot
		85,		// Techno Blast
		75,		// Relic Song
		85,		// Secret Sword
		65,		// Glaciate
		130,		// Bolt Strike
		130,		// Blue Flare
		80,		// Fiery Dance
		140,		// Freeze Shock
		140,		// Ice Burn
		55,		// Snarl
		85,		// Icicle Crash
		180,		// V-create
		100,		// Fusion Flare
		100		// Fusion Bolt
	};
	return get_power[static_cast<size_t>(move)];
}

}	// unnamed namespace

}	// namespace technicalmachine
