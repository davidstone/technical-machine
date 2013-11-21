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
#include <type_traits>
#include <utility>

namespace technicalmachine {

optional<native_integer<30, 100>> accuracy(Moves const move) {
	static constexpr auto get_accuracy = make_ranged_optional_array(
		none,		// Switch0
		none,		// Switch1
		none,		// Switch2
		none,		// Switch3
		none,		// Switch4
		none,		// Switch5
		none,		// Hit Self
		100_ri,		// Pound
		100_ri,		// Karate Chop
		85_ri,		// DoubleSlap
		85_ri,		// Comet Punch
		85_ri,		// Mega Punch
		100_ri,		// Pay Day
		100_ri,		// Fire Punch
		100_ri,		// Ice Punch
		100_ri,		// ThunderPunch
		100_ri,		// Scratch
		100_ri,		// ViceGrip
		30_ri,		// Guillotine
		100_ri,		// Razor Wind
		none,		// Swords Dance
		95_ri,		// Cut
		100_ri,		// Gust
		100_ri,		// Wing Attack
		100_ri,		// Whirlwind
		95_ri,		// Fly
		85_ri,		// Bind
		75_ri,		// Slam
		100_ri,		// Vine Whip
		100_ri,		// Stomp
		100_ri,		// Double Kick
		75_ri,		// Mega Kick
		95_ri,		// Jump Kick
		85_ri,		// Rolling Kick
		100_ri,		// Sand-Attack
		100_ri,		// Headbutt
		100_ri,		// Horn Attack
		85_ri,		// Fury Attack
		30_ri,		// Horn Drill
		100_ri,		// Tackle
		100_ri,		// Body Slam
		90_ri,		// Wrap
		85_ri,		// Take Down
		100_ri,		// Thrash
		100_ri,		// Double-Edge
		100_ri,		// Tail Whip
		100_ri,		// Poison Sting
		100_ri,		// Twineedle
		85_ri,		// Pin Missile
		100_ri,		// Leer
		100_ri,		// Bite
		100_ri,		// Growl
		100_ri,		// Roar
		55_ri,		// Sing
		55_ri,		// Supersonic
		90_ri,		// SonicBoom
		100_ri,		// Disable
		100_ri,		// Acid
		100_ri,		// Ember
		100_ri,		// Flamethrower
		none,		// Mist
		100_ri,		// Water Gun
		80_ri,		// Hydro Pump
		100_ri,		// Surf
		100_ri,		// Ice Beam
		70_ri,		// Blizzard
		100_ri,		// Psybeam
		100_ri,		// BubbleBeam
		100_ri,		// Aurora Beam
		90_ri,		// Hyper Beam
		100_ri,		// Peck
		100_ri,		// Drill Peck
		80_ri,		// Submission
		100_ri,		// Low Kick
		100_ri,		// Counter
		100_ri,		// Seismic Toss
		100_ri,		// Strength
		100_ri,		// Absorb
		100_ri,		// Mega Drain
		90_ri,		// Leech Seed
		none,		// Growth
		95_ri,		// Razor Leaf
		100_ri,		// SolarBeam
		75_ri,		// PoisonPowder
		75_ri,		// Stun Spore
		75_ri,		// Sleep Powder
		100_ri,		// Petal Dance
		95_ri,		// String Shot
		100_ri,		// Dragon Rage
		85_ri,		// Fire Spin
		100_ri,		// ThunderShock
		100_ri,		// Thunderbolt
		100_ri,		// Thunder Wave
		70_ri,		// Thunder
		90_ri,		// Rock Throw
		100_ri,		// Earthquake
		30_ri,		// Fissure
		100_ri,		// Dig
		90_ri,		// Toxic
		100_ri,		// Confusion
		100_ri,		// Psychic
		60_ri,		// Hypnosis
		none,		// Meditate
		none,		// Agility
		100_ri,		// Quick Attack
		100_ri,		// Rage
		none,		// Teleport
		100_ri,		// Night Shade
		none,		// Mimic
		85_ri,		// Screech
		none,		// Double Team
		none,		// Recover
		none,		// Harden
		none,		// Minimize
		100_ri,		// SmokeScreen
		100_ri,		// Confuse Ray
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
		100_ri,		// Selfdestruct
		75_ri,		// Egg Bomb
		100_ri,		// Lick
		70_ri,		// Smog
		100_ri,		// Sludge
		85_ri,		// Bone Club
		85_ri,		// Fire Blast
		100_ri,		// Waterfall
		85_ri,		// Clamp
		none,		// Swift
		100_ri,		// Skull Bash
		100_ri,		// Spike Cannon
		100_ri,		// Constrict
		none,		// Amnesia
		80_ri,		// Kinesis
		none,		// Softboiled
		90_ri,		// Hi Jump Kick
		90_ri,		// Glare
		100_ri,		// Dream Eater
		80_ri,		// Poison Gas
		85_ri,		// Barrage
		100_ri,		// Leech Life
		75_ri,		// Lovely Kiss
		90_ri,		// Sky Attack
		none,		// Transform
		100_ri,		// Bubble
		100_ri,		// Dizzy Punch
		100_ri,		// Spore
		100_ri,		// Flash
		80_ri,		// Psywave
		none,		// Splash
		none,		// Acid Armor
		90_ri,		// Crabhammer
		100_ri,		// Explosion
		80_ri,		// Fury Swipes
		90_ri,		// Bonemerang
		none,		// Rest
		90_ri,		// Rock Slide
		90_ri,		// Hyper Fang
		none,		// Sharpen
		none,		// Conversion
		100_ri,		// Tri Attack
		90_ri,		// Super Fang
		100_ri,		// Slash
		none,		// Substitute
		none,		// Struggle
		none,		// Sketch
		90_ri,		// Triple Kick
		100_ri,		// Thief
		none,		// Spider Web
		none,		// Mind Reader
		100_ri,		// Nightmare
		100_ri,		// Flame Wheel
		100_ri,		// Snore
		none,		// Curse
		100_ri,		// Flail
		none,		// Conversion 2
		95_ri,		// Aeroblast
		100_ri,		// Cotton Spore
		100_ri,		// Reversal
		100_ri,		// Spite
		100_ri,		// Powder Snow
		none,		// Protect
		100_ri,		// Mach Punch
		100_ri,		// Scary Face
		none,		// Faint Attack
		75_ri,		// Sweet Kiss
		none,		// Belly Drum
		100_ri,		// Sludge Bomb
		100_ri,		// Mud-Slap
		85_ri,		// Octazooka
		none,		// Spikes
		50_ri,		// Zap Cannon
		none,		// Foresight
		none,		// Destiny Bond
		none,		// Perish Song
		95_ri,		// Icy Wind
		none,		// Detect
		90_ri,		// Bone Rush
		none,		// Lock-On
		100_ri,		// Outrage
		none,		// Sandstorm
		100_ri,		// Giga Drain
		none,		// Endure
		100_ri,		// Charm
		90_ri,		// Rollout
		100_ri,		// False Swipe
		90_ri,		// Swagger
		none,		// Milk Drink
		100_ri,		// Spark
		95_ri,		// Fury Cutter
		90_ri,		// Steel Wing
		none,		// Mean Look
		100_ri,		// Attract
		none,		// Sleep Talk
		none,		// Heal Bell
		100_ri,		// Return
		90_ri,		// Present
		100_ri,		// Frustration
		none,		// Safeguard
		none,		// Pain Split
		95_ri,		// Sacred Fire
		100_ri,		// Magnitude
		50_ri,		// DynamicPunch
		85_ri,		// Megahorn
		100_ri,		// DragonBreath
		none,		// Baton Pass
		100_ri,		// Encore
		100_ri,		// Pursuit
		100_ri,		// Rapid Spin
		100_ri,		// Sweet Scent
		75_ri,		// Iron Tail
		95_ri,		// Metal Claw
		none,		// Vital Throw
		none,		// Morning Sun
		none,		// Synthesis
		none,		// Moonlight
		100_ri,		// Hidden Power
		80_ri,		// Cross Chop
		100_ri,		// Twister
		none,		// Rain Dance
		none,		// Sunny Day
		100_ri,		// Crunch
		100_ri,		// Mirror Coat
		none,		// Psych Up
		100_ri,		// ExtremeSpeed
		100_ri,		// AncientPower
		100_ri,		// Shadow Ball
		100_ri,		// Future Sight
		100_ri,		// Rock Smash
		85_ri,		// Whirlpool
		100_ri,		// Beat Up
		100_ri,		// Fake Out
		100_ri,		// Uproar
		none,		// Stockpile
		100_ri,		// Spit Up
		none,		// Swallow
		90_ri,		// Heat Wave
		none,		// Hail
		100_ri,		// Torment
		100_ri,		// Flatter
		75_ri,		// Will-O-Wisp
		100_ri,		// Memento
		100_ri,		// Facade
		100_ri,		// Focus Punch
		100_ri,		// SmellingSalt
		none,		// Follow Me
		none,		// Nature Power
		none,		// Charge
		100_ri,		// Taunt
		none,		// Helping Hand
		100_ri,		// Trick
		none,		// Role Play
		none,		// Wish
		none,		// Assist
		none,		// Ingrain
		100_ri,		// Superpower
		none,		// Magic Coat
		none,		// Recycle
		100_ri,		// Revenge
		100_ri,		// Brick Break
		none,		// Yawn
		100_ri,		// Knock Off
		100_ri,		// Endeavor
		100_ri,		// Eruption
		none,		// Skill Swap
		none,		// Imprison
		none,		// Refresh
		none,		// Grudge
		none,		// Snatch
		100_ri,		// Secret Power
		100_ri,		// Dive
		100_ri,		// Arm Thrust
		none,		// Camouflage
		none,		// Tail Glow
		100_ri,		// Luster Purge
		100_ri,		// Mist Ball
		100_ri,		// FeatherDance
		100_ri,		// Teeter Dance
		90_ri,		// Blaze Kick
		none,		// Mud Sport
		90_ri,		// Ice Ball
		100_ri,		// Needle Arm
		none,		// Slack Off
		100_ri,		// Hyper Voice
		100_ri,		// Poison Fang
		95_ri,		// Crush Claw
		90_ri,		// Blast Burn
		90_ri,		// Hydro Cannon
		85_ri,		// Meteor Mash
		100_ri,		// Astonish
		100_ri,		// Weather Ball
		none,		// Aromatherapy
		100_ri,		// Fake Tears
		95_ri,		// Air Cutter
		90_ri,		// Overheat
		none,		// Odor Sleuth
		80_ri,		// Rock Tomb
		100_ri,		// Silver Wind
		85_ri,		// Metal Sound
		55_ri,		// GrassWhistle
		100_ri,		// Tickle
		none,		// Cosmic Power
		100_ri,		// Water Spout
		100_ri,		// Signal Beam
		none,		// Shadow Punch
		100_ri,		// Extrasensory
		90_ri,		// Sky Uppercut
		85_ri,		// Sand Tomb
		30_ri,		// Sheer Cold
		85_ri,		// Muddy Water
		100_ri,		// Bullet Seed
		none,		// Aerial Ace
		100_ri,		// Icicle Spear
		none,		// Iron Defense
		none,		// Block
		none,		// Howl
		100_ri,		// Dragon Claw
		90_ri,		// Frenzy Plant
		none,		// Bulk Up
		85_ri,		// Bounce
		95_ri,		// Mud Shot
		100_ri,		// Poison Tail
		100_ri,		// Covet
		100_ri,		// Volt Tackle
		none,		// Magical Leaf
		none,		// Water Sport
		none,		// Calm Mind
		100_ri,		// Leaf Blade
		none,		// Dragon Dance
		90_ri,		// Rock Blast
		none,		// Shock Wave
		100_ri,		// Water Pulse
		100_ri,		// Doom Desire
		90_ri,		// Psycho Boost
		none,		// Roost
		none,		// Gravity
		none,		// Miracle Eye
		100_ri,		// Wake-Up Slap
		90_ri,		// Hammer Arm
		100_ri,		// Gyro Ball
		none,		// Healing Wish
		100_ri,		// Brine
		100_ri,		// Natural Gift
		100_ri,		// Feint
		100_ri,		// Pluck
		none,		// Tailwind
		none,		// Acupressure
		100_ri,		// Metal Burst
		100_ri,		// U-turn
		100_ri,		// Close Combat
		100_ri,		// Payback
		100_ri,		// Assurance
		100_ri,		// Embargo
		100_ri,		// Fling
		90_ri,		// Psycho Shift
		none,		// Trump Card
		100_ri,		// Heal Block
		100_ri,		// Wring Out
		none,		// Power Trick
		100_ri,		// Gastro Acid
		none,		// Lucky Chant
		none,		// Me First
		none,		// Copycat
		none,		// Power Swap
		none,		// Guard Swap
		100_ri,		// Punishment
		100_ri,		// Last Resort
		100_ri,		// Worry Seed
		100_ri,		// Sucker Punch
		none,		// Toxic Spikes
		none,		// Heart Swap
		none,		// Aqua Ring
		none,		// Magnet Rise
		100_ri,		// Flare Blitz
		100_ri,		// Force Palm
		none,		// Aura Sphere
		none,		// Rock Polish
		100_ri,		// Poison Jab
		100_ri,		// Dark Pulse
		100_ri,		// Night Slash
		90_ri,		// Aqua Tail
		100_ri,		// Seed Bomb
		95_ri,		// Air Slash
		100_ri,		// X-Scissor
		100_ri,		// Bug Buzz
		100_ri,		// Dragon Pulse
		75_ri,		// Dragon Rush
		100_ri,		// Power Gem
		100_ri,		// Drain Punch
		100_ri,		// Vacuum Wave
		70_ri,		// Focus Blast
		100_ri,		// Energy Ball
		100_ri,		// Brave Bird
		100_ri,		// Earth Power
		100_ri,		// Switcheroo
		90_ri,		// Giga Impact
		none,		// Nasty Plot
		100_ri,		// Bullet Punch
		100_ri,		// Avalanche
		100_ri,		// Ice Shard
		100_ri,		// Shadow Claw
		95_ri,		// Thunder Fang
		95_ri,		// Ice Fang
		95_ri,		// Fire Fang
		100_ri,		// Shadow Sneak
		85_ri,		// Mud Bomb
		100_ri,		// Psycho Cut
		90_ri,		// Zen Headbutt
		85_ri,		// Mirror Shot
		100_ri,		// Flash Cannon
		85_ri,		// Rock Climb
		none,		// Defog
		none,		// Trick Room
		90_ri,		// Draco Meteor
		100_ri,		// Discharge
		100_ri,		// Lava Plume
		90_ri,		// Leaf Storm
		85_ri,		// Power Whip
		90_ri,		// Rock Wrecker
		100_ri,		// Cross Poison
		70_ri,		// Gunk Shot
		100_ri,		// Iron Head
		none,		// Magnet Bomb
		80_ri,		// Stone Edge
		100_ri,		// Captivate
		none,		// Stealth Rock
		100_ri,		// Grass Knot
		100_ri,		// Chatter
		100_ri,		// Judgment
		100_ri,		// Bug Bite
		90_ri,		// Charge Beam
		100_ri,		// Wood Hammer
		100_ri,		// Aqua Jet
		100_ri,		// Attack Order
		none,		// Defend Order
		none,		// Heal Order
		80_ri,		// Head Smash
		90_ri,		// Double Hit
		90_ri,		// Roar of Time
		95_ri,		// Spacial Rend
		none,		// Lunar Dance
		100_ri,		// Crush Grip
		75_ri,		// Magma Storm
		80_ri,		// Dark Void
		85_ri,		// Seed Flare
		100_ri,		// Ominous Wind
		100_ri,		// Shadow Force
		none,		// Hone Claws
		none,		// Wide Guard
		none,		// Guard Split
		none,		// Power Split
		none,		// Wonder Room
		100_ri,		// Psyshock
		100_ri,		// Venoshock
		none,		// Autotomize
		none,		// Rage Powder
		none,		// Telekinesis
		none,		// Magic Room
		100_ri,		// Smack Down
		100_ri,		// Storm Throw
		100_ri,		// Flame Burst
		100_ri,		// Sludge Wave
		none,		// Quiver Dance
		100_ri,		// Heavy Slam
		100_ri,		// Synchronoise
		100_ri,		// Electro Ball
		100_ri,		// Soak
		100_ri,		// Flame Charge
		none,		// Coil
		100_ri,		// Low Sweep
		100_ri,		// Acid Spray
		100_ri,		// Foul Play
		100_ri,		// Simple Beam
		100_ri,		// Entrainment
		none,		// After You
		100_ri,		// Round
		100_ri,		// Echoed Voice
		100_ri,		// Chip Away
		none,		// Clear Smog
		100_ri,		// Stored Power
		none,		// Quick Guard
		none,		// Ally Switch
		100_ri,		// Scald
		none,		// Shell Smash
		none,		// Heal Pulse
		100_ri,		// Hex
		100_ri,		// Sky Drop
		none,		// Shift Gear
		90_ri,		// Circle Throw
		100_ri,		// Incinerate
		100_ri,		// Quash
		100_ri,		// Acrobatics
		none,		// Reflect Type
		100_ri,		// Retaliate
		100_ri,		// Final Gambit
		none,		// Bestow
		50_ri,		// Inferno
		100_ri,		// Water Pledge
		100_ri,		// Fire Pledge
		100_ri,		// Grass Pledge
		100_ri,		// Volt Switch
		100_ri,		// Struggle Bug
		100_ri,		// Bulldoze
		90_ri,		// Frost Breath
		90_ri,		// Dragon Tail
		none,		// Work Up
		95_ri,		// Electroweb
		100_ri,		// Wild Charge
		95_ri,		// Drill Run
		90_ri,		// Dual Chop
		100_ri,		// Heart Stamp
		100_ri,		// Horn Leech
		100_ri,		// Sacred Sword
		95_ri,		// Razor Shell
		100_ri,		// Heat Crash
		90_ri,		// Leaf Tornado
		100_ri,		// Steamroller
		none,		// Cotton Guard
		95_ri,		// Night Daze
		100_ri,		// Psystrike
		85_ri,		// Tail Slap
		70_ri,		// Hurricane
		100_ri,		// Head Charge
		85_ri,		// Gear Grind
		100_ri,		// Searing Shot
		100_ri,		// Techno Blast
		100_ri,		// Relic Song
		100_ri,		// Secret Sword
		95_ri,		// Glaciate
		85_ri,		// Bolt Strike
		85_ri,		// Blue Flare
		100_ri,		// Fiery Dance
		90_ri,		// Freeze Shock
		90_ri,		// Ice Burn
		95_ri,		// Snarl
		90_ri,		// Icicle Crash
		95_ri,		// V-create
		100_ri,		// Fusion Flare
		100_ri		// Fusion Bolt
	);
	static_assert(std::is_same<decltype(get_accuracy)::value_type, decltype(accuracy(std::declval<Moves>()))>::value, "Array type is not the same as the return type of function.");
	return get_accuracy[static_cast<size_t>(move)];
}

}	// namespace technicalmachine
