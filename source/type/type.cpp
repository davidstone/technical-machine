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
#include <stdexcept>

namespace technicalmachine {

auto is_strengthened_by_weather(Type const type, Weather const weather) -> bool {
	return (weather.rain() and type == Type::Water) or (weather.sun() and type == Type::Fire);
}

auto is_weakened_by_weather(Type const type, Weather const weather) -> bool {
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
		return value + ((get_stat(pokemon, pair.first).iv().value() % 2_bi) << pair.second);
	};
	intermediate_type const initial = get_hp(pokemon).iv().value() % 2_bi;
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

struct InvalidMove : std::exception {};

}	// namespace

auto get_type(Moves const move, Pokemon const & pokemon) -> Type {
	switch (move) {
		case Moves::Switch0: return Type::Typeless;
		case Moves::Switch1: return Type::Typeless;
		case Moves::Switch2: return Type::Typeless;
		case Moves::Switch3: return Type::Typeless;
		case Moves::Switch4: return Type::Typeless;
		case Moves::Switch5: return Type::Typeless;
		case Moves::Hit_Self: return Type::Typeless;
		case Moves::Pound: return Type::Normal;
		case Moves::Karate_Chop: return Type::Fighting;
		case Moves::DoubleSlap: return Type::Normal;
		case Moves::Comet_Punch: return Type::Normal;
		case Moves::Mega_Punch: return Type::Normal;
		case Moves::Pay_Day: return Type::Normal;
		case Moves::Fire_Punch: return Type::Fire;
		case Moves::Ice_Punch: return Type::Ice;
		case Moves::ThunderPunch: return Type::Electric;
		case Moves::Scratch: return Type::Normal;
		case Moves::ViceGrip: return Type::Normal;
		case Moves::Guillotine: return Type::Normal;
		case Moves::Razor_Wind: return Type::Normal;
		case Moves::Swords_Dance: return Type::Normal;
		case Moves::Cut: return Type::Normal;
		case Moves::Gust: return Type::Flying;
		case Moves::Wing_Attack: return Type::Flying;
		case Moves::Whirlwind: return Type::Normal;
		case Moves::Fly: return Type::Flying;
		case Moves::Bind: return Type::Normal;
		case Moves::Slam: return Type::Normal;
		case Moves::Vine_Whip: return Type::Grass;
		case Moves::Stomp: return Type::Normal;
		case Moves::Double_Kick: return Type::Fighting;
		case Moves::Mega_Kick: return Type::Normal;
		case Moves::Jump_Kick: return Type::Fighting;
		case Moves::Rolling_Kick: return Type::Fighting;
		case Moves::Sand_Attack: return Type::Ground;
		case Moves::Headbutt: return Type::Normal;
		case Moves::Horn_Attack: return Type::Normal;
		case Moves::Fury_Attack: return Type::Normal;
		case Moves::Horn_Drill: return Type::Normal;
		case Moves::Tackle: return Type::Normal;
		case Moves::Body_Slam: return Type::Normal;
		case Moves::Wrap: return Type::Normal;
		case Moves::Take_Down: return Type::Normal;
		case Moves::Thrash: return Type::Normal;
		case Moves::Double_Edge: return Type::Normal;
		case Moves::Tail_Whip: return Type::Normal;
		case Moves::Poison_Sting: return Type::Poison;
		case Moves::Twineedle: return Type::Bug;
		case Moves::Pin_Missile: return Type::Bug;
		case Moves::Leer: return Type::Normal;
		case Moves::Bite: return Type::Dark;
		case Moves::Growl: return Type::Normal;
		case Moves::Roar: return Type::Normal;
		case Moves::Sing: return Type::Normal;
		case Moves::Supersonic: return Type::Normal;
		case Moves::SonicBoom: return Type::Normal;
		case Moves::Disable: return Type::Normal;
		case Moves::Acid: return Type::Poison;
		case Moves::Ember: return Type::Fire;
		case Moves::Flamethrower: return Type::Fire;
		case Moves::Mist: return Type::Ice;
		case Moves::Water_Gun: return Type::Water;
		case Moves::Hydro_Pump: return Type::Water;
		case Moves::Surf: return Type::Water;
		case Moves::Ice_Beam: return Type::Ice;
		case Moves::Blizzard: return Type::Ice;
		case Moves::Psybeam: return Type::Psychic;
		case Moves::BubbleBeam: return Type::Water;
		case Moves::Aurora_Beam: return Type::Ice;
		case Moves::Hyper_Beam: return Type::Normal;
		case Moves::Peck: return Type::Flying;
		case Moves::Drill_Peck: return Type::Flying;
		case Moves::Submission: return Type::Fighting;
		case Moves::Low_Kick: return Type::Fighting;
		case Moves::Counter: return Type::Fighting;
		case Moves::Seismic_Toss: return Type::Fighting;
		case Moves::Strength: return Type::Normal;
		case Moves::Absorb: return Type::Grass;
		case Moves::Mega_Drain: return Type::Grass;
		case Moves::Leech_Seed: return Type::Grass;
		case Moves::Growth: return Type::Normal;
		case Moves::Razor_Leaf: return Type::Grass;
		case Moves::SolarBeam: return Type::Grass;
		case Moves::PoisonPowder: return Type::Poison;
		case Moves::Stun_Spore: return Type::Grass;
		case Moves::Sleep_Powder: return Type::Grass;
		case Moves::Petal_Dance: return Type::Grass;
		case Moves::String_Shot: return Type::Bug;
		case Moves::Dragon_Rage: return Type::Dragon;
		case Moves::Fire_Spin: return Type::Fire;
		case Moves::ThunderShock: return Type::Electric;
		case Moves::Thunderbolt: return Type::Electric;
		case Moves::Thunder_Wave: return Type::Electric;
		case Moves::Thunder: return Type::Electric;
		case Moves::Rock_Throw: return Type::Rock;
		case Moves::Earthquake: return Type::Ground;
		case Moves::Fissure: return Type::Ground;
		case Moves::Dig: return Type::Ground;
		case Moves::Toxic: return Type::Poison;
		case Moves::Confusion: return Type::Psychic;
		case Moves::Psychic: return Type::Psychic;
		case Moves::Hypnosis: return Type::Psychic;
		case Moves::Meditate: return Type::Psychic;
		case Moves::Agility: return Type::Psychic;
		case Moves::Quick_Attack: return Type::Normal;
		case Moves::Rage: return Type::Normal;
		case Moves::Teleport: return Type::Psychic;
		case Moves::Night_Shade: return Type::Ghost;
		case Moves::Mimic: return Type::Normal;
		case Moves::Screech: return Type::Normal;
		case Moves::Double_Team: return Type::Normal;
		case Moves::Recover: return Type::Normal;
		case Moves::Harden: return Type::Normal;
		case Moves::Minimize: return Type::Normal;
		case Moves::SmokeScreen: return Type::Normal;
		case Moves::Confuse_Ray: return Type::Ghost;
		case Moves::Withdraw: return Type::Water;
		case Moves::Defense_Curl: return Type::Normal;
		case Moves::Barrier: return Type::Psychic;
		case Moves::Light_Screen: return Type::Psychic;
		case Moves::Haze: return Type::Ice;
		case Moves::Reflect: return Type::Psychic;
		case Moves::Focus_Energy: return Type::Normal;
		case Moves::Bide: return Type::Normal;
		case Moves::Metronome: return Type::Normal;
		case Moves::Mirror_Move: return Type::Flying;
		case Moves::Selfdestruct: return Type::Normal;
		case Moves::Egg_Bomb: return Type::Normal;
		case Moves::Lick: return Type::Ghost;
		case Moves::Smog: return Type::Poison;
		case Moves::Sludge: return Type::Poison;
		case Moves::Bone_Club: return Type::Ground;
		case Moves::Fire_Blast: return Type::Fire;
		case Moves::Waterfall: return Type::Water;
		case Moves::Clamp: return Type::Water;
		case Moves::Swift: return Type::Normal;
		case Moves::Skull_Bash: return Type::Normal;
		case Moves::Spike_Cannon: return Type::Normal;
		case Moves::Constrict: return Type::Normal;
		case Moves::Amnesia: return Type::Psychic;
		case Moves::Kinesis: return Type::Psychic;
		case Moves::Softboiled: return Type::Normal;
		case Moves::Hi_Jump_Kick: return Type::Fighting;
		case Moves::Glare: return Type::Normal;
		case Moves::Dream_Eater: return Type::Psychic;
		case Moves::Poison_Gas: return Type::Poison;
		case Moves::Barrage: return Type::Normal;
		case Moves::Leech_Life: return Type::Bug;
		case Moves::Lovely_Kiss: return Type::Normal;
		case Moves::Sky_Attack: return Type::Flying;
		case Moves::Transform: return Type::Normal;
		case Moves::Bubble: return Type::Water;
		case Moves::Dizzy_Punch: return Type::Normal;
		case Moves::Spore: return Type::Grass;
		case Moves::Flash: return Type::Normal;
		case Moves::Psywave: return Type::Psychic;
		case Moves::Splash: return Type::Normal;
		case Moves::Acid_Armor: return Type::Poison;
		case Moves::Crabhammer: return Type::Water;
		case Moves::Explosion: return Type::Normal;
		case Moves::Fury_Swipes: return Type::Normal;
		case Moves::Bonemerang: return Type::Ground;
		case Moves::Rest: return Type::Psychic;
		case Moves::Rock_Slide: return Type::Rock;
		case Moves::Hyper_Fang: return Type::Normal;
		case Moves::Sharpen: return Type::Normal;
		case Moves::Conversion: return Type::Normal;
		case Moves::Tri_Attack: return Type::Normal;
		case Moves::Super_Fang: return Type::Normal;
		case Moves::Slash: return Type::Normal;
		case Moves::Substitute: return Type::Normal;
		case Moves::Struggle: return Type::Typeless;
		case Moves::Sketch: return Type::Normal;
		case Moves::Triple_Kick: return Type::Fighting;
		case Moves::Thief: return Type::Dark;
		case Moves::Spider_Web: return Type::Bug;
		case Moves::Mind_Reader: return Type::Normal;
		case Moves::Nightmare: return Type::Ghost;
		case Moves::Flame_Wheel: return Type::Fire;
		case Moves::Snore: return Type::Normal;
		case Moves::Curse: return Type::Ghost;
		case Moves::Flail: return Type::Normal;
		case Moves::Conversion_2: return Type::Normal;
		case Moves::Aeroblast: return Type::Flying;
		case Moves::Cotton_Spore: return Type::Grass;
		case Moves::Reversal: return Type::Fighting;
		case Moves::Spite: return Type::Ghost;
		case Moves::Powder_Snow: return Type::Ice;
		case Moves::Protect: return Type::Normal;
		case Moves::Mach_Punch: return Type::Fighting;
		case Moves::Scary_Face: return Type::Normal;
		case Moves::Faint_Attack: return Type::Dark;
		case Moves::Sweet_Kiss: return Type::Normal;
		case Moves::Belly_Drum: return Type::Normal;
		case Moves::Sludge_Bomb: return Type::Poison;
		case Moves::Mud_Slap: return Type::Ground;
		case Moves::Octazooka: return Type::Water;
		case Moves::Spikes: return Type::Ground;
		case Moves::Zap_Cannon: return Type::Electric;
		case Moves::Foresight: return Type::Normal;
		case Moves::Destiny_Bond: return Type::Ghost;
		case Moves::Perish_Song: return Type::Normal;
		case Moves::Icy_Wind: return Type::Ice;
		case Moves::Detect: return Type::Fighting;
		case Moves::Bone_Rush: return Type::Ground;
		case Moves::Lock_On: return Type::Normal;
		case Moves::Outrage: return Type::Dragon;
		case Moves::Sandstorm: return Type::Rock;
		case Moves::Giga_Drain: return Type::Grass;
		case Moves::Endure: return Type::Normal;
		case Moves::Charm: return Type::Normal;
		case Moves::Rollout: return Type::Rock;
		case Moves::False_Swipe: return Type::Normal;
		case Moves::Swagger: return Type::Normal;
		case Moves::Milk_Drink: return Type::Normal;
		case Moves::Spark: return Type::Electric;
		case Moves::Fury_Cutter: return Type::Bug;
		case Moves::Steel_Wing: return Type::Steel;
		case Moves::Mean_Look: return Type::Normal;
		case Moves::Attract: return Type::Normal;
		case Moves::Sleep_Talk: return Type::Normal;
		case Moves::Heal_Bell: return Type::Normal;
		case Moves::Return: return Type::Normal;
		case Moves::Present: return Type::Normal;
		case Moves::Frustration: return Type::Normal;
		case Moves::Safeguard: return Type::Normal;
		case Moves::Pain_Split: return Type::Normal;
		case Moves::Sacred_Fire: return Type::Fire;
		case Moves::Magnitude: return Type::Ground;
		case Moves::DynamicPunch: return Type::Fighting;
		case Moves::Megahorn: return Type::Bug;
		case Moves::DragonBreath: return Type::Dragon;
		case Moves::Baton_Pass: return Type::Normal;
		case Moves::Encore: return Type::Normal;
		case Moves::Pursuit: return Type::Dark;
		case Moves::Rapid_Spin: return Type::Normal;
		case Moves::Sweet_Scent: return Type::Normal;
		case Moves::Iron_Tail: return Type::Steel;
		case Moves::Metal_Claw: return Type::Steel;
		case Moves::Vital_Throw: return Type::Fighting;
		case Moves::Morning_Sun: return Type::Normal;
		case Moves::Synthesis: return Type::Grass;
		case Moves::Moonlight: return Type::Normal;
		case Moves::Hidden_Power: return hidden_power_type(pokemon);
		case Moves::Cross_Chop: return Type::Fighting;
		case Moves::Twister: return Type::Dragon;
		case Moves::Rain_Dance: return Type::Water;
		case Moves::Sunny_Day: return Type::Fire;
		case Moves::Crunch: return Type::Dark;
		case Moves::Mirror_Coat: return Type::Psychic;
		case Moves::Psych_Up: return Type::Normal;
		case Moves::ExtremeSpeed: return Type::Normal;
		case Moves::AncientPower: return Type::Rock;
		case Moves::Shadow_Ball: return Type::Ghost;
		case Moves::Future_Sight: return Type::Psychic;
		case Moves::Rock_Smash: return Type::Fighting;
		case Moves::Whirlpool: return Type::Water;
		case Moves::Beat_Up: return Type::Dark;
		case Moves::Fake_Out: return Type::Normal;
		case Moves::Uproar: return Type::Normal;
		case Moves::Stockpile: return Type::Normal;
		case Moves::Spit_Up: return Type::Normal;
		case Moves::Swallow: return Type::Normal;
		case Moves::Heat_Wave: return Type::Fire;
		case Moves::Hail: return Type::Ice;
		case Moves::Torment: return Type::Dark;
		case Moves::Flatter: return Type::Dark;
		case Moves::Will_O_Wisp: return Type::Fire;
		case Moves::Memento: return Type::Dark;
		case Moves::Facade: return Type::Normal;
		case Moves::Focus_Punch: return Type::Fighting;
		case Moves::SmellingSalt: return Type::Normal;
		case Moves::Follow_Me: return Type::Normal;
		case Moves::Nature_Power: return Type::Normal;
		case Moves::Charge: return Type::Electric;
		case Moves::Taunt: return Type::Dark;
		case Moves::Helping_Hand: return Type::Normal;
		case Moves::Trick: return Type::Psychic;
		case Moves::Role_Play: return Type::Psychic;
		case Moves::Wish: return Type::Normal;
		case Moves::Assist: return Type::Normal;
		case Moves::Ingrain: return Type::Grass;
		case Moves::Superpower: return Type::Fighting;
		case Moves::Magic_Coat: return Type::Psychic;
		case Moves::Recycle: return Type::Normal;
		case Moves::Revenge: return Type::Fighting;
		case Moves::Brick_Break: return Type::Fighting;
		case Moves::Yawn: return Type::Normal;
		case Moves::Knock_Off: return Type::Dark;
		case Moves::Endeavor: return Type::Normal;
		case Moves::Eruption: return Type::Fire;
		case Moves::Skill_Swap: return Type::Psychic;
		case Moves::Imprison: return Type::Psychic;
		case Moves::Refresh: return Type::Normal;
		case Moves::Grudge: return Type::Ghost;
		case Moves::Snatch: return Type::Dark;
		case Moves::Secret_Power: return Type::Normal;
		case Moves::Dive: return Type::Water;
		case Moves::Arm_Thrust: return Type::Fighting;
		case Moves::Camouflage: return Type::Normal;
		case Moves::Tail_Glow: return Type::Bug;
		case Moves::Luster_Purge: return Type::Psychic;
		case Moves::Mist_Ball: return Type::Psychic;
		case Moves::FeatherDance: return Type::Flying;
		case Moves::Teeter_Dance: return Type::Normal;
		case Moves::Blaze_Kick: return Type::Fire;
		case Moves::Mud_Sport: return Type::Ground;
		case Moves::Ice_Ball: return Type::Ice;
		case Moves::Needle_Arm: return Type::Grass;
		case Moves::Slack_Off: return Type::Normal;
		case Moves::Hyper_Voice: return Type::Normal;
		case Moves::Poison_Fang: return Type::Poison;
		case Moves::Crush_Claw: return Type::Normal;
		case Moves::Blast_Burn: return Type::Fire;
		case Moves::Hydro_Cannon: return Type::Water;
		case Moves::Meteor_Mash: return Type::Steel;
		case Moves::Astonish: return Type::Ghost;
		case Moves::Weather_Ball: return Type::Normal;
		case Moves::Aromatherapy: return Type::Grass;
		case Moves::Fake_Tears: return Type::Dark;
		case Moves::Air_Cutter: return Type::Flying;
		case Moves::Overheat: return Type::Fire;
		case Moves::Odor_Sleuth: return Type::Normal;
		case Moves::Rock_Tomb: return Type::Rock;
		case Moves::Silver_Wind: return Type::Bug;
		case Moves::Metal_Sound: return Type::Steel;
		case Moves::GrassWhistle: return Type::Grass;
		case Moves::Tickle: return Type::Normal;
		case Moves::Cosmic_Power: return Type::Psychic;
		case Moves::Water_Spout: return Type::Water;
		case Moves::Signal_Beam: return Type::Bug;
		case Moves::Shadow_Punch: return Type::Ghost;
		case Moves::Extrasensory: return Type::Psychic;
		case Moves::Sky_Uppercut: return Type::Fighting;
		case Moves::Sand_Tomb: return Type::Ground;
		case Moves::Sheer_Cold: return Type::Ice;
		case Moves::Muddy_Water: return Type::Water;
		case Moves::Bullet_Seed: return Type::Grass;
		case Moves::Aerial_Ace: return Type::Flying;
		case Moves::Icicle_Spear: return Type::Ice;
		case Moves::Iron_Defense: return Type::Steel;
		case Moves::Block: return Type::Normal;
		case Moves::Howl: return Type::Normal;
		case Moves::Dragon_Claw: return Type::Dragon;
		case Moves::Frenzy_Plant: return Type::Grass;
		case Moves::Bulk_Up: return Type::Fighting;
		case Moves::Bounce: return Type::Flying;
		case Moves::Mud_Shot: return Type::Ground;
		case Moves::Poison_Tail: return Type::Poison;
		case Moves::Covet: return Type::Normal;
		case Moves::Volt_Tackle: return Type::Electric;
		case Moves::Magical_Leaf: return Type::Grass;
		case Moves::Water_Sport: return Type::Water;
		case Moves::Calm_Mind: return Type::Psychic;
		case Moves::Leaf_Blade: return Type::Grass;
		case Moves::Dragon_Dance: return Type::Dragon;
		case Moves::Rock_Blast: return Type::Rock;
		case Moves::Shock_Wave: return Type::Electric;
		case Moves::Water_Pulse: return Type::Water;
		case Moves::Doom_Desire: return Type::Steel;
		case Moves::Psycho_Boost: return Type::Psychic;
		case Moves::Roost: return Type::Flying;
		case Moves::Gravity: return Type::Psychic;
		case Moves::Miracle_Eye: return Type::Psychic;
		case Moves::Wake_Up_Slap: return Type::Fighting;
		case Moves::Hammer_Arm: return Type::Fighting;
		case Moves::Gyro_Ball: return Type::Steel;
		case Moves::Healing_Wish: return Type::Psychic;
		case Moves::Brine: return Type::Water;
		case Moves::Natural_Gift: return Type::Normal;
		case Moves::Feint: return Type::Normal;
		case Moves::Pluck: return Type::Flying;
		case Moves::Tailwind: return Type::Flying;
		case Moves::Acupressure: return Type::Normal;
		case Moves::Metal_Burst: return Type::Steel;
		case Moves::U_turn: return Type::Bug;
		case Moves::Close_Combat: return Type::Fighting;
		case Moves::Payback: return Type::Dark;
		case Moves::Assurance: return Type::Dark;
		case Moves::Embargo: return Type::Dark;
		case Moves::Fling: return Type::Dark;
		case Moves::Psycho_Shift: return Type::Psychic;
		case Moves::Trump_Card: return Type::Normal;
		case Moves::Heal_Block: return Type::Psychic;
		case Moves::Wring_Out: return Type::Normal;
		case Moves::Power_Trick: return Type::Psychic;
		case Moves::Gastro_Acid: return Type::Poison;
		case Moves::Lucky_Chant: return Type::Normal;
		case Moves::Me_First: return Type::Normal;
		case Moves::Copycat: return Type::Normal;
		case Moves::Power_Swap: return Type::Psychic;
		case Moves::Guard_Swap: return Type::Psychic;
		case Moves::Punishment: return Type::Dark;
		case Moves::Last_Resort: return Type::Normal;
		case Moves::Worry_Seed: return Type::Grass;
		case Moves::Sucker_Punch: return Type::Dark;
		case Moves::Toxic_Spikes: return Type::Poison;
		case Moves::Heart_Swap: return Type::Psychic;
		case Moves::Aqua_Ring: return Type::Water;
		case Moves::Magnet_Rise: return Type::Electric;
		case Moves::Flare_Blitz: return Type::Fire;
		case Moves::Force_Palm: return Type::Fighting;
		case Moves::Aura_Sphere: return Type::Fighting;
		case Moves::Rock_Polish: return Type::Rock;
		case Moves::Poison_Jab: return Type::Poison;
		case Moves::Dark_Pulse: return Type::Dark;
		case Moves::Night_Slash: return Type::Dark;
		case Moves::Aqua_Tail: return Type::Water;
		case Moves::Seed_Bomb: return Type::Grass;
		case Moves::Air_Slash: return Type::Flying;
		case Moves::X_Scissor: return Type::Bug;
		case Moves::Bug_Buzz: return Type::Bug;
		case Moves::Dragon_Pulse: return Type::Dragon;
		case Moves::Dragon_Rush: return Type::Dragon;
		case Moves::Power_Gem: return Type::Rock;
		case Moves::Drain_Punch: return Type::Fighting;
		case Moves::Vacuum_Wave: return Type::Fighting;
		case Moves::Focus_Blast: return Type::Fighting;
		case Moves::Energy_Ball: return Type::Grass;
		case Moves::Brave_Bird: return Type::Flying;
		case Moves::Earth_Power: return Type::Ground;
		case Moves::Switcheroo: return Type::Dark;
		case Moves::Giga_Impact: return Type::Normal;
		case Moves::Nasty_Plot: return Type::Dark;
		case Moves::Bullet_Punch: return Type::Steel;
		case Moves::Avalanche: return Type::Ice;
		case Moves::Ice_Shard: return Type::Ice;
		case Moves::Shadow_Claw: return Type::Ghost;
		case Moves::Thunder_Fang: return Type::Electric;
		case Moves::Ice_Fang: return Type::Ice;
		case Moves::Fire_Fang: return Type::Fire;
		case Moves::Shadow_Sneak: return Type::Ghost;
		case Moves::Mud_Bomb: return Type::Ground;
		case Moves::Psycho_Cut: return Type::Psychic;
		case Moves::Zen_Headbutt: return Type::Psychic;
		case Moves::Mirror_Shot: return Type::Steel;
		case Moves::Flash_Cannon: return Type::Steel;
		case Moves::Rock_Climb: return Type::Normal;
		case Moves::Defog: return Type::Flying;
		case Moves::Trick_Room: return Type::Psychic;
		case Moves::Draco_Meteor: return Type::Dragon;
		case Moves::Discharge: return Type::Electric;
		case Moves::Lava_Plume: return Type::Fire;
		case Moves::Leaf_Storm: return Type::Grass;
		case Moves::Power_Whip: return Type::Grass;
		case Moves::Rock_Wrecker: return Type::Rock;
		case Moves::Cross_Poison: return Type::Poison;
		case Moves::Gunk_Shot: return Type::Poison;
		case Moves::Iron_Head: return Type::Steel;
		case Moves::Magnet_Bomb: return Type::Steel;
		case Moves::Stone_Edge: return Type::Rock;
		case Moves::Captivate: return Type::Normal;
		case Moves::Stealth_Rock: return Type::Rock;
		case Moves::Grass_Knot: return Type::Grass;
		case Moves::Chatter: return Type::Flying;
		case Moves::Judgment: return Type::Normal;
		case Moves::Bug_Bite: return Type::Bug;
		case Moves::Charge_Beam: return Type::Electric;
		case Moves::Wood_Hammer: return Type::Grass;
		case Moves::Aqua_Jet: return Type::Water;
		case Moves::Attack_Order: return Type::Bug;
		case Moves::Defend_Order: return Type::Bug;
		case Moves::Heal_Order: return Type::Bug;
		case Moves::Head_Smash: return Type::Rock;
		case Moves::Double_Hit: return Type::Normal;
		case Moves::Roar_of_Time: return Type::Dragon;
		case Moves::Spacial_Rend: return Type::Dragon;
		case Moves::Lunar_Dance: return Type::Psychic;
		case Moves::Crush_Grip: return Type::Normal;
		case Moves::Magma_Storm: return Type::Fire;
		case Moves::Dark_Void: return Type::Dark;
		case Moves::Seed_Flare: return Type::Grass;
		case Moves::Ominous_Wind: return Type::Ghost;
		case Moves::Shadow_Force: return Type::Ghost;
		case Moves::Hone_Claws: return Type::Dark;
		case Moves::Wide_Guard: return Type::Rock;
		case Moves::Guard_Split: return Type::Psychic;
		case Moves::Power_Split: return Type::Psychic;
		case Moves::Wonder_Room: return Type::Psychic;
		case Moves::Psyshock: return Type::Psychic;
		case Moves::Venoshock: return Type::Poison;
		case Moves::Autotomize: return Type::Steel;
		case Moves::Rage_Powder: return Type::Bug;
		case Moves::Telekinesis: return Type::Psychic;
		case Moves::Magic_Room: return Type::Psychic;
		case Moves::Smack_Down: return Type::Rock;
		case Moves::Storm_Throw: return Type::Fighting;
		case Moves::Flame_Burst: return Type::Fire;
		case Moves::Sludge_Wave: return Type::Poison;
		case Moves::Quiver_Dance: return Type::Bug;
		case Moves::Heavy_Slam: return Type::Steel;
		case Moves::Synchronoise: return Type::Psychic;
		case Moves::Electro_Ball: return Type::Electric;
		case Moves::Soak: return Type::Water;
		case Moves::Flame_Charge: return Type::Fire;
		case Moves::Coil: return Type::Poison;
		case Moves::Low_Sweep: return Type::Fighting;
		case Moves::Acid_Spray: return Type::Poison;
		case Moves::Foul_Play: return Type::Dark;
		case Moves::Simple_Beam: return Type::Normal;
		case Moves::Entrainment: return Type::Normal;
		case Moves::After_You: return Type::Normal;
		case Moves::Round: return Type::Normal;
		case Moves::Echoed_Voice: return Type::Normal;
		case Moves::Chip_Away: return Type::Normal;
		case Moves::Clear_Smog: return Type::Poison;
		case Moves::Stored_Power: return Type::Psychic;
		case Moves::Quick_Guard: return Type::Fighting;
		case Moves::Ally_Switch: return Type::Psychic;
		case Moves::Scald: return Type::Water;
		case Moves::Shell_Smash: return Type::Normal;
		case Moves::Heal_Pulse: return Type::Psychic;
		case Moves::Hex: return Type::Ghost;
		case Moves::Sky_Drop: return Type::Flying;
		case Moves::Shift_Gear: return Type::Steel;
		case Moves::Circle_Throw: return Type::Fighting;
		case Moves::Incinerate: return Type::Fire;
		case Moves::Quash: return Type::Dark;
		case Moves::Acrobatics: return Type::Flying;
		case Moves::Reflect_Type: return Type::Normal;
		case Moves::Retaliate: return Type::Normal;
		case Moves::Final_Gambit: return Type::Fighting;
		case Moves::Bestow: return Type::Normal;
		case Moves::Inferno: return Type::Fire;
		case Moves::Water_Pledge: return Type::Water;
		case Moves::Fire_Pledge: return Type::Fire;
		case Moves::Grass_Pledge: return Type::Grass;
		case Moves::Volt_Switch: return Type::Electric;
		case Moves::Struggle_Bug: return Type::Bug;
		case Moves::Bulldoze: return Type::Ground;
		case Moves::Frost_Breath: return Type::Ice;
		case Moves::Dragon_Tail: return Type::Dragon;
		case Moves::Work_Up: return Type::Normal;
		case Moves::Electroweb: return Type::Electric;
		case Moves::Wild_Charge: return Type::Electric;
		case Moves::Drill_Run: return Type::Ground;
		case Moves::Dual_Chop: return Type::Dragon;
		case Moves::Heart_Stamp: return Type::Psychic;
		case Moves::Horn_Leech: return Type::Grass;
		case Moves::Sacred_Sword: return Type::Fighting;
		case Moves::Razor_Shell: return Type::Water;
		case Moves::Heat_Crash: return Type::Fire;
		case Moves::Leaf_Tornado: return Type::Grass;
		case Moves::Steamroller: return Type::Bug;
		case Moves::Cotton_Guard: return Type::Grass;
		case Moves::Night_Daze: return Type::Dark;
		case Moves::Psystrike: return Type::Psychic;
		case Moves::Tail_Slap: return Type::Normal;
		case Moves::Hurricane: return Type::Flying;
		case Moves::Head_Charge: return Type::Normal;
		case Moves::Gear_Grind: return Type::Steel;
		case Moves::Searing_Shot: return Type::Fire;
		case Moves::Techno_Blast: return Type::Normal;
		case Moves::Relic_Song: return Type::Normal;
		case Moves::Secret_Sword: return Type::Fighting;
		case Moves::Glaciate: return Type::Ice;
		case Moves::Bolt_Strike: return Type::Electric;
		case Moves::Blue_Flare: return Type::Fire;
		case Moves::Fiery_Dance: return Type::Fire;
		case Moves::Freeze_Shock: return Type::Ice;
		case Moves::Ice_Burn: return Type::Ice;
		case Moves::Snarl: return Type::Dark;
		case Moves::Icicle_Crash: return Type::Ice;
		case Moves::V_create: return Type::Fire;
		case Moves::Fusion_Flare: return Type::Fire;
		case Moves::Fusion_Bolt: return Type::Electric;
		default : throw InvalidMove{};
	}
}

}	// namespace technicalmachine
