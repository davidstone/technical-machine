// class Move
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

#include "move.hpp"

#include <cassert>
#include <cstdint>

#include "moves.hpp"

#include "../rational.hpp"

#include "../pokemon/active_pokemon.hpp"

#include "../string_conversions/conversion.hpp"

#include "../type/effectiveness.hpp"
#include "../type/type.hpp"

namespace technicalmachine {
namespace {

constexpr uint8_t indeterminate_power = 0xFF;
constexpr uint8_t variable_power = indeterminate_power - 1;
Type get_type (Moves move);
uint8_t get_base_power (Moves move);

}	// unnamed namespace

Move::Move (Moves const move, unsigned const pp_ups) :
	m_name(move),
	pp(move, pp_ups),
	cached_base_power(get_base_power(move)),
	cached_type(get_type(move)) {
}

Moves Move::name() const {
	return m_name;
}

void Move::reset () {
	disable.reset();
	times_used.reset();
}

bool Move::is_regular() const {
	return !is_struggle() and !is_switch();
}

bool Move::is_damaging() const {
	return base_power() != 0;
}

bool Move::is_physical() const {
	return Classification(name()).is_physical();
}

bool Move::is_special() const {
	return Classification(name()).is_special();
}

bool Move::is_blocked_by_taunt() const {
	return !is_damaging();
}

bool Move::can_critical_hit() const {
	switch (base_power()) {
		case 0:
		case indeterminate_power:
			return false;
		default:
			return true;
	}
}

uint64_t Move::hash () const {
	return static_cast<uint64_t>(name()) + static_cast<uint64_t>(Moves::END) *
			(pp.hash() + pp.max_hash() *
			(disable.hash() + disable.max_hash() *
			times_used.hash()));
}

uint64_t Move::max_hash() const {
	return times_used.hash() * disable.max_hash() * pp.max_hash() * static_cast<uint64_t>(Moves::END);
}

bool operator== (Move const & lhs, Move const & rhs) {
	return lhs.name() == rhs.name() and
			lhs.disable == rhs.disable and
			lhs.pp == rhs.pp and
			lhs.times_used == rhs.times_used;
}

bool operator!= (Move const & lhs, Move const & rhs) {
	return !(lhs == rhs);
}

Type Move::type() const {
	return cached_type;
}

void Move::set_type(Type::Types const t) {
	assert(name() == Moves::Hidden_Power);
	cached_type = t;
}

unsigned Move::base_power() const {
	return cached_base_power;
}

Priority Move::priority() const {
	return Priority(name());
}

bool Move::is_struggle_or_switch () const {
	return is_struggle() or is_switch();
}

bool Move::is_struggle() const {
	return name() == Moves::Struggle;
}

bool Move::is_switch (Moves const name) {
	static_assert(static_cast<unsigned>(Moves::Switch0) == 0, "Switching is not the first Move enum.");
	return name <= Moves::Switch5;
}

bool Move::is_switch () const {
	return is_switch(name());
}

Moves Move::from_replacement (unsigned const replacement) {
	return static_cast<Moves>(replacement + static_cast<unsigned>(Moves::Switch0));
}

unsigned Move::to_replacement (Moves const name) {
	assert (is_switch(name));
	return static_cast<unsigned>(name) - static_cast<unsigned>(Moves::Switch0);
}

unsigned Move::to_replacement () const {
	return to_replacement(name());
}

bool Move::affects_target(ActivePokemon const & target, Weather const & weather) const {
	return !type().get_effectiveness(target).has_no_effect() and (type() != Type::Ground or grounded(target, weather));
}

bool Move::has_follow_up_decision () const {
	switch (name()) {
		case Moves::Baton_Pass:
		case Moves::U_turn:
			return true;
		default:
			return false;
	}
}

bool Move::calls_other_move () const {
	switch (name()) {
//		case Moves::Nature_Power:
		case Moves::Assist:
		case Moves::Copycat:
		case Moves::Me_First:
		case Moves::Metronome:
		case Moves::Mirror_Move:
		case Moves::Sleep_Talk:
			return true;
		default:
			return false;
	}
}

bool Move::cannot_ko () const {
	return name() == Moves::False_Swipe;
}

bool Move::breaks_screens () const {
	return name() == Moves::Brick_Break;
}

void Move::increment_use_counter() {
	times_used.increment();
}

bool Move::was_used_last () const {
	return times_used.was_used_last();
}

bool Move::is_bide() const {
	return name() == Moves::Bide;
}

unsigned Move::fury_cutter_power() const {
	return times_used.fury_cutter_power();
}

unsigned Move::momentum_move_power() const {
	return times_used.momentum_move_power();
}

unsigned Move::triple_kick_power() const {
	return times_used.triple_kick_power();
}

Rational Move::metronome_boost() const {
	return times_used.metronome_boost();
}

bool Move::is_phaze (Moves name) {
	switch (name) {
		case Moves::Roar:
		case Moves::Whirlwind:
			return true;
		default:
			return false;
	}
}

bool Move::is_phaze () const {
	return is_phaze (name());
}

bool Move::is_healing (Moves name) {
	switch (name) {
		case Moves::Heal_Order:
		case Moves::Milk_Drink:
		case Moves::Moonlight:
		case Moves::Morning_Sun:
		case Moves::Recover:
		case Moves::Rest:
		case Moves::Roost:
		case Moves::Slack_Off:
		case Moves::Softboiled:
		case Moves::Swallow:
		case Moves::Synthesis:
		case Moves::Wish:
			return true;
		default:
			return false;
	}
}

bool Move::is_healing () const {
	return is_healing (name());
}

bool Move::is_blocked_by_gravity () const {
	switch (name()) {
		case Moves::Bounce:
		case Moves::Fly:
		case Moves::Hi_Jump_Kick:
		case Moves::Jump_Kick:
		case Moves::Magnet_Rise:
		case Moves::Splash:
			return true;
		default:
			return false;
	}
}

bool Move::is_boosted_by_iron_fist () const {
	switch (name()) {
		case Moves::Bullet_Punch:
		case Moves::Comet_Punch:
		case Moves::Dizzy_Punch:
		case Moves::Drain_Punch:
		case Moves::DynamicPunch:
		case Moves::Fire_Punch:
		case Moves::Focus_Punch:
		case Moves::Hammer_Arm:
		case Moves::Ice_Punch:
		case Moves::Mach_Punch:
		case Moves::Mega_Punch:
		case Moves::Meteor_Mash:
		case Moves::Shadow_Punch:
		case Moves::Sky_Uppercut:
		case Moves::ThunderPunch:
			return true;
		default:
			return false;
	}
}

bool Move::is_boosted_by_reckless() const {
	switch (name()) {
		case Moves::Brave_Bird:
		case Moves::Double_Edge:
		case Moves::Flare_Blitz:
		case Moves::Head_Smash:
		case Moves::Submission:
		case Moves::Take_Down:
		case Moves::Volt_Tackle:
		case Moves::Wood_Hammer:
			return true;
		default:
			return false;
	}
}

bool Move::is_usable_while_sleeping () const {
	switch (name()) {
		case Moves::Sleep_Talk:
		case Moves::Snore:
			return true;
		default:
			return false;
	}
}

bool Move::is_usable_while_frozen () const {
	switch (name()) {
		case Moves::Flame_Wheel:
		case Moves::Sacred_Fire:
			return true;
		default:
			return false;
	}
}

bool Move::is_sound_based () const {
	switch (name()) {
		case Moves::Bug_Buzz:
		case Moves::Chatter:
		case Moves::GrassWhistle:
		case Moves::Growl:
		case Moves::Heal_Bell:
		case Moves::Hyper_Voice:
		case Moves::Metal_Sound:
		case Moves::Perish_Song:
		case Moves::Roar:
		case Moves::Screech:
		case Moves::Sing:
		case Moves::Snore:
		case Moves::Supersonic:
		case Moves::Uproar:
			return true;
		default:
			return false;
	}
}

bool Move::is_self_KO () const {
	switch (name()) {
		case Moves::Explosion:
		case Moves::Selfdestruct:
			return true;
		default:
			return false;
	}
}

namespace {

Type get_type (Moves const move) {
	static constexpr Type::Types move_type [] = {
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
	};
	return Type(move_type[static_cast<size_t>(move)]);
}

uint8_t get_base_power (Moves const move) {
	static constexpr uint8_t get_power [] = {
		0,		// Switch0
		0,		// Switch1
		0,		// Switch2
		0,		// Switch3
		0,		// Switch4
		0,		// Switch5
		40,		// Hit Self
		40,		// Pound
		50,		// Karate Chop
		15,		// DoubleSlap
		18,		// Comet Punch
		80,		// Mega Punch
		40,		// Pay Day
		75,		// Fire Punch
		75,		// Ice Punch
		75,		// ThunderPunch
		40,		// Scratch
		55,		// ViceGrip
		indeterminate_power,		// Guillotine
		80,		// Razor Wind
		0,		// Swords Dance
		50,		// Cut
		40,		// Gust
		60,		// Wing Attack
		0,		// Whirlwind
		90,		// Fly
		15,		// Bind
		80,		// Slam
		35,		// Vine Whip
		65,		// Stomp
		30,		// Double Kick
		120,		// Mega Kick
		100,		// Jump Kick
		60,		// Rolling Kick
		0,		// Sand-Attack
		70,		// Headbutt
		65,		// Horn Attack
		15,		// Fury Attack
		indeterminate_power,		// Horn Drill
		50,		// Tackle
		85,		// Body Slam
		15,		// Wrap
		90,		// Take Down
		120,		// Thrash
		120,		// Double-Edge
		0,		// Tail Whip
		15,		// Poison Sting
		25,		// Twineedle
		14,		// Pin Missile
		0,		// Leer
		60,		// Bite
		0,		// Growl
		0,		// Roar
		0,		// Sing
		0,		// Supersonic
		indeterminate_power,		// SonicBoom
		0,		// Disable
		40,		// Acid
		40,		// Ember
		95,		// Flamethrower
		0,		// Mist
		40,		// Water Gun
		120,		// Hydro Pump
		95,		// Surf
		95,		// Ice Beam
		120,		// Blizzard
		65,		// Psybeam
		65,		// BubbleBeam
		65,		// Aurora Beam
		150,		// Hyper Beam
		35,		// Peck
		80,		// Drill Peck
		80,		// Submission
		variable_power,		// Low Kick
		indeterminate_power,		// Counter
		indeterminate_power,		// Seismic Toss
		80,		// Strength
		20,		// Absorb
		40,		// Mega Drain
		0,		// Leech Seed
		0,		// Growth
		55,		// Razor Leaf
		120,		// SolarBeam
		0,		// PoisonPowder
		0,		// Stun Spore
		0,		// Sleep Powder
		120,		// Petal Dance
		0,		// String Shot
		indeterminate_power,		// Dragon Rage
		35,		// Fire Spin
		40,		// ThunderShock
		95,		// Thunderbolt
		0,		// Thunder Wave
		120,		// Thunder
		50,		// Rock Throw
		100,		// Earthquake
		indeterminate_power,		// Fissure
		80,		// Dig
		0,		// Toxic
		50,		// Confusion
		90,		// Psychic
		0,		// Hypnosis
		0,		// Meditate
		0,		// Agility
		40,		// Quick Attack
		20,		// Rage
		0,		// Teleport
		indeterminate_power,		// Night Shade
		0,		// Mimic
		0,		// Screech
		0,		// Double Team
		0,		// Recover
		0,		// Harden
		0,		// Minimize
		0,		// SmokeScreen
		0,		// Confuse Ray
		0,		// Withdraw
		0,		// Defense Curl
		0,		// Barrier
		0,		// Light Screen
		0,		// Haze
		0,		// Reflect
		0,		// Focus Energy
		variable_power,		// Bide
		0,		// Metronome
		0,		// Mirror Move
		200,		// Selfdestruct
		100,		// Egg Bomb
		20,		// Lick
		20,		// Smog
		65,		// Sludge
		65,		// Bone Club
		120,		// Fire Blast
		80,		// Waterfall
		35,		// Clamp
		60,		// Swift
		100,		// Skull Bash
		20,		// Spike Cannon
		10,		// Constrict
		0,		// Amnesia
		0,		// Kinesis
		0,		// Softboiled
		130,		// Hi Jump Kick
		0,		// Glare
		100,		// Dream Eater
		0,		// Poison Gas
		15,		// Barrage
		20,		// Leech Life
		0,		// Lovely Kiss
		140,		// Sky Attack
		0,		// Transform
		20,		// Bubble
		70,		// Dizzy Punch
		0,		// Spore
		0,		// Flash
		indeterminate_power,		// Psywave
		0,		// Splash
		0,		// Acid Armor
		90,		// Crabhammer
		250,		// Explosion
		18,		// Fury Swipes
		50,		// Bonemerang
		0,		// Rest
		75,		// Rock Slide
		80,		// Hyper Fang
		0,		// Sharpen
		0,		// Conversion
		80,		// Tri Attack
		indeterminate_power,		// Super Fang
		70,		// Slash
		0,		// Substitute
		50,		// Struggle
		0,		// Sketch
		10,		// Triple Kick
		40,		// Thief
		0,		// Spider Web
		0,		// Mind Reader
		0,		// Nightmare
		60,		// Flame Wheel
		40,		// Snore
		0,		// Curse
		variable_power,		// Flail
		0,		// Conversion 2
		100,		// Aeroblast
		0,		// Cotton Spore
		variable_power,		// Reversal
		0,		// Spite
		40,		// Powder Snow
		0,		// Protect
		40,		// Mach Punch
		0,		// Scary Face
		60,		// Faint Attack
		0,		// Sweet Kiss
		0,		// Belly Drum
		90,		// Sludge Bomb
		20,		// Mud-Slap
		65,		// Octazooka
		0,		// Spikes
		120,		// Zap Cannon
		0,		// Foresight
		0,		// Destiny Bond
		0,		// Perish Song
		55,		// Icy Wind
		0,		// Detect
		25,		// Bone Rush
		0,		// Lock-On
		120,		// Outrage
		0,		// Sandstorm
		75,		// Giga Drain
		0,		// Endure
		0,		// Charm
		30,		// Rollout
		40,		// False Swipe
		0,		// Swagger
		0,		// Milk Drink
		65,		// Spark
		20,		// Fury Cutter
		70,		// Steel Wing
		0,		// Mean Look
		0,		// Attract
		0,		// Sleep Talk
		0,		// Heal Bell
		variable_power,		// Return
		variable_power,		// Present
		variable_power,		// Frustration
		0,		// Safeguard
		0,		// Pain Split
		100,		// Sacred Fire
		variable_power,		// Magnitude
		100,		// DynamicPunch
		120,		// Megahorn
		60,		// DragonBreath
		0,		// Baton Pass
		0,		// Encore
		40,		// Pursuit
		20,		// Rapid Spin
		0,		// Sweet Scent
		100,		// Iron Tail
		50,		// Metal Claw
		70,		// Vital Throw
		0,		// Morning Sun
		0,		// Synthesis
		0,		// Moonlight
		variable_power,		// Hidden Power
		100,		// Cross Chop
		40,		// Twister
		0,		// Rain Dance
		0,		// Sunny Day
		80,		// Crunch
		indeterminate_power,		// Mirror Coat
		0,		// Psych Up
		80,		// ExtremeSpeed
		60,		// AncientPower
		80,		// Shadow Ball
		100,		// Future Sight
		40,		// Rock Smash
		35,		// Whirlpool
		variable_power,		// Beat Up
		40,		// Fake Out
		90,		// Uproar
		0,		// Stockpile
		variable_power,		// Spit Up
		0,		// Swallow
		100,		// Heat Wave
		0,		// Hail
		0,		// Torment
		0,		// Flatter
		0,		// Will-O-Wisp
		0,		// Memento
		70,		// Facade
		150,		// Focus Punch
		60,		// SmellingSalt
		0,		// Follow Me
		0,		// Nature Power
		0,		// Charge
		0,		// Taunt
		0,		// Helping Hand
		0,		// Trick
		0,		// Role Play
		0,		// Wish
		0,		// Assist
		0,		// Ingrain
		120,		// Superpower
		0,		// Magic Coat
		0,		// Recycle
		60,		// Revenge
		75,		// Brick Break
		0,		// Yawn
		20,		// Knock Off
		indeterminate_power,		// Endeavor
		150,		// Eruption
		0,		// Skill Swap
		0,		// Imprison
		0,		// Refresh
		0,		// Grudge
		0,		// Snatch
		70,		// Secret Power
		80,		// Dive
		15,		// Arm Thrust
		0,		// Camouflage
		0,		// Tail Glow
		70,		// Luster Purge
		70,		// Mist Ball
		0,		// FeatherDance
		0,		// Teeter Dance
		85,		// Blaze Kick
		0,		// Mud Sport
		30,		// Ice Ball
		60,		// Needle Arm
		0,		// Slack Off
		90,		// Hyper Voice
		50,		// Poison Fang
		75,		// Crush Claw
		150,		// Blast Burn
		150,		// Hydro Cannon
		100,		// Meteor Mash
		30,		// Astonish
		50,		// Weather Ball
		0,		// Aromatherapy
		0,		// Fake Tears
		55,		// Air Cutter
		140,		// Overheat
		0,		// Odor Sleuth
		50,		// Rock Tomb
		60,		// Silver Wind
		0,		// Metal Sound
		0,		// GrassWhistle
		0,		// Tickle
		0,		// Cosmic Power
		150,		// Water Spout
		75,		// Signal Beam
		60,		// Shadow Punch
		80,		// Extrasensory
		85,		// Sky Uppercut
		35,		// Sand Tomb
		indeterminate_power,		// Sheer Cold
		95,		// Muddy Water
		25,		// Bullet Seed
		60,		// Aerial Ace
		25,		// Icicle Spear
		0,		// Iron Defense
		0,		// Block
		0,		// Howl
		80,		// Dragon Claw
		150,		// Frenzy Plant
		0,		// Bulk Up
		85,		// Bounce
		55,		// Mud Shot
		50,		// Poison Tail
		60,		// Covet
		120,		// Volt Tackle
		60,		// Magical Leaf
		0,		// Water Sport
		0,		// Calm Mind
		90,		// Leaf Blade
		0,		// Dragon Dance
		25,		// Rock Blast
		60,		// Shock Wave
		60,		// Water Pulse
		140,		// Doom Desire
		140,		// Psycho Boost
		0,		// Roost
		0,		// Gravity
		0,		// Miracle Eye
		60,		// Wake-Up Slap
		100,		// Hammer Arm
		variable_power,		// Gyro Ball
		0,		// Healing Wish
		65,		// Brine
		variable_power,		// Natural Gift
		30,		// Feint
		60,		// Pluck
		0,		// Tailwind
		0,		// Acupressure
		indeterminate_power,		// Metal Burst
		70,		// U-turn
		120,		// Close Combat
		50,		// Payback
		50,		// Assurance
		0,		// Embargo
		variable_power,		// Fling
		0,		// Psycho Shift
		variable_power,		// Trump Card
		0,		// Heal Block
		variable_power,		// Wring Out
		0,		// Power Trick
		0,		// Gastro Acid
		0,		// Lucky Chant
		variable_power,		// Me First
		0,		// Copycat
		0,		// Power Swap
		0,		// Guard Swap
		variable_power,		// Punishment
		140,		// Last Resort
		0,		// Worry Seed
		80,		// Sucker Punch
		0,		// Toxic Spikes
		0,		// Heart Swap
		0,		// Aqua Ring
		0,		// Magnet Rise
		120,		// Flare Blitz
		60,		// Force Palm
		90,		// Aura Sphere
		0,		// Rock Polish
		80,		// Poison Jab
		80,		// Dark Pulse
		70,		// Night Slash
		90,		// Aqua Tail
		80,		// Seed Bomb
		75,		// Air Slash
		80,		// X-Scissor
		90,		// Bug Buzz
		90,		// Dragon Pulse
		100,		// Dragon Rush
		70,		// Power Gem
		75,		// Drain Punch
		40,		// Vacuum Wave
		120,		// Focus Blast
		80,		// Energy Ball
		120,		// Brave Bird
		90,		// Earth Power
		0,		// Switcheroo
		150,		// Giga Impact
		0,		// Nasty Plot
		40,		// Bullet Punch
		60,		// Avalanche
		40,		// Ice Shard
		70,		// Shadow Claw
		65,		// Thunder Fang
		65,		// Ice Fang
		65,		// Fire Fang
		40,		// Shadow Sneak
		65,		// Mud Bomb
		70,		// Psycho Cut
		80,		// Zen Headbutt
		65,		// Mirror Shot
		80,		// Flash Cannon
		90,		// Rock Climb
		0,		// Defog
		0,		// Trick Room
		140,		// Draco Meteor
		80,		// Discharge
		80,		// Lava Plume
		140,		// Leaf Storm
		120,		// Power Whip
		150,		// Rock Wrecker
		70,		// Cross Poison
		120,		// Gunk Shot
		80,		// Iron Head
		60,		// Magnet Bomb
		100,		// Stone Edge
		0,		// Captivate
		0,		// Stealth Rock
		variable_power,		// Grass Knot
		60,		// Chatter
		100,		// Judgment
		60,		// Bug Bite
		50,		// Charge Beam
		120,		// Wood Hammer
		40,		// Aqua Jet
		90,		// Attack Order
		0,		// Defend Order
		0,		// Heal Order
		150,		// Head Smash
		35,		// Double Hit
		150,		// Roar of Time
		100,		// Spacial Rend
		0,		// Lunar Dance
		variable_power,		// Crush Grip
		120,		// Magma Storm
		0,		// Dark Void
		120,		// Seed Flare
		60,		// Ominous Wind
		120,		// Shadow Force
		0,		// Hone Claws
		0,		// Wide Guard
		0,		// Guard Split
		0,		// Power Split
		0,		// Wonder Room
		80,		// Psyshock
		65,		// Venoshock
		0,		// Autotomize
		0,		// Rage Powder
		0,		// Telekinesis
		0,		// Magic Room
		50,		// Smack Down
		40,		// Storm Throw
		70,		// Flame Burst
		95,		// Sludge Wave
		0,		// Quiver Dance
		variable_power,		// Heavy Slam
		70,		// Synchronoise
		variable_power,		// Electro Ball
		0,		// Soak
		50,		// Flame Charge
		0,		// Coil
		60,		// Low Sweep
		40,		// Acid Spray
		95,		// Foul Play
		0,		// Simple Beam
		0,		// Entrainment
		0,		// After You
		60,		// Round
		40,		// Echoed Voice
		70,		// Chip Away
		50,		// Clear Smog
		20,		// Stored Power
		0,		// Quick Guard
		0,		// Ally Switch
		80,		// Scald
		0,		// Shell Smash
		0,		// Heal Pulse
		50,		// Hex
		60,		// Sky Drop
		0,		// Shift Gear
		60,		// Circle Throw
		30,		// Incinerate
		0,		// Quash
		55,		// Acrobatics
		0,		// Reflect Type
		70,		// Retaliate
		indeterminate_power,		// Final Gambit
		0,		// Bestow
		100,		// Inferno
		50,		// Water Pledge
		50,		// Fire Pledge
		50,		// Grass Pledge
		70,		// Volt Switch
		30,		// Struggle Bug
		60,		// Bulldoze
		40,		// Frost Breath
		60,		// Dragon Tail
		0,		// Work Up
		55,		// Electroweb
		90,		// Wild Charge
		80,		// Drill Run
		40,		// Dual Chop
		60,		// Heart Stamp
		75,		// Horn Leech
		90,		// Sacred Sword
		75,		// Razor Shell
		variable_power,		// Heat Crash
		65,		// Leaf Tornado
		65,		// Steamroller
		0,		// Cotton Guard
		85,		// Night Daze
		100,		// Psystrike
		25,		// Tail Slap
		120,		// Hurricane
		120,		// Head Charge
		50,		// Gear Grind
		100,		// Searing Shot
		85,		// Techno Blast
		75,		// Relic Song
		85,		// Secret Sword
		65,		// Glaciate
		130,		// Bolt Strike
		130,		// Blue Flare
		80,		// Fiery Dance
		140,		// Freeze Shock
		140,		// Ice Burn
		55,		// Snarl
		85,		// Icicle Crash
		180,		// V-create
		100,		// Fusion Flare
		100		// Fusion Bolt
	};
	return get_power [static_cast<size_t>(move)];
}

}	// unnamed namespace

std::string Move::to_string() const {
	return ::technicalmachine::to_string(name());
}

}	// namespace technicalmachine
