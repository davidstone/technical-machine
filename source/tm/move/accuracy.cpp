// Accuracy functions
// Copyright (C) 2019 David Stone
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

#include <tm/move/accuracy.hpp>

#include <tm/move/moves.hpp>

#include <tm/generation.hpp>
#include <tm/weather.hpp>

#include <cassert>

namespace technicalmachine {
using namespace bounded::literal;

// TODO: Simplify some of these after fix for
// https://bugs.llvm.org/show_bug.cgi?id=44666
auto accuracy(Generation const generation, Moves const move, Weather const weather, bool const weather_blocked) -> BaseAccuracy {
	using bounded::none;
	switch (move) {
		case Moves::Pass: return none;
		case Moves::Switch0: return none;
		case Moves::Switch1: return none;
		case Moves::Switch2: return none;
		case Moves::Switch3: return none;
		case Moves::Switch4: return none;
		case Moves::Switch5: return none;
		case Moves::Hit_Self: return none;
		case Moves::Pound: return 100_bi;
		case Moves::Karate_Chop: return 100_bi;
		case Moves::Double_Slap: return 85_bi;
		case Moves::Comet_Punch: return 85_bi;
		case Moves::Mega_Punch: return 85_bi;
		case Moves::Pay_Day: return 100_bi;
		case Moves::Fire_Punch: return 100_bi;
		case Moves::Ice_Punch: return 100_bi;
		case Moves::Thunder_Punch: return 100_bi;
		case Moves::Scratch: return 100_bi;
		case Moves::Vice_Grip: return 100_bi;
		case Moves::Guillotine: return 30_bi;
		case Moves::Razor_Wind: return BOUNDED_CONDITIONAL(generation <= Generation::two, 75_bi, 100_bi);
		case Moves::Swords_Dance: return none;
		case Moves::Cut: return 95_bi;
		case Moves::Gust: return 100_bi;
		case Moves::Wing_Attack: return 100_bi;
		case Moves::Whirlwind:
			switch (generation) {
				case Generation::one:
					return 85_bi;
				case Generation::two:
				case Generation::three:
				case Generation::four:
				case Generation::five:
					return 100_bi;
				default:
					return none;
			}
		case Moves::Fly: return 95_bi;
		case Moves::Bind: return BOUNDED_CONDITIONAL(generation <= Generation::four, 75_bi, 85_bi);
		case Moves::Slam: return 75_bi;
		case Moves::Vine_Whip: return 100_bi;
		case Moves::Stomp: return 100_bi;
		case Moves::Double_Kick: return 100_bi;
		case Moves::Mega_Kick: return 75_bi;
		case Moves::Jump_Kick: return 95_bi;
		case Moves::Rolling_Kick: return 85_bi;
		case Moves::Sand_Attack: return 100_bi;
		case Moves::Headbutt: return 100_bi;
		case Moves::Horn_Attack: return 100_bi;
		case Moves::Fury_Attack: return 85_bi;
		case Moves::Horn_Drill: return 30_bi;
		case Moves::Tackle: return BOUNDED_CONDITIONAL(generation <= Generation::four, 95_bi, 100_bi);
		case Moves::Body_Slam: return 100_bi;
		case Moves::Wrap: return BOUNDED_CONDITIONAL(generation <= Generation::four, 85_bi, 90_bi);
		case Moves::Take_Down: return 85_bi;
		case Moves::Thrash: return 100_bi;
		case Moves::Double_Edge: return 100_bi;
		case Moves::Tail_Whip: return 100_bi;
		case Moves::Poison_Sting: return 100_bi;
		case Moves::Twineedle: return 100_bi;
		case Moves::Pin_Missile: return BOUNDED_CONDITIONAL(generation <= Generation::five, 85_bi, 95_bi);
		case Moves::Leer: return 100_bi;
		case Moves::Bite: return 100_bi;
		case Moves::Growl: return 100_bi;
		case Moves::Roar: return bounded::optional<bounded::integer<30, 100>>(BOUNDED_CONDITIONAL(generation <= Generation::five, 100_bi, none));
		case Moves::Sing: return 55_bi;
		case Moves::Supersonic: return 55_bi;
		case Moves::Sonic_Boom: return 90_bi;
		case Moves::Disable:
			switch (generation) {
				case Generation::one:
				case Generation::two:
				case Generation::three:
					return 55_bi;
				case Generation::four:
					return 80_bi;
				default:
					return 100_bi;
			}
		case Moves::Acid: return 100_bi;
		case Moves::Ember: return 100_bi;
		case Moves::Flamethrower: return 100_bi;
		case Moves::Mist: return none;
		case Moves::Water_Gun: return 100_bi;
		case Moves::Hydro_Pump: return 80_bi;
		case Moves::Surf: return 100_bi;
		case Moves::Ice_Beam: return 100_bi;
		case Moves::Blizzard:
			if (weather.hail(weather_blocked)) {
				return none;
			}
			return BOUNDED_CONDITIONAL(generation == Generation::one, 90_bi, 70_bi);
		case Moves::Psybeam: return 100_bi;
		case Moves::Bubble_Beam: return 100_bi;
		case Moves::Aurora_Beam: return 100_bi;
		case Moves::Hyper_Beam: return 90_bi;
		case Moves::Peck: return 100_bi;
		case Moves::Drill_Peck: return 100_bi;
		case Moves::Submission: return 80_bi;
		case Moves::Low_Kick: return BOUNDED_CONDITIONAL(generation <= Generation::two, 90_bi, 100_bi);
		case Moves::Counter: return 100_bi;
		case Moves::Seismic_Toss: return 100_bi;
		case Moves::Strength: return 100_bi;
		case Moves::Absorb: return 100_bi;
		case Moves::Mega_Drain: return 100_bi;
		case Moves::Leech_Seed: return 90_bi;
		case Moves::Growth: return none;
		case Moves::Razor_Leaf: return 95_bi;
		case Moves::Solar_Beam: return 100_bi;
		case Moves::Poison_Powder: return 75_bi;
		case Moves::Stun_Spore: return 75_bi;
		case Moves::Sleep_Powder: return 75_bi;
		case Moves::Petal_Dance: return 100_bi;
		case Moves::String_Shot: return 95_bi;
		case Moves::Dragon_Rage: return 100_bi;
		case Moves::Fire_Spin: return BOUNDED_CONDITIONAL(generation <= Generation::four, 70_bi, 85_bi);
		case Moves::Thunder_Shock: return 100_bi;
		case Moves::Thunderbolt: return 100_bi;
		case Moves::Thunder_Wave: return BOUNDED_CONDITIONAL(generation <= Generation::six, 100_bi, 90_bi);
		case Moves::Thunder:
			return
				weather.rain(weather_blocked) ? BaseAccuracy(none) :
				weather.sun(weather_blocked) ? BaseAccuracy(50_bi) :
				BaseAccuracy(70_bi);
		case Moves::Rock_Throw: return BOUNDED_CONDITIONAL(generation == Generation::one, 65_bi, 90_bi);
		case Moves::Earthquake: return 100_bi;
		case Moves::Fissure: return 30_bi;
		case Moves::Dig: return 100_bi;
		case Moves::Toxic: return BOUNDED_CONDITIONAL(generation <= Generation::four, 85_bi, 90_bi);
		case Moves::Confusion: return 100_bi;
		case Moves::Psychic: return 100_bi;
		case Moves::Hypnosis: return 60_bi;
		case Moves::Meditate: return none;
		case Moves::Agility: return none;
		case Moves::Quick_Attack: return 100_bi;
		case Moves::Rage: return 100_bi;
		case Moves::Teleport: return none;
		case Moves::Night_Shade: return 100_bi;
		case Moves::Mimic: return bounded::optional<bounded::integer<30, 100>>(BOUNDED_CONDITIONAL(generation <= Generation::two, 100_bi, none));
		case Moves::Screech: return 85_bi;
		case Moves::Double_Team: return none;
		case Moves::Recover: return none;
		case Moves::Harden: return none;
		case Moves::Minimize: return none;
		case Moves::Smokescreen: return 100_bi;
		case Moves::Confuse_Ray: return 100_bi;
		case Moves::Withdraw: return none;
		case Moves::Defense_Curl: return none;
		case Moves::Barrier: return none;
		case Moves::Light_Screen: return none;
		case Moves::Haze: return none;
		case Moves::Reflect: return none;
		case Moves::Focus_Energy: return none;
		case Moves::Bide:
			switch (generation) {
				case Generation::one:
					return none;
				case Generation::two:
				case Generation::three:
					return 100_bi;
				default:
					return none;
			}
		case Moves::Metronome: return none;
		case Moves::Mirror_Move: return none;
		case Moves::Self_Destruct: return 100_bi;
		case Moves::Egg_Bomb: return 75_bi;
		case Moves::Lick: return 100_bi;
		case Moves::Smog: return 70_bi;
		case Moves::Sludge: return 100_bi;
		case Moves::Bone_Club: return 85_bi;
		case Moves::Fire_Blast: return 85_bi;
		case Moves::Waterfall: return 100_bi;
		case Moves::Clamp: return BOUNDED_CONDITIONAL(generation <= Generation::four, 75_bi, 85_bi);
		case Moves::Swift: return none;
		case Moves::Skull_Bash: return 100_bi;
		case Moves::Spike_Cannon: return 100_bi;
		case Moves::Constrict: return 100_bi;
		case Moves::Amnesia: return none;
		case Moves::Kinesis: return 80_bi;
		case Moves::Soft_Boiled: return none;
		case Moves::High_Jump_Kick: return 90_bi;
		case Moves::Glare:
			switch (generation) {
				case Generation::one:
				case Generation::two:
				case Generation::three:
				case Generation::four:
					return 75_bi;
				case Generation::five:
					return 90_bi;
				default:
					return 100_bi;
			}
		case Moves::Dream_Eater: return 100_bi;
		case Moves::Poison_Gas:
			switch (generation) {
				case Generation::one:
				case Generation::two:
				case Generation::three:
				case Generation::four:
					return 55_bi;
				case Generation::five:
					return 80_bi;
				default:
					return 90_bi;
			}
		case Moves::Barrage: return 85_bi;
		case Moves::Leech_Life: return 100_bi;
		case Moves::Lovely_Kiss: return 75_bi;
		case Moves::Sky_Attack: return 90_bi;
		case Moves::Transform: return none;
		case Moves::Bubble: return 100_bi;
		case Moves::Dizzy_Punch: return 100_bi;
		case Moves::Spore: return 100_bi;
		case Moves::Flash: return BOUNDED_CONDITIONAL(generation <= Generation::three, 70_bi, 100_bi);
		case Moves::Psywave: return BOUNDED_CONDITIONAL(generation <= Generation::five, 80_bi, 100_bi);
		case Moves::Splash: return none;
		case Moves::Acid_Armor: return none;
		case Moves::Crabhammer: return BOUNDED_CONDITIONAL(generation <= Generation::four, 85_bi, 90_bi);
		case Moves::Explosion: return 100_bi;
		case Moves::Fury_Swipes: return 80_bi;
		case Moves::Bonemerang: return 90_bi;
		case Moves::Rest: return none;
		case Moves::Rock_Slide: return 90_bi;
		case Moves::Hyper_Fang: return 90_bi;
		case Moves::Sharpen: return none;
		case Moves::Conversion: return none;
		case Moves::Tri_Attack: return 100_bi;
		case Moves::Super_Fang: return 90_bi;
		case Moves::Slash: return 100_bi;
		case Moves::Substitute: return none;
		case Moves::Struggle: return bounded::optional<bounded::integer<30, 100>>(BOUNDED_CONDITIONAL(generation <= Generation::three, 100_bi, none));
		case Moves::Sketch: return none;
		case Moves::Triple_Kick: return 90_bi;
		case Moves::Thief: return 100_bi;
		case Moves::Spider_Web: return none;
		case Moves::Mind_Reader: return bounded::optional<bounded::integer<30, 100>>(BOUNDED_CONDITIONAL(generation <= Generation::three, 100_bi, none));
		case Moves::Nightmare: return 100_bi;
		case Moves::Flame_Wheel: return 100_bi;
		case Moves::Snore: return 100_bi;
		case Moves::Curse: return none;
		case Moves::Flail: return 100_bi;
		case Moves::Conversion_2: return none;
		case Moves::Aeroblast: return 95_bi;
		case Moves::Cotton_Spore: return BOUNDED_CONDITIONAL(generation <= Generation::four, 85_bi, 100_bi);
		case Moves::Reversal: return 100_bi;
		case Moves::Spite: return 100_bi;
		case Moves::Powder_Snow: return 100_bi;
		case Moves::Protect: return none;
		case Moves::Mach_Punch: return 100_bi;
		case Moves::Scary_Face: return BOUNDED_CONDITIONAL(generation <= Generation::four, 90_bi, 100_bi);
		case Moves::Feint_Attack: return none;
		case Moves::Sweet_Kiss: return 75_bi;
		case Moves::Belly_Drum: return none;
		case Moves::Sludge_Bomb: return 100_bi;
		case Moves::Mud_Slap: return 100_bi;
		case Moves::Octazooka: return 85_bi;
		case Moves::Spikes: return none;
		case Moves::Zap_Cannon: return 50_bi;
		case Moves::Foresight: return bounded::optional<bounded::integer<30, 100>>(BOUNDED_CONDITIONAL(generation <= Generation::three, 100_bi, none));
		case Moves::Destiny_Bond: return none;
		case Moves::Perish_Song: return none;
		case Moves::Icy_Wind: return 95_bi;
		case Moves::Detect: return none;
		case Moves::Bone_Rush: return BOUNDED_CONDITIONAL(generation <= Generation::four, 80_bi, 90_bi);
		case Moves::Lock_On: return bounded::optional<bounded::integer<30, 100>>(BOUNDED_CONDITIONAL(generation <= Generation::three, 100_bi, none));
		case Moves::Outrage: return 100_bi;
		case Moves::Sandstorm: return none;
		case Moves::Giga_Drain: return 100_bi;
		case Moves::Endure: return none;
		case Moves::Charm: return 100_bi;
		case Moves::Rollout: return 90_bi;
		case Moves::False_Swipe: return 100_bi;
		case Moves::Swagger: return BOUNDED_CONDITIONAL(generation <= Generation::six, 90_bi, 85_bi);
		case Moves::Milk_Drink: return none;
		case Moves::Spark: return 100_bi;
		case Moves::Fury_Cutter: return 95_bi;
		case Moves::Steel_Wing: return 90_bi;
		case Moves::Mean_Look: return none;
		case Moves::Attract: return 100_bi;
		case Moves::Sleep_Talk: return none;
		case Moves::Heal_Bell: return none;
		case Moves::Return: return 100_bi;
		case Moves::Present: return 90_bi;
		case Moves::Frustration: return 100_bi;
		case Moves::Safeguard: return none;
		case Moves::Pain_Split: return bounded::optional<bounded::integer<30, 100>>(BOUNDED_CONDITIONAL(generation <= Generation::two, 100_bi, none));
		case Moves::Sacred_Fire: return 95_bi;
		case Moves::Magnitude: return 100_bi;
		case Moves::Dynamic_Punch: return 50_bi;
		case Moves::Megahorn: return 85_bi;
		case Moves::Dragon_Breath: return 100_bi;
		case Moves::Baton_Pass: return none;
		case Moves::Encore: return 100_bi;
		case Moves::Pursuit: return 100_bi;
		case Moves::Rapid_Spin: return 100_bi;
		case Moves::Sweet_Scent: return 100_bi;
		case Moves::Iron_Tail: return 75_bi;
		case Moves::Metal_Claw: return 95_bi;
		case Moves::Vital_Throw: return none;
		case Moves::Morning_Sun: return none;
		case Moves::Synthesis: return none;
		case Moves::Moonlight: return none;
		case Moves::Hidden_Power: return 100_bi;
		case Moves::Cross_Chop: return 80_bi;
		case Moves::Twister: return 100_bi;
		case Moves::Rain_Dance: return none;
		case Moves::Sunny_Day: return none;
		case Moves::Crunch: return 100_bi;
		case Moves::Mirror_Coat: return 100_bi;
		case Moves::Psych_Up: return none;
		case Moves::Extreme_Speed: return 100_bi;
		case Moves::Ancient_Power: return 100_bi;
		case Moves::Shadow_Ball: return 100_bi;
		case Moves::Future_Sight: return BOUNDED_CONDITIONAL(generation <= Generation::four, 90_bi, 100_bi);
		case Moves::Rock_Smash: return 100_bi;
		case Moves::Whirlpool: return BOUNDED_CONDITIONAL(generation <= Generation::four, 70_bi, 85_bi);
		case Moves::Beat_Up: return 100_bi;
		case Moves::Fake_Out: return 100_bi;
		case Moves::Uproar: return 100_bi;
		case Moves::Stockpile: return none;
		case Moves::Spit_Up: return 100_bi;
		case Moves::Swallow: return none;
		case Moves::Heat_Wave: return 90_bi;
		case Moves::Hail: return none;
		case Moves::Torment: return 100_bi;
		case Moves::Flatter: return 100_bi;
		case Moves::Will_O_Wisp: return BOUNDED_CONDITIONAL(generation <= Generation::five, 75_bi, 85_bi);
		case Moves::Memento: return 100_bi;
		case Moves::Facade: return 100_bi;
		case Moves::Focus_Punch: return 100_bi;
		case Moves::Smelling_Salts: return 100_bi;
		case Moves::Follow_Me: return none;
		case Moves::Nature_Power: return none;
		case Moves::Charge: return none;
		case Moves::Taunt: return 100_bi;
		case Moves::Helping_Hand: return none;
		case Moves::Trick: return 100_bi;
		case Moves::Role_Play: return none;
		case Moves::Wish: return none;
		case Moves::Assist: return none;
		case Moves::Ingrain: return none;
		case Moves::Superpower: return 100_bi;
		case Moves::Magic_Coat: return none;
		case Moves::Recycle: return none;
		case Moves::Revenge: return 100_bi;
		case Moves::Brick_Break: return 100_bi;
		case Moves::Yawn: return none;
		case Moves::Knock_Off: return 100_bi;
		case Moves::Endeavor: return 100_bi;
		case Moves::Eruption: return 100_bi;
		case Moves::Skill_Swap: return none;
		case Moves::Imprison: return none;
		case Moves::Refresh: return none;
		case Moves::Grudge: return none;
		case Moves::Snatch: return none;
		case Moves::Secret_Power: return 100_bi;
		case Moves::Dive: return 100_bi;
		case Moves::Arm_Thrust: return 100_bi;
		case Moves::Camouflage: return none;
		case Moves::Tail_Glow: return none;
		case Moves::Luster_Purge: return 100_bi;
		case Moves::Mist_Ball: return 100_bi;
		case Moves::Feather_Dance: return 100_bi;
		case Moves::Teeter_Dance: return 100_bi;
		case Moves::Blaze_Kick: return 90_bi;
		case Moves::Mud_Sport: return none;
		case Moves::Ice_Ball: return 90_bi;
		case Moves::Needle_Arm: return 100_bi;
		case Moves::Slack_Off: return none;
		case Moves::Hyper_Voice: return 100_bi;
		case Moves::Poison_Fang: return 100_bi;
		case Moves::Crush_Claw: return 95_bi;
		case Moves::Blast_Burn: return 90_bi;
		case Moves::Hydro_Cannon: return 90_bi;
		case Moves::Meteor_Mash: return BOUNDED_CONDITIONAL(generation <= Generation::five, 85_bi, 90_bi);
		case Moves::Astonish: return 100_bi;
		case Moves::Weather_Ball: return 100_bi;
		case Moves::Aromatherapy: return none;
		case Moves::Fake_Tears: return 100_bi;
		case Moves::Air_Cutter: return 95_bi;
		case Moves::Overheat: return 90_bi;
		case Moves::Odor_Sleuth: return bounded::optional<bounded::integer<30, 100>>(BOUNDED_CONDITIONAL(generation <= Generation::three, 100_bi, none));
		case Moves::Rock_Tomb: return BOUNDED_CONDITIONAL(generation <= Generation::five, 80_bi, 95_bi);
		case Moves::Silver_Wind: return 100_bi;
		case Moves::Metal_Sound: return 85_bi;
		case Moves::Grass_Whistle: return 55_bi;
		case Moves::Tickle: return 100_bi;
		case Moves::Cosmic_Power: return none;
		case Moves::Water_Spout: return 100_bi;
		case Moves::Signal_Beam: return 100_bi;
		case Moves::Shadow_Punch: return none;
		case Moves::Extrasensory: return 100_bi;
		case Moves::Sky_Uppercut: return 90_bi;
		case Moves::Sand_Tomb: return BOUNDED_CONDITIONAL(generation <= Generation::four, 70_bi, 85_bi);
		case Moves::Sheer_Cold: return none;
		case Moves::Muddy_Water: return 85_bi;
		case Moves::Bullet_Seed: return 100_bi;
		case Moves::Aerial_Ace: return none;
		case Moves::Icicle_Spear: return 100_bi;
		case Moves::Iron_Defense: return none;
		case Moves::Block: return none;
		case Moves::Howl: return none;
		case Moves::Dragon_Claw: return 100_bi;
		case Moves::Frenzy_Plant: return 90_bi;
		case Moves::Bulk_Up: return none;
		case Moves::Bounce: return 85_bi;
		case Moves::Mud_Shot: return 95_bi;
		case Moves::Poison_Tail: return 100_bi;
		case Moves::Covet: return 100_bi;
		case Moves::Volt_Tackle: return 100_bi;
		case Moves::Magical_Leaf: return none;
		case Moves::Water_Sport: return none;
		case Moves::Calm_Mind: return none;
		case Moves::Leaf_Blade: return 100_bi;
		case Moves::Dragon_Dance: return none;
		case Moves::Rock_Blast: return BOUNDED_CONDITIONAL(generation <= Generation::four, 80_bi, 90_bi);
		case Moves::Shock_Wave: return none;
		case Moves::Water_Pulse: return 100_bi;
		case Moves::Doom_Desire: return BOUNDED_CONDITIONAL(generation <= Generation::four, 85_bi, 100_bi);
		case Moves::Psycho_Boost: return 90_bi;
		case Moves::Roost: return none;
		case Moves::Gravity: return none;
		case Moves::Miracle_Eye: return none;
		case Moves::Wake_Up_Slap: return 100_bi;
		case Moves::Hammer_Arm: return 90_bi;
		case Moves::Gyro_Ball: return 100_bi;
		case Moves::Healing_Wish: return none;
		case Moves::Brine: return 100_bi;
		case Moves::Natural_Gift: return 100_bi;
		case Moves::Feint: return 100_bi;
		case Moves::Pluck: return 100_bi;
		case Moves::Tailwind: return none;
		case Moves::Acupressure: return none;
		case Moves::Metal_Burst: return 100_bi;
		case Moves::U_turn: return 100_bi;
		case Moves::Close_Combat: return 100_bi;
		case Moves::Payback: return 100_bi;
		case Moves::Assurance: return 100_bi;
		case Moves::Embargo: return 100_bi;
		case Moves::Fling: return 100_bi;
		case Moves::Psycho_Shift: return BOUNDED_CONDITIONAL(generation <= Generation::five, 90_bi, 100_bi);
		case Moves::Trump_Card: return none;
		case Moves::Heal_Block: return 100_bi;
		case Moves::Wring_Out: return 100_bi;
		case Moves::Power_Trick: return none;
		case Moves::Gastro_Acid: return 100_bi;
		case Moves::Lucky_Chant: return none;
		case Moves::Me_First: return none;
		case Moves::Copycat: return none;
		case Moves::Power_Swap: return none;
		case Moves::Guard_Swap: return none;
		case Moves::Punishment: return 100_bi;
		case Moves::Last_Resort: return 100_bi;
		case Moves::Worry_Seed: return 100_bi;
		case Moves::Sucker_Punch: return 100_bi;
		case Moves::Toxic_Spikes: return none;
		case Moves::Heart_Swap: return none;
		case Moves::Aqua_Ring: return none;
		case Moves::Magnet_Rise: return none;
		case Moves::Flare_Blitz: return 100_bi;
		case Moves::Force_Palm: return 100_bi;
		case Moves::Aura_Sphere: return none;
		case Moves::Rock_Polish: return none;
		case Moves::Poison_Jab: return 100_bi;
		case Moves::Dark_Pulse: return 100_bi;
		case Moves::Night_Slash: return 100_bi;
		case Moves::Aqua_Tail: return 90_bi;
		case Moves::Seed_Bomb: return 100_bi;
		case Moves::Air_Slash: return 95_bi;
		case Moves::X_Scissor: return 100_bi;
		case Moves::Bug_Buzz: return 100_bi;
		case Moves::Dragon_Pulse: return 100_bi;
		case Moves::Dragon_Rush: return 75_bi;
		case Moves::Power_Gem: return 100_bi;
		case Moves::Drain_Punch: return 100_bi;
		case Moves::Vacuum_Wave: return 100_bi;
		case Moves::Focus_Blast: return 70_bi;
		case Moves::Energy_Ball: return 100_bi;
		case Moves::Brave_Bird: return 100_bi;
		case Moves::Earth_Power: return 100_bi;
		case Moves::Switcheroo: return 100_bi;
		case Moves::Giga_Impact: return 90_bi;
		case Moves::Nasty_Plot: return none;
		case Moves::Bullet_Punch: return 100_bi;
		case Moves::Avalanche: return 100_bi;
		case Moves::Ice_Shard: return 100_bi;
		case Moves::Shadow_Claw: return 100_bi;
		case Moves::Thunder_Fang: return 95_bi;
		case Moves::Ice_Fang: return 95_bi;
		case Moves::Fire_Fang: return 95_bi;
		case Moves::Shadow_Sneak: return 100_bi;
		case Moves::Mud_Bomb: return 85_bi;
		case Moves::Psycho_Cut: return 100_bi;
		case Moves::Zen_Headbutt: return 90_bi;
		case Moves::Mirror_Shot: return 85_bi;
		case Moves::Flash_Cannon: return 100_bi;
		case Moves::Rock_Climb: return 85_bi;
		case Moves::Defog: return none;
		case Moves::Trick_Room: return none;
		case Moves::Draco_Meteor: return 90_bi;
		case Moves::Discharge: return 100_bi;
		case Moves::Lava_Plume: return 100_bi;
		case Moves::Leaf_Storm: return 90_bi;
		case Moves::Power_Whip: return 85_bi;
		case Moves::Rock_Wrecker: return 90_bi;
		case Moves::Cross_Poison: return 100_bi;
		case Moves::Gunk_Shot: return BOUNDED_CONDITIONAL(generation <= Generation::five, 70_bi, 80_bi);
		case Moves::Iron_Head: return 100_bi;
		case Moves::Magnet_Bomb: return none;
		case Moves::Stone_Edge: return 80_bi;
		case Moves::Captivate: return 100_bi;
		case Moves::Stealth_Rock: return none;
		case Moves::Grass_Knot: return 100_bi;
		case Moves::Chatter: return 100_bi;
		case Moves::Judgment: return 100_bi;
		case Moves::Bug_Bite: return 100_bi;
		case Moves::Charge_Beam: return 90_bi;
		case Moves::Wood_Hammer: return 100_bi;
		case Moves::Aqua_Jet: return 100_bi;
		case Moves::Attack_Order: return 100_bi;
		case Moves::Defend_Order: return none;
		case Moves::Heal_Order: return none;
		case Moves::Head_Smash: return 80_bi;
		case Moves::Double_Hit: return 90_bi;
		case Moves::Roar_of_Time: return 90_bi;
		case Moves::Spacial_Rend: return 95_bi;
		case Moves::Lunar_Dance: return none;
		case Moves::Crush_Grip: return 100_bi;
		case Moves::Magma_Storm: return BOUNDED_CONDITIONAL(generation <= Generation::four, 70_bi, 75_bi);
		case Moves::Dark_Void: return BOUNDED_CONDITIONAL(generation <= Generation::six, 80_bi, 50_bi);
		case Moves::Seed_Flare: return 85_bi;
		case Moves::Ominous_Wind: return 100_bi;
		case Moves::Shadow_Force: return 100_bi;
		case Moves::Hone_Claws: return none;
		case Moves::Wide_Guard: return none;
		case Moves::Guard_Split: return none;
		case Moves::Power_Split: return none;
		case Moves::Wonder_Room: return none;
		case Moves::Psyshock: return 100_bi;
		case Moves::Venoshock: return 100_bi;
		case Moves::Autotomize: return none;
		case Moves::Rage_Powder: return none;
		case Moves::Telekinesis: return none;
		case Moves::Magic_Room: return none;
		case Moves::Smack_Down: return 100_bi;
		case Moves::Storm_Throw: return 100_bi;
		case Moves::Flame_Burst: return 100_bi;
		case Moves::Sludge_Wave: return 100_bi;
		case Moves::Quiver_Dance: return none;
		case Moves::Heavy_Slam: return 100_bi;
		case Moves::Synchronoise: return 100_bi;
		case Moves::Electro_Ball: return 100_bi;
		case Moves::Soak: return 100_bi;
		case Moves::Flame_Charge: return 100_bi;
		case Moves::Coil: return none;
		case Moves::Low_Sweep: return 100_bi;
		case Moves::Acid_Spray: return 100_bi;
		case Moves::Foul_Play: return 100_bi;
		case Moves::Simple_Beam: return 100_bi;
		case Moves::Entrainment: return 100_bi;
		case Moves::After_You: return none;
		case Moves::Round: return 100_bi;
		case Moves::Echoed_Voice: return 100_bi;
		case Moves::Chip_Away: return 100_bi;
		case Moves::Clear_Smog: return none;
		case Moves::Stored_Power: return 100_bi;
		case Moves::Quick_Guard: return none;
		case Moves::Ally_Switch: return none;
		case Moves::Scald: return 100_bi;
		case Moves::Shell_Smash: return none;
		case Moves::Heal_Pulse: return none;
		case Moves::Hex: return 100_bi;
		case Moves::Sky_Drop: return 100_bi;
		case Moves::Shift_Gear: return none;
		case Moves::Circle_Throw: return 90_bi;
		case Moves::Incinerate: return 100_bi;
		case Moves::Quash: return 100_bi;
		case Moves::Acrobatics: return 100_bi;
		case Moves::Reflect_Type: return none;
		case Moves::Retaliate: return 100_bi;
		case Moves::Final_Gambit: return 100_bi;
		case Moves::Bestow: return none;
		case Moves::Inferno: return 50_bi;
		case Moves::Water_Pledge: return 100_bi;
		case Moves::Fire_Pledge: return 100_bi;
		case Moves::Grass_Pledge: return 100_bi;
		case Moves::Volt_Switch: return 100_bi;
		case Moves::Struggle_Bug: return 100_bi;
		case Moves::Bulldoze: return 100_bi;
		case Moves::Frost_Breath: return 90_bi;
		case Moves::Dragon_Tail: return 90_bi;
		case Moves::Work_Up: return none;
		case Moves::Electroweb: return 95_bi;
		case Moves::Wild_Charge: return 100_bi;
		case Moves::Drill_Run: return 95_bi;
		case Moves::Dual_Chop: return 90_bi;
		case Moves::Heart_Stamp: return 100_bi;
		case Moves::Horn_Leech: return 100_bi;
		case Moves::Sacred_Sword: return 100_bi;
		case Moves::Razor_Shell: return 95_bi;
		case Moves::Heat_Crash: return 100_bi;
		case Moves::Leaf_Tornado: return 90_bi;
		case Moves::Steamroller: return 100_bi;
		case Moves::Cotton_Guard: return none;
		case Moves::Night_Daze: return 95_bi;
		case Moves::Psystrike: return 100_bi;
		case Moves::Tail_Slap: return 85_bi;
		case Moves::Hurricane: return 70_bi;
		case Moves::Head_Charge: return 100_bi;
		case Moves::Gear_Grind: return 85_bi;
		case Moves::Searing_Shot: return 100_bi;
		case Moves::Techno_Blast: return 100_bi;
		case Moves::Relic_Song: return 100_bi;
		case Moves::Secret_Sword: return 100_bi;
		case Moves::Glaciate: return 95_bi;
		case Moves::Bolt_Strike: return 85_bi;
		case Moves::Blue_Flare: return 85_bi;
		case Moves::Fiery_Dance: return 100_bi;
		case Moves::Freeze_Shock: return 90_bi;
		case Moves::Ice_Burn: return 90_bi;
		case Moves::Snarl: return 95_bi;
		case Moves::Icicle_Crash: return 90_bi;
		case Moves::V_create: return 95_bi;
		case Moves::Fusion_Flare: return 100_bi;
		case Moves::Fusion_Bolt: return 100_bi;
		case Moves::Flying_Press: return 95_bi;
		case Moves::Mat_Block: return none;
		case Moves::Belch: return 90_bi;
		case Moves::Rototiller: return none;
		case Moves::Sticky_Web: return none;
		case Moves::Fell_Stinger: return 100_bi;
		case Moves::Phantom_Force: return 100_bi;
		case Moves::Trick_or_Treat: return 100_bi;
		case Moves::Noble_Roar: return 100_bi;
		case Moves::Ion_Deluge: return none;
		case Moves::Parabolic_Charge: return 100_bi;
		case Moves::Forests_Curse: return 100_bi;
		case Moves::Petal_Blizzard: return 100_bi;
		case Moves::Freeze_Dry: return 100_bi;
		case Moves::Disarming_Voice: return none;
		case Moves::Parting_Shot: return 100_bi;
		case Moves::Topsy_Turvy: return bounded::optional<bounded::integer<30, 100>>(BOUNDED_CONDITIONAL(generation <= Generation::six, 100_bi, none));
		case Moves::Draining_Kiss: return 100_bi;
		case Moves::Crafty_Shield: return none;
		case Moves::Flower_Shield: return none;
		case Moves::Grassy_Terrain: return none;
		case Moves::Misty_Terrain: return none;
		case Moves::Electrify: return none;
		case Moves::Play_Rough: return 90_bi;
		case Moves::Fairy_Wind: return 100_bi;
		case Moves::Moonblast: return 100_bi;
		case Moves::Boomburst: return 100_bi;
		case Moves::Fairy_Lock: return none;
		case Moves::Kings_Shield: return none;
		case Moves::Play_Nice: return none;
		case Moves::Confide: return none;
		case Moves::Diamond_Storm: return 95_bi;
		case Moves::Steam_Eruption: return 95_bi;
		case Moves::Hyperspace_Hole: return none;
		case Moves::Water_Shuriken: return 100_bi;
		case Moves::Mystical_Fire: return 100_bi;
		case Moves::Spiky_Shield: return none;
		case Moves::Aromatic_Mist: return none;
		case Moves::Eerie_Impulse: return 100_bi;
		case Moves::Venom_Drench: return 100_bi;
		case Moves::Powder: return 100_bi;
		case Moves::Geomancy: return none;
		case Moves::Magnetic_Flux: return none;
		case Moves::Happy_Hour: return none;
		case Moves::Electric_Terrain: return none;
		case Moves::Dazzling_Gleam: return 100_bi;
		case Moves::Celebrate: return none;
		case Moves::Hold_Hands: return none;
		case Moves::Baby_Doll_Eyes: return 100_bi;
		case Moves::Nuzzle: return 100_bi;
		case Moves::Hold_Back: return 100_bi;
		case Moves::Infestation: return 100_bi;
		case Moves::Power_Up_Punch: return 100_bi;
		case Moves::Oblivion_Wing: return 100_bi;
		case Moves::Thousand_Arrows: return 100_bi;
		case Moves::Thousand_Waves: return 100_bi;
		case Moves::Lands_Wrath: return 100_bi;
		case Moves::Light_of_Ruin: return 90_bi;
		case Moves::Origin_Pulse: return 85_bi;
		case Moves::Precipice_Blades: return 85_bi;
		case Moves::Dragon_Ascent: return 100_bi;
		case Moves::Hyperspace_Fury: return none;
		case Moves::Breakneck_Blitz: return none;
		case Moves::All_Out_Pummeling: return none;
		case Moves::Supersonic_Skystrike: return none;
		case Moves::Acid_Downpour: return none;
		case Moves::Tectonic_Rage: return none;
		case Moves::Continental_Crush: return none;
		case Moves::Savage_Spin_Out: return none;
		case Moves::Never_Ending_Nightmare: return none;
		case Moves::Corkscrew_Crash: return none;
		case Moves::Inferno_Overdrive: return none;
		case Moves::Hydro_Vortex: return none;
		case Moves::Bloom_Doom: return none;
		case Moves::Gigavolt_Havoc: return none;
		case Moves::Shattered_Psyche: return none;
		case Moves::Subzero_Slammer: return none;
		case Moves::Devastating_Drake: return none;
		case Moves::Black_Hole_Eclipse: return none;
		case Moves::Twinkle_Tackle: return none;
		case Moves::Catastropika: return none;
		case Moves::Shore_Up: return none;
		case Moves::First_Impression: return 100_bi;
		case Moves::Baneful_Bunker: return none;
		case Moves::Spirit_Shackle: return 100_bi;
		case Moves::Darkest_Lariat: return 100_bi;
		case Moves::Sparkling_Aria: return 100_bi;
		case Moves::Ice_Hammer: return 90_bi;
		case Moves::Floral_Healing: return none;
		case Moves::High_Horsepower: return 95_bi;
		case Moves::Strength_Sap: return 100_bi;
		case Moves::Solar_Blade: return 100_bi;
		case Moves::Leafage: return 100_bi;
		case Moves::Spotlight: return none;
		case Moves::Toxic_Thread: return 100_bi;
		case Moves::Laser_Focus: return none;
		case Moves::Gear_Up: return none;
		case Moves::Throat_Chop: return 100_bi;
		case Moves::Pollen_Puff: return 100_bi;
		case Moves::Anchor_Shot: return 100_bi;
		case Moves::Psychic_Terrain: return none;
		case Moves::Lunge: return 100_bi;
		case Moves::Fire_Lash: return 100_bi;
		case Moves::Power_Trip: return 100_bi;
		case Moves::Burn_Up: return 100_bi;
		case Moves::Speed_Swap: return none;
		case Moves::Smart_Strike: return none;
		case Moves::Purify: return none;
		case Moves::Revelation_Dance: return 100_bi;
		case Moves::Core_Enforcer: return 100_bi;
		case Moves::Trop_Kick: return 100_bi;
		case Moves::Instruct: return none;
		case Moves::Beak_Blast: return 100_bi;
		case Moves::Clanging_Scales: return 100_bi;
		case Moves::Dragon_Hammer: return 100_bi;
		case Moves::Brutal_Swing: return 100_bi;
		case Moves::Aurora_Veil: return none;
		case Moves::Sinister_Arrow_Raid: return none;
		case Moves::Malicious_Moonsault: return none;
		case Moves::Oceanic_Operetta: return none;
		case Moves::Guardian_of_Alola: return none;
		case Moves::Soul_Stealing_7_Star_Strike: return none;
		case Moves::Stoked_Sparksurfer: return none;
		case Moves::Pulverizing_Pancake: return none;
		case Moves::Extreme_Evoboost: return none;
		case Moves::Genesis_Supernova: return none;
		case Moves::Shell_Trap: return 100_bi;
		case Moves::Fleur_Cannon: return 90_bi;
		case Moves::Psychic_Fangs: return 100_bi;
		case Moves::Stomping_Tantrum: return 100_bi;
		case Moves::Shadow_Bone: return 100_bi;
		case Moves::Accelerock: return 100_bi;
		case Moves::Liquidation: return 100_bi;
		case Moves::Prismatic_Laser: return 100_bi;
		case Moves::Spectral_Thief: return 100_bi;
		case Moves::Sunsteel_Strike: return 100_bi;
		case Moves::Moongeist_Beam: return 100_bi;
		case Moves::Tearful_Look: return none;
		case Moves::Zing_Zap: return 100_bi;
		case Moves::Natures_Madness: return 90_bi;
		case Moves::Multi_Attack: return 100_bi;
		case Moves::m10000000_Volt_Thunderbolt: return none;
		case Moves::Mind_Blown: return 100_bi;
		case Moves::Plasma_Fists: return 100_bi;
		case Moves::Photon_Geyser: return 100_bi;
		case Moves::Light_That_Burns_the_Sky: return none;
		case Moves::Searing_Sunraze_Smash: return none;
		case Moves::Menacing_Moonraze_Maelstrom: return none;
		case Moves::Lets_Snuggle_Forever: return none;
		case Moves::Splintered_Stormshards: return none;
		case Moves::Clangorous_Soulblaze: return none;
		case Moves::Zippy_Zap: return 100_bi;
		case Moves::Splishy_Splash: return 100_bi;
		case Moves::Floaty_Fall: return 95_bi;
		case Moves::Pika_Papow: return none;
		case Moves::Bouncy_Bubble: return 100_bi;
		case Moves::Buzzy_Buzz: return 100_bi;
		case Moves::Sizzly_Slide: return 100_bi;
		case Moves::Glitzy_Glow: return 100_bi;
		case Moves::Baddy_Bad: return 100_bi;
		case Moves::Sappy_Seed: return 100_bi;
		case Moves::Freezy_Frost: return 100_bi;
		case Moves::Sparkly_Swirl: return 100_bi;
		case Moves::Veevee_Volley: return none;
		case Moves::Double_Iron_Bash: return 100_bi;
	}
}

}	// namespace technicalmachine
