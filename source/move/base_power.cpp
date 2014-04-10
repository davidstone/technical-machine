// Base Power calculation
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

#include "base_power.hpp"
#include <bounded_integer/array.hpp>

namespace technicalmachine {
using namespace bounded::literal;

auto base_power(Moves const move) -> bounded::optional<bounded::integer<0, 250>> {
	// It doesn't matter if variable_power happens to have the same value as
	// anything else, as long as it is not 0.
	static constexpr auto variable_power = 1_bi;
	static constexpr auto power = bounded::make_optional_array(
		0_bi,		// Switch0
		0_bi,		// Switch1
		0_bi,		// Switch2
		0_bi,		// Switch3
		0_bi,		// Switch4
		0_bi,		// Switch5
		40_bi,		// Hit Self
		40_bi,		// Pound
		50_bi,		// Karate Chop
		15_bi,		// DoubleSlap
		18_bi,		// Comet Punch
		80_bi,		// Mega Punch
		40_bi,		// Pay Day
		75_bi,		// Fire Punch
		75_bi,		// Ice Punch
		75_bi,		// ThunderPunch
		40_bi,		// Scratch
		55_bi,		// ViceGrip
		bounded::none,		// Guillotine
		80_bi,		// Razor Wind
		0_bi,		// Swords Dance
		50_bi,		// Cut
		40_bi,		// Gust
		60_bi,		// Wing Attack
		0_bi,		// Whirlwind
		90_bi,		// Fly
		15_bi,		// Bind
		80_bi,		// Slam
		35_bi,		// Vine Whip
		65_bi,		// Stomp
		30_bi,		// Double Kick
		120_bi,		// Mega Kick
		100_bi,		// Jump Kick
		60_bi,		// Rolling Kick
		0_bi,		// Sand-Attack
		70_bi,		// Headbutt
		65_bi,		// Horn Attack
		15_bi,		// Fury Attack
		bounded::none,		// Horn Drill
		50_bi,		// Tackle
		85_bi,		// Body Slam
		15_bi,		// Wrap
		90_bi,		// Take Down
		120_bi,		// Thrash
		120_bi,		// Double-Edge
		0_bi,		// Tail Whip
		15_bi,		// Poison Sting
		25_bi,		// Twineedle
		14_bi,		// Pin Missile
		0_bi,		// Leer
		60_bi,		// Bite
		0_bi,		// Growl
		0_bi,		// Roar
		0_bi,		// Sing
		0_bi,		// Supersonic
		bounded::none,		// SonicBoom
		0_bi,		// Disable
		40_bi,		// Acid
		40_bi,		// Ember
		95_bi,		// Flamethrower
		0_bi,		// Mist
		40_bi,		// Water Gun
		120_bi,		// Hydro Pump
		95_bi,		// Surf
		95_bi,		// Ice Beam
		120_bi,		// Blizzard
		65_bi,		// Psybeam
		65_bi,		// BubbleBeam
		65_bi,		// Aurora Beam
		150_bi,		// Hyper Beam
		35_bi,		// Peck
		80_bi,		// Drill Peck
		80_bi,		// Submission
		variable_power,		// Low Kick
		bounded::none,		// Counter
		bounded::none,		// Seismic Toss
		80_bi,		// Strength
		20_bi,		// Absorb
		40_bi,		// Mega Drain
		0_bi,		// Leech Seed
		0_bi,		// Growth
		55_bi,		// Razor Leaf
		120_bi,		// SolarBeam
		0_bi,		// PoisonPowder
		0_bi,		// Stun Spore
		0_bi,		// Sleep Powder
		120_bi,		// Petal Dance
		0_bi,		// String Shot
		bounded::none,		// Dragon Rage
		35_bi,		// Fire Spin
		40_bi,		// ThunderShock
		95_bi,		// Thunderbolt
		0_bi,		// Thunder Wave
		120_bi,		// Thunder
		50_bi,		// Rock Throw
		100_bi,		// Earthquake
		bounded::none,		// Fissure
		80_bi,		// Dig
		0_bi,		// Toxic
		50_bi,		// Confusion
		90_bi,		// Psychic
		0_bi,		// Hypnosis
		0_bi,		// Meditate
		0_bi,		// Agility
		40_bi,		// Quick Attack
		20_bi,		// Rage
		0_bi,		// Teleport
		bounded::none,		// Night Shade
		0_bi,		// Mimic
		0_bi,		// Screech
		0_bi,		// Double Team
		0_bi,		// Recover
		0_bi,		// Harden
		0_bi,		// Minimize
		0_bi,		// SmokeScreen
		0_bi,		// Confuse Ray
		0_bi,		// Withdraw
		0_bi,		// Defense Curl
		0_bi,		// Barrier
		0_bi,		// Light Screen
		0_bi,		// Haze
		0_bi,		// Reflect
		0_bi,		// Focus Energy
		variable_power,		// Bide
		0_bi,		// Metronome
		0_bi,		// Mirror Move
		200_bi,		// Selfdestruct
		100_bi,		// Egg Bomb
		20_bi,		// Lick
		20_bi,		// Smog
		65_bi,		// Sludge
		65_bi,		// Bone Club
		120_bi,		// Fire Blast
		80_bi,		// Waterfall
		35_bi,		// Clamp
		60_bi,		// Swift
		100_bi,		// Skull Bash
		20_bi,		// Spike Cannon
		10_bi,		// Constrict
		0_bi,		// Amnesia
		0_bi,		// Kinesis
		0_bi,		// Softboiled
		130_bi,		// Hi Jump Kick
		0_bi,		// Glare
		100_bi,		// Dream Eater
		0_bi,		// Poison Gas
		15_bi,		// Barrage
		20_bi,		// Leech Life
		0_bi,		// Lovely Kiss
		140_bi,		// Sky Attack
		0_bi,		// Transform
		20_bi,		// Bubble
		70_bi,		// Dizzy Punch
		0_bi,		// Spore
		0_bi,		// Flash
		bounded::none,		// Psywave
		0_bi,		// Splash
		0_bi,		// Acid Armor
		90_bi,		// Crabhammer
		250_bi,		// Explosion
		18_bi,		// Fury Swipes
		50_bi,		// Bonemerang
		0_bi,		// Rest
		75_bi,		// Rock Slide
		80_bi,		// Hyper Fang
		0_bi,		// Sharpen
		0_bi,		// Conversion
		80_bi,		// Tri Attack
		bounded::none,		// Super Fang
		70_bi,		// Slash
		0_bi,		// Substitute
		50_bi,		// Struggle
		0_bi,		// Sketch
		10_bi,		// Triple Kick
		40_bi,		// Thief
		0_bi,		// Spider Web
		0_bi,		// Mind Reader
		0_bi,		// Nightmare
		60_bi,		// Flame Wheel
		40_bi,		// Snore
		0_bi,		// Curse
		variable_power,		// Flail
		0_bi,		// Conversion 2
		100_bi,		// Aeroblast
		0_bi,		// Cotton Spore
		variable_power,		// Reversal
		0_bi,		// Spite
		40_bi,		// Powder Snow
		0_bi,		// Protect
		40_bi,		// Mach Punch
		0_bi,		// Scary Face
		60_bi,		// Faint Attack
		0_bi,		// Sweet Kiss
		0_bi,		// Belly Drum
		90_bi,		// Sludge Bomb
		20_bi,		// Mud-Slap
		65_bi,		// Octazooka
		0_bi,		// Spikes
		120_bi,		// Zap Cannon
		0_bi,		// Foresight
		0_bi,		// Destiny Bond
		0_bi,		// Perish Song
		55_bi,		// Icy Wind
		0_bi,		// Detect
		25_bi,		// Bone Rush
		0_bi,		// Lock-On
		120_bi,		// Outrage
		0_bi,		// Sandstorm
		75_bi,		// Giga Drain
		0_bi,		// Endure
		0_bi,		// Charm
		30_bi,		// Rollout
		40_bi,		// False Swipe
		0_bi,		// Swagger
		0_bi,		// Milk Drink
		65_bi,		// Spark
		20_bi,		// Fury Cutter
		70_bi,		// Steel Wing
		0_bi,		// Mean Look
		0_bi,		// Attract
		0_bi,		// Sleep Talk
		0_bi,		// Heal Bell
		variable_power,		// Return
		variable_power,		// Present
		variable_power,		// Frustration
		0_bi,		// Safeguard
		0_bi,		// Pain Split
		100_bi,		// Sacred Fire
		variable_power,		// Magnitude
		100_bi,		// DynamicPunch
		120_bi,		// Megahorn
		60_bi,		// DragonBreath
		0_bi,		// Baton Pass
		0_bi,		// Encore
		40_bi,		// Pursuit
		20_bi,		// Rapid Spin
		0_bi,		// Sweet Scent
		100_bi,		// Iron Tail
		50_bi,		// Metal Claw
		70_bi,		// Vital Throw
		0_bi,		// Morning Sun
		0_bi,		// Synthesis
		0_bi,		// Moonlight
		variable_power,		// Hidden Power
		100_bi,		// Cross Chop
		40_bi,		// Twister
		0_bi,		// Rain Dance
		0_bi,		// Sunny Day
		80_bi,		// Crunch
		bounded::none,		// Mirror Coat
		0_bi,		// Psych Up
		80_bi,		// ExtremeSpeed
		60_bi,		// AncientPower
		80_bi,		// Shadow Ball
		100_bi,		// Future Sight
		40_bi,		// Rock Smash
		35_bi,		// Whirlpool
		variable_power,		// Beat Up
		40_bi,		// Fake Out
		90_bi,		// Uproar
		0_bi,		// Stockpile
		variable_power,		// Spit Up
		0_bi,		// Swallow
		100_bi,		// Heat Wave
		0_bi,		// Hail
		0_bi,		// Torment
		0_bi,		// Flatter
		0_bi,		// Will-O-Wisp
		0_bi,		// Memento
		70_bi,		// Facade
		150_bi,		// Focus Punch
		60_bi,		// SmellingSalt
		0_bi,		// Follow Me
		0_bi,		// Nature Power
		0_bi,		// Charge
		0_bi,		// Taunt
		0_bi,		// Helping Hand
		0_bi,		// Trick
		0_bi,		// Role Play
		0_bi,		// Wish
		0_bi,		// Assist
		0_bi,		// Ingrain
		120_bi,		// Superpower
		0_bi,		// Magic Coat
		0_bi,		// Recycle
		60_bi,		// Revenge
		75_bi,		// Brick Break
		0_bi,		// Yawn
		20_bi,		// Knock Off
		bounded::none,		// Endeavor
		150_bi,		// Eruption
		0_bi,		// Skill Swap
		0_bi,		// Imprison
		0_bi,		// Refresh
		0_bi,		// Grudge
		0_bi,		// Snatch
		70_bi,		// Secret Power
		80_bi,		// Dive
		15_bi,		// Arm Thrust
		0_bi,		// Camouflage
		0_bi,		// Tail Glow
		70_bi,		// Luster Purge
		70_bi,		// Mist Ball
		0_bi,		// FeatherDance
		0_bi,		// Teeter Dance
		85_bi,		// Blaze Kick
		0_bi,		// Mud Sport
		30_bi,		// Ice Ball
		60_bi,		// Needle Arm
		0_bi,		// Slack Off
		90_bi,		// Hyper Voice
		50_bi,		// Poison Fang
		75_bi,		// Crush Claw
		150_bi,		// Blast Burn
		150_bi,		// Hydro Cannon
		100_bi,		// Meteor Mash
		30_bi,		// Astonish
		50_bi,		// Weather Ball
		0_bi,		// Aromatherapy
		0_bi,		// Fake Tears
		55_bi,		// Air Cutter
		140_bi,		// Overheat
		0_bi,		// Odor Sleuth
		50_bi,		// Rock Tomb
		60_bi,		// Silver Wind
		0_bi,		// Metal Sound
		0_bi,		// GrassWhistle
		0_bi,		// Tickle
		0_bi,		// Cosmic Power
		150_bi,		// Water Spout
		75_bi,		// Signal Beam
		60_bi,		// Shadow Punch
		80_bi,		// Extrasensory
		85_bi,		// Sky Uppercut
		35_bi,		// Sand Tomb
		bounded::none,		// Sheer Cold
		95_bi,		// Muddy Water
		25_bi,		// Bullet Seed
		60_bi,		// Aerial Ace
		25_bi,		// Icicle Spear
		0_bi,		// Iron Defense
		0_bi,		// Block
		0_bi,		// Howl
		80_bi,		// Dragon Claw
		150_bi,		// Frenzy Plant
		0_bi,		// Bulk Up
		85_bi,		// Bounce
		55_bi,		// Mud Shot
		50_bi,		// Poison Tail
		60_bi,		// Covet
		120_bi,		// Volt Tackle
		60_bi,		// Magical Leaf
		0_bi,		// Water Sport
		0_bi,		// Calm Mind
		90_bi,		// Leaf Blade
		0_bi,		// Dragon Dance
		25_bi,		// Rock Blast
		60_bi,		// Shock Wave
		60_bi,		// Water Pulse
		140_bi,		// Doom Desire
		140_bi,		// Psycho Boost
		0_bi,		// Roost
		0_bi,		// Gravity
		0_bi,		// Miracle Eye
		60_bi,		// Wake-Up Slap
		100_bi,		// Hammer Arm
		variable_power,		// Gyro Ball
		0_bi,		// Healing Wish
		65_bi,		// Brine
		variable_power,		// Natural Gift
		30_bi,		// Feint
		60_bi,		// Pluck
		0_bi,		// Tailwind
		0_bi,		// Acupressure
		bounded::none,		// Metal Burst
		70_bi,		// U-turn
		120_bi,		// Close Combat
		50_bi,		// Payback
		50_bi,		// Assurance
		0_bi,		// Embargo
		variable_power,		// Fling
		0_bi,		// Psycho Shift
		variable_power,		// Trump Card
		0_bi,		// Heal Block
		variable_power,		// Wring Out
		0_bi,		// Power Trick
		0_bi,		// Gastro Acid
		0_bi,		// Lucky Chant
		variable_power,		// Me First
		0_bi,		// Copycat
		0_bi,		// Power Swap
		0_bi,		// Guard Swap
		variable_power,		// Punishment
		140_bi,		// Last Resort
		0_bi,		// Worry Seed
		80_bi,		// Sucker Punch
		0_bi,		// Toxic Spikes
		0_bi,		// Heart Swap
		0_bi,		// Aqua Ring
		0_bi,		// Magnet Rise
		120_bi,		// Flare Blitz
		60_bi,		// Force Palm
		90_bi,		// Aura Sphere
		0_bi,		// Rock Polish
		80_bi,		// Poison Jab
		80_bi,		// Dark Pulse
		70_bi,		// Night Slash
		90_bi,		// Aqua Tail
		80_bi,		// Seed Bomb
		75_bi,		// Air Slash
		80_bi,		// X-Scissor
		90_bi,		// Bug Buzz
		90_bi,		// Dragon Pulse
		100_bi,		// Dragon Rush
		70_bi,		// Power Gem
		75_bi,		// Drain Punch
		40_bi,		// Vacuum Wave
		120_bi,		// Focus Blast
		80_bi,		// Energy Ball
		120_bi,		// Brave Bird
		90_bi,		// Earth Power
		0_bi,		// Switcheroo
		150_bi,		// Giga Impact
		0_bi,		// Nasty Plot
		40_bi,		// Bullet Punch
		60_bi,		// Avalanche
		40_bi,		// Ice Shard
		70_bi,		// Shadow Claw
		65_bi,		// Thunder Fang
		65_bi,		// Ice Fang
		65_bi,		// Fire Fang
		40_bi,		// Shadow Sneak
		65_bi,		// Mud Bomb
		70_bi,		// Psycho Cut
		80_bi,		// Zen Headbutt
		65_bi,		// Mirror Shot
		80_bi,		// Flash Cannon
		90_bi,		// Rock Climb
		0_bi,		// Defog
		0_bi,		// Trick Room
		140_bi,		// Draco Meteor
		80_bi,		// Discharge
		80_bi,		// Lava Plume
		140_bi,		// Leaf Storm
		120_bi,		// Power Whip
		150_bi,		// Rock Wrecker
		70_bi,		// Cross Poison
		120_bi,		// Gunk Shot
		80_bi,		// Iron Head
		60_bi,		// Magnet Bomb
		100_bi,		// Stone Edge
		0_bi,		// Captivate
		0_bi,		// Stealth Rock
		variable_power,		// Grass Knot
		60_bi,		// Chatter
		100_bi,		// Judgment
		60_bi,		// Bug Bite
		50_bi,		// Charge Beam
		120_bi,		// Wood Hammer
		40_bi,		// Aqua Jet
		90_bi,		// Attack Order
		0_bi,		// Defend Order
		0_bi,		// Heal Order
		150_bi,		// Head Smash
		35_bi,		// Double Hit
		150_bi,		// Roar of Time
		100_bi,		// Spacial Rend
		0_bi,		// Lunar Dance
		variable_power,		// Crush Grip
		120_bi,		// Magma Storm
		0_bi,		// Dark Void
		120_bi,		// Seed Flare
		60_bi,		// Ominous Wind
		120_bi,		// Shadow Force
		0_bi,		// Hone Claws
		0_bi,		// Wide Guard
		0_bi,		// Guard Split
		0_bi,		// Power Split
		0_bi,		// Wonder Room
		80_bi,		// Psyshock
		65_bi,		// Venoshock
		0_bi,		// Autotomize
		0_bi,		// Rage Powder
		0_bi,		// Telekinesis
		0_bi,		// Magic Room
		50_bi,		// Smack Down
		40_bi,		// Storm Throw
		70_bi,		// Flame Burst
		95_bi,		// Sludge Wave
		0_bi,		// Quiver Dance
		variable_power,		// Heavy Slam
		70_bi,		// Synchronoise
		variable_power,		// Electro Ball
		0_bi,		// Soak
		50_bi,		// Flame Charge
		0_bi,		// Coil
		60_bi,		// Low Sweep
		40_bi,		// Acid Spray
		95_bi,		// Foul Play
		0_bi,		// Simple Beam
		0_bi,		// Entrainment
		0_bi,		// After You
		60_bi,		// Round
		40_bi,		// Echoed Voice
		70_bi,		// Chip Away
		50_bi,		// Clear Smog
		20_bi,		// Stored Power
		0_bi,		// Quick Guard
		0_bi,		// Ally Switch
		80_bi,		// Scald
		0_bi,		// Shell Smash
		0_bi,		// Heal Pulse
		50_bi,		// Hex
		60_bi,		// Sky Drop
		0_bi,		// Shift Gear
		60_bi,		// Circle Throw
		30_bi,		// Incinerate
		0_bi,		// Quash
		55_bi,		// Acrobatics
		0_bi,		// Reflect Type
		70_bi,		// Retaliate
		bounded::none,		// Final Gambit
		0_bi,		// Bestow
		100_bi,		// Inferno
		50_bi,		// Water Pledge
		50_bi,		// Fire Pledge
		50_bi,		// Grass Pledge
		70_bi,		// Volt Switch
		30_bi,		// Struggle Bug
		60_bi,		// Bulldoze
		40_bi,		// Frost Breath
		60_bi,		// Dragon Tail
		0_bi,		// Work Up
		55_bi,		// Electroweb
		90_bi,		// Wild Charge
		80_bi,		// Drill Run
		40_bi,		// Dual Chop
		60_bi,		// Heart Stamp
		75_bi,		// Horn Leech
		90_bi,		// Sacred Sword
		75_bi,		// Razor Shell
		variable_power,		// Heat Crash
		65_bi,		// Leaf Tornado
		65_bi,		// Steamroller
		0_bi,		// Cotton Guard
		85_bi,		// Night Daze
		100_bi,		// Psystrike
		25_bi,		// Tail Slap
		120_bi,		// Hurricane
		120_bi,		// Head Charge
		50_bi,		// Gear Grind
		100_bi,		// Searing Shot
		85_bi,		// Techno Blast
		75_bi,		// Relic Song
		85_bi,		// Secret Sword
		65_bi,		// Glaciate
		130_bi,		// Bolt Strike
		130_bi,		// Blue Flare
		80_bi,		// Fiery Dance
		140_bi,		// Freeze Shock
		140_bi,		// Ice Burn
		55_bi,		// Snarl
		85_bi,		// Icicle Crash
		180_bi,		// V-create
		100_bi,		// Fusion Flare
		100_bi		// Fusion Bolt
	);
	return power.at(move);
}

}	// namespace technicalmachine
