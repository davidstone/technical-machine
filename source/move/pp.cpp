// Copyright (C) 2015 David Stone
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

#include "moves.hpp"

#include "../ability.hpp"

#include <bounded/optional.hpp>

#include <containers/array/make_array.hpp>

#include <algorithm>
#include <cassert>
#include <type_traits>

namespace technicalmachine {
namespace {

auto base_pp(Moves const move) {
	// This has to use a bounded::array instead of a switch statement to take
	// advantage of type deduction
	using bounded::none;
	static constexpr auto base = containers::make_array(
		none,		// Switch0
		none,		// Switch1
		none,		// Switch2
		none,		// Switch3
		none,		// Switch4
		none,		// Switch5
		none,		// Hit_Self
		35_bi,		// Pound
		25_bi,		// Karate Chop
		10_bi,		// DoubleSlap
		15_bi,		// Comet Punch
		20_bi,		// Mega Punch
		20_bi,		// Pay Day
		15_bi,		// Fire Punch
		15_bi,		// Ice Punch
		15_bi,		// ThunderPunch
		35_bi,		// Scratch
		30_bi,		// ViceGrip
		5_bi,		// Guillotine
		10_bi,		// Razor Wind
		30_bi,		// Swords Dance
		30_bi,		// Cut
		35_bi,		// Gust
		35_bi,		// Wing Attack
		20_bi,		// Whirlwind
		15_bi,		// Fly
		20_bi,		// Bind
		20_bi,		// Slam
		15_bi,		// Vine Whip
		20_bi,		// Stomp
		30_bi,		// Double Kick
		5_bi,		// Mega Kick
		10_bi,		// Jump Kick
		15_bi,		// Rolling Kick
		15_bi,		// Sand-Attack
		15_bi,		// Headbutt
		25_bi,		// Horn Attack
		20_bi,		// Fury Attack
		5_bi,		// Horn Drill
		35_bi,		// Tackle
		15_bi,		// Body Slam
		20_bi,		// Wrap
		20_bi,		// Take Down
		10_bi,		// Thrash
		15_bi,		// Double-Edge
		30_bi,		// Tail Whip
		35_bi,		// Poison Sting
		20_bi,		// Twineedle
		20_bi,		// Pin Missile
		30_bi,		// Leer
		25_bi,		// Bite
		40_bi,		// Growl
		20_bi,		// Roar
		15_bi,		// Sing
		20_bi,		// Supersonic
		20_bi,		// SonicBoom
		20_bi,		// Disable
		30_bi,		// Acid
		25_bi,		// Ember
		15_bi,		// Flamethrower
		30_bi,		// Mist
		25_bi,		// Water Gun
		5_bi,		// Hydro Pump
		15_bi,		// Surf
		10_bi,		// Ice Beam
		5_bi,		// Blizzard
		20_bi,		// Psybeam
		20_bi,		// BubbleBeam
		20_bi,		// Aurora Beam
		5_bi,		// Hyper Beam
		35_bi,		// Peck
		20_bi,		// Drill Peck
		25_bi,		// Submission
		20_bi,		// Low Kick
		20_bi,		// Counter
		20_bi,		// Seismic Toss
		15_bi,		// Strength
		25_bi,		// Absorb
		15_bi,		// Mega Drain
		10_bi,		// Leech Seed
		40_bi,		// Growth
		25_bi,		// Razor Leaf
		10_bi,		// SolarBeam
		35_bi,		// PoisonPowder
		30_bi,		// Stun Spore
		15_bi,		// Sleep Powder
		10_bi,		// Petal Dance
		40_bi,		// String Shot
		10_bi,		// Dragon Rage
		15_bi,		// Fire Spin
		30_bi,		// ThunderShock
		15_bi,		// Thunderbolt
		20_bi,		// Thunder Wave
		10_bi,		// Thunder
		15_bi,		// Rock Throw
		10_bi,		// Earthquake
		5_bi,		// Fissure
		10_bi,		// Dig
		10_bi,		// Toxic
		25_bi,		// Confusion
		10_bi,		// Psychic
		20_bi,		// Hypnosis
		40_bi,		// Meditate
		30_bi,		// Agility
		30_bi,		// Quick Attack
		20_bi,		// Rage
		20_bi,		// Teleport
		15_bi,		// Night Shade
		10_bi,		// Mimic
		40_bi,		// Screech
		15_bi,		// Double Team
		10_bi,		// Recover
		30_bi,		// Harden
		20_bi,		// Minimize
		20_bi,		// SmokeScreen
		10_bi,		// Confuse Ray
		40_bi,		// Withdraw
		40_bi,		// Defense Curl
		30_bi,		// Barrier
		30_bi,		// Light Screen
		30_bi,		// Haze
		20_bi,		// Reflect
		30_bi,		// Focus Energy
		10_bi,		// Bide
		10_bi,		// Metronome
		20_bi,		// Mirror Move
		5_bi,		// Selfdestruct
		10_bi,		// Egg Bomb
		30_bi,		// Lick
		20_bi,		// Smog
		20_bi,		// Sludge
		20_bi,		// Bone Club
		5_bi,		// Fire Blast
		15_bi,		// Waterfall
		15_bi,		// Clamp
		20_bi,		// Swift
		15_bi,		// Skull Bash
		15_bi,		// Spike Cannon
		35_bi,		// Constrict
		20_bi,		// Amnesia
		15_bi,		// Kinesis
		10_bi,		// Softboiled
		10_bi,		// Hi Jump Kick
		30_bi,		// Glare
		15_bi,		// Dream Eater
		40_bi,		// Poison Gas
		20_bi,		// Barrage
		15_bi,		// Leech Life
		10_bi,		// Lovely Kiss
		5_bi,		// Sky Attack
		10_bi,		// Transform
		30_bi,		// Bubble
		10_bi,		// Dizzy Punch
		15_bi,		// Spore
		20_bi,		// Flash
		15_bi,		// Psywave
		40_bi,		// Splash
		40_bi,		// Acid Armor
		10_bi,		// Crabhammer
		5_bi,		// Explosion
		15_bi,		// Fury Swipes
		10_bi,		// Bonemerang
		10_bi,		// Rest
		10_bi,		// Rock Slide
		15_bi,		// Hyper Fang
		30_bi,		// Sharpen
		30_bi,		// Conversion
		10_bi,		// Tri Attack
		10_bi,		// Super Fang
		20_bi,		// Slash
		10_bi,		// Substitute
		none,		// Struggle
		1_bi,		// Sketch
		10_bi,		// Triple Kick
		10_bi,		// Thief
		10_bi,		// Spider Web
		5_bi,		// Mind Reader
		15_bi,		// Nightmare
		25_bi,		// Flame Wheel
		15_bi,		// Snore
		10_bi,		// Curse
		15_bi,		// Flail
		30_bi,		// Conversion 2
		5_bi,		// Aeroblast
		40_bi,		// Cotton Spore
		15_bi,		// Reversal
		10_bi,		// Spite
		25_bi,		// Powder Snow
		10_bi,		// Protect
		30_bi,		// Mach Punch
		10_bi,		// Scary Face
		20_bi,		// Faint Attack
		10_bi,		// Sweet Kiss
		10_bi,		// Belly Drum
		10_bi,		// Sludge Bomb
		10_bi,		// Mud-Slap
		10_bi,		// Octazooka
		20_bi,		// Spikes
		5_bi,		// Zap Cannon
		40_bi,		// Foresight
		5_bi,		// Destiny Bond
		5_bi,		// Perish Song
		15_bi,		// Icy Wind
		5_bi,		// Detect
		10_bi,		// Bone Rush
		5_bi,		// Lock-On
		10_bi,		// Outrage
		10_bi,		// Sandstorm
		10_bi,		// Giga Drain
		10_bi,		// Endure
		20_bi,		// Charm
		20_bi,		// Rollout
		40_bi,		// False Swipe
		15_bi,		// Swagger
		10_bi,		// Milk Drink
		20_bi,		// Spark
		20_bi,		// Fury Cutter
		25_bi,		// Steel Wing
		5_bi,		// Mean Look
		15_bi,		// Attract
		10_bi,		// Sleep Talk
		5_bi,		// Heal Bell
		20_bi,		// Return
		15_bi,		// Present
		20_bi,		// Frustration
		25_bi,		// Safeguard
		20_bi,		// Pain Split
		5_bi,		// Sacred Fire
		30_bi,		// Magnitude
		5_bi,		// DynamicPunch
		10_bi,		// Megahorn
		20_bi,		// DragonBreath
		40_bi,		// Baton Pass
		5_bi,		// Encore
		20_bi,		// Pursuit
		40_bi,		// Rapid Spin
		20_bi,		// Sweet Scent
		15_bi,		// Iron Tail
		35_bi,		// Metal Claw
		10_bi,		// Vital Throw
		5_bi,		// Morning Sun
		5_bi,		// Synthesis
		5_bi,		// Moonlight
		15_bi,		// Hidden Power
		5_bi,		// Cross Chop
		20_bi,		// Twister
		5_bi,		// Rain Dance
		5_bi,		// Sunny Day
		15_bi,		// Crunch
		20_bi,		// Mirror Coat
		10_bi,		// Psych Up
		5_bi,		// ExtremeSpeed
		5_bi,		// AncientPower
		15_bi,		// Shadow Ball
		10_bi,		// Future Sight
		15_bi,		// Rock Smash
		15_bi,		// Whirlpool
		10_bi,		// Beat Up
		10_bi,		// Fake Out
		10_bi,		// Uproar
		20_bi,		// Stockpile
		10_bi,		// Spit Up
		10_bi,		// Swallow
		10_bi,		// Heat Wave
		10_bi,		// Hail
		15_bi,		// Torment
		15_bi,		// Flatter
		15_bi,		// Will-O-Wisp
		10_bi,		// Memento
		20_bi,		// Facade
		20_bi,		// Focus Punch
		10_bi,		// SmellingSalt
		20_bi,		// Follow Me
		20_bi,		// Nature Power
		20_bi,		// Charge
		20_bi,		// Taunt
		20_bi,		// Helping Hand
		10_bi,		// Trick
		10_bi,		// Role Play
		10_bi,		// Wish
		20_bi,		// Assist
		20_bi,		// Ingrain
		5_bi,		// Superpower
		15_bi,		// Magic Coat
		10_bi,		// Recycle
		10_bi,		// Revenge
		15_bi,		// Brick Break
		10_bi,		// Yawn
		20_bi,		// Knock Off
		5_bi,		// Endeavor
		5_bi,		// Eruption
		10_bi,		// Skill Swap
		10_bi,		// Imprison
		20_bi,		// Refresh
		5_bi,		// Grudge
		10_bi,		// Snatch
		20_bi,		// Secret Power
		10_bi,		// Dive
		20_bi,		// Arm Thrust
		20_bi,		// Camouflage
		20_bi,		// Tail Glow
		5_bi,		// Luster Purge
		5_bi,		// Mist Ball
		15_bi,		// FeatherDance
		20_bi,		// Teeter Dance
		10_bi,		// Blaze Kick
		15_bi,		// Mud Sport
		20_bi,		// Ice Ball
		15_bi,		// Needle Arm
		10_bi,		// Slack Off
		10_bi,		// Hyper Voice
		15_bi,		// Poison Fang
		10_bi,		// Crush Claw
		5_bi,		// Blast Burn
		5_bi,		// Hydro Cannon
		10_bi,		// Meteor Mash
		15_bi,		// Astonish
		10_bi,		// Weather Ball
		5_bi,		// Aromatherapy
		20_bi,		// Fake Tears
		25_bi,		// Air Cutter
		5_bi,		// Overheat
		40_bi,		// Odor Sleuth
		10_bi,		// Rock Tomb
		5_bi,		// Silver Wind
		40_bi,		// Metal Sound
		15_bi,		// GrassWhistle
		20_bi,		// Tickle
		20_bi,		// Cosmic Power
		5_bi,		// Water Spout
		15_bi,		// Signal Beam
		20_bi,		// Shadow Punch
		30_bi,		// Extrasensory
		15_bi,		// Sky Uppercut
		15_bi,		// Sand Tomb
		5_bi,		// Sheer Cold
		10_bi,		// Muddy Water
		30_bi,		// Bullet Seed
		20_bi,		// Aerial Ace
		30_bi,		// Icicle Spear
		15_bi,		// Iron Defense
		5_bi,		// Block
		40_bi,		// Howl
		15_bi,		// Dragon Claw
		5_bi,		// Frenzy Plant
		20_bi,		// Bulk Up
		5_bi,		// Bounce
		15_bi,		// Mud Shot
		25_bi,		// Poison Tail
		40_bi,		// Covet
		15_bi,		// Volt Tackle
		20_bi,		// Magical Leaf
		15_bi,		// Water Sport
		20_bi,		// Calm Mind
		15_bi,		// Leaf Blade
		20_bi,		// Dragon Dance
		10_bi,		// Rock Blast
		20_bi,		// Shock Wave
		20_bi,		// Water Pulse
		5_bi,		// Doom Desire
		5_bi,		// Psycho Boost
		10_bi,		// Roost
		5_bi,		// Gravity
		40_bi,		// Miracle Eye
		10_bi,		// Wake-Up Slap
		10_bi,		// Hammer Arm
		5_bi,		// Gyro Ball
		10_bi,		// Healing Wish
		10_bi,		// Brine
		15_bi,		// Natural Gift
		10_bi,		// Feint
		20_bi,		// Pluck
		30_bi,		// Tailwind
		30_bi,		// Acupressure
		10_bi,		// Metal Burst
		20_bi,		// U-turn
		5_bi,		// Close Combat
		10_bi,		// Payback
		10_bi,		// Assurance
		15_bi,		// Embargo
		10_bi,		// Fling
		10_bi,		// Psycho Shift
		5_bi,		// Trump Card
		15_bi,		// Heal Block
		5_bi,		// Wring Out
		10_bi,		// Power Trick
		10_bi,		// Gastro Acid
		30_bi,		// Lucky Chant
		20_bi,		// Me First
		20_bi,		// Copycat
		10_bi,		// Power Swap
		10_bi,		// Guard Swap
		5_bi,		// Punishment
		5_bi,		// Last Resort
		10_bi,		// Worry Seed
		5_bi,		// Sucker Punch
		20_bi,		// Toxic Spikes
		10_bi,		// Heart Swap
		20_bi,		// Aqua Ring
		10_bi,		// Magnet Rise
		15_bi,		// Flare Blitz
		10_bi,		// Force Palm
		20_bi,		// Aura Sphere
		20_bi,		// Rock Polish
		20_bi,		// Poison Jab
		15_bi,		// Dark Pulse
		15_bi,		// Night Slash
		10_bi,		// Aqua Tail
		15_bi,		// Seed Bomb
		20_bi,		// Air Slash
		15_bi,		// X-Scissor
		10_bi,		// Bug Buzz
		10_bi,		// Dragon Pulse
		10_bi,		// Dragon Rush
		20_bi,		// Power Gem
		10_bi,		// Drain Punch
		30_bi,		// Vacuum Wave
		5_bi,		// Focus Blast
		10_bi,		// Energy Ball
		15_bi,		// Brave Bird
		10_bi,		// Earth Power
		10_bi,		// Switcheroo
		5_bi,		// Giga Impact
		20_bi,		// Nasty Plot
		30_bi,		// Bullet Punch
		10_bi,		// Avalanche
		30_bi,		// Ice Shard
		15_bi,		// Shadow Claw
		15_bi,		// Thunder Fang
		15_bi,		// Ice Fang
		15_bi,		// Fire Fang
		30_bi,		// Shadow Sneak
		10_bi,		// Mud Bomb
		20_bi,		// Psycho Cut
		15_bi,		// Zen Headbutt
		10_bi,		// Mirror Shot
		10_bi,		// Flash Cannon
		20_bi,		// Rock Climb
		15_bi,		// Defog
		5_bi,		// Trick Room
		5_bi,		// Draco Meteor
		15_bi,		// Discharge
		15_bi,		// Lava Plume
		5_bi,		// Leaf Storm
		10_bi,		// Power Whip
		5_bi,		// Rock Wrecker
		20_bi,		// Cross Poison
		5_bi,		// Gunk Shot
		15_bi,		// Iron Head
		20_bi,		// Magnet Bomb
		5_bi,		// Stone Edge
		20_bi,		// Captivate
		20_bi,		// Stealth Rock
		20_bi,		// Grass Knot
		20_bi,		// Chatter
		10_bi,		// Judgment
		20_bi,		// Bug Bite
		10_bi,		// Charge Beam
		15_bi,		// Wood Hammer
		20_bi,		// Aqua Jet
		15_bi,		// Attack Order
		10_bi,		// Defend Order
		10_bi,		// Heal Order
		5_bi,		// Head Smash
		10_bi,		// Double Hit
		5_bi,		// Roar of Time
		5_bi,		// Spacial Rend
		10_bi,		// Lunar Dance
		5_bi,		// Crush Grip
		5_bi,		// Magma Storm
		10_bi,		// Dark Void
		5_bi,		// Seed Flare
		5_bi,		// Ominous Wind
		5_bi,		// Shadow Force
		15_bi,		// Hone Claws
		10_bi,		// Wide Guard
		10_bi,		// Guard Split
		10_bi,		// Power Split
		10_bi,		// Wonder Room
		10_bi,		// Psyshock
		10_bi,		// Venoshock
		15_bi,		// Autotomize
		20_bi,		// Rage Powder
		15_bi,		// Telekinesis
		10_bi,		// Magic Room
		15_bi,		// Smack Down
		10_bi,		// Storm Throw
		15_bi,		// Flame Burst
		10_bi,		// Sludge Wave
		20_bi,		// Quiver Dance
		10_bi,		// Heavy Slam
		15_bi,		// Synchronoise
		10_bi,		// Electro Ball
		20_bi,		// Soak
		20_bi,		// Flame Charge
		20_bi,		// Coil
		20_bi,		// Low Sweep
		20_bi,		// Acid Spray
		15_bi,		// Foul Play
		15_bi,		// Simple Beam
		15_bi,		// Entrainment
		15_bi,		// After You
		15_bi,		// Round
		15_bi,		// Echoed Voice
		20_bi,		// Chip Away
		15_bi,		// Clear Smog
		10_bi,		// Stored Power
		15_bi,		// Quick Guard
		15_bi,		// Ally Switch
		15_bi,		// Scald
		15_bi,		// Shell Smash
		10_bi,		// Heal Pulse
		10_bi,		// Hex
		10_bi,		// Sky Drop
		10_bi,		// Shift Gear
		10_bi,		// Circle Throw
		15_bi,		// Incinerate
		15_bi,		// Quash
		15_bi,		// Acrobatics
		15_bi,		// Reflect Type
		5_bi,		// Retaliate
		5_bi,		// Final Gambit
		15_bi,		// Bestow
		5_bi,		// Inferno
		10_bi,		// Water Pledge
		10_bi,		// Fire Pledge
		10_bi,		// Grass Pledge
		20_bi,		// Volt Switch
		20_bi,		// Struggle Bug
		20_bi,		// Bulldoze
		10_bi,		// Frost Breath
		10_bi,		// Dragon Tail
		30_bi,		// Work Up
		15_bi,		// Electroweb
		15_bi,		// Wild Charge
		10_bi,		// Drill Run
		15_bi,		// Dual Chop
		25_bi,		// Heart Stamp
		10_bi,		// Horn Leech
		20_bi,		// Sacred Sword
		10_bi,		// Razor Shell
		10_bi,		// Heat Crash
		10_bi,		// Leaf Tornado
		20_bi,		// Steamroller
		10_bi,		// Cotton Guard
		10_bi,		// Night Daze
		10_bi,		// Psystrike
		10_bi,		// Tail Slap
		10_bi,		// Hurricane
		15_bi,		// Head Charge
		15_bi,		// Gear Grind
		5_bi,		// Searing Shot
		5_bi,		// Techno Blast
		10_bi,		// Relic Song
		10_bi,		// Secret Sword
		10_bi,		// Glaciate
		5_bi,		// Bolt Strike
		5_bi,		// Blue Flare
		10_bi,		// Fiery Dance
		5_bi,		// Freeze Shock
		5_bi,		// Ice Burn
		15_bi,		// Snarl
		10_bi,		// Icicle Crash
		5_bi,		// V-create
		5_bi,		// Fusion Flare
		5_bi		// Fusion Bolt
	);
	return base[move];
}
using BasePP = decltype(base_pp(std::declval<Moves>()));

auto max(BasePP const base, Pp::pp_ups_type const pp_ups) {
	return static_cast<bool>(base) ? make_optional(*base * (pp_ups + 5_bi) / 5_bi) : bounded::none;
}
using Max = decltype(max(std::declval<BasePP>(), std::declval<Pp::pp_ups_type>()));

}	// namespace

Pp::Pp(Moves const move, pp_ups_type const pp_ups):
	m_max(max(base_pp(move), pp_ups)),
	m_current(static_cast<bool>(m_max) ? bounded::optional<current_type>(*m_max) : bounded::none) {
	static_assert(std::is_same<bounded::optional<base_type>, BasePP>::value, "Incorrect PP type.");
	static_assert(std::is_same<decltype(m_max), Max>::value, "Incorrect PP type.");
}

auto Pp::is_empty() const -> bool {
	return m_current == 0_bi;
}

auto Pp::has_unlimited_pp() const -> bool {
	return static_cast<bool>(m_max);
}

auto Pp::decrement(Ability const & foe_ability) -> void {
	if (has_unlimited_pp())
		return;
	// I think it is always an error to try to decrement a move without PP.
	assert(m_current != 0_bi);
	*m_current -= BOUNDED_CONDITIONAL(foe_ability.uses_extra_pp(), 2_bi, 1_bi);
}

auto Pp::trump_card_power() const -> bounded::integer<40, 200> {
	// Should be safe because we assume we are using Trump Card
	switch (m_current->value()) {
		case 0:
			return 200_bi;
		case 1:
			return 80_bi;
		case 2:
			return 60_bi;
		case 3:
			return 50_bi;
		default:
			return 40_bi;
	}
}

auto operator== (Pp const & lhs, Pp const & rhs) -> bool {
	return lhs.m_current == rhs.m_current;
}

}	// namespace technicalmachine
