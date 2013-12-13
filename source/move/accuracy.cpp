// Accuracy functions
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

#include "accuracy.hpp"
#include "moves.hpp"
#include <type_traits>
#include <utility>

namespace technicalmachine {
using namespace bounded_integer::literal;

BaseAccuracy accuracy(Moves const move) {
	using bounded_integer::none;
	static constexpr auto get_accuracy = make_optional_array(
		none,		// Switch0
		none,		// Switch1
		none,		// Switch2
		none,		// Switch3
		none,		// Switch4
		none,		// Switch5
		none,		// Hit Self
		100_bi,		// Pound
		100_bi,		// Karate Chop
		85_bi,		// DoubleSlap
		85_bi,		// Comet Punch
		85_bi,		// Mega Punch
		100_bi,		// Pay Day
		100_bi,		// Fire Punch
		100_bi,		// Ice Punch
		100_bi,		// ThunderPunch
		100_bi,		// Scratch
		100_bi,		// ViceGrip
		30_bi,		// Guillotine
		100_bi,		// Razor Wind
		none,		// Swords Dance
		95_bi,		// Cut
		100_bi,		// Gust
		100_bi,		// Wing Attack
		100_bi,		// Whirlwind
		95_bi,		// Fly
		85_bi,		// Bind
		75_bi,		// Slam
		100_bi,		// Vine Whip
		100_bi,		// Stomp
		100_bi,		// Double Kick
		75_bi,		// Mega Kick
		95_bi,		// Jump Kick
		85_bi,		// Rolling Kick
		100_bi,		// Sand-Attack
		100_bi,		// Headbutt
		100_bi,		// Horn Attack
		85_bi,		// Fury Attack
		30_bi,		// Horn Drill
		100_bi,		// Tackle
		100_bi,		// Body Slam
		90_bi,		// Wrap
		85_bi,		// Take Down
		100_bi,		// Thrash
		100_bi,		// Double-Edge
		100_bi,		// Tail Whip
		100_bi,		// Poison Sting
		100_bi,		// Twineedle
		85_bi,		// Pin Missile
		100_bi,		// Leer
		100_bi,		// Bite
		100_bi,		// Growl
		100_bi,		// Roar
		55_bi,		// Sing
		55_bi,		// Supersonic
		90_bi,		// SonicBoom
		100_bi,		// Disable
		100_bi,		// Acid
		100_bi,		// Ember
		100_bi,		// Flamethrower
		none,		// Mist
		100_bi,		// Water Gun
		80_bi,		// Hydro Pump
		100_bi,		// Surf
		100_bi,		// Ice Beam
		70_bi,		// Blizzard
		100_bi,		// Psybeam
		100_bi,		// BubbleBeam
		100_bi,		// Aurora Beam
		90_bi,		// Hyper Beam
		100_bi,		// Peck
		100_bi,		// Drill Peck
		80_bi,		// Submission
		100_bi,		// Low Kick
		100_bi,		// Counter
		100_bi,		// Seismic Toss
		100_bi,		// Strength
		100_bi,		// Absorb
		100_bi,		// Mega Drain
		90_bi,		// Leech Seed
		none,		// Growth
		95_bi,		// Razor Leaf
		100_bi,		// SolarBeam
		75_bi,		// PoisonPowder
		75_bi,		// Stun Spore
		75_bi,		// Sleep Powder
		100_bi,		// Petal Dance
		95_bi,		// String Shot
		100_bi,		// Dragon Rage
		85_bi,		// Fire Spin
		100_bi,		// ThunderShock
		100_bi,		// Thunderbolt
		100_bi,		// Thunder Wave
		70_bi,		// Thunder
		90_bi,		// Rock Throw
		100_bi,		// Earthquake
		30_bi,		// Fissure
		100_bi,		// Dig
		90_bi,		// Toxic
		100_bi,		// Confusion
		100_bi,		// Psychic
		60_bi,		// Hypnosis
		none,		// Meditate
		none,		// Agility
		100_bi,		// Quick Attack
		100_bi,		// Rage
		none,		// Teleport
		100_bi,		// Night Shade
		none,		// Mimic
		85_bi,		// Screech
		none,		// Double Team
		none,		// Recover
		none,		// Harden
		none,		// Minimize
		100_bi,		// SmokeScreen
		100_bi,		// Confuse Ray
		none,		// Withdraw
		none,		// Defense Curl
		none,		// Barrier
		none,		// Light Screen
		none,		// Haze
		none,		// Reflect
		none,		// Focus Energy
		none,		// Bide
		none,		// Metronome
		none,		// Mirror Move
		100_bi,		// Selfdestruct
		75_bi,		// Egg Bomb
		100_bi,		// Lick
		70_bi,		// Smog
		100_bi,		// Sludge
		85_bi,		// Bone Club
		85_bi,		// Fire Blast
		100_bi,		// Waterfall
		85_bi,		// Clamp
		none,		// Swift
		100_bi,		// Skull Bash
		100_bi,		// Spike Cannon
		100_bi,		// Constrict
		none,		// Amnesia
		80_bi,		// Kinesis
		none,		// Softboiled
		90_bi,		// Hi Jump Kick
		90_bi,		// Glare
		100_bi,		// Dream Eater
		80_bi,		// Poison Gas
		85_bi,		// Barrage
		100_bi,		// Leech Life
		75_bi,		// Lovely Kiss
		90_bi,		// Sky Attack
		none,		// Transform
		100_bi,		// Bubble
		100_bi,		// Dizzy Punch
		100_bi,		// Spore
		100_bi,		// Flash
		80_bi,		// Psywave
		none,		// Splash
		none,		// Acid Armor
		90_bi,		// Crabhammer
		100_bi,		// Explosion
		80_bi,		// Fury Swipes
		90_bi,		// Bonemerang
		none,		// Rest
		90_bi,		// Rock Slide
		90_bi,		// Hyper Fang
		none,		// Sharpen
		none,		// Conversion
		100_bi,		// Tri Attack
		90_bi,		// Super Fang
		100_bi,		// Slash
		none,		// Substitute
		none,		// Struggle
		none,		// Sketch
		90_bi,		// Triple Kick
		100_bi,		// Thief
		none,		// Spider Web
		none,		// Mind Reader
		100_bi,		// Nightmare
		100_bi,		// Flame Wheel
		100_bi,		// Snore
		none,		// Curse
		100_bi,		// Flail
		none,		// Conversion 2
		95_bi,		// Aeroblast
		100_bi,		// Cotton Spore
		100_bi,		// Reversal
		100_bi,		// Spite
		100_bi,		// Powder Snow
		none,		// Protect
		100_bi,		// Mach Punch
		100_bi,		// Scary Face
		none,		// Faint Attack
		75_bi,		// Sweet Kiss
		none,		// Belly Drum
		100_bi,		// Sludge Bomb
		100_bi,		// Mud-Slap
		85_bi,		// Octazooka
		none,		// Spikes
		50_bi,		// Zap Cannon
		none,		// Foresight
		none,		// Destiny Bond
		none,		// Perish Song
		95_bi,		// Icy Wind
		none,		// Detect
		90_bi,		// Bone Rush
		none,		// Lock-On
		100_bi,		// Outrage
		none,		// Sandstorm
		100_bi,		// Giga Drain
		none,		// Endure
		100_bi,		// Charm
		90_bi,		// Rollout
		100_bi,		// False Swipe
		90_bi,		// Swagger
		none,		// Milk Drink
		100_bi,		// Spark
		95_bi,		// Fury Cutter
		90_bi,		// Steel Wing
		none,		// Mean Look
		100_bi,		// Attract
		none,		// Sleep Talk
		none,		// Heal Bell
		100_bi,		// Return
		90_bi,		// Present
		100_bi,		// Frustration
		none,		// Safeguard
		none,		// Pain Split
		95_bi,		// Sacred Fire
		100_bi,		// Magnitude
		50_bi,		// DynamicPunch
		85_bi,		// Megahorn
		100_bi,		// DragonBreath
		none,		// Baton Pass
		100_bi,		// Encore
		100_bi,		// Pursuit
		100_bi,		// Rapid Spin
		100_bi,		// Sweet Scent
		75_bi,		// Iron Tail
		95_bi,		// Metal Claw
		none,		// Vital Throw
		none,		// Morning Sun
		none,		// Synthesis
		none,		// Moonlight
		100_bi,		// Hidden Power
		80_bi,		// Cross Chop
		100_bi,		// Twister
		none,		// Rain Dance
		none,		// Sunny Day
		100_bi,		// Crunch
		100_bi,		// Mirror Coat
		none,		// Psych Up
		100_bi,		// ExtremeSpeed
		100_bi,		// AncientPower
		100_bi,		// Shadow Ball
		100_bi,		// Future Sight
		100_bi,		// Rock Smash
		85_bi,		// Whirlpool
		100_bi,		// Beat Up
		100_bi,		// Fake Out
		100_bi,		// Uproar
		none,		// Stockpile
		100_bi,		// Spit Up
		none,		// Swallow
		90_bi,		// Heat Wave
		none,		// Hail
		100_bi,		// Torment
		100_bi,		// Flatter
		75_bi,		// Will-O-Wisp
		100_bi,		// Memento
		100_bi,		// Facade
		100_bi,		// Focus Punch
		100_bi,		// SmellingSalt
		none,		// Follow Me
		none,		// Nature Power
		none,		// Charge
		100_bi,		// Taunt
		none,		// Helping Hand
		100_bi,		// Trick
		none,		// Role Play
		none,		// Wish
		none,		// Assist
		none,		// Ingrain
		100_bi,		// Superpower
		none,		// Magic Coat
		none,		// Recycle
		100_bi,		// Revenge
		100_bi,		// Brick Break
		none,		// Yawn
		100_bi,		// Knock Off
		100_bi,		// Endeavor
		100_bi,		// Eruption
		none,		// Skill Swap
		none,		// Imprison
		none,		// Refresh
		none,		// Grudge
		none,		// Snatch
		100_bi,		// Secret Power
		100_bi,		// Dive
		100_bi,		// Arm Thrust
		none,		// Camouflage
		none,		// Tail Glow
		100_bi,		// Luster Purge
		100_bi,		// Mist Ball
		100_bi,		// FeatherDance
		100_bi,		// Teeter Dance
		90_bi,		// Blaze Kick
		none,		// Mud Sport
		90_bi,		// Ice Ball
		100_bi,		// Needle Arm
		none,		// Slack Off
		100_bi,		// Hyper Voice
		100_bi,		// Poison Fang
		95_bi,		// Crush Claw
		90_bi,		// Blast Burn
		90_bi,		// Hydro Cannon
		85_bi,		// Meteor Mash
		100_bi,		// Astonish
		100_bi,		// Weather Ball
		none,		// Aromatherapy
		100_bi,		// Fake Tears
		95_bi,		// Air Cutter
		90_bi,		// Overheat
		none,		// Odor Sleuth
		80_bi,		// Rock Tomb
		100_bi,		// Silver Wind
		85_bi,		// Metal Sound
		55_bi,		// GrassWhistle
		100_bi,		// Tickle
		none,		// Cosmic Power
		100_bi,		// Water Spout
		100_bi,		// Signal Beam
		none,		// Shadow Punch
		100_bi,		// Extrasensory
		90_bi,		// Sky Uppercut
		85_bi,		// Sand Tomb
		30_bi,		// Sheer Cold
		85_bi,		// Muddy Water
		100_bi,		// Bullet Seed
		none,		// Aerial Ace
		100_bi,		// Icicle Spear
		none,		// Iron Defense
		none,		// Block
		none,		// Howl
		100_bi,		// Dragon Claw
		90_bi,		// Frenzy Plant
		none,		// Bulk Up
		85_bi,		// Bounce
		95_bi,		// Mud Shot
		100_bi,		// Poison Tail
		100_bi,		// Covet
		100_bi,		// Volt Tackle
		none,		// Magical Leaf
		none,		// Water Sport
		none,		// Calm Mind
		100_bi,		// Leaf Blade
		none,		// Dragon Dance
		90_bi,		// Rock Blast
		none,		// Shock Wave
		100_bi,		// Water Pulse
		100_bi,		// Doom Desire
		90_bi,		// Psycho Boost
		none,		// Roost
		none,		// Gravity
		none,		// Miracle Eye
		100_bi,		// Wake-Up Slap
		90_bi,		// Hammer Arm
		100_bi,		// Gyro Ball
		none,		// Healing Wish
		100_bi,		// Brine
		100_bi,		// Natural Gift
		100_bi,		// Feint
		100_bi,		// Pluck
		none,		// Tailwind
		none,		// Acupressure
		100_bi,		// Metal Burst
		100_bi,		// U-turn
		100_bi,		// Close Combat
		100_bi,		// Payback
		100_bi,		// Assurance
		100_bi,		// Embargo
		100_bi,		// Fling
		90_bi,		// Psycho Shift
		none,		// Trump Card
		100_bi,		// Heal Block
		100_bi,		// Wring Out
		none,		// Power Trick
		100_bi,		// Gastro Acid
		none,		// Lucky Chant
		none,		// Me First
		none,		// Copycat
		none,		// Power Swap
		none,		// Guard Swap
		100_bi,		// Punishment
		100_bi,		// Last Resort
		100_bi,		// Worry Seed
		100_bi,		// Sucker Punch
		none,		// Toxic Spikes
		none,		// Heart Swap
		none,		// Aqua Ring
		none,		// Magnet Rise
		100_bi,		// Flare Blitz
		100_bi,		// Force Palm
		none,		// Aura Sphere
		none,		// Rock Polish
		100_bi,		// Poison Jab
		100_bi,		// Dark Pulse
		100_bi,		// Night Slash
		90_bi,		// Aqua Tail
		100_bi,		// Seed Bomb
		95_bi,		// Air Slash
		100_bi,		// X-Scissor
		100_bi,		// Bug Buzz
		100_bi,		// Dragon Pulse
		75_bi,		// Dragon Rush
		100_bi,		// Power Gem
		100_bi,		// Drain Punch
		100_bi,		// Vacuum Wave
		70_bi,		// Focus Blast
		100_bi,		// Energy Ball
		100_bi,		// Brave Bird
		100_bi,		// Earth Power
		100_bi,		// Switcheroo
		90_bi,		// Giga Impact
		none,		// Nasty Plot
		100_bi,		// Bullet Punch
		100_bi,		// Avalanche
		100_bi,		// Ice Shard
		100_bi,		// Shadow Claw
		95_bi,		// Thunder Fang
		95_bi,		// Ice Fang
		95_bi,		// Fire Fang
		100_bi,		// Shadow Sneak
		85_bi,		// Mud Bomb
		100_bi,		// Psycho Cut
		90_bi,		// Zen Headbutt
		85_bi,		// Mirror Shot
		100_bi,		// Flash Cannon
		85_bi,		// Rock Climb
		none,		// Defog
		none,		// Trick Room
		90_bi,		// Draco Meteor
		100_bi,		// Discharge
		100_bi,		// Lava Plume
		90_bi,		// Leaf Storm
		85_bi,		// Power Whip
		90_bi,		// Rock Wrecker
		100_bi,		// Cross Poison
		70_bi,		// Gunk Shot
		100_bi,		// Iron Head
		none,		// Magnet Bomb
		80_bi,		// Stone Edge
		100_bi,		// Captivate
		none,		// Stealth Rock
		100_bi,		// Grass Knot
		100_bi,		// Chatter
		100_bi,		// Judgment
		100_bi,		// Bug Bite
		90_bi,		// Charge Beam
		100_bi,		// Wood Hammer
		100_bi,		// Aqua Jet
		100_bi,		// Attack Order
		none,		// Defend Order
		none,		// Heal Order
		80_bi,		// Head Smash
		90_bi,		// Double Hit
		90_bi,		// Roar of Time
		95_bi,		// Spacial Rend
		none,		// Lunar Dance
		100_bi,		// Crush Grip
		75_bi,		// Magma Storm
		80_bi,		// Dark Void
		85_bi,		// Seed Flare
		100_bi,		// Ominous Wind
		100_bi,		// Shadow Force
		none,		// Hone Claws
		none,		// Wide Guard
		none,		// Guard Split
		none,		// Power Split
		none,		// Wonder Room
		100_bi,		// Psyshock
		100_bi,		// Venoshock
		none,		// Autotomize
		none,		// Rage Powder
		none,		// Telekinesis
		none,		// Magic Room
		100_bi,		// Smack Down
		100_bi,		// Storm Throw
		100_bi,		// Flame Burst
		100_bi,		// Sludge Wave
		none,		// Quiver Dance
		100_bi,		// Heavy Slam
		100_bi,		// Synchronoise
		100_bi,		// Electro Ball
		100_bi,		// Soak
		100_bi,		// Flame Charge
		none,		// Coil
		100_bi,		// Low Sweep
		100_bi,		// Acid Spray
		100_bi,		// Foul Play
		100_bi,		// Simple Beam
		100_bi,		// Entrainment
		none,		// After You
		100_bi,		// Round
		100_bi,		// Echoed Voice
		100_bi,		// Chip Away
		none,		// Clear Smog
		100_bi,		// Stored Power
		none,		// Quick Guard
		none,		// Ally Switch
		100_bi,		// Scald
		none,		// Shell Smash
		none,		// Heal Pulse
		100_bi,		// Hex
		100_bi,		// Sky Drop
		none,		// Shift Gear
		90_bi,		// Circle Throw
		100_bi,		// Incinerate
		100_bi,		// Quash
		100_bi,		// Acrobatics
		none,		// Reflect Type
		100_bi,		// Retaliate
		100_bi,		// Final Gambit
		none,		// Bestow
		50_bi,		// Inferno
		100_bi,		// Water Pledge
		100_bi,		// Fire Pledge
		100_bi,		// Grass Pledge
		100_bi,		// Volt Switch
		100_bi,		// Struggle Bug
		100_bi,		// Bulldoze
		90_bi,		// Frost Breath
		90_bi,		// Dragon Tail
		none,		// Work Up
		95_bi,		// Electroweb
		100_bi,		// Wild Charge
		95_bi,		// Drill Run
		90_bi,		// Dual Chop
		100_bi,		// Heart Stamp
		100_bi,		// Horn Leech
		100_bi,		// Sacred Sword
		95_bi,		// Razor Shell
		100_bi,		// Heat Crash
		90_bi,		// Leaf Tornado
		100_bi,		// Steamroller
		none,		// Cotton Guard
		95_bi,		// Night Daze
		100_bi,		// Psystrike
		85_bi,		// Tail Slap
		70_bi,		// Hurricane
		100_bi,		// Head Charge
		85_bi,		// Gear Grind
		100_bi,		// Searing Shot
		100_bi,		// Techno Blast
		100_bi,		// Relic Song
		100_bi,		// Secret Sword
		95_bi,		// Glaciate
		85_bi,		// Bolt Strike
		85_bi,		// Blue Flare
		100_bi,		// Fiery Dance
		90_bi,		// Freeze Shock
		90_bi,		// Ice Burn
		95_bi,		// Snarl
		90_bi,		// Icicle Crash
		95_bi,		// V-create
		100_bi,		// Fusion Flare
		100_bi		// Fusion Bolt
	);
	static_assert(std::is_same<decltype(get_accuracy)::value_type, BaseAccuracy>::value, "Array type is not the same as the return type of function.");
	return get_accuracy.at(move);
}

}	// namespace technicalmachine
