// Moves classification (physical, special, or other)
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

#include "classification.hpp"

#include <cstddef>
#include <cstdint>

namespace technicalmachine {

enum class Classification::Cached : int8_t { special = -1, neither = 0, physical = 1 };

Classification::Classification(Moves const move):
	cached(classification(move)) {
}

bool Classification::is_physical() const {
	return cached == Cached::physical;
}

bool Classification::is_special() const {
	return cached == Cached::special;
}

Classification::Cached Classification::classification (Moves const move) {
	static constexpr Cached lookup [] = {
		Cached::neither,		// Switch0
		Cached::neither,		// Switch1
		Cached::neither,		// Switch2
		Cached::neither,		// Switch3
		Cached::neither,		// Switch4
		Cached::neither,		// Switch5
		Cached::physical,		// Hit Self
		Cached::physical,		// Pound
		Cached::physical,		// Karate Chop
		Cached::physical,		// DoubleSlap
		Cached::physical,		// Comet Punch
		Cached::physical,		// Mega Punch
		Cached::physical,		// Pay Day
		Cached::physical,		// Fire Punch
		Cached::physical,		// Ice Punch
		Cached::physical,		// ThunderPunch
		Cached::physical,		// Scratch
		Cached::physical,		// ViceGrip
		Cached::physical,		// Guillotine
		Cached::special,		// Razor Wind
		Cached::neither,		// Swords Dance
		Cached::physical,		// Cut
		Cached::special,		// Gust
		Cached::physical,		// Wing Attack
		Cached::neither,		// Whirlwind
		Cached::physical,		// Fly
		Cached::physical,		// Bind
		Cached::physical,		// Slam
		Cached::physical,		// Vine Whip
		Cached::physical,		// Stomp
		Cached::physical,		// Double Kick
		Cached::physical,		// Mega Kick
		Cached::physical,		// Jump Kick
		Cached::physical,		// Rolling Kick
		Cached::neither,		// Sand-Attack
		Cached::physical,		// Headbutt
		Cached::physical,		// Horn Attack
		Cached::physical,		// Fury Attack
		Cached::physical,		// Horn Drill
		Cached::physical,		// Tackle
		Cached::physical,		// Body Slam
		Cached::physical,		// Wrap
		Cached::physical,		// Take Down
		Cached::physical,		// Thrash
		Cached::physical,		// Double-Edge
		Cached::neither,		// Tail Whip
		Cached::physical,		// Poison Sting
		Cached::physical,		// Twineedle
		Cached::physical,		// Pin Missile
		Cached::neither,		// Leer
		Cached::physical,		// Bite
		Cached::neither,		// Growl
		Cached::neither,		// Roar
		Cached::neither,		// Sing
		Cached::neither,		// Supersonic
		Cached::special,		// SonicBoom
		Cached::neither,		// Disable
		Cached::special,		// Acid
		Cached::special,		// Ember
		Cached::special,		// Flamethrower
		Cached::neither,		// Mist
		Cached::special,		// Water Gun
		Cached::special,		// Hydro Pump
		Cached::special,		// Surf
		Cached::special,		// Ice Beam
		Cached::special,		// Blizzard
		Cached::special,		// Psybeam
		Cached::special,		// BubbleBeam
		Cached::special,		// Aurora Beam
		Cached::special,		// Hyper Beam
		Cached::physical,		// Peck
		Cached::physical,		// Drill Peck
		Cached::physical,		// Submission
		Cached::physical,		// Low Kick
		Cached::physical,		// Counter
		Cached::physical,		// Seismic Toss
		Cached::physical,		// Strength
		Cached::special,		// Absorb
		Cached::special,		// Mega Drain
		Cached::neither,		// Leech Seed
		Cached::neither,		// Growth
		Cached::physical,		// Razor Leaf
		Cached::special,		// SolarBeam
		Cached::neither,		// PoisonPowder
		Cached::neither,		// Stun Spore
		Cached::neither,		// Sleep Powder
		Cached::special,		// Petal Dance
		Cached::neither,		// String Shot
		Cached::special,		// Dragon Rage
		Cached::special,		// Fire Spin
		Cached::special,		// ThunderShock
		Cached::special,		// Thunderbolt
		Cached::neither,		// Thunder Wave
		Cached::special,		// Thunder
		Cached::physical,		// Rock Throw
		Cached::physical,		// Earthquake
		Cached::physical,		// Fissure
		Cached::physical,		// Dig
		Cached::neither,		// Toxic
		Cached::special,		// Confusion
		Cached::special,		// Psychic
		Cached::neither,		// Hypnosis
		Cached::neither,		// Meditate
		Cached::neither,		// Agility
		Cached::physical,		// Quick Attack
		Cached::physical,		// Rage
		Cached::neither,		// Teleport
		Cached::special,		// Night Shade
		Cached::neither,		// Mimic
		Cached::neither,		// Screech
		Cached::neither,		// Double Team
		Cached::neither,		// Recover
		Cached::neither,		// Harden
		Cached::neither,		// Minimize
		Cached::neither,		// SmokeScreen
		Cached::neither,		// Confuse Ray
		Cached::neither,		// Withdraw
		Cached::neither,		// Defense Curl
		Cached::neither,		// Barrier
		Cached::neither,		// Light Screen
		Cached::neither,		// Haze
		Cached::neither,		// Reflect
		Cached::neither,		// Focus Energy
		Cached::physical,		// Bide
		Cached::neither,		// Metronome
		Cached::neither,		// Mirror Move
		Cached::physical,		// Selfdestruct
		Cached::physical,		// Egg Bomb
		Cached::physical,		// Lick
		Cached::special,		// Smog
		Cached::special,		// Sludge
		Cached::physical,		// Bone Club
		Cached::special,		// Fire Blast
		Cached::physical,		// Waterfall
		Cached::physical,		// Clamp
		Cached::special,		// Swift
		Cached::physical,		// Skull Bash
		Cached::physical,		// Spike Cannon
		Cached::physical,		// Constrict
		Cached::neither,		// Amnesia
		Cached::neither,		// Kinesis
		Cached::neither,		// Softboiled
		Cached::physical,		// Hi Jump Kick
		Cached::neither,		// Glare
		Cached::special,		// Dream Eater
		Cached::neither,		// Poison Gas
		Cached::physical,		// Barrage
		Cached::physical,		// Leech Life
		Cached::neither,		// Lovely Kiss
		Cached::physical,		// Sky Attack
		Cached::neither,		// Transform
		Cached::special,		// Bubble
		Cached::physical,		// Dizzy Punch
		Cached::neither,		// Spore
		Cached::neither,		// Flash
		Cached::special,		// Psywave
		Cached::neither,		// Splash
		Cached::neither,		// Acid Armor
		Cached::physical,		// Crabhammer
		Cached::physical,		// Explosion
		Cached::physical,		// Fury Swipes
		Cached::physical,		// Bonemerang
		Cached::neither,		// Rest
		Cached::physical,		// Rock Slide
		Cached::physical,		// Hyper Fang
		Cached::neither,		// Sharpen
		Cached::neither,		// Conversion
		Cached::special,		// Tri Attack
		Cached::physical,		// Super Fang
		Cached::physical,		// Slash
		Cached::neither,		// Substitute
		Cached::physical,		// Struggle
		Cached::neither,		// Sketch
		Cached::physical,		// Triple Kick
		Cached::physical,		// Thief
		Cached::neither,		// Spider Web
		Cached::neither,		// Mind Reader
		Cached::neither,		// Nightmare
		Cached::physical,		// Flame Wheel
		Cached::special,		// Snore
		Cached::neither,		// Curse
		Cached::physical,		// Flail
		Cached::neither,		// Conversion 2
		Cached::special,		// Aeroblast
		Cached::neither,		// Cotton Spore
		Cached::physical,		// Reversal
		Cached::neither,		// Spite
		Cached::special,		// Powder Snow
		Cached::neither,		// Protect
		Cached::physical,		// Mach Punch
		Cached::neither,		// Scary Face
		Cached::physical,		// Faint Attack
		Cached::neither,		// Sweet Kiss
		Cached::neither,		// Belly Drum
		Cached::special,		// Sludge Bomb
		Cached::special,		// Mud-Slap
		Cached::special,		// Octazooka
		Cached::neither,		// Spikes
		Cached::special,		// Zap Cannon
		Cached::neither,		// Foresight
		Cached::neither,		// Destiny Bond
		Cached::neither,		// Perish Song
		Cached::special,		// Icy Wind
		Cached::neither,		// Detect
		Cached::physical,		// Bone Rush
		Cached::neither,		// Lock-On
		Cached::physical,		// Outrage
		Cached::neither,		// Sandstorm
		Cached::special,		// Giga Drain
		Cached::neither,		// Endure
		Cached::neither,		// Charm
		Cached::physical,		// Rollout
		Cached::physical,		// False Swipe
		Cached::neither,		// Swagger
		Cached::neither,		// Milk Drink
		Cached::physical,		// Spark
		Cached::physical,		// Fury Cutter
		Cached::physical,		// Steel Wing
		Cached::neither,		// Mean Look
		Cached::neither,		// Attract
		Cached::neither,		// Sleep Talk
		Cached::neither,		// Heal Bell
		Cached::physical,		// Return
		Cached::physical,		// Present
		Cached::physical,		// Frustration
		Cached::neither,		// Safeguard
		Cached::neither,		// Pain Split
		Cached::physical,		// Sacred Fire
		Cached::physical,		// Magnitude
		Cached::physical,		// DynamicPunch
		Cached::physical,		// Megahorn
		Cached::special,		// DragonBreath
		Cached::neither,		// Baton Pass
		Cached::neither,		// Encore
		Cached::physical,		// Pursuit
		Cached::physical,		// Rapid Spin
		Cached::neither,		// Sweet Scent
		Cached::physical,		// Iron Tail
		Cached::physical,		// Metal Claw
		Cached::physical,		// Vital Throw
		Cached::neither,		// Morning Sun
		Cached::neither,		// Synthesis
		Cached::neither,		// Moonlight
		Cached::special,		// Hidden Power
		Cached::physical,		// Cross Chop
		Cached::special,		// Twister
		Cached::neither,		// Rain Dance
		Cached::neither,		// Sunny Day
		Cached::physical,		// Crunch
		Cached::special,		// Mirror Coat
		Cached::neither,		// Psych Up
		Cached::physical,		// ExtremeSpeed
		Cached::special,		// AncientPower
		Cached::special,		// Shadow Ball
		Cached::special,		// Future Sight
		Cached::physical,		// Rock Smash
		Cached::special,		// Whirlpool
		Cached::physical,		// Beat Up
		Cached::physical,		// Fake Out
		Cached::special,		// Uproar
		Cached::neither,		// Stockpile
		Cached::special,		// Spit Up
		Cached::neither,		// Swallow
		Cached::special,		// Heat Wave
		Cached::neither,		// Hail
		Cached::neither,		// Torment
		Cached::neither,		// Flatter
		Cached::neither,		// Will-O-Wisp
		Cached::neither,		// Memento
		Cached::physical,		// Facade
		Cached::physical,		// Focus Punch
		Cached::physical,		// SmellingSalt
		Cached::neither,		// Follow Me
		Cached::neither,		// Nature Power
		Cached::neither,		// Charge
		Cached::neither,		// Taunt
		Cached::neither,		// Helping Hand
		Cached::neither,		// Trick
		Cached::neither,		// Role Play
		Cached::neither,		// Wish
		Cached::neither,		// Assist
		Cached::neither,		// Ingrain
		Cached::physical,		// Superpower
		Cached::neither,		// Magic Coat
		Cached::neither,		// Recycle
		Cached::physical,		// Revenge
		Cached::physical,		// Brick Break
		Cached::neither,		// Yawn
		Cached::physical,		// Knock Off
		Cached::physical,		// Endeavor
		Cached::special,		// Eruption
		Cached::neither,		// Skill Swap
		Cached::neither,		// Imprison
		Cached::neither,		// Refresh
		Cached::neither,		// Grudge
		Cached::neither,		// Snatch
		Cached::physical,		// Secret Power
		Cached::physical,		// Dive
		Cached::physical,		// Arm Thrust
		Cached::neither,		// Camouflage
		Cached::neither,		// Tail Glow
		Cached::special,		// Luster Purge
		Cached::special,		// Mist Ball
		Cached::neither,		// FeatherDance
		Cached::neither,		// Teeter Dance
		Cached::physical,		// Blaze Kick
		Cached::neither,		// Mud Sport
		Cached::physical,		// Ice Ball
		Cached::physical,		// Needle Arm
		Cached::neither,		// Slack Off
		Cached::special,		// Hyper Voice
		Cached::physical,		// Poison Fang
		Cached::physical,		// Crush Claw
		Cached::special,		// Blast Burn
		Cached::special,		// Hydro Cannon
		Cached::physical,		// Meteor Mash
		Cached::physical,		// Astonish
		Cached::special,		// Weather Ball
		Cached::neither,		// Aromatherapy
		Cached::neither,		// Fake Tears
		Cached::special,		// Air Cutter
		Cached::special,		// Overheat
		Cached::neither,		// Odor Sleuth
		Cached::physical,		// Rock Tomb
		Cached::special,		// Silver Wind
		Cached::neither,		// Metal Sound
		Cached::neither,		// GrassWhistle
		Cached::neither,		// Tickle
		Cached::neither,		// Cosmic Power
		Cached::special,		// Water Spout
		Cached::special,		// Signal Beam
		Cached::physical,		// Shadow Punch
		Cached::special,		// Extrasensory
		Cached::physical,		// Sky Uppercut
		Cached::physical,		// Sand Tomb
		Cached::special,		// Sheer Cold
		Cached::special,		// Muddy Water
		Cached::physical,		// Bullet Seed
		Cached::physical,		// Aerial Ace
		Cached::physical,		// Icicle Spear
		Cached::neither,		// Iron Defense
		Cached::neither,		// Block
		Cached::neither,		// Howl
		Cached::physical,		// Dragon Claw
		Cached::special,		// Frenzy Plant
		Cached::neither,		// Bulk Up
		Cached::physical,		// Bounce
		Cached::special,		// Mud Shot
		Cached::physical,		// Poison Tail
		Cached::physical,		// Covet
		Cached::physical,		// Volt Tackle
		Cached::special,		// Magical Leaf
		Cached::neither,		// Water Sport
		Cached::neither,		// Calm Mind
		Cached::physical,		// Leaf Blade
		Cached::neither,		// Dragon Dance
		Cached::physical,		// Rock Blast
		Cached::special,		// Shock Wave
		Cached::special,		// Water Pulse
		Cached::special,		// Doom Desire
		Cached::special,		// Psycho Boost
		Cached::neither,		// Roost
		Cached::neither,		// Gravity
		Cached::neither,		// Miracle Eye
		Cached::physical,		// Wake-Up Slap
		Cached::physical,		// Hammer Arm
		Cached::physical,		// Gyro Ball
		Cached::neither,		// Healing Wish
		Cached::special,		// Brine
		Cached::physical,		// Natural Gift
		Cached::physical,		// Feint
		Cached::physical,		// Pluck
		Cached::neither,		// Tailwind
		Cached::neither,		// Acupressure
		Cached::physical,		// Metal Burst
		Cached::physical,		// U-turn
		Cached::physical,		// Close Combat
		Cached::physical,		// Payback
		Cached::physical,		// Assurance
		Cached::neither,		// Embargo
		Cached::physical,		// Fling
		Cached::neither,		// Psycho Shift
		Cached::special,		// Trump Card
		Cached::neither,		// Heal Block
		Cached::special,		// Wring Out
		Cached::neither,		// Power Trick
		Cached::neither,		// Gastro Acid
		Cached::neither,		// Lucky Chant
		Cached::neither,		// Me First
		Cached::neither,		// Copycat
		Cached::neither,		// Power Swap
		Cached::neither,		// Guard Swap
		Cached::physical,		// Punishment
		Cached::physical,		// Last Resort
		Cached::neither,		// Worry Seed
		Cached::physical,		// Sucker Punch
		Cached::neither,		// Toxic Spikes
		Cached::neither,		// Heart Swap
		Cached::neither,		// Aqua Ring
		Cached::neither,		// Magnet Rise
		Cached::physical,		// Flare Blitz
		Cached::physical,		// Force Palm
		Cached::special,		// Aura Sphere
		Cached::neither,		// Rock Polish
		Cached::physical,		// Poison Jab
		Cached::special,		// Dark Pulse
		Cached::physical,		// Night Slash
		Cached::physical,		// Aqua Tail
		Cached::physical,		// Seed Bomb
		Cached::special,		// Air Slash
		Cached::physical,		// X-Scissor
		Cached::special,		// Bug Buzz
		Cached::special,		// Dragon Pulse
		Cached::physical,		// Dragon Rush
		Cached::special,		// Power Gem
		Cached::physical,		// Drain Punch
		Cached::special,		// Vacuum Wave
		Cached::special,		// Focus Blast
		Cached::special,		// Energy Ball
		Cached::physical,		// Brave Bird
		Cached::special,		// Earth Power
		Cached::neither,		// Switcheroo
		Cached::physical,		// Giga Impact
		Cached::neither,		// Nasty Plot
		Cached::physical,		// Bullet Punch
		Cached::physical,		// Avalanche
		Cached::physical,		// Ice Shard
		Cached::physical,		// Shadow Claw
		Cached::physical,		// Thunder Fang
		Cached::physical,		// Ice Fang
		Cached::physical,		// Fire Fang
		Cached::physical,		// Shadow Sneak
		Cached::special,		// Mud Bomb
		Cached::physical,		// Psycho Cut
		Cached::physical,		// Zen Headbutt
		Cached::special,		// Mirror Shot
		Cached::special,		// Flash Cannon
		Cached::physical,		// Rock Climb
		Cached::neither,		// Defog
		Cached::neither,		// Trick Room
		Cached::special,		// Draco Meteor
		Cached::special,		// Discharge
		Cached::special,		// Lava Plume
		Cached::special,		// Leaf Storm
		Cached::physical,		// Power Whip
		Cached::physical,		// Rock Wrecker
		Cached::physical,		// Cross Poison
		Cached::physical,		// Gunk Shot
		Cached::physical,		// Iron Head
		Cached::physical,		// Magnet Bomb
		Cached::physical,		// Stone Edge
		Cached::neither,		// Captivate
		Cached::neither,		// Stealth Rock
		Cached::special,		// Grass Knot
		Cached::special,		// Chatter
		Cached::special,		// Judgment
		Cached::physical,		// Bug Bite
		Cached::special,		// Charge Beam
		Cached::physical,		// Wood Hammer
		Cached::physical,		// Aqua Jet
		Cached::physical,		// Attack Order
		Cached::neither,		// Defend Order
		Cached::neither,		// Heal Order
		Cached::physical,		// Head Smash
		Cached::physical,		// Double Hit
		Cached::special,		// Roar of Time
		Cached::special,		// Spacial Rend
		Cached::neither,		// Lunar Dance
		Cached::physical,		// Crush Grip
		Cached::special,		// Magma Storm
		Cached::neither,		// Dark Void
		Cached::special,		// Seed Flare
		Cached::special,		// Ominous Wind
		Cached::physical,		// Shadow Force
		Cached::neither,		// Hone Claws
		Cached::neither,		// Wide Guard
		Cached::neither,		// Guard Split
		Cached::neither,		// Power Split
		Cached::neither,		// Wonder Room
		Cached::special,		// Psyshock
		Cached::special,		// Venoshock
		Cached::neither,		// Autotomize
		Cached::neither,		// Rage Powder
		Cached::neither,		// Telekinesis
		Cached::neither,		// Magic Room
		Cached::physical,		// Smack Down
		Cached::physical,		// Storm Throw
		Cached::special,		// Flame Burst
		Cached::special,		// Sludge Wave
		Cached::neither,		// Quiver Dance
		Cached::physical,		// Heavy Slam
		Cached::special,		// Synchronoise
		Cached::special,		// Electro Ball
		Cached::neither,		// Soak
		Cached::physical,		// Flame Charge
		Cached::neither,		// Coil
		Cached::physical,		// Low Sweep
		Cached::special,		// Acid Spray
		Cached::physical,		// Foul Play
		Cached::neither,		// Simple Beam
		Cached::neither,		// Entrainment
		Cached::neither,		// After You
		Cached::special,		// Round
		Cached::special,		// Echoed Voice
		Cached::physical,		// Chip Away
		Cached::special,		// Clear Smog
		Cached::special,		// Stored Power
		Cached::neither,		// Quick Guard
		Cached::neither,		// Ally Switch
		Cached::special,		// Scald
		Cached::neither,		// Shell Smash
		Cached::neither,		// Heal Pulse
		Cached::special,		// Hex
		Cached::physical,		// Sky Drop
		Cached::neither,		// Shift Gear
		Cached::physical,		// Circle Throw
		Cached::special,		// Incinerate
		Cached::neither,		// Quash
		Cached::physical,		// Acrobatics
		Cached::neither,		// Reflect Type
		Cached::physical,		// Retaliate
		Cached::special,		// Final Gambit
		Cached::neither,		// Bestow
		Cached::special,		// Inferno
		Cached::special,		// Water Pledge
		Cached::special,		// Fire Pledge
		Cached::special,		// Grass Pledge
		Cached::special,		// Volt Switch
		Cached::special,		// Struggle Bug
		Cached::physical,		// Bulldoze
		Cached::special,		// Frost Breath
		Cached::physical,		// Dragon Tail
		Cached::neither,		// Work Up
		Cached::special,		// Electroweb
		Cached::physical,		// Wild Charge
		Cached::physical,		// Drill Run
		Cached::physical,		// Dual Chop
		Cached::physical,		// Heart Stamp
		Cached::physical,		// Horn Leech
		Cached::physical,		// Sacred Sword
		Cached::physical,		// Razor Shell
		Cached::physical,		// Heat Crash
		Cached::special,		// Leaf Tornado
		Cached::physical,		// Steamroller
		Cached::neither,		// Cotton Guard
		Cached::special,		// Night Daze
		Cached::special,		// Psystrike
		Cached::physical,		// Tail Slap
		Cached::special,		// Hurricane
		Cached::physical,		// Head Charge
		Cached::physical,		// Gear Grind
		Cached::special,		// Searing Shot
		Cached::special,		// Techno Blast
		Cached::special,		// Relic Song
		Cached::special,		// Secret Sword
		Cached::special,		// Glaciate
		Cached::physical,		// Bolt Strike
		Cached::special,		// Blue Flare
		Cached::special,		// Fiery Dance
		Cached::physical,		// Freeze Shock
		Cached::special,		// Ice Burn
		Cached::special,		// Snarl
		Cached::physical,		// Icicle Crash
		Cached::physical,		// V-create
		Cached::special,		// Fusion Flare
		Cached::physical		// Fusion Bolt
	};
	return lookup[static_cast<size_t>(move)];
}

}	// namespace technicalmachine
