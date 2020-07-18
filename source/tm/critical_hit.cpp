// Copyright (C) 2020 David Stone
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

#include <tm/critical_hit.hpp>

#include <tm/pokemon/species.hpp>

namespace technicalmachine {

namespace {

enum class MoveCriticalHit {
	never,
	normal,
	high,
	very_high,
	always,
};

constexpr auto move_critical_hit(Generation const generation, Moves const move_name) {
	switch (move_name) {
		// Weird moves
		case Moves::Pass: return MoveCriticalHit::never;
		case Moves::Switch0: return MoveCriticalHit::never;
		case Moves::Switch1: return MoveCriticalHit::never;
		case Moves::Switch2: return MoveCriticalHit::never;
		case Moves::Switch3: return MoveCriticalHit::never;
		case Moves::Switch4: return MoveCriticalHit::never;
		case Moves::Switch5: return MoveCriticalHit::never;
		case Moves::Hit_Self: return MoveCriticalHit::never;
	
		// Generation 1
		case Moves::Pound: return MoveCriticalHit::normal;
		case Moves::Karate_Chop: return MoveCriticalHit::high;
		case Moves::Double_Slap: return MoveCriticalHit::normal;
		case Moves::Comet_Punch: return MoveCriticalHit::normal;
		case Moves::Mega_Punch: return MoveCriticalHit::normal;
		case Moves::Pay_Day: return MoveCriticalHit::normal;
		case Moves::Fire_Punch: return MoveCriticalHit::normal;
		case Moves::Ice_Punch: return MoveCriticalHit::normal;
		case Moves::Thunder_Punch: return MoveCriticalHit::normal;
		case Moves::Scratch: return MoveCriticalHit::normal;
		case Moves::Vice_Grip: return MoveCriticalHit::normal;
		case Moves::Guillotine: return MoveCriticalHit::never;
		case Moves::Razor_Wind:
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
		case Moves::Swords_Dance: return MoveCriticalHit::never;
		case Moves::Cut: return MoveCriticalHit::normal;
		case Moves::Gust: return MoveCriticalHit::normal;
		case Moves::Wing_Attack: return MoveCriticalHit::normal;
		case Moves::Whirlwind: return MoveCriticalHit::never;
		case Moves::Fly: return MoveCriticalHit::normal;
		case Moves::Bind: return MoveCriticalHit::normal;
		case Moves::Slam: return MoveCriticalHit::normal;
		case Moves::Vine_Whip: return MoveCriticalHit::normal;
		case Moves::Stomp: return MoveCriticalHit::normal;
		case Moves::Double_Kick: return MoveCriticalHit::normal;
		case Moves::Mega_Kick: return MoveCriticalHit::normal;
		case Moves::Jump_Kick: return MoveCriticalHit::normal;
		case Moves::Rolling_Kick: return MoveCriticalHit::normal;
		case Moves::Sand_Attack: return MoveCriticalHit::normal;
		case Moves::Headbutt: return MoveCriticalHit::normal;
		case Moves::Horn_Attack: return MoveCriticalHit::normal;
		case Moves::Fury_Attack: return MoveCriticalHit::normal;
		case Moves::Horn_Drill: return MoveCriticalHit::never;
		case Moves::Tackle: return MoveCriticalHit::normal;
		case Moves::Body_Slam: return MoveCriticalHit::normal;
		case Moves::Wrap: return MoveCriticalHit::normal;
		case Moves::Take_Down: return MoveCriticalHit::normal;
		case Moves::Thrash: return MoveCriticalHit::normal;
		case Moves::Double_Edge: return MoveCriticalHit::normal;
		case Moves::Tail_Whip: return MoveCriticalHit::never;
		case Moves::Poison_Sting: return MoveCriticalHit::normal;
		case Moves::Twineedle: return MoveCriticalHit::normal;
		case Moves::Pin_Missile: return MoveCriticalHit::normal;
		case Moves::Leer: return MoveCriticalHit::never;
		case Moves::Bite: return MoveCriticalHit::normal;
		case Moves::Growl: return MoveCriticalHit::never;
		case Moves::Roar: return MoveCriticalHit::never;
		case Moves::Sing: return MoveCriticalHit::never;
		case Moves::Supersonic: return MoveCriticalHit::never;
		case Moves::Sonic_Boom: return MoveCriticalHit::never;
		case Moves::Disable: return MoveCriticalHit::never;
		case Moves::Acid: return MoveCriticalHit::normal;
		case Moves::Ember: return MoveCriticalHit::normal;
		case Moves::Flamethrower: return MoveCriticalHit::normal;
		case Moves::Mist: return MoveCriticalHit::never;
		case Moves::Water_Gun: return MoveCriticalHit::normal;
		case Moves::Hydro_Pump: return MoveCriticalHit::normal;
		case Moves::Surf: return MoveCriticalHit::normal;
		case Moves::Ice_Beam: return MoveCriticalHit::normal;
		case Moves::Blizzard: return MoveCriticalHit::normal;
		case Moves::Psybeam: return MoveCriticalHit::normal;
		case Moves::Bubble_Beam: return MoveCriticalHit::normal;
		case Moves::Aurora_Beam: return MoveCriticalHit::normal;
		case Moves::Hyper_Beam: return MoveCriticalHit::normal;
		case Moves::Peck: return MoveCriticalHit::normal;
		case Moves::Drill_Peck: return MoveCriticalHit::normal;
		case Moves::Submission: return MoveCriticalHit::normal;
		case Moves::Low_Kick: return MoveCriticalHit::normal;
		case Moves::Counter: return MoveCriticalHit::never;
		case Moves::Seismic_Toss: return MoveCriticalHit::never;
		case Moves::Strength: return MoveCriticalHit::normal;
		case Moves::Absorb: return MoveCriticalHit::normal;
		case Moves::Mega_Drain: return MoveCriticalHit::normal;
		case Moves::Leech_Seed: return MoveCriticalHit::normal;
		case Moves::Growth: return MoveCriticalHit::never;
		case Moves::Razor_Leaf: return MoveCriticalHit::high;
		case Moves::Solar_Beam: return MoveCriticalHit::normal;
		case Moves::Poison_Powder: return MoveCriticalHit::never;
		case Moves::Stun_Spore: return MoveCriticalHit::never;
		case Moves::Sleep_Powder: return MoveCriticalHit::never;
		case Moves::Petal_Dance: return MoveCriticalHit::normal;
		case Moves::String_Shot: return MoveCriticalHit::never;
		case Moves::Dragon_Rage: return MoveCriticalHit::never;
		case Moves::Fire_Spin: return MoveCriticalHit::normal;
		case Moves::Thunder_Shock: return MoveCriticalHit::normal;
		case Moves::Thunderbolt: return MoveCriticalHit::normal;
		case Moves::Thunder_Wave: return MoveCriticalHit::never;
		case Moves::Thunder: return MoveCriticalHit::normal;
		case Moves::Rock_Throw: return MoveCriticalHit::normal;
		case Moves::Earthquake: return MoveCriticalHit::normal;
		case Moves::Fissure: return MoveCriticalHit::never;
		case Moves::Dig: return MoveCriticalHit::normal;
		case Moves::Toxic: return MoveCriticalHit::never;
		case Moves::Confusion: return MoveCriticalHit::normal;
		case Moves::Psychic: return MoveCriticalHit::normal;
		case Moves::Hypnosis: return MoveCriticalHit::never;
		case Moves::Meditate: return MoveCriticalHit::never;
		case Moves::Agility: return MoveCriticalHit::never;
		case Moves::Quick_Attack: return MoveCriticalHit::normal;
		case Moves::Rage: return MoveCriticalHit::normal;
		case Moves::Teleport: return MoveCriticalHit::never;
		case Moves::Night_Shade: return MoveCriticalHit::never;
		case Moves::Mimic: return MoveCriticalHit::never;
		case Moves::Screech: return MoveCriticalHit::never;
		case Moves::Double_Team: return MoveCriticalHit::never;
		case Moves::Recover: return MoveCriticalHit::never;
		case Moves::Harden: return MoveCriticalHit::never;
		case Moves::Minimize: return MoveCriticalHit::never;
		case Moves::Smokescreen: return MoveCriticalHit::never;
		case Moves::Confuse_Ray: return MoveCriticalHit::never;
		case Moves::Withdraw: return MoveCriticalHit::never;
		case Moves::Defense_Curl: return MoveCriticalHit::never;
		case Moves::Barrier: return MoveCriticalHit::never;
		case Moves::Light_Screen: return MoveCriticalHit::never;
		case Moves::Haze: return MoveCriticalHit::never;
		case Moves::Reflect: return MoveCriticalHit::never;
		case Moves::Focus_Energy: return MoveCriticalHit::never;
		case Moves::Bide: return MoveCriticalHit::never;
		case Moves::Metronome: return MoveCriticalHit::never;
		case Moves::Mirror_Move: return MoveCriticalHit::never;
		case Moves::Self_Destruct: return MoveCriticalHit::normal;
		case Moves::Egg_Bomb: return MoveCriticalHit::normal;
		case Moves::Lick: return MoveCriticalHit::normal;
		case Moves::Smog: return MoveCriticalHit::normal;
		case Moves::Sludge: return MoveCriticalHit::normal;
		case Moves::Bone_Club: return MoveCriticalHit::normal;
		case Moves::Fire_Blast: return MoveCriticalHit::normal;
		case Moves::Waterfall: return MoveCriticalHit::normal;
		case Moves::Clamp: return MoveCriticalHit::normal;
		case Moves::Swift: return MoveCriticalHit::normal;
		case Moves::Skull_Bash: return MoveCriticalHit::normal;
		case Moves::Spike_Cannon: return MoveCriticalHit::normal;
		case Moves::Constrict: return MoveCriticalHit::normal;
		case Moves::Amnesia: return MoveCriticalHit::never;
		case Moves::Kinesis: return MoveCriticalHit::never;
		case Moves::Soft_Boiled: return MoveCriticalHit::never;
		case Moves::High_Jump_Kick: return MoveCriticalHit::normal;
		case Moves::Glare: return MoveCriticalHit::never;
		case Moves::Dream_Eater: return MoveCriticalHit::normal;
		case Moves::Poison_Gas: return MoveCriticalHit::never;
		case Moves::Barrage: return MoveCriticalHit::normal;
		case Moves::Leech_Life: return MoveCriticalHit::normal;
		case Moves::Lovely_Kiss: return MoveCriticalHit::never;
		case Moves::Sky_Attack: return generation <= Generation::two ? MoveCriticalHit::normal : MoveCriticalHit::high;
		case Moves::Transform: return MoveCriticalHit::never;
		case Moves::Bubble: return MoveCriticalHit::normal;
		case Moves::Dizzy_Punch: return MoveCriticalHit::normal;
		case Moves::Spore: return MoveCriticalHit::never;
		case Moves::Flash: return MoveCriticalHit::never;
		case Moves::Psywave: return MoveCriticalHit::never;
		case Moves::Splash: return MoveCriticalHit::never;
		case Moves::Acid_Armor: return MoveCriticalHit::never;
		case Moves::Crabhammer: return MoveCriticalHit::high;
		case Moves::Explosion: return MoveCriticalHit::normal;
		case Moves::Fury_Swipes: return MoveCriticalHit::normal;
		case Moves::Bonemerang: return MoveCriticalHit::normal;
		case Moves::Rest: return MoveCriticalHit::never;
		case Moves::Rock_Slide: return MoveCriticalHit::normal;
		case Moves::Hyper_Fang: return MoveCriticalHit::normal;
		case Moves::Sharpen: return MoveCriticalHit::never;
		case Moves::Conversion: return MoveCriticalHit::never;
		case Moves::Tri_Attack: return MoveCriticalHit::normal;
		case Moves::Super_Fang: return MoveCriticalHit::never;
		case Moves::Slash: return MoveCriticalHit::high;
		case Moves::Substitute: return MoveCriticalHit::never;
		case Moves::Struggle: return MoveCriticalHit::never;

		// Generation 2
		case Moves::Sketch: return MoveCriticalHit::never;
		case Moves::Triple_Kick: return MoveCriticalHit::normal;
		case Moves::Thief: return MoveCriticalHit::normal;
		case Moves::Spider_Web: return MoveCriticalHit::never;
		case Moves::Mind_Reader: return MoveCriticalHit::never;
		case Moves::Nightmare: return MoveCriticalHit::never;
		case Moves::Flame_Wheel: return MoveCriticalHit::normal;
		case Moves::Snore: return MoveCriticalHit::normal;
		case Moves::Curse: return MoveCriticalHit::never;
		case Moves::Flail: return MoveCriticalHit::normal;
		case Moves::Conversion_2: return MoveCriticalHit::never;
		case Moves::Aeroblast: return MoveCriticalHit::high;
		case Moves::Cotton_Spore: return MoveCriticalHit::never;
		case Moves::Reversal: return MoveCriticalHit::normal;
		case Moves::Spite: return MoveCriticalHit::never;
		case Moves::Powder_Snow: return MoveCriticalHit::normal;
		case Moves::Protect: return MoveCriticalHit::never;
		case Moves::Mach_Punch: return MoveCriticalHit::normal;
		case Moves::Scary_Face: return MoveCriticalHit::never;
		case Moves::Feint_Attack: return MoveCriticalHit::normal;
		case Moves::Sweet_Kiss: return MoveCriticalHit::never;
		case Moves::Belly_Drum: return MoveCriticalHit::never;
		case Moves::Sludge_Bomb: return MoveCriticalHit::normal;
		case Moves::Mud_Slap: return MoveCriticalHit::normal;
		case Moves::Octazooka: return MoveCriticalHit::normal;
		case Moves::Spikes: return MoveCriticalHit::never;
		case Moves::Zap_Cannon: return MoveCriticalHit::normal;
		case Moves::Foresight: return MoveCriticalHit::never;
		case Moves::Destiny_Bond: return MoveCriticalHit::never;
		case Moves::Perish_Song: return MoveCriticalHit::never;
		case Moves::Icy_Wind: return MoveCriticalHit::normal;
		case Moves::Detect: return MoveCriticalHit::never;
		case Moves::Bone_Rush: return MoveCriticalHit::normal;
		case Moves::Lock_On: return MoveCriticalHit::never;
		case Moves::Outrage: return MoveCriticalHit::normal;
		case Moves::Sandstorm: return MoveCriticalHit::never;
		case Moves::Giga_Drain: return MoveCriticalHit::normal;
		case Moves::Endure: return MoveCriticalHit::never;
		case Moves::Charm: return MoveCriticalHit::never;
		case Moves::Rollout: return MoveCriticalHit::normal;
		case Moves::False_Swipe: return MoveCriticalHit::normal;
		case Moves::Swagger: return MoveCriticalHit::never;
		case Moves::Milk_Drink: return MoveCriticalHit::never;
		case Moves::Spark: return MoveCriticalHit::normal;
		case Moves::Fury_Cutter: return MoveCriticalHit::normal;
		case Moves::Steel_Wing: return MoveCriticalHit::normal;
		case Moves::Mean_Look: return MoveCriticalHit::never;
		case Moves::Attract: return MoveCriticalHit::never;
		case Moves::Sleep_Talk: return MoveCriticalHit::never;
		case Moves::Heal_Bell: return MoveCriticalHit::never;
		case Moves::Return: return MoveCriticalHit::normal;
		case Moves::Present: return MoveCriticalHit::normal;
		case Moves::Frustration: return MoveCriticalHit::normal;
		case Moves::Safeguard: return MoveCriticalHit::never;
		case Moves::Pain_Split: return MoveCriticalHit::never;
		case Moves::Sacred_Fire: return MoveCriticalHit::normal;
		case Moves::Magnitude: return MoveCriticalHit::normal;
		case Moves::Dynamic_Punch: return MoveCriticalHit::normal;
		case Moves::Megahorn: return MoveCriticalHit::normal;
		case Moves::Dragon_Breath: return MoveCriticalHit::normal;
		case Moves::Baton_Pass: return MoveCriticalHit::never;
		case Moves::Encore: return MoveCriticalHit::never;
		case Moves::Pursuit: return MoveCriticalHit::normal;
		case Moves::Rapid_Spin: return MoveCriticalHit::normal;
		case Moves::Sweet_Scent: return MoveCriticalHit::never;
		case Moves::Iron_Tail: return MoveCriticalHit::normal;
		case Moves::Metal_Claw: return MoveCriticalHit::normal;
		case Moves::Vital_Throw: return MoveCriticalHit::normal;
		case Moves::Morning_Sun: return MoveCriticalHit::never;
		case Moves::Synthesis: return MoveCriticalHit::never;
		case Moves::Moonlight: return MoveCriticalHit::never;
		case Moves::Hidden_Power: return MoveCriticalHit::normal;
		case Moves::Cross_Chop: return MoveCriticalHit::high;
		case Moves::Twister: return MoveCriticalHit::normal;
		case Moves::Rain_Dance: return MoveCriticalHit::never;
		case Moves::Sunny_Day: return MoveCriticalHit::never;
		case Moves::Crunch: return MoveCriticalHit::normal;
		case Moves::Mirror_Coat: return MoveCriticalHit::never;
		case Moves::Psych_Up: return MoveCriticalHit::never;
		case Moves::Extreme_Speed: return MoveCriticalHit::normal;
		case Moves::Ancient_Power: return MoveCriticalHit::normal;
		case Moves::Shadow_Ball: return MoveCriticalHit::normal;
		case Moves::Future_Sight: return MoveCriticalHit::never;
		case Moves::Rock_Smash: return MoveCriticalHit::normal;
		case Moves::Whirlpool: return MoveCriticalHit::normal;
		case Moves::Beat_Up: return MoveCriticalHit::never;
	
		// Generation 3
		case Moves::Fake_Out: return MoveCriticalHit::normal;
		case Moves::Uproar: return MoveCriticalHit::normal;
		case Moves::Stockpile: return MoveCriticalHit::never;
		case Moves::Spit_Up: return MoveCriticalHit::normal;
		case Moves::Swallow: return MoveCriticalHit::never;
		case Moves::Heat_Wave: return MoveCriticalHit::normal;
		case Moves::Hail: return MoveCriticalHit::never;
		case Moves::Torment: return MoveCriticalHit::never;
		case Moves::Flatter: return MoveCriticalHit::never;
		case Moves::Will_O_Wisp: return MoveCriticalHit::never;
		case Moves::Memento: return MoveCriticalHit::never;
		case Moves::Facade: return MoveCriticalHit::normal;
		case Moves::Focus_Punch: return MoveCriticalHit::normal;
		case Moves::Smelling_Salts: return MoveCriticalHit::normal;
		case Moves::Follow_Me: return MoveCriticalHit::never;
		case Moves::Nature_Power: return MoveCriticalHit::never;
		case Moves::Charge: return MoveCriticalHit::never;
		case Moves::Taunt: return MoveCriticalHit::never;
		case Moves::Helping_Hand: return MoveCriticalHit::never;
		case Moves::Trick: return MoveCriticalHit::never;
		case Moves::Role_Play: return MoveCriticalHit::never;
		case Moves::Wish: return MoveCriticalHit::never;
		case Moves::Assist: return MoveCriticalHit::never;
		case Moves::Ingrain: return MoveCriticalHit::never;
		case Moves::Superpower: return MoveCriticalHit::normal;
		case Moves::Magic_Coat: return MoveCriticalHit::never;
		case Moves::Recycle: return MoveCriticalHit::never;
		case Moves::Revenge: return MoveCriticalHit::normal;
		case Moves::Brick_Break: return MoveCriticalHit::normal;
		case Moves::Yawn: return MoveCriticalHit::never;
		case Moves::Knock_Off: return MoveCriticalHit::normal;
		case Moves::Endeavor: return MoveCriticalHit::never;
		case Moves::Eruption: return MoveCriticalHit::normal;
		case Moves::Skill_Swap: return MoveCriticalHit::never;
		case Moves::Imprison: return MoveCriticalHit::never;
		case Moves::Refresh: return MoveCriticalHit::never;
		case Moves::Grudge: return MoveCriticalHit::never;
		case Moves::Snatch: return MoveCriticalHit::never;
		case Moves::Secret_Power: return MoveCriticalHit::normal;
		case Moves::Dive: return MoveCriticalHit::normal;
		case Moves::Arm_Thrust: return MoveCriticalHit::normal;
		case Moves::Camouflage: return MoveCriticalHit::never;
		case Moves::Tail_Glow: return MoveCriticalHit::never;
		case Moves::Luster_Purge: return MoveCriticalHit::normal;
		case Moves::Mist_Ball: return MoveCriticalHit::normal;
		case Moves::Feather_Dance: return MoveCriticalHit::never;
		case Moves::Teeter_Dance: return MoveCriticalHit::never;
		case Moves::Blaze_Kick: return MoveCriticalHit::high;
		case Moves::Mud_Sport: return MoveCriticalHit::never;
		case Moves::Ice_Ball: return MoveCriticalHit::normal;
		case Moves::Needle_Arm: return MoveCriticalHit::normal;
		case Moves::Slack_Off: return MoveCriticalHit::never;
		case Moves::Hyper_Voice: return MoveCriticalHit::normal;
		case Moves::Poison_Fang: return MoveCriticalHit::normal;
		case Moves::Crush_Claw: return MoveCriticalHit::normal;
		case Moves::Blast_Burn: return MoveCriticalHit::normal;
		case Moves::Hydro_Cannon: return MoveCriticalHit::normal;
		case Moves::Meteor_Mash: return MoveCriticalHit::normal;
		case Moves::Astonish: return MoveCriticalHit::normal;
		case Moves::Weather_Ball: return MoveCriticalHit::normal;
		case Moves::Aromatherapy: return MoveCriticalHit::never;
		case Moves::Fake_Tears: return MoveCriticalHit::never;
		case Moves::Air_Cutter: return MoveCriticalHit::high;
		case Moves::Overheat: return MoveCriticalHit::normal;
		case Moves::Odor_Sleuth: return MoveCriticalHit::never;
		case Moves::Rock_Tomb: return MoveCriticalHit::normal;
		case Moves::Silver_Wind: return MoveCriticalHit::normal;
		case Moves::Metal_Sound: return MoveCriticalHit::never;
		case Moves::Grass_Whistle: return MoveCriticalHit::never;
		case Moves::Tickle: return MoveCriticalHit::never;
		case Moves::Cosmic_Power: return MoveCriticalHit::never;
		case Moves::Water_Spout: return MoveCriticalHit::normal;
		case Moves::Signal_Beam: return MoveCriticalHit::normal;
		case Moves::Shadow_Punch: return MoveCriticalHit::normal;
		case Moves::Extrasensory: return MoveCriticalHit::normal;
		case Moves::Sky_Uppercut: return MoveCriticalHit::normal;
		case Moves::Sand_Tomb: return MoveCriticalHit::normal;
		case Moves::Sheer_Cold: return MoveCriticalHit::never;
		case Moves::Muddy_Water: return MoveCriticalHit::normal;
		case Moves::Bullet_Seed: return MoveCriticalHit::normal;
		case Moves::Aerial_Ace: return MoveCriticalHit::normal;
		case Moves::Icicle_Spear: return MoveCriticalHit::normal;
		case Moves::Iron_Defense: return MoveCriticalHit::never;
		case Moves::Block: return MoveCriticalHit::never;
		case Moves::Howl: return MoveCriticalHit::never;
		case Moves::Dragon_Claw: return MoveCriticalHit::normal;
		case Moves::Frenzy_Plant: return MoveCriticalHit::normal;
		case Moves::Bulk_Up: return MoveCriticalHit::never;
		case Moves::Bounce: return MoveCriticalHit::normal;
		case Moves::Mud_Shot: return MoveCriticalHit::normal;
		case Moves::Poison_Tail: return MoveCriticalHit::high;
		case Moves::Covet: return MoveCriticalHit::normal;
		case Moves::Volt_Tackle: return MoveCriticalHit::normal;
		case Moves::Magical_Leaf: return MoveCriticalHit::normal;
		case Moves::Water_Sport: return MoveCriticalHit::never;
		case Moves::Calm_Mind: return MoveCriticalHit::never;
		case Moves::Leaf_Blade: return MoveCriticalHit::high;
		case Moves::Dragon_Dance: return MoveCriticalHit::never;
		case Moves::Rock_Blast: return MoveCriticalHit::normal;
		case Moves::Shock_Wave: return MoveCriticalHit::normal;
		case Moves::Water_Pulse: return MoveCriticalHit::normal;
		case Moves::Doom_Desire: return MoveCriticalHit::never;
		case Moves::Psycho_Boost: return MoveCriticalHit::normal;
	
		// Generation 4
		case Moves::Roost: return MoveCriticalHit::never;
		case Moves::Gravity: return MoveCriticalHit::never;
		case Moves::Miracle_Eye: return MoveCriticalHit::never;
		case Moves::Wake_Up_Slap: return MoveCriticalHit::normal;
		case Moves::Hammer_Arm: return MoveCriticalHit::normal;
		case Moves::Gyro_Ball: return MoveCriticalHit::normal;
		case Moves::Healing_Wish: return MoveCriticalHit::never;
		case Moves::Brine: return MoveCriticalHit::normal;
		case Moves::Natural_Gift: return MoveCriticalHit::normal;
		case Moves::Feint: return MoveCriticalHit::normal;
		case Moves::Pluck: return MoveCriticalHit::normal;
		case Moves::Tailwind: return MoveCriticalHit::never;
		case Moves::Acupressure: return MoveCriticalHit::never;
		case Moves::Metal_Burst: return MoveCriticalHit::never;
		case Moves::U_turn: return MoveCriticalHit::normal;
		case Moves::Close_Combat: return MoveCriticalHit::normal;
		case Moves::Payback: return MoveCriticalHit::normal;
		case Moves::Assurance: return MoveCriticalHit::normal;
		case Moves::Embargo: return MoveCriticalHit::never;
		case Moves::Fling: return MoveCriticalHit::normal;
		case Moves::Psycho_Shift: return MoveCriticalHit::never;
		case Moves::Trump_Card: return MoveCriticalHit::normal;
		case Moves::Heal_Block: return MoveCriticalHit::never;
		case Moves::Wring_Out: return MoveCriticalHit::normal;
		case Moves::Power_Trick: return MoveCriticalHit::never;
		case Moves::Gastro_Acid: return MoveCriticalHit::never;
		case Moves::Lucky_Chant: return MoveCriticalHit::never;
		case Moves::Me_First: return MoveCriticalHit::never;
		case Moves::Copycat: return MoveCriticalHit::never;
		case Moves::Power_Swap: return MoveCriticalHit::never;
		case Moves::Guard_Swap: return MoveCriticalHit::never;
		case Moves::Punishment: return MoveCriticalHit::normal;
		case Moves::Last_Resort: return MoveCriticalHit::normal;
		case Moves::Worry_Seed: return MoveCriticalHit::never;
		case Moves::Sucker_Punch: return MoveCriticalHit::normal;
		case Moves::Toxic_Spikes: return MoveCriticalHit::never;
		case Moves::Heart_Swap: return MoveCriticalHit::never;
		case Moves::Aqua_Ring: return MoveCriticalHit::never;
		case Moves::Magnet_Rise: return MoveCriticalHit::never;
		case Moves::Flare_Blitz: return MoveCriticalHit::normal;
		case Moves::Force_Palm: return MoveCriticalHit::normal;
		case Moves::Aura_Sphere: return MoveCriticalHit::normal;
		case Moves::Rock_Polish: return MoveCriticalHit::never;
		case Moves::Poison_Jab: return MoveCriticalHit::normal;
		case Moves::Dark_Pulse: return MoveCriticalHit::normal;
		case Moves::Night_Slash: return MoveCriticalHit::high;
		case Moves::Aqua_Tail: return MoveCriticalHit::normal;
		case Moves::Seed_Bomb: return MoveCriticalHit::normal;
		case Moves::Air_Slash: return MoveCriticalHit::normal;
		case Moves::X_Scissor: return MoveCriticalHit::normal;
		case Moves::Bug_Buzz: return MoveCriticalHit::normal;
		case Moves::Dragon_Pulse: return MoveCriticalHit::normal;
		case Moves::Dragon_Rush: return MoveCriticalHit::normal;
		case Moves::Power_Gem: return MoveCriticalHit::normal;
		case Moves::Drain_Punch: return MoveCriticalHit::normal;
		case Moves::Vacuum_Wave: return MoveCriticalHit::normal;
		case Moves::Focus_Blast: return MoveCriticalHit::normal;
		case Moves::Energy_Ball: return MoveCriticalHit::normal;
		case Moves::Brave_Bird: return MoveCriticalHit::normal;
		case Moves::Earth_Power: return MoveCriticalHit::normal;
		case Moves::Switcheroo: return MoveCriticalHit::never;
		case Moves::Giga_Impact: return MoveCriticalHit::normal;
		case Moves::Nasty_Plot: return MoveCriticalHit::never;
		case Moves::Bullet_Punch: return MoveCriticalHit::normal;
		case Moves::Avalanche: return MoveCriticalHit::normal;
		case Moves::Ice_Shard: return MoveCriticalHit::normal;
		case Moves::Shadow_Claw: return MoveCriticalHit::high;
		case Moves::Thunder_Fang: return MoveCriticalHit::normal;
		case Moves::Ice_Fang: return MoveCriticalHit::normal;
		case Moves::Fire_Fang: return MoveCriticalHit::normal;
		case Moves::Shadow_Sneak: return MoveCriticalHit::normal;
		case Moves::Mud_Bomb: return MoveCriticalHit::normal;
		case Moves::Psycho_Cut: return MoveCriticalHit::high;
		case Moves::Zen_Headbutt: return MoveCriticalHit::normal;
		case Moves::Mirror_Shot: return MoveCriticalHit::normal;
		case Moves::Flash_Cannon: return MoveCriticalHit::normal;
		case Moves::Rock_Climb: return MoveCriticalHit::normal;
		case Moves::Defog: return MoveCriticalHit::never;
		case Moves::Trick_Room: return MoveCriticalHit::never;
		case Moves::Draco_Meteor: return MoveCriticalHit::normal;
		case Moves::Discharge: return MoveCriticalHit::normal;
		case Moves::Lava_Plume: return MoveCriticalHit::normal;
		case Moves::Leaf_Storm: return MoveCriticalHit::normal;
		case Moves::Power_Whip: return MoveCriticalHit::normal;
		case Moves::Rock_Wrecker: return MoveCriticalHit::normal;
		case Moves::Cross_Poison: return MoveCriticalHit::high;
		case Moves::Gunk_Shot: return MoveCriticalHit::normal;
		case Moves::Iron_Head: return MoveCriticalHit::normal;
		case Moves::Magnet_Bomb: return MoveCriticalHit::normal;
		case Moves::Stone_Edge: return MoveCriticalHit::high;
		case Moves::Captivate: return MoveCriticalHit::never;
		case Moves::Stealth_Rock: return MoveCriticalHit::never;
		case Moves::Grass_Knot: return MoveCriticalHit::normal;
		case Moves::Chatter: return MoveCriticalHit::normal;
		case Moves::Judgment: return MoveCriticalHit::normal;
		case Moves::Bug_Bite: return MoveCriticalHit::normal;
		case Moves::Charge_Beam: return MoveCriticalHit::normal;
		case Moves::Wood_Hammer: return MoveCriticalHit::normal;
		case Moves::Aqua_Jet: return MoveCriticalHit::normal;
		case Moves::Attack_Order: return MoveCriticalHit::high;
		case Moves::Defend_Order: return MoveCriticalHit::never;
		case Moves::Heal_Order: return MoveCriticalHit::never;
		case Moves::Head_Smash: return MoveCriticalHit::normal;
		case Moves::Double_Hit: return MoveCriticalHit::normal;
		case Moves::Roar_of_Time: return MoveCriticalHit::normal;
		case Moves::Spacial_Rend: return MoveCriticalHit::high;
		case Moves::Lunar_Dance: return MoveCriticalHit::never;
		case Moves::Crush_Grip: return MoveCriticalHit::normal;
		case Moves::Magma_Storm: return MoveCriticalHit::normal;
		case Moves::Dark_Void: return MoveCriticalHit::never;
		case Moves::Seed_Flare: return MoveCriticalHit::normal;
		case Moves::Ominous_Wind: return MoveCriticalHit::normal;
		case Moves::Shadow_Force: return MoveCriticalHit::normal;
	
		// Generation 5
		case Moves::Hone_Claws: return MoveCriticalHit::never;
		case Moves::Wide_Guard: return MoveCriticalHit::never;
		case Moves::Guard_Split: return MoveCriticalHit::never;
		case Moves::Power_Split: return MoveCriticalHit::never;
		case Moves::Wonder_Room: return MoveCriticalHit::never;
		case Moves::Psyshock: return MoveCriticalHit::normal;
		case Moves::Venoshock: return MoveCriticalHit::normal;
		case Moves::Autotomize: return MoveCriticalHit::never;
		case Moves::Rage_Powder: return MoveCriticalHit::never;
		case Moves::Telekinesis: return MoveCriticalHit::never;
		case Moves::Magic_Room: return MoveCriticalHit::never;
		case Moves::Smack_Down: return MoveCriticalHit::normal;
		case Moves::Storm_Throw: return MoveCriticalHit::always;
		case Moves::Flame_Burst: return MoveCriticalHit::normal;
		case Moves::Sludge_Wave: return MoveCriticalHit::normal;
		case Moves::Quiver_Dance: return MoveCriticalHit::never;
		case Moves::Heavy_Slam: return MoveCriticalHit::normal;
		case Moves::Synchronoise: return MoveCriticalHit::normal;
		case Moves::Electro_Ball: return MoveCriticalHit::normal;
		case Moves::Soak: return MoveCriticalHit::never;
		case Moves::Flame_Charge: return MoveCriticalHit::normal;
		case Moves::Coil: return MoveCriticalHit::never;
		case Moves::Low_Sweep: return MoveCriticalHit::normal;
		case Moves::Acid_Spray: return MoveCriticalHit::normal;
		case Moves::Foul_Play: return MoveCriticalHit::normal;
		case Moves::Simple_Beam: return MoveCriticalHit::never;
		case Moves::Entrainment: return MoveCriticalHit::never;
		case Moves::After_You: return MoveCriticalHit::never;
		case Moves::Round: return MoveCriticalHit::normal;
		case Moves::Echoed_Voice: return MoveCriticalHit::normal;
		case Moves::Chip_Away: return MoveCriticalHit::normal;
		case Moves::Clear_Smog: return MoveCriticalHit::normal;
		case Moves::Stored_Power: return MoveCriticalHit::normal;
		case Moves::Quick_Guard: return MoveCriticalHit::never;
		case Moves::Ally_Switch: return MoveCriticalHit::never;
		case Moves::Scald: return MoveCriticalHit::normal;
		case Moves::Shell_Smash: return MoveCriticalHit::never;
		case Moves::Heal_Pulse: return MoveCriticalHit::never;
		case Moves::Hex: return MoveCriticalHit::normal;
		case Moves::Sky_Drop: return MoveCriticalHit::normal;
		case Moves::Shift_Gear: return MoveCriticalHit::never;
		case Moves::Circle_Throw: return MoveCriticalHit::normal;
		case Moves::Incinerate: return MoveCriticalHit::normal;
		case Moves::Quash: return MoveCriticalHit::never;
		case Moves::Acrobatics: return MoveCriticalHit::normal;
		case Moves::Reflect_Type: return MoveCriticalHit::never;
		case Moves::Retaliate: return MoveCriticalHit::normal;
		case Moves::Final_Gambit: return MoveCriticalHit::normal;
		case Moves::Bestow: return MoveCriticalHit::never;
		case Moves::Inferno: return MoveCriticalHit::normal;
		case Moves::Water_Pledge: return MoveCriticalHit::normal;
		case Moves::Fire_Pledge: return MoveCriticalHit::normal;
		case Moves::Grass_Pledge: return MoveCriticalHit::normal;
		case Moves::Volt_Switch: return MoveCriticalHit::normal;
		case Moves::Struggle_Bug: return MoveCriticalHit::normal;
		case Moves::Bulldoze: return MoveCriticalHit::normal;
		case Moves::Frost_Breath: return MoveCriticalHit::always;
		case Moves::Dragon_Tail: return MoveCriticalHit::normal;
		case Moves::Work_Up: return MoveCriticalHit::never;
		case Moves::Electroweb: return MoveCriticalHit::normal;
		case Moves::Wild_Charge: return MoveCriticalHit::normal;
		case Moves::Drill_Run: return MoveCriticalHit::high;
		case Moves::Dual_Chop: return MoveCriticalHit::normal;
		case Moves::Heart_Stamp: return MoveCriticalHit::normal;
		case Moves::Horn_Leech: return MoveCriticalHit::normal;
		case Moves::Sacred_Sword: return MoveCriticalHit::normal;
		case Moves::Razor_Shell: return MoveCriticalHit::normal;
		case Moves::Heat_Crash: return MoveCriticalHit::normal;
		case Moves::Leaf_Tornado: return MoveCriticalHit::normal;
		case Moves::Steamroller: return MoveCriticalHit::normal;
		case Moves::Cotton_Guard: return MoveCriticalHit::never;
		case Moves::Night_Daze: return MoveCriticalHit::normal;
		case Moves::Psystrike: return MoveCriticalHit::normal;
		case Moves::Tail_Slap: return MoveCriticalHit::normal;
		case Moves::Hurricane: return MoveCriticalHit::normal;
		case Moves::Head_Charge: return MoveCriticalHit::normal;
		case Moves::Gear_Grind: return MoveCriticalHit::normal;
		case Moves::Searing_Shot: return MoveCriticalHit::normal;
		case Moves::Techno_Blast: return MoveCriticalHit::normal;
		case Moves::Relic_Song: return MoveCriticalHit::normal;
		case Moves::Secret_Sword: return MoveCriticalHit::normal;
		case Moves::Glaciate: return MoveCriticalHit::normal;
		case Moves::Bolt_Strike: return MoveCriticalHit::normal;
		case Moves::Blue_Flare: return MoveCriticalHit::normal;
		case Moves::Fiery_Dance: return MoveCriticalHit::normal;
		case Moves::Freeze_Shock: return MoveCriticalHit::normal;
		case Moves::Ice_Burn: return MoveCriticalHit::normal;
		case Moves::Snarl: return MoveCriticalHit::normal;
		case Moves::Icicle_Crash: return MoveCriticalHit::normal;
		case Moves::V_create: return MoveCriticalHit::normal;
		case Moves::Fusion_Flare: return MoveCriticalHit::normal;
		case Moves::Fusion_Bolt: return MoveCriticalHit::normal;
	
		// Generation 6
		case Moves::Flying_Press: return MoveCriticalHit::normal;
		case Moves::Mat_Block: return MoveCriticalHit::never;
		case Moves::Belch: return MoveCriticalHit::normal;
		case Moves::Rototiller: return MoveCriticalHit::never;
		case Moves::Sticky_Web: return MoveCriticalHit::never;
		case Moves::Fell_Stinger: return MoveCriticalHit::normal;
		case Moves::Phantom_Force: return MoveCriticalHit::normal;
		case Moves::Trick_or_Treat: return MoveCriticalHit::never;
		case Moves::Noble_Roar: return MoveCriticalHit::never;
		case Moves::Ion_Deluge: return MoveCriticalHit::never;
		case Moves::Parabolic_Charge: return MoveCriticalHit::normal;
		case Moves::Forests_Curse: return MoveCriticalHit::never;
		case Moves::Petal_Blizzard: return MoveCriticalHit::normal;
		case Moves::Freeze_Dry: return MoveCriticalHit::normal;
		case Moves::Disarming_Voice: return MoveCriticalHit::normal;
		case Moves::Parting_Shot: return MoveCriticalHit::never;
		case Moves::Topsy_Turvy: return MoveCriticalHit::never;
		case Moves::Draining_Kiss: return MoveCriticalHit::normal;
		case Moves::Crafty_Shield: return MoveCriticalHit::never;
		case Moves::Flower_Shield: return MoveCriticalHit::never;
		case Moves::Grassy_Terrain: return MoveCriticalHit::never;
		case Moves::Misty_Terrain: return MoveCriticalHit::never;
		case Moves::Electrify: return MoveCriticalHit::never;
		case Moves::Play_Rough: return MoveCriticalHit::normal;
		case Moves::Fairy_Wind: return MoveCriticalHit::normal;
		case Moves::Moonblast: return MoveCriticalHit::normal;
		case Moves::Boomburst: return MoveCriticalHit::normal;
		case Moves::Fairy_Lock: return MoveCriticalHit::never;
		case Moves::Kings_Shield: return MoveCriticalHit::never;
		case Moves::Play_Nice: return MoveCriticalHit::never;
		case Moves::Confide: return MoveCriticalHit::never;
		case Moves::Diamond_Storm: return MoveCriticalHit::normal;
		case Moves::Steam_Eruption: return MoveCriticalHit::normal;
		case Moves::Hyperspace_Hole: return MoveCriticalHit::normal;
		case Moves::Water_Shuriken: return MoveCriticalHit::normal;
		case Moves::Mystical_Fire: return MoveCriticalHit::normal;
		case Moves::Spiky_Shield: return MoveCriticalHit::never;
		case Moves::Aromatic_Mist: return MoveCriticalHit::never;
		case Moves::Eerie_Impulse: return MoveCriticalHit::never;
		case Moves::Venom_Drench: return MoveCriticalHit::never;
		case Moves::Powder: return MoveCriticalHit::never;
		case Moves::Geomancy: return MoveCriticalHit::never;
		case Moves::Magnetic_Flux: return MoveCriticalHit::never;
		case Moves::Happy_Hour: return MoveCriticalHit::never;
		case Moves::Electric_Terrain: return MoveCriticalHit::never;
		case Moves::Dazzling_Gleam: return MoveCriticalHit::normal;
		case Moves::Celebrate: return MoveCriticalHit::never;
		case Moves::Hold_Hands: return MoveCriticalHit::never;
		case Moves::Baby_Doll_Eyes: return MoveCriticalHit::never;
		case Moves::Nuzzle: return MoveCriticalHit::normal;
		case Moves::Hold_Back: return MoveCriticalHit::normal;
		case Moves::Infestation: return MoveCriticalHit::normal;
		case Moves::Power_Up_Punch: return MoveCriticalHit::normal;
		case Moves::Oblivion_Wing: return MoveCriticalHit::normal;
		case Moves::Thousand_Arrows: return MoveCriticalHit::normal;
		case Moves::Thousand_Waves: return MoveCriticalHit::normal;
		case Moves::Lands_Wrath: return MoveCriticalHit::normal;
		case Moves::Light_of_Ruin: return MoveCriticalHit::normal;
		case Moves::Origin_Pulse: return MoveCriticalHit::normal;
		case Moves::Precipice_Blades: return MoveCriticalHit::normal;
		case Moves::Dragon_Ascent: return MoveCriticalHit::normal;
		case Moves::Hyperspace_Fury: return MoveCriticalHit::normal;
	
		// Generation 7
		case Moves::Breakneck_Blitz: return MoveCriticalHit::never;
		case Moves::All_Out_Pummeling: return MoveCriticalHit::never;
		case Moves::Supersonic_Skystrike: return MoveCriticalHit::never;
		case Moves::Acid_Downpour: return MoveCriticalHit::never;
		case Moves::Tectonic_Rage: return MoveCriticalHit::never;
		case Moves::Continental_Crush: return MoveCriticalHit::never;
		case Moves::Savage_Spin_Out: return MoveCriticalHit::never;
		case Moves::Never_Ending_Nightmare: return MoveCriticalHit::never;
		case Moves::Corkscrew_Crash: return MoveCriticalHit::never;
		case Moves::Inferno_Overdrive: return MoveCriticalHit::never;
		case Moves::Hydro_Vortex: return MoveCriticalHit::never;
		case Moves::Bloom_Doom: return MoveCriticalHit::never;
		case Moves::Gigavolt_Havoc: return MoveCriticalHit::never;
		case Moves::Shattered_Psyche: return MoveCriticalHit::never;
		case Moves::Subzero_Slammer: return MoveCriticalHit::never;
		case Moves::Devastating_Drake: return MoveCriticalHit::never;
		case Moves::Black_Hole_Eclipse: return MoveCriticalHit::never;
		case Moves::Twinkle_Tackle: return MoveCriticalHit::never;
		case Moves::Catastropika: return MoveCriticalHit::normal;
		case Moves::Shore_Up: return MoveCriticalHit::never;
		case Moves::First_Impression: return MoveCriticalHit::normal;
		case Moves::Baneful_Bunker: return MoveCriticalHit::never;
		case Moves::Spirit_Shackle: return MoveCriticalHit::normal;
		case Moves::Darkest_Lariat: return MoveCriticalHit::normal;
		case Moves::Sparkling_Aria: return MoveCriticalHit::normal;
		case Moves::Ice_Hammer: return MoveCriticalHit::normal;
		case Moves::Floral_Healing: return MoveCriticalHit::never;
		case Moves::High_Horsepower: return MoveCriticalHit::normal;
		case Moves::Strength_Sap: return MoveCriticalHit::never;
		case Moves::Solar_Blade: return MoveCriticalHit::normal;
		case Moves::Leafage: return MoveCriticalHit::normal;
		case Moves::Spotlight: return MoveCriticalHit::never;
		case Moves::Toxic_Thread: return MoveCriticalHit::never;
		case Moves::Laser_Focus: return MoveCriticalHit::never;
		case Moves::Gear_Up: return MoveCriticalHit::never;
		case Moves::Throat_Chop: return MoveCriticalHit::normal;
		case Moves::Pollen_Puff: return MoveCriticalHit::normal;
		case Moves::Anchor_Shot: return MoveCriticalHit::normal;
		case Moves::Psychic_Terrain: return MoveCriticalHit::never;
		case Moves::Lunge: return MoveCriticalHit::normal;
		case Moves::Fire_Lash: return MoveCriticalHit::normal;
		case Moves::Power_Trip: return MoveCriticalHit::normal;
		case Moves::Burn_Up: return MoveCriticalHit::normal;
		case Moves::Speed_Swap: return MoveCriticalHit::never;
		case Moves::Smart_Strike: return MoveCriticalHit::normal;
		case Moves::Purify: return MoveCriticalHit::never;
		case Moves::Revelation_Dance: return MoveCriticalHit::normal;
		case Moves::Core_Enforcer: return MoveCriticalHit::normal;
		case Moves::Trop_Kick: return MoveCriticalHit::normal;
		case Moves::Instruct: return MoveCriticalHit::never;
		case Moves::Beak_Blast: return MoveCriticalHit::normal;
		case Moves::Clanging_Scales: return MoveCriticalHit::normal;
		case Moves::Dragon_Hammer: return MoveCriticalHit::normal;
		case Moves::Brutal_Swing: return MoveCriticalHit::normal;
		case Moves::Aurora_Veil: return MoveCriticalHit::never;
		case Moves::Sinister_Arrow_Raid: return MoveCriticalHit::normal;
		case Moves::Malicious_Moonsault: return MoveCriticalHit::normal;
		case Moves::Oceanic_Operetta: return MoveCriticalHit::normal;
		case Moves::Guardian_of_Alola: return MoveCriticalHit::never;
		case Moves::Soul_Stealing_7_Star_Strike: return MoveCriticalHit::normal;
		case Moves::Stoked_Sparksurfer: return MoveCriticalHit::normal;
		case Moves::Pulverizing_Pancake: return MoveCriticalHit::normal;
		case Moves::Extreme_Evoboost: return MoveCriticalHit::never;
		case Moves::Genesis_Supernova: return MoveCriticalHit::normal;
		case Moves::Shell_Trap: return MoveCriticalHit::normal;
		case Moves::Fleur_Cannon: return MoveCriticalHit::normal;
		case Moves::Psychic_Fangs: return MoveCriticalHit::normal;
		case Moves::Stomping_Tantrum: return MoveCriticalHit::normal;
		case Moves::Shadow_Bone: return MoveCriticalHit::normal;
		case Moves::Accelerock: return MoveCriticalHit::normal;
		case Moves::Liquidation: return MoveCriticalHit::normal;
		case Moves::Prismatic_Laser: return MoveCriticalHit::normal;
		case Moves::Spectral_Thief: return MoveCriticalHit::normal;
		case Moves::Sunsteel_Strike: return MoveCriticalHit::normal;
		case Moves::Moongeist_Beam: return MoveCriticalHit::normal;
		case Moves::Tearful_Look: return MoveCriticalHit::never;
		case Moves::Zing_Zap: return MoveCriticalHit::normal;
		case Moves::Natures_Madness: return MoveCriticalHit::never;
		case Moves::Multi_Attack: return MoveCriticalHit::normal;
		case Moves::m10000000_Volt_Thunderbolt: return MoveCriticalHit::very_high;
		case Moves::Mind_Blown: return MoveCriticalHit::normal;
		case Moves::Plasma_Fists: return MoveCriticalHit::normal;
		case Moves::Photon_Geyser: return MoveCriticalHit::normal;
		case Moves::Light_That_Burns_the_Sky: return MoveCriticalHit::normal;
		case Moves::Searing_Sunraze_Smash: return MoveCriticalHit::normal;
		case Moves::Menacing_Moonraze_Maelstrom: return MoveCriticalHit::normal;
		case Moves::Lets_Snuggle_Forever: return MoveCriticalHit::normal;
		case Moves::Splintered_Stormshards: return MoveCriticalHit::normal;
		case Moves::Clangorous_Soulblaze: return MoveCriticalHit::normal;
		case Moves::Zippy_Zap: return MoveCriticalHit::always;
		case Moves::Splishy_Splash: return MoveCriticalHit::normal;
		case Moves::Floaty_Fall: return MoveCriticalHit::normal;
		case Moves::Pika_Papow: return MoveCriticalHit::never;
		case Moves::Bouncy_Bubble: return MoveCriticalHit::normal;
		case Moves::Buzzy_Buzz: return MoveCriticalHit::normal;
		case Moves::Sizzly_Slide: return MoveCriticalHit::normal;
		case Moves::Glitzy_Glow: return MoveCriticalHit::normal;
		case Moves::Baddy_Bad: return MoveCriticalHit::normal;
		case Moves::Sappy_Seed: return MoveCriticalHit::normal;
		case Moves::Freezy_Frost: return MoveCriticalHit::normal;
		case Moves::Sparkly_Swirl: return MoveCriticalHit::normal;
		case Moves::Veevee_Volley: return MoveCriticalHit::never;
		case Moves::Double_Iron_Bash: return MoveCriticalHit::normal;
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

template<Generation generation>
auto gen_one_critical_hit(ActivePokemon<generation> const attacker, MoveCriticalHit const move_adjustment) {
	auto const initial = BaseStats(Generation::one, attacker.species()).spe() / 2_bi;
	auto const focused = BOUNDED_CONDITIONAL(attacker.has_focused_energy(), initial / 2_bi, initial * 2_bi);
	auto const final = bounded::min(255_bi, focused * generation_one_move_critical_hit_multiplier(move_adjustment));
	return double(final) / 256.0;
}

auto move_stage(Generation const generation, MoveCriticalHit const move_adjustment) -> bounded::integer<0, 2> {
	switch (move_adjustment) {
		case MoveCriticalHit::never: bounded::unreachable();
		case MoveCriticalHit::normal: return 0_bi;
		case MoveCriticalHit::high: return BOUNDED_CONDITIONAL(generation == Generation::two, 2_bi, 1_bi);
		case MoveCriticalHit::very_high: return 2_bi;
		case MoveCriticalHit::always: bounded::unreachable();
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

template<Generation generation>
auto boosted_stage(ActivePokemon<generation> const attacker) {
	auto const boosted_value = BOUNDED_CONDITIONAL(generation == Generation::two, 1_bi, 2_bi);
	return BOUNDED_CONDITIONAL(attacker.has_focused_energy(), boosted_value, 0_bi);
}

constexpr auto critical_hit_rate_from_stage(Generation const generation, bounded::integer<0, 7> const stage) {
	switch (generation) {
		case Generation::one:
			bounded::unreachable();
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

template<Generation generation>
auto new_gen_critical_hit(ActivePokemon<generation> const attacker, MoveCriticalHit const move_adjustment, Weather const weather) {
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

template<Generation generation>
auto base_critical_hit_probability(ActivePokemon<generation> const attacker, Moves const move_name, Weather const weather) {
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

} // namespace

template<Generation generation>
auto critical_hit_probability(ActivePokemon<generation> const attacker, Moves const move, Ability const defender_ability, Weather const weather) -> double {
	switch (defender_ability) {
		case Ability::Battle_Armor:
		case Ability::Shell_Armor:
			return 0.0;
		default: {
			return base_critical_hit_probability(attacker, move, weather);
		}
	}
}

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION(generation) \
	template auto critical_hit_probability<generation>(ActivePokemon<generation> const attacker, Moves const move, Ability const defender_ability, Weather const weather) -> double

TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::one);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::two);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::three);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::four);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::five);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::six);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::seven);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::eight);

} // namespace technicalmachine