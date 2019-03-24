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

#include <tm/string_conversions/move.hpp>

#include <tm/string_conversions/invalid_string_conversion.hpp>

#include <tm/move/moves.hpp>

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

		case Moves::Pound: return "Pound";
		case Moves::Karate_Chop: return "Karate Chop";
		case Moves::Double_Slap: return "Double Slap";
		case Moves::Comet_Punch: return "Comet Punch";
		case Moves::Mega_Punch: return "Mega Punch";
		case Moves::Pay_Day: return "Pay Day";
		case Moves::Fire_Punch: return "Fire Punch";
		case Moves::Ice_Punch: return "Ice Punch";
		case Moves::Thunder_Punch: return "Thunder Punch";
		case Moves::Scratch: return "Scratch";
		case Moves::Vice_Grip: return "Vice Grip";
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
		case Moves::Sand_Attack: return "Sand Attack";
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
		case Moves::Sonic_Boom: return "Sonic Boom";
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
		case Moves::Bubble_Beam: return "Bubble Beam";
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
		case Moves::Solar_Beam: return "Solar Beam";
		case Moves::Poison_Powder: return "Poison Powder";
		case Moves::Stun_Spore: return "Stun Spore";
		case Moves::Sleep_Powder: return "Sleep Powder";
		case Moves::Petal_Dance: return "Petal Dance";
		case Moves::String_Shot: return "String Shot";
		case Moves::Dragon_Rage: return "Dragon Rage";
		case Moves::Fire_Spin: return "Fire Spin";
		case Moves::Thunder_Shock: return "Thunder Shock";
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
		case Moves::Smokescreen: return "Smokescreen";
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
		case Moves::Self_Destruct: return "Self-Destruct";
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
		case Moves::Soft_Boiled: return "Soft-Boiled";
		case Moves::High_Jump_Kick: return "High Jump Kick";
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
		case Moves::Feint_Attack: return "Feint Attack";
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
		case Moves::Dynamic_Punch: return "Dynamic Punch";
		case Moves::Megahorn: return "Megahorn";
		case Moves::Dragon_Breath: return "Dragon Breath";
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
		case Moves::Extreme_Speed: return "Extreme Speed";
		case Moves::Ancient_Power: return "Ancient Power";
		case Moves::Shadow_Ball: return "Shadow Ball";
		case Moves::Future_Sight: return "Future Sight";
		case Moves::Rock_Smash: return "Rock Smash";
		case Moves::Whirlpool: return "Whirlpool";
		case Moves::Beat_Up: return "Beat Up";
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
		case Moves::Smelling_Salts: return "Smelling Salts";
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
		case Moves::Feather_Dance: return "Feather Dance";
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
		case Moves::Grass_Whistle: return "Grass Whistle";
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
		case Moves::Flying_Press: return "Flying Press";
		case Moves::Mat_Block: return "Mat Block";
		case Moves::Belch: return "Belch";
		case Moves::Rototiller: return "Rototiller";
		case Moves::Sticky_Web: return "Sticky Web";
		case Moves::Fell_Stinger: return "Fell Stinger";
		case Moves::Phantom_Force: return "Phantom Force";
		case Moves::Trick_or_Treat: return "Trick-or-Treat";
		case Moves::Noble_Roar: return "Noble Roar";
		case Moves::Ion_Deluge: return "Ion Deluge";
		case Moves::Parabolic_Charge: return "Parabolic Charge";
		case Moves::Forests_Curse: return "Forest's Curse";
		case Moves::Petal_Blizzard: return "Petal Blizzard";
		case Moves::Freeze_Dry: return "Freeze-Dry";
		case Moves::Disarming_Voice: return "Disarming Voice";
		case Moves::Parting_Shot: return "Parting Shot";
		case Moves::Topsy_Turvy: return "Topsy-Turvy";
		case Moves::Draining_Kiss: return "Draining Kiss";
		case Moves::Crafty_Shield: return "Crafty Shield";
		case Moves::Flower_Shield: return "Flower Shield";
		case Moves::Grassy_Terrain: return "Grassy Terrain";
		case Moves::Misty_Terrain: return "Misty Terrain";
		case Moves::Electrify: return "Electrify";
		case Moves::Play_Rough: return "Play Rough";
		case Moves::Fairy_Wind: return "Fairy Wind";
		case Moves::Moonblast: return "Moonblast";
		case Moves::Boomburst: return "Boomburst";
		case Moves::Fairy_Lock: return "Fairy Lock";
		case Moves::Kings_Shield: return "King's Shield";
		case Moves::Play_Nice: return "Play Nice";
		case Moves::Confide: return "Confide";
		case Moves::Diamond_Storm: return "Diamond Storm";
		case Moves::Steam_Eruption: return "Steam Eruption";
		case Moves::Hyperspace_Hole: return "Hyperspace Hole";
		case Moves::Water_Shuriken: return "Water Shuriken";
		case Moves::Mystical_Fire: return "Mystical Fire";
		case Moves::Spiky_Shield: return "Spiky Shield";
		case Moves::Aromatic_Mist: return "Aromatic Mist";
		case Moves::Eerie_Impulse: return "Eerie Impulse";
		case Moves::Venom_Drench: return "Venom Drench";
		case Moves::Powder: return "Powder";
		case Moves::Geomancy: return "Geomancy";
		case Moves::Magnetic_Flux: return "Magnetic Flux";
		case Moves::Happy_Hour: return "Happy Hour";
		case Moves::Electric_Terrain: return "Electric Terrain";
		case Moves::Dazzling_Gleam: return "Dazzling Gleam";
		case Moves::Celebrate: return "Celebrate";
		case Moves::Hold_Hands: return "Hold Hands";
		case Moves::Baby_Doll_Eyes: return "Baby-Doll Eyes";
		case Moves::Nuzzle: return "Nuzzle";
		case Moves::Hold_Back: return "Hold Back";
		case Moves::Infestation: return "Infestation";
		case Moves::Power_Up_Punch: return "Power-Up Punch";
		case Moves::Oblivion_Wing: return "Oblivion Wing";
		case Moves::Thousand_Arrows: return "Thousand Arrows";
		case Moves::Thousand_Waves: return "Thousand Waves";
		case Moves::Lands_Wrath: return "Land's Wrath";
		case Moves::Light_of_Ruin: return "Light of Ruin";
		case Moves::Origin_Pulse: return "Origin Pulse";
		case Moves::Precipice_Blades: return "Precipice Blades";
		case Moves::Dragon_Ascent: return "Dragon Ascent";
		case Moves::Hyperspace_Fury: return "Hyperspace Fury";
		case Moves::Breakneck_Blitz: return "Breakneck Blitz";
		case Moves::All_Out_Pummeling: return "All-Out Pummeling";
		case Moves::Supersonic_Skystrike: return "Supersonic Skystrike";
		case Moves::Acid_Downpour: return "Acid Downpour";
		case Moves::Tectonic_Rage: return "Tectonic Rage";
		case Moves::Continental_Crush: return "Continental Crush";
		case Moves::Savage_Spin_Out: return "Savage Spin-Out";
		case Moves::Never_Ending_Nightmare: return "Never-Ending Nightmare";
		case Moves::Corkscrew_Crash: return "Corkscrew Crash";
		case Moves::Inferno_Overdrive: return "Inferno Overdrive";
		case Moves::Hydro_Vortex: return "Hydro Vortex";
		case Moves::Bloom_Doom: return "Bloom Doom";
		case Moves::Gigavolt_Havoc: return "Gigavolt Havoc";
		case Moves::Shattered_Psyche: return "Shattered Psyche";
		case Moves::Subzero_Slammer: return "Subzero Slammer";
		case Moves::Devastating_Drake: return "Devastating Drake";
		case Moves::Black_Hole_Eclipse: return "Black Hole Eclipse";
		case Moves::Twinkle_Tackle: return "Twinkle Tackle";
		case Moves::Catastropika: return "Catastropika";
		case Moves::Shore_Up: return "Shore Up";
		case Moves::First_Impression: return "First Impression";
		case Moves::Baneful_Bunker: return "Baneful Bunker";
		case Moves::Spirit_Shackle: return "Spirit Shackle";
		case Moves::Darkest_Lariat: return "Darkest Lariat";
		case Moves::Sparkling_Aria: return "Sparkling Aria";
		case Moves::Ice_Hammer: return "Ice Hammer";
		case Moves::Floral_Healing: return "Floral Healing";
		case Moves::High_Horsepower: return "High Horsepower";
		case Moves::Strength_Sap: return "Strength Sap";
		case Moves::Solar_Blade: return "Solar Blade";
		case Moves::Leafage: return "Leafage";
		case Moves::Spotlight: return "Spotlight";
		case Moves::Toxic_Thread: return "Toxic Thread";
		case Moves::Laser_Focus: return "Laser Focus";
		case Moves::Gear_Up: return "Gear Up";
		case Moves::Throat_Chop: return "Throat Chop";
		case Moves::Pollen_Puff: return "Pollen Puff";
		case Moves::Anchor_Shot: return "Anchor Shot";
		case Moves::Psychic_Terrain: return "Psychic Terrain";
		case Moves::Lunge: return "Lunge";
		case Moves::Fire_Lash: return "Fire Lash";
		case Moves::Power_Trip: return "Power Trip";
		case Moves::Burn_Up: return "Burn Up";
		case Moves::Speed_Swap: return "Speed Swap";
		case Moves::Smart_Strike: return "Smart Strike";
		case Moves::Purify: return "Purify";
		case Moves::Revelation_Dance: return "Revelation Dance";
		case Moves::Core_Enforcer: return "Core Enforcer";
		case Moves::Trop_Kick: return "Trop Kick";
		case Moves::Instruct: return "Instruct";
		case Moves::Beak_Blast: return "Beak Blast";
		case Moves::Clanging_Scales: return "Clanging Scales";
		case Moves::Dragon_Hammer: return "Dragon Hammer";
		case Moves::Brutal_Swing: return "Brutal Swing";
		case Moves::Aurora_Veil: return "Aurora Veil";
		case Moves::Sinister_Arrow_Raid: return "Sinister Arrow Raid";
		case Moves::Malicious_Moonsault: return "Malicious Moonsault";
		case Moves::Oceanic_Operetta: return "Oceanic Operetta";
		case Moves::Guardian_of_Alola: return "Guardian of Alola";
		case Moves::Soul_Stealing_7_Star_Strike: return "Soul-Stealing 7-Star Strike";
		case Moves::Stoked_Sparksurfer: return "Stoked Sparksurfer";
		case Moves::Pulverizing_Pancake: return "Pulverizing Pancake";
		case Moves::Extreme_Evoboost: return "Extreme Evoboost";
		case Moves::Genesis_Supernova: return "Genesis Supernova";
		case Moves::Shell_Trap: return "Shell Trap";
		case Moves::Fleur_Cannon: return "Fleur Cannon";
		case Moves::Psychic_Fangs: return "Psychic Fangs";
		case Moves::Stomping_Tantrum: return "Stomping Tantrum";
		case Moves::Shadow_Bone: return "Shadow Bone";
		case Moves::Accelerock: return "Accelerock";
		case Moves::Liquidation: return "Liquidation";
		case Moves::Prismatic_Laser: return "Prismatic Laser";
		case Moves::Spectral_Thief: return "Spectral Thief";
		case Moves::Sunsteel_Strike: return "Sunsteel Strike";
		case Moves::Moongeist_Beam: return "Moongeist Beam";
		case Moves::Tearful_Look: return "Tearful Look";
		case Moves::Zing_Zap: return "Zing Zap";
		case Moves::Natures_Madness: return "Nature's Madness";
		case Moves::Multi_Attack: return "Multi-Attack";
		case Moves::m10000000_Volt_Thunderbolt: return "10,000,000 Volt Thunderbolt";
		case Moves::Mind_Blown: return "Mind Blown";
		case Moves::Plasma_Fists: return "Plasma Fists";
		case Moves::Photon_Geyser: return "Photon Geyser";
		case Moves::Light_That_Burns_the_Sky: return "Light That Burns the Sky";
		case Moves::Searing_Sunraze_Smash: return "Searing Sunraze Smash";
		case Moves::Menacing_Moonraze_Maelstrom: return "Menacing Moonraze Maelstrom";
		case Moves::Lets_Snuggle_Forever: return "Let's Snuggle Forever";
		case Moves::Splintered_Stormshards: return "Splintered Stormshards";
		case Moves::Clangorous_Soulblaze: return "Clangorous Soulblaze";
		case Moves::Zippy_Zap: return "Zippy Zap";
		case Moves::Splishy_Splash: return "Splishy Splash";
		case Moves::Floaty_Fall: return "Floaty Fall";
		case Moves::Pika_Papow: return "Pika Papow";
		case Moves::Bouncy_Bubble: return "Bouncy Bubble";
		case Moves::Buzzy_Buzz: return "Buzzy Buzz";
		case Moves::Sizzly_Slide: return "Sizzly Slide";
		case Moves::Glitzy_Glow: return "Glitzy Glow";
		case Moves::Baddy_Bad: return "Baddy Bad";
		case Moves::Sappy_Seed: return "Sappy Seed";
		case Moves::Freezy_Frost: return "Freezy Frost";
		case Moves::Sparkly_Swirl: return "Sparkly Swirl";
		case Moves::Veevee_Volley: return "Veevee Volley";
		case Moves::Double_Iron_Bash: return "Double Iron Bash";
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
		{ "Pound", Moves::Pound },
		{ "Karate Chop", Moves::Karate_Chop },
		{ "Double Slap", Moves::Double_Slap },
		{ "Comet Punch", Moves::Comet_Punch },
		{ "Mega Punch", Moves::Mega_Punch },
		{ "Pay Day", Moves::Pay_Day },
		{ "Fire Punch", Moves::Fire_Punch },
		{ "Ice Punch", Moves::Ice_Punch },
		{ "Thunder Punch", Moves::Thunder_Punch },
		{ "Scratch", Moves::Scratch },
		{ "Vice Grip", Moves::Vice_Grip },
		{ "Guillotine", Moves::Guillotine },
		{ "Razor Wind", Moves::Razor_Wind },
		{ "Swords Dance", Moves::Swords_Dance },
		{ "Cut", Moves::Cut },
		{ "Gust", Moves::Gust },
		{ "Wing Attack", Moves::Wing_Attack },
		{ "Whirlwind", Moves::Whirlwind },
		{ "Fly", Moves::Fly },
		{ "Bind", Moves::Bind },
		{ "Slam", Moves::Slam },
		{ "Vine Whip", Moves::Vine_Whip },
		{ "Stomp", Moves::Stomp },
		{ "Double Kick", Moves::Double_Kick },
		{ "Mega Kick", Moves::Mega_Kick },
		{ "Jump Kick", Moves::Jump_Kick },
		{ "Rolling Kick", Moves::Rolling_Kick },
		{ "Sand Attack", Moves::Sand_Attack },
		{ "Headbutt", Moves::Headbutt },
		{ "Horn Attack", Moves::Horn_Attack },
		{ "Fury Attack", Moves::Fury_Attack },
		{ "Horn Drill", Moves::Horn_Drill },
		{ "Tackle", Moves::Tackle },
		{ "Body Slam", Moves::Body_Slam },
		{ "Wrap", Moves::Wrap },
		{ "Take Down", Moves::Take_Down },
		{ "Thrash", Moves::Thrash },
		{ "Double-Edge", Moves::Double_Edge },
		{ "Tail Whip", Moves::Tail_Whip },
		{ "Poison Sting", Moves::Poison_Sting },
		{ "Twineedle", Moves::Twineedle },
		{ "Pin Missile", Moves::Pin_Missile },
		{ "Leer", Moves::Leer },
		{ "Bite", Moves::Bite },
		{ "Growl", Moves::Growl },
		{ "Roar", Moves::Roar },
		{ "Sing", Moves::Sing },
		{ "Supersonic", Moves::Supersonic },
		{ "Sonic Boom", Moves::Sonic_Boom },
		{ "Disable", Moves::Disable },
		{ "Acid", Moves::Acid },
		{ "Ember", Moves::Ember },
		{ "Flamethrower", Moves::Flamethrower },
		{ "Mist", Moves::Mist },
		{ "Water Gun", Moves::Water_Gun },
		{ "Hydro Pump", Moves::Hydro_Pump },
		{ "Surf", Moves::Surf },
		{ "Ice Beam", Moves::Ice_Beam },
		{ "Blizzard", Moves::Blizzard },
		{ "Psybeam", Moves::Psybeam },
		{ "Bubble Beam", Moves::Bubble_Beam },
		{ "Aurora Beam", Moves::Aurora_Beam },
		{ "Hyper Beam", Moves::Hyper_Beam },
		{ "Peck", Moves::Peck },
		{ "Drill Peck", Moves::Drill_Peck },
		{ "Submission", Moves::Submission },
		{ "Low Kick", Moves::Low_Kick },
		{ "Counter", Moves::Counter },
		{ "Seismic Toss", Moves::Seismic_Toss },
		{ "Strength", Moves::Strength },
		{ "Absorb", Moves::Absorb },
		{ "Mega Drain", Moves::Mega_Drain },
		{ "Leech Seed", Moves::Leech_Seed },
		{ "Growth", Moves::Growth },
		{ "Razor Leaf", Moves::Razor_Leaf },
		{ "Solar Beam", Moves::Solar_Beam },
		{ "Poison Powder", Moves::Poison_Powder },
		{ "Stun Spore", Moves::Stun_Spore },
		{ "Sleep Powder", Moves::Sleep_Powder },
		{ "Petal Dance", Moves::Petal_Dance },
		{ "String Shot", Moves::String_Shot },
		{ "Dragon Rage", Moves::Dragon_Rage },
		{ "Fire Spin", Moves::Fire_Spin },
		{ "Thunder Shock", Moves::Thunder_Shock },
		{ "Thunderbolt", Moves::Thunderbolt },
		{ "Thunder Wave", Moves::Thunder_Wave },
		{ "Thunder", Moves::Thunder },
		{ "Rock Throw", Moves::Rock_Throw },
		{ "Earthquake", Moves::Earthquake },
		{ "Fissure", Moves::Fissure },
		{ "Dig", Moves::Dig },
		{ "Toxic", Moves::Toxic },
		{ "Confusion", Moves::Confusion },
		{ "Psychic", Moves::Psychic },
		{ "Hypnosis", Moves::Hypnosis },
		{ "Meditate", Moves::Meditate },
		{ "Agility", Moves::Agility },
		{ "Quick Attack", Moves::Quick_Attack },
		{ "Rage", Moves::Rage },
		{ "Teleport", Moves::Teleport },
		{ "Night Shade", Moves::Night_Shade },
		{ "Mimic", Moves::Mimic },
		{ "Screech", Moves::Screech },
		{ "Double Team", Moves::Double_Team },
		{ "Recover", Moves::Recover },
		{ "Harden", Moves::Harden },
		{ "Minimize", Moves::Minimize },
		{ "Smokescreen", Moves::Smokescreen },
		{ "Confuse Ray", Moves::Confuse_Ray },
		{ "Withdraw", Moves::Withdraw },
		{ "Defense Curl", Moves::Defense_Curl },
		{ "Barrier", Moves::Barrier },
		{ "Light Screen", Moves::Light_Screen },
		{ "Haze", Moves::Haze },
		{ "Reflect", Moves::Reflect },
		{ "Focus Energy", Moves::Focus_Energy },
		{ "Bide", Moves::Bide },
		{ "Metronome", Moves::Metronome },
		{ "Mirror Move", Moves::Mirror_Move },
		{ "Self-Destruct", Moves::Self_Destruct },
		{ "Egg Bomb", Moves::Egg_Bomb },
		{ "Lick", Moves::Lick },
		{ "Smog", Moves::Smog },
		{ "Sludge", Moves::Sludge },
		{ "Bone Club", Moves::Bone_Club },
		{ "Fire Blast", Moves::Fire_Blast },
		{ "Waterfall", Moves::Waterfall },
		{ "Clamp", Moves::Clamp },
		{ "Swift", Moves::Swift },
		{ "Skull Bash", Moves::Skull_Bash },
		{ "Spike Cannon", Moves::Spike_Cannon },
		{ "Constrict", Moves::Constrict },
		{ "Amnesia", Moves::Amnesia },
		{ "Kinesis", Moves::Kinesis },
		{ "Soft-Boiled", Moves::Soft_Boiled },
		{ "Hi Jump Kick", Moves::High_Jump_Kick },
		{ "High Jump Kick", Moves::High_Jump_Kick },
		{ "Glare", Moves::Glare },
		{ "Dream Eater", Moves::Dream_Eater },
		{ "Poison Gas", Moves::Poison_Gas },
		{ "Barrage", Moves::Barrage },
		{ "Leech Life", Moves::Leech_Life },
		{ "Lovely Kiss", Moves::Lovely_Kiss },
		{ "Sky Attack", Moves::Sky_Attack },
		{ "Transform", Moves::Transform },
		{ "Bubble", Moves::Bubble },
		{ "Dizzy Punch", Moves::Dizzy_Punch },
		{ "Spore", Moves::Spore },
		{ "Flash", Moves::Flash },
		{ "Psywave", Moves::Psywave },
		{ "Splash", Moves::Splash },
		{ "Acid Armor", Moves::Acid_Armor },
		{ "Crabhammer", Moves::Crabhammer },
		{ "Explosion", Moves::Explosion },
		{ "Fury Swipes", Moves::Fury_Swipes },
		{ "Bonemerang", Moves::Bonemerang },
		{ "Rest", Moves::Rest },
		{ "Rock Slide", Moves::Rock_Slide },
		{ "Hyper Fang", Moves::Hyper_Fang },
		{ "Sharpen", Moves::Sharpen },
		{ "Conversion", Moves::Conversion },
		{ "Tri Attack", Moves::Tri_Attack },
		{ "Super Fang", Moves::Super_Fang },
		{ "Slash", Moves::Slash },
		{ "Substitute", Moves::Substitute },
		{ "Struggle", Moves::Struggle },
		{ "Sketch", Moves::Sketch },
		{ "Triple Kick", Moves::Triple_Kick },
		{ "Thief", Moves::Thief },
		{ "Spider Web", Moves::Spider_Web },
		{ "Mind Reader", Moves::Mind_Reader },
		{ "Nightmare", Moves::Nightmare },
		{ "Flame Wheel", Moves::Flame_Wheel },
		{ "Snore", Moves::Snore },
		{ "Curse", Moves::Curse },
		{ "Flail", Moves::Flail },
		{ "Conversion 2", Moves::Conversion_2 },
		{ "Aeroblast", Moves::Aeroblast },
		{ "Cotton Spore", Moves::Cotton_Spore },
		{ "Reversal", Moves::Reversal },
		{ "Spite", Moves::Spite },
		{ "Powder Snow", Moves::Powder_Snow },
		{ "Protect", Moves::Protect },
		{ "Mach Punch", Moves::Mach_Punch },
		{ "Scary Face", Moves::Scary_Face },
		{ "Faint Attack", Moves::Feint_Attack },
		{ "Feint Attack", Moves::Feint_Attack },
		{ "Sweet Kiss", Moves::Sweet_Kiss },
		{ "Belly Drum", Moves::Belly_Drum },
		{ "Sludge Bomb", Moves::Sludge_Bomb },
		{ "Mud-Slap", Moves::Mud_Slap },
		{ "Octazooka", Moves::Octazooka },
		{ "Spikes", Moves::Spikes },
		{ "Zap Cannon", Moves::Zap_Cannon },
		{ "Foresight", Moves::Foresight },
		{ "Destiny Bond", Moves::Destiny_Bond },
		{ "Perish Song", Moves::Perish_Song },
		{ "Icy Wind", Moves::Icy_Wind },
		{ "Detect", Moves::Detect },
		{ "Bone Rush", Moves::Bone_Rush },
		{ "Lock-On", Moves::Lock_On },
		{ "Outrage", Moves::Outrage },
		{ "Sandstorm", Moves::Sandstorm },
		{ "Giga Drain", Moves::Giga_Drain },
		{ "Endure", Moves::Endure },
		{ "Charm", Moves::Charm },
		{ "Rollout", Moves::Rollout },
		{ "False Swipe", Moves::False_Swipe },
		{ "Swagger", Moves::Swagger },
		{ "Milk Drink", Moves::Milk_Drink },
		{ "Spark", Moves::Spark },
		{ "Fury Cutter", Moves::Fury_Cutter },
		{ "Steel Wing", Moves::Steel_Wing },
		{ "Mean Look", Moves::Mean_Look },
		{ "Attract", Moves::Attract },
		{ "Sleep Talk", Moves::Sleep_Talk },
		{ "Heal Bell", Moves::Heal_Bell },
		{ "Return", Moves::Return },
		{ "Present", Moves::Present },
		{ "Frustration", Moves::Frustration },
		{ "Safeguard", Moves::Safeguard },
		{ "Pain Split", Moves::Pain_Split },
		{ "Sacred Fire", Moves::Sacred_Fire },
		{ "Magnitude", Moves::Magnitude },
		{ "Dynamic Punch", Moves::Dynamic_Punch },
		{ "Megahorn", Moves::Megahorn },
		{ "Dragon Breath", Moves::Dragon_Breath },
		{ "Baton Pass", Moves::Baton_Pass },
		{ "Encore", Moves::Encore },
		{ "Pursuit", Moves::Pursuit },
		{ "Rapid Spin", Moves::Rapid_Spin },
		{ "Sweet Scent", Moves::Sweet_Scent },
		{ "Iron Tail", Moves::Iron_Tail },
		{ "Metal Claw", Moves::Metal_Claw },
		{ "Vital Throw", Moves::Vital_Throw },
		{ "Morning Sun", Moves::Morning_Sun },
		{ "Synthesis", Moves::Synthesis },
		{ "Moonlight", Moves::Moonlight },
		{ "Hidden Power", Moves::Hidden_Power },
		{ "Hidden Power Bug", Moves::Hidden_Power },
		{ "Hidden Power Dark", Moves::Hidden_Power },
		{ "Hidden Power Dragon", Moves::Hidden_Power },
		{ "Hidden Power Electric", Moves::Hidden_Power },
		{ "Hidden Power Fighting", Moves::Hidden_Power },
		{ "Hidden Power Fire", Moves::Hidden_Power },
		{ "Hidden Power Flying", Moves::Hidden_Power },
		{ "Hidden Power Ghost", Moves::Hidden_Power },
		{ "Hidden Power Grass", Moves::Hidden_Power },
		{ "Hidden Power Ground", Moves::Hidden_Power },
		{ "Hidden Power Ice", Moves::Hidden_Power },
		{ "Hidden Power Poison", Moves::Hidden_Power },
		{ "Hidden Power Psychic", Moves::Hidden_Power },
		{ "Hidden Power Rock", Moves::Hidden_Power },
		{ "Hidden Power Steel", Moves::Hidden_Power },
		{ "Hidden Power Water", Moves::Hidden_Power },
		{ "Cross Chop", Moves::Cross_Chop },
		{ "Twister", Moves::Twister },
		{ "Rain Dance", Moves::Rain_Dance },
		{ "Sunny Day", Moves::Sunny_Day },
		{ "Crunch", Moves::Crunch },
		{ "Mirror Coat", Moves::Mirror_Coat },
		{ "Psych Up", Moves::Psych_Up },
		{ "Extreme Speed", Moves::Extreme_Speed },
		{ "Ancient Power", Moves::Ancient_Power },
		{ "Shadow Ball", Moves::Shadow_Ball },
		{ "Future Sight", Moves::Future_Sight },
		{ "Rock Smash", Moves::Rock_Smash },
		{ "Whirlpool", Moves::Whirlpool },
		{ "Beat Up", Moves::Beat_Up },
		{ "Fake Out", Moves::Fake_Out },
		{ "Uproar", Moves::Uproar },
		{ "Stockpile", Moves::Stockpile },
		{ "Spit Up", Moves::Spit_Up },
		{ "Swallow", Moves::Swallow },
		{ "Heat Wave", Moves::Heat_Wave },
		{ "Hail", Moves::Hail },
		{ "Torment", Moves::Torment },
		{ "Flatter", Moves::Flatter },
		{ "Will-O-Wisp", Moves::Will_O_Wisp },
		{ "Memento", Moves::Memento },
		{ "Facade", Moves::Facade },
		{ "Focus Punch", Moves::Focus_Punch },
		{ "SmellingSalt", Moves::Smelling_Salts },
		{ "Smelling Salts", Moves::Smelling_Salts },
		{ "Follow Me", Moves::Follow_Me },
		{ "Nature Power", Moves::Nature_Power },
		{ "Charge", Moves::Charge },
		{ "Taunt", Moves::Taunt },
		{ "Helping Hand", Moves::Helping_Hand },
		{ "Trick", Moves::Trick },
		{ "Role Play", Moves::Role_Play },
		{ "Wish", Moves::Wish },
		{ "Assist", Moves::Assist },
		{ "Ingrain", Moves::Ingrain },
		{ "Superpower", Moves::Superpower },
		{ "Magic Coat", Moves::Magic_Coat },
		{ "Recycle", Moves::Recycle },
		{ "Revenge", Moves::Revenge },
		{ "Brick Break", Moves::Brick_Break },
		{ "Yawn", Moves::Yawn },
		{ "Knock Off", Moves::Knock_Off },
		{ "Endeavor", Moves::Endeavor },
		{ "Eruption", Moves::Eruption },
		{ "Skill Swap", Moves::Skill_Swap },
		{ "Imprison", Moves::Imprison },
		{ "Refresh", Moves::Refresh },
		{ "Grudge", Moves::Grudge },
		{ "Snatch", Moves::Snatch },
		{ "Secret Power", Moves::Secret_Power },
		{ "Dive", Moves::Dive },
		{ "Arm Thrust", Moves::Arm_Thrust },
		{ "Camouflage", Moves::Camouflage },
		{ "Tail Glow", Moves::Tail_Glow },
		{ "Luster Purge", Moves::Luster_Purge },
		{ "Mist Ball", Moves::Mist_Ball },
		{ "Feather Dance", Moves::Feather_Dance },
		{ "Teeter Dance", Moves::Teeter_Dance },
		{ "Blaze Kick", Moves::Blaze_Kick },
		{ "Mud Sport", Moves::Mud_Sport },
		{ "Ice Ball", Moves::Ice_Ball },
		{ "Needle Arm", Moves::Needle_Arm },
		{ "Slack Off", Moves::Slack_Off },
		{ "Hyper Voice", Moves::Hyper_Voice },
		{ "Poison Fang", Moves::Poison_Fang },
		{ "Crush Claw", Moves::Crush_Claw },
		{ "Blast Burn", Moves::Blast_Burn },
		{ "Hydro Cannon", Moves::Hydro_Cannon },
		{ "Meteor Mash", Moves::Meteor_Mash },
		{ "Astonish", Moves::Astonish },
		{ "Weather Ball", Moves::Weather_Ball },
		{ "Aromatherapy", Moves::Aromatherapy },
		{ "Fake Tears", Moves::Fake_Tears },
		{ "Air Cutter", Moves::Air_Cutter },
		{ "Overheat", Moves::Overheat },
		{ "Odor Sleuth", Moves::Odor_Sleuth },
		{ "Rock Tomb", Moves::Rock_Tomb },
		{ "Silver Wind", Moves::Silver_Wind },
		{ "Metal Sound", Moves::Metal_Sound },
		{ "Grass Whistle", Moves::Grass_Whistle },
		{ "Tickle", Moves::Tickle },
		{ "Cosmic Power", Moves::Cosmic_Power },
		{ "Water Spout", Moves::Water_Spout },
		{ "Signal Beam", Moves::Signal_Beam },
		{ "Shadow Punch", Moves::Shadow_Punch },
		{ "Extrasensory", Moves::Extrasensory },
		{ "Sky Uppercut", Moves::Sky_Uppercut },
		{ "Sand Tomb", Moves::Sand_Tomb },
		{ "Sheer Cold", Moves::Sheer_Cold },
		{ "Muddy Water", Moves::Muddy_Water },
		{ "Bullet Seed", Moves::Bullet_Seed },
		{ "Aerial Ace", Moves::Aerial_Ace },
		{ "Icicle Spear", Moves::Icicle_Spear },
		{ "Iron Defense", Moves::Iron_Defense },
		{ "Block", Moves::Block },
		{ "Howl", Moves::Howl },
		{ "Dragon Claw", Moves::Dragon_Claw },
		{ "Frenzy Plant", Moves::Frenzy_Plant },
		{ "Bulk Up", Moves::Bulk_Up },
		{ "Bounce", Moves::Bounce },
		{ "Mud Shot", Moves::Mud_Shot },
		{ "Poison Tail", Moves::Poison_Tail },
		{ "Covet", Moves::Covet },
		{ "Volt Tackle", Moves::Volt_Tackle },
		{ "Magical Leaf", Moves::Magical_Leaf },
		{ "Water Sport", Moves::Water_Sport },
		{ "Calm Mind", Moves::Calm_Mind },
		{ "Leaf Blade", Moves::Leaf_Blade },
		{ "Dragon Dance", Moves::Dragon_Dance },
		{ "Rock Blast", Moves::Rock_Blast },
		{ "Shock Wave", Moves::Shock_Wave },
		{ "Water Pulse", Moves::Water_Pulse },
		{ "Doom Desire", Moves::Doom_Desire },
		{ "Psycho Boost", Moves::Psycho_Boost },
		{ "Roost", Moves::Roost },
		{ "Gravity", Moves::Gravity },
		{ "Miracle Eye", Moves::Miracle_Eye },
		{ "Wake-Up Slap", Moves::Wake_Up_Slap },
		{ "Hammer Arm", Moves::Hammer_Arm },
		{ "Gyro Ball", Moves::Gyro_Ball },
		{ "Healing Wish", Moves::Healing_Wish },
		{ "Brine", Moves::Brine },
		{ "Natural Gift", Moves::Natural_Gift },
		{ "Feint", Moves::Feint },
		{ "Pluck", Moves::Pluck },
		{ "Tailwind", Moves::Tailwind },
		{ "Acupressure", Moves::Acupressure },
		{ "Metal Burst", Moves::Metal_Burst },
		{ "U-turn", Moves::U_turn },
		{ "Close Combat", Moves::Close_Combat },
		{ "Payback", Moves::Payback },
		{ "Assurance", Moves::Assurance },
		{ "Embargo", Moves::Embargo },
		{ "Fling", Moves::Fling },
		{ "Psycho Shift", Moves::Psycho_Shift },
		{ "Trump Card", Moves::Trump_Card },
		{ "Heal Block", Moves::Heal_Block },
		{ "Wring Out", Moves::Wring_Out },
		{ "Power Trick", Moves::Power_Trick },
		{ "Gastro Acid", Moves::Gastro_Acid },
		{ "Lucky Chant", Moves::Lucky_Chant },
		{ "Me First", Moves::Me_First },
		{ "Copycat", Moves::Copycat },
		{ "Power Swap", Moves::Power_Swap },
		{ "Guard Swap", Moves::Guard_Swap },
		{ "Punishment", Moves::Punishment },
		{ "Last Resort", Moves::Last_Resort },
		{ "Worry Seed", Moves::Worry_Seed },
		{ "Sucker Punch", Moves::Sucker_Punch },
		{ "Toxic Spikes", Moves::Toxic_Spikes },
		{ "Heart Swap", Moves::Heart_Swap },
		{ "Aqua Ring", Moves::Aqua_Ring },
		{ "Magnet Rise", Moves::Magnet_Rise },
		{ "Flare Blitz", Moves::Flare_Blitz },
		{ "Force Palm", Moves::Force_Palm },
		{ "Aura Sphere", Moves::Aura_Sphere },
		{ "Rock Polish", Moves::Rock_Polish },
		{ "Poison Jab", Moves::Poison_Jab },
		{ "Dark Pulse", Moves::Dark_Pulse },
		{ "Night Slash", Moves::Night_Slash },
		{ "Aqua Tail", Moves::Aqua_Tail },
		{ "Seed Bomb", Moves::Seed_Bomb },
		{ "Air Slash", Moves::Air_Slash },
		{ "X-Scissor", Moves::X_Scissor },
		{ "Bug Buzz", Moves::Bug_Buzz },
		{ "Dragon Pulse", Moves::Dragon_Pulse },
		{ "Dragon Rush", Moves::Dragon_Rush },
		{ "Power Gem", Moves::Power_Gem },
		{ "Drain Punch", Moves::Drain_Punch },
		{ "Vacuum Wave", Moves::Vacuum_Wave },
		{ "Focus Blast", Moves::Focus_Blast },
		{ "Energy Ball", Moves::Energy_Ball },
		{ "Brave Bird", Moves::Brave_Bird },
		{ "Earth Power", Moves::Earth_Power },
		{ "Switcheroo", Moves::Switcheroo },
		{ "Giga Impact", Moves::Giga_Impact },
		{ "Nasty Plot", Moves::Nasty_Plot },
		{ "Bullet Punch", Moves::Bullet_Punch },
		{ "Avalanche", Moves::Avalanche },
		{ "Ice Shard", Moves::Ice_Shard },
		{ "Shadow Claw", Moves::Shadow_Claw },
		{ "Thunder Fang", Moves::Thunder_Fang },
		{ "Ice Fang", Moves::Ice_Fang },
		{ "Fire Fang", Moves::Fire_Fang },
		{ "Shadow Sneak", Moves::Shadow_Sneak },
		{ "Mud Bomb", Moves::Mud_Bomb },
		{ "Psycho Cut", Moves::Psycho_Cut },
		{ "Zen Headbutt", Moves::Zen_Headbutt },
		{ "Mirror Shot", Moves::Mirror_Shot },
		{ "Flash Cannon", Moves::Flash_Cannon },
		{ "Rock Climb", Moves::Rock_Climb },
		{ "Defog", Moves::Defog },
		{ "Trick Room", Moves::Trick_Room },
		{ "Draco Meteor", Moves::Draco_Meteor },
		{ "Discharge", Moves::Discharge },
		{ "Lava Plume", Moves::Lava_Plume },
		{ "Leaf Storm", Moves::Leaf_Storm },
		{ "Power Whip", Moves::Power_Whip },
		{ "Rock Wrecker", Moves::Rock_Wrecker },
		{ "Cross Poison", Moves::Cross_Poison },
		{ "Gunk Shot", Moves::Gunk_Shot },
		{ "Iron Head", Moves::Iron_Head },
		{ "Magnet Bomb", Moves::Magnet_Bomb },
		{ "Stone Edge", Moves::Stone_Edge },
		{ "Captivate", Moves::Captivate },
		{ "Stealth Rock", Moves::Stealth_Rock },
		{ "Grass Knot", Moves::Grass_Knot },
		{ "Chatter", Moves::Chatter },
		{ "Judgment", Moves::Judgment },
		{ "Bug Bite", Moves::Bug_Bite },
		{ "Charge Beam", Moves::Charge_Beam },
		{ "Wood Hammer", Moves::Wood_Hammer },
		{ "Aqua Jet", Moves::Aqua_Jet },
		{ "Attack Order", Moves::Attack_Order },
		{ "Defend Order", Moves::Defend_Order },
		{ "Heal Order", Moves::Heal_Order },
		{ "Head Smash", Moves::Head_Smash },
		{ "Double Hit", Moves::Double_Hit },
		{ "Roar of Time", Moves::Roar_of_Time },
		{ "Spacial Rend", Moves::Spacial_Rend },
		{ "Lunar Dance", Moves::Lunar_Dance },
		{ "Crush Grip", Moves::Crush_Grip },
		{ "Magma Storm", Moves::Magma_Storm },
		{ "Dark Void", Moves::Dark_Void },
		{ "Seed Flare", Moves::Seed_Flare },
		{ "Ominous Wind", Moves::Ominous_Wind },
		{ "Shadow Force", Moves::Shadow_Force },
		{ "Hone Claws", Moves::Hone_Claws },
		{ "Wide Guard", Moves::Wide_Guard },
		{ "Guard Split", Moves::Guard_Split },
		{ "Power Split", Moves::Power_Split },
		{ "Wonder Room", Moves::Wonder_Room },
		{ "Psyshock", Moves::Psyshock },
		{ "Venoshock", Moves::Venoshock },
		{ "Autotomize", Moves::Autotomize },
		{ "Rage Powder", Moves::Rage_Powder },
		{ "Telekinesis", Moves::Telekinesis },
		{ "Magic Room", Moves::Magic_Room },
		{ "Smack Down", Moves::Smack_Down },
		{ "Storm Throw", Moves::Storm_Throw },
		{ "Flame Burst", Moves::Flame_Burst },
		{ "Sludge Wave", Moves::Sludge_Wave },
		{ "Quiver Dance", Moves::Quiver_Dance },
		{ "Heavy Slam", Moves::Heavy_Slam },
		{ "Synchronoise", Moves::Synchronoise },
		{ "Electro Ball", Moves::Electro_Ball },
		{ "Soak", Moves::Soak },
		{ "Flame Charge", Moves::Flame_Charge },
		{ "Coil", Moves::Coil },
		{ "Low Sweep", Moves::Low_Sweep },
		{ "Acid Spray", Moves::Acid_Spray },
		{ "Foul Play", Moves::Foul_Play },
		{ "Simple Beam", Moves::Simple_Beam },
		{ "Entrainment", Moves::Entrainment },
		{ "After You", Moves::After_You },
		{ "Round", Moves::Round },
		{ "Echoed Voice", Moves::Echoed_Voice },
		{ "Chip Away", Moves::Chip_Away },
		{ "Clear Smog", Moves::Clear_Smog },
		{ "Stored Power", Moves::Stored_Power },
		{ "Quick Guard", Moves::Quick_Guard },
		{ "Ally Switch", Moves::Ally_Switch },
		{ "Scald", Moves::Scald },
		{ "Shell Smash", Moves::Shell_Smash },
		{ "Heal Pulse", Moves::Heal_Pulse },
		{ "Hex", Moves::Hex },
		{ "Sky Drop", Moves::Sky_Drop },
		{ "Shift Gear", Moves::Shift_Gear },
		{ "Circle Throw", Moves::Circle_Throw },
		{ "Incinerate", Moves::Incinerate },
		{ "Quash", Moves::Quash },
		{ "Acrobatics", Moves::Acrobatics },
		{ "Reflect Type", Moves::Reflect_Type },
		{ "Retaliate", Moves::Retaliate },
		{ "Final Gambit", Moves::Final_Gambit },
		{ "Bestow", Moves::Bestow },
		{ "Inferno", Moves::Inferno },
		{ "Water Pledge", Moves::Water_Pledge },
		{ "Fire Pledge", Moves::Fire_Pledge },
		{ "Grass Pledge", Moves::Grass_Pledge },
		{ "Volt Switch", Moves::Volt_Switch },
		{ "Struggle Bug", Moves::Struggle_Bug },
		{ "Bulldoze", Moves::Bulldoze },
		{ "Frost Breath", Moves::Frost_Breath },
		{ "Dragon Tail", Moves::Dragon_Tail },
		{ "Work Up", Moves::Work_Up },
		{ "Electroweb", Moves::Electroweb },
		{ "Wild Charge", Moves::Wild_Charge },
		{ "Drill Run", Moves::Drill_Run },
		{ "Dual Chop", Moves::Dual_Chop },
		{ "Heart Stamp", Moves::Heart_Stamp },
		{ "Horn Leech", Moves::Horn_Leech },
		{ "Sacred Sword", Moves::Sacred_Sword },
		{ "Razor Shell", Moves::Razor_Shell },
		{ "Heat Crash", Moves::Heat_Crash },
		{ "Leaf Tornado", Moves::Leaf_Tornado },
		{ "Steamroller", Moves::Steamroller },
		{ "Cotton Guard", Moves::Cotton_Guard },
		{ "Night Daze", Moves::Night_Daze },
		{ "Psystrike", Moves::Psystrike },
		{ "Tail Slap", Moves::Tail_Slap },
		{ "Hurricane", Moves::Hurricane },
		{ "Head Charge", Moves::Head_Charge },
		{ "Gear Grind", Moves::Gear_Grind },
		{ "Searing Shot", Moves::Searing_Shot },
		{ "Techno Blast", Moves::Techno_Blast },
		{ "Relic Song", Moves::Relic_Song },
		{ "Secret Sword", Moves::Secret_Sword },
		{ "Glaciate", Moves::Glaciate },
		{ "Bolt Strike", Moves::Bolt_Strike },
		{ "Blue Flare", Moves::Blue_Flare },
		{ "Fiery Dance", Moves::Fiery_Dance },
		{ "Freeze Shock", Moves::Freeze_Shock },
		{ "Ice Burn", Moves::Ice_Burn },
		{ "Snarl", Moves::Snarl },
		{ "Icicle Crash", Moves::Icicle_Crash },
		{ "V-create", Moves::V_create },
		{ "Fusion Flare", Moves::Fusion_Flare },
		{ "Fusion Bolt", Moves::Fusion_Bolt },
		{ "Flying Press", Moves::Flying_Press },
		{ "Mat Block", Moves::Mat_Block },
		{ "Belch", Moves::Belch },
		{ "Rototiller", Moves::Rototiller },
		{ "Sticky Web", Moves::Sticky_Web },
		{ "Fell Stinger", Moves::Fell_Stinger },
		{ "Phantom Force", Moves::Phantom_Force },
		{ "Trick-or-Treat", Moves::Trick_or_Treat },
		{ "Noble Roar", Moves::Noble_Roar },
		{ "Ion Deluge", Moves::Ion_Deluge },
		{ "Parabolic Charge", Moves::Parabolic_Charge },
		{ "Forest's Curse", Moves::Forests_Curse },
		{ "Petal Blizzard", Moves::Petal_Blizzard },
		{ "Freeze-Dry", Moves::Freeze_Dry },
		{ "Disarming Voice", Moves::Disarming_Voice },
		{ "Parting Shot", Moves::Parting_Shot },
		{ "Topsy-Turvy", Moves::Topsy_Turvy },
		{ "Draining Kiss", Moves::Draining_Kiss },
		{ "Crafty Shield", Moves::Crafty_Shield },
		{ "Flower Shield", Moves::Flower_Shield },
		{ "Grassy Terrain", Moves::Grassy_Terrain },
		{ "Misty Terrain", Moves::Misty_Terrain },
		{ "Electrify", Moves::Electrify },
		{ "Play Rough", Moves::Play_Rough },
		{ "Fairy Wind", Moves::Fairy_Wind },
		{ "Moonblast", Moves::Moonblast },
		{ "Boomburst", Moves::Boomburst },
		{ "Fairy Lock", Moves::Fairy_Lock },
		{ "King's Shield", Moves::Kings_Shield },
		{ "Play Nice", Moves::Play_Nice },
		{ "Confide", Moves::Confide },
		{ "Diamond Storm", Moves::Diamond_Storm },
		{ "Steam Eruption", Moves::Steam_Eruption },
		{ "Hyperspace Hole", Moves::Hyperspace_Hole },
		{ "Water Shuriken", Moves::Water_Shuriken },
		{ "Mystical Fire", Moves::Mystical_Fire },
		{ "Spiky Shield", Moves::Spiky_Shield },
		{ "Aromatic Mist", Moves::Aromatic_Mist },
		{ "Eerie Impulse", Moves::Eerie_Impulse },
		{ "Venom Drench", Moves::Venom_Drench },
		{ "Powder", Moves::Powder },
		{ "Geomancy", Moves::Geomancy },
		{ "Magnetic Flux", Moves::Magnetic_Flux },
		{ "Happy Hour", Moves::Happy_Hour },
		{ "Electric Terrain", Moves::Electric_Terrain },
		{ "Dazzling Gleam", Moves::Dazzling_Gleam },
		{ "Celebrate", Moves::Celebrate },
		{ "Hold Hands", Moves::Hold_Hands },
		{ "Baby-Doll Eyes", Moves::Baby_Doll_Eyes },
		{ "Nuzzle", Moves::Nuzzle },
		{ "Hold Back", Moves::Hold_Back },
		{ "Infestation", Moves::Infestation },
		{ "Power-Up Punch", Moves::Power_Up_Punch },
		{ "Oblivion Wing", Moves::Oblivion_Wing },
		{ "Thousand Arrows", Moves::Thousand_Arrows },
		{ "Thousand Waves", Moves::Thousand_Waves },
		{ "Land's Wrath", Moves::Lands_Wrath },
		{ "Light of Ruin", Moves::Light_of_Ruin },
		{ "Origin Pulse", Moves::Origin_Pulse },
		{ "Precipice Blades", Moves::Precipice_Blades },
		{ "Dragon Ascent", Moves::Dragon_Ascent },
		{ "Hyperspace Fury", Moves::Hyperspace_Fury },
		{ "Breakneck Blitz", Moves::Breakneck_Blitz },
		{ "All-Out Pummeling", Moves::All_Out_Pummeling },
		{ "Supersonic Skystrike", Moves::Supersonic_Skystrike },
		{ "Acid Downpour", Moves::Acid_Downpour },
		{ "Tectonic Rage", Moves::Tectonic_Rage },
		{ "Continental Crush", Moves::Continental_Crush },
		{ "Savage Spin-Out", Moves::Savage_Spin_Out },
		{ "Never-Ending Nightmare", Moves::Never_Ending_Nightmare },
		{ "Corkscrew Crash", Moves::Corkscrew_Crash },
		{ "Inferno Overdrive", Moves::Inferno_Overdrive },
		{ "Hydro Vortex", Moves::Hydro_Vortex },
		{ "Bloom Doom", Moves::Bloom_Doom },
		{ "Gigavolt Havoc", Moves::Gigavolt_Havoc },
		{ "Shattered Psyche", Moves::Shattered_Psyche },
		{ "Subzero Slammer", Moves::Subzero_Slammer },
		{ "Devastating Drake", Moves::Devastating_Drake },
		{ "Black Hole Eclipse", Moves::Black_Hole_Eclipse },
		{ "Twinkle Tackle", Moves::Twinkle_Tackle },
		{ "Catastropika", Moves::Catastropika },
		{ "Shore Up", Moves::Shore_Up },
		{ "First Impression", Moves::First_Impression },
		{ "Baneful Bunker", Moves::Baneful_Bunker },
		{ "Spirit Shackle", Moves::Spirit_Shackle },
		{ "Darkest Lariat", Moves::Darkest_Lariat },
		{ "Sparkling Aria", Moves::Sparkling_Aria },
		{ "Ice Hammer", Moves::Ice_Hammer },
		{ "Floral Healing", Moves::Floral_Healing },
		{ "High Horsepower", Moves::High_Horsepower },
		{ "Strength Sap", Moves::Strength_Sap },
		{ "Solar Blade", Moves::Solar_Blade },
		{ "Leafage", Moves::Leafage },
		{ "Spotlight", Moves::Spotlight },
		{ "Toxic Thread", Moves::Toxic_Thread },
		{ "Laser Focus", Moves::Laser_Focus },
		{ "Gear Up", Moves::Gear_Up },
		{ "Throat Chop", Moves::Throat_Chop },
		{ "Pollen Puff", Moves::Pollen_Puff },
		{ "Anchor Shot", Moves::Anchor_Shot },
		{ "Psychic Terrain", Moves::Psychic_Terrain },
		{ "Lunge", Moves::Lunge },
		{ "Fire Lash", Moves::Fire_Lash },
		{ "Power Trip", Moves::Power_Trip },
		{ "Burn Up", Moves::Burn_Up },
		{ "Speed Swap", Moves::Speed_Swap },
		{ "Smart Strike", Moves::Smart_Strike },
		{ "Purify", Moves::Purify },
		{ "Revelation Dance", Moves::Revelation_Dance },
		{ "Core Enforcer", Moves::Core_Enforcer },
		{ "Trop Kick", Moves::Trop_Kick },
		{ "Instruct", Moves::Instruct },
		{ "Beak Blast", Moves::Beak_Blast },
		{ "Clanging Scales", Moves::Clanging_Scales },
		{ "Dragon Hammer", Moves::Dragon_Hammer },
		{ "Brutal Swing", Moves::Brutal_Swing },
		{ "Aurora Veil", Moves::Aurora_Veil },
		{ "Sinister Arrow Raid", Moves::Sinister_Arrow_Raid },
		{ "Malicious Moonsault", Moves::Malicious_Moonsault },
		{ "Oceanic Operetta", Moves::Oceanic_Operetta },
		{ "Guardian of Alola", Moves::Guardian_of_Alola },
		{ "Soul-Stealing 7-Star Strike", Moves::Soul_Stealing_7_Star_Strike },
		{ "Stoked Sparksurfer", Moves::Stoked_Sparksurfer },
		{ "Pulverizing Pancake", Moves::Pulverizing_Pancake },
		{ "Extreme Evoboost", Moves::Extreme_Evoboost },
		{ "Genesis Supernova", Moves::Genesis_Supernova },
		{ "Shell Trap", Moves::Shell_Trap },
		{ "Fleur Cannon", Moves::Fleur_Cannon },
		{ "Psychic Fangs", Moves::Psychic_Fangs },
		{ "Stomping Tantrum", Moves::Stomping_Tantrum },
		{ "Shadow Bone", Moves::Shadow_Bone },
		{ "Accelerock", Moves::Accelerock },
		{ "Liquidation", Moves::Liquidation },
		{ "Prismatic Laser", Moves::Prismatic_Laser },
		{ "Spectral Thief", Moves::Spectral_Thief },
		{ "Sunsteel Strike", Moves::Sunsteel_Strike },
		{ "Moongeist Beam", Moves::Moongeist_Beam },
		{ "Tearful Look", Moves::Tearful_Look },
		{ "Zing Zap", Moves::Zing_Zap },
		{ "Nature's Madness", Moves::Natures_Madness },
		{ "Multi-Attack", Moves::Multi_Attack },
		{ "10,000,000 Volt Thunderbolt", Moves::m10000000_Volt_Thunderbolt },
		{ "Mind Blown", Moves::Mind_Blown },
		{ "Plasma Fists", Moves::Plasma_Fists },
		{ "Photon Geyser", Moves::Photon_Geyser },
		{ "Light That Burns the Sky", Moves::Light_That_Burns_the_Sky },
		{ "Searing Sunraze Smash", Moves::Searing_Sunraze_Smash },
		{ "Menacing Moonraze Maelstrom", Moves::Menacing_Moonraze_Maelstrom },
		{ "Let's Snuggle Forever", Moves::Lets_Snuggle_Forever },
		{ "Splintered Stormshards", Moves::Splintered_Stormshards },
		{ "Clangorous Soulblaze", Moves::Clangorous_Soulblaze },
		{ "Zippy Zap", Moves::Zippy_Zap },
		{ "Splishy Splash", Moves::Splishy_Splash },
		{ "Floaty Fall", Moves::Floaty_Fall },
		{ "Pika Papow", Moves::Pika_Papow },
		{ "Bouncy Bubble", Moves::Bouncy_Bubble },
		{ "Buzzy Buzz", Moves::Buzzy_Buzz },
		{ "Sizzly Slide", Moves::Sizzly_Slide },
		{ "Glitzy Glow", Moves::Glitzy_Glow },
		{ "Baddy Bad", Moves::Baddy_Bad },
		{ "Sappy Seed", Moves::Sappy_Seed },
		{ "Freezy Frost", Moves::Freezy_Frost },
		{ "Sparkly Swirl", Moves::Sparkly_Swirl },
		{ "Veevee Volley", Moves::Veevee_Volley },
		{ "Double Iron Bash", Moves::Double_Iron_Bash },
	};
	auto const it = converter.find(str);
	if (it != end(converter)) {
		return it->second;
	} else {
		throw InvalidFromStringConversion("Move", str);
	}
}

}	// namespace technicalmachine
