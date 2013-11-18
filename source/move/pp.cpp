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
#include <type_traits>

#include "moves_forward.hpp"

#include "../ability.hpp"

namespace technicalmachine {

Pp::Pp (Moves const move, pp_ups_type const pp_ups):
	max(calculate_max(get_base_pp(move), pp_ups)),
	current(static_cast<bool>(max) ? optional<current_type>(*max) : optional<current_type>{}) {
}

auto Pp::calculate_max(optional<base_type> base, pp_ups_type pp_ups) -> optional<max_type> {
	// Macro needed until C++14 automatic return type deduction
	#define TECHNICALMACHINE_PP_MAX \
		make_optional(*base * (pp_ups + 5_ri) / 5_ri)
	static_assert(std::is_same<optional<max_type>, decltype(TECHNICALMACHINE_PP_MAX)>::value, "Incorrect PP type.");
	return static_cast<bool>(base) ? TECHNICALMACHINE_PP_MAX : decltype(TECHNICALMACHINE_PP_MAX){};
	#undef TECHNICALMACHINE_PP_MAX
}

bool Pp::is_empty() const {
	return current == 0_ri;
}

bool Pp::has_unlimited_pp() const {
	return static_cast<bool>(max);
}

void Pp::decrement(Ability const & foe_ability) {
	if (has_unlimited_pp())
		return;
	// I think it is always an error to try to decrement a move without PP.
	assert(current != 0_ri);
	*current -= ternary_conditional(foe_ability.uses_extra_pp(), 2_ri, 1_ri);
}

native_integer<40, 200> Pp::trump_card_power() const {
	// Should be safe because we assume we are using Trump Card
	switch (current->value()) {
		case 0:
			return 200_ri;
		case 1:
			return 80_ri;
		case 2:
			return 60_ri;
		case 3:
			return 50_ri;
		default:
			return 40_ri;
	}
}

uint64_t Pp::hash() const {
	return static_cast<bool>(current) ? static_cast<uint64_t>(*current) : 0;
}

uint64_t Pp::max_hash() const {
	return static_cast<bool>(max) ? static_cast<uint64_t>(*max + 1_ri) : 1;
}

bool operator== (Pp const & lhs, Pp const & rhs) {
	return lhs.current == rhs.current;
}

bool operator!= (Pp const & lhs, Pp const & rhs) {
	return !(lhs == rhs);
}

auto Pp::get_base_pp(Moves const move) -> optional<base_type> {
	static constexpr optional<base_type> get_pp[] = {
		{},		// Switch0
		{},		// Switch1
		{},		// Switch2
		{},		// Switch3
		{},		// Switch4
		{},		// Switch5
		{},		// Hit_Self
		35_ri,		// Pound
		25_ri,		// Karate Chop
		10_ri,		// DoubleSlap
		15_ri,		// Comet Punch
		20_ri,		// Mega Punch
		20_ri,		// Pay Day
		15_ri,		// Fire Punch
		15_ri,		// Ice Punch
		15_ri,		// ThunderPunch
		35_ri,		// Scratch
		30_ri,		// ViceGrip
		5_ri,		// Guillotine
		10_ri,		// Razor Wind
		30_ri,		// Swords Dance
		30_ri,		// Cut
		35_ri,		// Gust
		35_ri,		// Wing Attack
		20_ri,		// Whirlwind
		15_ri,		// Fly
		20_ri,		// Bind
		20_ri,		// Slam
		15_ri,		// Vine Whip
		20_ri,		// Stomp
		30_ri,		// Double Kick
		5_ri,		// Mega Kick
		10_ri,		// Jump Kick
		15_ri,		// Rolling Kick
		15_ri,		// Sand-Attack
		15_ri,		// Headbutt
		25_ri,		// Horn Attack
		20_ri,		// Fury Attack
		5_ri,		// Horn Drill
		35_ri,		// Tackle
		15_ri,		// Body Slam
		20_ri,		// Wrap
		20_ri,		// Take Down
		10_ri,		// Thrash
		15_ri,		// Double-Edge
		30_ri,		// Tail Whip
		35_ri,		// Poison Sting
		20_ri,		// Twineedle
		20_ri,		// Pin Missile
		30_ri,		// Leer
		25_ri,		// Bite
		40_ri,		// Growl
		20_ri,		// Roar
		15_ri,		// Sing
		20_ri,		// Supersonic
		20_ri,		// SonicBoom
		20_ri,		// Disable
		30_ri,		// Acid
		25_ri,		// Ember
		15_ri,		// Flamethrower
		30_ri,		// Mist
		25_ri,		// Water Gun
		5_ri,		// Hydro Pump
		15_ri,		// Surf
		10_ri,		// Ice Beam
		5_ri,		// Blizzard
		20_ri,		// Psybeam
		20_ri,		// BubbleBeam
		20_ri,		// Aurora Beam
		5_ri,		// Hyper Beam
		35_ri,		// Peck
		20_ri,		// Drill Peck
		25_ri,		// Submission
		20_ri,		// Low Kick
		20_ri,		// Counter
		20_ri,		// Seismic Toss
		15_ri,		// Strength
		25_ri,		// Absorb
		15_ri,		// Mega Drain
		10_ri,		// Leech Seed
		40_ri,		// Growth
		25_ri,		// Razor Leaf
		10_ri,		// SolarBeam
		35_ri,		// PoisonPowder
		30_ri,		// Stun Spore
		15_ri,		// Sleep Powder
		10_ri,		// Petal Dance
		40_ri,		// String Shot
		10_ri,		// Dragon Rage
		15_ri,		// Fire Spin
		30_ri,		// ThunderShock
		15_ri,		// Thunderbolt
		20_ri,		// Thunder Wave
		10_ri,		// Thunder
		15_ri,		// Rock Throw
		10_ri,		// Earthquake
		5_ri,		// Fissure
		10_ri,		// Dig
		10_ri,		// Toxic
		25_ri,		// Confusion
		10_ri,		// Psychic
		20_ri,		// Hypnosis
		40_ri,		// Meditate
		30_ri,		// Agility
		30_ri,		// Quick Attack
		20_ri,		// Rage
		20_ri,		// Teleport
		15_ri,		// Night Shade
		10_ri,		// Mimic
		40_ri,		// Screech
		15_ri,		// Double Team
		10_ri,		// Recover
		30_ri,		// Harden
		20_ri,		// Minimize
		20_ri,		// SmokeScreen
		10_ri,		// Confuse Ray
		40_ri,		// Withdraw
		40_ri,		// Defense Curl
		30_ri,		// Barrier
		30_ri,		// Light Screen
		30_ri,		// Haze
		20_ri,		// Reflect
		30_ri,		// Focus Energy
		10_ri,		// Bide
		10_ri,		// Metronome
		20_ri,		// Mirror Move
		5_ri,		// Selfdestruct
		10_ri,		// Egg Bomb
		30_ri,		// Lick
		20_ri,		// Smog
		20_ri,		// Sludge
		20_ri,		// Bone Club
		5_ri,		// Fire Blast
		15_ri,		// Waterfall
		15_ri,		// Clamp
		20_ri,		// Swift
		15_ri,		// Skull Bash
		15_ri,		// Spike Cannon
		35_ri,		// Constrict
		20_ri,		// Amnesia
		15_ri,		// Kinesis
		10_ri,		// Softboiled
		10_ri,		// Hi Jump Kick
		30_ri,		// Glare
		15_ri,		// Dream Eater
		40_ri,		// Poison Gas
		20_ri,		// Barrage
		15_ri,		// Leech Life
		10_ri,		// Lovely Kiss
		5_ri,		// Sky Attack
		10_ri,		// Transform
		30_ri,		// Bubble
		10_ri,		// Dizzy Punch
		15_ri,		// Spore
		20_ri,		// Flash
		15_ri,		// Psywave
		40_ri,		// Splash
		40_ri,		// Acid Armor
		10_ri,		// Crabhammer
		5_ri,		// Explosion
		15_ri,		// Fury Swipes
		10_ri,		// Bonemerang
		10_ri,		// Rest
		10_ri,		// Rock Slide
		15_ri,		// Hyper Fang
		30_ri,		// Sharpen
		30_ri,		// Conversion
		10_ri,		// Tri Attack
		10_ri,		// Super Fang
		20_ri,		// Slash
		10_ri,		// Substitute
		{},		// Struggle
		1_ri,		// Sketch
		10_ri,		// Triple Kick
		10_ri,		// Thief
		10_ri,		// Spider Web
		5_ri,		// Mind Reader
		15_ri,		// Nightmare
		25_ri,		// Flame Wheel
		15_ri,		// Snore
		10_ri,		// Curse
		15_ri,		// Flail
		30_ri,		// Conversion 2
		5_ri,		// Aeroblast
		40_ri,		// Cotton Spore
		15_ri,		// Reversal
		10_ri,		// Spite
		25_ri,		// Powder Snow
		10_ri,		// Protect
		30_ri,		// Mach Punch
		10_ri,		// Scary Face
		20_ri,		// Faint Attack
		10_ri,		// Sweet Kiss
		10_ri,		// Belly Drum
		10_ri,		// Sludge Bomb
		10_ri,		// Mud-Slap
		10_ri,		// Octazooka
		20_ri,		// Spikes
		5_ri,		// Zap Cannon
		40_ri,		// Foresight
		5_ri,		// Destiny Bond
		5_ri,		// Perish Song
		15_ri,		// Icy Wind
		5_ri,		// Detect
		10_ri,		// Bone Rush
		5_ri,		// Lock-On
		10_ri,		// Outrage
		10_ri,		// Sandstorm
		10_ri,		// Giga Drain
		10_ri,		// Endure
		20_ri,		// Charm
		20_ri,		// Rollout
		40_ri,		// False Swipe
		15_ri,		// Swagger
		10_ri,		// Milk Drink
		20_ri,		// Spark
		20_ri,		// Fury Cutter
		25_ri,		// Steel Wing
		5_ri,		// Mean Look
		15_ri,		// Attract
		10_ri,		// Sleep Talk
		5_ri,		// Heal Bell
		20_ri,		// Return
		15_ri,		// Present
		20_ri,		// Frustration
		25_ri,		// Safeguard
		20_ri,		// Pain Split
		5_ri,		// Sacred Fire
		30_ri,		// Magnitude
		5_ri,		// DynamicPunch
		10_ri,		// Megahorn
		20_ri,		// DragonBreath
		40_ri,		// Baton Pass
		5_ri,		// Encore
		20_ri,		// Pursuit
		40_ri,		// Rapid Spin
		20_ri,		// Sweet Scent
		15_ri,		// Iron Tail
		35_ri,		// Metal Claw
		10_ri,		// Vital Throw
		5_ri,		// Morning Sun
		5_ri,		// Synthesis
		5_ri,		// Moonlight
		15_ri,		// Hidden Power
		5_ri,		// Cross Chop
		20_ri,		// Twister
		5_ri,		// Rain Dance
		5_ri,		// Sunny Day
		15_ri,		// Crunch
		20_ri,		// Mirror Coat
		10_ri,		// Psych Up
		5_ri,		// ExtremeSpeed
		5_ri,		// AncientPower
		15_ri,		// Shadow Ball
		10_ri,		// Future Sight
		15_ri,		// Rock Smash
		15_ri,		// Whirlpool
		10_ri,		// Beat Up
		10_ri,		// Fake Out
		10_ri,		// Uproar
		20_ri,		// Stockpile
		10_ri,		// Spit Up
		10_ri,		// Swallow
		10_ri,		// Heat Wave
		10_ri,		// Hail
		15_ri,		// Torment
		15_ri,		// Flatter
		15_ri,		// Will-O-Wisp
		10_ri,		// Memento
		20_ri,		// Facade
		20_ri,		// Focus Punch
		10_ri,		// SmellingSalt
		20_ri,		// Follow Me
		20_ri,		// Nature Power
		20_ri,		// Charge
		20_ri,		// Taunt
		20_ri,		// Helping Hand
		10_ri,		// Trick
		10_ri,		// Role Play
		10_ri,		// Wish
		20_ri,		// Assist
		20_ri,		// Ingrain
		5_ri,		// Superpower
		15_ri,		// Magic Coat
		10_ri,		// Recycle
		10_ri,		// Revenge
		15_ri,		// Brick Break
		10_ri,		// Yawn
		20_ri,		// Knock Off
		5_ri,		// Endeavor
		5_ri,		// Eruption
		10_ri,		// Skill Swap
		10_ri,		// Imprison
		20_ri,		// Refresh
		5_ri,		// Grudge
		10_ri,		// Snatch
		20_ri,		// Secret Power
		10_ri,		// Dive
		20_ri,		// Arm Thrust
		20_ri,		// Camouflage
		20_ri,		// Tail Glow
		5_ri,		// Luster Purge
		5_ri,		// Mist Ball
		15_ri,		// FeatherDance
		20_ri,		// Teeter Dance
		10_ri,		// Blaze Kick
		15_ri,		// Mud Sport
		20_ri,		// Ice Ball
		15_ri,		// Needle Arm
		10_ri,		// Slack Off
		10_ri,		// Hyper Voice
		15_ri,		// Poison Fang
		10_ri,		// Crush Claw
		5_ri,		// Blast Burn
		5_ri,		// Hydro Cannon
		10_ri,		// Meteor Mash
		15_ri,		// Astonish
		10_ri,		// Weather Ball
		5_ri,		// Aromatherapy
		20_ri,		// Fake Tears
		25_ri,		// Air Cutter
		5_ri,		// Overheat
		40_ri,		// Odor Sleuth
		10_ri,		// Rock Tomb
		5_ri,		// Silver Wind
		40_ri,		// Metal Sound
		15_ri,		// GrassWhistle
		20_ri,		// Tickle
		20_ri,		// Cosmic Power
		5_ri,		// Water Spout
		15_ri,		// Signal Beam
		20_ri,		// Shadow Punch
		30_ri,		// Extrasensory
		15_ri,		// Sky Uppercut
		15_ri,		// Sand Tomb
		5_ri,		// Sheer Cold
		10_ri,		// Muddy Water
		30_ri,		// Bullet Seed
		20_ri,		// Aerial Ace
		30_ri,		// Icicle Spear
		15_ri,		// Iron Defense
		5_ri,		// Block
		40_ri,		// Howl
		15_ri,		// Dragon Claw
		5_ri,		// Frenzy Plant
		20_ri,		// Bulk Up
		5_ri,		// Bounce
		15_ri,		// Mud Shot
		25_ri,		// Poison Tail
		40_ri,		// Covet
		15_ri,		// Volt Tackle
		20_ri,		// Magical Leaf
		15_ri,		// Water Sport
		20_ri,		// Calm Mind
		15_ri,		// Leaf Blade
		20_ri,		// Dragon Dance
		10_ri,		// Rock Blast
		20_ri,		// Shock Wave
		20_ri,		// Water Pulse
		5_ri,		// Doom Desire
		5_ri,		// Psycho Boost
		10_ri,		// Roost
		5_ri,		// Gravity
		40_ri,		// Miracle Eye
		10_ri,		// Wake-Up Slap
		10_ri,		// Hammer Arm
		5_ri,		// Gyro Ball
		10_ri,		// Healing Wish
		10_ri,		// Brine
		15_ri,		// Natural Gift
		10_ri,		// Feint
		20_ri,		// Pluck
		30_ri,		// Tailwind
		30_ri,		// Acupressure
		10_ri,		// Metal Burst
		20_ri,		// U-turn
		5_ri,		// Close Combat
		10_ri,		// Payback
		10_ri,		// Assurance
		15_ri,		// Embargo
		10_ri,		// Fling
		10_ri,		// Psycho Shift
		5_ri,		// Trump Card
		15_ri,		// Heal Block
		5_ri,		// Wring Out
		10_ri,		// Power Trick
		10_ri,		// Gastro Acid
		30_ri,		// Lucky Chant
		20_ri,		// Me First
		20_ri,		// Copycat
		10_ri,		// Power Swap
		10_ri,		// Guard Swap
		5_ri,		// Punishment
		5_ri,		// Last Resort
		10_ri,		// Worry Seed
		5_ri,		// Sucker Punch
		20_ri,		// Toxic Spikes
		10_ri,		// Heart Swap
		20_ri,		// Aqua Ring
		10_ri,		// Magnet Rise
		15_ri,		// Flare Blitz
		10_ri,		// Force Palm
		20_ri,		// Aura Sphere
		20_ri,		// Rock Polish
		20_ri,		// Poison Jab
		15_ri,		// Dark Pulse
		15_ri,		// Night Slash
		10_ri,		// Aqua Tail
		15_ri,		// Seed Bomb
		20_ri,		// Air Slash
		15_ri,		// X-Scissor
		10_ri,		// Bug Buzz
		10_ri,		// Dragon Pulse
		10_ri,		// Dragon Rush
		20_ri,		// Power Gem
		10_ri,		// Drain Punch
		30_ri,		// Vacuum Wave
		5_ri,		// Focus Blast
		10_ri,		// Energy Ball
		15_ri,		// Brave Bird
		10_ri,		// Earth Power
		10_ri,		// Switcheroo
		5_ri,		// Giga Impact
		20_ri,		// Nasty Plot
		30_ri,		// Bullet Punch
		10_ri,		// Avalanche
		30_ri,		// Ice Shard
		15_ri,		// Shadow Claw
		15_ri,		// Thunder Fang
		15_ri,		// Ice Fang
		15_ri,		// Fire Fang
		30_ri,		// Shadow Sneak
		10_ri,		// Mud Bomb
		20_ri,		// Psycho Cut
		15_ri,		// Zen Headbutt
		10_ri,		// Mirror Shot
		10_ri,		// Flash Cannon
		20_ri,		// Rock Climb
		15_ri,		// Defog
		5_ri,		// Trick Room
		5_ri,		// Draco Meteor
		15_ri,		// Discharge
		15_ri,		// Lava Plume
		5_ri,		// Leaf Storm
		10_ri,		// Power Whip
		5_ri,		// Rock Wrecker
		20_ri,		// Cross Poison
		5_ri,		// Gunk Shot
		15_ri,		// Iron Head
		20_ri,		// Magnet Bomb
		5_ri,		// Stone Edge
		20_ri,		// Captivate
		20_ri,		// Stealth Rock
		20_ri,		// Grass Knot
		20_ri,		// Chatter
		10_ri,		// Judgment
		20_ri,		// Bug Bite
		10_ri,		// Charge Beam
		15_ri,		// Wood Hammer
		20_ri,		// Aqua Jet
		15_ri,		// Attack Order
		10_ri,		// Defend Order
		10_ri,		// Heal Order
		5_ri,		// Head Smash
		10_ri,		// Double Hit
		5_ri,		// Roar of Time
		5_ri,		// Spacial Rend
		10_ri,		// Lunar Dance
		5_ri,		// Crush Grip
		5_ri,		// Magma Storm
		10_ri,		// Dark Void
		5_ri,		// Seed Flare
		5_ri,		// Ominous Wind
		5_ri,		// Shadow Force
		15_ri,		// Hone Claws
		10_ri,		// Wide Guard
		10_ri,		// Guard Split
		10_ri,		// Power Split
		10_ri,		// Wonder Room
		10_ri,		// Psyshock
		10_ri,		// Venoshock
		15_ri,		// Autotomize
		20_ri,		// Rage Powder
		15_ri,		// Telekinesis
		10_ri,		// Magic Room
		15_ri,		// Smack Down
		10_ri,		// Storm Throw
		15_ri,		// Flame Burst
		10_ri,		// Sludge Wave
		20_ri,		// Quiver Dance
		10_ri,		// Heavy Slam
		15_ri,		// Synchronoise
		10_ri,		// Electro Ball
		20_ri,		// Soak
		20_ri,		// Flame Charge
		20_ri,		// Coil
		20_ri,		// Low Sweep
		20_ri,		// Acid Spray
		15_ri,		// Foul Play
		15_ri,		// Simple Beam
		15_ri,		// Entrainment
		15_ri,		// After You
		15_ri,		// Round
		15_ri,		// Echoed Voice
		20_ri,		// Chip Away
		15_ri,		// Clear Smog
		10_ri,		// Stored Power
		15_ri,		// Quick Guard
		15_ri,		// Ally Switch
		15_ri,		// Scald
		15_ri,		// Shell Smash
		10_ri,		// Heal Pulse
		10_ri,		// Hex
		10_ri,		// Sky Drop
		10_ri,		// Shift Gear
		10_ri,		// Circle Throw
		15_ri,		// Incinerate
		15_ri,		// Quash
		15_ri,		// Acrobatics
		15_ri,		// Reflect Type
		5_ri,		// Retaliate
		5_ri,		// Final Gambit
		15_ri,		// Bestow
		5_ri,		// Inferno
		10_ri,		// Water Pledge
		10_ri,		// Fire Pledge
		10_ri,		// Grass Pledge
		20_ri,		// Volt Switch
		20_ri,		// Struggle Bug
		20_ri,		// Bulldoze
		10_ri,		// Frost Breath
		10_ri,		// Dragon Tail
		30_ri,		// Work Up
		15_ri,		// Electroweb
		15_ri,		// Wild Charge
		10_ri,		// Drill Run
		15_ri,		// Dual Chop
		25_ri,		// Heart Stamp
		10_ri,		// Horn Leech
		20_ri,		// Sacred Sword
		10_ri,		// Razor Shell
		10_ri,		// Heat Crash
		10_ri,		// Leaf Tornado
		20_ri,		// Steamroller
		10_ri,		// Cotton Guard
		10_ri,		// Night Daze
		10_ri,		// Psystrike
		10_ri,		// Tail Slap
		10_ri,		// Hurricane
		15_ri,		// Head Charge
		15_ri,		// Gear Grind
		5_ri,		// Searing Shot
		5_ri,		// Techno Blast
		10_ri,		// Relic Song
		10_ri,		// Secret Sword
		10_ri,		// Glaciate
		5_ri,		// Bolt Strike
		5_ri,		// Blue Flare
		10_ri,		// Fiery Dance
		5_ri,		// Freeze Shock
		5_ri,		// Ice Burn
		15_ri,		// Snarl
		10_ri,		// Icicle Crash
		5_ri,		// V-create
		5_ri,		// Fusion Flare
		5_ri		// Fusion Bolt
	};
	return get_pp[static_cast<std::size_t>(move)];
}

}	// namespace technicalmachine
