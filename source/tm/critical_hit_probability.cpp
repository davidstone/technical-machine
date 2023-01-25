// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <tm/for_each_generation.hpp>

#include <bounded/conditional.hpp>

export module tm.critical_hit_probability;

import tm.move.move_name;

import tm.pokemon.active_pokemon;
import tm.pokemon.any_pokemon;
import tm.pokemon.species;
import tm.pokemon.substitute;

import tm.stat.base_stats;

import tm.rational;
import tm.ability;
import tm.generation;
import tm.item;
import tm.weather;

import bounded;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

enum class MoveCriticalHit {
	never,
	normal,
	high,
	very_high,
	always,
};

constexpr auto move_critical_hit(Generation const generation, MoveName const move_name) {
	switch (move_name) {
		// Weird moves
		case MoveName::Pass: return MoveCriticalHit::never;
		case MoveName::Switch0: return MoveCriticalHit::never;
		case MoveName::Switch1: return MoveCriticalHit::never;
		case MoveName::Switch2: return MoveCriticalHit::never;
		case MoveName::Switch3: return MoveCriticalHit::never;
		case MoveName::Switch4: return MoveCriticalHit::never;
		case MoveName::Switch5: return MoveCriticalHit::never;
		case MoveName::Hit_Self: return MoveCriticalHit::never;

		// Generation 1
		case MoveName::Pound: return MoveCriticalHit::normal;
		case MoveName::Karate_Chop: return MoveCriticalHit::high;
		case MoveName::Double_Slap: return MoveCriticalHit::normal;
		case MoveName::Comet_Punch: return MoveCriticalHit::normal;
		case MoveName::Mega_Punch: return MoveCriticalHit::normal;
		case MoveName::Pay_Day: return MoveCriticalHit::normal;
		case MoveName::Fire_Punch: return MoveCriticalHit::normal;
		case MoveName::Ice_Punch: return MoveCriticalHit::normal;
		case MoveName::Thunder_Punch: return MoveCriticalHit::normal;
		case MoveName::Scratch: return MoveCriticalHit::normal;
		case MoveName::Vise_Grip: return MoveCriticalHit::normal;
		case MoveName::Guillotine: return MoveCriticalHit::never;
		case MoveName::Razor_Wind:
			switch (generation) {
				case Generation::one: return MoveCriticalHit::normal;
				case Generation::two: return MoveCriticalHit::high;
				case Generation::three: return MoveCriticalHit::normal;
				case Generation::four: return MoveCriticalHit::high;
				case Generation::five: return MoveCriticalHit::high;
				case Generation::six: return MoveCriticalHit::high;
				case Generation::seven: return MoveCriticalHit::high;
				case Generation::eight: return MoveCriticalHit::high;
			}
		case MoveName::Swords_Dance: return MoveCriticalHit::never;
		case MoveName::Cut: return MoveCriticalHit::normal;
		case MoveName::Gust: return MoveCriticalHit::normal;
		case MoveName::Wing_Attack: return MoveCriticalHit::normal;
		case MoveName::Whirlwind: return MoveCriticalHit::never;
		case MoveName::Fly: return MoveCriticalHit::normal;
		case MoveName::Bind: return MoveCriticalHit::normal;
		case MoveName::Slam: return MoveCriticalHit::normal;
		case MoveName::Vine_Whip: return MoveCriticalHit::normal;
		case MoveName::Stomp: return MoveCriticalHit::normal;
		case MoveName::Double_Kick: return MoveCriticalHit::normal;
		case MoveName::Mega_Kick: return MoveCriticalHit::normal;
		case MoveName::Jump_Kick: return MoveCriticalHit::normal;
		case MoveName::Rolling_Kick: return MoveCriticalHit::normal;
		case MoveName::Sand_Attack: return MoveCriticalHit::normal;
		case MoveName::Headbutt: return MoveCriticalHit::normal;
		case MoveName::Horn_Attack: return MoveCriticalHit::normal;
		case MoveName::Fury_Attack: return MoveCriticalHit::normal;
		case MoveName::Horn_Drill: return MoveCriticalHit::never;
		case MoveName::Tackle: return MoveCriticalHit::normal;
		case MoveName::Body_Slam: return MoveCriticalHit::normal;
		case MoveName::Wrap: return MoveCriticalHit::normal;
		case MoveName::Take_Down: return MoveCriticalHit::normal;
		case MoveName::Thrash: return MoveCriticalHit::normal;
		case MoveName::Double_Edge: return MoveCriticalHit::normal;
		case MoveName::Tail_Whip: return MoveCriticalHit::never;
		case MoveName::Poison_Sting: return MoveCriticalHit::normal;
		case MoveName::Twineedle: return MoveCriticalHit::normal;
		case MoveName::Pin_Missile: return MoveCriticalHit::normal;
		case MoveName::Leer: return MoveCriticalHit::never;
		case MoveName::Bite: return MoveCriticalHit::normal;
		case MoveName::Growl: return MoveCriticalHit::never;
		case MoveName::Roar: return MoveCriticalHit::never;
		case MoveName::Sing: return MoveCriticalHit::never;
		case MoveName::Supersonic: return MoveCriticalHit::never;
		case MoveName::Sonic_Boom: return MoveCriticalHit::never;
		case MoveName::Disable: return MoveCriticalHit::never;
		case MoveName::Acid: return MoveCriticalHit::normal;
		case MoveName::Ember: return MoveCriticalHit::normal;
		case MoveName::Flamethrower: return MoveCriticalHit::normal;
		case MoveName::Mist: return MoveCriticalHit::never;
		case MoveName::Water_Gun: return MoveCriticalHit::normal;
		case MoveName::Hydro_Pump: return MoveCriticalHit::normal;
		case MoveName::Surf: return MoveCriticalHit::normal;
		case MoveName::Ice_Beam: return MoveCriticalHit::normal;
		case MoveName::Blizzard: return MoveCriticalHit::normal;
		case MoveName::Psybeam: return MoveCriticalHit::normal;
		case MoveName::Bubble_Beam: return MoveCriticalHit::normal;
		case MoveName::Aurora_Beam: return MoveCriticalHit::normal;
		case MoveName::Hyper_Beam: return MoveCriticalHit::normal;
		case MoveName::Peck: return MoveCriticalHit::normal;
		case MoveName::Drill_Peck: return MoveCriticalHit::normal;
		case MoveName::Submission: return MoveCriticalHit::normal;
		case MoveName::Low_Kick: return MoveCriticalHit::normal;
		case MoveName::Counter: return MoveCriticalHit::never;
		case MoveName::Seismic_Toss: return MoveCriticalHit::never;
		case MoveName::Strength: return MoveCriticalHit::normal;
		case MoveName::Absorb: return MoveCriticalHit::normal;
		case MoveName::Mega_Drain: return MoveCriticalHit::normal;
		case MoveName::Leech_Seed: return MoveCriticalHit::normal;
		case MoveName::Growth: return MoveCriticalHit::never;
		case MoveName::Razor_Leaf: return MoveCriticalHit::high;
		case MoveName::Solar_Beam: return MoveCriticalHit::normal;
		case MoveName::Poison_Powder: return MoveCriticalHit::never;
		case MoveName::Stun_Spore: return MoveCriticalHit::never;
		case MoveName::Sleep_Powder: return MoveCriticalHit::never;
		case MoveName::Petal_Dance: return MoveCriticalHit::normal;
		case MoveName::String_Shot: return MoveCriticalHit::never;
		case MoveName::Dragon_Rage: return MoveCriticalHit::never;
		case MoveName::Fire_Spin: return MoveCriticalHit::normal;
		case MoveName::Thunder_Shock: return MoveCriticalHit::normal;
		case MoveName::Thunderbolt: return MoveCriticalHit::normal;
		case MoveName::Thunder_Wave: return MoveCriticalHit::never;
		case MoveName::Thunder: return MoveCriticalHit::normal;
		case MoveName::Rock_Throw: return MoveCriticalHit::normal;
		case MoveName::Earthquake: return MoveCriticalHit::normal;
		case MoveName::Fissure: return MoveCriticalHit::never;
		case MoveName::Dig: return MoveCriticalHit::normal;
		case MoveName::Toxic: return MoveCriticalHit::never;
		case MoveName::Confusion: return MoveCriticalHit::normal;
		case MoveName::Psychic: return MoveCriticalHit::normal;
		case MoveName::Hypnosis: return MoveCriticalHit::never;
		case MoveName::Meditate: return MoveCriticalHit::never;
		case MoveName::Agility: return MoveCriticalHit::never;
		case MoveName::Quick_Attack: return MoveCriticalHit::normal;
		case MoveName::Rage: return MoveCriticalHit::normal;
		case MoveName::Teleport: return MoveCriticalHit::never;
		case MoveName::Night_Shade: return MoveCriticalHit::never;
		case MoveName::Mimic: return MoveCriticalHit::never;
		case MoveName::Screech: return MoveCriticalHit::never;
		case MoveName::Double_Team: return MoveCriticalHit::never;
		case MoveName::Recover: return MoveCriticalHit::never;
		case MoveName::Harden: return MoveCriticalHit::never;
		case MoveName::Minimize: return MoveCriticalHit::never;
		case MoveName::Smokescreen: return MoveCriticalHit::never;
		case MoveName::Confuse_Ray: return MoveCriticalHit::never;
		case MoveName::Withdraw: return MoveCriticalHit::never;
		case MoveName::Defense_Curl: return MoveCriticalHit::never;
		case MoveName::Barrier: return MoveCriticalHit::never;
		case MoveName::Light_Screen: return MoveCriticalHit::never;
		case MoveName::Haze: return MoveCriticalHit::never;
		case MoveName::Reflect: return MoveCriticalHit::never;
		case MoveName::Focus_Energy: return MoveCriticalHit::never;
		case MoveName::Bide: return MoveCriticalHit::never;
		case MoveName::Metronome: return MoveCriticalHit::never;
		case MoveName::Mirror_Move: return MoveCriticalHit::never;
		case MoveName::Self_Destruct: return MoveCriticalHit::normal;
		case MoveName::Egg_Bomb: return MoveCriticalHit::normal;
		case MoveName::Lick: return MoveCriticalHit::normal;
		case MoveName::Smog: return MoveCriticalHit::normal;
		case MoveName::Sludge: return MoveCriticalHit::normal;
		case MoveName::Bone_Club: return MoveCriticalHit::normal;
		case MoveName::Fire_Blast: return MoveCriticalHit::normal;
		case MoveName::Waterfall: return MoveCriticalHit::normal;
		case MoveName::Clamp: return MoveCriticalHit::normal;
		case MoveName::Swift: return MoveCriticalHit::normal;
		case MoveName::Skull_Bash: return MoveCriticalHit::normal;
		case MoveName::Spike_Cannon: return MoveCriticalHit::normal;
		case MoveName::Constrict: return MoveCriticalHit::normal;
		case MoveName::Amnesia: return MoveCriticalHit::never;
		case MoveName::Kinesis: return MoveCriticalHit::never;
		case MoveName::Soft_Boiled: return MoveCriticalHit::never;
		case MoveName::High_Jump_Kick: return MoveCriticalHit::normal;
		case MoveName::Glare: return MoveCriticalHit::never;
		case MoveName::Dream_Eater: return MoveCriticalHit::normal;
		case MoveName::Poison_Gas: return MoveCriticalHit::never;
		case MoveName::Barrage: return MoveCriticalHit::normal;
		case MoveName::Leech_Life: return MoveCriticalHit::normal;
		case MoveName::Lovely_Kiss: return MoveCriticalHit::never;
		case MoveName::Sky_Attack: return generation <= Generation::two ? MoveCriticalHit::normal : MoveCriticalHit::high;
		case MoveName::Transform: return MoveCriticalHit::never;
		case MoveName::Bubble: return MoveCriticalHit::normal;
		case MoveName::Dizzy_Punch: return MoveCriticalHit::normal;
		case MoveName::Spore: return MoveCriticalHit::never;
		case MoveName::Flash: return MoveCriticalHit::never;
		case MoveName::Psywave: return MoveCriticalHit::never;
		case MoveName::Splash: return MoveCriticalHit::never;
		case MoveName::Acid_Armor: return MoveCriticalHit::never;
		case MoveName::Crabhammer: return MoveCriticalHit::high;
		case MoveName::Explosion: return MoveCriticalHit::normal;
		case MoveName::Fury_Swipes: return MoveCriticalHit::normal;
		case MoveName::Bonemerang: return MoveCriticalHit::normal;
		case MoveName::Rest: return MoveCriticalHit::never;
		case MoveName::Rock_Slide: return MoveCriticalHit::normal;
		case MoveName::Hyper_Fang: return MoveCriticalHit::normal;
		case MoveName::Sharpen: return MoveCriticalHit::never;
		case MoveName::Conversion: return MoveCriticalHit::never;
		case MoveName::Tri_Attack: return MoveCriticalHit::normal;
		case MoveName::Super_Fang: return MoveCriticalHit::never;
		case MoveName::Slash: return MoveCriticalHit::high;
		case MoveName::Substitute: return MoveCriticalHit::never;
		case MoveName::Struggle: return MoveCriticalHit::never;

		// Generation 2
		case MoveName::Sketch: return MoveCriticalHit::never;
		case MoveName::Triple_Kick: return MoveCriticalHit::normal;
		case MoveName::Thief: return MoveCriticalHit::normal;
		case MoveName::Spider_Web: return MoveCriticalHit::never;
		case MoveName::Mind_Reader: return MoveCriticalHit::never;
		case MoveName::Nightmare: return MoveCriticalHit::never;
		case MoveName::Flame_Wheel: return MoveCriticalHit::normal;
		case MoveName::Snore: return MoveCriticalHit::normal;
		case MoveName::Curse: return MoveCriticalHit::never;
		case MoveName::Flail: return generation <= Generation::two ? MoveCriticalHit::never : MoveCriticalHit::normal;
		case MoveName::Conversion_2: return MoveCriticalHit::never;
		case MoveName::Aeroblast: return MoveCriticalHit::high;
		case MoveName::Cotton_Spore: return MoveCriticalHit::never;
		case MoveName::Reversal: return generation <= Generation::two ? MoveCriticalHit::never : MoveCriticalHit::normal;
		case MoveName::Spite: return MoveCriticalHit::never;
		case MoveName::Powder_Snow: return MoveCriticalHit::normal;
		case MoveName::Protect: return MoveCriticalHit::never;
		case MoveName::Mach_Punch: return MoveCriticalHit::normal;
		case MoveName::Scary_Face: return MoveCriticalHit::never;
		case MoveName::Feint_Attack: return MoveCriticalHit::normal;
		case MoveName::Sweet_Kiss: return MoveCriticalHit::never;
		case MoveName::Belly_Drum: return MoveCriticalHit::never;
		case MoveName::Sludge_Bomb: return MoveCriticalHit::normal;
		case MoveName::Mud_Slap: return MoveCriticalHit::normal;
		case MoveName::Octazooka: return MoveCriticalHit::normal;
		case MoveName::Spikes: return MoveCriticalHit::never;
		case MoveName::Zap_Cannon: return MoveCriticalHit::normal;
		case MoveName::Foresight: return MoveCriticalHit::never;
		case MoveName::Destiny_Bond: return MoveCriticalHit::never;
		case MoveName::Perish_Song: return MoveCriticalHit::never;
		case MoveName::Icy_Wind: return MoveCriticalHit::normal;
		case MoveName::Detect: return MoveCriticalHit::never;
		case MoveName::Bone_Rush: return MoveCriticalHit::normal;
		case MoveName::Lock_On: return MoveCriticalHit::never;
		case MoveName::Outrage: return MoveCriticalHit::normal;
		case MoveName::Sandstorm: return MoveCriticalHit::never;
		case MoveName::Giga_Drain: return MoveCriticalHit::normal;
		case MoveName::Endure: return MoveCriticalHit::never;
		case MoveName::Charm: return MoveCriticalHit::never;
		case MoveName::Rollout: return MoveCriticalHit::normal;
		case MoveName::False_Swipe: return MoveCriticalHit::normal;
		case MoveName::Swagger: return MoveCriticalHit::never;
		case MoveName::Milk_Drink: return MoveCriticalHit::never;
		case MoveName::Spark: return MoveCriticalHit::normal;
		case MoveName::Fury_Cutter: return MoveCriticalHit::normal;
		case MoveName::Steel_Wing: return MoveCriticalHit::normal;
		case MoveName::Mean_Look: return MoveCriticalHit::never;
		case MoveName::Attract: return MoveCriticalHit::never;
		case MoveName::Sleep_Talk: return MoveCriticalHit::never;
		case MoveName::Heal_Bell: return MoveCriticalHit::never;
		case MoveName::Return: return MoveCriticalHit::normal;
		case MoveName::Present: return MoveCriticalHit::normal;
		case MoveName::Frustration: return MoveCriticalHit::normal;
		case MoveName::Safeguard: return MoveCriticalHit::never;
		case MoveName::Pain_Split: return MoveCriticalHit::never;
		case MoveName::Sacred_Fire: return MoveCriticalHit::normal;
		case MoveName::Magnitude: return MoveCriticalHit::normal;
		case MoveName::Dynamic_Punch: return MoveCriticalHit::normal;
		case MoveName::Megahorn: return MoveCriticalHit::normal;
		case MoveName::Dragon_Breath: return MoveCriticalHit::normal;
		case MoveName::Baton_Pass: return MoveCriticalHit::never;
		case MoveName::Encore: return MoveCriticalHit::never;
		case MoveName::Pursuit: return MoveCriticalHit::normal;
		case MoveName::Rapid_Spin: return MoveCriticalHit::normal;
		case MoveName::Sweet_Scent: return MoveCriticalHit::never;
		case MoveName::Iron_Tail: return MoveCriticalHit::normal;
		case MoveName::Metal_Claw: return MoveCriticalHit::normal;
		case MoveName::Vital_Throw: return MoveCriticalHit::normal;
		case MoveName::Morning_Sun: return MoveCriticalHit::never;
		case MoveName::Synthesis: return MoveCriticalHit::never;
		case MoveName::Moonlight: return MoveCriticalHit::never;
		case MoveName::Hidden_Power: return MoveCriticalHit::normal;
		case MoveName::Cross_Chop: return MoveCriticalHit::high;
		case MoveName::Twister: return MoveCriticalHit::normal;
		case MoveName::Rain_Dance: return MoveCriticalHit::never;
		case MoveName::Sunny_Day: return MoveCriticalHit::never;
		case MoveName::Crunch: return MoveCriticalHit::normal;
		case MoveName::Mirror_Coat: return MoveCriticalHit::never;
		case MoveName::Psych_Up: return MoveCriticalHit::never;
		case MoveName::Extreme_Speed: return MoveCriticalHit::normal;
		case MoveName::Ancient_Power: return MoveCriticalHit::normal;
		case MoveName::Shadow_Ball: return MoveCriticalHit::normal;
		case MoveName::Future_Sight: return MoveCriticalHit::never;
		case MoveName::Rock_Smash: return MoveCriticalHit::normal;
		case MoveName::Whirlpool: return MoveCriticalHit::normal;
		case MoveName::Beat_Up: return MoveCriticalHit::never;

		// Generation 3
		case MoveName::Fake_Out: return MoveCriticalHit::normal;
		case MoveName::Uproar: return MoveCriticalHit::normal;
		case MoveName::Stockpile: return MoveCriticalHit::never;
		case MoveName::Spit_Up: return MoveCriticalHit::normal;
		case MoveName::Swallow: return MoveCriticalHit::never;
		case MoveName::Heat_Wave: return MoveCriticalHit::normal;
		case MoveName::Hail: return MoveCriticalHit::never;
		case MoveName::Torment: return MoveCriticalHit::never;
		case MoveName::Flatter: return MoveCriticalHit::never;
		case MoveName::Will_O_Wisp: return MoveCriticalHit::never;
		case MoveName::Memento: return MoveCriticalHit::never;
		case MoveName::Facade: return MoveCriticalHit::normal;
		case MoveName::Focus_Punch: return MoveCriticalHit::normal;
		case MoveName::Smelling_Salts: return MoveCriticalHit::normal;
		case MoveName::Follow_Me: return MoveCriticalHit::never;
		case MoveName::Nature_Power: return MoveCriticalHit::never;
		case MoveName::Charge: return MoveCriticalHit::never;
		case MoveName::Taunt: return MoveCriticalHit::never;
		case MoveName::Helping_Hand: return MoveCriticalHit::never;
		case MoveName::Trick: return MoveCriticalHit::never;
		case MoveName::Role_Play: return MoveCriticalHit::never;
		case MoveName::Wish: return MoveCriticalHit::never;
		case MoveName::Assist: return MoveCriticalHit::never;
		case MoveName::Ingrain: return MoveCriticalHit::never;
		case MoveName::Superpower: return MoveCriticalHit::normal;
		case MoveName::Magic_Coat: return MoveCriticalHit::never;
		case MoveName::Recycle: return MoveCriticalHit::never;
		case MoveName::Revenge: return MoveCriticalHit::normal;
		case MoveName::Brick_Break: return MoveCriticalHit::normal;
		case MoveName::Yawn: return MoveCriticalHit::never;
		case MoveName::Knock_Off: return MoveCriticalHit::normal;
		case MoveName::Endeavor: return MoveCriticalHit::never;
		case MoveName::Eruption: return MoveCriticalHit::normal;
		case MoveName::Skill_Swap: return MoveCriticalHit::never;
		case MoveName::Imprison: return MoveCriticalHit::never;
		case MoveName::Refresh: return MoveCriticalHit::never;
		case MoveName::Grudge: return MoveCriticalHit::never;
		case MoveName::Snatch: return MoveCriticalHit::never;
		case MoveName::Secret_Power: return MoveCriticalHit::normal;
		case MoveName::Dive: return MoveCriticalHit::normal;
		case MoveName::Arm_Thrust: return MoveCriticalHit::normal;
		case MoveName::Camouflage: return MoveCriticalHit::never;
		case MoveName::Tail_Glow: return MoveCriticalHit::never;
		case MoveName::Luster_Purge: return MoveCriticalHit::normal;
		case MoveName::Mist_Ball: return MoveCriticalHit::normal;
		case MoveName::Feather_Dance: return MoveCriticalHit::never;
		case MoveName::Teeter_Dance: return MoveCriticalHit::never;
		case MoveName::Blaze_Kick: return MoveCriticalHit::high;
		case MoveName::Mud_Sport: return MoveCriticalHit::never;
		case MoveName::Ice_Ball: return MoveCriticalHit::normal;
		case MoveName::Needle_Arm: return MoveCriticalHit::normal;
		case MoveName::Slack_Off: return MoveCriticalHit::never;
		case MoveName::Hyper_Voice: return MoveCriticalHit::normal;
		case MoveName::Poison_Fang: return MoveCriticalHit::normal;
		case MoveName::Crush_Claw: return MoveCriticalHit::normal;
		case MoveName::Blast_Burn: return MoveCriticalHit::normal;
		case MoveName::Hydro_Cannon: return MoveCriticalHit::normal;
		case MoveName::Meteor_Mash: return MoveCriticalHit::normal;
		case MoveName::Astonish: return MoveCriticalHit::normal;
		case MoveName::Weather_Ball: return MoveCriticalHit::normal;
		case MoveName::Aromatherapy: return MoveCriticalHit::never;
		case MoveName::Fake_Tears: return MoveCriticalHit::never;
		case MoveName::Air_Cutter: return MoveCriticalHit::high;
		case MoveName::Overheat: return MoveCriticalHit::normal;
		case MoveName::Odor_Sleuth: return MoveCriticalHit::never;
		case MoveName::Rock_Tomb: return MoveCriticalHit::normal;
		case MoveName::Silver_Wind: return MoveCriticalHit::normal;
		case MoveName::Metal_Sound: return MoveCriticalHit::never;
		case MoveName::Grass_Whistle: return MoveCriticalHit::never;
		case MoveName::Tickle: return MoveCriticalHit::never;
		case MoveName::Cosmic_Power: return MoveCriticalHit::never;
		case MoveName::Water_Spout: return MoveCriticalHit::normal;
		case MoveName::Signal_Beam: return MoveCriticalHit::normal;
		case MoveName::Shadow_Punch: return MoveCriticalHit::normal;
		case MoveName::Extrasensory: return MoveCriticalHit::normal;
		case MoveName::Sky_Uppercut: return MoveCriticalHit::normal;
		case MoveName::Sand_Tomb: return MoveCriticalHit::normal;
		case MoveName::Sheer_Cold: return MoveCriticalHit::never;
		case MoveName::Muddy_Water: return MoveCriticalHit::normal;
		case MoveName::Bullet_Seed: return MoveCriticalHit::normal;
		case MoveName::Aerial_Ace: return MoveCriticalHit::normal;
		case MoveName::Icicle_Spear: return MoveCriticalHit::normal;
		case MoveName::Iron_Defense: return MoveCriticalHit::never;
		case MoveName::Block: return MoveCriticalHit::never;
		case MoveName::Howl: return MoveCriticalHit::never;
		case MoveName::Dragon_Claw: return MoveCriticalHit::normal;
		case MoveName::Frenzy_Plant: return MoveCriticalHit::normal;
		case MoveName::Bulk_Up: return MoveCriticalHit::never;
		case MoveName::Bounce: return MoveCriticalHit::normal;
		case MoveName::Mud_Shot: return MoveCriticalHit::normal;
		case MoveName::Poison_Tail: return MoveCriticalHit::high;
		case MoveName::Covet: return MoveCriticalHit::normal;
		case MoveName::Volt_Tackle: return MoveCriticalHit::normal;
		case MoveName::Magical_Leaf: return MoveCriticalHit::normal;
		case MoveName::Water_Sport: return MoveCriticalHit::never;
		case MoveName::Calm_Mind: return MoveCriticalHit::never;
		case MoveName::Leaf_Blade: return MoveCriticalHit::high;
		case MoveName::Dragon_Dance: return MoveCriticalHit::never;
		case MoveName::Rock_Blast: return MoveCriticalHit::normal;
		case MoveName::Shock_Wave: return MoveCriticalHit::normal;
		case MoveName::Water_Pulse: return MoveCriticalHit::normal;
		case MoveName::Doom_Desire: return MoveCriticalHit::never;
		case MoveName::Psycho_Boost: return MoveCriticalHit::normal;

		// Generation 4
		case MoveName::Roost: return MoveCriticalHit::never;
		case MoveName::Gravity: return MoveCriticalHit::never;
		case MoveName::Miracle_Eye: return MoveCriticalHit::never;
		case MoveName::Wake_Up_Slap: return MoveCriticalHit::normal;
		case MoveName::Hammer_Arm: return MoveCriticalHit::normal;
		case MoveName::Gyro_Ball: return MoveCriticalHit::normal;
		case MoveName::Healing_Wish: return MoveCriticalHit::never;
		case MoveName::Brine: return MoveCriticalHit::normal;
		case MoveName::Natural_Gift: return MoveCriticalHit::normal;
		case MoveName::Feint: return MoveCriticalHit::normal;
		case MoveName::Pluck: return MoveCriticalHit::normal;
		case MoveName::Tailwind: return MoveCriticalHit::never;
		case MoveName::Acupressure: return MoveCriticalHit::never;
		case MoveName::Metal_Burst: return MoveCriticalHit::never;
		case MoveName::U_turn: return MoveCriticalHit::normal;
		case MoveName::Close_Combat: return MoveCriticalHit::normal;
		case MoveName::Payback: return MoveCriticalHit::normal;
		case MoveName::Assurance: return MoveCriticalHit::normal;
		case MoveName::Embargo: return MoveCriticalHit::never;
		case MoveName::Fling: return MoveCriticalHit::normal;
		case MoveName::Psycho_Shift: return MoveCriticalHit::never;
		case MoveName::Trump_Card: return MoveCriticalHit::normal;
		case MoveName::Heal_Block: return MoveCriticalHit::never;
		case MoveName::Wring_Out: return MoveCriticalHit::normal;
		case MoveName::Power_Trick: return MoveCriticalHit::never;
		case MoveName::Gastro_Acid: return MoveCriticalHit::never;
		case MoveName::Lucky_Chant: return MoveCriticalHit::never;
		case MoveName::Me_First: return MoveCriticalHit::never;
		case MoveName::Copycat: return MoveCriticalHit::never;
		case MoveName::Power_Swap: return MoveCriticalHit::never;
		case MoveName::Guard_Swap: return MoveCriticalHit::never;
		case MoveName::Punishment: return MoveCriticalHit::normal;
		case MoveName::Last_Resort: return MoveCriticalHit::normal;
		case MoveName::Worry_Seed: return MoveCriticalHit::never;
		case MoveName::Sucker_Punch: return MoveCriticalHit::normal;
		case MoveName::Toxic_Spikes: return MoveCriticalHit::never;
		case MoveName::Heart_Swap: return MoveCriticalHit::never;
		case MoveName::Aqua_Ring: return MoveCriticalHit::never;
		case MoveName::Magnet_Rise: return MoveCriticalHit::never;
		case MoveName::Flare_Blitz: return MoveCriticalHit::normal;
		case MoveName::Force_Palm: return MoveCriticalHit::normal;
		case MoveName::Aura_Sphere: return MoveCriticalHit::normal;
		case MoveName::Rock_Polish: return MoveCriticalHit::never;
		case MoveName::Poison_Jab: return MoveCriticalHit::normal;
		case MoveName::Dark_Pulse: return MoveCriticalHit::normal;
		case MoveName::Night_Slash: return MoveCriticalHit::high;
		case MoveName::Aqua_Tail: return MoveCriticalHit::normal;
		case MoveName::Seed_Bomb: return MoveCriticalHit::normal;
		case MoveName::Air_Slash: return MoveCriticalHit::normal;
		case MoveName::X_Scissor: return MoveCriticalHit::normal;
		case MoveName::Bug_Buzz: return MoveCriticalHit::normal;
		case MoveName::Dragon_Pulse: return MoveCriticalHit::normal;
		case MoveName::Dragon_Rush: return MoveCriticalHit::normal;
		case MoveName::Power_Gem: return MoveCriticalHit::normal;
		case MoveName::Drain_Punch: return MoveCriticalHit::normal;
		case MoveName::Vacuum_Wave: return MoveCriticalHit::normal;
		case MoveName::Focus_Blast: return MoveCriticalHit::normal;
		case MoveName::Energy_Ball: return MoveCriticalHit::normal;
		case MoveName::Brave_Bird: return MoveCriticalHit::normal;
		case MoveName::Earth_Power: return MoveCriticalHit::normal;
		case MoveName::Switcheroo: return MoveCriticalHit::never;
		case MoveName::Giga_Impact: return MoveCriticalHit::normal;
		case MoveName::Nasty_Plot: return MoveCriticalHit::never;
		case MoveName::Bullet_Punch: return MoveCriticalHit::normal;
		case MoveName::Avalanche: return MoveCriticalHit::normal;
		case MoveName::Ice_Shard: return MoveCriticalHit::normal;
		case MoveName::Shadow_Claw: return MoveCriticalHit::high;
		case MoveName::Thunder_Fang: return MoveCriticalHit::normal;
		case MoveName::Ice_Fang: return MoveCriticalHit::normal;
		case MoveName::Fire_Fang: return MoveCriticalHit::normal;
		case MoveName::Shadow_Sneak: return MoveCriticalHit::normal;
		case MoveName::Mud_Bomb: return MoveCriticalHit::normal;
		case MoveName::Psycho_Cut: return MoveCriticalHit::high;
		case MoveName::Zen_Headbutt: return MoveCriticalHit::normal;
		case MoveName::Mirror_Shot: return MoveCriticalHit::normal;
		case MoveName::Flash_Cannon: return MoveCriticalHit::normal;
		case MoveName::Rock_Climb: return MoveCriticalHit::normal;
		case MoveName::Defog: return MoveCriticalHit::never;
		case MoveName::Trick_Room: return MoveCriticalHit::never;
		case MoveName::Draco_Meteor: return MoveCriticalHit::normal;
		case MoveName::Discharge: return MoveCriticalHit::normal;
		case MoveName::Lava_Plume: return MoveCriticalHit::normal;
		case MoveName::Leaf_Storm: return MoveCriticalHit::normal;
		case MoveName::Power_Whip: return MoveCriticalHit::normal;
		case MoveName::Rock_Wrecker: return MoveCriticalHit::normal;
		case MoveName::Cross_Poison: return MoveCriticalHit::high;
		case MoveName::Gunk_Shot: return MoveCriticalHit::normal;
		case MoveName::Iron_Head: return MoveCriticalHit::normal;
		case MoveName::Magnet_Bomb: return MoveCriticalHit::normal;
		case MoveName::Stone_Edge: return MoveCriticalHit::high;
		case MoveName::Captivate: return MoveCriticalHit::never;
		case MoveName::Stealth_Rock: return MoveCriticalHit::never;
		case MoveName::Grass_Knot: return MoveCriticalHit::normal;
		case MoveName::Chatter: return MoveCriticalHit::normal;
		case MoveName::Judgment: return MoveCriticalHit::normal;
		case MoveName::Bug_Bite: return MoveCriticalHit::normal;
		case MoveName::Charge_Beam: return MoveCriticalHit::normal;
		case MoveName::Wood_Hammer: return MoveCriticalHit::normal;
		case MoveName::Aqua_Jet: return MoveCriticalHit::normal;
		case MoveName::Attack_Order: return MoveCriticalHit::high;
		case MoveName::Defend_Order: return MoveCriticalHit::never;
		case MoveName::Heal_Order: return MoveCriticalHit::never;
		case MoveName::Head_Smash: return MoveCriticalHit::normal;
		case MoveName::Double_Hit: return MoveCriticalHit::normal;
		case MoveName::Roar_of_Time: return MoveCriticalHit::normal;
		case MoveName::Spacial_Rend: return MoveCriticalHit::high;
		case MoveName::Lunar_Dance: return MoveCriticalHit::never;
		case MoveName::Crush_Grip: return MoveCriticalHit::normal;
		case MoveName::Magma_Storm: return MoveCriticalHit::normal;
		case MoveName::Dark_Void: return MoveCriticalHit::never;
		case MoveName::Seed_Flare: return MoveCriticalHit::normal;
		case MoveName::Ominous_Wind: return MoveCriticalHit::normal;
		case MoveName::Shadow_Force: return MoveCriticalHit::normal;

		// Generation 5
		case MoveName::Hone_Claws: return MoveCriticalHit::never;
		case MoveName::Wide_Guard: return MoveCriticalHit::never;
		case MoveName::Guard_Split: return MoveCriticalHit::never;
		case MoveName::Power_Split: return MoveCriticalHit::never;
		case MoveName::Wonder_Room: return MoveCriticalHit::never;
		case MoveName::Psyshock: return MoveCriticalHit::normal;
		case MoveName::Venoshock: return MoveCriticalHit::normal;
		case MoveName::Autotomize: return MoveCriticalHit::never;
		case MoveName::Rage_Powder: return MoveCriticalHit::never;
		case MoveName::Telekinesis: return MoveCriticalHit::never;
		case MoveName::Magic_Room: return MoveCriticalHit::never;
		case MoveName::Smack_Down: return MoveCriticalHit::normal;
		case MoveName::Storm_Throw: return MoveCriticalHit::always;
		case MoveName::Flame_Burst: return MoveCriticalHit::normal;
		case MoveName::Sludge_Wave: return MoveCriticalHit::normal;
		case MoveName::Quiver_Dance: return MoveCriticalHit::never;
		case MoveName::Heavy_Slam: return MoveCriticalHit::normal;
		case MoveName::Synchronoise: return MoveCriticalHit::normal;
		case MoveName::Electro_Ball: return MoveCriticalHit::normal;
		case MoveName::Soak: return MoveCriticalHit::never;
		case MoveName::Flame_Charge: return MoveCriticalHit::normal;
		case MoveName::Coil: return MoveCriticalHit::never;
		case MoveName::Low_Sweep: return MoveCriticalHit::normal;
		case MoveName::Acid_Spray: return MoveCriticalHit::normal;
		case MoveName::Foul_Play: return MoveCriticalHit::normal;
		case MoveName::Simple_Beam: return MoveCriticalHit::never;
		case MoveName::Entrainment: return MoveCriticalHit::never;
		case MoveName::After_You: return MoveCriticalHit::never;
		case MoveName::Round: return MoveCriticalHit::normal;
		case MoveName::Echoed_Voice: return MoveCriticalHit::normal;
		case MoveName::Chip_Away: return MoveCriticalHit::normal;
		case MoveName::Clear_Smog: return MoveCriticalHit::normal;
		case MoveName::Stored_Power: return MoveCriticalHit::normal;
		case MoveName::Quick_Guard: return MoveCriticalHit::never;
		case MoveName::Ally_Switch: return MoveCriticalHit::never;
		case MoveName::Scald: return MoveCriticalHit::normal;
		case MoveName::Shell_Smash: return MoveCriticalHit::never;
		case MoveName::Heal_Pulse: return MoveCriticalHit::never;
		case MoveName::Hex: return MoveCriticalHit::normal;
		case MoveName::Sky_Drop: return MoveCriticalHit::normal;
		case MoveName::Shift_Gear: return MoveCriticalHit::never;
		case MoveName::Circle_Throw: return MoveCriticalHit::normal;
		case MoveName::Incinerate: return MoveCriticalHit::normal;
		case MoveName::Quash: return MoveCriticalHit::never;
		case MoveName::Acrobatics: return MoveCriticalHit::normal;
		case MoveName::Reflect_Type: return MoveCriticalHit::never;
		case MoveName::Retaliate: return MoveCriticalHit::normal;
		case MoveName::Final_Gambit: return MoveCriticalHit::normal;
		case MoveName::Bestow: return MoveCriticalHit::never;
		case MoveName::Inferno: return MoveCriticalHit::normal;
		case MoveName::Water_Pledge: return MoveCriticalHit::normal;
		case MoveName::Fire_Pledge: return MoveCriticalHit::normal;
		case MoveName::Grass_Pledge: return MoveCriticalHit::normal;
		case MoveName::Volt_Switch: return MoveCriticalHit::normal;
		case MoveName::Struggle_Bug: return MoveCriticalHit::normal;
		case MoveName::Bulldoze: return MoveCriticalHit::normal;
		case MoveName::Frost_Breath: return MoveCriticalHit::always;
		case MoveName::Dragon_Tail: return MoveCriticalHit::normal;
		case MoveName::Work_Up: return MoveCriticalHit::never;
		case MoveName::Electroweb: return MoveCriticalHit::normal;
		case MoveName::Wild_Charge: return MoveCriticalHit::normal;
		case MoveName::Drill_Run: return MoveCriticalHit::high;
		case MoveName::Dual_Chop: return MoveCriticalHit::normal;
		case MoveName::Heart_Stamp: return MoveCriticalHit::normal;
		case MoveName::Horn_Leech: return MoveCriticalHit::normal;
		case MoveName::Sacred_Sword: return MoveCriticalHit::normal;
		case MoveName::Razor_Shell: return MoveCriticalHit::normal;
		case MoveName::Heat_Crash: return MoveCriticalHit::normal;
		case MoveName::Leaf_Tornado: return MoveCriticalHit::normal;
		case MoveName::Steamroller: return MoveCriticalHit::normal;
		case MoveName::Cotton_Guard: return MoveCriticalHit::never;
		case MoveName::Night_Daze: return MoveCriticalHit::normal;
		case MoveName::Psystrike: return MoveCriticalHit::normal;
		case MoveName::Tail_Slap: return MoveCriticalHit::normal;
		case MoveName::Hurricane: return MoveCriticalHit::normal;
		case MoveName::Head_Charge: return MoveCriticalHit::normal;
		case MoveName::Gear_Grind: return MoveCriticalHit::normal;
		case MoveName::Searing_Shot: return MoveCriticalHit::normal;
		case MoveName::Techno_Blast: return MoveCriticalHit::normal;
		case MoveName::Relic_Song: return MoveCriticalHit::normal;
		case MoveName::Secret_Sword: return MoveCriticalHit::normal;
		case MoveName::Glaciate: return MoveCriticalHit::normal;
		case MoveName::Bolt_Strike: return MoveCriticalHit::normal;
		case MoveName::Blue_Flare: return MoveCriticalHit::normal;
		case MoveName::Fiery_Dance: return MoveCriticalHit::normal;
		case MoveName::Freeze_Shock: return MoveCriticalHit::normal;
		case MoveName::Ice_Burn: return MoveCriticalHit::normal;
		case MoveName::Snarl: return MoveCriticalHit::normal;
		case MoveName::Icicle_Crash: return MoveCriticalHit::normal;
		case MoveName::V_create: return MoveCriticalHit::normal;
		case MoveName::Fusion_Flare: return MoveCriticalHit::normal;
		case MoveName::Fusion_Bolt: return MoveCriticalHit::normal;

		// Generation 6
		case MoveName::Flying_Press: return MoveCriticalHit::normal;
		case MoveName::Mat_Block: return MoveCriticalHit::never;
		case MoveName::Belch: return MoveCriticalHit::normal;
		case MoveName::Rototiller: return MoveCriticalHit::never;
		case MoveName::Sticky_Web: return MoveCriticalHit::never;
		case MoveName::Fell_Stinger: return MoveCriticalHit::normal;
		case MoveName::Phantom_Force: return MoveCriticalHit::normal;
		case MoveName::Trick_or_Treat: return MoveCriticalHit::never;
		case MoveName::Noble_Roar: return MoveCriticalHit::never;
		case MoveName::Ion_Deluge: return MoveCriticalHit::never;
		case MoveName::Parabolic_Charge: return MoveCriticalHit::normal;
		case MoveName::Forests_Curse: return MoveCriticalHit::never;
		case MoveName::Petal_Blizzard: return MoveCriticalHit::normal;
		case MoveName::Freeze_Dry: return MoveCriticalHit::normal;
		case MoveName::Disarming_Voice: return MoveCriticalHit::normal;
		case MoveName::Parting_Shot: return MoveCriticalHit::never;
		case MoveName::Topsy_Turvy: return MoveCriticalHit::never;
		case MoveName::Draining_Kiss: return MoveCriticalHit::normal;
		case MoveName::Crafty_Shield: return MoveCriticalHit::never;
		case MoveName::Flower_Shield: return MoveCriticalHit::never;
		case MoveName::Grassy_Terrain: return MoveCriticalHit::never;
		case MoveName::Misty_Terrain: return MoveCriticalHit::never;
		case MoveName::Electrify: return MoveCriticalHit::never;
		case MoveName::Play_Rough: return MoveCriticalHit::normal;
		case MoveName::Fairy_Wind: return MoveCriticalHit::normal;
		case MoveName::Moonblast: return MoveCriticalHit::normal;
		case MoveName::Boomburst: return MoveCriticalHit::normal;
		case MoveName::Fairy_Lock: return MoveCriticalHit::never;
		case MoveName::Kings_Shield: return MoveCriticalHit::never;
		case MoveName::Play_Nice: return MoveCriticalHit::never;
		case MoveName::Confide: return MoveCriticalHit::never;
		case MoveName::Diamond_Storm: return MoveCriticalHit::normal;
		case MoveName::Steam_Eruption: return MoveCriticalHit::normal;
		case MoveName::Hyperspace_Hole: return MoveCriticalHit::normal;
		case MoveName::Water_Shuriken: return MoveCriticalHit::normal;
		case MoveName::Mystical_Fire: return MoveCriticalHit::normal;
		case MoveName::Spiky_Shield: return MoveCriticalHit::never;
		case MoveName::Aromatic_Mist: return MoveCriticalHit::never;
		case MoveName::Eerie_Impulse: return MoveCriticalHit::never;
		case MoveName::Venom_Drench: return MoveCriticalHit::never;
		case MoveName::Powder: return MoveCriticalHit::never;
		case MoveName::Geomancy: return MoveCriticalHit::never;
		case MoveName::Magnetic_Flux: return MoveCriticalHit::never;
		case MoveName::Happy_Hour: return MoveCriticalHit::never;
		case MoveName::Electric_Terrain: return MoveCriticalHit::never;
		case MoveName::Dazzling_Gleam: return MoveCriticalHit::normal;
		case MoveName::Celebrate: return MoveCriticalHit::never;
		case MoveName::Hold_Hands: return MoveCriticalHit::never;
		case MoveName::Baby_Doll_Eyes: return MoveCriticalHit::never;
		case MoveName::Nuzzle: return MoveCriticalHit::normal;
		case MoveName::Hold_Back: return MoveCriticalHit::normal;
		case MoveName::Infestation: return MoveCriticalHit::normal;
		case MoveName::Power_Up_Punch: return MoveCriticalHit::normal;
		case MoveName::Oblivion_Wing: return MoveCriticalHit::normal;
		case MoveName::Thousand_Arrows: return MoveCriticalHit::normal;
		case MoveName::Thousand_Waves: return MoveCriticalHit::normal;
		case MoveName::Lands_Wrath: return MoveCriticalHit::normal;
		case MoveName::Light_of_Ruin: return MoveCriticalHit::normal;
		case MoveName::Origin_Pulse: return MoveCriticalHit::normal;
		case MoveName::Precipice_Blades: return MoveCriticalHit::normal;
		case MoveName::Dragon_Ascent: return MoveCriticalHit::normal;
		case MoveName::Hyperspace_Fury: return MoveCriticalHit::normal;

		// Generation 7
		case MoveName::Breakneck_Blitz: return MoveCriticalHit::never;
		case MoveName::All_Out_Pummeling: return MoveCriticalHit::never;
		case MoveName::Supersonic_Skystrike: return MoveCriticalHit::never;
		case MoveName::Acid_Downpour: return MoveCriticalHit::never;
		case MoveName::Tectonic_Rage: return MoveCriticalHit::never;
		case MoveName::Continental_Crush: return MoveCriticalHit::never;
		case MoveName::Savage_Spin_Out: return MoveCriticalHit::never;
		case MoveName::Never_Ending_Nightmare: return MoveCriticalHit::never;
		case MoveName::Corkscrew_Crash: return MoveCriticalHit::never;
		case MoveName::Inferno_Overdrive: return MoveCriticalHit::never;
		case MoveName::Hydro_Vortex: return MoveCriticalHit::never;
		case MoveName::Bloom_Doom: return MoveCriticalHit::never;
		case MoveName::Gigavolt_Havoc: return MoveCriticalHit::never;
		case MoveName::Shattered_Psyche: return MoveCriticalHit::never;
		case MoveName::Subzero_Slammer: return MoveCriticalHit::never;
		case MoveName::Devastating_Drake: return MoveCriticalHit::never;
		case MoveName::Black_Hole_Eclipse: return MoveCriticalHit::never;
		case MoveName::Twinkle_Tackle: return MoveCriticalHit::never;
		case MoveName::Catastropika: return MoveCriticalHit::normal;
		case MoveName::Shore_Up: return MoveCriticalHit::never;
		case MoveName::First_Impression: return MoveCriticalHit::normal;
		case MoveName::Baneful_Bunker: return MoveCriticalHit::never;
		case MoveName::Spirit_Shackle: return MoveCriticalHit::normal;
		case MoveName::Darkest_Lariat: return MoveCriticalHit::normal;
		case MoveName::Sparkling_Aria: return MoveCriticalHit::normal;
		case MoveName::Ice_Hammer: return MoveCriticalHit::normal;
		case MoveName::Floral_Healing: return MoveCriticalHit::never;
		case MoveName::High_Horsepower: return MoveCriticalHit::normal;
		case MoveName::Strength_Sap: return MoveCriticalHit::never;
		case MoveName::Solar_Blade: return MoveCriticalHit::normal;
		case MoveName::Leafage: return MoveCriticalHit::normal;
		case MoveName::Spotlight: return MoveCriticalHit::never;
		case MoveName::Toxic_Thread: return MoveCriticalHit::never;
		case MoveName::Laser_Focus: return MoveCriticalHit::never;
		case MoveName::Gear_Up: return MoveCriticalHit::never;
		case MoveName::Throat_Chop: return MoveCriticalHit::normal;
		case MoveName::Pollen_Puff: return MoveCriticalHit::normal;
		case MoveName::Anchor_Shot: return MoveCriticalHit::normal;
		case MoveName::Psychic_Terrain: return MoveCriticalHit::never;
		case MoveName::Lunge: return MoveCriticalHit::normal;
		case MoveName::Fire_Lash: return MoveCriticalHit::normal;
		case MoveName::Power_Trip: return MoveCriticalHit::normal;
		case MoveName::Burn_Up: return MoveCriticalHit::normal;
		case MoveName::Speed_Swap: return MoveCriticalHit::never;
		case MoveName::Smart_Strike: return MoveCriticalHit::normal;
		case MoveName::Purify: return MoveCriticalHit::never;
		case MoveName::Revelation_Dance: return MoveCriticalHit::normal;
		case MoveName::Core_Enforcer: return MoveCriticalHit::normal;
		case MoveName::Trop_Kick: return MoveCriticalHit::normal;
		case MoveName::Instruct: return MoveCriticalHit::never;
		case MoveName::Beak_Blast: return MoveCriticalHit::normal;
		case MoveName::Clanging_Scales: return MoveCriticalHit::normal;
		case MoveName::Dragon_Hammer: return MoveCriticalHit::normal;
		case MoveName::Brutal_Swing: return MoveCriticalHit::normal;
		case MoveName::Aurora_Veil: return MoveCriticalHit::never;
		case MoveName::Sinister_Arrow_Raid: return MoveCriticalHit::normal;
		case MoveName::Malicious_Moonsault: return MoveCriticalHit::normal;
		case MoveName::Oceanic_Operetta: return MoveCriticalHit::normal;
		case MoveName::Guardian_of_Alola: return MoveCriticalHit::never;
		case MoveName::Soul_Stealing_7_Star_Strike: return MoveCriticalHit::normal;
		case MoveName::Stoked_Sparksurfer: return MoveCriticalHit::normal;
		case MoveName::Pulverizing_Pancake: return MoveCriticalHit::normal;
		case MoveName::Extreme_Evoboost: return MoveCriticalHit::never;
		case MoveName::Genesis_Supernova: return MoveCriticalHit::normal;
		case MoveName::Shell_Trap: return MoveCriticalHit::normal;
		case MoveName::Fleur_Cannon: return MoveCriticalHit::normal;
		case MoveName::Psychic_Fangs: return MoveCriticalHit::normal;
		case MoveName::Stomping_Tantrum: return MoveCriticalHit::normal;
		case MoveName::Shadow_Bone: return MoveCriticalHit::normal;
		case MoveName::Accelerock: return MoveCriticalHit::normal;
		case MoveName::Liquidation: return MoveCriticalHit::normal;
		case MoveName::Prismatic_Laser: return MoveCriticalHit::normal;
		case MoveName::Spectral_Thief: return MoveCriticalHit::normal;
		case MoveName::Sunsteel_Strike: return MoveCriticalHit::normal;
		case MoveName::Moongeist_Beam: return MoveCriticalHit::normal;
		case MoveName::Tearful_Look: return MoveCriticalHit::never;
		case MoveName::Zing_Zap: return MoveCriticalHit::normal;
		case MoveName::Natures_Madness: return MoveCriticalHit::never;
		case MoveName::Multi_Attack: return MoveCriticalHit::normal;
		case MoveName::m10000000_Volt_Thunderbolt: return MoveCriticalHit::very_high;
		case MoveName::Mind_Blown: return MoveCriticalHit::normal;
		case MoveName::Plasma_Fists: return MoveCriticalHit::normal;
		case MoveName::Photon_Geyser: return MoveCriticalHit::normal;
		case MoveName::Light_That_Burns_the_Sky: return MoveCriticalHit::normal;
		case MoveName::Searing_Sunraze_Smash: return MoveCriticalHit::normal;
		case MoveName::Menacing_Moonraze_Maelstrom: return MoveCriticalHit::normal;
		case MoveName::Lets_Snuggle_Forever: return MoveCriticalHit::normal;
		case MoveName::Splintered_Stormshards: return MoveCriticalHit::normal;
		case MoveName::Clangorous_Soulblaze: return MoveCriticalHit::normal;
		case MoveName::Double_Iron_Bash: return MoveCriticalHit::normal;

		// Generation 8
		case MoveName::Max_Guard: return MoveCriticalHit::normal;
		case MoveName::Dynamax_Cannon: return MoveCriticalHit::normal;
		case MoveName::Snipe_Shot: return MoveCriticalHit::normal;
		case MoveName::Jaw_Lock: return MoveCriticalHit::normal;
		case MoveName::Stuff_Cheeks: return MoveCriticalHit::normal;
		case MoveName::No_Retreat: return MoveCriticalHit::normal;
		case MoveName::Tar_Shot: return MoveCriticalHit::normal;
		case MoveName::Magic_Powder: return MoveCriticalHit::normal;
		case MoveName::Dragon_Darts: return MoveCriticalHit::normal;
		case MoveName::Teatime: return MoveCriticalHit::normal;
		case MoveName::Octolock: return MoveCriticalHit::normal;
		case MoveName::Bolt_Beak: return MoveCriticalHit::normal;
		case MoveName::Fishious_Rend: return MoveCriticalHit::normal;
		case MoveName::Court_Change: return MoveCriticalHit::normal;
		case MoveName::Max_Flare: return MoveCriticalHit::normal;
		case MoveName::Max_Flutterby: return MoveCriticalHit::normal;
		case MoveName::Max_Lightning: return MoveCriticalHit::normal;
		case MoveName::Max_Strike: return MoveCriticalHit::normal;
		case MoveName::Max_Knuckle: return MoveCriticalHit::normal;
		case MoveName::Max_Phantasm: return MoveCriticalHit::normal;
		case MoveName::Max_Hailstorm: return MoveCriticalHit::normal;
		case MoveName::Max_Ooze: return MoveCriticalHit::normal;
		case MoveName::Max_Geyser: return MoveCriticalHit::normal;
		case MoveName::Max_Airstream: return MoveCriticalHit::normal;
		case MoveName::Max_Starfall: return MoveCriticalHit::normal;
		case MoveName::Max_Wyrmwind: return MoveCriticalHit::normal;
		case MoveName::Max_Mindstorm: return MoveCriticalHit::normal;
		case MoveName::Max_Rockfall: return MoveCriticalHit::normal;
		case MoveName::Max_Quake: return MoveCriticalHit::normal;
		case MoveName::Max_Darkness: return MoveCriticalHit::normal;
		case MoveName::Max_Overgrowth: return MoveCriticalHit::normal;
		case MoveName::Max_Steelspike: return MoveCriticalHit::normal;
		case MoveName::Clangorous_Soul: return MoveCriticalHit::normal;
		case MoveName::Body_Press: return MoveCriticalHit::normal;
		case MoveName::Decorate: return MoveCriticalHit::normal;
		case MoveName::Drum_Beating: return MoveCriticalHit::normal;
		case MoveName::Snap_Trap: return MoveCriticalHit::normal;
		case MoveName::Pyro_Ball: return MoveCriticalHit::normal;
		case MoveName::Behemoth_Blade: return MoveCriticalHit::normal;
		case MoveName::Behemoth_Bash: return MoveCriticalHit::normal;
		case MoveName::Aura_Wheel: return MoveCriticalHit::normal;
		case MoveName::Breaking_Swipe: return MoveCriticalHit::normal;
		case MoveName::Branch_Poke: return MoveCriticalHit::normal;
		case MoveName::Overdrive: return MoveCriticalHit::normal;
		case MoveName::Apple_Acid: return MoveCriticalHit::normal;
		case MoveName::Grav_Apple: return MoveCriticalHit::normal;
		case MoveName::Spirit_Break: return MoveCriticalHit::normal;
		case MoveName::Strange_Steam: return MoveCriticalHit::normal;
		case MoveName::Life_Dew: return MoveCriticalHit::normal;
		case MoveName::Obstruct: return MoveCriticalHit::normal;
		case MoveName::False_Surrender: return MoveCriticalHit::normal;
		case MoveName::Meteor_Assault: return MoveCriticalHit::normal;
		case MoveName::Eternabeam: return MoveCriticalHit::normal;
		case MoveName::Steel_Beam: return MoveCriticalHit::normal;
		case MoveName::Expanding_Force: return MoveCriticalHit::normal;
		case MoveName::Steel_Roller: return MoveCriticalHit::normal;
		case MoveName::Scale_Shot: return MoveCriticalHit::normal;
		case MoveName::Meteor_Beam: return MoveCriticalHit::normal;
		case MoveName::Shell_Side_Arm: return MoveCriticalHit::normal;
		case MoveName::Misty_Explosion: return MoveCriticalHit::normal;
		case MoveName::Grassy_Glide: return MoveCriticalHit::normal;
		case MoveName::Rising_Voltage: return MoveCriticalHit::normal;
		case MoveName::Terrain_Pulse: return MoveCriticalHit::normal;
		case MoveName::Skitter_Smack: return MoveCriticalHit::normal;
		case MoveName::Burning_Jealousy: return MoveCriticalHit::normal;
		case MoveName::Lash_Out: return MoveCriticalHit::normal;
		case MoveName::Poltergeist: return MoveCriticalHit::normal;
		case MoveName::Corrosive_Gas: return MoveCriticalHit::normal;
		case MoveName::Coaching: return MoveCriticalHit::normal;
		case MoveName::Flip_Turn: return MoveCriticalHit::normal;
		case MoveName::Triple_Axel: return MoveCriticalHit::normal;
		case MoveName::Dual_Wingbeat: return MoveCriticalHit::normal;
		case MoveName::Scorching_Sands: return MoveCriticalHit::normal;
		case MoveName::Jungle_Healing: return MoveCriticalHit::normal;
		case MoveName::Wicked_Blow: return MoveCriticalHit::normal;
		case MoveName::Surging_Strikes: return MoveCriticalHit::normal;
		case MoveName::Thunder_Cage: return MoveCriticalHit::normal;
		case MoveName::Dragon_Energy: return MoveCriticalHit::normal;
		case MoveName::Freezing_Glare: return MoveCriticalHit::normal;
		case MoveName::Fiery_Wrath: return MoveCriticalHit::normal;
		case MoveName::Thunderous_Kick: return MoveCriticalHit::normal;
		case MoveName::Glacial_Lance: return MoveCriticalHit::normal;
		case MoveName::Astral_Barrage: return MoveCriticalHit::normal;
		case MoveName::Eerie_Spell: return MoveCriticalHit::normal;
	}
}

