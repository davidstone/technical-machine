// Random effects of moves
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

#include "variable.hpp"
#include <algorithm>
#include <cassert>
#include "phazing_in_same_pokemon.hpp"
#include "team.hpp"
#include "move/moves.hpp"
#include "pokemon/level.hpp"

namespace technicalmachine {

Variable::Variable():
	m_value(0),
	m_probability(1, 1) {
}

Variable::Variable(unsigned v, Rational set_probability):
	m_value (v),
	m_probability(set_probability) {
}

void Variable::set_phaze_index(Team const & team, Species const species) {
	// Note: team is the Team that was phazed, not the team that used the
	// phazing move
	assert(team.size() > 1);
	// This is required to work with my current battle implementation
	auto const pokemon_index = team.pokemon().has_switched() ? team.all_pokemon().replacement() : team.pokemon().index();
	auto const new_index = team.all_pokemon().find_index(species);
	if (new_index == pokemon_index) {
		throw PhazingInSamePokemon(new_index);
	}
	m_value = static_cast<unsigned>(BOUNDED_INTEGER_CONDITIONAL(new_index < pokemon_index, new_index, new_index - 1_bi));
}

void Variable::set_flinch(bool const set) {
	m_value = set ? 1 : 0;
}

unsigned Variable::value() const {
	return m_value;
}

Rational Variable::probability() const {
	return m_probability;
}

bool Variable::effect_activates() const {
	return value() != 0;
}

uint8_t Variable::phaze_index(uint8_t const foe_index) const {
	return (value() < foe_index) ? value() : value() + 1;
}

bool Variable::present_heals() const {
	return value() != 0;
}

unsigned Variable::psywave_damage(Level const level) const {
	return static_cast<unsigned>(level()) * value() / 10;
}

void Variable::set_magnitude(unsigned const magnitude) {
	switch (magnitude) {
		case 4:
			m_value = 10;
			break;
		case 5:
			m_value = 30;
			break;
		case 6:
			m_value = 50;
			break;
		case 7:
			m_value = 70;
			break;
		case 8:
			m_value = 90;
			break;
		case 9:
			m_value = 110;
			break;
		case 10:
			m_value = 150;
			break;
		default:
			assert(false);
			break;
	}
}

namespace {
bool variable_is_set(Moves const move);
Probabilities magnitude_variables();
Probabilities present_variables();
Probabilities psywave_variables();
Rational probability_sum(Probabilities const & value);

std::array<Probabilities, number_of_moves> initial_probabilities() {
	std::array<Probabilities, number_of_moves> activate_probability {{
		{ },				// Switch0
		{ },				// Switch1
		{ },				// Switch2
		{ },				// Switch3
		{ },				// Switch4
		{ },				// Switch5
		{ },				// Hit self in confusion
		{ },				// Pound
		{ },				// Karate Chop
		{ },				// DoubleSlap
		{ },				// Comet Punch
		{ },				// Mega Punch
		{ },				// Pay Day
		{ Variable(1, Rational(1, 10)) },		// Fire Punch
		{ Variable(1, Rational(1, 10)) },		// Ice Punch
		{ Variable(1, Rational(1, 10)) },		// ThunderPunch
		{ },				// Scratch
		{ },				// ViceGrip
		{ },				// Guillotine
		{ },				// Razor Wind
		{ },				// Swords Dance
		{ },				// Cut
		{ },				// Gust
		{ },				// Wing Attack
		{ },				// Whirlwind
		{ },				// Fly
		{ },				// Bind
		{ },				// Slam
		{ },				// Vine Whip
		{ Variable(1, Rational(3, 10)) },		// Stomp
		{ },				// Double Kick
		{ },				// Mega Kick
		{ },				// Jump Kick
		{ Variable(1, Rational(3, 10)) },		// Rolling Kick
		{ },				// Sand-Attack
		{ Variable(1, Rational(3, 10)) },		// Headbutt
		{ },				// Horn Attack
		{ },				// Fury Attack
		{ },				// Horn Drill
		{ },				// Tackle
		{ Variable(1, Rational(3, 10)) },		// Body Slam
		{ },				// Wrap
		{ },				// Take Down
		{ },				// Thrash
		{ },				// Double-Edge
		{ },				// Tail Whip
		{ Variable(1, Rational(3, 10)) },		// Poison Sting
		{ Variable(1, Rational(1, 5)) },		// Twineedle
		{ },				// Pin Missile
		{ },				// Leer
		{ Variable(1, Rational(3, 10)) },		// Bite
		{ },				// Growl
		{ },				// Roar
		{ },				// Sing
		{ },				// Supersonic
		{ },				// SonicBoom
		{ },				// Disable
		{ Variable(1, Rational(1, 10)) },		// Acid
		{ Variable(1, Rational(1, 10)) },		// Ember
		{ Variable(1, Rational(1, 10)) },		// Flamethrower
		{ },				// Mist
		{ },				// Water Gun
		{ },				// Hydro Pump
		{ },				// Surf
		{ Variable(1, Rational(1, 10)) },		// Ice Beam
		{ Variable(1, Rational(1, 10)) },		// Blizzard
		{ Variable(1, Rational(1, 10)) },		// Psybeam
		{ Variable(1, Rational(1, 10)) },		// BubbleBeam
		{ Variable(1, Rational(1, 10)) },		// Aurora Beam
		{ },				// Hyper Beam
		{ },				// Peck
		{ },				// Drill Peck
		{ },				// Submission
		{ },				// Low Kick
		{ },				// Counter
		{ },				// Seismic Toss
		{ },				// Strength
		{ },				// Absorb
		{ },				// Mega Drain
		{ },				// Leech Seed
		{ },				// Growth
		{ },				// Razor Leaf
		{ },				// SolarBeam
		{ },				// PoisonPowder
		{ },				// Stun Spore
		{ },				// Sleep Powder
		{ },				// Petal Dance
		{ },				// String Shot
		{ },				// Dragon Rage
		{ },				// Fire Spin
		{ Variable(1, Rational(1, 10)) },		// ThunderShock
		{ Variable(1, Rational(1, 10)) },		// Thunderbolt
		{ },				// Thunder Wave
		{ Variable(1, Rational(3, 10)) },		// Thunder
		{ },				// Rock Throw
		{ },				// Earthquake
		{ },				// Fissure
		{ },				// Dig
		{ },				// Toxic
		{ Variable(1, Rational(1, 10)) },		// Confusion
		{ Variable(1, Rational(1, 10)) },		// Psychic
		{ },				// Hypnosis
		{ },				// Meditate
		{ },				// Agility
		{ },				// Quick Attack
		{ },				// Rage
		{ },				// Teleport
		{ },				// Night Shade
		{ },				// Mimic
		{ },				// Screech
		{ },				// Double Team
		{ },				// Recover
		{ },				// Harden
		{ },				// Minimize
		{ },				// SmokeScreen
		{ },				// Confuse Ray
		{ },				// Withdraw
		{ },				// Defense Curl
		{ },				// Barrier
		{ },				// Light Screen
		{ },				// Haze
		{ },				// Reflect
		{ },				// Focus Energy
		{ },				// Bide
		{ },				// Metronome
		{ },				// Mirror Move
		{ },				// Selfdestruct
		{ },				// Egg Bomb
		{ Variable(1, Rational(3, 10)) },		// Lick
		{ Variable(1, Rational(2, 5)) },		// Smog
		{ Variable(1, Rational(3, 10)) },		// Sludge
		{ Variable(1, Rational(1, 10)) },		// Bone Club
		{ Variable(1, Rational(1, 10)) },		// Fire Blast
		{ Variable(1, Rational(1, 5)) },		// Waterfall
		{ },				// Clamp
		{ },				// Swift
		{ },				// Skull Bash
		{ },				// Spike Cannon
		{ Variable(1, Rational(1, 10)) },		// Constrict
		{ },				// Amnesia
		{ },				// Kinesis
		{ },				// Softboiled
		{ },				// Hi Jump Kick
		{ },				// Glare
		{ },				// Dream Eater
		{ },				// Poison Gas
		{ },				// Barrage
		{ },				// Leech Life
		{ },				// Lovely Kiss
		{ Variable(1, Rational(3, 10)) },		// Sky Attack
		{ },				// Transform
		{ Variable(1, Rational(1, 10)) },		// Bubble
		{ Variable(1, Rational(1, 5)) },		// Dizzy Punch
		{ },				// Spore
		{ },				// Flash
		{ psywave_variables() },				// Psywave
		{ },				// Splash
		{ },				// Acid Armor
		{ },				// Crabhammer
		{ },				// Explosion
		{ },				// Fury Swipes
		{ },				// Bonemerang
		{ },				// Rest
		{ Variable(1, Rational(3, 10)) },		// Rock Slide
		{ Variable(1, Rational(1, 10)) },		// Hyper Fang
		{ },				// Sharpen
		{ },				// Conversion
		{ Variable(1, Rational(1, 15)), Variable(2, Rational(1, 15)), Variable(3, Rational(1, 15)) },		// Tri Attack
		{ },				// Super Fang
		{ },				// Slash
		{ },				// Substitute
		{ },				// Struggle
		{ },				// Sketch
		{ },				// Triple Kick
		{ },				// Thief
		{ },				// Spider Web
		{ },				// Mind Reader
		{ },				// Nightmare
		{ Variable(1, Rational(1, 10)) },		// Flame Wheel
		{ Variable(1, Rational(3, 10)) },		// Snore
		{ },				// Curse
		{ },				// Flail
		{ },				// Conversion 2
		{ },				// Aeroblast
		{ },				// Cotton Spore
		{ },				// Reversal
		{ },				// Spite
		{ Variable(1, Rational(1, 10)) },		// Powder Snow
		{ },				// Protect
		{ },				// Mach Punch
		{ },				// Scary Face
		{ },				// Faint Attack
		{ },				// Sweet Kiss
		{ },				// Belly Drum
		{ Variable(1, Rational(3, 10)) },		// Sludge Bomb
		{ },				// Mud-Slap
		{ Variable(1, Rational(1, 2)) },		// Octazooka
		{ },				// Spikes
		{ Variable(1, Rational(1, 1)) },		// Zap Cannon
		{ },				// Foresight
		{ },				// Destiny Bond
		{ },				// Perish Song
		{ },				// Icy Wind
		{ },				// Detect
		{ },				// Bone Rush
		{ },				// Lock-On
		{ },				// Outrage
		{ },				// Sandstorm
		{ },				// Giga Drain
		{ },				// Endure
		{ },				// Charm
		{ },				// Rollout
		{ },				// False Swipe
		{ },				// Swagger
		{ },				// Milk Drink
		{ Variable(1, Rational(3, 10)) },		// Spark
		{ },				// Fury Cutter
		{ Variable(1, Rational(1, 10)) },		// Steel Wing
		{ },				// Mean Look
		{ },				// Attract
		{ },				// Sleep Talk
		{ },				// Heal Bell
		{ },				// Return
		{ present_variables() },				// Present
		{ },				// Frustration
		{ },				// Safeguard
		{ },				// Pain Split
		{ Variable(1, Rational(1, 2)) },		// Sacred Fire
		{ magnitude_variables() },			// Magnitude
		{ Variable(1, Rational(1, 1)) },		// DynamicPunch
		{ },				// Megahorn
		{ Variable(1, Rational(3, 10)) },		// DragonBreath
		{ },				// Baton Pass
		{ },				// Encore
		{ },				// Pursuit
		{ },				// Rapid Spin
		{ },				// Sweet Scent
		{ Variable(1, Rational(1, 10)) },		// Iron Tail
		{ Variable(1, Rational(1, 10)) },		// Metal Claw
		{ },				// Vital Throw
		{ },				// Morning Sun
		{ },				// Synthesis
		{ },				// Moonlight
		{ },				// Hidden Power
		{ },				// Cross Chop
		{ Variable(1, Rational(1, 5)) },		// Twister
		{ },				// Rain Dance
		{ },				// Sunny Day
		{ Variable(1, Rational(1, 5)) },		// Crunch
		{ },				// Mirror Coat
		{ },				// Psych Up
		{ },				// ExtremeSpeed
		{ Variable(1, Rational(1, 10)) },		// AncientPower
		{ Variable(1, Rational(1, 5)) },		// Shadow Ball
		{ },				// Future Sight
		{ Variable(1, Rational(1, 2)) },		// Rock Smash
		{ },				// Whirlpool
		{ },				// Beat Up
		{ },				// Fake Out
		{ },				// Uproar
		{ },				// Stockpile
		{ },				// Spit Up
		{ },				// Swallow
		{ Variable(1, Rational(1, 10)) },		// Heat Wave
		{ },				// Hail
		{ },				// Torment
		{ },				// Flatter
		{ },				// Will-O-Wisp
		{ },				// Memento
		{ },				// Facade
		{ },				// Focus Punch
		{ },				// SmellingSalt
		{ },				// Follow Me
		{ },				// Nature Power
		{ },				// Charge
		{ },				// Taunt
		{ },				// Helping Hand
		{ },				// Trick
		{ },				// Role Play
		{ },				// Wish
		{ },				// Assist
		{ },				// Ingrain
		{ },				// Superpower
		{ },				// Magic Coat
		{ },				// Recycle
		{ },				// Revenge
		{ },				// Brick Break
		{ },				// Yawn
		{ },				// Knock Off
		{ },				// Endeavor
		{ },				// Eruption
		{ },				// Skill Swap
		{ },				// Imprison
		{ },				// Refresh
		{ },				// Grudge
		{ },				// Snatch
		{ Variable(1, Rational(3, 10)) },		// Secret Power
		{ },				// Dive
		{ },				// Arm Thrust
		{ },				// Camouflage
		{ },				// Tail Glow
		{ },				// Luster Purge
		{ Variable(1, Rational(1, 2)) },		// Mist Ball
		{ },				// FeatherDance
		{ },				// Teeter Dance
		{ Variable(1, Rational(1, 10)) },		// Blaze Kick
		{ },				// Mud Sport
		{ },				// Ice Ball
		{ Variable(1, Rational(3, 10)) },		// Needle Arm
		{ },				// Slack Off
		{ },				// Hyper Voice
		{ Variable(1, Rational(3, 10)) },		// Poison Fang
		{ Variable(1, Rational(1, 2)) },		// Crush Claw
		{ },				// Blast Burn
		{ },				// Hydro Cannon
		{ Variable(1, Rational(1, 5)) },		// Meteor Mash
		{ Variable(1, Rational(3, 10)) },		// Astonish
		{ },				// Weather Ball
		{ },				// Aromatherapy
		{ },				// Fake Tears
		{ },				// Air Cutter
		{ },				// Overheat
		{ },				// Odor Sleuth
		{ },				// Rock Tomb
		{ Variable(1, Rational(1, 10)) },		// Silver Wind
		{ },				// Metal Sound
		{ },				// GrassWhistle
		{ },				// Tickle
		{ },				// Cosmic Power
		{ },				// Water Spout
		{ Variable(1, Rational(1, 10)) },		// Signal Beam
		{ },				// Shadow Punch
		{ Variable(1, Rational(1, 10)) },		// Extrasensory
		{ },				// Sky Uppercut
		{ },				// Sand Tomb
		{ },				// Sheer Cold
		{ Variable(1, Rational(3, 10)) },		// Muddy Water
		{ },				// Bullet Seed
		{ },				// Aerial Ace
		{ },				// Icicle Spear
		{ },				// Iron Defense
		{ },				// Block
		{ },				// Howl
		{ },				// Dragon Claw
		{ },				// Frenzy Plant
		{ },				// Bulk Up
		{ Variable(1, Rational(3, 10)) },		// Bounce
		{ },				// Mud Shot
		{ Variable(1, Rational(1, 10)) },		// Poison Tail
		{ },				// Covet
		{ Variable(1, Rational(1, 10)) },		// Volt Tackle
		{ },				// Magical Leaf
		{ },				// Water Sport
		{ },				// Calm Mind
		{ },				// Leaf Blade
		{ },				// Dragon Dance
		{ },				// Rock Blast
		{ },				// Shock Wave
		{ Variable(1, Rational(1, 5)) },		// Water Pulse
		{ },				// Doom Desire
		{ },				// Psycho Boost
		{ },				// Roost
		{ },				// Gravity
		{ },				// Miracle Eye
		{ },				// Wake-Up Slap
		{ },				// Hammer Arm
		{ },				// Gyro Ball
		{ },				// Healing Wish
		{ },				// Brine
		{ },				// Natural Gift
		{ },				// Feint
		{ },				// Pluck
		{ },				// Tailwind
		{ },				// Acupressure
		{ },				// Metal Burst
		{ },				// U-turn
		{ },				// Close Combat
		{ },				// Payback
		{ },				// Assurance
		{ },				// Embargo
		{ },				// Fling
		{ },				// Psycho Shift
		{ },				// Trump Card
		{ },				// Heal Block
		{ },				// Wring Out
		{ },				// Power Trick
		{ },				// Gastro Acid
		{ },				// Lucky Chant
		{ },				// Me First
		{ },				// Copycat
		{ },				// Power Swap
		{ },				// Guard Swap
		{ },				// Punishment
		{ },				// Last Resort
		{ },				// Worry Seed
		{ },				// Sucker Punch
		{ },				// Toxic Spikes
		{ },				// Heart Swap
		{ },				// Aqua Ring
		{ },				// Magnet Rise
		{ Variable(1, Rational(1, 10)) },		// Flare Blitz
		{ Variable(1, Rational(3, 10)) },		// Force Palm
		{ },				// Aura Sphere
		{ },				// Rock Polish
		{ Variable(1, Rational(3, 10)) },		// Poison Jab
		{ Variable(1, Rational(1, 5)) },		// Dark Pulse
		{ },				// Night Slash
		{ },				// Aqua Tail
		{ },				// Seed Bomb
		{ Variable(1, Rational(3, 10)) },		// Air Slash
		{ },				// X-Scissor
		{ Variable(1, Rational(1, 10)) },		// Bug Buzz
		{ },				// Dragon Pulse
		{ Variable(1, Rational(1, 5)) },		// Dragon Rush
		{ },				// Power Gem
		{ },				// Drain Punch
		{ },				// Vacuum Wave
		{ Variable(1, Rational(1, 10)) },		// Focus Blast
		{ Variable(1, Rational(1, 10)) },		// Energy Ball
		{ },				// Brave Bird
		{ Variable(1, Rational(1, 10)) },		// Earth Power
		{ },				// Switcheroo
		{ },				// Giga Impact
		{ },				// Nasty Plot
		{ },				// Bullet Punch
		{ },				// Avalanche
		{ },				// Ice Shard
		{ },				// Shadow Claw
		{ Variable(1, Rational(1, 10)), Variable(2, Rational(1, 10)), Variable(3, Rational(1, 100)) },		// Thunder Fang
		{ Variable(1, Rational(1, 10)), Variable(2, Rational(1, 10)), Variable(3, Rational(1, 100)) },		// Ice Fang
		{ Variable(1, Rational(1, 10)), Variable(2, Rational(1, 10)), Variable(3, Rational(1, 100)) },		// Fire Fang
		{ },				// Shadow Sneak
		{ Variable(1, Rational(3, 10)) },		// Mud Bomb
		{ },				// Psycho Cut
		{ Variable(1, Rational(1, 5)) },		// Zen Headbutt
		{ Variable(1, Rational(3, 10)) },		// Mirror Shot
		{ Variable(1, Rational(1, 10)) },		// Flash Cannon
		{ Variable(1, Rational(1, 5)) },		// Rock Climb
		{ },				// Defog
		{ },				// Trick Room
		{ },				// Draco Meteor
		{ Variable(1, Rational(3, 10)) },		// Discharge
		{ Variable(1, Rational(3, 10)) },		// Lava Plume
		{ },				// Leaf Storm
		{ },				// Power Whip
		{ },				// Rock Wrecker
		{ Variable(1, Rational(1, 10)) },		// Cross Poison
		{ Variable(1, Rational(3, 10)) },		// Gunk Shot
		{ Variable(1, Rational(3, 10)) },		// Iron Head
		{ },				// Magnet Bomb
		{ },				// Stone Edge
		{ },				// Captivate
		{ },				// Stealth Rock
		{ },				// Grass Knot
		{ },				// Chatter
		{ },				// Judgment
		{ },				// Bug Bite
		{ Variable(1, Rational(7, 10)) },		// Charge Beam
		{ },				// Wood Hammer
		{ },				// Aqua Jet
		{ },				// Attack Order
		{ },				// Defend Order
		{ },				// Heal Order
		{ },				// Head Smash
		{ },				// Double Hit
		{ },				// Roar of Time
		{ },				// Spacial Rend
		{ },				// Lunar Dance
		{ },				// Crush Grip
		{ },				// Magma Storm
		{ },				// Dark Void
		{ Variable(1, Rational(2, 5)) },		// Seed Flare
		{ Variable(1, Rational(1, 10)) },		// Ominous Wind
		{ },				// Shadow Force
		{ },				// Hone Claws
		{ },				// Wide Guard
		{ },				// Guard Split
		{ },				// Power Split
		{ },				// Wonder Room
		{ },				// Psyshock
		{ },				// Venoshock
		{ },				// Autotomize
		{ },				// Rage Powder
		{ },				// Telekinesis
		{ },				// Magic Room
		{ },				// Smack Down
		{ },				// Storm Throw
		{ },				// Flame Burst
		{ Variable(1, Rational(1, 10)) },		// Sludge Wave
		{ },				// Quiver Dance
		{ },				// Heavy Slam
		{ },				// Synchronoise
		{ },				// Electro Ball
		{ },				// Soak
		{ },				// Flame Charge
		{ },				// Coil
		{ },				// Low Sweep
		{ },				// Acid Spray
		{ },				// Foul Play
		{ },				// Simple Beam
		{ },				// Entrainment
		{ },				// After You
		{ },				// Round
		{ },				// Echoed Voice
		{ },				// Chip Away
		{ },				// Clear Smog
		{ },				// Stored Power
		{ },				// Quick Guard
		{ },				// Ally Switch
		{ Variable(1, Rational(3, 10)) },		// Scald
		{ },				// Shell Smash
		{ },				// Heal Pulse
		{ },				// Hex
		{ },				// Sky Drop
		{ },				// Shift Gear
		{ },				// Circle Throw
		{ },				// Incinerate
		{ },				// Quash
		{ },				// Acrobatics
		{ },				// Reflect Type
		{ },				// Retaliate
		{ },				// Final Gambit
		{ },				// Bestow
		{ Variable(1, Rational(1, 1)) },		// Inferno
		{ },				// Water Pledge
		{ },				// Fire Pledge
		{ },				// Grass Pledge
		{ },				// Volt Switch
		{ },				// Struggle Bug
		{ },				// Bulldoze
		{ },				// Frost Breath
		{ },				// Dragon Tail
		{ },				// Work Up
		{ },				// Electroweb
		{ },				// Wild Charge
		{ },				// Drill Run
		{ },				// Dual Chop
		{ Variable(1, Rational(3, 10)) },		// Heart Stamp
		{ },				// Horn Leech
		{ },				// Sacred Sword
		{ Variable(1, Rational(1, 2)) },		// Razor Shell
		{ },				// Heat Crash
		{ Variable(1, Rational(3, 10)) },		// Leaf Tornado
		{ Variable(1, Rational(3, 10)) },		// Steamroller
		{ },				// Cotton Guard
		{ Variable(1, Rational(2, 5)) },		// Night Daze
		{ },				// Psystrike
		{ },				// Tail Slap
		{ Variable(1, Rational(3, 10)) },		// Hurricane
		{ },				// Head Charge
		{ },				// Gear Grind
		{ Variable(1, Rational(3, 10)) },		// Searing Shot
		{ },				// Techno Blast
		{ Variable(1, Rational(1, 10)) },		// Relic Song
		{ },				// Secret Sword
		{ },				// Glaciate
		{ Variable(1, Rational(1, 5)) },		// Bolt Strike
		{ Variable(1, Rational(1, 5)) },		// Blue Flare
		{ },				// Fiery Dance
		{ Variable(1, Rational(3, 10)) },		// Freeze Shock
		{ Variable(1, Rational(3, 10)) },		// Ice Burn
		{ },				// Snarl
		{ Variable(1, Rational(3, 10)) },		// Icicle Crash
		{ },				// V-create
		{ },				// Fusion Flare
		{ }		// Fusion Bolt
	}};
	for (auto move = static_cast<Moves>(0); move != Moves::END; move = static_cast<Moves>(static_cast<unsigned>(move) + 1)) {
		if (variable_is_set(move)) {
			continue;
		}
		auto & value = activate_probability[static_cast<unsigned>(move)];
		value.emplace(std::begin(value), 0, complement(probability_sum(value)));
	}
	return activate_probability;
}

bool variable_is_set(Moves const move) {
	switch (move) {
		case Moves::Magnitude:
		case Moves::Present:
		case Moves::Psywave:
			return true;
		default:
			return false;
	}
}

Rational probability_sum(Probabilities const & value) {
	using std::begin;
	using std::end;
	using std::accumulate;
	return accumulate(begin(value), end(value), Rational(0, 1), [](Rational const rational, Variable const & variable) {
		return rational + variable.probability();
	});
}

Probabilities magnitude_variables() {
	static Probabilities const variables {
		Variable(10, Rational(1, 20)),
		Variable(30, Rational(1, 10)),
		Variable(50, Rational(1, 5)),
		Variable(70, Rational(3, 10)),
		Variable(90, Rational(1, 5)),
		Variable(110, Rational(1, 10)),
		Variable(150, Rational(1, 20))
	};
	assert(probability_sum(variables) == Rational(1, 1));
	return variables;
}

Probabilities present_variables() {
	Probabilities probabilities;
	for (auto const n : { 0, 40, 80, 120 }) {
		probabilities.emplace_back(n, Rational(1, 4));
	}
	return probabilities;
}

Probabilities psywave_variables() {
	Probabilities probabilities;
	constexpr unsigned min = 5;
	constexpr unsigned max = 15 + 1;
	for (unsigned n = min; n != max; ++n) {
		probabilities.emplace_back(n, Rational(1, max - min));
	}
	return probabilities;
}

Probabilities phaze_probability(unsigned const foe_size) {
	Probabilities probabilities;
	unsigned const possible_replacements = (foe_size > 2) ? (foe_size - 1) : 1;
	for (unsigned n = 0; n != possible_replacements; ++n) {
		probabilities.emplace_back(n, Rational(1, possible_replacements));
	}
	return probabilities;
}

Probabilities acupressure_probability(ActivePokemon const & pokemon) {
	static constexpr StatNames boostable_stats[] = {
		StatNames::ATK, StatNames::DEF, StatNames::SPA, StatNames::SPD, StatNames::SPE, StatNames::ACC, StatNames::EVA
	};
	auto const non_maxed_stats = std::count_if(std::begin(boostable_stats), std::end(boostable_stats), [&](StatNames const stat) {
		return pokemon.stage()[stat] != 6_bi;
	});
	if (non_maxed_stats == 0) {
		// Arbitrary value so the code doesn't break when Acupressure can no
		// longer work
		return Probabilities(1, Variable(0, Rational(1, 1)));
	}
	Rational const probability(1, static_cast<unsigned>(non_maxed_stats));
	Probabilities probabilities;
	for (auto const stat : boostable_stats) {
		probabilities.emplace_back(static_cast<unsigned>(stat), probability);
	}
	return probabilities;
}

}	// namespace

Probabilities all_probabilities(ActivePokemon const & pokemon, unsigned const foe_size) {
	static auto const probability = initial_probabilities();
	Moves const move = pokemon.move();
	if (is_phaze(move)) {
		return phaze_probability(foe_size);
	}
	else if (move == Moves::Acupressure) {
		return acupressure_probability(pokemon);
	}
	else {
		auto const index = static_cast<unsigned>(move);
		assert(index < number_of_moves);
		return probability[index];
	}
}
}	// namespace technicalmachine
