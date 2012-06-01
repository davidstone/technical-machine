// Accuracy data structure
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

#include "accuracy.hpp"

namespace technicalmachine {
namespace {

constexpr uint8_t perfect_accuracy = 0xFF;
uint8_t get_accuracy (Moves move);

}	// unnamed namespace

Accuracy::Accuracy(Moves const move) :
	accuracy(get_accuracy(move)) {
}

uint8_t Accuracy::operator() () const {
	return accuracy;
}

bool Accuracy::can_miss () const {
	return accuracy != perfect_accuracy;
}

namespace {

uint8_t get_accuracy (Moves const move) {
	static constexpr uint8_t get_accuracy [] = {
		100,		// Absorb
		100,		// Acid
		perfect_accuracy,		// Acid Armor
		perfect_accuracy,		// Acupressure
		perfect_accuracy,		// Aerial Ace
		95,		// Aeroblast
		perfect_accuracy,		// Agility
		95,		// Air Cutter
		95,		// Air Slash
		perfect_accuracy,		// Amnesia
		100,		// AncientPower
		100,		// Aqua Jet
		perfect_accuracy,		// Aqua Ring
		90,		// Aqua Tail
		100,		// Arm Thrust
		perfect_accuracy,		// Aromatherapy
		perfect_accuracy,		// Assist
		100,		// Assurance
		100,		// Astonish
		100,		// Attack Order
		100,		// Attract
		perfect_accuracy,		// Aura Sphere
		100,		// Aurora Beam
		100,		// Avalanche
		85,		// Barrage
		perfect_accuracy,		// Barrier
		perfect_accuracy,		// Baton Pass
		100,		// Beat Up
		perfect_accuracy,		// Belly Drum
		perfect_accuracy,		// Bide
		75,		// Bind
		100,		// Bite
		90,		// Blast Burn
		90,		// Blaze Kick
		70,		// Blizzard
		perfect_accuracy,		// Block
		100,		// Body Slam
		85,		// Bone Club
		80,		// Bone Rush
		90,		// Bonemerang
		85,		// Bounce
		100,		// Brave Bird
		100,		// Brick Break
		100,		// Brine
		100,		// Bubble
		100,		// BubbleBeam
		100,		// Bug Bite
		100,		// Bug Buzz
		perfect_accuracy,		// Bulk Up
		100,		// Bullet Punch
		100,		// Bullet Seed
		perfect_accuracy,		// Calm Mind
		perfect_accuracy,		// Camouflage
		100,		// Captivate
		perfect_accuracy,		// Charge
		90,		// Charge Beam
		100,		// Charm
		100,		// Chatter
		75,		// Clamp
		100,		// Close Combat
		85,		// Comet Punch
		100,		// Confuse Ray
		100,		// Confusion
		100,		// Constrict
		perfect_accuracy,		// Conversion
		perfect_accuracy,		// Conversion2
		perfect_accuracy,		// Copycat
		perfect_accuracy,		// Cosmic Power
		85,		// Cotton Spore
		100,		// Counter
		100,		// Covet
		85,		// Crabhammer
		80,		// Cross Chop
		100,		// Cross Poison
		100,		// Crunch
		95,		// Crush Claw
		100,		// Crush Grip
		perfect_accuracy,		// Curse
		95,		// Cut
		100,		// Dark Pulse
		80,		// Dark Void
		perfect_accuracy,		// Defend Order
		perfect_accuracy,		// Defense Curl
		perfect_accuracy,		// Defog
		perfect_accuracy,		// Destiny Bond
		perfect_accuracy,		// Detect
		100,		// Dig
		80,		// Disable
		100,		// Discharge
		100,		// Dive
		100,		// Dizzy Punch
		85,		// Doom Desire
		90,		// Double Hit
		100,		// Double Kick
		perfect_accuracy,		// Double Team
		100,		// Double-Edge
		85,		// DoubleSlap
		90,		// Draco Meteor
		100,		// Dragon Claw
		perfect_accuracy,		// Dragon Dance
		100,		// Dragon Pulse
		100,		// Dragon Rage
		75,		// Dragon Rush
		100,		// DragonBreath
		100,		// Drain Punch
		100,		// Dream Eater
		100,		// Drill Peck
		50,		// DynamicPunch
		100,		// Earth Power
		100,		// Earthquake
		75,		// Egg Bomb
		100,		// Embargo
		100,		// Ember
		100,		// Encore
		100,		// Endeavor
		perfect_accuracy,		// Endure
		100,		// Energy Ball
		100,		// Eruption
		100,		// Explosion
		100,		// Extrasensory
		100,		// ExtremeSpeed
		100,		// Facade
		perfect_accuracy,		// Faint Attack
		100,		// Fake Out
		100,		// Fake Tears
		100,		// False Swipe
		100,		// FeatherDance
		100,		// Feint
		85,		// Fire Blast
		95,		// Fire Fang
		100,		// Fire Punch
		70,		// Fire Spin
		30,		// Fissure
		100,		// Flail
		100,		// Flame Wheel
		100,		// Flamethrower
		100,		// Flare Blitz
		100,		// Flash
		100,		// Flash Cannon
		100,		// Flatter
		100,		// Fling
		95,		// Fly
		70,		// Focus Blast
		perfect_accuracy,		// Focus Energy
		100,		// Focus Punch
		perfect_accuracy,		// Follow Me
		100,		// Force Palm
		perfect_accuracy,		// Foresight
		90,		// Frenzy Plant
		100,		// Frustration
		85,		// Fury Attack
		95,		// Fury Cutter
		80,		// Fury Swipes
		90,		// Future Sight
		100,		// Gastro Acid
		100,		// Giga Drain
		90,		// Giga Impact
		75,		// Glare
		100,		// Grass Knot
		55,		// GrassWhistle
		perfect_accuracy,		// Gravity
		100,		// Growl
		perfect_accuracy,		// Growth
		perfect_accuracy,		// Grudge
		perfect_accuracy,		// Guard Swap
		30,		// Guillotine
		70,		// Gunk Shot
		100,		// Gust
		100,		// Gyro Ball
		perfect_accuracy,		// Hail
		90,		// Hammer Arm
		perfect_accuracy,		// Harden
		perfect_accuracy,		// Haze
		80,		// Head Smash
		100,		// Headbutt
		perfect_accuracy,		// Heal Bell
		100,		// Heal Block
		perfect_accuracy,		// Heal Order
		perfect_accuracy,		// Healing Wish
		perfect_accuracy,		// Heart Swap
		90,		// Heat Wave
		perfect_accuracy,		// Helping Hand
		90,		// Hi Jump Kick
		100,		// Hidden Power
		100,		// Horn Attack
		30,		// Horn Drill
		perfect_accuracy,		// Howl
		90,		// Hydro Cannon
		80,		// Hydro Pump
		90,		// Hyper Beam
		90,		// Hyper Fang
		100,		// Hyper Voice
		60,		// Hypnosis
		90,		// Ice Ball
		100,		// Ice Beam
		95,		// Ice Fang
		100,		// Ice Punch
		100,		// Ice Shard
		100,		// Icicle Spear
		95,		// Icy Wind
		perfect_accuracy,		// Imprison
		perfect_accuracy,		// Ingrain
		perfect_accuracy,		// Iron Defense
		100,		// Iron Head
		75,		// Iron Tail
		100,		// Judgment
		95,		// Jump Kick
		100,		// Karate Chop
		80,		// Kinesis
		100,		// Knock Off
		100,		// Last Resort
		100,		// Lava Plume
		100,		// Leaf Blade
		90,		// Leaf Storm
		100,		// Leech Life
		90,		// Leech Seed
		100,		// Leer
		100,		// Lick
		perfect_accuracy,		// Light Screen
		perfect_accuracy,		// Lock-On
		75,		// Lovely Kiss
		100,		// Low Kick
		perfect_accuracy,		// Lucky Chant
		perfect_accuracy,		// Lunar Dance
		100,		// Luster Purge
		100,		// Mach Punch
		perfect_accuracy,		// Magic Coat
		perfect_accuracy,		// Magical Leaf
		70,		// Magma Storm
		perfect_accuracy,		// Magnet Bomb
		perfect_accuracy,		// Magnet Rise
		100,		// Magnitude
		perfect_accuracy,		// Me First
		perfect_accuracy,		// Mean Look
		perfect_accuracy,		// Meditate
		100,		// Mega Drain
		75,		// Mega Kick
		85,		// Mega Punch
		85,		// Megahorn
		100,		// Memento
		100,		// Metal Burst
		95,		// Metal Claw
		85,		// Metal Sound
		85,		// Meteor Mash
		perfect_accuracy,		// Metronome
		perfect_accuracy,		// Milk Drink
		perfect_accuracy,		// Mimic
		perfect_accuracy,		// Mind Reader
		perfect_accuracy,		// Minimize
		perfect_accuracy,		// Miracle Eye
		100,		// Mirror Coat
		perfect_accuracy,		// Mirror Move
		85,		// Mirror Shot
		perfect_accuracy,		// Mist
		100,		// Mist Ball
		perfect_accuracy,		// Moonlight
		perfect_accuracy,		// Morning Sun
		85,		// Mud Bomb
		95,		// Mud Shot
		perfect_accuracy,		// Mud Sport
		100,		// Mud-Slap
		85,		// Muddy Water
		perfect_accuracy,		// Nasty Plot
		100,		// Natural Gift
		perfect_accuracy,		// Nature Power
		100,		// Needle Arm
		100,		// Night Shade
		100,		// Night Slash
		100,		// Nightmare
		85,		// Octazooka
		perfect_accuracy,		// Odor Sleuth
		100,		// Ominous Wind
		100,		// Outrage
		90,		// Overheat
		perfect_accuracy,		// Pain Split
		100,		// Pay Day
		100,		// Payback
		100,		// Peck
		perfect_accuracy,		// Perish Song
		100,		// Petal Dance
		85,		// Pin Missile
		100,		// Pluck
		100,		// Poison Fang
		55,		// Poison Gas
		100,		// Poison Jab
		100,		// Poison Sting
		100,		// Poison Tail
		75,		// PoisonPowder
		100,		// Pound
		100,		// Powder Snow
		100,		// Power Gem
		perfect_accuracy,		// Power Swap
		perfect_accuracy,		// Power Trick
		85,		// Power Whip
		90,		// Present
		perfect_accuracy,		// Protect
		100,		// Psybeam
		perfect_accuracy,		// Psych Up
		100,		// Psychic
		90,		// Psycho Boost
		100,		// Psycho Cut
		90,		// Psycho Shift
		80,		// Psywave
		100,		// Punishment
		100,		// Pursuit
		100,		// Quick Attack
		100,		// Rage
		perfect_accuracy,		// Rain Dance
		100,		// Rapid Spin
		95,		// Razor Leaf
		100,		// Razor Wind
		perfect_accuracy,		// Recover
		perfect_accuracy,		// Recycle
		perfect_accuracy,		// Reflect
		perfect_accuracy,		// Refresh
		perfect_accuracy,		// Rest
		100,		// Return
		100,		// Revenge
		100,		// Reversal
		100,		// Roar
		90,		// Roar Of Time
		80,		// Rock Blast
		85,		// Rock Climb
		perfect_accuracy,		// Rock Polish
		90,		// Rock Slide
		100,		// Rock Smash
		90,		// Rock Throw
		80,		// Rock Tomb
		90,		// Rock Wrecker
		perfect_accuracy,		// Role Play
		85,		// Rolling Kick
		90,		// Rollout
		perfect_accuracy,		// Roost
		95,		// Sacred Fire
		perfect_accuracy,		// Safeguard
		70,		// Sand Tomb
		100,		// Sand-Attack
		perfect_accuracy,		// Sandstorm
		90,		// Scary Face
		100,		// Scratch
		85,		// Screech
		100,		// Secret Power
		100,		// Seed Bomb
		85,		// Seed Flare
		100,		// Seismic Toss
		100,		// Selfdestruct
		100,		// Shadow Ball
		100,		// Shadow Claw
		100,		// Shadow Force
		perfect_accuracy,		// Shadow Punch
		100,		// Shadow Sneak
		perfect_accuracy,		// Sharpen
		30,		// Sheer Cold
		perfect_accuracy,		// Shock Wave
		100,		// Signal Beam
		100,		// Silver Wind
		55,		// Sing
		perfect_accuracy,		// Sketch
		perfect_accuracy,		// Skill Swap
		100,		// Skull Bash
		90,		// Sky Attack
		90,		// Sky Uppercut
		perfect_accuracy,		// Slack Off
		75,		// Slam
		100,		// Slash
		75,		// Sleep Powder
		perfect_accuracy,		// Sleep Talk
		100,		// Sludge
		100,		// Sludge Bomb
		100,		// SmellingSalt
		70,		// Smog
		100,		// SmokeScreen
		perfect_accuracy,		// Snatch
		100,		// Snore
		perfect_accuracy,		// Softboiled
		100,		// SolarBeam
		90,		// SonicBoom
		95,		// Spacial Rend
		100,		// Spark
		perfect_accuracy,		// Spider Web
		100,		// Spike Cannon
		perfect_accuracy,		// Spikes
		100,		// Spit Up
		100,		// Spite
		perfect_accuracy,		// Splash
		100,		// Spore
		perfect_accuracy,		// Stealth Rock
		90,		// Steel Wing
		perfect_accuracy,		// Stockpile
		100,		// Stomp
		80,		// Stone Edge
		100,		// Strength
		95,		// String Shot
		perfect_accuracy,		// Struggle
		75,		// Stun Spore
		80,		// Submission
		perfect_accuracy,		// Substitute
		100,		// Sucker Punch
		perfect_accuracy,		// Sunny Day
		90,		// Super Fang
		100,		// Superpower
		55,		// Supersonic
		100,		// Surf
		90,		// Swagger
		perfect_accuracy,		// Swallow
		75,		// Sweet Kiss
		100,		// Sweet Scent
		perfect_accuracy,		// Swift
		perfect_accuracy,		// Switch0
		perfect_accuracy,		// Switch1
		perfect_accuracy,		// Switch2
		perfect_accuracy,		// Switch3
		perfect_accuracy,		// Switch4
		perfect_accuracy,		// Switch5
		100,		// Switcheroo
		perfect_accuracy,		// Swords Dance
		perfect_accuracy,		// Synthesis
		95,		// Tackle
		perfect_accuracy,		// Tail Glow
		100,		// Tail Whip
		perfect_accuracy,		// Tailwind
		85,		// Take Down
		100,		// Taunt
		100,		// Teeter Dance
		perfect_accuracy,		// Teleport
		100,		// Thief
		100,		// Thrash
		70,		// Thunder
		95,		// Thunder Fang
		100,		// Thunder Wave
		100,		// Thunderbolt
		100,		// ThunderPunch
		100,		// ThunderShock
		100,		// Tickle
		100,		// Torment
		85,		// Toxic
		perfect_accuracy,		// Toxic Spikes
		perfect_accuracy,		// Transform
		100,		// Tri Attack
		100,		// Trick
		perfect_accuracy,		// Trick Room
		90,		// Triple Kick
		perfect_accuracy,		// Trump Card
		100,		// Twineedle
		100,		// Twister
		100,		// U-turn
		100,		// Uproar
		100,		// Vacuum Wave
		100,		// ViceGrip
		100,		// Vine Whip
		perfect_accuracy,		// Vital Throw
		100,		// Volt Tackle
		100,		// Wake-Up Slap
		100,		// Water Gun
		100,		// Water Pulse
		perfect_accuracy,		// Water Sport
		100,		// Water Spout
		100,		// Waterfall
		100,		// Weather Ball
		70,		// Whirlpool
		100,		// Whirlwind
		75,		// Will-O-Wisp
		100,		// Wing Attack
		perfect_accuracy,		// Wish
		perfect_accuracy,		// Withdraw
		100,		// Wood Hammer
		100,		// Worry Seed
		85,		// Wrap
		100,		// Wring Out
		100,		// X-Scissor
		perfect_accuracy,		// Yawn
		50,		// Zap Cannon
		90		// Zen Headbutt
	};
	return get_accuracy [static_cast<unsigned>(move)];
}

}	// unnamed namespace

}	// namespace technicalmachine
