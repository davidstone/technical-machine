// Type function definitions
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

#include "type.hpp"

#include "effectiveness.hpp"
#include "../weather.hpp"
#include "../move/moves.hpp"
#include "../pokemon/pokemon.hpp"

#include <bounded_integer/array.hpp>

#include <algorithm>

namespace technicalmachine {

auto is_strengthened_by_weather(Type const type, Weather const & weather) -> bool {
	return (weather.rain() and type == Type::Water) or (weather.sun() and type == Type::Fire);
}

auto is_weakened_by_weather(Type const type, Weather const & weather) -> bool {
	return (weather.rain() and type == Type::Fire) or (weather.sun() and type == Type::Water);
}

namespace {

auto hidden_power_type(Pokemon const & pokemon) {
	using modifier_type = std::pair<StatNames, bounded::integer<1, 5>>;
	static constexpr auto modifiers = bounded::make_array(
		modifier_type(StatNames::ATK, 1_bi),
		modifier_type(StatNames::DEF, 2_bi),
		modifier_type(StatNames::SPE, 3_bi),
		modifier_type(StatNames::SPA, 4_bi),
		modifier_type(StatNames::SPD, 5_bi)
	);
	using intermediate_type = bounded::checked_integer<0, 63>;
	auto const sum = [&](intermediate_type const value, modifier_type const & pair) {
		return value + ((get_stat(pokemon, pair.first).iv.value() % 2_bi) << pair.second);
	};
	intermediate_type const initial = get_hp(pokemon).iv.value() % 2_bi;
	auto const index = std::accumulate(std::begin(modifiers), std::end(modifiers), initial, sum) * 15_bi / 63_bi;
	constexpr static auto lookup = bounded::make_array(
		Type::Fighting,
		Type::Flying,
		Type::Poison,
		Type::Ground,
		Type::Rock,
		Type::Bug,
		Type::Ghost,
		Type::Steel,
		Type::Fire,
		Type::Water,
		Type::Grass,
		Type::Electric,
		Type::Psychic,
		Type::Ice,
		Type::Dragon,
		Type::Dark
	);
	static_assert(std::numeric_limits<decltype(index)>::min() == 0_bi, "Incorrect minimum index.");
	static_assert(std::numeric_limits<decltype(index)>::max() == lookup.size() - 1_bi, "Incorrect maximum index.");
	return lookup[index];
}

}	// namespace

auto get_type(Moves const move, Pokemon const & pokemon) -> Type {
	static constexpr auto move_type = bounded::make_array(
		Type::Typeless,		// Switch0
		Type::Typeless,		// Switch1
		Type::Typeless,		// Switch2
		Type::Typeless,		// Switch3
		Type::Typeless,		// Switch4
		Type::Typeless,		// Switch5
		Type::Typeless,		// Hit Self
		Type::Normal,		// Pound
		Type::Fighting,		// Karate Chop
		Type::Normal,		// DoubleSlap
		Type::Normal,		// Comet Punch
		Type::Normal,		// Mega Punch
		Type::Normal,		// Pay Day
		Type::Fire,		// Fire Punch
		Type::Ice,		// Ice Punch
		Type::Electric,		// ThunderPunch
		Type::Normal,		// Scratch
		Type::Normal,		// ViceGrip
		Type::Normal,		// Guillotine
		Type::Normal,		// Razor Wind
		Type::Normal,		// Swords Dance
		Type::Normal,		// Cut
		Type::Flying,		// Gust
		Type::Flying,		// Wing Attack
		Type::Normal,		// Whirlwind
		Type::Flying,		// Fly
		Type::Normal,		// Bind
		Type::Normal,		// Slam
		Type::Grass,		// Vine Whip
		Type::Normal,		// Stomp
		Type::Fighting,		// Double Kick
		Type::Normal,		// Mega Kick
		Type::Fighting,		// Jump Kick
		Type::Fighting,		// Rolling Kick
		Type::Ground,		// Sand-Attack
		Type::Normal,		// Headbutt
		Type::Normal,		// Horn Attack
		Type::Normal,		// Fury Attack
		Type::Normal,		// Horn Drill
		Type::Normal,		// Tackle
		Type::Normal,		// Body Slam
		Type::Normal,		// Wrap
		Type::Normal,		// Take Down
		Type::Normal,		// Thrash
		Type::Normal,		// Double-Edge
		Type::Normal,		// Tail Whip
		Type::Poison,		// Poison Sting
		Type::Bug,		// Twineedle
		Type::Bug,		// Pin Missile
		Type::Normal,		// Leer
		Type::Dark,		// Bite
		Type::Normal,		// Growl
		Type::Normal,		// Roar
		Type::Normal,		// Sing
		Type::Normal,		// Supersonic
		Type::Normal,		// SonicBoom
		Type::Normal,		// Disable
		Type::Poison,		// Acid
		Type::Fire,		// Ember
		Type::Fire,		// Flamethrower
		Type::Ice,		// Mist
		Type::Water,		// Water Gun
		Type::Water,		// Hydro Pump
		Type::Water,		// Surf
		Type::Ice,		// Ice Beam
		Type::Ice,		// Blizzard
		Type::Psychic,		// Psybeam
		Type::Water,		// BubbleBeam
		Type::Ice,		// Aurora Beam
		Type::Normal,		// Hyper Beam
		Type::Flying,		// Peck
		Type::Flying,		// Drill Peck
		Type::Fighting,		// Submission
		Type::Fighting,		// Low Kick
		Type::Fighting,		// Counter
		Type::Fighting,		// Seismic Toss
		Type::Normal,		// Strength
		Type::Grass,		// Absorb
		Type::Grass,		// Mega Drain
		Type::Grass,		// Leech Seed
		Type::Normal,		// Growth
		Type::Grass,		// Razor Leaf
		Type::Grass,		// SolarBeam
		Type::Poison,		// PoisonPowder
		Type::Grass,		// Stun Spore
		Type::Grass,		// Sleep Powder
		Type::Grass,		// Petal Dance
		Type::Bug,		// String Shot
		Type::Dragon,		// Dragon Rage
		Type::Fire,		// Fire Spin
		Type::Electric,		// ThunderShock
		Type::Electric,		// Thunderbolt
		Type::Electric,		// Thunder Wave
		Type::Electric,		// Thunder
		Type::Rock,		// Rock Throw
		Type::Ground,		// Earthquake
		Type::Ground,		// Fissure
		Type::Ground,		// Dig
		Type::Poison,		// Toxic
		Type::Psychic,		// Confusion
		Type::Psychic,		// Psychic
		Type::Psychic,		// Hypnosis
		Type::Psychic,		// Meditate
		Type::Psychic,		// Agility
		Type::Normal,		// Quick Attack
		Type::Normal,		// Rage
		Type::Psychic,		// Teleport
		Type::Ghost,		// Night Shade
		Type::Normal,		// Mimic
		Type::Normal,		// Screech
		Type::Normal,		// Double Team
		Type::Normal,		// Recover
		Type::Normal,		// Harden
		Type::Normal,		// Minimize
		Type::Normal,		// SmokeScreen
		Type::Ghost,		// Confuse Ray
		Type::Water,		// Withdraw
		Type::Normal,		// Defense Curl
		Type::Psychic,		// Barrier
		Type::Psychic,		// Light Screen
		Type::Ice,		// Haze
		Type::Psychic,		// Reflect
		Type::Normal,		// Focus Energy
		Type::Normal,		// Bide
		Type::Normal,		// Metronome
		Type::Flying,		// Mirror Move
		Type::Normal,		// Selfdestruct
		Type::Normal,		// Egg Bomb
		Type::Ghost,		// Lick
		Type::Poison,		// Smog
		Type::Poison,		// Sludge
		Type::Ground,		// Bone Club
		Type::Fire,		// Fire Blast
		Type::Water,		// Waterfall
		Type::Water,		// Clamp
		Type::Normal,		// Swift
		Type::Normal,		// Skull Bash
		Type::Normal,		// Spike Cannon
		Type::Normal,		// Constrict
		Type::Psychic,		// Amnesia
		Type::Psychic,		// Kinesis
		Type::Normal,		// Softboiled
		Type::Fighting,		// Hi Jump Kick
		Type::Normal,		// Glare
		Type::Psychic,		// Dream Eater
		Type::Poison,		// Poison Gas
		Type::Normal,		// Barrage
		Type::Bug,		// Leech Life
		Type::Normal,		// Lovely Kiss
		Type::Flying,		// Sky Attack
		Type::Normal,		// Transform
		Type::Water,		// Bubble
		Type::Normal,		// Dizzy Punch
		Type::Grass,		// Spore
		Type::Normal,		// Flash
		Type::Psychic,		// Psywave
		Type::Normal,		// Splash
		Type::Poison,		// Acid Armor
		Type::Water,		// Crabhammer
		Type::Normal,		// Explosion
		Type::Normal,		// Fury Swipes
		Type::Ground,		// Bonemerang
		Type::Psychic,		// Rest
		Type::Rock,		// Rock Slide
		Type::Normal,		// Hyper Fang
		Type::Normal,		// Sharpen
		Type::Normal,		// Conversion
		Type::Normal,		// Tri Attack
		Type::Normal,		// Super Fang
		Type::Normal,		// Slash
		Type::Normal,		// Substitute
		Type::Typeless,		// Struggle
		Type::Normal,		// Sketch
		Type::Fighting,		// Triple Kick
		Type::Dark,		// Thief
		Type::Bug,		// Spider Web
		Type::Normal,		// Mind Reader
		Type::Ghost,		// Nightmare
		Type::Fire,		// Flame Wheel
		Type::Normal,		// Snore
		Type::Ghost,		// Curse
		Type::Normal,		// Flail
		Type::Normal,		// Conversion 2
		Type::Flying,		// Aeroblast
		Type::Grass,		// Cotton Spore
		Type::Fighting,		// Reversal
		Type::Ghost,		// Spite
		Type::Ice,		// Powder Snow
		Type::Normal,		// Protect
		Type::Fighting,		// Mach Punch
		Type::Normal,		// Scary Face
		Type::Dark,		// Faint Attack
		Type::Normal,		// Sweet Kiss
		Type::Normal,		// Belly Drum
		Type::Poison,		// Sludge Bomb
		Type::Ground,		// Mud-Slap
		Type::Water,		// Octazooka
		Type::Ground,		// Spikes
		Type::Electric,		// Zap Cannon
		Type::Normal,		// Foresight
		Type::Ghost,		// Destiny Bond
		Type::Normal,		// Perish Song
		Type::Ice,		// Icy Wind
		Type::Fighting,		// Detect
		Type::Ground,		// Bone Rush
		Type::Normal,		// Lock-On
		Type::Dragon,		// Outrage
		Type::Rock,		// Sandstorm
		Type::Grass,		// Giga Drain
		Type::Normal,		// Endure
		Type::Normal,		// Charm
		Type::Rock,		// Rollout
		Type::Normal,		// False Swipe
		Type::Normal,		// Swagger
		Type::Normal,		// Milk Drink
		Type::Electric,		// Spark
		Type::Bug,		// Fury Cutter
		Type::Steel,		// Steel Wing
		Type::Normal,		// Mean Look
		Type::Normal,		// Attract
		Type::Normal,		// Sleep Talk
		Type::Normal,		// Heal Bell
		Type::Normal,		// Return
		Type::Normal,		// Present
		Type::Normal,		// Frustration
		Type::Normal,		// Safeguard
		Type::Normal,		// Pain Split
		Type::Fire,		// Sacred Fire
		Type::Ground,		// Magnitude
		Type::Fighting,		// DynamicPunch
		Type::Bug,		// Megahorn
		Type::Dragon,		// DragonBreath
		Type::Normal,		// Baton Pass
		Type::Normal,		// Encore
		Type::Dark,		// Pursuit
		Type::Normal,		// Rapid Spin
		Type::Normal,		// Sweet Scent
		Type::Steel,		// Iron Tail
		Type::Steel,		// Metal Claw
		Type::Fighting,		// Vital Throw
		Type::Normal,		// Morning Sun
		Type::Grass,		// Synthesis
		Type::Normal,		// Moonlight
		Type::Normal,		// Hidden Power
		Type::Fighting,		// Cross Chop
		Type::Dragon,		// Twister
		Type::Water,		// Rain Dance
		Type::Fire,		// Sunny Day
		Type::Dark,		// Crunch
		Type::Psychic,		// Mirror Coat
		Type::Normal,		// Psych Up
		Type::Normal,		// ExtremeSpeed
		Type::Rock,		// AncientPower
		Type::Ghost,		// Shadow Ball
		Type::Psychic,		// Future Sight
		Type::Fighting,		// Rock Smash
		Type::Water,		// Whirlpool
		Type::Dark,		// Beat Up
		Type::Normal,		// Fake Out
		Type::Normal,		// Uproar
		Type::Normal,		// Stockpile
		Type::Normal,		// Spit Up
		Type::Normal,		// Swallow
		Type::Fire,		// Heat Wave
		Type::Ice,		// Hail
		Type::Dark,		// Torment
		Type::Dark,		// Flatter
		Type::Fire,		// Will-O-Wisp
		Type::Dark,		// Memento
		Type::Normal,		// Facade
		Type::Fighting,		// Focus Punch
		Type::Normal,		// SmellingSalt
		Type::Normal,		// Follow Me
		Type::Normal,		// Nature Power
		Type::Electric,		// Charge
		Type::Dark,		// Taunt
		Type::Normal,		// Helping Hand
		Type::Psychic,		// Trick
		Type::Psychic,		// Role Play
		Type::Normal,		// Wish
		Type::Normal,		// Assist
		Type::Grass,		// Ingrain
		Type::Fighting,		// Superpower
		Type::Psychic,		// Magic Coat
		Type::Normal,		// Recycle
		Type::Fighting,		// Revenge
		Type::Fighting,		// Brick Break
		Type::Normal,		// Yawn
		Type::Dark,		// Knock Off
		Type::Normal,		// Endeavor
		Type::Fire,		// Eruption
		Type::Psychic,		// Skill Swap
		Type::Psychic,		// Imprison
		Type::Normal,		// Refresh
		Type::Ghost,		// Grudge
		Type::Dark,		// Snatch
		Type::Normal,		// Secret Power
		Type::Water,		// Dive
		Type::Fighting,		// Arm Thrust
		Type::Normal,		// Camouflage
		Type::Bug,		// Tail Glow
		Type::Psychic,		// Luster Purge
		Type::Psychic,		// Mist Ball
		Type::Flying,		// FeatherDance
		Type::Normal,		// Teeter Dance
		Type::Fire,		// Blaze Kick
		Type::Ground,		// Mud Sport
		Type::Ice,		// Ice Ball
		Type::Grass,		// Needle Arm
		Type::Normal,		// Slack Off
		Type::Normal,		// Hyper Voice
		Type::Poison,		// Poison Fang
		Type::Normal,		// Crush Claw
		Type::Fire,		// Blast Burn
		Type::Water,		// Hydro Cannon
		Type::Steel,		// Meteor Mash
		Type::Ghost,		// Astonish
		Type::Normal,		// Weather Ball
		Type::Grass,		// Aromatherapy
		Type::Dark,		// Fake Tears
		Type::Flying,		// Air Cutter
		Type::Fire,		// Overheat
		Type::Normal,		// Odor Sleuth
		Type::Rock,		// Rock Tomb
		Type::Bug,		// Silver Wind
		Type::Steel,		// Metal Sound
		Type::Grass,		// GrassWhistle
		Type::Normal,		// Tickle
		Type::Psychic,		// Cosmic Power
		Type::Water,		// Water Spout
		Type::Bug,		// Signal Beam
		Type::Ghost,		// Shadow Punch
		Type::Psychic,		// Extrasensory
		Type::Fighting,		// Sky Uppercut
		Type::Ground,		// Sand Tomb
		Type::Ice,		// Sheer Cold
		Type::Water,		// Muddy Water
		Type::Grass,		// Bullet Seed
		Type::Flying,		// Aerial Ace
		Type::Ice,		// Icicle Spear
		Type::Steel,		// Iron Defense
		Type::Normal,		// Block
		Type::Normal,		// Howl
		Type::Dragon,		// Dragon Claw
		Type::Grass,		// Frenzy Plant
		Type::Fighting,		// Bulk Up
		Type::Flying,		// Bounce
		Type::Ground,		// Mud Shot
		Type::Poison,		// Poison Tail
		Type::Normal,		// Covet
		Type::Electric,		// Volt Tackle
		Type::Grass,		// Magical Leaf
		Type::Water,		// Water Sport
		Type::Psychic,		// Calm Mind
		Type::Grass,		// Leaf Blade
		Type::Dragon,		// Dragon Dance
		Type::Rock,		// Rock Blast
		Type::Electric,		// Shock Wave
		Type::Water,		// Water Pulse
		Type::Steel,		// Doom Desire
		Type::Psychic,		// Psycho Boost
		Type::Flying,		// Roost
		Type::Psychic,		// Gravity
		Type::Psychic,		// Miracle Eye
		Type::Fighting,		// Wake-Up Slap
		Type::Fighting,		// Hammer Arm
		Type::Steel,		// Gyro Ball
		Type::Psychic,		// Healing Wish
		Type::Water,		// Brine
		Type::Normal,		// Natural Gift
		Type::Normal,		// Feint
		Type::Flying,		// Pluck
		Type::Flying,		// Tailwind
		Type::Normal,		// Acupressure
		Type::Steel,		// Metal Burst
		Type::Bug,		// U-turn
		Type::Fighting,		// Close Combat
		Type::Dark,		// Payback
		Type::Dark,		// Assurance
		Type::Dark,		// Embargo
		Type::Dark,		// Fling
		Type::Psychic,		// Psycho Shift
		Type::Normal,		// Trump Card
		Type::Psychic,		// Heal Block
		Type::Normal,		// Wring Out
		Type::Psychic,		// Power Trick
		Type::Poison,		// Gastro Acid
		Type::Normal,		// Lucky Chant
		Type::Normal,		// Me First
		Type::Normal,		// Copycat
		Type::Psychic,		// Power Swap
		Type::Psychic,		// Guard Swap
		Type::Dark,		// Punishment
		Type::Normal,		// Last Resort
		Type::Grass,		// Worry Seed
		Type::Dark,		// Sucker Punch
		Type::Poison,		// Toxic Spikes
		Type::Psychic,		// Heart Swap
		Type::Water,		// Aqua Ring
		Type::Electric,		// Magnet Rise
		Type::Fire,		// Flare Blitz
		Type::Fighting,		// Force Palm
		Type::Fighting,		// Aura Sphere
		Type::Rock,		// Rock Polish
		Type::Poison,		// Poison Jab
		Type::Dark,		// Dark Pulse
		Type::Dark,		// Night Slash
		Type::Water,		// Aqua Tail
		Type::Grass,		// Seed Bomb
		Type::Flying,		// Air Slash
		Type::Bug,		// X-Scissor
		Type::Bug,		// Bug Buzz
		Type::Dragon,		// Dragon Pulse
		Type::Dragon,		// Dragon Rush
		Type::Rock,		// Power Gem
		Type::Fighting,		// Drain Punch
		Type::Fighting,		// Vacuum Wave
		Type::Fighting,		// Focus Blast
		Type::Grass,		// Energy Ball
		Type::Flying,		// Brave Bird
		Type::Ground,		// Earth Power
		Type::Dark,		// Switcheroo
		Type::Normal,		// Giga Impact
		Type::Dark,		// Nasty Plot
		Type::Steel,		// Bullet Punch
		Type::Ice,		// Avalanche
		Type::Ice,		// Ice Shard
		Type::Ghost,		// Shadow Claw
		Type::Electric,		// Thunder Fang
		Type::Ice,		// Ice Fang
		Type::Fire,		// Fire Fang
		Type::Ghost,		// Shadow Sneak
		Type::Ground,		// Mud Bomb
		Type::Psychic,		// Psycho Cut
		Type::Psychic,		// Zen Headbutt
		Type::Steel,		// Mirror Shot
		Type::Steel,		// Flash Cannon
		Type::Normal,		// Rock Climb
		Type::Flying,		// Defog
		Type::Psychic,		// Trick Room
		Type::Dragon,		// Draco Meteor
		Type::Electric,		// Discharge
		Type::Fire,		// Lava Plume
		Type::Grass,		// Leaf Storm
		Type::Grass,		// Power Whip
		Type::Rock,		// Rock Wrecker
		Type::Poison,		// Cross Poison
		Type::Poison,		// Gunk Shot
		Type::Steel,		// Iron Head
		Type::Steel,		// Magnet Bomb
		Type::Rock,		// Stone Edge
		Type::Normal,		// Captivate
		Type::Rock,		// Stealth Rock
		Type::Grass,		// Grass Knot
		Type::Flying,		// Chatter
		Type::Normal,		// Judgment
		Type::Bug,		// Bug Bite
		Type::Electric,		// Charge Beam
		Type::Grass,		// Wood Hammer
		Type::Water,		// Aqua Jet
		Type::Bug,		// Attack Order
		Type::Bug,		// Defend Order
		Type::Bug,		// Heal Order
		Type::Rock,		// Head Smash
		Type::Normal,		// Double Hit
		Type::Dragon,		// Roar of Time
		Type::Dragon,		// Spacial Rend
		Type::Psychic,		// Lunar Dance
		Type::Normal,		// Crush Grip
		Type::Fire,		// Magma Storm
		Type::Dark,		// Dark Void
		Type::Grass,		// Seed Flare
		Type::Ghost,		// Ominous Wind
		Type::Ghost,		// Shadow Force
		Type::Dark,		// Hone Claws
		Type::Rock,		// Wide Guard
		Type::Psychic,		// Guard Split
		Type::Psychic,		// Power Split
		Type::Psychic,		// Wonder Room
		Type::Psychic,		// Psyshock
		Type::Poison,		// Venoshock
		Type::Steel,		// Autotomize
		Type::Bug,		// Rage Powder
		Type::Psychic,		// Telekinesis
		Type::Psychic,		// Magic Room
		Type::Rock,		// Smack Down
		Type::Fighting,		// Storm Throw
		Type::Fire,		// Flame Burst
		Type::Poison,		// Sludge Wave
		Type::Bug,		// Quiver Dance
		Type::Steel,		// Heavy Slam
		Type::Psychic,		// Synchronoise
		Type::Electric,		// Electro Ball
		Type::Water,		// Soak
		Type::Fire,		// Flame Charge
		Type::Poison,		// Coil
		Type::Fighting,		// Low Sweep
		Type::Poison,		// Acid Spray
		Type::Dark,		// Foul Play
		Type::Normal,		// Simple Beam
		Type::Normal,		// Entrainment
		Type::Normal,		// After You
		Type::Normal,		// Round
		Type::Normal,		// Echoed Voice
		Type::Normal,		// Chip Away
		Type::Poison,		// Clear Smog
		Type::Psychic,		// Stored Power
		Type::Fighting,		// Quick Guard
		Type::Psychic,		// Ally Switch
		Type::Water,		// Scald
		Type::Normal,		// Shell Smash
		Type::Psychic,		// Heal Pulse
		Type::Ghost,		// Hex
		Type::Flying,		// Sky Drop
		Type::Steel,		// Shift Gear
		Type::Fighting,		// Circle Throw
		Type::Fire,		// Incinerate
		Type::Dark,		// Quash
		Type::Flying,		// Acrobatics
		Type::Normal,		// Reflect Type
		Type::Normal,		// Retaliate
		Type::Fighting,		// Final Gambit
		Type::Normal,		// Bestow
		Type::Fire,		// Inferno
		Type::Water,		// Water Pledge
		Type::Fire,		// Fire Pledge
		Type::Grass,		// Grass Pledge
		Type::Electric,		// Volt Switch
		Type::Bug,		// Struggle Bug
		Type::Ground,		// Bulldoze
		Type::Ice,		// Frost Breath
		Type::Dragon,		// Dragon Tail
		Type::Normal,		// Work Up
		Type::Electric,		// Electroweb
		Type::Electric,		// Wild Charge
		Type::Ground,		// Drill Run
		Type::Dragon,		// Dual Chop
		Type::Psychic,		// Heart Stamp
		Type::Grass,		// Horn Leech
		Type::Fighting,		// Sacred Sword
		Type::Water,		// Razor Shell
		Type::Fire,		// Heat Crash
		Type::Grass,		// Leaf Tornado
		Type::Bug,		// Steamroller
		Type::Grass,		// Cotton Guard
		Type::Dark,		// Night Daze
		Type::Psychic,		// Psystrike
		Type::Normal,		// Tail Slap
		Type::Flying,		// Hurricane
		Type::Normal,		// Head Charge
		Type::Steel,		// Gear Grind
		Type::Fire,		// Searing Shot
		Type::Normal,		// Techno Blast
		Type::Normal,		// Relic Song
		Type::Fighting,		// Secret Sword
		Type::Ice,		// Glaciate
		Type::Electric,		// Bolt Strike
		Type::Fire,		// Blue Flare
		Type::Fire,		// Fiery Dance
		Type::Ice,		// Freeze Shock
		Type::Ice,		// Ice Burn
		Type::Dark,		// Snarl
		Type::Ice,		// Icicle Crash
		Type::Fire,		// V-create
		Type::Fire,		// Fusion Flare
		Type::Electric		// Fusion Bolt
	);
	return move == Moves::Hidden_Power ?
		hidden_power_type(pokemon) :
		move_type.at(move);
}

}	// namespace technicalmachine