constexpr auto generation_one_move_critical_hit_multiplier(MoveCriticalHit const move_adjustment) -> rational<bounded::integer<0, 4>, bounded::integer<1, 2>> {
	switch (move_adjustment) {
		case MoveCriticalHit::never:
			return rational(0_bi, 1_bi);
		case MoveCriticalHit::normal:
			return rational(1_bi, 2_bi);
		case MoveCriticalHit::high:
			return rational(4_bi, 1_bi);
		// TODO: Mark `very_high` and `always` as unreachable. This is not done
		// yet because we need to ensure at the client level that only moves
		// valid for the generation are allowed through.
		case MoveCriticalHit::very_high:
		case MoveCriticalHit::always:
			return rational(0_bi, 1_bi);
	}
}

constexpr auto gen_one_critical_hit(any_active_pokemon auto const attacker, MoveCriticalHit const move_adjustment) {
	auto const initial = BaseStats(Generation::one, attacker.species()).spe() / 2_bi;
	auto const focused = BOUNDED_CONDITIONAL(attacker.has_focused_energy(), initial / 2_bi, initial * 2_bi);
	auto const final = bounded::min(255_bi, focused * generation_one_move_critical_hit_multiplier(move_adjustment));
	return double(final) / 256.0;
}

constexpr auto move_stage(Generation const generation, MoveCriticalHit const move_adjustment) -> bounded::integer<0, 2> {
	switch (move_adjustment) {
		case MoveCriticalHit::never: std::unreachable();
		case MoveCriticalHit::normal: return 0_bi;
		case MoveCriticalHit::high: return BOUNDED_CONDITIONAL(generation == Generation::two, 2_bi, 1_bi);
		case MoveCriticalHit::very_high: return 2_bi;
		case MoveCriticalHit::always: std::unreachable();
	}
}

