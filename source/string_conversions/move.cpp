// Move string conversions
// Copyright (C) 2018 David Stone
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

#include "invalid_string_conversion.hpp"

#include "../move/moves.hpp"

#include <map>

namespace technicalmachine {

std::string_view to_string(Moves const move) {
	switch (move) {
		// Weird moves
		case Moves::Pass: return "Pass";
		case Moves::Switch0: return "Switch0";
		case Moves::Switch1: return "Switch1";
		case Moves::Switch2: return "Switch2";
		case Moves::Switch3: return "Switch3";
		case Moves::Switch4: return "Switch4";
		case Moves::Switch5: return "Switch5";
		case Moves::Hit_Self: return "Hit self in confusion";

		// Generation 1
		case Moves::Pound: return "Pound";
		case Moves::Karate_Chop: return "Karate Chop";
		case Moves::DoubleSlap: return "DoubleSlap";
		case Moves::Comet_Punch: return "Comet Punch";
		case Moves::Mega_Punch: return "Mega Punch";
		case Moves::Pay_Day: return "Pay Day";
		case Moves::Fire_Punch: return "Fire Punch";
		case Moves::Ice_Punch: return "Ice Punch";
		case Moves::ThunderPunch: return "ThunderPunch";
		case Moves::Scratch: return "Scratch";
		case Moves::ViceGrip: return "ViceGrip";
		case Moves::Guillotine: return "Guillotine";
		case Moves::Razor_Wind: return "Razor Wind";
		case Moves::Swords_Dance: return "Swords Dance";
		case Moves::Cut: return "Cut";
		case Moves::Gust: return "Gust";
		case Moves::Wing_Attack: return "Wing Attack";
		case Moves::Whirlwind: return "Whirlwind";
		case Moves::Fly: return "Fly";
		case Moves::Bind: return "Bind";
		case Moves::Slam: return "Slam";
		case Moves::Vine_Whip: return "Vine Whip";
		case Moves::Stomp: return "Stomp";
		case Moves::Double_Kick: return "Double Kick";
		case Moves::Mega_Kick: return "Mega Kick";
		case Moves::Jump_Kick: return "Jump Kick";
		case Moves::Rolling_Kick: return "Rolling Kick";
		case Moves::Sand_Attack: return "Sand-Attack";
		case Moves::Headbutt: return "Headbutt";
		case Moves::Horn_Attack: return "Horn Attack";
		case Moves::Fury_Attack: return "Fury Attack";
		case Moves::Horn_Drill: return "Horn Drill";
		case Moves::Tackle: return "Tackle";
		case Moves::Body_Slam: return "Body Slam";
		case Moves::Wrap: return "Wrap";
		case Moves::Take_Down: return "Take Down";
		case Moves::Thrash: return "Thrash";
		case Moves::Double_Edge: return "Double-Edge";
		case Moves::Tail_Whip: return "Tail Whip";
		case Moves::Poison_Sting: return "Poison Sting";
		case Moves::Twineedle: return "Twineedle";
		case Moves::Pin_Missile: return "Pin Missile";
		case Moves::Leer: return "Leer";
		case Moves::Bite: return "Bite";
		case Moves::Growl: return "Growl";
		case Moves::Roar: return "Roar";
		case Moves::Sing: return "Sing";
		case Moves::Supersonic: return "Supersonic";
		case Moves::SonicBoom: return "SonicBoom";
		case Moves::Disable: return "Disable";
		case Moves::Acid: return "Acid";
		case Moves::Ember: return "Ember";
		case Moves::Flamethrower: return "Flamethrower";
		case Moves::Mist: return "Mist";
		case Moves::Water_Gun: return "Water Gun";
		case Moves::Hydro_Pump: return "Hydro Pump";
		case Moves::Surf: return "Surf";
		case Moves::Ice_Beam: return "Ice Beam";
		case Moves::Blizzard: return "Blizzard";
		case Moves::Psybeam: return "Psybeam";
		case Moves::BubbleBeam: return "BubbleBeam";
		case Moves::Aurora_Beam: return "Aurora Beam";
		case Moves::Hyper_Beam: return "Hyper Beam";
		case Moves::Peck: return "Peck";
		case Moves::Drill_Peck: return "Drill Peck";
		case Moves::Submission: return "Submission";
		case Moves::Low_Kick: return "Low Kick";
		case Moves::Counter: return "Counter";
		case Moves::Seismic_Toss: return "Seismic Toss";
		case Moves::Strength: return "Strength";
		case Moves::Absorb: return "Absorb";
		case Moves::Mega_Drain: return "Mega Drain";
		case Moves::Leech_Seed: return "Leech Seed";
		case Moves::Growth: return "Growth";
		case Moves::Razor_Leaf: return "Razor Leaf";
		case Moves::SolarBeam: return "SolarBeam";
		case Moves::PoisonPowder: return "PoisonPowder";
		case Moves::Stun_Spore: return "Stun Spore";
		case Moves::Sleep_Powder: return "Sleep Powder";
		case Moves::Petal_Dance: return "Petal Dance";
		case Moves::String_Shot: return "String Shot";
		case Moves::Dragon_Rage: return "Dragon Rage";
		case Moves::Fire_Spin: return "Fire Spin";
		case Moves::ThunderShock: return "ThunderShock";
		case Moves::Thunderbolt: return "Thunderbolt";
		case Moves::Thunder_Wave: return "Thunder Wave";
		case Moves::Thunder: return "Thunder";
		case Moves::Rock_Throw: return "Rock Throw";
		case Moves::Earthquake: return "Earthquake";
		case Moves::Fissure: return "Fissure";
		case Moves::Dig: return "Dig";
		case Moves::Toxic: return "Toxic";
		case Moves::Confusion: return "Confusion";
		case Moves::Psychic: return "Psychic";
		case Moves::Hypnosis: return "Hypnosis";
		case Moves::Meditate: return "Meditate";
		case Moves::Agility: return "Agility";
		case Moves::Quick_Attack: return "Quick Attack";
		case Moves::Rage: return "Rage";
		case Moves::Teleport: return "Teleport";
		case Moves::Night_Shade: return "Night Shade";
		case Moves::Mimic: return "Mimic";
		case Moves::Screech: return "Screech";
		case Moves::Double_Team: return "Double Team";
		case Moves::Recover: return "Recover";
		case Moves::Harden: return "Harden";
		case Moves::Minimize: return "Minimize";
		case Moves::SmokeScreen: return "SmokeScreen";
		case Moves::Confuse_Ray: return "Confuse Ray";
		case Moves::Withdraw: return "Withdraw";
		case Moves::Defense_Curl: return "Defense Curl";
		case Moves::Barrier: return "Barrier";
		case Moves::Light_Screen: return "Light Screen";
		case Moves::Haze: return "Haze";
		case Moves::Reflect: return "Reflect";
		case Moves::Focus_Energy: return "Focus Energy";
		case Moves::Bide: return "Bide";
		case Moves::Metronome: return "Metronome";
		case Moves::Mirror_Move: return "Mirror Move";
		case Moves::Selfdestruct: return "Selfdestruct";
		case Moves::Egg_Bomb: return "Egg Bomb";
		case Moves::Lick: return "Lick";
		case Moves::Smog: return "Smog";
		case Moves::Sludge: return "Sludge";
		case Moves::Bone_Club: return "Bone Club";
		case Moves::Fire_Blast: return "Fire Blast";
		case Moves::Waterfall: return "Waterfall";
		case Moves::Clamp: return "Clamp";
		case Moves::Swift: return "Swift";
		case Moves::Skull_Bash: return "Skull Bash";
		case Moves::Spike_Cannon: return "Spike Cannon";
		case Moves::Constrict: return "Constrict";
		case Moves::Amnesia: return "Amnesia";
		case Moves::Kinesis: return "Kinesis";
		case Moves::Softboiled: return "Softboiled";
		case Moves::Hi_Jump_Kick: return "Hi Jump Kick";
		case Moves::Glare: return "Glare";
		case Moves::Dream_Eater: return "Dream Eater";
		case Moves::Poison_Gas: return "Poison Gas";
		case Moves::Barrage: return "Barrage";
		case Moves::Leech_Life: return "Leech Life";
		case Moves::Lovely_Kiss: return "Lovely Kiss";
		case Moves::Sky_Attack: return "Sky Attack";
		case Moves::Transform: return "Transform";
		case Moves::Bubble: return "Bubble";
		case Moves::Dizzy_Punch: return "Dizzy Punch";
		case Moves::Spore: return "Spore";
		case Moves::Flash: return "Flash";
		case Moves::Psywave: return "Psywave";
		case Moves::Splash: return "Splash";
		case Moves::Acid_Armor: return "Acid Armor";
		case Moves::Crabhammer: return "Crabhammer";
		case Moves::Explosion: return "Explosion";
		case Moves::Fury_Swipes: return "Fury Swipes";
		case Moves::Bonemerang: return "Bonemerang";
		case Moves::Rest: return "Rest";
		case Moves::Rock_Slide: return "Rock Slide";
		case Moves::Hyper_Fang: return "Hyper Fang";
		case Moves::Sharpen: return "Sharpen";
		case Moves::Conversion: return "Conversion";
		case Moves::Tri_Attack: return "Tri Attack";
		case Moves::Super_Fang: return "Super Fang";
		case Moves::Slash: return "Slash";
		case Moves::Substitute: return "Substitute";
		case Moves::Struggle: return "Struggle";
		
		// Generation 2
		case Moves::Sketch: return "Sketch";
		case Moves::Triple_Kick: return "Triple Kick";
		case Moves::Thief: return "Thief";
		case Moves::Spider_Web: return "Spider Web";
		case Moves::Mind_Reader: return "Mind Reader";
		case Moves::Nightmare: return "Nightmare";
		case Moves::Flame_Wheel: return "Flame Wheel";
		case Moves::Snore: return "Snore";
		case Moves::Curse: return "Curse";
		case Moves::Flail: return "Flail";
		case Moves::Conversion_2: return "Conversion 2";
		case Moves::Aeroblast: return "Aeroblast";
		case Moves::Cotton_Spore: return "Cotton Spore";
		case Moves::Reversal: return "Reversal";
		case Moves::Spite: return "Spite";
		case Moves::Powder_Snow: return "Powder Snow";
		case Moves::Protect: return "Protect";
		case Moves::Mach_Punch: return "Mach Punch";
		case Moves::Scary_Face: return "Scary Face";
		case Moves::Faint_Attack: return "Faint Attack";
		case Moves::Sweet_Kiss: return "Sweet Kiss";
		case Moves::Belly_Drum: return "Belly Drum";
		case Moves::Sludge_Bomb: return "Sludge Bomb";
		case Moves::Mud_Slap: return "Mud-Slap";
		case Moves::Octazooka: return "Octazooka";
		case Moves::Spikes: return "Spikes";
		case Moves::Zap_Cannon: return "Zap Cannon";
		case Moves::Foresight: return "Foresight";
		case Moves::Destiny_Bond: return "Destiny Bond";
		case Moves::Perish_Song: return "Perish Song";
		case Moves::Icy_Wind: return "Icy Wind";
		case Moves::Detect: return "Detect";
		case Moves::Bone_Rush: return "Bone Rush";
		case Moves::Lock_On: return "Lock-On";
		case Moves::Outrage: return "Outrage";
		case Moves::Sandstorm: return "Sandstorm";
		case Moves::Giga_Drain: return "Giga Drain";
		case Moves::Endure: return "Endure";
		case Moves::Charm: return "Charm";
		case Moves::Rollout: return "Rollout";
		case Moves::False_Swipe: return "False Swipe";
		case Moves::Swagger: return "Swagger";
		case Moves::Milk_Drink: return "Milk Drink";
		case Moves::Spark: return "Spark";
		case Moves::Fury_Cutter: return "Fury Cutter";
		case Moves::Steel_Wing: return "Steel Wing";
		case Moves::Mean_Look: return "Mean Look";
		case Moves::Attract: return "Attract";
		case Moves::Sleep_Talk: return "Sleep Talk";
		case Moves::Heal_Bell: return "Heal Bell";
		case Moves::Return: return "Return";
		case Moves::Present: return "Present";
		case Moves::Frustration: return "Frustration";
		case Moves::Safeguard: return "Safeguard";
		case Moves::Pain_Split: return "Pain Split";
		case Moves::Sacred_Fire: return "Sacred Fire";
		case Moves::Magnitude: return "Magnitude";
		case Moves::DynamicPunch: return "DynamicPunch";
		case Moves::Megahorn: return "Megahorn";
		case Moves::DragonBreath: return "DragonBreath";
		case Moves::Baton_Pass: return "Baton Pass";
		case Moves::Encore: return "Encore";
		case Moves::Pursuit: return "Pursuit";
		case Moves::Rapid_Spin: return "Rapid Spin";
		case Moves::Sweet_Scent: return "Sweet Scent";
		case Moves::Iron_Tail: return "Iron Tail";
		case Moves::Metal_Claw: return "Metal Claw";
		case Moves::Vital_Throw: return "Vital Throw";
		case Moves::Morning_Sun: return "Morning Sun";
		case Moves::Synthesis: return "Synthesis";
		case Moves::Moonlight: return "Moonlight";
		case Moves::Hidden_Power: return "Hidden Power";
		case Moves::Cross_Chop: return "Cross Chop";
		case Moves::Twister: return "Twister";
		case Moves::Rain_Dance: return "Rain Dance";
		case Moves::Sunny_Day: return "Sunny Day";
		case Moves::Crunch: return "Crunch";
		case Moves::Mirror_Coat: return "Mirror Coat";
		case Moves::Psych_Up: return "Psych Up";
		case Moves::ExtremeSpeed: return "ExtremeSpeed";
		case Moves::AncientPower: return "AncientPower";
		case Moves::Shadow_Ball: return "Shadow Ball";
		case Moves::Future_Sight: return "Future Sight";
		case Moves::Rock_Smash: return "Rock Smash";
		case Moves::Whirlpool: return "Whirlpool";
		case Moves::Beat_Up: return "Beat Up";
		
		// Generation 3
		case Moves::Fake_Out: return "Fake Out";
		case Moves::Uproar: return "Uproar";
		case Moves::Stockpile: return "Stockpile";
		case Moves::Spit_Up: return "Spit Up";
		case Moves::Swallow: return "Swallow";
		case Moves::Heat_Wave: return "Heat Wave";
		case Moves::Hail: return "Hail";
		case Moves::Torment: return "Torment";
		case Moves::Flatter: return "Flatter";
		case Moves::Will_O_Wisp: return "Will-O-Wisp";
		case Moves::Memento: return "Memento";
		case Moves::Facade: return "Facade";
		case Moves::Focus_Punch: return "Focus Punch";
		case Moves::SmellingSalt: return "SmellingSalt";
		case Moves::Follow_Me: return "Follow Me";
		case Moves::Nature_Power: return "Nature Power";
		case Moves::Charge: return "Charge";
		case Moves::Taunt: return "Taunt";
		case Moves::Helping_Hand: return "Helping Hand";
		case Moves::Trick: return "Trick";
		case Moves::Role_Play: return "Role Play";
		case Moves::Wish: return "Wish";
		case Moves::Assist: return "Assist";
		case Moves::Ingrain: return "Ingrain";
		case Moves::Superpower: return "Superpower";
		case Moves::Magic_Coat: return "Magic Coat";
		case Moves::Recycle: return "Recycle";
		case Moves::Revenge: return "Revenge";
		case Moves::Brick_Break: return "Brick Break";
		case Moves::Yawn: return "Yawn";
		case Moves::Knock_Off: return "Knock Off";
		case Moves::Endeavor: return "Endeavor";
		case Moves::Eruption: return "Eruption";
		case Moves::Skill_Swap: return "Skill Swap";
		case Moves::Imprison: return "Imprison";
		case Moves::Refresh: return "Refresh";
		case Moves::Grudge: return "Grudge";
		case Moves::Snatch: return "Snatch";
		case Moves::Secret_Power: return "Secret Power";
		case Moves::Dive: return "Dive";
		case Moves::Arm_Thrust: return "Arm Thrust";
		case Moves::Camouflage: return "Camouflage";
		case Moves::Tail_Glow: return "Tail Glow";
		case Moves::Luster_Purge: return "Luster Purge";
		case Moves::Mist_Ball: return "Mist Ball";
		case Moves::FeatherDance: return "FeatherDance";
		case Moves::Teeter_Dance: return "Teeter Dance";
		case Moves::Blaze_Kick: return "Blaze Kick";
		case Moves::Mud_Sport: return "Mud Sport";
		case Moves::Ice_Ball: return "Ice Ball";
		case Moves::Needle_Arm: return "Needle Arm";
		case Moves::Slack_Off: return "Slack Off";
		case Moves::Hyper_Voice: return "Hyper Voice";
		case Moves::Poison_Fang: return "Poison Fang";
		case Moves::Crush_Claw: return "Crush Claw";
		case Moves::Blast_Burn: return "Blast Burn";
		case Moves::Hydro_Cannon: return "Hydro Cannon";
		case Moves::Meteor_Mash: return "Meteor Mash";
		case Moves::Astonish: return "Astonish";
		case Moves::Weather_Ball: return "Weather Ball";
		case Moves::Aromatherapy: return "Aromatherapy";
		case Moves::Fake_Tears: return "Fake Tears";
		case Moves::Air_Cutter: return "Air Cutter";
		case Moves::Overheat: return "Overheat";
		case Moves::Odor_Sleuth: return "Odor Sleuth";
		case Moves::Rock_Tomb: return "Rock Tomb";
		case Moves::Silver_Wind: return "Silver Wind";
		case Moves::Metal_Sound: return "Metal Sound";
		case Moves::GrassWhistle: return "GrassWhistle";
		case Moves::Tickle: return "Tickle";
		case Moves::Cosmic_Power: return "Cosmic Power";
		case Moves::Water_Spout: return "Water Spout";
		case Moves::Signal_Beam: return "Signal Beam";
		case Moves::Shadow_Punch: return "Shadow Punch";
		case Moves::Extrasensory: return "Extrasensory";
		case Moves::Sky_Uppercut: return "Sky Uppercut";
		case Moves::Sand_Tomb: return "Sand Tomb";
		case Moves::Sheer_Cold: return "Sheer Cold";
		case Moves::Muddy_Water: return "Muddy Water";
		case Moves::Bullet_Seed: return "Bullet Seed";
		case Moves::Aerial_Ace: return "Aerial Ace";
		case Moves::Icicle_Spear: return "Icicle Spear";
		case Moves::Iron_Defense: return "Iron Defense";
		case Moves::Block: return "Block";
		case Moves::Howl: return "Howl";
		case Moves::Dragon_Claw: return "Dragon Claw";
		case Moves::Frenzy_Plant: return "Frenzy Plant";
		case Moves::Bulk_Up: return "Bulk Up";
		case Moves::Bounce: return "Bounce";
		case Moves::Mud_Shot: return "Mud Shot";
		case Moves::Poison_Tail: return "Poison Tail";
		case Moves::Covet: return "Covet";
		case Moves::Volt_Tackle: return "Volt Tackle";
		case Moves::Magical_Leaf: return "Magical Leaf";
		case Moves::Water_Sport: return "Water Sport";
		case Moves::Calm_Mind: return "Calm Mind";
		case Moves::Leaf_Blade: return "Leaf Blade";
		case Moves::Dragon_Dance: return "Dragon Dance";
		case Moves::Rock_Blast: return "Rock Blast";
		case Moves::Shock_Wave: return "Shock Wave";
		case Moves::Water_Pulse: return "Water Pulse";
		case Moves::Doom_Desire: return "Doom Desire";
		case Moves::Psycho_Boost: return "Psycho Boost";

		// Generation 4
		case Moves::Roost: return "Roost";
		case Moves::Gravity: return "Gravity";
		case Moves::Miracle_Eye: return "Miracle Eye";
		case Moves::Wake_Up_Slap: return "Wake-Up Slap";
		case Moves::Hammer_Arm: return "Hammer Arm";
		case Moves::Gyro_Ball: return "Gyro Ball";
		case Moves::Healing_Wish: return "Healing Wish";
		case Moves::Brine: return "Brine";
		case Moves::Natural_Gift: return "Natural Gift";
		case Moves::Feint: return "Feint";
		case Moves::Pluck: return "Pluck";
		case Moves::Tailwind: return "Tailwind";
		case Moves::Acupressure: return "Acupressure";
		case Moves::Metal_Burst: return "Metal Burst";
		case Moves::U_turn: return "U-turn";
		case Moves::Close_Combat: return "Close Combat";
		case Moves::Payback: return "Payback";
		case Moves::Assurance: return "Assurance";
		case Moves::Embargo: return "Embargo";
		case Moves::Fling: return "Fling";
		case Moves::Psycho_Shift: return "Psycho Shift";
		case Moves::Trump_Card: return "Trump Card";
		case Moves::Heal_Block: return "Heal Block";
		case Moves::Wring_Out: return "Wring Out";
		case Moves::Power_Trick: return "Power Trick";
		case Moves::Gastro_Acid: return "Gastro Acid";
		case Moves::Lucky_Chant: return "Lucky Chant";
		case Moves::Me_First: return "Me First";
		case Moves::Copycat: return "Copycat";
		case Moves::Power_Swap: return "Power Swap";
		case Moves::Guard_Swap: return "Guard Swap";
		case Moves::Punishment: return "Punishment";
		case Moves::Last_Resort: return "Last Resort";
		case Moves::Worry_Seed: return "Worry Seed";
		case Moves::Sucker_Punch: return "Sucker Punch";
		case Moves::Toxic_Spikes: return "Toxic Spikes";
		case Moves::Heart_Swap: return "Heart Swap";
		case Moves::Aqua_Ring: return "Aqua Ring";
		case Moves::Magnet_Rise: return "Magnet Rise";
		case Moves::Flare_Blitz: return "Flare Blitz";
		case Moves::Force_Palm: return "Force Palm";
		case Moves::Aura_Sphere: return "Aura Sphere";
		case Moves::Rock_Polish: return "Rock Polish";
		case Moves::Poison_Jab: return "Poison Jab";
		case Moves::Dark_Pulse: return "Dark Pulse";
		case Moves::Night_Slash: return "Night Slash";
		case Moves::Aqua_Tail: return "Aqua Tail";
		case Moves::Seed_Bomb: return "Seed Bomb";
		case Moves::Air_Slash: return "Air Slash";
		case Moves::X_Scissor: return "X-Scissor";
		case Moves::Bug_Buzz: return "Bug Buzz";
		case Moves::Dragon_Pulse: return "Dragon Pulse";
		case Moves::Dragon_Rush: return "Dragon Rush";
		case Moves::Power_Gem: return "Power Gem";
		case Moves::Drain_Punch: return "Drain Punch";
		case Moves::Vacuum_Wave: return "Vacuum Wave";
		case Moves::Focus_Blast: return "Focus Blast";
		case Moves::Energy_Ball: return "Energy Ball";
		case Moves::Brave_Bird: return "Brave Bird";
		case Moves::Earth_Power: return "Earth Power";
		case Moves::Switcheroo: return "Switcheroo";
		case Moves::Giga_Impact: return "Giga Impact";
		case Moves::Nasty_Plot: return "Nasty Plot";
		case Moves::Bullet_Punch: return "Bullet Punch";
		case Moves::Avalanche: return "Avalanche";
		case Moves::Ice_Shard: return "Ice Shard";
		case Moves::Shadow_Claw: return "Shadow Claw";
		case Moves::Thunder_Fang: return "Thunder Fang";
		case Moves::Ice_Fang: return "Ice Fang";
		case Moves::Fire_Fang: return "Fire Fang";
		case Moves::Shadow_Sneak: return "Shadow Sneak";
		case Moves::Mud_Bomb: return "Mud Bomb";
		case Moves::Psycho_Cut: return "Psycho Cut";
		case Moves::Zen_Headbutt: return "Zen Headbutt";
		case Moves::Mirror_Shot: return "Mirror Shot";
		case Moves::Flash_Cannon: return "Flash Cannon";
		case Moves::Rock_Climb: return "Rock Climb";
		case Moves::Defog: return "Defog";
		case Moves::Trick_Room: return "Trick Room";
		case Moves::Draco_Meteor: return "Draco Meteor";
		case Moves::Discharge: return "Discharge";
		case Moves::Lava_Plume: return "Lava Plume";
		case Moves::Leaf_Storm: return "Leaf Storm";
		case Moves::Power_Whip: return "Power Whip";
		case Moves::Rock_Wrecker: return "Rock Wrecker";
		case Moves::Cross_Poison: return "Cross Poison";
		case Moves::Gunk_Shot: return "Gunk Shot";
		case Moves::Iron_Head: return "Iron Head";
		case Moves::Magnet_Bomb: return "Magnet Bomb";
		case Moves::Stone_Edge: return "Stone Edge";
		case Moves::Captivate: return "Captivate";
		case Moves::Stealth_Rock: return "Stealth Rock";
		case Moves::Grass_Knot: return "Grass Knot";
		case Moves::Chatter: return "Chatter";
		case Moves::Judgment: return "Judgment";
		case Moves::Bug_Bite: return "Bug Bite";
		case Moves::Charge_Beam: return "Charge Beam";
		case Moves::Wood_Hammer: return "Wood Hammer";
		case Moves::Aqua_Jet: return "Aqua Jet";
		case Moves::Attack_Order: return "Attack Order";
		case Moves::Defend_Order: return "Defend Order";
		case Moves::Heal_Order: return "Heal Order";
		case Moves::Head_Smash: return "Head Smash";
		case Moves::Double_Hit: return "Double Hit";
		case Moves::Roar_of_Time: return "Roar of Time";
		case Moves::Spacial_Rend: return "Spacial Rend";
		case Moves::Lunar_Dance: return "Lunar Dance";
		case Moves::Crush_Grip: return "Crush Grip";
		case Moves::Magma_Storm: return "Magma Storm";
		case Moves::Dark_Void: return "Dark Void";
		case Moves::Seed_Flare: return "Seed Flare";
		case Moves::Ominous_Wind: return "Ominous Wind";
		case Moves::Shadow_Force: return "Shadow Force";
		
		// Generation 5
		case Moves::Hone_Claws: return "Hone Claws";
		case Moves::Wide_Guard: return "Wide Guard";
		case Moves::Guard_Split: return "Guard Split";
		case Moves::Power_Split: return "Power Split";
		case Moves::Wonder_Room: return "Wonder Room";
		case Moves::Psyshock: return "Psyshock";
		case Moves::Venoshock: return "Venoshock";
		case Moves::Autotomize: return "Autotomize";
		case Moves::Rage_Powder: return "Rage Powder";
		case Moves::Telekinesis: return "Telekinesis";
		case Moves::Magic_Room: return "Magic Room";
		case Moves::Smack_Down: return "Smack Down";
		case Moves::Storm_Throw: return "Storm Throw";
		case Moves::Flame_Burst: return "Flame Burst";
		case Moves::Sludge_Wave: return "Sludge Wave";
		case Moves::Quiver_Dance: return "Quiver Dance";
		case Moves::Heavy_Slam: return "Heavy Slam";
		case Moves::Synchronoise: return "Synchronoise";
		case Moves::Electro_Ball: return "Electro Ball";
		case Moves::Soak: return "Soak";
		case Moves::Flame_Charge: return "Flame Charge";
		case Moves::Coil: return "Coil";
		case Moves::Low_Sweep: return "Low Sweep";
		case Moves::Acid_Spray: return "Acid Spray";
		case Moves::Foul_Play: return "Foul Play";
		case Moves::Simple_Beam: return "Simple Beam";
		case Moves::Entrainment: return "Entrainment";
		case Moves::After_You: return "After You";
		case Moves::Round: return "Round";
		case Moves::Echoed_Voice: return "Echoed Voice";
		case Moves::Chip_Away: return "Chip Away";
		case Moves::Clear_Smog: return "Clear Smog";
		case Moves::Stored_Power: return "Stored Power";
		case Moves::Quick_Guard: return "Quick Guard";
		case Moves::Ally_Switch: return "Ally Switch";
		case Moves::Scald: return "Scald";
		case Moves::Shell_Smash: return "Shell Smash";
		case Moves::Heal_Pulse: return "Heal Pulse";
		case Moves::Hex: return "Hex";
		case Moves::Sky_Drop: return "Sky Drop";
		case Moves::Shift_Gear: return "Shift Gear";
		case Moves::Circle_Throw: return "Circle Throw";
		case Moves::Incinerate: return "Incinerate";
		case Moves::Quash: return "Quash";
		case Moves::Acrobatics: return "Acrobatics";
		case Moves::Reflect_Type: return "Reflect Type";
		case Moves::Retaliate: return "Retaliate";
		case Moves::Final_Gambit: return "Final Gambit";
		case Moves::Bestow: return "Bestow";
		case Moves::Inferno: return "Inferno";
		case Moves::Water_Pledge: return "Water Pledge";
		case Moves::Fire_Pledge: return "Fire Pledge";
		case Moves::Grass_Pledge: return "Grass Pledge";
		case Moves::Volt_Switch: return "Volt Switch";
		case Moves::Struggle_Bug: return "Struggle Bug";
		case Moves::Bulldoze: return "Bulldoze";
		case Moves::Frost_Breath: return "Frost Breath";
		case Moves::Dragon_Tail: return "Dragon Tail";
		case Moves::Work_Up: return "Work Up";
		case Moves::Electroweb: return "Electroweb";
		case Moves::Wild_Charge: return "Wild Charge";
		case Moves::Drill_Run: return "Drill Run";
		case Moves::Dual_Chop: return "Dual Chop";
		case Moves::Heart_Stamp: return "Heart Stamp";
		case Moves::Horn_Leech: return "Horn Leech";
		case Moves::Sacred_Sword: return "Sacred Sword";
		case Moves::Razor_Shell: return "Razor Shell";
		case Moves::Heat_Crash: return "Heat Crash";
		case Moves::Leaf_Tornado: return "Leaf Tornado";
		case Moves::Steamroller: return "Steamroller";
		case Moves::Cotton_Guard: return "Cotton Guard";
		case Moves::Night_Daze: return "Night Daze";
		case Moves::Psystrike: return "Psystrike";
		case Moves::Tail_Slap: return "Tail Slap";
		case Moves::Hurricane: return "Hurricane";
		case Moves::Head_Charge: return "Head Charge";
		case Moves::Gear_Grind: return "Gear Grind";
		case Moves::Searing_Shot: return "Searing Shot";
		case Moves::Techno_Blast: return "Techno Blast";
		case Moves::Relic_Song: return "Relic Song";
		case Moves::Secret_Sword: return "Secret Sword";
		case Moves::Glaciate: return "Glaciate";
		case Moves::Bolt_Strike: return "Bolt Strike";
		case Moves::Blue_Flare: return "Blue Flare";
		case Moves::Fiery_Dance: return "Fiery Dance";
		case Moves::Freeze_Shock: return "Freeze Shock";
		case Moves::Ice_Burn: return "Ice Burn";
		case Moves::Snarl: return "Snarl";
		case Moves::Icicle_Crash: return "Icicle Crash";
		case Moves::V_create: return "V-create";
		case Moves::Fusion_Flare: return "Fusion Flare";
		case Moves::Fusion_Bolt: return "Fusion Bolt";
	}
}

template<>
Moves from_string(std::string_view const str) {
	static std::map<std::string_view, Moves, lowercase_alphanumeric> const converter {
		{ "pass", Moves::Pass },
		{ "switch0", Moves::Switch0 },
		{ "switch1", Moves::Switch1 },
		{ "switch2", Moves::Switch2 },
		{ "switch3", Moves::Switch3 },
		{ "switch4", Moves::Switch4 },
		{ "switch5", Moves::Switch5 },
		{ "hit self in confusion", Moves::Hit_Self },
		{ "pound", Moves::Pound },
		{ "karatechop", Moves::Karate_Chop },
		{ "doubleslap", Moves::DoubleSlap },
		{ "cometpunch", Moves::Comet_Punch },
		{ "megapunch", Moves::Mega_Punch },
		{ "payday", Moves::Pay_Day },
		{ "firepunch", Moves::Fire_Punch },
		{ "icepunch", Moves::Ice_Punch },
		{ "thunderpunch", Moves::ThunderPunch },
		{ "scratch", Moves::Scratch },
		{ "vicegrip", Moves::ViceGrip },
		{ "guillotine", Moves::Guillotine },
		{ "razorwind", Moves::Razor_Wind },
		{ "swordsdance", Moves::Swords_Dance },
		{ "cut", Moves::Cut },
		{ "gust", Moves::Gust },
		{ "wingattack", Moves::Wing_Attack },
		{ "whirlwind", Moves::Whirlwind },
		{ "fly", Moves::Fly },
		{ "bind", Moves::Bind },
		{ "slam", Moves::Slam },
		{ "vinewhip", Moves::Vine_Whip },
		{ "stomp", Moves::Stomp },
		{ "doublekick", Moves::Double_Kick },
		{ "megakick", Moves::Mega_Kick },
		{ "jumpkick", Moves::Jump_Kick },
		{ "rollingkick", Moves::Rolling_Kick },
		{ "sandattack", Moves::Sand_Attack },
		{ "headbutt", Moves::Headbutt },
		{ "hornattack", Moves::Horn_Attack },
		{ "furyattack", Moves::Fury_Attack },
		{ "horndrill", Moves::Horn_Drill },
		{ "tackle", Moves::Tackle },
		{ "bodyslam", Moves::Body_Slam },
		{ "wrap", Moves::Wrap },
		{ "takedown", Moves::Take_Down },
		{ "thrash", Moves::Thrash },
		{ "doubleedge", Moves::Double_Edge },
		{ "tailwhip", Moves::Tail_Whip },
		{ "poisonsting", Moves::Poison_Sting },
		{ "twineedle", Moves::Twineedle },
		{ "pinmissile", Moves::Pin_Missile },
		{ "leer", Moves::Leer },
		{ "bite", Moves::Bite },
		{ "growl", Moves::Growl },
		{ "roar", Moves::Roar },
		{ "sing", Moves::Sing },
		{ "supersonic", Moves::Supersonic },
		{ "sonicboom", Moves::SonicBoom },
		{ "disable", Moves::Disable },
		{ "acid", Moves::Acid },
		{ "ember", Moves::Ember },
		{ "flamethrower", Moves::Flamethrower },
		{ "mist", Moves::Mist },
		{ "watergun", Moves::Water_Gun },
		{ "hydropump", Moves::Hydro_Pump },
		{ "surf", Moves::Surf },
		{ "icebeam", Moves::Ice_Beam },
		{ "blizzard", Moves::Blizzard },
		{ "psybeam", Moves::Psybeam },
		{ "bubblebeam", Moves::BubbleBeam },
		{ "aurorabeam", Moves::Aurora_Beam },
		{ "hyperbeam", Moves::Hyper_Beam },
		{ "peck", Moves::Peck },
		{ "drillpeck", Moves::Drill_Peck },
		{ "submission", Moves::Submission },
		{ "lowkick", Moves::Low_Kick },
		{ "counter", Moves::Counter },
		{ "seismictoss", Moves::Seismic_Toss },
		{ "strength", Moves::Strength },
		{ "absorb", Moves::Absorb },
		{ "megadrain", Moves::Mega_Drain },
		{ "leechseed", Moves::Leech_Seed },
		{ "growth", Moves::Growth },
		{ "razorleaf", Moves::Razor_Leaf },
		{ "solarbeam", Moves::SolarBeam },
		{ "poisonpowder", Moves::PoisonPowder },
		{ "stunspore", Moves::Stun_Spore },
		{ "sleeppowder", Moves::Sleep_Powder },
		{ "petaldance", Moves::Petal_Dance },
		{ "stringshot", Moves::String_Shot },
		{ "dragonrage", Moves::Dragon_Rage },
		{ "firespin", Moves::Fire_Spin },
		{ "thundershock", Moves::ThunderShock },
		{ "thunderbolt", Moves::Thunderbolt },
		{ "thunderwave", Moves::Thunder_Wave },
		{ "thunder", Moves::Thunder },
		{ "rockthrow", Moves::Rock_Throw },
		{ "earthquake", Moves::Earthquake },
		{ "fissure", Moves::Fissure },
		{ "dig", Moves::Dig },
		{ "toxic", Moves::Toxic },
		{ "confusion", Moves::Confusion },
		{ "psychic", Moves::Psychic },
		{ "hypnosis", Moves::Hypnosis },
		{ "meditate", Moves::Meditate },
		{ "agility", Moves::Agility },
		{ "quickattack", Moves::Quick_Attack },
		{ "rage", Moves::Rage },
		{ "teleport", Moves::Teleport },
		{ "nightshade", Moves::Night_Shade },
		{ "mimic", Moves::Mimic },
		{ "screech", Moves::Screech },
		{ "doubleteam", Moves::Double_Team },
		{ "recover", Moves::Recover },
		{ "harden", Moves::Harden },
		{ "minimize", Moves::Minimize },
		{ "smokescreen", Moves::SmokeScreen },
		{ "confuseray", Moves::Confuse_Ray },
		{ "withdraw", Moves::Withdraw },
		{ "defensecurl", Moves::Defense_Curl },
		{ "barrier", Moves::Barrier },
		{ "lightscreen", Moves::Light_Screen },
		{ "haze", Moves::Haze },
		{ "reflect", Moves::Reflect },
		{ "focusenergy", Moves::Focus_Energy },
		{ "bide", Moves::Bide },
		{ "metronome", Moves::Metronome },
		{ "mirrormove", Moves::Mirror_Move },
		{ "selfdestruct", Moves::Selfdestruct },
		{ "eggbomb", Moves::Egg_Bomb },
		{ "lick", Moves::Lick },
		{ "smog", Moves::Smog },
		{ "sludge", Moves::Sludge },
		{ "boneclub", Moves::Bone_Club },
		{ "fireblast", Moves::Fire_Blast },
		{ "waterfall", Moves::Waterfall },
		{ "clamp", Moves::Clamp },
		{ "swift", Moves::Swift },
		{ "skullbash", Moves::Skull_Bash },
		{ "spikecannon", Moves::Spike_Cannon },
		{ "constrict", Moves::Constrict },
		{ "amnesia", Moves::Amnesia },
		{ "kinesis", Moves::Kinesis },
		{ "softboiled", Moves::Softboiled },
		{ "highjumpkick", Moves::Hi_Jump_Kick },
		{ "hijumpkick", Moves::Hi_Jump_Kick },
		{ "glare", Moves::Glare },
		{ "dreameater", Moves::Dream_Eater },
		{ "poisongas", Moves::Poison_Gas },
		{ "barrage", Moves::Barrage },
		{ "leechlife", Moves::Leech_Life },
		{ "lovelykiss", Moves::Lovely_Kiss },
		{ "skyattack", Moves::Sky_Attack },
		{ "transform", Moves::Transform },
		{ "bubble", Moves::Bubble },
		{ "dizzypunch", Moves::Dizzy_Punch },
		{ "spore", Moves::Spore },
		{ "flash", Moves::Flash },
		{ "psywave", Moves::Psywave },
		{ "splash", Moves::Splash },
		{ "acidarmor", Moves::Acid_Armor },
		{ "crabhammer", Moves::Crabhammer },
		{ "explosion", Moves::Explosion },
		{ "furyswipes", Moves::Fury_Swipes },
		{ "bonemerang", Moves::Bonemerang },
		{ "rest", Moves::Rest },
		{ "rockslide", Moves::Rock_Slide },
		{ "hyperfang", Moves::Hyper_Fang },
		{ "sharpen", Moves::Sharpen },
		{ "conversion", Moves::Conversion },
		{ "triattack", Moves::Tri_Attack },
		{ "superfang", Moves::Super_Fang },
		{ "slash", Moves::Slash },
		{ "substitute", Moves::Substitute },
		{ "struggle", Moves::Struggle },
		{ "sketch", Moves::Sketch },
		{ "triplekick", Moves::Triple_Kick },
		{ "thief", Moves::Thief },
		{ "spiderweb", Moves::Spider_Web },
		{ "mindreader", Moves::Mind_Reader },
		{ "nightmare", Moves::Nightmare },
		{ "flamewheel", Moves::Flame_Wheel },
		{ "snore", Moves::Snore },
		{ "curse", Moves::Curse },
		{ "flail", Moves::Flail },
		{ "conversion2", Moves::Conversion_2 },
		{ "aeroblast", Moves::Aeroblast },
		{ "cottonspore", Moves::Cotton_Spore },
		{ "reversal", Moves::Reversal },
		{ "spite", Moves::Spite },
		{ "powdersnow", Moves::Powder_Snow },
		{ "protect", Moves::Protect },
		{ "machpunch", Moves::Mach_Punch },
		{ "scaryface", Moves::Scary_Face },
		{ "faintattack", Moves::Faint_Attack },
		{ "sweetkiss", Moves::Sweet_Kiss },
		{ "bellydrum", Moves::Belly_Drum },
		{ "sludgebomb", Moves::Sludge_Bomb },
		{ "mudslap", Moves::Mud_Slap },
		{ "mudslap", Moves::Mud_Slap },
		{ "octazooka", Moves::Octazooka },
		{ "spikes", Moves::Spikes },
		{ "zapcannon", Moves::Zap_Cannon },
		{ "foresight", Moves::Foresight },
		{ "destinybond", Moves::Destiny_Bond },
		{ "perishsong", Moves::Perish_Song },
		{ "icywind", Moves::Icy_Wind },
		{ "detect", Moves::Detect },
		{ "bonerush", Moves::Bone_Rush },
		{ "lockon", Moves::Lock_On },
		{ "outrage", Moves::Outrage },
		{ "sandstorm", Moves::Sandstorm },
		{ "gigadrain", Moves::Giga_Drain },
		{ "endure", Moves::Endure },
		{ "charm", Moves::Charm },
		{ "rollout", Moves::Rollout },
		{ "falseswipe", Moves::False_Swipe },
		{ "swagger", Moves::Swagger },
		{ "milkdrink", Moves::Milk_Drink },
		{ "spark", Moves::Spark },
		{ "furycutter", Moves::Fury_Cutter },
		{ "steelwing", Moves::Steel_Wing },
		{ "meanlook", Moves::Mean_Look },
		{ "attract", Moves::Attract },
		{ "sleeptalk", Moves::Sleep_Talk },
		{ "healbell", Moves::Heal_Bell },
		{ "return", Moves::Return },
		{ "present", Moves::Present },
		{ "frustration", Moves::Frustration },
		{ "safeguard", Moves::Safeguard },
		{ "painsplit", Moves::Pain_Split },
		{ "sacredfire", Moves::Sacred_Fire },
		{ "magnitude", Moves::Magnitude },
		{ "dynamicpunch", Moves::DynamicPunch },
		{ "megahorn", Moves::Megahorn },
		{ "dragonbreath", Moves::DragonBreath },
		{ "batonpass", Moves::Baton_Pass },
		{ "encore", Moves::Encore },
		{ "pursuit", Moves::Pursuit },
		{ "rapidspin", Moves::Rapid_Spin },
		{ "sweetscent", Moves::Sweet_Scent },
		{ "irontail", Moves::Iron_Tail },
		{ "metalclaw", Moves::Metal_Claw },
		{ "vitalthrow", Moves::Vital_Throw },
		{ "morningsun", Moves::Morning_Sun },
		{ "synthesis", Moves::Synthesis },
		{ "moonlight", Moves::Moonlight },
		{ "hiddenpower", Moves::Hidden_Power },
		{ "hiddenpowerbug", Moves::Hidden_Power },
		{ "hiddenpowerdark", Moves::Hidden_Power },
		{ "hiddenpowerdragon", Moves::Hidden_Power },
		{ "hiddenpowerelectric", Moves::Hidden_Power },
		{ "hiddenpowerfighting", Moves::Hidden_Power },
		{ "hiddenpowerfire", Moves::Hidden_Power },
		{ "hiddenpowerflying", Moves::Hidden_Power },
		{ "hiddenpowerghost", Moves::Hidden_Power },
		{ "hiddenpowergrass", Moves::Hidden_Power },
		{ "hiddenpowerground", Moves::Hidden_Power },
		{ "hiddenpowerice", Moves::Hidden_Power },
		{ "hiddenpowerpoison", Moves::Hidden_Power },
		{ "hiddenpowerpsychic", Moves::Hidden_Power },
		{ "hiddenpowerrock", Moves::Hidden_Power },
		{ "hiddenpowersteel", Moves::Hidden_Power },
		{ "hiddenpowerwater", Moves::Hidden_Power },
		{ "crosschop", Moves::Cross_Chop },
		{ "twister", Moves::Twister },
		{ "raindance", Moves::Rain_Dance },
		{ "sunnyday", Moves::Sunny_Day },
		{ "crunch", Moves::Crunch },
		{ "mirrorcoat", Moves::Mirror_Coat },
		{ "psychup", Moves::Psych_Up },
		{ "extremespeed", Moves::ExtremeSpeed },
		{ "ancientpower", Moves::AncientPower },
		{ "shadowball", Moves::Shadow_Ball },
		{ "futuresight", Moves::Future_Sight },
		{ "rocksmash", Moves::Rock_Smash },
		{ "whirlpool", Moves::Whirlpool },
		{ "beatup", Moves::Beat_Up },
		{ "fakeout", Moves::Fake_Out },
		{ "uproar", Moves::Uproar },
		{ "stockpile", Moves::Stockpile },
		{ "spitup", Moves::Spit_Up },
		{ "swallow", Moves::Swallow },
		{ "heatwave", Moves::Heat_Wave },
		{ "hail", Moves::Hail },
		{ "torment", Moves::Torment },
		{ "flatter", Moves::Flatter },
		{ "willowisp", Moves::Will_O_Wisp },
		{ "memento", Moves::Memento },
		{ "facade", Moves::Facade },
		{ "focuspunch", Moves::Focus_Punch },
		{ "smellingsalt", Moves::SmellingSalt },
		{ "followme", Moves::Follow_Me },
		{ "naturepower", Moves::Nature_Power },
		{ "charge", Moves::Charge },
		{ "taunt", Moves::Taunt },
		{ "helpinghand", Moves::Helping_Hand },
		{ "trick", Moves::Trick },
		{ "roleplay", Moves::Role_Play },
		{ "wish", Moves::Wish },
		{ "assist", Moves::Assist },
		{ "ingrain", Moves::Ingrain },
		{ "superpower", Moves::Superpower },
		{ "magiccoat", Moves::Magic_Coat },
		{ "recycle", Moves::Recycle },
		{ "revenge", Moves::Revenge },
		{ "brickbreak", Moves::Brick_Break },
		{ "yawn", Moves::Yawn },
		{ "knockoff", Moves::Knock_Off },
		{ "endeavor", Moves::Endeavor },
		{ "eruption", Moves::Eruption },
		{ "skillswap", Moves::Skill_Swap },
		{ "imprison", Moves::Imprison },
		{ "refresh", Moves::Refresh },
		{ "grudge", Moves::Grudge },
		{ "snatch", Moves::Snatch },
		{ "secretpower", Moves::Secret_Power },
		{ "dive", Moves::Dive },
		{ "armthrust", Moves::Arm_Thrust },
		{ "camouflage", Moves::Camouflage },
		{ "tailglow", Moves::Tail_Glow },
		{ "lusterpurge", Moves::Luster_Purge },
		{ "mistball", Moves::Mist_Ball },
		{ "featherdance", Moves::FeatherDance },
		{ "teeterdance", Moves::Teeter_Dance },
		{ "blazekick", Moves::Blaze_Kick },
		{ "mudsport", Moves::Mud_Sport },
		{ "iceball", Moves::Ice_Ball },
		{ "needlearm", Moves::Needle_Arm },
		{ "slackoff", Moves::Slack_Off },
		{ "hypervoice", Moves::Hyper_Voice },
		{ "poisonfang", Moves::Poison_Fang },
		{ "crushclaw", Moves::Crush_Claw },
		{ "blastburn", Moves::Blast_Burn },
		{ "hydrocannon", Moves::Hydro_Cannon },
		{ "meteormash", Moves::Meteor_Mash },
		{ "astonish", Moves::Astonish },
		{ "weatherball", Moves::Weather_Ball },
		{ "aromatherapy", Moves::Aromatherapy },
		{ "faketears", Moves::Fake_Tears },
		{ "aircutter", Moves::Air_Cutter },
		{ "overheat", Moves::Overheat },
		{ "odorsleuth", Moves::Odor_Sleuth },
		{ "rocktomb", Moves::Rock_Tomb },
		{ "silverwind", Moves::Silver_Wind },
		{ "metalsound", Moves::Metal_Sound },
		{ "grasswhistle", Moves::GrassWhistle },
		{ "tickle", Moves::Tickle },
		{ "cosmicpower", Moves::Cosmic_Power },
		{ "waterspout", Moves::Water_Spout },
		{ "signalbeam", Moves::Signal_Beam },
		{ "shadowpunch", Moves::Shadow_Punch },
		{ "extrasensory", Moves::Extrasensory },
		{ "skyuppercut", Moves::Sky_Uppercut },
		{ "sandtomb", Moves::Sand_Tomb },
		{ "sheercold", Moves::Sheer_Cold },
		{ "muddywater", Moves::Muddy_Water },
		{ "bulletseed", Moves::Bullet_Seed },
		{ "aerialace", Moves::Aerial_Ace },
		{ "iciclespear", Moves::Icicle_Spear },
		{ "irondefense", Moves::Iron_Defense },
		{ "block", Moves::Block },
		{ "howl", Moves::Howl },
		{ "dragonclaw", Moves::Dragon_Claw },
		{ "frenzyplant", Moves::Frenzy_Plant },
		{ "bulkup", Moves::Bulk_Up },
		{ "bounce", Moves::Bounce },
		{ "mudshot", Moves::Mud_Shot },
		{ "poisontail", Moves::Poison_Tail },
		{ "covet", Moves::Covet },
		{ "volttackle", Moves::Volt_Tackle },
		{ "magicalleaf", Moves::Magical_Leaf },
		{ "watersport", Moves::Water_Sport },
		{ "calmmind", Moves::Calm_Mind },
		{ "leafblade", Moves::Leaf_Blade },
		{ "dragondance", Moves::Dragon_Dance },
		{ "rockblast", Moves::Rock_Blast },
		{ "shockwave", Moves::Shock_Wave },
		{ "waterpulse", Moves::Water_Pulse },
		{ "doomdesire", Moves::Doom_Desire },
		{ "psychoboost", Moves::Psycho_Boost },
		{ "roost", Moves::Roost },
		{ "gravity", Moves::Gravity },
		{ "miracleeye", Moves::Miracle_Eye },
		{ "wakeupslap", Moves::Wake_Up_Slap },
		{ "hammerarm", Moves::Hammer_Arm },
		{ "gyroball", Moves::Gyro_Ball },
		{ "healingwish", Moves::Healing_Wish },
		{ "brine", Moves::Brine },
		{ "naturalgift", Moves::Natural_Gift },
		{ "feint", Moves::Feint },
		{ "pluck", Moves::Pluck },
		{ "tailwind", Moves::Tailwind },
		{ "acupressure", Moves::Acupressure },
		{ "metalburst", Moves::Metal_Burst },
		{ "uturn", Moves::U_turn },
		{ "closecombat", Moves::Close_Combat },
		{ "payback", Moves::Payback },
		{ "assurance", Moves::Assurance },
		{ "embargo", Moves::Embargo },
		{ "fling", Moves::Fling },
		{ "psychoshift", Moves::Psycho_Shift },
		{ "trumpcard", Moves::Trump_Card },
		{ "healblock", Moves::Heal_Block },
		{ "wringout", Moves::Wring_Out },
		{ "powertrick", Moves::Power_Trick },
		{ "gastroacid", Moves::Gastro_Acid },
		{ "luckychant", Moves::Lucky_Chant },
		{ "mefirst", Moves::Me_First },
		{ "copycat", Moves::Copycat },
		{ "powerswap", Moves::Power_Swap },
		{ "guardswap", Moves::Guard_Swap },
		{ "punishment", Moves::Punishment },
		{ "lastresort", Moves::Last_Resort },
		{ "worryseed", Moves::Worry_Seed },
		{ "suckerpunch", Moves::Sucker_Punch },
		{ "toxicspikes", Moves::Toxic_Spikes },
		{ "heartswap", Moves::Heart_Swap },
		{ "aquaring", Moves::Aqua_Ring },
		{ "magnetrise", Moves::Magnet_Rise },
		{ "flareblitz", Moves::Flare_Blitz },
		{ "forcepalm", Moves::Force_Palm },
		{ "aurasphere", Moves::Aura_Sphere },
		{ "rockpolish", Moves::Rock_Polish },
		{ "poisonjab", Moves::Poison_Jab },
		{ "darkpulse", Moves::Dark_Pulse },
		{ "nightslash", Moves::Night_Slash },
		{ "aquatail", Moves::Aqua_Tail },
		{ "seedbomb", Moves::Seed_Bomb },
		{ "airslash", Moves::Air_Slash },
		{ "xscissor", Moves::X_Scissor },
		{ "bugbuzz", Moves::Bug_Buzz },
		{ "dragonpulse", Moves::Dragon_Pulse },
		{ "dragonrush", Moves::Dragon_Rush },
		{ "powergem", Moves::Power_Gem },
		{ "drainpunch", Moves::Drain_Punch },
		{ "vacuumwave", Moves::Vacuum_Wave },
		{ "focusblast", Moves::Focus_Blast },
		{ "energyball", Moves::Energy_Ball },
		{ "bravebird", Moves::Brave_Bird },
		{ "earthpower", Moves::Earth_Power },
		{ "switcheroo", Moves::Switcheroo },
		{ "gigaimpact", Moves::Giga_Impact },
		{ "nastyplot", Moves::Nasty_Plot },
		{ "bulletpunch", Moves::Bullet_Punch },
		{ "avalanche", Moves::Avalanche },
		{ "iceshard", Moves::Ice_Shard },
		{ "shadowclaw", Moves::Shadow_Claw },
		{ "thunderfang", Moves::Thunder_Fang },
		{ "icefang", Moves::Ice_Fang },
		{ "firefang", Moves::Fire_Fang },
		{ "shadowsneak", Moves::Shadow_Sneak },
		{ "mudbomb", Moves::Mud_Bomb },
		{ "psychocut", Moves::Psycho_Cut },
		{ "zenheadbutt", Moves::Zen_Headbutt },
		{ "mirrorshot", Moves::Mirror_Shot },
		{ "flashcannon", Moves::Flash_Cannon },
		{ "rockclimb", Moves::Rock_Climb },
		{ "defog", Moves::Defog },
		{ "trickroom", Moves::Trick_Room },
		{ "dracometeor", Moves::Draco_Meteor },
		{ "discharge", Moves::Discharge },
		{ "lavaplume", Moves::Lava_Plume },
		{ "leafstorm", Moves::Leaf_Storm },
		{ "powerwhip", Moves::Power_Whip },
		{ "rockwrecker", Moves::Rock_Wrecker },
		{ "crosspoison", Moves::Cross_Poison },
		{ "gunkshot", Moves::Gunk_Shot },
		{ "ironhead", Moves::Iron_Head },
		{ "magnetbomb", Moves::Magnet_Bomb },
		{ "stoneedge", Moves::Stone_Edge },
		{ "captivate", Moves::Captivate },
		{ "stealthrock", Moves::Stealth_Rock },
		{ "grassknot", Moves::Grass_Knot },
		{ "chatter", Moves::Chatter },
		{ "judgment", Moves::Judgment },
		{ "bugbite", Moves::Bug_Bite },
		{ "chargebeam", Moves::Charge_Beam },
		{ "woodhammer", Moves::Wood_Hammer },
		{ "aquajet", Moves::Aqua_Jet },
		{ "attackorder", Moves::Attack_Order },
		{ "defendorder", Moves::Defend_Order },
		{ "healorder", Moves::Heal_Order },
		{ "headsmash", Moves::Head_Smash },
		{ "doublehit", Moves::Double_Hit },
		{ "roaroftime", Moves::Roar_of_Time },
		{ "roaroftime", Moves::Roar_of_Time },
		{ "spacialrend", Moves::Spacial_Rend },
		{ "lunardance", Moves::Lunar_Dance },
		{ "crushgrip", Moves::Crush_Grip },
		{ "magmastorm", Moves::Magma_Storm },
		{ "darkvoid", Moves::Dark_Void },
		{ "seedflare", Moves::Seed_Flare },
		{ "ominouswind", Moves::Ominous_Wind },
		{ "shadowforce", Moves::Shadow_Force },
		{ "honeclaws", Moves::Hone_Claws },
		{ "wideguard", Moves::Wide_Guard },
		{ "guardsplit", Moves::Guard_Split },
		{ "powersplit", Moves::Power_Split },
		{ "wonderroom", Moves::Wonder_Room },
		{ "psyshock", Moves::Psyshock },
		{ "venoshock", Moves::Venoshock },
		{ "autotomize", Moves::Autotomize },
		{ "ragepowder", Moves::Rage_Powder },
		{ "telekinesis", Moves::Telekinesis },
		{ "magicroom", Moves::Magic_Room },
		{ "smackdown", Moves::Smack_Down },
		{ "stormthrow", Moves::Storm_Throw },
		{ "flameburst", Moves::Flame_Burst },
		{ "sludgewave", Moves::Sludge_Wave },
		{ "quiverdance", Moves::Quiver_Dance },
		{ "heavyslam", Moves::Heavy_Slam },
		{ "synchronoise", Moves::Synchronoise },
		{ "electroball", Moves::Electro_Ball },
		{ "soak", Moves::Soak },
		{ "flamecharge", Moves::Flame_Charge },
		{ "coil", Moves::Coil },
		{ "lowsweep", Moves::Low_Sweep },
		{ "acidspray", Moves::Acid_Spray },
		{ "foulplay", Moves::Foul_Play },
		{ "simplebeam", Moves::Simple_Beam },
		{ "entrainment", Moves::Entrainment },
		{ "afteryou", Moves::After_You },
		{ "round", Moves::Round },
		{ "echoedvoice", Moves::Echoed_Voice },
		{ "chipaway", Moves::Chip_Away },
		{ "clearsmog", Moves::Clear_Smog },
		{ "storedpower", Moves::Stored_Power },
		{ "quickguard", Moves::Quick_Guard },
		{ "allyswitch", Moves::Ally_Switch },
		{ "scald", Moves::Scald },
		{ "shellsmash", Moves::Shell_Smash },
		{ "healpulse", Moves::Heal_Pulse },
		{ "hex", Moves::Hex },
		{ "skydrop", Moves::Sky_Drop },
		{ "shiftgear", Moves::Shift_Gear },
		{ "circlethrow", Moves::Circle_Throw },
		{ "incinerate", Moves::Incinerate },
		{ "quash", Moves::Quash },
		{ "acrobatics", Moves::Acrobatics },
		{ "reflecttype", Moves::Reflect_Type },
		{ "retaliate", Moves::Retaliate },
		{ "finalgambit", Moves::Final_Gambit },
		{ "bestow", Moves::Bestow },
		{ "inferno", Moves::Inferno },
		{ "waterpledge", Moves::Water_Pledge },
		{ "firepledge", Moves::Fire_Pledge },
		{ "grasspledge", Moves::Grass_Pledge },
		{ "voltswitch", Moves::Volt_Switch },
		{ "strugglebug", Moves::Struggle_Bug },
		{ "bulldoze", Moves::Bulldoze },
		{ "frostbreath", Moves::Frost_Breath },
		{ "dragontail", Moves::Dragon_Tail },
		{ "workup", Moves::Work_Up },
		{ "electroweb", Moves::Electroweb },
		{ "wildcharge", Moves::Wild_Charge },
		{ "drillrun", Moves::Drill_Run },
		{ "dualchop", Moves::Dual_Chop },
		{ "heartstamp", Moves::Heart_Stamp },
		{ "hornleech", Moves::Horn_Leech },
		{ "sacredsword", Moves::Sacred_Sword },
		{ "razorshell", Moves::Razor_Shell },
		{ "heatcrash", Moves::Heat_Crash },
		{ "leaftornado", Moves::Leaf_Tornado },
		{ "steamroller", Moves::Steamroller },
		{ "cottonguard", Moves::Cotton_Guard },
		{ "nightdaze", Moves::Night_Daze },
		{ "psystrike", Moves::Psystrike },
		{ "tailslap", Moves::Tail_Slap },
		{ "hurricane", Moves::Hurricane },
		{ "headcharge", Moves::Head_Charge },
		{ "geargrind", Moves::Gear_Grind },
		{ "searingshot", Moves::Searing_Shot },
		{ "technoblast", Moves::Techno_Blast },
		{ "relicsong", Moves::Relic_Song },
		{ "secretsword", Moves::Secret_Sword },
		{ "glaciate", Moves::Glaciate },
		{ "boltstrike", Moves::Bolt_Strike },
		{ "blueflare", Moves::Blue_Flare },
		{ "fierydance", Moves::Fiery_Dance },
		{ "freezeshock", Moves::Freeze_Shock },
		{ "iceburn", Moves::Ice_Burn },
		{ "snarl", Moves::Snarl },
		{ "iciclecrash", Moves::Icicle_Crash },
		{ "vcreate", Moves::V_create },
		{ "fusionflare", Moves::Fusion_Flare },
		{ "fusionbolt", Moves::Fusion_Bolt },
	};
	auto const it = converter.find(str);
	if (it != end(converter)) {
		return it->second;
	} else {
		throw InvalidFromStringConversion("Move", str);
	}
}

}	// namespace technicalmachine
