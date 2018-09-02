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
		// Generation 1
		case Moves::Switch0: return "Switch0";
		case Moves::Switch1: return "Switch1";
		case Moves::Switch2: return "Switch2";
		case Moves::Switch3: return "Switch3";
		case Moves::Switch4: return "Switch4";
		case Moves::Switch5: return "Switch5";
		case Moves::Hit_Self: return "Hit self in confusion";
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
	static std::map<std::string_view, Moves, lowercase_ordering> const converter {
		{ "switch0", Moves::Switch0 },
		{ "switch1", Moves::Switch1 },
		{ "switch2", Moves::Switch2 },
		{ "switch3", Moves::Switch3 },
		{ "switch4", Moves::Switch4 },
		{ "switch5", Moves::Switch5 },
		{ "hit self in confusion", Moves::Hit_Self },
		{ "pound", Moves::Pound },
		{ "karate chop", Moves::Karate_Chop },
		{ "karatechop", Moves::Karate_Chop },
		{ "doubleslap", Moves::DoubleSlap },
		{ "comet punch", Moves::Comet_Punch },
		{ "cometpunch", Moves::Comet_Punch },
		{ "mega punch", Moves::Mega_Punch },
		{ "megapunch", Moves::Mega_Punch },
		{ "pay day", Moves::Pay_Day },
		{ "payday", Moves::Pay_Day },
		{ "fire punch", Moves::Fire_Punch },
		{ "firepunch", Moves::Fire_Punch },
		{ "ice punch", Moves::Ice_Punch },
		{ "icepunch", Moves::Ice_Punch },
		{ "thunderpunch", Moves::ThunderPunch },
		{ "scratch", Moves::Scratch },
		{ "vicegrip", Moves::ViceGrip },
		{ "guillotine", Moves::Guillotine },
		{ "razor wind", Moves::Razor_Wind },
		{ "razorwind", Moves::Razor_Wind },
		{ "swords dance", Moves::Swords_Dance },
		{ "swordsdance", Moves::Swords_Dance },
		{ "cut", Moves::Cut },
		{ "gust", Moves::Gust },
		{ "wing attack", Moves::Wing_Attack },
		{ "wingattack", Moves::Wing_Attack },
		{ "whirlwind", Moves::Whirlwind },
		{ "fly", Moves::Fly },
		{ "bind", Moves::Bind },
		{ "slam", Moves::Slam },
		{ "vine whip", Moves::Vine_Whip },
		{ "vinewhip", Moves::Vine_Whip },
		{ "stomp", Moves::Stomp },
		{ "double kick", Moves::Double_Kick },
		{ "doublekick", Moves::Double_Kick },
		{ "mega kick", Moves::Mega_Kick },
		{ "megakick", Moves::Mega_Kick },
		{ "jump kick", Moves::Jump_Kick },
		{ "jumpkick", Moves::Jump_Kick },
		{ "rolling kick", Moves::Rolling_Kick },
		{ "rollingkick", Moves::Rolling_Kick },
		{ "sand-attack", Moves::Sand_Attack },
		{ "headbutt", Moves::Headbutt },
		{ "horn attack", Moves::Horn_Attack },
		{ "hornattack", Moves::Horn_Attack },
		{ "fury attack", Moves::Fury_Attack },
		{ "furyattack", Moves::Fury_Attack },
		{ "horn drill", Moves::Horn_Drill },
		{ "horndrill", Moves::Horn_Drill },
		{ "tackle", Moves::Tackle },
		{ "body slam", Moves::Body_Slam },
		{ "bodyslam", Moves::Body_Slam },
		{ "wrap", Moves::Wrap },
		{ "take down", Moves::Take_Down },
		{ "takedown", Moves::Take_Down },
		{ "thrash", Moves::Thrash },
		{ "double-edge", Moves::Double_Edge },
		{ "doubleedge", Moves::Double_Edge },
		{ "tail whip", Moves::Tail_Whip },
		{ "tailwhip", Moves::Tail_Whip },
		{ "poison sting", Moves::Poison_Sting },
		{ "poisonsting", Moves::Poison_Sting },
		{ "twineedle", Moves::Twineedle },
		{ "pin missile", Moves::Pin_Missile },
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
		{ "water gun", Moves::Water_Gun },
		{ "watergun", Moves::Water_Gun },
		{ "hydro pump", Moves::Hydro_Pump },
		{ "hydropump", Moves::Hydro_Pump },
		{ "surf", Moves::Surf },
		{ "ice beam", Moves::Ice_Beam },
		{ "icebeam", Moves::Ice_Beam },
		{ "blizzard", Moves::Blizzard },
		{ "psybeam", Moves::Psybeam },
		{ "bubblebeam", Moves::BubbleBeam },
		{ "aurora beam", Moves::Aurora_Beam },
		{ "aurorabeam", Moves::Aurora_Beam },
		{ "hyper beam", Moves::Hyper_Beam },
		{ "hyperbeam", Moves::Hyper_Beam },
		{ "peck", Moves::Peck },
		{ "drill peck", Moves::Drill_Peck },
		{ "drillpeck", Moves::Drill_Peck },
		{ "submission", Moves::Submission },
		{ "low kick", Moves::Low_Kick },
		{ "lowkick", Moves::Low_Kick },
		{ "counter", Moves::Counter },
		{ "seismic toss", Moves::Seismic_Toss },
		{ "seismictoss", Moves::Seismic_Toss },
		{ "strength", Moves::Strength },
		{ "absorb", Moves::Absorb },
		{ "mega drain", Moves::Mega_Drain },
		{ "megadrain", Moves::Mega_Drain },
		{ "leech seed", Moves::Leech_Seed },
		{ "leechseed", Moves::Leech_Seed },
		{ "growth", Moves::Growth },
		{ "razor leaf", Moves::Razor_Leaf },
		{ "razorleaf", Moves::Razor_Leaf },
		{ "solar beam", Moves::SolarBeam },
		{ "solarbeam", Moves::SolarBeam },
		{ "poisonpowder", Moves::PoisonPowder },
		{ "stun spore", Moves::Stun_Spore },
		{ "stunspore", Moves::Stun_Spore },
		{ "sleep powder", Moves::Sleep_Powder },
		{ "sleeppowder", Moves::Sleep_Powder },
		{ "petal dance", Moves::Petal_Dance },
		{ "petaldance", Moves::Petal_Dance },
		{ "string shot", Moves::String_Shot },
		{ "stringshot", Moves::String_Shot },
		{ "dragon rage", Moves::Dragon_Rage },
		{ "dragonrage", Moves::Dragon_Rage },
		{ "fire spin", Moves::Fire_Spin },
		{ "firespin", Moves::Fire_Spin },
		{ "thundershock", Moves::ThunderShock },
		{ "thunderbolt", Moves::Thunderbolt },
		{ "thunder wave", Moves::Thunder_Wave },
		{ "thunderwave", Moves::Thunder_Wave },
		{ "thunder", Moves::Thunder },
		{ "rock throw", Moves::Rock_Throw },
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
		{ "quick attack", Moves::Quick_Attack },
		{ "quickattack", Moves::Quick_Attack },
		{ "rage", Moves::Rage },
		{ "teleport", Moves::Teleport },
		{ "night shade", Moves::Night_Shade },
		{ "nightshade", Moves::Night_Shade },
		{ "mimic", Moves::Mimic },
		{ "screech", Moves::Screech },
		{ "double team", Moves::Double_Team },
		{ "doubleteam", Moves::Double_Team },
		{ "recover", Moves::Recover },
		{ "harden", Moves::Harden },
		{ "minimize", Moves::Minimize },
		{ "smokescreen", Moves::SmokeScreen },
		{ "confuse ray", Moves::Confuse_Ray },
		{ "confuseray", Moves::Confuse_Ray },
		{ "withdraw", Moves::Withdraw },
		{ "defense curl", Moves::Defense_Curl },
		{ "defensecurl", Moves::Defense_Curl },
		{ "barrier", Moves::Barrier },
		{ "light screen", Moves::Light_Screen },
		{ "lightscreen", Moves::Light_Screen },
		{ "haze", Moves::Haze },
		{ "reflect", Moves::Reflect },
		{ "focus energy", Moves::Focus_Energy },
		{ "focusenergy", Moves::Focus_Energy },
		{ "bide", Moves::Bide },
		{ "metronome", Moves::Metronome },
		{ "mirror move", Moves::Mirror_Move },
		{ "mirrormove", Moves::Mirror_Move },
		{ "selfdestruct", Moves::Selfdestruct },
		{ "egg bomb", Moves::Egg_Bomb },
		{ "eggbomb", Moves::Egg_Bomb },
		{ "lick", Moves::Lick },
		{ "smog", Moves::Smog },
		{ "sludge", Moves::Sludge },
		{ "bone club", Moves::Bone_Club },
		{ "boneclub", Moves::Bone_Club },
		{ "fire blast", Moves::Fire_Blast },
		{ "fireblast", Moves::Fire_Blast },
		{ "waterfall", Moves::Waterfall },
		{ "clamp", Moves::Clamp },
		{ "swift", Moves::Swift },
		{ "skull bash", Moves::Skull_Bash },
		{ "skullbash", Moves::Skull_Bash },
		{ "spike cannon", Moves::Spike_Cannon },
		{ "spikecannon", Moves::Spike_Cannon },
		{ "constrict", Moves::Constrict },
		{ "amnesia", Moves::Amnesia },
		{ "kinesis", Moves::Kinesis },
		{ "softboiled", Moves::Softboiled },
		{ "hi jump kick", Moves::Hi_Jump_Kick },
		{ "highjumpkick", Moves::Hi_Jump_Kick },
		{ "glare", Moves::Glare },
		{ "dream eater", Moves::Dream_Eater },
		{ "dreameater", Moves::Dream_Eater },
		{ "poison gas", Moves::Poison_Gas },
		{ "poisongas", Moves::Poison_Gas },
		{ "barrage", Moves::Barrage },
		{ "leech life", Moves::Leech_Life },
		{ "leechlife", Moves::Leech_Life },
		{ "lovely kiss", Moves::Lovely_Kiss },
		{ "lovelykiss", Moves::Lovely_Kiss },
		{ "sky attack", Moves::Sky_Attack },
		{ "skyattack", Moves::Sky_Attack },
		{ "transform", Moves::Transform },
		{ "bubble", Moves::Bubble },
		{ "dizzy punch", Moves::Dizzy_Punch },
		{ "dizzypunch", Moves::Dizzy_Punch },
		{ "spore", Moves::Spore },
		{ "flash", Moves::Flash },
		{ "psywave", Moves::Psywave },
		{ "splash", Moves::Splash },
		{ "acid armor", Moves::Acid_Armor },
		{ "acidarmor", Moves::Acid_Armor },
		{ "crabhammer", Moves::Crabhammer },
		{ "explosion", Moves::Explosion },
		{ "fury swipes", Moves::Fury_Swipes },
		{ "furyswipes", Moves::Fury_Swipes },
		{ "bonemerang", Moves::Bonemerang },
		{ "rest", Moves::Rest },
		{ "rock slide", Moves::Rock_Slide },
		{ "rockslide", Moves::Rock_Slide },
		{ "hyper fang", Moves::Hyper_Fang },
		{ "hyperfang", Moves::Hyper_Fang },
		{ "sharpen", Moves::Sharpen },
		{ "conversion", Moves::Conversion },
		{ "tri attack", Moves::Tri_Attack },
		{ "triattack", Moves::Tri_Attack },
		{ "super fang", Moves::Super_Fang },
		{ "superfang", Moves::Super_Fang },
		{ "slash", Moves::Slash },
		{ "substitute", Moves::Substitute },
		{ "struggle", Moves::Struggle },
		{ "sketch", Moves::Sketch },
		{ "triple kick", Moves::Triple_Kick },
		{ "triplekick", Moves::Triple_Kick },
		{ "thief", Moves::Thief },
		{ "spider web", Moves::Spider_Web },
		{ "spiderweb", Moves::Spider_Web },
		{ "mind reader", Moves::Mind_Reader },
		{ "mindreader", Moves::Mind_Reader },
		{ "nightmare", Moves::Nightmare },
		{ "flame wheel", Moves::Flame_Wheel },
		{ "flamewheel", Moves::Flame_Wheel },
		{ "snore", Moves::Snore },
		{ "curse", Moves::Curse },
		{ "flail", Moves::Flail },
		{ "conversion 2", Moves::Conversion_2 },
		{ "conversion2", Moves::Conversion_2 },
		{ "aeroblast", Moves::Aeroblast },
		{ "cotton spore", Moves::Cotton_Spore },
		{ "cottonspore", Moves::Cotton_Spore },
		{ "reversal", Moves::Reversal },
		{ "spite", Moves::Spite },
		{ "powder snow", Moves::Powder_Snow },
		{ "powdersnow", Moves::Powder_Snow },
		{ "protect", Moves::Protect },
		{ "mach punch", Moves::Mach_Punch },
		{ "machpunch", Moves::Mach_Punch },
		{ "scary face", Moves::Scary_Face },
		{ "scaryface", Moves::Scary_Face },
		{ "faint attack", Moves::Faint_Attack },
		{ "faintattack", Moves::Faint_Attack },
		{ "sweet kiss", Moves::Sweet_Kiss },
		{ "sweetkiss", Moves::Sweet_Kiss },
		{ "belly drum", Moves::Belly_Drum },
		{ "bellydrum", Moves::Belly_Drum },
		{ "sludge bomb", Moves::Sludge_Bomb },
		{ "sludgebomb", Moves::Sludge_Bomb },
		{ "mud-slap", Moves::Mud_Slap },
		{ "mudslap", Moves::Mud_Slap },
		{ "octazooka", Moves::Octazooka },
		{ "spikes", Moves::Spikes },
		{ "zap cannon", Moves::Zap_Cannon },
		{ "zapcannon", Moves::Zap_Cannon },
		{ "foresight", Moves::Foresight },
		{ "destiny bond", Moves::Destiny_Bond },
		{ "destinybond", Moves::Destiny_Bond },
		{ "perish song", Moves::Perish_Song },
		{ "perishsong", Moves::Perish_Song },
		{ "icy wind", Moves::Icy_Wind },
		{ "icywind", Moves::Icy_Wind },
		{ "detect", Moves::Detect },
		{ "bone rush", Moves::Bone_Rush },
		{ "bonerush", Moves::Bone_Rush },
		{ "lock-on", Moves::Lock_On },
		{ "lockon", Moves::Lock_On },
		{ "outrage", Moves::Outrage },
		{ "sandstorm", Moves::Sandstorm },
		{ "giga drain", Moves::Giga_Drain },
		{ "gigadrain", Moves::Giga_Drain },
		{ "endure", Moves::Endure },
		{ "charm", Moves::Charm },
		{ "rollout", Moves::Rollout },
		{ "false swipe", Moves::False_Swipe },
		{ "falseswipe", Moves::False_Swipe },
		{ "swagger", Moves::Swagger },
		{ "milk drink", Moves::Milk_Drink },
		{ "milkdrink", Moves::Milk_Drink },
		{ "spark", Moves::Spark },
		{ "fury cutter", Moves::Fury_Cutter },
		{ "furycutter", Moves::Fury_Cutter },
		{ "steel wing", Moves::Steel_Wing },
		{ "steelwing", Moves::Steel_Wing },
		{ "mean look", Moves::Mean_Look },
		{ "meanlook", Moves::Mean_Look },
		{ "attract", Moves::Attract },
		{ "sleep talk", Moves::Sleep_Talk },
		{ "sleeptalk", Moves::Sleep_Talk },
		{ "heal bell", Moves::Heal_Bell },
		{ "healbell", Moves::Heal_Bell },
		{ "return", Moves::Return },
		{ "present", Moves::Present },
		{ "frustration", Moves::Frustration },
		{ "safeguard", Moves::Safeguard },
		{ "pain split", Moves::Pain_Split },
		{ "painsplit", Moves::Pain_Split },
		{ "sacred fire", Moves::Sacred_Fire },
		{ "sacredfire", Moves::Sacred_Fire },
		{ "magnitude", Moves::Magnitude },
		{ "dynamicpunch", Moves::DynamicPunch },
		{ "megahorn", Moves::Megahorn },
		{ "dragonbreath", Moves::DragonBreath },
		{ "baton pass", Moves::Baton_Pass },
		{ "batonpass", Moves::Baton_Pass },
		{ "encore", Moves::Encore },
		{ "pursuit", Moves::Pursuit },
		{ "rapid spin", Moves::Rapid_Spin },
		{ "rapidspin", Moves::Rapid_Spin },
		{ "sweet scent", Moves::Sweet_Scent },
		{ "sweetscent", Moves::Sweet_Scent },
		{ "iron tail", Moves::Iron_Tail },
		{ "irontail", Moves::Iron_Tail },
		{ "metal claw", Moves::Metal_Claw },
		{ "metalclaw", Moves::Metal_Claw },
		{ "vital throw", Moves::Vital_Throw },
		{ "vitalthrow", Moves::Vital_Throw },
		{ "morning sun", Moves::Morning_Sun },
		{ "morningsun", Moves::Morning_Sun },
		{ "synthesis", Moves::Synthesis },
		{ "moonlight", Moves::Moonlight },
		{ "hidden power", Moves::Hidden_Power },
		{ "hiddenpower", Moves::Hidden_Power },
		{ "hidden power bug", Moves::Hidden_Power },
		{ "hiddenpowerbug", Moves::Hidden_Power },
		{ "hidden power dark", Moves::Hidden_Power },
		{ "hiddenpowerdark", Moves::Hidden_Power },
		{ "hidden power dragon", Moves::Hidden_Power },
		{ "hiddenpowerdragon", Moves::Hidden_Power },
		{ "hidden power electric", Moves::Hidden_Power },
		{ "hiddenpowerelectric", Moves::Hidden_Power },
		{ "hidden power fighting", Moves::Hidden_Power },
		{ "hiddenpowerfighting", Moves::Hidden_Power },
		{ "hidden power fire", Moves::Hidden_Power },
		{ "hiddenpowerfire", Moves::Hidden_Power },
		{ "hidden power flying", Moves::Hidden_Power },
		{ "hiddenpowerflying", Moves::Hidden_Power },
		{ "hidden power ghost", Moves::Hidden_Power },
		{ "hiddenpowerghost", Moves::Hidden_Power },
		{ "hidden power grass", Moves::Hidden_Power },
		{ "hiddenpowergrass", Moves::Hidden_Power },
		{ "hidden power ground", Moves::Hidden_Power },
		{ "hiddenpowerground", Moves::Hidden_Power },
		{ "hidden power ice", Moves::Hidden_Power },
		{ "hiddenpowerice", Moves::Hidden_Power },
		{ "hidden power poison", Moves::Hidden_Power },
		{ "hiddenpowerpoison", Moves::Hidden_Power },
		{ "hidden power psychic", Moves::Hidden_Power },
		{ "hiddenpowerpsychic", Moves::Hidden_Power },
		{ "hidden power rock", Moves::Hidden_Power },
		{ "hiddenpowerrock", Moves::Hidden_Power },
		{ "hidden power steel", Moves::Hidden_Power },
		{ "hiddenpowersteel", Moves::Hidden_Power },
		{ "hidden power water", Moves::Hidden_Power },
		{ "hiddenpowerwater", Moves::Hidden_Power },
		{ "cross chop", Moves::Cross_Chop },
		{ "crosschop", Moves::Cross_Chop },
		{ "twister", Moves::Twister },
		{ "rain dance", Moves::Rain_Dance },
		{ "raindance", Moves::Rain_Dance },
		{ "sunny day", Moves::Sunny_Day },
		{ "sunnyday", Moves::Sunny_Day },
		{ "crunch", Moves::Crunch },
		{ "mirror coat", Moves::Mirror_Coat },
		{ "mirrorcoat", Moves::Mirror_Coat },
		{ "psych up", Moves::Psych_Up },
		{ "psychup", Moves::Psych_Up },
		{ "extremespeed", Moves::ExtremeSpeed },
		{ "ancientpower", Moves::AncientPower },
		{ "shadow ball", Moves::Shadow_Ball },
		{ "shadowball", Moves::Shadow_Ball },
		{ "future sight", Moves::Future_Sight },
		{ "futuresight", Moves::Future_Sight },
		{ "rock smash", Moves::Rock_Smash },
		{ "rocksmash", Moves::Rock_Smash },
		{ "whirlpool", Moves::Whirlpool },
		{ "beat up", Moves::Beat_Up },
		{ "beatup", Moves::Beat_Up },
		{ "fake out", Moves::Fake_Out },
		{ "fakeout", Moves::Fake_Out },
		{ "uproar", Moves::Uproar },
		{ "stockpile", Moves::Stockpile },
		{ "spit up", Moves::Spit_Up },
		{ "spitup", Moves::Spit_Up },
		{ "swallow", Moves::Swallow },
		{ "heat wave", Moves::Heat_Wave },
		{ "heatwave", Moves::Heat_Wave },
		{ "hail", Moves::Hail },
		{ "torment", Moves::Torment },
		{ "flatter", Moves::Flatter },
		{ "will-o-wisp", Moves::Will_O_Wisp },
		{ "willowisp", Moves::Will_O_Wisp },
		{ "memento", Moves::Memento },
		{ "facade", Moves::Facade },
		{ "focus punch", Moves::Focus_Punch },
		{ "focuspunch", Moves::Focus_Punch },
		{ "smellingsalt", Moves::SmellingSalt },
		{ "follow me", Moves::Follow_Me },
		{ "followme", Moves::Follow_Me },
		{ "nature power", Moves::Nature_Power },
		{ "naturepower", Moves::Nature_Power },
		{ "charge", Moves::Charge },
		{ "taunt", Moves::Taunt },
		{ "helping hand", Moves::Helping_Hand },
		{ "helpinghand", Moves::Helping_Hand },
		{ "trick", Moves::Trick },
		{ "role play", Moves::Role_Play },
		{ "roleplay", Moves::Role_Play },
		{ "wish", Moves::Wish },
		{ "assist", Moves::Assist },
		{ "ingrain", Moves::Ingrain },
		{ "superpower", Moves::Superpower },
		{ "magic coat", Moves::Magic_Coat },
		{ "magiccoat", Moves::Magic_Coat },
		{ "recycle", Moves::Recycle },
		{ "revenge", Moves::Revenge },
		{ "brick break", Moves::Brick_Break },
		{ "brickbreak", Moves::Brick_Break },
		{ "yawn", Moves::Yawn },
		{ "knock off", Moves::Knock_Off },
		{ "knockoff", Moves::Knock_Off },
		{ "endeavor", Moves::Endeavor },
		{ "eruption", Moves::Eruption },
		{ "skill swap", Moves::Skill_Swap },
		{ "skillswap", Moves::Skill_Swap },
		{ "imprison", Moves::Imprison },
		{ "refresh", Moves::Refresh },
		{ "grudge", Moves::Grudge },
		{ "snatch", Moves::Snatch },
		{ "secret power", Moves::Secret_Power },
		{ "secretpower", Moves::Secret_Power },
		{ "dive", Moves::Dive },
		{ "arm thrust", Moves::Arm_Thrust },
		{ "armthrust", Moves::Arm_Thrust },
		{ "camouflage", Moves::Camouflage },
		{ "tail glow", Moves::Tail_Glow },
		{ "tailglow", Moves::Tail_Glow },
		{ "luster purge", Moves::Luster_Purge },
		{ "lusterpurge", Moves::Luster_Purge },
		{ "mist ball", Moves::Mist_Ball },
		{ "mistball", Moves::Mist_Ball },
		{ "featherdance", Moves::FeatherDance },
		{ "teeter dance", Moves::Teeter_Dance },
		{ "teeterdance", Moves::Teeter_Dance },
		{ "blaze kick", Moves::Blaze_Kick },
		{ "blazekick", Moves::Blaze_Kick },
		{ "mud sport", Moves::Mud_Sport },
		{ "mudsport", Moves::Mud_Sport },
		{ "ice ball", Moves::Ice_Ball },
		{ "iceball", Moves::Ice_Ball },
		{ "needle arm", Moves::Needle_Arm },
		{ "needlearm", Moves::Needle_Arm },
		{ "slack off", Moves::Slack_Off },
		{ "slackoff", Moves::Slack_Off },
		{ "hyper voice", Moves::Hyper_Voice },
		{ "hypervoice", Moves::Hyper_Voice },
		{ "poison fang", Moves::Poison_Fang },
		{ "poisonfang", Moves::Poison_Fang },
		{ "crush claw", Moves::Crush_Claw },
		{ "crushclaw", Moves::Crush_Claw },
		{ "blast burn", Moves::Blast_Burn },
		{ "blastburn", Moves::Blast_Burn },
		{ "hydro cannon", Moves::Hydro_Cannon },
		{ "hydrocannon", Moves::Hydro_Cannon },
		{ "meteor mash", Moves::Meteor_Mash },
		{ "meteormash", Moves::Meteor_Mash },
		{ "astonish", Moves::Astonish },
		{ "weather ball", Moves::Weather_Ball },
		{ "weatherball", Moves::Weather_Ball },
		{ "aromatherapy", Moves::Aromatherapy },
		{ "fake tears", Moves::Fake_Tears },
		{ "faketears", Moves::Fake_Tears },
		{ "air cutter", Moves::Air_Cutter },
		{ "aircutter", Moves::Air_Cutter },
		{ "overheat", Moves::Overheat },
		{ "odor sleuth", Moves::Odor_Sleuth },
		{ "odorsleuth", Moves::Odor_Sleuth },
		{ "rock tomb", Moves::Rock_Tomb },
		{ "rocktomb", Moves::Rock_Tomb },
		{ "silver wind", Moves::Silver_Wind },
		{ "silverwind", Moves::Silver_Wind },
		{ "metal sound", Moves::Metal_Sound },
		{ "metalsound", Moves::Metal_Sound },
		{ "grasswhistle", Moves::GrassWhistle },
		{ "tickle", Moves::Tickle },
		{ "cosmic power", Moves::Cosmic_Power },
		{ "cosmicpower", Moves::Cosmic_Power },
		{ "water spout", Moves::Water_Spout },
		{ "waterspout", Moves::Water_Spout },
		{ "signal beam", Moves::Signal_Beam },
		{ "signalbeam", Moves::Signal_Beam },
		{ "shadow punch", Moves::Shadow_Punch },
		{ "shadowpunch", Moves::Shadow_Punch },
		{ "extrasensory", Moves::Extrasensory },
		{ "sky uppercut", Moves::Sky_Uppercut },
		{ "skyuppercut", Moves::Sky_Uppercut },
		{ "sand tomb", Moves::Sand_Tomb },
		{ "sandtomb", Moves::Sand_Tomb },
		{ "sheer cold", Moves::Sheer_Cold },
		{ "sheercold", Moves::Sheer_Cold },
		{ "muddy water", Moves::Muddy_Water },
		{ "muddywater", Moves::Muddy_Water },
		{ "bullet seed", Moves::Bullet_Seed },
		{ "bulletseed", Moves::Bullet_Seed },
		{ "aerial ace", Moves::Aerial_Ace },
		{ "aerialace", Moves::Aerial_Ace },
		{ "icicle spear", Moves::Icicle_Spear },
		{ "iciclespear", Moves::Icicle_Spear },
		{ "iron defense", Moves::Iron_Defense },
		{ "irondefense", Moves::Iron_Defense },
		{ "block", Moves::Block },
		{ "howl", Moves::Howl },
		{ "dragon claw", Moves::Dragon_Claw },
		{ "dragonclaw", Moves::Dragon_Claw },
		{ "frenzy plant", Moves::Frenzy_Plant },
		{ "frenzyplant", Moves::Frenzy_Plant },
		{ "bulk up", Moves::Bulk_Up },
		{ "bulkup", Moves::Bulk_Up },
		{ "bounce", Moves::Bounce },
		{ "mud shot", Moves::Mud_Shot },
		{ "mudshot", Moves::Mud_Shot },
		{ "poison tail", Moves::Poison_Tail },
		{ "poisontail", Moves::Poison_Tail },
		{ "covet", Moves::Covet },
		{ "volt tackle", Moves::Volt_Tackle },
		{ "volttackle", Moves::Volt_Tackle },
		{ "magical leaf", Moves::Magical_Leaf },
		{ "magicalleaf", Moves::Magical_Leaf },
		{ "water sport", Moves::Water_Sport },
		{ "watersport", Moves::Water_Sport },
		{ "calm mind", Moves::Calm_Mind },
		{ "calmmind", Moves::Calm_Mind },
		{ "leaf blade", Moves::Leaf_Blade },
		{ "leafblade", Moves::Leaf_Blade },
		{ "dragon dance", Moves::Dragon_Dance },
		{ "dragondance", Moves::Dragon_Dance },
		{ "rock blast", Moves::Rock_Blast },
		{ "rockblast", Moves::Rock_Blast },
		{ "shock wave", Moves::Shock_Wave },
		{ "shockwave", Moves::Shock_Wave },
		{ "water pulse", Moves::Water_Pulse },
		{ "waterpulse", Moves::Water_Pulse },
		{ "doom desire", Moves::Doom_Desire },
		{ "doomdesire", Moves::Doom_Desire },
		{ "psycho boost", Moves::Psycho_Boost },
		{ "psychoboost", Moves::Psycho_Boost },
		{ "roost", Moves::Roost },
		{ "gravity", Moves::Gravity },
		{ "miracle eye", Moves::Miracle_Eye },
		{ "miracleeye", Moves::Miracle_Eye },
		{ "wake-up slap", Moves::Wake_Up_Slap },
		{ "wakeupslap", Moves::Wake_Up_Slap },
		{ "hammer arm", Moves::Hammer_Arm },
		{ "hammerarm", Moves::Hammer_Arm },
		{ "gyro ball", Moves::Gyro_Ball },
		{ "gyroball", Moves::Gyro_Ball },
		{ "healing wish", Moves::Healing_Wish },
		{ "healingwish", Moves::Healing_Wish },
		{ "brine", Moves::Brine },
		{ "natural gift", Moves::Natural_Gift },
		{ "naturalgift", Moves::Natural_Gift },
		{ "feint", Moves::Feint },
		{ "pluck", Moves::Pluck },
		{ "tailwind", Moves::Tailwind },
		{ "acupressure", Moves::Acupressure },
		{ "metal burst", Moves::Metal_Burst },
		{ "metalburst", Moves::Metal_Burst },
		{ "u-turn", Moves::U_turn },
		{ "uturn", Moves::U_turn },
		{ "close combat", Moves::Close_Combat },
		{ "closecombat", Moves::Close_Combat },
		{ "payback", Moves::Payback },
		{ "assurance", Moves::Assurance },
		{ "embargo", Moves::Embargo },
		{ "fling", Moves::Fling },
		{ "psycho shift", Moves::Psycho_Shift },
		{ "psychoshift", Moves::Psycho_Shift },
		{ "trump card", Moves::Trump_Card },
		{ "trumpcard", Moves::Trump_Card },
		{ "heal block", Moves::Heal_Block },
		{ "healblock", Moves::Heal_Block },
		{ "wring out", Moves::Wring_Out },
		{ "wringout", Moves::Wring_Out },
		{ "power trick", Moves::Power_Trick },
		{ "powertrick", Moves::Power_Trick },
		{ "gastro acid", Moves::Gastro_Acid },
		{ "gastroacid", Moves::Gastro_Acid },
		{ "lucky chant", Moves::Lucky_Chant },
		{ "luckychant", Moves::Lucky_Chant },
		{ "me first", Moves::Me_First },
		{ "mefirst", Moves::Me_First },
		{ "copycat", Moves::Copycat },
		{ "power swap", Moves::Power_Swap },
		{ "powerswap", Moves::Power_Swap },
		{ "guard swap", Moves::Guard_Swap },
		{ "guardswap", Moves::Guard_Swap },
		{ "punishment", Moves::Punishment },
		{ "last resort", Moves::Last_Resort },
		{ "lastresort", Moves::Last_Resort },
		{ "worry seed", Moves::Worry_Seed },
		{ "worryseed", Moves::Worry_Seed },
		{ "sucker punch", Moves::Sucker_Punch },
		{ "suckerpunch", Moves::Sucker_Punch },
		{ "toxic spikes", Moves::Toxic_Spikes },
		{ "toxicspikes", Moves::Toxic_Spikes },
		{ "heart swap", Moves::Heart_Swap },
		{ "heartswap", Moves::Heart_Swap },
		{ "aqua ring", Moves::Aqua_Ring },
		{ "aquaring", Moves::Aqua_Ring },
		{ "magnet rise", Moves::Magnet_Rise },
		{ "magnetrise", Moves::Magnet_Rise },
		{ "flare blitz", Moves::Flare_Blitz },
		{ "flareblitz", Moves::Flare_Blitz },
		{ "force palm", Moves::Force_Palm },
		{ "forcepalm", Moves::Force_Palm },
		{ "aura sphere", Moves::Aura_Sphere },
		{ "aurasphere", Moves::Aura_Sphere },
		{ "rock polish", Moves::Rock_Polish },
		{ "rockpolish", Moves::Rock_Polish },
		{ "poison jab", Moves::Poison_Jab },
		{ "poisonjab", Moves::Poison_Jab },
		{ "dark pulse", Moves::Dark_Pulse },
		{ "darkpulse", Moves::Dark_Pulse },
		{ "night slash", Moves::Night_Slash },
		{ "nightslash", Moves::Night_Slash },
		{ "aqua tail", Moves::Aqua_Tail },
		{ "aquatail", Moves::Aqua_Tail },
		{ "seed bomb", Moves::Seed_Bomb },
		{ "seedbomb", Moves::Seed_Bomb },
		{ "air slash", Moves::Air_Slash },
		{ "airslash", Moves::Air_Slash },
		{ "x-scissor", Moves::X_Scissor },
		{ "xscissor", Moves::X_Scissor },
		{ "bug buzz", Moves::Bug_Buzz },
		{ "bugbuzz", Moves::Bug_Buzz },
		{ "dragon pulse", Moves::Dragon_Pulse },
		{ "dragonpulse", Moves::Dragon_Pulse },
		{ "dragon rush", Moves::Dragon_Rush },
		{ "dragonrush", Moves::Dragon_Rush },
		{ "power gem", Moves::Power_Gem },
		{ "powergem", Moves::Power_Gem },
		{ "drain punch", Moves::Drain_Punch },
		{ "drainpunch", Moves::Drain_Punch },
		{ "vacuum wave", Moves::Vacuum_Wave },
		{ "vacuumwave", Moves::Vacuum_Wave },
		{ "focus blast", Moves::Focus_Blast },
		{ "focusblast", Moves::Focus_Blast },
		{ "energy ball", Moves::Energy_Ball },
		{ "energyball", Moves::Energy_Ball },
		{ "brave bird", Moves::Brave_Bird },
		{ "bravebird", Moves::Brave_Bird },
		{ "earth power", Moves::Earth_Power },
		{ "earthpower", Moves::Earth_Power },
		{ "switcheroo", Moves::Switcheroo },
		{ "giga impact", Moves::Giga_Impact },
		{ "gigaimpact", Moves::Giga_Impact },
		{ "nasty plot", Moves::Nasty_Plot },
		{ "nastyplot", Moves::Nasty_Plot },
		{ "bullet punch", Moves::Bullet_Punch },
		{ "bulletpunch", Moves::Bullet_Punch },
		{ "avalanche", Moves::Avalanche },
		{ "ice shard", Moves::Ice_Shard },
		{ "iceshard", Moves::Ice_Shard },
		{ "shadow claw", Moves::Shadow_Claw },
		{ "shadowclaw", Moves::Shadow_Claw },
		{ "thunder fang", Moves::Thunder_Fang },
		{ "thunderfang", Moves::Thunder_Fang },
		{ "ice fang", Moves::Ice_Fang },
		{ "icefang", Moves::Ice_Fang },
		{ "fire fang", Moves::Fire_Fang },
		{ "firefang", Moves::Fire_Fang },
		{ "shadow sneak", Moves::Shadow_Sneak },
		{ "shadowsneak", Moves::Shadow_Sneak },
		{ "mud bomb", Moves::Mud_Bomb },
		{ "mudbomb", Moves::Mud_Bomb },
		{ "psycho cut", Moves::Psycho_Cut },
		{ "psychocut", Moves::Psycho_Cut },
		{ "zen headbutt", Moves::Zen_Headbutt },
		{ "zenheadbutt", Moves::Zen_Headbutt },
		{ "mirror shot", Moves::Mirror_Shot },
		{ "mirrorshot", Moves::Mirror_Shot },
		{ "flash cannon", Moves::Flash_Cannon },
		{ "flashcannon", Moves::Flash_Cannon },
		{ "rock climb", Moves::Rock_Climb },
		{ "rockclimb", Moves::Rock_Climb },
		{ "defog", Moves::Defog },
		{ "trick room", Moves::Trick_Room },
		{ "trickroom", Moves::Trick_Room },
		{ "draco meteor", Moves::Draco_Meteor },
		{ "dracometeor", Moves::Draco_Meteor },
		{ "discharge", Moves::Discharge },
		{ "lava plume", Moves::Lava_Plume },
		{ "lavaplume", Moves::Lava_Plume },
		{ "leaf storm", Moves::Leaf_Storm },
		{ "leafstorm", Moves::Leaf_Storm },
		{ "power whip", Moves::Power_Whip },
		{ "powerwhip", Moves::Power_Whip },
		{ "rock wrecker", Moves::Rock_Wrecker },
		{ "rockwrecker", Moves::Rock_Wrecker },
		{ "cross poison", Moves::Cross_Poison },
		{ "crosspoison", Moves::Cross_Poison },
		{ "gunk shot", Moves::Gunk_Shot },
		{ "gunkshot", Moves::Gunk_Shot },
		{ "iron head", Moves::Iron_Head },
		{ "ironhead", Moves::Iron_Head },
		{ "magnet bomb", Moves::Magnet_Bomb },
		{ "magnetbomb", Moves::Magnet_Bomb },
		{ "stone edge", Moves::Stone_Edge },
		{ "stoneedge", Moves::Stone_Edge },
		{ "captivate", Moves::Captivate },
		{ "stealth rock", Moves::Stealth_Rock },
		{ "stealthrock", Moves::Stealth_Rock },
		{ "grass knot", Moves::Grass_Knot },
		{ "grassknot", Moves::Grass_Knot },
		{ "chatter", Moves::Chatter },
		{ "judgment", Moves::Judgment },
		{ "bug bite", Moves::Bug_Bite },
		{ "bugbite", Moves::Bug_Bite },
		{ "charge beam", Moves::Charge_Beam },
		{ "chargebeam", Moves::Charge_Beam },
		{ "wood hammer", Moves::Wood_Hammer },
		{ "woodhammer", Moves::Wood_Hammer },
		{ "aqua jet", Moves::Aqua_Jet },
		{ "aquajet", Moves::Aqua_Jet },
		{ "attack order", Moves::Attack_Order },
		{ "attackorder", Moves::Attack_Order },
		{ "defend order", Moves::Defend_Order },
		{ "defendorder", Moves::Defend_Order },
		{ "heal order", Moves::Heal_Order },
		{ "healorder", Moves::Heal_Order },
		{ "head smash", Moves::Head_Smash },
		{ "headsmash", Moves::Head_Smash },
		{ "double hit", Moves::Double_Hit },
		{ "doublehit", Moves::Double_Hit },
		{ "roar of time", Moves::Roar_of_Time },
		{ "roaroftime", Moves::Roar_of_Time },
		{ "roaroftime", Moves::Roar_of_Time },
		{ "spacial rend", Moves::Spacial_Rend },
		{ "spacialrend", Moves::Spacial_Rend },
		{ "lunar dance", Moves::Lunar_Dance },
		{ "lunardance", Moves::Lunar_Dance },
		{ "crush grip", Moves::Crush_Grip },
		{ "crushgrip", Moves::Crush_Grip },
		{ "magma storm", Moves::Magma_Storm },
		{ "magmastorm", Moves::Magma_Storm },
		{ "dark void", Moves::Dark_Void },
		{ "darkvoid", Moves::Dark_Void },
		{ "seed flare", Moves::Seed_Flare },
		{ "seedflare", Moves::Seed_Flare },
		{ "ominous wind", Moves::Ominous_Wind },
		{ "ominouswind", Moves::Ominous_Wind },
		{ "shadow force", Moves::Shadow_Force },
		{ "shadowforce", Moves::Shadow_Force },
		{ "hone claws", Moves::Hone_Claws },
		{ "honeclaws", Moves::Hone_Claws },
		{ "wide guard", Moves::Wide_Guard },
		{ "wideguard", Moves::Wide_Guard },
		{ "guard split", Moves::Guard_Split },
		{ "guardsplit", Moves::Guard_Split },
		{ "power split", Moves::Power_Split },
		{ "powersplit", Moves::Power_Split },
		{ "wonder room", Moves::Wonder_Room },
		{ "wonderroom", Moves::Wonder_Room },
		{ "psyshock", Moves::Psyshock },
		{ "venoshock", Moves::Venoshock },
		{ "autotomize", Moves::Autotomize },
		{ "rage powder", Moves::Rage_Powder },
		{ "ragepowder", Moves::Rage_Powder },
		{ "telekinesis", Moves::Telekinesis },
		{ "magic room", Moves::Magic_Room },
		{ "magicroom", Moves::Magic_Room },
		{ "smack down", Moves::Smack_Down },
		{ "smackdown", Moves::Smack_Down },
		{ "storm throw", Moves::Storm_Throw },
		{ "stormthrow", Moves::Storm_Throw },
		{ "flame burst", Moves::Flame_Burst },
		{ "flameburst", Moves::Flame_Burst },
		{ "sludge wave", Moves::Sludge_Wave },
		{ "sludgewave", Moves::Sludge_Wave },
		{ "quiver dance", Moves::Quiver_Dance },
		{ "quiverdance", Moves::Quiver_Dance },
		{ "heavy slam", Moves::Heavy_Slam },
		{ "heavyslam", Moves::Heavy_Slam },
		{ "synchronoise", Moves::Synchronoise },
		{ "electro ball", Moves::Electro_Ball },
		{ "electroball", Moves::Electro_Ball },
		{ "soak", Moves::Soak },
		{ "flame charge", Moves::Flame_Charge },
		{ "flamecharge", Moves::Flame_Charge },
		{ "coil", Moves::Coil },
		{ "low sweep", Moves::Low_Sweep },
		{ "lowsweep", Moves::Low_Sweep },
		{ "acid spray", Moves::Acid_Spray },
		{ "acidspray", Moves::Acid_Spray },
		{ "foul play", Moves::Foul_Play },
		{ "foulplay", Moves::Foul_Play },
		{ "simple beam", Moves::Simple_Beam },
		{ "simplebeam", Moves::Simple_Beam },
		{ "entrainment", Moves::Entrainment },
		{ "after you", Moves::After_You },
		{ "afteryou", Moves::After_You },
		{ "round", Moves::Round },
		{ "echoed voice", Moves::Echoed_Voice },
		{ "echoedvoice", Moves::Echoed_Voice },
		{ "chip away", Moves::Chip_Away },
		{ "chipaway", Moves::Chip_Away },
		{ "clear smog", Moves::Clear_Smog },
		{ "clearsmog", Moves::Clear_Smog },
		{ "stored power", Moves::Stored_Power },
		{ "storedpower", Moves::Stored_Power },
		{ "quick guard", Moves::Quick_Guard },
		{ "quickguard", Moves::Quick_Guard },
		{ "ally switch", Moves::Ally_Switch },
		{ "allyswitch", Moves::Ally_Switch },
		{ "scald", Moves::Scald },
		{ "shell smash", Moves::Shell_Smash },
		{ "shellsmash", Moves::Shell_Smash },
		{ "heal pulse", Moves::Heal_Pulse },
		{ "healpulse", Moves::Heal_Pulse },
		{ "hex", Moves::Hex },
		{ "sky drop", Moves::Sky_Drop },
		{ "skydrop", Moves::Sky_Drop },
		{ "shift gear", Moves::Shift_Gear },
		{ "shiftgear", Moves::Shift_Gear },
		{ "circle throw", Moves::Circle_Throw },
		{ "circlethrow", Moves::Circle_Throw },
		{ "incinerate", Moves::Incinerate },
		{ "quash", Moves::Quash },
		{ "acrobatics", Moves::Acrobatics },
		{ "reflect type", Moves::Reflect_Type },
		{ "reflecttype", Moves::Reflect_Type },
		{ "retaliate", Moves::Retaliate },
		{ "final gambit", Moves::Final_Gambit },
		{ "finalgambit", Moves::Final_Gambit },
		{ "bestow", Moves::Bestow },
		{ "inferno", Moves::Inferno },
		{ "water pledge", Moves::Water_Pledge },
		{ "waterpledge", Moves::Water_Pledge },
		{ "fire pledge", Moves::Fire_Pledge },
		{ "firepledge", Moves::Fire_Pledge },
		{ "grass pledge", Moves::Grass_Pledge },
		{ "grasspledge", Moves::Grass_Pledge },
		{ "volt switch", Moves::Volt_Switch },
		{ "voltswitch", Moves::Volt_Switch },
		{ "struggle bug", Moves::Struggle_Bug },
		{ "strugglebug", Moves::Struggle_Bug },
		{ "bulldoze", Moves::Bulldoze },
		{ "frost breath", Moves::Frost_Breath },
		{ "frostbreath", Moves::Frost_Breath },
		{ "dragon tail", Moves::Dragon_Tail },
		{ "dragontail", Moves::Dragon_Tail },
		{ "work up", Moves::Work_Up },
		{ "workup", Moves::Work_Up },
		{ "electroweb", Moves::Electroweb },
		{ "wild charge", Moves::Wild_Charge },
		{ "wildcharge", Moves::Wild_Charge },
		{ "drill run", Moves::Drill_Run },
		{ "drillrun", Moves::Drill_Run },
		{ "dual chop", Moves::Dual_Chop },
		{ "dualchop", Moves::Dual_Chop },
		{ "heart stamp", Moves::Heart_Stamp },
		{ "heartstamp", Moves::Heart_Stamp },
		{ "horn leech", Moves::Horn_Leech },
		{ "hornleech", Moves::Horn_Leech },
		{ "sacred sword", Moves::Sacred_Sword },
		{ "sacredsword", Moves::Sacred_Sword },
		{ "razor shell", Moves::Razor_Shell },
		{ "razorshell", Moves::Razor_Shell },
		{ "heat crash", Moves::Heat_Crash },
		{ "heatcrash", Moves::Heat_Crash },
		{ "leaf tornado", Moves::Leaf_Tornado },
		{ "leaftornado", Moves::Leaf_Tornado },
		{ "steamroller", Moves::Steamroller },
		{ "cotton guard", Moves::Cotton_Guard },
		{ "cottonguard", Moves::Cotton_Guard },
		{ "night daze", Moves::Night_Daze },
		{ "nightdaze", Moves::Night_Daze },
		{ "psystrike", Moves::Psystrike },
		{ "tail slap", Moves::Tail_Slap },
		{ "tailslap", Moves::Tail_Slap },
		{ "hurricane", Moves::Hurricane },
		{ "head charge", Moves::Head_Charge },
		{ "headcharge", Moves::Head_Charge },
		{ "gear grind", Moves::Gear_Grind },
		{ "geargrind", Moves::Gear_Grind },
		{ "searing shot", Moves::Searing_Shot },
		{ "searingshot", Moves::Searing_Shot },
		{ "techno blast", Moves::Techno_Blast },
		{ "technoblast", Moves::Techno_Blast },
		{ "relic song", Moves::Relic_Song },
		{ "relicsong", Moves::Relic_Song },
		{ "secret sword", Moves::Secret_Sword },
		{ "secretsword", Moves::Secret_Sword },
		{ "glaciate", Moves::Glaciate },
		{ "bolt strike", Moves::Bolt_Strike },
		{ "boltstrike", Moves::Bolt_Strike },
		{ "blue flare", Moves::Blue_Flare },
		{ "blueflare", Moves::Blue_Flare },
		{ "fiery dance", Moves::Fiery_Dance },
		{ "fierydance", Moves::Fiery_Dance },
		{ "freeze shock", Moves::Freeze_Shock },
		{ "freezeshock", Moves::Freeze_Shock },
		{ "ice burn", Moves::Ice_Burn },
		{ "iceburn", Moves::Ice_Burn },
		{ "snarl", Moves::Snarl },
		{ "icicle crash", Moves::Icicle_Crash },
		{ "iciclecrash", Moves::Icicle_Crash },
		{ "v-create", Moves::V_create },
		{ "vcreate", Moves::V_create },
		{ "fusion flare", Moves::Fusion_Flare },
		{ "fusionflare", Moves::Fusion_Flare },
		{ "fusion bolt", Moves::Fusion_Bolt }
	};
	auto const it = converter.find(str);
	if (it != end(converter)) {
		return it->second;
	} else {
		throw InvalidFromStringConversion("Move", str);
	}
}

}	// namespace technicalmachine