constexpr auto item_stage(Item const item, Species const species) -> bounded::integer<0, 2> {
	switch (item) {
		case Item::Razor_Claw:
		case Item::Scope_Lens:
			return 1_bi;
		case Item::Stick:
		case Item::Leek:
			return BOUNDED_CONDITIONAL(species == Species::Farfetchd, 2_bi, 0_bi);
		case Item::Lucky_Punch:
			return BOUNDED_CONDITIONAL(species == Species::Chansey, 2_bi, 0_bi);
		default:
			return 0_bi;
	}
}

constexpr auto ability_stage(Ability const ability) {
	return BOUNDED_CONDITIONAL(ability == Ability::Super_Luck, 1_bi, 0_bi);
}

template<any_active_pokemon ActivePokemonType>
constexpr auto boosted_stage(ActivePokemonType const attacker) {
	auto const boosted_value = BOUNDED_CONDITIONAL(generation_from<ActivePokemonType> == Generation::two, 1_bi, 2_bi);
	return BOUNDED_CONDITIONAL(attacker.has_focused_energy(), boosted_value, 0_bi);
}

constexpr auto critical_hit_rate_from_stage(Generation const generation, bounded::integer<0, 7> const stage) {
	switch (generation) {
		case Generation::one:
			std::unreachable();
		case Generation::two:
		case Generation::three:
		case Generation::four:
		case Generation::five:
			switch (stage.value()) {
				case 0: return 1.0 / 16.0;
				case 1: return 1.0 / 8.0;
				case 2: return 1.0 / 4.0;
				case 3: return 1.0 / 3.0;
				default: return 1.0 / 2.0;
			}
		case Generation::six:
			switch (stage.value()) {
				case 0: return 1.0 / 16.0;
				case 1: return 1.0 / 8.0;
				case 2: return 1.0 / 2.0;
				default: return 1.0;
			}
		case Generation::seven:
		case Generation::eight:
			switch (stage.value()) {
				case 0: return 1.0 / 24.0;
				case 1: return 1.0 / 8.0;
				case 2: return 1.0 / 2.0;
				default: return 1.0;
			}
	}
}

