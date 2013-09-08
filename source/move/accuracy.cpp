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
#include <cstddef>
#include <cstdint>

namespace technicalmachine {
namespace {

constexpr uint8_t perfect_accuracy = 0xFF;

}	// unnamed namespace

bool can_miss(Moves const move) {
	return accuracy(move) != perfect_accuracy;
}

// A number between 0 (1?) and 100, according to poccil.
uint8_t accuracy(Moves move) {
	static constexpr uint8_t get_accuracy [] = {
		perfect_accuracy,		// Switch0
		perfect_accuracy,		// Switch1
		perfect_accuracy,		// Switch2
		perfect_accuracy,		// Switch3
		perfect_accuracy,		// Switch4
		perfect_accuracy,		// Switch5
		perfect_accuracy,		// Hit Self
		100,		// Pound
		100,		// Karate Chop
		85,		// DoubleSlap
		85,		// Comet Punch
		85,		// Mega Punch
		100,		// Pay Day
		100,		// Fire Punch
		100,		// Ice Punch
		100,		// ThunderPunch
		100,		// Scratch
		100,		// ViceGrip
		30,		// Guillotine
		100,		// Razor Wind
		perfect_accuracy,		// Swords Dance
		95,		// Cut
		100,		// Gust
		100,		// Wing Attack
		100,		// Whirlwind
		95,		// Fly
		85,		// Bind
		75,		// Slam
		100,		// Vine Whip
		100,		// Stomp
		100,		// Double Kick
		75,		// Mega Kick
		95,		// Jump Kick
		85,		// Rolling Kick
		100,		// Sand-Attack
		100,		// Headbutt
		100,		// Horn Attack
		85,		// Fury Attack
		30,		// Horn Drill
		100,		// Tackle
		100,		// Body Slam
		90,		// Wrap
		85,		// Take Down
		100,		// Thrash
		100,		// Double-Edge
		100,		// Tail Whip
		100,		// Poison Sting
		100,		// Twineedle
		85,		// Pin Missile
		100,		// Leer
		100,		// Bite
		100,		// Growl
		100,		// Roar
		55,		// Sing
		55,		// Supersonic
		90,		// SonicBoom
		100,		// Disable
		100,		// Acid
		100,		// Ember
		100,		// Flamethrower
		perfect_accuracy,		// Mist
		100,		// Water Gun
		80,		// Hydro Pump
		100,		// Surf
		100,		// Ice Beam
		70,		// Blizzard
		100,		// Psybeam
		100,		// BubbleBeam
		100,		// Aurora Beam
		90,		// Hyper Beam
		100,		// Peck
		100,		// Drill Peck
		80,		// Submission
		100,		// Low Kick
		100,		// Counter
		100,		// Seismic Toss
		100,		// Strength
		100,		// Absorb
		100,		// Mega Drain
		90,		// Leech Seed
		perfect_accuracy,		// Growth
		95,		// Razor Leaf
		100,		// SolarBeam
		75,		// PoisonPowder
		75,		// Stun Spore
		75,		// Sleep Powder
		100,		// Petal Dance
		95,		// String Shot
		100,		// Dragon Rage
		85,		// Fire Spin
		100,		// ThunderShock
		100,		// Thunderbolt
		100,		// Thunder Wave
		70,		// Thunder
		90,		// Rock Throw
		100,		// Earthquake
		30,		// Fissure
		100,		// Dig
		90,		// Toxic
		100,		// Confusion
		100,		// Psychic
		60,		// Hypnosis
		perfect_accuracy,		// Meditate
		perfect_accuracy,		// Agility
		100,		// Quick Attack
		100,		// Rage
		perfect_accuracy,		// Teleport
		100,		// Night Shade
		perfect_accuracy,		// Mimic
		85,		// Screech
		perfect_accuracy,		// Double Team
		perfect_accuracy,		// Recover
		perfect_accuracy,		// Harden
		perfect_accuracy,		// Minimize
		100,		// SmokeScreen
		100,		// Confuse Ray
		perfect_accuracy,		// Withdraw
		perfect_accuracy,		// Defense Curl
		perfect_accuracy,		// Barrier
		perfect_accuracy,		// Light Screen
		perfect_accuracy,		// Haze
		perfect_accuracy,		// Reflect
		perfect_accuracy,		// Focus Energy
		perfect_accuracy,		// Bide
		perfect_accuracy,		// Metronome
		perfect_accuracy,		// Mirror Move
		100,		// Selfdestruct
		75,		// Egg Bomb
		100,		// Lick
		70,		// Smog
		100,		// Sludge
		85,		// Bone Club
		85,		// Fire Blast
		100,		// Waterfall
		85,		// Clamp
		perfect_accuracy,		// Swift
		100,		// Skull Bash
		100,		// Spike Cannon
		100,		// Constrict
		perfect_accuracy,		// Amnesia
		80,		// Kinesis
		perfect_accuracy,		// Softboiled
		90,		// Hi Jump Kick
		90,		// Glare
		100,		// Dream Eater
		80,		// Poison Gas
		85,		// Barrage
		100,		// Leech Life
		75,		// Lovely Kiss
		90,		// Sky Attack
		perfect_accuracy,		// Transform
		100,		// Bubble
		100,		// Dizzy Punch
		100,		// Spore
		100,		// Flash
		80,		// Psywave
		perfect_accuracy,		// Splash
		perfect_accuracy,		// Acid Armor
		90,		// Crabhammer
		100,		// Explosion
		80,		// Fury Swipes
		90,		// Bonemerang
		perfect_accuracy,		// Rest
		90,		// Rock Slide
		90,		// Hyper Fang
		perfect_accuracy,		// Sharpen
		perfect_accuracy,		// Conversion
		100,		// Tri Attack
		90,		// Super Fang
		100,		// Slash
		perfect_accuracy,		// Substitute
		perfect_accuracy,		// Struggle
		perfect_accuracy,		// Sketch
		90,		// Triple Kick
		100,		// Thief
		perfect_accuracy,		// Spider Web
		perfect_accuracy,		// Mind Reader
		100,		// Nightmare
		100,		// Flame Wheel
		100,		// Snore
		perfect_accuracy,		// Curse
		100,		// Flail
		perfect_accuracy,		// Conversion 2
		95,		// Aeroblast
		100,		// Cotton Spore
		100,		// Reversal
		100,		// Spite
		100,		// Powder Snow
		perfect_accuracy,		// Protect
		100,		// Mach Punch
		100,		// Scary Face
		perfect_accuracy,		// Faint Attack
		75,		// Sweet Kiss
		perfect_accuracy,		// Belly Drum
		100,		// Sludge Bomb
		100,		// Mud-Slap
		85,		// Octazooka
		perfect_accuracy,		// Spikes
		50,		// Zap Cannon
		perfect_accuracy,		// Foresight
		perfect_accuracy,		// Destiny Bond
		perfect_accuracy,		// Perish Song
		95,		// Icy Wind
		perfect_accuracy,		// Detect
		90,		// Bone Rush
		perfect_accuracy,		// Lock-On
		100,		// Outrage
		perfect_accuracy,		// Sandstorm
		100,		// Giga Drain
		perfect_accuracy,		// Endure
		100,		// Charm
		90,		// Rollout
		100,		// False Swipe
		90,		// Swagger
		perfect_accuracy,		// Milk Drink
		100,		// Spark
		95,		// Fury Cutter
		90,		// Steel Wing
		perfect_accuracy,		// Mean Look
		100,		// Attract
		perfect_accuracy,		// Sleep Talk
		perfect_accuracy,		// Heal Bell
		100,		// Return
		90,		// Present
		100,		// Frustration
		perfect_accuracy,		// Safeguard
		perfect_accuracy,		// Pain Split
		95,		// Sacred Fire
		100,		// Magnitude
		50,		// DynamicPunch
		85,		// Megahorn
		100,		// DragonBreath
		perfect_accuracy,		// Baton Pass
		100,		// Encore
		100,		// Pursuit
		100,		// Rapid Spin
		100,		// Sweet Scent
		75,		// Iron Tail
		95,		// Metal Claw
		perfect_accuracy,		// Vital Throw
		perfect_accuracy,		// Morning Sun
		perfect_accuracy,		// Synthesis
		perfect_accuracy,		// Moonlight
		100,		// Hidden Power
		80,		// Cross Chop
		100,		// Twister
		perfect_accuracy,		// Rain Dance
		perfect_accuracy,		// Sunny Day
		100,		// Crunch
		100,		// Mirror Coat
		perfect_accuracy,		// Psych Up
		100,		// ExtremeSpeed
		100,		// AncientPower
		100,		// Shadow Ball
		100,		// Future Sight
		100,		// Rock Smash
		85,		// Whirlpool
		100,		// Beat Up
		100,		// Fake Out
		100,		// Uproar
		perfect_accuracy,		// Stockpile
		100,		// Spit Up
		perfect_accuracy,		// Swallow
		90,		// Heat Wave
		perfect_accuracy,		// Hail
		100,		// Torment
		100,		// Flatter
		75,		// Will-O-Wisp
		100,		// Memento
		100,		// Facade
		100,		// Focus Punch
		100,		// SmellingSalt
		perfect_accuracy,		// Follow Me
		perfect_accuracy,		// Nature Power
		perfect_accuracy,		// Charge
		100,		// Taunt
		perfect_accuracy,		// Helping Hand
		100,		// Trick
		perfect_accuracy,		// Role Play
		perfect_accuracy,		// Wish
		perfect_accuracy,		// Assist
		perfect_accuracy,		// Ingrain
		100,		// Superpower
		perfect_accuracy,		// Magic Coat
		perfect_accuracy,		// Recycle
		100,		// Revenge
		100,		// Brick Break
		perfect_accuracy,		// Yawn
		100,		// Knock Off
		100,		// Endeavor
		100,		// Eruption
		perfect_accuracy,		// Skill Swap
		perfect_accuracy,		// Imprison
		perfect_accuracy,		// Refresh
		perfect_accuracy,		// Grudge
		perfect_accuracy,		// Snatch
		100,		// Secret Power
		100,		// Dive
		100,		// Arm Thrust
		perfect_accuracy,		// Camouflage
		perfect_accuracy,		// Tail Glow
		100,		// Luster Purge
		100,		// Mist Ball
		100,		// FeatherDance
		100,		// Teeter Dance
		90,		// Blaze Kick
		perfect_accuracy,		// Mud Sport
		90,		// Ice Ball
		100,		// Needle Arm
		perfect_accuracy,		// Slack Off
		100,		// Hyper Voice
		100,		// Poison Fang
		95,		// Crush Claw
		90,		// Blast Burn
		90,		// Hydro Cannon
		85,		// Meteor Mash
		100,		// Astonish
		100,		// Weather Ball
		perfect_accuracy,		// Aromatherapy
		100,		// Fake Tears
		95,		// Air Cutter
		90,		// Overheat
		perfect_accuracy,		// Odor Sleuth
		80,		// Rock Tomb
		100,		// Silver Wind
		85,		// Metal Sound
		55,		// GrassWhistle
		100,		// Tickle
		perfect_accuracy,		// Cosmic Power
		100,		// Water Spout
		100,		// Signal Beam
		perfect_accuracy,		// Shadow Punch
		100,		// Extrasensory
		90,		// Sky Uppercut
		85,		// Sand Tomb
		30,		// Sheer Cold
		85,		// Muddy Water
		100,		// Bullet Seed
		perfect_accuracy,		// Aerial Ace
		100,		// Icicle Spear
		perfect_accuracy,		// Iron Defense
		perfect_accuracy,		// Block
		perfect_accuracy,		// Howl
		100,		// Dragon Claw
		90,		// Frenzy Plant
		perfect_accuracy,		// Bulk Up
		85,		// Bounce
		95,		// Mud Shot
		100,		// Poison Tail
		100,		// Covet
		100,		// Volt Tackle
		perfect_accuracy,		// Magical Leaf
		perfect_accuracy,		// Water Sport
		perfect_accuracy,		// Calm Mind
		100,		// Leaf Blade
		perfect_accuracy,		// Dragon Dance
		90,		// Rock Blast
		perfect_accuracy,		// Shock Wave
		100,		// Water Pulse
		100,		// Doom Desire
		90,		// Psycho Boost
		perfect_accuracy,		// Roost
		perfect_accuracy,		// Gravity
		perfect_accuracy,		// Miracle Eye
		100,		// Wake-Up Slap
		90,		// Hammer Arm
		100,		// Gyro Ball
		perfect_accuracy,		// Healing Wish
		100,		// Brine
		100,		// Natural Gift
		100,		// Feint
		100,		// Pluck
		perfect_accuracy,		// Tailwind
		perfect_accuracy,		// Acupressure
		100,		// Metal Burst
		100,		// U-turn
		100,		// Close Combat
		100,		// Payback
		100,		// Assurance
		100,		// Embargo
		100,		// Fling
		90,		// Psycho Shift
		perfect_accuracy,		// Trump Card
		100,		// Heal Block
		100,		// Wring Out
		perfect_accuracy,		// Power Trick
		100,		// Gastro Acid
		perfect_accuracy,		// Lucky Chant
		perfect_accuracy,		// Me First
		perfect_accuracy,		// Copycat
		perfect_accuracy,		// Power Swap
		perfect_accuracy,		// Guard Swap
		100,		// Punishment
		100,		// Last Resort
		100,		// Worry Seed
		100,		// Sucker Punch
		perfect_accuracy,		// Toxic Spikes
		perfect_accuracy,		// Heart Swap
		perfect_accuracy,		// Aqua Ring
		perfect_accuracy,		// Magnet Rise
		100,		// Flare Blitz
		100,		// Force Palm
		perfect_accuracy,		// Aura Sphere
		perfect_accuracy,		// Rock Polish
		100,		// Poison Jab
		100,		// Dark Pulse
		100,		// Night Slash
		90,		// Aqua Tail
		100,		// Seed Bomb
		95,		// Air Slash
		100,		// X-Scissor
		100,		// Bug Buzz
		100,		// Dragon Pulse
		75,		// Dragon Rush
		100,		// Power Gem
		100,		// Drain Punch
		100,		// Vacuum Wave
		70,		// Focus Blast
		100,		// Energy Ball
		100,		// Brave Bird
		100,		// Earth Power
		100,		// Switcheroo
		90,		// Giga Impact
		perfect_accuracy,		// Nasty Plot
		100,		// Bullet Punch
		100,		// Avalanche
		100,		// Ice Shard
		100,		// Shadow Claw
		95,		// Thunder Fang
		95,		// Ice Fang
		95,		// Fire Fang
		100,		// Shadow Sneak
		85,		// Mud Bomb
		100,		// Psycho Cut
		90,		// Zen Headbutt
		85,		// Mirror Shot
		100,		// Flash Cannon
		85,		// Rock Climb
		perfect_accuracy,		// Defog
		perfect_accuracy,		// Trick Room
		90,		// Draco Meteor
		100,		// Discharge
		100,		// Lava Plume
		90,		// Leaf Storm
		85,		// Power Whip
		90,		// Rock Wrecker
		100,		// Cross Poison
		70,		// Gunk Shot
		100,		// Iron Head
		perfect_accuracy,		// Magnet Bomb
		80,		// Stone Edge
		100,		// Captivate
		perfect_accuracy,		// Stealth Rock
		100,		// Grass Knot
		100,		// Chatter
		100,		// Judgment
		100,		// Bug Bite
		90,		// Charge Beam
		100,		// Wood Hammer
		100,		// Aqua Jet
		100,		// Attack Order
		perfect_accuracy,		// Defend Order
		perfect_accuracy,		// Heal Order
		80,		// Head Smash
		90,		// Double Hit
		90,		// Roar of Time
		95,		// Spacial Rend
		perfect_accuracy,		// Lunar Dance
		100,		// Crush Grip
		75,		// Magma Storm
		80,		// Dark Void
		85,		// Seed Flare
		100,		// Ominous Wind
		100,		// Shadow Force
		perfect_accuracy,		// Hone Claws
		perfect_accuracy,		// Wide Guard
		perfect_accuracy,		// Guard Split
		perfect_accuracy,		// Power Split
		perfect_accuracy,		// Wonder Room
		100,		// Psyshock
		100,		// Venoshock
		perfect_accuracy,		// Autotomize
		perfect_accuracy,		// Rage Powder
		perfect_accuracy,		// Telekinesis
		perfect_accuracy,		// Magic Room
		100,		// Smack Down
		100,		// Storm Throw
		100,		// Flame Burst
		100,		// Sludge Wave
		perfect_accuracy,		// Quiver Dance
		100,		// Heavy Slam
		100,		// Synchronoise
		100,		// Electro Ball
		100,		// Soak
		100,		// Flame Charge
		perfect_accuracy,		// Coil
		100,		// Low Sweep
		100,		// Acid Spray
		100,		// Foul Play
		100,		// Simple Beam
		100,		// Entrainment
		perfect_accuracy,		// After You
		100,		// Round
		100,		// Echoed Voice
		100,		// Chip Away
		perfect_accuracy,		// Clear Smog
		100,		// Stored Power
		perfect_accuracy,		// Quick Guard
		perfect_accuracy,		// Ally Switch
		100,		// Scald
		perfect_accuracy,		// Shell Smash
		perfect_accuracy,		// Heal Pulse
		100,		// Hex
		100,		// Sky Drop
		perfect_accuracy,		// Shift Gear
		90,		// Circle Throw
		100,		// Incinerate
		100,		// Quash
		100,		// Acrobatics
		perfect_accuracy,		// Reflect Type
		100,		// Retaliate
		100,		// Final Gambit
		perfect_accuracy,		// Bestow
		50,		// Inferno
		100,		// Water Pledge
		100,		// Fire Pledge
		100,		// Grass Pledge
		100,		// Volt Switch
		100,		// Struggle Bug
		100,		// Bulldoze
		90,		// Frost Breath
		90,		// Dragon Tail
		perfect_accuracy,		// Work Up
		95,		// Electroweb
		100,		// Wild Charge
		95,		// Drill Run
		90,		// Dual Chop
		100,		// Heart Stamp
		100,		// Horn Leech
		100,		// Sacred Sword
		95,		// Razor Shell
		100,		// Heat Crash
		90,		// Leaf Tornado
		100,		// Steamroller
		perfect_accuracy,		// Cotton Guard
		95,		// Night Daze
		100,		// Psystrike
		85,		// Tail Slap
		70,		// Hurricane
		100,		// Head Charge
		85,		// Gear Grind
		100,		// Searing Shot
		100,		// Techno Blast
		100,		// Relic Song
		100,		// Secret Sword
		95,		// Glaciate
		85,		// Bolt Strike
		85,		// Blue Flare
		100,		// Fiery Dance
		90,		// Freeze Shock
		90,		// Ice Burn
		95,		// Snarl
		90,		// Icicle Crash
		95,		// V-create
		100,		// Fusion Flare
		100		// Fusion Bolt
	};
	return get_accuracy [static_cast<size_t>(move)];
}

}	// namespace technicalmachine