template<any_active_pokemon ActivePokemonType>
constexpr auto new_gen_critical_hit(ActivePokemonType const attacker, MoveCriticalHit const move_adjustment, Weather const weather) {
	constexpr auto generation = generation_from<ActivePokemonType>;
	switch (move_adjustment) {
		case MoveCriticalHit::never:
			return 0.0;
		case MoveCriticalHit::normal:
		case MoveCriticalHit::high:
		case MoveCriticalHit::very_high:
			return critical_hit_rate_from_stage(
				generation,
				(
					move_stage(generation, move_adjustment) +
					item_stage(attacker.item(weather), attacker.species()) +
					ability_stage(attacker.ability()) +
					boosted_stage(attacker)
				)
			);
		case MoveCriticalHit::always:
			return 1.0;
	}
}

template<any_active_pokemon ActivePokemonType>
constexpr auto base_critical_hit_probability(ActivePokemonType const attacker, MoveName const move_name, Weather const weather) {
	constexpr auto generation = generation_from<ActivePokemonType>;
	auto const move_adjustment = move_critical_hit(generation, move_name);
	switch (generation) {
		case Generation::one:
			return gen_one_critical_hit(attacker, move_adjustment);
		case Generation::two:
		case Generation::three:
		case Generation::four:
		case Generation::five:
		case Generation::six:
		case Generation::seven:
		case Generation::eight:
			return new_gen_critical_hit(attacker, move_adjustment, weather);
	}
}

export constexpr auto critical_hit_probability(any_active_pokemon auto const attacker, MoveName const move, Ability const defender_ability, Weather const weather) -> double {
	switch (defender_ability) {
		case Ability::Battle_Armor:
		case Ability::Shell_Armor:
			return 0.0;
		default:
			return base_critical_hit_probability(attacker, move, weather);
	}
}

#if 0

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION(generation) \
	template auto critical_hit_probability<ActivePokemon<generation>>(ActivePokemon<generation> const attacker, MoveName const move, Ability const defender_ability, Weather const weather) -> double

TECHNICALMACHINE_FOR_EACH_GENERATION(TECHNICALMACHINE_EXPLICIT_INSTANTIATION);

#endif

} // namespace technicalmachine