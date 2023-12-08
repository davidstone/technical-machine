// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string_view>

export module tm.string_conversions.move_name;

export import tm.string_conversions.from_string;
import tm.string_conversions.invalid_string_conversion;
import tm.string_conversions.lowercase_alphanumeric;

import tm.move.move_name;

import bounded;
import containers;
import std_module;

namespace technicalmachine {

using namespace bounded::literal;

export constexpr auto to_string(MoveName const move) -> std::string_view {
	switch (move) {
		// Weird moves
		case MoveName::Pass: return "Pass";
		case MoveName::Switch0: return "Switch0";
		case MoveName::Switch1: return "Switch1";
		case MoveName::Switch2: return "Switch2";
		case MoveName::Switch3: return "Switch3";
		case MoveName::Switch4: return "Switch4";
		case MoveName::Switch5: return "Switch5";
		case MoveName::Hit_Self: return "Hit self in confusion";

		case MoveName::Pound: return "Pound";
		case MoveName::Karate_Chop: return "Karate Chop";
		case MoveName::Double_Slap: return "Double Slap";
		case MoveName::Comet_Punch: return "Comet Punch";
		case MoveName::Mega_Punch: return "Mega Punch";
		case MoveName::Pay_Day: return "Pay Day";
		case MoveName::Fire_Punch: return "Fire Punch";
		case MoveName::Ice_Punch: return "Ice Punch";
		case MoveName::Thunder_Punch: return "Thunder Punch";
		case MoveName::Scratch: return "Scratch";
		case MoveName::Vise_Grip: return "Vise Grip";
		case MoveName::Guillotine: return "Guillotine";
		case MoveName::Razor_Wind: return "Razor Wind";
		case MoveName::Swords_Dance: return "Swords Dance";
		case MoveName::Cut: return "Cut";
		case MoveName::Gust: return "Gust";
		case MoveName::Wing_Attack: return "Wing Attack";
		case MoveName::Whirlwind: return "Whirlwind";
		case MoveName::Fly: return "Fly";
		case MoveName::Bind: return "Bind";
		case MoveName::Slam: return "Slam";
		case MoveName::Vine_Whip: return "Vine Whip";
		case MoveName::Stomp: return "Stomp";
		case MoveName::Double_Kick: return "Double Kick";
		case MoveName::Mega_Kick: return "Mega Kick";
		case MoveName::Jump_Kick: return "Jump Kick";
		case MoveName::Rolling_Kick: return "Rolling Kick";
		case MoveName::Sand_Attack: return "Sand Attack";
		case MoveName::Headbutt: return "Headbutt";
		case MoveName::Horn_Attack: return "Horn Attack";
		case MoveName::Fury_Attack: return "Fury Attack";
		case MoveName::Horn_Drill: return "Horn Drill";
		case MoveName::Tackle: return "Tackle";
		case MoveName::Body_Slam: return "Body Slam";
		case MoveName::Wrap: return "Wrap";
		case MoveName::Take_Down: return "Take Down";
		case MoveName::Thrash: return "Thrash";
		case MoveName::Double_Edge: return "Double-Edge";
		case MoveName::Tail_Whip: return "Tail Whip";
		case MoveName::Poison_Sting: return "Poison Sting";
		case MoveName::Twineedle: return "Twineedle";
		case MoveName::Pin_Missile: return "Pin Missile";
		case MoveName::Leer: return "Leer";
		case MoveName::Bite: return "Bite";
		case MoveName::Growl: return "Growl";
		case MoveName::Roar: return "Roar";
		case MoveName::Sing: return "Sing";
		case MoveName::Supersonic: return "Supersonic";
		case MoveName::Sonic_Boom: return "Sonic Boom";
		case MoveName::Disable: return "Disable";
		case MoveName::Acid: return "Acid";
		case MoveName::Ember: return "Ember";
		case MoveName::Flamethrower: return "Flamethrower";
		case MoveName::Mist: return "Mist";
		case MoveName::Water_Gun: return "Water Gun";
		case MoveName::Hydro_Pump: return "Hydro Pump";
		case MoveName::Surf: return "Surf";
		case MoveName::Ice_Beam: return "Ice Beam";
		case MoveName::Blizzard: return "Blizzard";
		case MoveName::Psybeam: return "Psybeam";
		case MoveName::Bubble_Beam: return "Bubble Beam";
		case MoveName::Aurora_Beam: return "Aurora Beam";
		case MoveName::Hyper_Beam: return "Hyper Beam";
		case MoveName::Peck: return "Peck";
		case MoveName::Drill_Peck: return "Drill Peck";
		case MoveName::Submission: return "Submission";
		case MoveName::Low_Kick: return "Low Kick";
		case MoveName::Counter: return "Counter";
		case MoveName::Seismic_Toss: return "Seismic Toss";
		case MoveName::Strength: return "Strength";
		case MoveName::Absorb: return "Absorb";
		case MoveName::Mega_Drain: return "Mega Drain";
		case MoveName::Leech_Seed: return "Leech Seed";
		case MoveName::Growth: return "Growth";
		case MoveName::Razor_Leaf: return "Razor Leaf";
		case MoveName::Solar_Beam: return "Solar Beam";
		case MoveName::Poison_Powder: return "Poison Powder";
		case MoveName::Stun_Spore: return "Stun Spore";
		case MoveName::Sleep_Powder: return "Sleep Powder";
		case MoveName::Petal_Dance: return "Petal Dance";
		case MoveName::String_Shot: return "String Shot";
		case MoveName::Dragon_Rage: return "Dragon Rage";
		case MoveName::Fire_Spin: return "Fire Spin";
		case MoveName::Thunder_Shock: return "Thunder Shock";
		case MoveName::Thunderbolt: return "Thunderbolt";
		case MoveName::Thunder_Wave: return "Thunder Wave";
		case MoveName::Thunder: return "Thunder";
		case MoveName::Rock_Throw: return "Rock Throw";
		case MoveName::Earthquake: return "Earthquake";
		case MoveName::Fissure: return "Fissure";
		case MoveName::Dig: return "Dig";
		case MoveName::Toxic: return "Toxic";
		case MoveName::Confusion: return "Confusion";
		case MoveName::Psychic: return "Psychic";
		case MoveName::Hypnosis: return "Hypnosis";
		case MoveName::Meditate: return "Meditate";
		case MoveName::Agility: return "Agility";
		case MoveName::Quick_Attack: return "Quick Attack";
		case MoveName::Rage: return "Rage";
		case MoveName::Teleport: return "Teleport";
		case MoveName::Night_Shade: return "Night Shade";
		case MoveName::Mimic: return "Mimic";
		case MoveName::Screech: return "Screech";
		case MoveName::Double_Team: return "Double Team";
		case MoveName::Recover: return "Recover";
		case MoveName::Harden: return "Harden";
		case MoveName::Minimize: return "Minimize";
		case MoveName::Smokescreen: return "Smokescreen";
		case MoveName::Confuse_Ray: return "Confuse Ray";
		case MoveName::Withdraw: return "Withdraw";
		case MoveName::Defense_Curl: return "Defense Curl";
		case MoveName::Barrier: return "Barrier";
		case MoveName::Light_Screen: return "Light Screen";
		case MoveName::Haze: return "Haze";
		case MoveName::Reflect: return "Reflect";
		case MoveName::Focus_Energy: return "Focus Energy";
		case MoveName::Bide: return "Bide";
		case MoveName::Metronome: return "Metronome";
		case MoveName::Mirror_Move: return "Mirror Move";
		case MoveName::Self_Destruct: return "Self-Destruct";
		case MoveName::Egg_Bomb: return "Egg Bomb";
		case MoveName::Lick: return "Lick";
		case MoveName::Smog: return "Smog";
		case MoveName::Sludge: return "Sludge";
		case MoveName::Bone_Club: return "Bone Club";
		case MoveName::Fire_Blast: return "Fire Blast";
		case MoveName::Waterfall: return "Waterfall";
		case MoveName::Clamp: return "Clamp";
		case MoveName::Swift: return "Swift";
		case MoveName::Skull_Bash: return "Skull Bash";
		case MoveName::Spike_Cannon: return "Spike Cannon";
		case MoveName::Constrict: return "Constrict";
		case MoveName::Amnesia: return "Amnesia";
		case MoveName::Kinesis: return "Kinesis";
		case MoveName::Soft_Boiled: return "Soft-Boiled";
		case MoveName::High_Jump_Kick: return "High Jump Kick";
		case MoveName::Glare: return "Glare";
		case MoveName::Dream_Eater: return "Dream Eater";
		case MoveName::Poison_Gas: return "Poison Gas";
		case MoveName::Barrage: return "Barrage";
		case MoveName::Leech_Life: return "Leech Life";
		case MoveName::Lovely_Kiss: return "Lovely Kiss";
		case MoveName::Sky_Attack: return "Sky Attack";
		case MoveName::Transform: return "Transform";
		case MoveName::Bubble: return "Bubble";
		case MoveName::Dizzy_Punch: return "Dizzy Punch";
		case MoveName::Spore: return "Spore";
		case MoveName::Flash: return "Flash";
		case MoveName::Psywave: return "Psywave";
		case MoveName::Splash: return "Splash";
		case MoveName::Acid_Armor: return "Acid Armor";
		case MoveName::Crabhammer: return "Crabhammer";
		case MoveName::Explosion: return "Explosion";
		case MoveName::Fury_Swipes: return "Fury Swipes";
		case MoveName::Bonemerang: return "Bonemerang";
		case MoveName::Rest: return "Rest";
		case MoveName::Rock_Slide: return "Rock Slide";
		case MoveName::Hyper_Fang: return "Hyper Fang";
		case MoveName::Sharpen: return "Sharpen";
		case MoveName::Conversion: return "Conversion";
		case MoveName::Tri_Attack: return "Tri Attack";
		case MoveName::Super_Fang: return "Super Fang";
		case MoveName::Slash: return "Slash";
		case MoveName::Substitute: return "Substitute";
		case MoveName::Struggle: return "Struggle";
		case MoveName::Sketch: return "Sketch";
		case MoveName::Triple_Kick: return "Triple Kick";
		case MoveName::Thief: return "Thief";
		case MoveName::Spider_Web: return "Spider Web";
		case MoveName::Mind_Reader: return "Mind Reader";
		case MoveName::Nightmare: return "Nightmare";
		case MoveName::Flame_Wheel: return "Flame Wheel";
		case MoveName::Snore: return "Snore";
		case MoveName::Curse: return "Curse";
		case MoveName::Flail: return "Flail";
		case MoveName::Conversion_2: return "Conversion 2";
		case MoveName::Aeroblast: return "Aeroblast";
		case MoveName::Cotton_Spore: return "Cotton Spore";
		case MoveName::Reversal: return "Reversal";
		case MoveName::Spite: return "Spite";
		case MoveName::Powder_Snow: return "Powder Snow";
		case MoveName::Protect: return "Protect";
		case MoveName::Mach_Punch: return "Mach Punch";
		case MoveName::Scary_Face: return "Scary Face";
		case MoveName::Feint_Attack: return "Feint Attack";
		case MoveName::Sweet_Kiss: return "Sweet Kiss";
		case MoveName::Belly_Drum: return "Belly Drum";
		case MoveName::Sludge_Bomb: return "Sludge Bomb";
		case MoveName::Mud_Slap: return "Mud-Slap";
		case MoveName::Octazooka: return "Octazooka";
		case MoveName::Spikes: return "Spikes";
		case MoveName::Zap_Cannon: return "Zap Cannon";
		case MoveName::Foresight: return "Foresight";
		case MoveName::Destiny_Bond: return "Destiny Bond";
		case MoveName::Perish_Song: return "Perish Song";
		case MoveName::Icy_Wind: return "Icy Wind";
		case MoveName::Detect: return "Detect";
		case MoveName::Bone_Rush: return "Bone Rush";
		case MoveName::Lock_On: return "Lock-On";
		case MoveName::Outrage: return "Outrage";
		case MoveName::Sandstorm: return "Sandstorm";
		case MoveName::Giga_Drain: return "Giga Drain";
		case MoveName::Endure: return "Endure";
		case MoveName::Charm: return "Charm";
		case MoveName::Rollout: return "Rollout";
		case MoveName::False_Swipe: return "False Swipe";
		case MoveName::Swagger: return "Swagger";
		case MoveName::Milk_Drink: return "Milk Drink";
		case MoveName::Spark: return "Spark";
		case MoveName::Fury_Cutter: return "Fury Cutter";
		case MoveName::Steel_Wing: return "Steel Wing";
		case MoveName::Mean_Look: return "Mean Look";
		case MoveName::Attract: return "Attract";
		case MoveName::Sleep_Talk: return "Sleep Talk";
		case MoveName::Heal_Bell: return "Heal Bell";
		case MoveName::Return: return "Return";
		case MoveName::Present: return "Present";
		case MoveName::Frustration: return "Frustration";
		case MoveName::Safeguard: return "Safeguard";
		case MoveName::Pain_Split: return "Pain Split";
		case MoveName::Sacred_Fire: return "Sacred Fire";
		case MoveName::Magnitude: return "Magnitude";
		case MoveName::Dynamic_Punch: return "Dynamic Punch";
		case MoveName::Megahorn: return "Megahorn";
		case MoveName::Dragon_Breath: return "Dragon Breath";
		case MoveName::Baton_Pass: return "Baton Pass";
		case MoveName::Encore: return "Encore";
		case MoveName::Pursuit: return "Pursuit";
		case MoveName::Rapid_Spin: return "Rapid Spin";
		case MoveName::Sweet_Scent: return "Sweet Scent";
		case MoveName::Iron_Tail: return "Iron Tail";
		case MoveName::Metal_Claw: return "Metal Claw";
		case MoveName::Vital_Throw: return "Vital Throw";
		case MoveName::Morning_Sun: return "Morning Sun";
		case MoveName::Synthesis: return "Synthesis";
		case MoveName::Moonlight: return "Moonlight";
		case MoveName::Hidden_Power: return "Hidden Power";
		case MoveName::Cross_Chop: return "Cross Chop";
		case MoveName::Twister: return "Twister";
		case MoveName::Rain_Dance: return "Rain Dance";
		case MoveName::Sunny_Day: return "Sunny Day";
		case MoveName::Crunch: return "Crunch";
		case MoveName::Mirror_Coat: return "Mirror Coat";
		case MoveName::Psych_Up: return "Psych Up";
		case MoveName::Extreme_Speed: return "Extreme Speed";
		case MoveName::Ancient_Power: return "Ancient Power";
		case MoveName::Shadow_Ball: return "Shadow Ball";
		case MoveName::Future_Sight: return "Future Sight";
		case MoveName::Rock_Smash: return "Rock Smash";
		case MoveName::Whirlpool: return "Whirlpool";
		case MoveName::Beat_Up: return "Beat Up";
		case MoveName::Fake_Out: return "Fake Out";
		case MoveName::Uproar: return "Uproar";
		case MoveName::Stockpile: return "Stockpile";
		case MoveName::Spit_Up: return "Spit Up";
		case MoveName::Swallow: return "Swallow";
		case MoveName::Heat_Wave: return "Heat Wave";
		case MoveName::Hail: return "Hail";
		case MoveName::Torment: return "Torment";
		case MoveName::Flatter: return "Flatter";
		case MoveName::Will_O_Wisp: return "Will-O-Wisp";
		case MoveName::Memento: return "Memento";
		case MoveName::Facade: return "Facade";
		case MoveName::Focus_Punch: return "Focus Punch";
		case MoveName::Smelling_Salts: return "Smelling Salts";
		case MoveName::Follow_Me: return "Follow Me";
		case MoveName::Nature_Power: return "Nature Power";
		case MoveName::Charge: return "Charge";
		case MoveName::Taunt: return "Taunt";
		case MoveName::Helping_Hand: return "Helping Hand";
		case MoveName::Trick: return "Trick";
		case MoveName::Role_Play: return "Role Play";
		case MoveName::Wish: return "Wish";
		case MoveName::Assist: return "Assist";
		case MoveName::Ingrain: return "Ingrain";
		case MoveName::Superpower: return "Superpower";
		case MoveName::Magic_Coat: return "Magic Coat";
		case MoveName::Recycle: return "Recycle";
		case MoveName::Revenge: return "Revenge";
		case MoveName::Brick_Break: return "Brick Break";
		case MoveName::Yawn: return "Yawn";
		case MoveName::Knock_Off: return "Knock Off";
		case MoveName::Endeavor: return "Endeavor";
		case MoveName::Eruption: return "Eruption";
		case MoveName::Skill_Swap: return "Skill Swap";
		case MoveName::Imprison: return "Imprison";
		case MoveName::Refresh: return "Refresh";
		case MoveName::Grudge: return "Grudge";
		case MoveName::Snatch: return "Snatch";
		case MoveName::Secret_Power: return "Secret Power";
		case MoveName::Dive: return "Dive";
		case MoveName::Arm_Thrust: return "Arm Thrust";
		case MoveName::Camouflage: return "Camouflage";
		case MoveName::Tail_Glow: return "Tail Glow";
		case MoveName::Luster_Purge: return "Luster Purge";
		case MoveName::Mist_Ball: return "Mist Ball";
		case MoveName::Feather_Dance: return "Feather Dance";
		case MoveName::Teeter_Dance: return "Teeter Dance";
		case MoveName::Blaze_Kick: return "Blaze Kick";
		case MoveName::Mud_Sport: return "Mud Sport";
		case MoveName::Ice_Ball: return "Ice Ball";
		case MoveName::Needle_Arm: return "Needle Arm";
		case MoveName::Slack_Off: return "Slack Off";
		case MoveName::Hyper_Voice: return "Hyper Voice";
		case MoveName::Poison_Fang: return "Poison Fang";
		case MoveName::Crush_Claw: return "Crush Claw";
		case MoveName::Blast_Burn: return "Blast Burn";
		case MoveName::Hydro_Cannon: return "Hydro Cannon";
		case MoveName::Meteor_Mash: return "Meteor Mash";
		case MoveName::Astonish: return "Astonish";
		case MoveName::Weather_Ball: return "Weather Ball";
		case MoveName::Aromatherapy: return "Aromatherapy";
		case MoveName::Fake_Tears: return "Fake Tears";
		case MoveName::Air_Cutter: return "Air Cutter";
		case MoveName::Overheat: return "Overheat";
		case MoveName::Odor_Sleuth: return "Odor Sleuth";
		case MoveName::Rock_Tomb: return "Rock Tomb";
		case MoveName::Silver_Wind: return "Silver Wind";
		case MoveName::Metal_Sound: return "Metal Sound";
		case MoveName::Grass_Whistle: return "Grass Whistle";
		case MoveName::Tickle: return "Tickle";
		case MoveName::Cosmic_Power: return "Cosmic Power";
		case MoveName::Water_Spout: return "Water Spout";
		case MoveName::Signal_Beam: return "Signal Beam";
		case MoveName::Shadow_Punch: return "Shadow Punch";
		case MoveName::Extrasensory: return "Extrasensory";
		case MoveName::Sky_Uppercut: return "Sky Uppercut";
		case MoveName::Sand_Tomb: return "Sand Tomb";
		case MoveName::Sheer_Cold: return "Sheer Cold";
		case MoveName::Muddy_Water: return "Muddy Water";
		case MoveName::Bullet_Seed: return "Bullet Seed";
		case MoveName::Aerial_Ace: return "Aerial Ace";
		case MoveName::Icicle_Spear: return "Icicle Spear";
		case MoveName::Iron_Defense: return "Iron Defense";
		case MoveName::Block: return "Block";
		case MoveName::Howl: return "Howl";
		case MoveName::Dragon_Claw: return "Dragon Claw";
		case MoveName::Frenzy_Plant: return "Frenzy Plant";
		case MoveName::Bulk_Up: return "Bulk Up";
		case MoveName::Bounce: return "Bounce";
		case MoveName::Mud_Shot: return "Mud Shot";
		case MoveName::Poison_Tail: return "Poison Tail";
		case MoveName::Covet: return "Covet";
		case MoveName::Volt_Tackle: return "Volt Tackle";
		case MoveName::Magical_Leaf: return "Magical Leaf";
		case MoveName::Water_Sport: return "Water Sport";
		case MoveName::Calm_Mind: return "Calm Mind";
		case MoveName::Leaf_Blade: return "Leaf Blade";
		case MoveName::Dragon_Dance: return "Dragon Dance";
		case MoveName::Rock_Blast: return "Rock Blast";
		case MoveName::Shock_Wave: return "Shock Wave";
		case MoveName::Water_Pulse: return "Water Pulse";
		case MoveName::Doom_Desire: return "Doom Desire";
		case MoveName::Psycho_Boost: return "Psycho Boost";
		case MoveName::Roost: return "Roost";
		case MoveName::Gravity: return "Gravity";
		case MoveName::Miracle_Eye: return "Miracle Eye";
		case MoveName::Wake_Up_Slap: return "Wake-Up Slap";
		case MoveName::Hammer_Arm: return "Hammer Arm";
		case MoveName::Gyro_Ball: return "Gyro Ball";
		case MoveName::Healing_Wish: return "Healing Wish";
		case MoveName::Brine: return "Brine";
		case MoveName::Natural_Gift: return "Natural Gift";
		case MoveName::Feint: return "Feint";
		case MoveName::Pluck: return "Pluck";
		case MoveName::Tailwind: return "Tailwind";
		case MoveName::Acupressure: return "Acupressure";
		case MoveName::Metal_Burst: return "Metal Burst";
		case MoveName::U_turn: return "U-turn";
		case MoveName::Close_Combat: return "Close Combat";
		case MoveName::Payback: return "Payback";
		case MoveName::Assurance: return "Assurance";
		case MoveName::Embargo: return "Embargo";
		case MoveName::Fling: return "Fling";
		case MoveName::Psycho_Shift: return "Psycho Shift";
		case MoveName::Trump_Card: return "Trump Card";
		case MoveName::Heal_Block: return "Heal Block";
		case MoveName::Wring_Out: return "Wring Out";
		case MoveName::Power_Trick: return "Power Trick";
		case MoveName::Gastro_Acid: return "Gastro Acid";
		case MoveName::Lucky_Chant: return "Lucky Chant";
		case MoveName::Me_First: return "Me First";
		case MoveName::Copycat: return "Copycat";
		case MoveName::Power_Swap: return "Power Swap";
		case MoveName::Guard_Swap: return "Guard Swap";
		case MoveName::Punishment: return "Punishment";
		case MoveName::Last_Resort: return "Last Resort";
		case MoveName::Worry_Seed: return "Worry Seed";
		case MoveName::Sucker_Punch: return "Sucker Punch";
		case MoveName::Toxic_Spikes: return "Toxic Spikes";
		case MoveName::Heart_Swap: return "Heart Swap";
		case MoveName::Aqua_Ring: return "Aqua Ring";
		case MoveName::Magnet_Rise: return "Magnet Rise";
		case MoveName::Flare_Blitz: return "Flare Blitz";
		case MoveName::Force_Palm: return "Force Palm";
		case MoveName::Aura_Sphere: return "Aura Sphere";
		case MoveName::Rock_Polish: return "Rock Polish";
		case MoveName::Poison_Jab: return "Poison Jab";
		case MoveName::Dark_Pulse: return "Dark Pulse";
		case MoveName::Night_Slash: return "Night Slash";
		case MoveName::Aqua_Tail: return "Aqua Tail";
		case MoveName::Seed_Bomb: return "Seed Bomb";
		case MoveName::Air_Slash: return "Air Slash";
		case MoveName::X_Scissor: return "X-Scissor";
		case MoveName::Bug_Buzz: return "Bug Buzz";
		case MoveName::Dragon_Pulse: return "Dragon Pulse";
		case MoveName::Dragon_Rush: return "Dragon Rush";
		case MoveName::Power_Gem: return "Power Gem";
		case MoveName::Drain_Punch: return "Drain Punch";
		case MoveName::Vacuum_Wave: return "Vacuum Wave";
		case MoveName::Focus_Blast: return "Focus Blast";
		case MoveName::Energy_Ball: return "Energy Ball";
		case MoveName::Brave_Bird: return "Brave Bird";
		case MoveName::Earth_Power: return "Earth Power";
		case MoveName::Switcheroo: return "Switcheroo";
		case MoveName::Giga_Impact: return "Giga Impact";
		case MoveName::Nasty_Plot: return "Nasty Plot";
		case MoveName::Bullet_Punch: return "Bullet Punch";
		case MoveName::Avalanche: return "Avalanche";
		case MoveName::Ice_Shard: return "Ice Shard";
		case MoveName::Shadow_Claw: return "Shadow Claw";
		case MoveName::Thunder_Fang: return "Thunder Fang";
		case MoveName::Ice_Fang: return "Ice Fang";
		case MoveName::Fire_Fang: return "Fire Fang";
		case MoveName::Shadow_Sneak: return "Shadow Sneak";
		case MoveName::Mud_Bomb: return "Mud Bomb";
		case MoveName::Psycho_Cut: return "Psycho Cut";
		case MoveName::Zen_Headbutt: return "Zen Headbutt";
		case MoveName::Mirror_Shot: return "Mirror Shot";
		case MoveName::Flash_Cannon: return "Flash Cannon";
		case MoveName::Rock_Climb: return "Rock Climb";
		case MoveName::Defog: return "Defog";
		case MoveName::Trick_Room: return "Trick Room";
		case MoveName::Draco_Meteor: return "Draco Meteor";
		case MoveName::Discharge: return "Discharge";
		case MoveName::Lava_Plume: return "Lava Plume";
		case MoveName::Leaf_Storm: return "Leaf Storm";
		case MoveName::Power_Whip: return "Power Whip";
		case MoveName::Rock_Wrecker: return "Rock Wrecker";
		case MoveName::Cross_Poison: return "Cross Poison";
		case MoveName::Gunk_Shot: return "Gunk Shot";
		case MoveName::Iron_Head: return "Iron Head";
		case MoveName::Magnet_Bomb: return "Magnet Bomb";
		case MoveName::Stone_Edge: return "Stone Edge";
		case MoveName::Captivate: return "Captivate";
		case MoveName::Stealth_Rock: return "Stealth Rock";
		case MoveName::Grass_Knot: return "Grass Knot";
		case MoveName::Chatter: return "Chatter";
		case MoveName::Judgment: return "Judgment";
		case MoveName::Bug_Bite: return "Bug Bite";
		case MoveName::Charge_Beam: return "Charge Beam";
		case MoveName::Wood_Hammer: return "Wood Hammer";
		case MoveName::Aqua_Jet: return "Aqua Jet";
		case MoveName::Attack_Order: return "Attack Order";
		case MoveName::Defend_Order: return "Defend Order";
		case MoveName::Heal_Order: return "Heal Order";
		case MoveName::Head_Smash: return "Head Smash";
		case MoveName::Double_Hit: return "Double Hit";
		case MoveName::Roar_of_Time: return "Roar of Time";
		case MoveName::Spacial_Rend: return "Spacial Rend";
		case MoveName::Lunar_Dance: return "Lunar Dance";
		case MoveName::Crush_Grip: return "Crush Grip";
		case MoveName::Magma_Storm: return "Magma Storm";
		case MoveName::Dark_Void: return "Dark Void";
		case MoveName::Seed_Flare: return "Seed Flare";
		case MoveName::Ominous_Wind: return "Ominous Wind";
		case MoveName::Shadow_Force: return "Shadow Force";
		case MoveName::Hone_Claws: return "Hone Claws";
		case MoveName::Wide_Guard: return "Wide Guard";
		case MoveName::Guard_Split: return "Guard Split";
		case MoveName::Power_Split: return "Power Split";
		case MoveName::Wonder_Room: return "Wonder Room";
		case MoveName::Psyshock: return "Psyshock";
		case MoveName::Venoshock: return "Venoshock";
		case MoveName::Autotomize: return "Autotomize";
		case MoveName::Rage_Powder: return "Rage Powder";
		case MoveName::Telekinesis: return "Telekinesis";
		case MoveName::Magic_Room: return "Magic Room";
		case MoveName::Smack_Down: return "Smack Down";
		case MoveName::Storm_Throw: return "Storm Throw";
		case MoveName::Flame_Burst: return "Flame Burst";
		case MoveName::Sludge_Wave: return "Sludge Wave";
		case MoveName::Quiver_Dance: return "Quiver Dance";
		case MoveName::Heavy_Slam: return "Heavy Slam";
		case MoveName::Synchronoise: return "Synchronoise";
		case MoveName::Electro_Ball: return "Electro Ball";
		case MoveName::Soak: return "Soak";
		case MoveName::Flame_Charge: return "Flame Charge";
		case MoveName::Coil: return "Coil";
		case MoveName::Low_Sweep: return "Low Sweep";
		case MoveName::Acid_Spray: return "Acid Spray";
		case MoveName::Foul_Play: return "Foul Play";
		case MoveName::Simple_Beam: return "Simple Beam";
		case MoveName::Entrainment: return "Entrainment";
		case MoveName::After_You: return "After You";
		case MoveName::Round: return "Round";
		case MoveName::Echoed_Voice: return "Echoed Voice";
		case MoveName::Chip_Away: return "Chip Away";
		case MoveName::Clear_Smog: return "Clear Smog";
		case MoveName::Stored_Power: return "Stored Power";
		case MoveName::Quick_Guard: return "Quick Guard";
		case MoveName::Ally_Switch: return "Ally Switch";
		case MoveName::Scald: return "Scald";
		case MoveName::Shell_Smash: return "Shell Smash";
		case MoveName::Heal_Pulse: return "Heal Pulse";
		case MoveName::Hex: return "Hex";
		case MoveName::Sky_Drop: return "Sky Drop";
		case MoveName::Shift_Gear: return "Shift Gear";
		case MoveName::Circle_Throw: return "Circle Throw";
		case MoveName::Incinerate: return "Incinerate";
		case MoveName::Quash: return "Quash";
		case MoveName::Acrobatics: return "Acrobatics";
		case MoveName::Reflect_Type: return "Reflect Type";
		case MoveName::Retaliate: return "Retaliate";
		case MoveName::Final_Gambit: return "Final Gambit";
		case MoveName::Bestow: return "Bestow";
		case MoveName::Inferno: return "Inferno";
		case MoveName::Water_Pledge: return "Water Pledge";
		case MoveName::Fire_Pledge: return "Fire Pledge";
		case MoveName::Grass_Pledge: return "Grass Pledge";
		case MoveName::Volt_Switch: return "Volt Switch";
		case MoveName::Struggle_Bug: return "Struggle Bug";
		case MoveName::Bulldoze: return "Bulldoze";
		case MoveName::Frost_Breath: return "Frost Breath";
		case MoveName::Dragon_Tail: return "Dragon Tail";
		case MoveName::Work_Up: return "Work Up";
		case MoveName::Electroweb: return "Electroweb";
		case MoveName::Wild_Charge: return "Wild Charge";
		case MoveName::Drill_Run: return "Drill Run";
		case MoveName::Dual_Chop: return "Dual Chop";
		case MoveName::Heart_Stamp: return "Heart Stamp";
		case MoveName::Horn_Leech: return "Horn Leech";
		case MoveName::Sacred_Sword: return "Sacred Sword";
		case MoveName::Razor_Shell: return "Razor Shell";
		case MoveName::Heat_Crash: return "Heat Crash";
		case MoveName::Leaf_Tornado: return "Leaf Tornado";
		case MoveName::Steamroller: return "Steamroller";
		case MoveName::Cotton_Guard: return "Cotton Guard";
		case MoveName::Night_Daze: return "Night Daze";
		case MoveName::Psystrike: return "Psystrike";
		case MoveName::Tail_Slap: return "Tail Slap";
		case MoveName::Hurricane: return "Hurricane";
		case MoveName::Head_Charge: return "Head Charge";
		case MoveName::Gear_Grind: return "Gear Grind";
		case MoveName::Searing_Shot: return "Searing Shot";
		case MoveName::Techno_Blast: return "Techno Blast";
		case MoveName::Relic_Song: return "Relic Song";
		case MoveName::Secret_Sword: return "Secret Sword";
		case MoveName::Glaciate: return "Glaciate";
		case MoveName::Bolt_Strike: return "Bolt Strike";
		case MoveName::Blue_Flare: return "Blue Flare";
		case MoveName::Fiery_Dance: return "Fiery Dance";
		case MoveName::Freeze_Shock: return "Freeze Shock";
		case MoveName::Ice_Burn: return "Ice Burn";
		case MoveName::Snarl: return "Snarl";
		case MoveName::Icicle_Crash: return "Icicle Crash";
		case MoveName::V_create: return "V-create";
		case MoveName::Fusion_Flare: return "Fusion Flare";
		case MoveName::Fusion_Bolt: return "Fusion Bolt";
		case MoveName::Flying_Press: return "Flying Press";
		case MoveName::Mat_Block: return "Mat Block";
		case MoveName::Belch: return "Belch";
		case MoveName::Rototiller: return "Rototiller";
		case MoveName::Sticky_Web: return "Sticky Web";
		case MoveName::Fell_Stinger: return "Fell Stinger";
		case MoveName::Phantom_Force: return "Phantom Force";
		case MoveName::Trick_or_Treat: return "Trick-or-Treat";
		case MoveName::Noble_Roar: return "Noble Roar";
		case MoveName::Ion_Deluge: return "Ion Deluge";
		case MoveName::Parabolic_Charge: return "Parabolic Charge";
		case MoveName::Forests_Curse: return "Forest's Curse";
		case MoveName::Petal_Blizzard: return "Petal Blizzard";
		case MoveName::Freeze_Dry: return "Freeze-Dry";
		case MoveName::Disarming_Voice: return "Disarming Voice";
		case MoveName::Parting_Shot: return "Parting Shot";
		case MoveName::Topsy_Turvy: return "Topsy-Turvy";
		case MoveName::Draining_Kiss: return "Draining Kiss";
		case MoveName::Crafty_Shield: return "Crafty Shield";
		case MoveName::Flower_Shield: return "Flower Shield";
		case MoveName::Grassy_Terrain: return "Grassy Terrain";
		case MoveName::Misty_Terrain: return "Misty Terrain";
		case MoveName::Electrify: return "Electrify";
		case MoveName::Play_Rough: return "Play Rough";
		case MoveName::Fairy_Wind: return "Fairy Wind";
		case MoveName::Moonblast: return "Moonblast";
		case MoveName::Boomburst: return "Boomburst";
		case MoveName::Fairy_Lock: return "Fairy Lock";
		case MoveName::Kings_Shield: return "King's Shield";
		case MoveName::Play_Nice: return "Play Nice";
		case MoveName::Confide: return "Confide";
		case MoveName::Diamond_Storm: return "Diamond Storm";
		case MoveName::Steam_Eruption: return "Steam Eruption";
		case MoveName::Hyperspace_Hole: return "Hyperspace Hole";
		case MoveName::Water_Shuriken: return "Water Shuriken";
		case MoveName::Mystical_Fire: return "Mystical Fire";
		case MoveName::Spiky_Shield: return "Spiky Shield";
		case MoveName::Aromatic_Mist: return "Aromatic Mist";
		case MoveName::Eerie_Impulse: return "Eerie Impulse";
		case MoveName::Venom_Drench: return "Venom Drench";
		case MoveName::Powder: return "Powder";
		case MoveName::Geomancy: return "Geomancy";
		case MoveName::Magnetic_Flux: return "Magnetic Flux";
		case MoveName::Happy_Hour: return "Happy Hour";
		case MoveName::Electric_Terrain: return "Electric Terrain";
		case MoveName::Dazzling_Gleam: return "Dazzling Gleam";
		case MoveName::Celebrate: return "Celebrate";
		case MoveName::Hold_Hands: return "Hold Hands";
		case MoveName::Baby_Doll_Eyes: return "Baby-Doll Eyes";
		case MoveName::Nuzzle: return "Nuzzle";
		case MoveName::Hold_Back: return "Hold Back";
		case MoveName::Infestation: return "Infestation";
		case MoveName::Power_Up_Punch: return "Power-Up Punch";
		case MoveName::Oblivion_Wing: return "Oblivion Wing";
		case MoveName::Thousand_Arrows: return "Thousand Arrows";
		case MoveName::Thousand_Waves: return "Thousand Waves";
		case MoveName::Lands_Wrath: return "Land's Wrath";
		case MoveName::Light_of_Ruin: return "Light of Ruin";
		case MoveName::Origin_Pulse: return "Origin Pulse";
		case MoveName::Precipice_Blades: return "Precipice Blades";
		case MoveName::Dragon_Ascent: return "Dragon Ascent";
		case MoveName::Hyperspace_Fury: return "Hyperspace Fury";
		case MoveName::Breakneck_Blitz: return "Breakneck Blitz";
		case MoveName::All_Out_Pummeling: return "All-Out Pummeling";
		case MoveName::Supersonic_Skystrike: return "Supersonic Skystrike";
		case MoveName::Acid_Downpour: return "Acid Downpour";
		case MoveName::Tectonic_Rage: return "Tectonic Rage";
		case MoveName::Continental_Crush: return "Continental Crush";
		case MoveName::Savage_Spin_Out: return "Savage Spin-Out";
		case MoveName::Never_Ending_Nightmare: return "Never-Ending Nightmare";
		case MoveName::Corkscrew_Crash: return "Corkscrew Crash";
		case MoveName::Inferno_Overdrive: return "Inferno Overdrive";
		case MoveName::Hydro_Vortex: return "Hydro Vortex";
		case MoveName::Bloom_Doom: return "Bloom Doom";
		case MoveName::Gigavolt_Havoc: return "Gigavolt Havoc";
		case MoveName::Shattered_Psyche: return "Shattered Psyche";
		case MoveName::Subzero_Slammer: return "Subzero Slammer";
		case MoveName::Devastating_Drake: return "Devastating Drake";
		case MoveName::Black_Hole_Eclipse: return "Black Hole Eclipse";
		case MoveName::Twinkle_Tackle: return "Twinkle Tackle";
		case MoveName::Catastropika: return "Catastropika";
		case MoveName::Shore_Up: return "Shore Up";
		case MoveName::First_Impression: return "First Impression";
		case MoveName::Baneful_Bunker: return "Baneful Bunker";
		case MoveName::Spirit_Shackle: return "Spirit Shackle";
		case MoveName::Darkest_Lariat: return "Darkest Lariat";
		case MoveName::Sparkling_Aria: return "Sparkling Aria";
		case MoveName::Ice_Hammer: return "Ice Hammer";
		case MoveName::Floral_Healing: return "Floral Healing";
		case MoveName::High_Horsepower: return "High Horsepower";
		case MoveName::Strength_Sap: return "Strength Sap";
		case MoveName::Solar_Blade: return "Solar Blade";
		case MoveName::Leafage: return "Leafage";
		case MoveName::Spotlight: return "Spotlight";
		case MoveName::Toxic_Thread: return "Toxic Thread";
		case MoveName::Laser_Focus: return "Laser Focus";
		case MoveName::Gear_Up: return "Gear Up";
		case MoveName::Throat_Chop: return "Throat Chop";
		case MoveName::Pollen_Puff: return "Pollen Puff";
		case MoveName::Anchor_Shot: return "Anchor Shot";
		case MoveName::Psychic_Terrain: return "Psychic Terrain";
		case MoveName::Lunge: return "Lunge";
		case MoveName::Fire_Lash: return "Fire Lash";
		case MoveName::Power_Trip: return "Power Trip";
		case MoveName::Burn_Up: return "Burn Up";
		case MoveName::Speed_Swap: return "Speed Swap";
		case MoveName::Smart_Strike: return "Smart Strike";
		case MoveName::Purify: return "Purify";
		case MoveName::Revelation_Dance: return "Revelation Dance";
		case MoveName::Core_Enforcer: return "Core Enforcer";
		case MoveName::Trop_Kick: return "Trop Kick";
		case MoveName::Instruct: return "Instruct";
		case MoveName::Beak_Blast: return "Beak Blast";
		case MoveName::Clanging_Scales: return "Clanging Scales";
		case MoveName::Dragon_Hammer: return "Dragon Hammer";
		case MoveName::Brutal_Swing: return "Brutal Swing";
		case MoveName::Aurora_Veil: return "Aurora Veil";
		case MoveName::Sinister_Arrow_Raid: return "Sinister Arrow Raid";
		case MoveName::Malicious_Moonsault: return "Malicious Moonsault";
		case MoveName::Oceanic_Operetta: return "Oceanic Operetta";
		case MoveName::Guardian_of_Alola: return "Guardian of Alola";
		case MoveName::Soul_Stealing_7_Star_Strike: return "Soul-Stealing 7-Star Strike";
		case MoveName::Stoked_Sparksurfer: return "Stoked Sparksurfer";
		case MoveName::Pulverizing_Pancake: return "Pulverizing Pancake";
		case MoveName::Extreme_Evoboost: return "Extreme Evoboost";
		case MoveName::Genesis_Supernova: return "Genesis Supernova";
		case MoveName::Shell_Trap: return "Shell Trap";
		case MoveName::Fleur_Cannon: return "Fleur Cannon";
		case MoveName::Psychic_Fangs: return "Psychic Fangs";
		case MoveName::Stomping_Tantrum: return "Stomping Tantrum";
		case MoveName::Shadow_Bone: return "Shadow Bone";
		case MoveName::Accelerock: return "Accelerock";
		case MoveName::Liquidation: return "Liquidation";
		case MoveName::Prismatic_Laser: return "Prismatic Laser";
		case MoveName::Spectral_Thief: return "Spectral Thief";
		case MoveName::Sunsteel_Strike: return "Sunsteel Strike";
		case MoveName::Moongeist_Beam: return "Moongeist Beam";
		case MoveName::Tearful_Look: return "Tearful Look";
		case MoveName::Zing_Zap: return "Zing Zap";
		case MoveName::Natures_Madness: return "Nature's Madness";
		case MoveName::Multi_Attack: return "Multi-Attack";
		case MoveName::m10000000_Volt_Thunderbolt: return "10,000,000 Volt Thunderbolt";
		case MoveName::Mind_Blown: return "Mind Blown";
		case MoveName::Plasma_Fists: return "Plasma Fists";
		case MoveName::Photon_Geyser: return "Photon Geyser";
		case MoveName::Light_That_Burns_the_Sky: return "Light That Burns the Sky";
		case MoveName::Searing_Sunraze_Smash: return "Searing Sunraze Smash";
		case MoveName::Menacing_Moonraze_Maelstrom: return "Menacing Moonraze Maelstrom";
		case MoveName::Lets_Snuggle_Forever: return "Let's Snuggle Forever";
		case MoveName::Splintered_Stormshards: return "Splintered Stormshards";
		case MoveName::Clangorous_Soulblaze: return "Clangorous Soulblaze";
		case MoveName::Double_Iron_Bash: return "Double Iron Bash";
		case MoveName::Max_Guard: return "Max Guard";
		case MoveName::Dynamax_Cannon: return "Dynamax Cannon";
		case MoveName::Snipe_Shot: return "Snipe Shot";
		case MoveName::Jaw_Lock: return "Jaw Lock";
		case MoveName::Stuff_Cheeks: return "Stuff Cheeks";
		case MoveName::No_Retreat: return "No Retreat";
		case MoveName::Tar_Shot: return "Tar Shot";
		case MoveName::Magic_Powder: return "Magic Powder";
		case MoveName::Dragon_Darts: return "Dragon Darts";
		case MoveName::Teatime: return "Teatime";
		case MoveName::Octolock: return "Octolock";
		case MoveName::Bolt_Beak: return "Bolt Beak";
		case MoveName::Fishious_Rend: return "Fishious Rend";
		case MoveName::Court_Change: return "Court Change";
		case MoveName::Max_Flare: return "Max Flare";
		case MoveName::Max_Flutterby: return "Max Flutterby";
		case MoveName::Max_Lightning: return "Max Lightning";
		case MoveName::Max_Strike: return "Max Strike";
		case MoveName::Max_Knuckle: return "Max Knuckle";
		case MoveName::Max_Phantasm: return "Max Phantasm";
		case MoveName::Max_Hailstorm: return "Max Hailstorm";
		case MoveName::Max_Ooze: return "Max Ooze";
		case MoveName::Max_Geyser: return "Max Geyser";
		case MoveName::Max_Airstream: return "Max Airstream";
		case MoveName::Max_Starfall: return "Max Starfall";
		case MoveName::Max_Wyrmwind: return "Max Wyrmwind";
		case MoveName::Max_Mindstorm: return "Max Mindstorm";
		case MoveName::Max_Rockfall: return "Max Rockfall";
		case MoveName::Max_Quake: return "Max Quake";
		case MoveName::Max_Darkness: return "Max Darkness";
		case MoveName::Max_Overgrowth: return "Max Overgrowth";
		case MoveName::Max_Steelspike: return "Max Steelspike";
		case MoveName::Clangorous_Soul: return "Clangorous Soul";
		case MoveName::Body_Press: return "Body Press";
		case MoveName::Decorate: return "Decorate";
		case MoveName::Drum_Beating: return "Drum Beating";
		case MoveName::Snap_Trap: return "Snap Trap";
		case MoveName::Pyro_Ball: return "Pyro Ball";
		case MoveName::Behemoth_Blade: return "Behemoth Blade";
		case MoveName::Behemoth_Bash: return "Behemoth Bash";
		case MoveName::Aura_Wheel: return "Aura Wheel";
		case MoveName::Breaking_Swipe: return "Breaking Swipe";
		case MoveName::Branch_Poke: return "Branch Poke";
		case MoveName::Overdrive: return "Overdrive";
		case MoveName::Apple_Acid: return "Apple Acid";
		case MoveName::Grav_Apple: return "Grav Apple";
		case MoveName::Spirit_Break: return "Spirit Break";
		case MoveName::Strange_Steam: return "Strange Steam";
		case MoveName::Life_Dew: return "Life Dew";
		case MoveName::Obstruct: return "Obstruct";
		case MoveName::False_Surrender: return "False Surrender";
		case MoveName::Meteor_Assault: return "Meteor Assault";
		case MoveName::Eternabeam: return "Eternabeam";
		case MoveName::Steel_Beam: return "Steel Beam";
		case MoveName::Expanding_Force: return "Expanding Force";
		case MoveName::Steel_Roller: return "Steel Roller";
		case MoveName::Scale_Shot: return "Scale Shot";
		case MoveName::Meteor_Beam: return "Meteor Beam";
		case MoveName::Shell_Side_Arm: return "Shell Side Arm";
		case MoveName::Misty_Explosion: return "Misty Explosion";
		case MoveName::Grassy_Glide: return "Grassy Glide";
		case MoveName::Rising_Voltage: return "Rising Voltage";
		case MoveName::Terrain_Pulse: return "Terrain Pulse";
		case MoveName::Skitter_Smack: return "Skitter Smack";
		case MoveName::Burning_Jealousy: return "Burning Jealousy";
		case MoveName::Lash_Out: return "Lash Out";
		case MoveName::Poltergeist: return "Poltergeist";
		case MoveName::Corrosive_Gas: return "Corrosive Gas";
		case MoveName::Coaching: return "Coaching";
		case MoveName::Flip_Turn: return "Flip Turn";
		case MoveName::Triple_Axel: return "Triple Axel";
		case MoveName::Dual_Wingbeat: return "Dual Wingbeat";
		case MoveName::Scorching_Sands: return "Scorching Sands";
		case MoveName::Jungle_Healing: return "Jungle Healing";
		case MoveName::Wicked_Blow: return "Wicked Blow";
		case MoveName::Surging_Strikes: return "Surging Strikes";
		case MoveName::Thunder_Cage: return "Thunder Cage";
		case MoveName::Dragon_Energy: return "Dragon Energy";
		case MoveName::Freezing_Glare: return "Freezing Glare";
		case MoveName::Fiery_Wrath: return "Fiery Wrath";
		case MoveName::Thunderous_Kick: return "Thunderous Kick";
		case MoveName::Glacial_Lance: return "Glacial Lance";
		case MoveName::Astral_Barrage: return "Astral Barrage";
		case MoveName::Eerie_Spell: return "Eerie Spell";
		case MoveName::Tera_Blast: return "Tera Blast";
		case MoveName::Silk_Trap: return "Silk Trap";
		case MoveName::Axe_Kick: return "Axe Kick";
		case MoveName::Last_Respects: return "Last Respects";
		case MoveName::Lumina_Crash: return "Lumina Crash";
		case MoveName::Order_Up: return "Order Up";
		case MoveName::Jet_Punch: return "Jet Punch";
		case MoveName::Spicy_Extract: return "Spicy Extract";
		case MoveName::Spin_Out: return "Spin Out";
		case MoveName::Population_Bomb: return "Population Bomb";
		case MoveName::Ice_Spinner: return "Ice Spinner";
		case MoveName::Glaive_Rush: return "Glaive Rush";
		case MoveName::Revival_Blessing: return "Revival Blessing";
		case MoveName::Salt_Cure: return "Salt Cure";
		case MoveName::Triple_Dive: return "Triple Dive";
		case MoveName::Mortal_Spin: return "Mortal Spin";
		case MoveName::Doodle: return "Doodle";
		case MoveName::Fillet_Away: return "Fillet Away";
		case MoveName::Kowtow_Cleave: return "Kowtow Cleave";
		case MoveName::Flower_Trick: return "Flower Trick";
		case MoveName::Torch_Song: return "Torch Song";
		case MoveName::Aqua_Step: return "Aqua Step";
		case MoveName::Raging_Bull: return "Raging Bull";
		case MoveName::Make_It_Rain: return "Make It Rain";
		case MoveName::Psyblade: return "Psyblade";
		case MoveName::Hydro_Steam: return "Hydro Steam";
		case MoveName::Ruination: return "Ruination";
		case MoveName::Collision_Course: return "Collision Course";
		case MoveName::Electro_Drift: return "Electro Drift";
		case MoveName::Shed_Tail: return "Shed Tail";
		case MoveName::Chilly_Reception: return "Chilly Reception";
		case MoveName::Tidy_Up: return "Tidy Up";
		case MoveName::Snowscape: return "Snowscape";
		case MoveName::Pounce: return "Pounce";
		case MoveName::Trailblaze: return "Trailblaze";
		case MoveName::Chilling_Water: return "Chilling Water";
		case MoveName::Hyper_Drill: return "Hyper Drill";
		case MoveName::Twin_Beam: return "Twin Beam";
		case MoveName::Rage_Fist: return "Rage Fist";
		case MoveName::Armor_Cannon: return "Armor Cannon";
		case MoveName::Bitter_Blade: return "Bitter Blade";
		case MoveName::Double_Shock: return "Double Shock";
		case MoveName::Gigaton_Hammer: return "Gigaton Hammer";
		case MoveName::Comeuppance: return "Comeuppance";
		case MoveName::Aqua_Cutter: return "Aqua Cutter";
		case MoveName::Blazing_Torque: return "Blazing Torque";
		case MoveName::Wicked_Torque: return "Wicked Torque";
		case MoveName::Noxious_Torque: return "Noxious Torque";
		case MoveName::Combat_Torque: return "Combat Torque";
		case MoveName::Magical_Torque: return "Magical Torque";
		case MoveName::Blood_Moon: return "Blood Moon";
		case MoveName::Matcha_Gotcha: return "Matcha Gotcha";
		case MoveName::Syrup_Bomb: return "Syrup Bomb";
		case MoveName::Ivy_Cudgel: return "Ivy Cudgel";
		case MoveName::Electro_Shot: return "Electro Shot";
		case MoveName::Thunderclap: return "Thunderclap";
		case MoveName::Tachyon_Cutter: return "Tachyon Cutter";
		case MoveName::Upper_Hand: return "Upper Hand";
		case MoveName::Psychic_Noise: return "Psychic Noise";
	}
}

export template<>
constexpr auto from_string(std::string_view const str) -> MoveName {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<std::string_view, MoveName>>({
			{"10000000voltthunderbolt", MoveName::m10000000_Volt_Thunderbolt},
			{"absorb", MoveName::Absorb},
			{"accelerock", MoveName::Accelerock},
			{"acid", MoveName::Acid},
			{"acidarmor", MoveName::Acid_Armor},
			{"aciddownpour", MoveName::Acid_Downpour},
			{"acidspray", MoveName::Acid_Spray},
			{"acrobatics", MoveName::Acrobatics},
			{"acupressure", MoveName::Acupressure},
			{"aerialace", MoveName::Aerial_Ace},
			{"aeroblast", MoveName::Aeroblast},
			{"afteryou", MoveName::After_You},
			{"agility", MoveName::Agility},
			{"aircutter", MoveName::Air_Cutter},
			{"airslash", MoveName::Air_Slash},
			{"alloutpummeling", MoveName::All_Out_Pummeling},
			{"allyswitch", MoveName::Ally_Switch},
			{"amnesia", MoveName::Amnesia},
			{"anchorshot", MoveName::Anchor_Shot},
			{"ancientpower", MoveName::Ancient_Power},
			{"appleacid", MoveName::Apple_Acid},
			{"aquacutter", MoveName::Aqua_Cutter},
			{"aquajet", MoveName::Aqua_Jet},
			{"aquaring", MoveName::Aqua_Ring},
			{"aquastep", MoveName::Aqua_Step},
			{"aquatail", MoveName::Aqua_Tail},
			{"armorcannon", MoveName::Armor_Cannon},
			{"armthrust", MoveName::Arm_Thrust},
			{"aromatherapy", MoveName::Aromatherapy},
			{"aromaticmist", MoveName::Aromatic_Mist},
			{"assist", MoveName::Assist},
			{"assurance", MoveName::Assurance},
			{"astonish", MoveName::Astonish},
			{"astralbarrage", MoveName::Astral_Barrage},
			{"attackorder", MoveName::Attack_Order},
			{"attract", MoveName::Attract},
			{"aurasphere", MoveName::Aura_Sphere},
			{"aurawheel", MoveName::Aura_Wheel},
			{"aurorabeam", MoveName::Aurora_Beam},
			{"auroraveil", MoveName::Aurora_Veil},
			{"autotomize", MoveName::Autotomize},
			{"avalanche", MoveName::Avalanche},
			{"axekick", MoveName::Axe_Kick},
			{"babydolleyes", MoveName::Baby_Doll_Eyes},
			{"banefulbunker", MoveName::Baneful_Bunker},
			{"barrage", MoveName::Barrage},
			{"barrier", MoveName::Barrier},
			{"batonpass", MoveName::Baton_Pass},
			{"beakblast", MoveName::Beak_Blast},
			{"beatup", MoveName::Beat_Up},
			{"behemothbash", MoveName::Behemoth_Bash},
			{"behemothblade", MoveName::Behemoth_Blade},
			{"belch", MoveName::Belch},
			{"bellydrum", MoveName::Belly_Drum},
			{"bestow", MoveName::Bestow},
			{"bide", MoveName::Bide},
			{"bind", MoveName::Bind},
			{"bite", MoveName::Bite},
			{"bitterblade", MoveName::Bitter_Blade},
			{"blackholeeclipse", MoveName::Black_Hole_Eclipse},
			{"blastburn", MoveName::Blast_Burn},
			{"blazekick", MoveName::Blaze_Kick},
			{"blazingtorque", MoveName::Blazing_Torque},
			{"blizzard", MoveName::Blizzard},
			{"block", MoveName::Block},
			{"bloodmoon", MoveName::Blood_Moon},
			{"bloomdoom", MoveName::Bloom_Doom},
			{"blueflare", MoveName::Blue_Flare},
			{"bodypress", MoveName::Body_Press},
			{"bodyslam", MoveName::Body_Slam},
			{"boltbeak", MoveName::Bolt_Beak},
			{"boltstrike", MoveName::Bolt_Strike},
			{"boneclub", MoveName::Bone_Club},
			{"bonemerang", MoveName::Bonemerang},
			{"bonerush", MoveName::Bone_Rush},
			{"boomburst", MoveName::Boomburst},
			{"bounce", MoveName::Bounce},
			{"branchpoke", MoveName::Branch_Poke},
			{"bravebird", MoveName::Brave_Bird},
			{"breakingswipe", MoveName::Breaking_Swipe},
			{"breakneckblitz", MoveName::Breakneck_Blitz},
			{"brickbreak", MoveName::Brick_Break},
			{"brine", MoveName::Brine},
			{"brutalswing", MoveName::Brutal_Swing},
			{"bubble", MoveName::Bubble},
			{"bubblebeam", MoveName::Bubble_Beam},
			{"bugbite", MoveName::Bug_Bite},
			{"bugbuzz", MoveName::Bug_Buzz},
			{"bulkup", MoveName::Bulk_Up},
			{"bulldoze", MoveName::Bulldoze},
			{"bulletpunch", MoveName::Bullet_Punch},
			{"bulletseed", MoveName::Bullet_Seed},
			{"burningjealousy", MoveName::Burning_Jealousy},
			{"burnup", MoveName::Burn_Up},
			{"calmmind", MoveName::Calm_Mind},
			{"camouflage", MoveName::Camouflage},
			{"captivate", MoveName::Captivate},
			{"catastropika", MoveName::Catastropika},
			{"celebrate", MoveName::Celebrate},
			{"charge", MoveName::Charge},
			{"chargebeam", MoveName::Charge_Beam},
			{"charm", MoveName::Charm},
			{"chatter", MoveName::Chatter},
			{"chillingwater", MoveName::Chilling_Water},
			{"chillyreception", MoveName::Chilly_Reception},
			{"chipaway", MoveName::Chip_Away},
			{"circlethrow", MoveName::Circle_Throw},
			{"clamp", MoveName::Clamp},
			{"clangingscales", MoveName::Clanging_Scales},
			{"clangoroussoul", MoveName::Clangorous_Soul},
			{"clangoroussoulblaze", MoveName::Clangorous_Soulblaze},
			{"clearsmog", MoveName::Clear_Smog},
			{"closecombat", MoveName::Close_Combat},
			{"coaching", MoveName::Coaching},
			{"coil", MoveName::Coil},
			{"collisioncourse", MoveName::Collision_Course},
			{"combattorque", MoveName::Combat_Torque},
			{"cometpunch", MoveName::Comet_Punch},
			{"comeuppance", MoveName::Comeuppance},
			{"confide", MoveName::Confide},
			{"confuseray", MoveName::Confuse_Ray},
			{"confusion", MoveName::Confusion},
			{"constrict", MoveName::Constrict},
			{"continentalcrush", MoveName::Continental_Crush},
			{"conversion", MoveName::Conversion},
			{"conversion2", MoveName::Conversion_2},
			{"copycat", MoveName::Copycat},
			{"coreenforcer", MoveName::Core_Enforcer},
			{"corkscrewcrash", MoveName::Corkscrew_Crash},
			{"corrosivegas", MoveName::Corrosive_Gas},
			{"cosmicpower", MoveName::Cosmic_Power},
			{"cottonguard", MoveName::Cotton_Guard},
			{"cottonspore", MoveName::Cotton_Spore},
			{"counter", MoveName::Counter},
			{"courtchange", MoveName::Court_Change},
			{"covet", MoveName::Covet},
			{"crabhammer", MoveName::Crabhammer},
			{"craftyshield", MoveName::Crafty_Shield},
			{"crosschop", MoveName::Cross_Chop},
			{"crosspoison", MoveName::Cross_Poison},
			{"crunch", MoveName::Crunch},
			{"crushclaw", MoveName::Crush_Claw},
			{"crushgrip", MoveName::Crush_Grip},
			{"curse", MoveName::Curse},
			{"cut", MoveName::Cut},
			{"darkestlariat", MoveName::Darkest_Lariat},
			{"darkpulse", MoveName::Dark_Pulse},
			{"darkvoid", MoveName::Dark_Void},
			{"dazzlinggleam", MoveName::Dazzling_Gleam},
			{"decorate", MoveName::Decorate},
			{"defendorder", MoveName::Defend_Order},
			{"defensecurl", MoveName::Defense_Curl},
			{"defog", MoveName::Defog},
			{"destinybond", MoveName::Destiny_Bond},
			{"detect", MoveName::Detect},
			{"devastatingdrake", MoveName::Devastating_Drake},
			{"diamondstorm", MoveName::Diamond_Storm},
			{"dig", MoveName::Dig},
			{"disable", MoveName::Disable},
			{"disarmingvoice", MoveName::Disarming_Voice},
			{"discharge", MoveName::Discharge},
			{"dive", MoveName::Dive},
			{"dizzypunch", MoveName::Dizzy_Punch},
			{"doodle", MoveName::Doodle},
			{"doomdesire", MoveName::Doom_Desire},
			{"doubleedge", MoveName::Double_Edge},
			{"doublehit", MoveName::Double_Hit},
			{"doubleironbash", MoveName::Double_Iron_Bash},
			{"doublekick", MoveName::Double_Kick},
			{"doubleshock", MoveName::Double_Shock},
			{"doubleslap", MoveName::Double_Slap},
			{"doubleteam", MoveName::Double_Team},
			{"dracometeor", MoveName::Draco_Meteor},
			{"dragonascent", MoveName::Dragon_Ascent},
			{"dragonbreath", MoveName::Dragon_Breath},
			{"dragonclaw", MoveName::Dragon_Claw},
			{"dragondance", MoveName::Dragon_Dance},
			{"dragondarts", MoveName::Dragon_Darts},
			{"dragonenergy", MoveName::Dragon_Energy},
			{"dragonhammer", MoveName::Dragon_Hammer},
			{"dragonpulse", MoveName::Dragon_Pulse},
			{"dragonrage", MoveName::Dragon_Rage},
			{"dragonrush", MoveName::Dragon_Rush},
			{"dragontail", MoveName::Dragon_Tail},
			{"drainingkiss", MoveName::Draining_Kiss},
			{"drainpunch", MoveName::Drain_Punch},
			{"dreameater", MoveName::Dream_Eater},
			{"drillpeck", MoveName::Drill_Peck},
			{"drillrun", MoveName::Drill_Run},
			{"drumbeating", MoveName::Drum_Beating},
			{"dualchop", MoveName::Dual_Chop},
			{"dualwingbeat", MoveName::Dual_Wingbeat},
			{"dynamaxcannon", MoveName::Dynamax_Cannon},
			{"dynamicpunch", MoveName::Dynamic_Punch},
			{"earthpower", MoveName::Earth_Power},
			{"earthquake", MoveName::Earthquake},
			{"echoedvoice", MoveName::Echoed_Voice},
			{"eerieimpulse", MoveName::Eerie_Impulse},
			{"eeriespell", MoveName::Eerie_Spell},
			{"eggbomb", MoveName::Egg_Bomb},
			{"electricterrain", MoveName::Electric_Terrain},
			{"electrify", MoveName::Electrify},
			{"electroball", MoveName::Electro_Ball},
			{"electrodrift", MoveName::Electro_Drift},
			{"electroshot", MoveName::Electro_Shot},
			{"electroweb", MoveName::Electroweb},
			{"embargo", MoveName::Embargo},
			{"ember", MoveName::Ember},
			{"encore", MoveName::Encore},
			{"endeavor", MoveName::Endeavor},
			{"endure", MoveName::Endure},
			{"energyball", MoveName::Energy_Ball},
			{"entrainment", MoveName::Entrainment},
			{"eruption", MoveName::Eruption},
			{"eternabeam", MoveName::Eternabeam},
			{"expandingforce", MoveName::Expanding_Force},
			{"explosion", MoveName::Explosion},
			{"extrasensory", MoveName::Extrasensory},
			{"extremeevoboost", MoveName::Extreme_Evoboost},
			{"extremespeed", MoveName::Extreme_Speed},
			{"facade", MoveName::Facade},
			{"faintattack", MoveName::Feint_Attack},
			{"fairylock", MoveName::Fairy_Lock},
			{"fairywind", MoveName::Fairy_Wind},
			{"fakeout", MoveName::Fake_Out},
			{"faketears", MoveName::Fake_Tears},
			{"falsesurrender", MoveName::False_Surrender},
			{"falseswipe", MoveName::False_Swipe},
			{"featherdance", MoveName::Feather_Dance},
			{"feint", MoveName::Feint},
			{"feintattack", MoveName::Feint_Attack},
			{"fellstinger", MoveName::Fell_Stinger},
			{"fierydance", MoveName::Fiery_Dance},
			{"fierywrath", MoveName::Fiery_Wrath},
			{"filletaway", MoveName::Fillet_Away},
			{"finalgambit", MoveName::Final_Gambit},
			{"fireblast", MoveName::Fire_Blast},
			{"firefang", MoveName::Fire_Fang},
			{"firelash", MoveName::Fire_Lash},
			{"firepledge", MoveName::Fire_Pledge},
			{"firepunch", MoveName::Fire_Punch},
			{"firespin", MoveName::Fire_Spin},
			{"firstimpression", MoveName::First_Impression},
			{"fishiousrend", MoveName::Fishious_Rend},
			{"fissure", MoveName::Fissure},
			{"flail", MoveName::Flail},
			{"flameburst", MoveName::Flame_Burst},
			{"flamecharge", MoveName::Flame_Charge},
			{"flamethrower", MoveName::Flamethrower},
			{"flamewheel", MoveName::Flame_Wheel},
			{"flareblitz", MoveName::Flare_Blitz},
			{"flash", MoveName::Flash},
			{"flashcannon", MoveName::Flash_Cannon},
			{"flatter", MoveName::Flatter},
			{"fleurcannon", MoveName::Fleur_Cannon},
			{"fling", MoveName::Fling},
			{"flipturn", MoveName::Flip_Turn},
			{"floralhealing", MoveName::Floral_Healing},
			{"flowershield", MoveName::Flower_Shield},
			{"flowertrick", MoveName::Flower_Trick},
			{"fly", MoveName::Fly},
			{"flyingpress", MoveName::Flying_Press},
			{"focusblast", MoveName::Focus_Blast},
			{"focusenergy", MoveName::Focus_Energy},
			{"focuspunch", MoveName::Focus_Punch},
			{"followme", MoveName::Follow_Me},
			{"forcepalm", MoveName::Force_Palm},
			{"foresight", MoveName::Foresight},
			{"forestscurse", MoveName::Forests_Curse},
			{"foulplay", MoveName::Foul_Play},
			{"freezedry", MoveName::Freeze_Dry},
			{"freezeshock", MoveName::Freeze_Shock},
			{"freezingglare", MoveName::Freezing_Glare},
			{"frenzyplant", MoveName::Frenzy_Plant},
			{"frostbreath", MoveName::Frost_Breath},
			{"frustration", MoveName::Frustration},
			{"frustrationnull", MoveName::Frustration}, // Pokemon Showdown
			{"furyattack", MoveName::Fury_Attack},
			{"furycutter", MoveName::Fury_Cutter},
			{"furyswipes", MoveName::Fury_Swipes},
			{"fusionbolt", MoveName::Fusion_Bolt},
			{"fusionflare", MoveName::Fusion_Flare},
			{"futuresight", MoveName::Future_Sight},
			{"gastroacid", MoveName::Gastro_Acid},
			{"geargrind", MoveName::Gear_Grind},
			{"gearup", MoveName::Gear_Up},
			{"genesissupernova", MoveName::Genesis_Supernova},
			{"geomancy", MoveName::Geomancy},
			{"gigadrain", MoveName::Giga_Drain},
			{"gigaimpact", MoveName::Giga_Impact},
			{"gigatonhammer", MoveName::Gigaton_Hammer},
			{"gigavolthavoc", MoveName::Gigavolt_Havoc},
			{"glaciallance", MoveName::Glacial_Lance},
			{"glaciate", MoveName::Glaciate},
			{"glaiverush", MoveName::Glaive_Rush},
			{"glare", MoveName::Glare},
			{"grassknot", MoveName::Grass_Knot},
			{"grasspledge", MoveName::Grass_Pledge},
			{"grasswhistle", MoveName::Grass_Whistle},
			{"grassyglide", MoveName::Grassy_Glide},
			{"grassyterrain", MoveName::Grassy_Terrain},
			{"gravapple", MoveName::Grav_Apple},
			{"gravity", MoveName::Gravity},
			{"growl", MoveName::Growl},
			{"growth", MoveName::Growth},
			{"grudge", MoveName::Grudge},
			{"guardianofalola", MoveName::Guardian_of_Alola},
			{"guardsplit", MoveName::Guard_Split},
			{"guardswap", MoveName::Guard_Swap},
			{"guillotine", MoveName::Guillotine},
			{"gunkshot", MoveName::Gunk_Shot},
			{"gust", MoveName::Gust},
			{"gyroball", MoveName::Gyro_Ball},
			{"hail", MoveName::Hail},
			{"hammerarm", MoveName::Hammer_Arm},
			{"happyhour", MoveName::Happy_Hour},
			{"harden", MoveName::Harden},
			{"haze", MoveName::Haze},
			{"headbutt", MoveName::Headbutt},
			{"headcharge", MoveName::Head_Charge},
			{"headsmash", MoveName::Head_Smash},
			{"healbell", MoveName::Heal_Bell},
			{"healblock", MoveName::Heal_Block},
			{"healingwish", MoveName::Healing_Wish},
			{"healorder", MoveName::Heal_Order},
			{"healpulse", MoveName::Heal_Pulse},
			{"heartstamp", MoveName::Heart_Stamp},
			{"heartswap", MoveName::Heart_Swap},
			{"heatcrash", MoveName::Heat_Crash},
			{"heatwave", MoveName::Heat_Wave},
			{"heavyslam", MoveName::Heavy_Slam},
			{"helpinghand", MoveName::Helping_Hand},
			{"hex", MoveName::Hex},
			{"hiddenpower", MoveName::Hidden_Power},
			{"hiddenpowerbug", MoveName::Hidden_Power},
			{"hiddenpowerdark", MoveName::Hidden_Power},
			{"hiddenpowerdragon", MoveName::Hidden_Power},
			{"hiddenpowerelectric", MoveName::Hidden_Power},
			{"hiddenpowerfighting", MoveName::Hidden_Power},
			{"hiddenpowerfire", MoveName::Hidden_Power},
			{"hiddenpowerflying", MoveName::Hidden_Power},
			{"hiddenpowerghost", MoveName::Hidden_Power},
			{"hiddenpowergrass", MoveName::Hidden_Power},
			{"hiddenpowerground", MoveName::Hidden_Power},
			{"hiddenpowerice", MoveName::Hidden_Power},
			{"hiddenpowerpoison", MoveName::Hidden_Power},
			{"hiddenpowerpsychic", MoveName::Hidden_Power},
			{"hiddenpowerrock", MoveName::Hidden_Power},
			{"hiddenpowersteel", MoveName::Hidden_Power},
			{"hiddenpowerwater", MoveName::Hidden_Power},
			{"highhorsepower", MoveName::High_Horsepower},
			{"highjumpkick", MoveName::High_Jump_Kick},
			{"hijumpkick", MoveName::High_Jump_Kick},
			{"hitselfinconfusion", MoveName::Hit_Self},
			{"holdback", MoveName::Hold_Back},
			{"holdhands", MoveName::Hold_Hands},
			{"honeclaws", MoveName::Hone_Claws},
			{"hornattack", MoveName::Horn_Attack},
			{"horndrill", MoveName::Horn_Drill},
			{"hornleech", MoveName::Horn_Leech},
			{"howl", MoveName::Howl},
			{"hpbug", MoveName::Hidden_Power},
			{"hpdark", MoveName::Hidden_Power},
			{"hpdragon", MoveName::Hidden_Power},
			{"hpelectric", MoveName::Hidden_Power},
			{"hpfighting", MoveName::Hidden_Power},
			{"hpfire", MoveName::Hidden_Power},
			{"hpflying", MoveName::Hidden_Power},
			{"hpghost", MoveName::Hidden_Power},
			{"hpgrass", MoveName::Hidden_Power},
			{"hpground", MoveName::Hidden_Power},
			{"hpice", MoveName::Hidden_Power},
			{"hppoison", MoveName::Hidden_Power},
			{"hppsychic", MoveName::Hidden_Power},
			{"hprock", MoveName::Hidden_Power},
			{"hpsteel", MoveName::Hidden_Power},
			{"hpwater", MoveName::Hidden_Power},
			{"hurricane", MoveName::Hurricane},
			{"hydrocannon", MoveName::Hydro_Cannon},
			{"hydropump", MoveName::Hydro_Pump},
			{"hydrosteam", MoveName::Hydro_Steam},
			{"hydrovortex", MoveName::Hydro_Vortex},
			{"hyperbeam", MoveName::Hyper_Beam},
			{"hyperdrill", MoveName::Hyper_Drill},
			{"hyperfang", MoveName::Hyper_Fang},
			{"hyperspacefury", MoveName::Hyperspace_Fury},
			{"hyperspacehole", MoveName::Hyperspace_Hole},
			{"hypervoice", MoveName::Hyper_Voice},
			{"hypnosis", MoveName::Hypnosis},
			{"iceball", MoveName::Ice_Ball},
			{"icebeam", MoveName::Ice_Beam},
			{"iceburn", MoveName::Ice_Burn},
			{"icefang", MoveName::Ice_Fang},
			{"icehammer", MoveName::Ice_Hammer},
			{"icepunch", MoveName::Ice_Punch},
			{"iceshard", MoveName::Ice_Shard},
			{"icespinner", MoveName::Ice_Spinner},
			{"iciclecrash", MoveName::Icicle_Crash},
			{"iciclespear", MoveName::Icicle_Spear},
			{"icywind", MoveName::Icy_Wind},
			{"imprison", MoveName::Imprison},
			{"incinerate", MoveName::Incinerate},
			{"inferno", MoveName::Inferno},
			{"infernooverdrive", MoveName::Inferno_Overdrive},
			{"infestation", MoveName::Infestation},
			{"ingrain", MoveName::Ingrain},
			{"instruct", MoveName::Instruct},
			{"iondeluge", MoveName::Ion_Deluge},
			{"irondefense", MoveName::Iron_Defense},
			{"ironhead", MoveName::Iron_Head},
			{"irontail", MoveName::Iron_Tail},
			{"ivycudgel", MoveName::Ivy_Cudgel},
			{"jawlock", MoveName::Jaw_Lock},
			{"jetpunch", MoveName::Jet_Punch},
			{"judgment", MoveName::Judgment},
			{"jumpkick", MoveName::Jump_Kick},
			{"junglehealing", MoveName::Jungle_Healing},
			{"karatechop", MoveName::Karate_Chop},
			{"kinesis", MoveName::Kinesis},
			{"kingsshield", MoveName::Kings_Shield},
			{"knockoff", MoveName::Knock_Off},
			{"kowtowcleave", MoveName::Kowtow_Cleave},
			{"landswrath", MoveName::Lands_Wrath},
			{"laserfocus", MoveName::Laser_Focus},
			{"lashout", MoveName::Lash_Out},
			{"lastresort", MoveName::Last_Resort},
			{"lastrespects", MoveName::Last_Respects},
			{"lavaplume", MoveName::Lava_Plume},
			{"leafage", MoveName::Leafage},
			{"leafblade", MoveName::Leaf_Blade},
			{"leafstorm", MoveName::Leaf_Storm},
			{"leaftornado", MoveName::Leaf_Tornado},
			{"leechlife", MoveName::Leech_Life},
			{"leechseed", MoveName::Leech_Seed},
			{"leer", MoveName::Leer},
			{"letssnuggleforever", MoveName::Lets_Snuggle_Forever},
			{"lick", MoveName::Lick},
			{"lifedew", MoveName::Life_Dew},
			{"lightofruin", MoveName::Light_of_Ruin},
			{"lightscreen", MoveName::Light_Screen},
			{"lightthatburnsthesky", MoveName::Light_That_Burns_the_Sky},
			{"liquidation", MoveName::Liquidation},
			{"lockon", MoveName::Lock_On},
			{"lovelykiss", MoveName::Lovely_Kiss},
			{"lowkick", MoveName::Low_Kick},
			{"lowsweep", MoveName::Low_Sweep},
			{"luckychant", MoveName::Lucky_Chant},
			{"luminacrash", MoveName::Lumina_Crash},
			{"lunardance", MoveName::Lunar_Dance},
			{"lunge", MoveName::Lunge},
			{"lusterpurge", MoveName::Luster_Purge},
			{"machpunch", MoveName::Mach_Punch},
			{"magicalleaf", MoveName::Magical_Leaf},
			{"magicaltorque", MoveName::Magical_Torque},
			{"magiccoat", MoveName::Magic_Coat},
			{"magicpowder", MoveName::Magic_Powder},
			{"magicroom", MoveName::Magic_Room},
			{"magmastorm", MoveName::Magma_Storm},
			{"magnetbomb", MoveName::Magnet_Bomb},
			{"magneticflux", MoveName::Magnetic_Flux},
			{"magnetrise", MoveName::Magnet_Rise},
			{"magnitude", MoveName::Magnitude},
			{"makeitrain", MoveName::Make_It_Rain},
			{"maliciousmoonsault", MoveName::Malicious_Moonsault},
			{"matblock", MoveName::Mat_Block},
			{"matchagotcha", MoveName::Matcha_Gotcha},
			{"maxairstream", MoveName::Max_Airstream},
			{"maxdarkness", MoveName::Max_Darkness},
			{"maxflare", MoveName::Max_Flare},
			{"maxflutterby", MoveName::Max_Flutterby},
			{"maxgeyser", MoveName::Max_Geyser},
			{"maxguard", MoveName::Max_Guard},
			{"maxhailstorm", MoveName::Max_Hailstorm},
			{"maxknuckle", MoveName::Max_Knuckle},
			{"maxlightning", MoveName::Max_Lightning},
			{"maxmindstorm", MoveName::Max_Mindstorm},
			{"maxooze", MoveName::Max_Ooze},
			{"maxovergrowth", MoveName::Max_Overgrowth},
			{"maxphantasm", MoveName::Max_Phantasm},
			{"maxquake", MoveName::Max_Quake},
			{"maxrockfall", MoveName::Max_Rockfall},
			{"maxstarfall", MoveName::Max_Starfall},
			{"maxsteelspike", MoveName::Max_Steelspike},
			{"maxstrike", MoveName::Max_Strike},
			{"maxwyrmwind", MoveName::Max_Wyrmwind},
			{"meanlook", MoveName::Mean_Look},
			{"meditate", MoveName::Meditate},
			{"mefirst", MoveName::Me_First},
			{"megadrain", MoveName::Mega_Drain},
			{"megahorn", MoveName::Megahorn},
			{"megakick", MoveName::Mega_Kick},
			{"megapunch", MoveName::Mega_Punch},
			{"memento", MoveName::Memento},
			{"menacingmoonrazemaelstrom", MoveName::Menacing_Moonraze_Maelstrom},
			{"metalburst", MoveName::Metal_Burst},
			{"metalclaw", MoveName::Metal_Claw},
			{"metalsound", MoveName::Metal_Sound},
			{"meteorassault", MoveName::Meteor_Assault},
			{"meteorbeam", MoveName::Meteor_Beam},
			{"meteormash", MoveName::Meteor_Mash},
			{"metronome", MoveName::Metronome},
			{"milkdrink", MoveName::Milk_Drink},
			{"mimic", MoveName::Mimic},
			{"mindblown", MoveName::Mind_Blown},
			{"mindreader", MoveName::Mind_Reader},
			{"minimize", MoveName::Minimize},
			{"miracleeye", MoveName::Miracle_Eye},
			{"mirrorcoat", MoveName::Mirror_Coat},
			{"mirrormove", MoveName::Mirror_Move},
			{"mirrorshot", MoveName::Mirror_Shot},
			{"mist", MoveName::Mist},
			{"mistball", MoveName::Mist_Ball},
			{"mistyexplosion", MoveName::Misty_Explosion},
			{"mistyterrain", MoveName::Misty_Terrain},
			{"moonblast", MoveName::Moonblast},
			{"moongeistbeam", MoveName::Moongeist_Beam},
			{"moonlight", MoveName::Moonlight},
			{"morningsun", MoveName::Morning_Sun},
			{"mortalspin", MoveName::Mortal_Spin},
			{"mudbomb", MoveName::Mud_Bomb},
			{"muddywater", MoveName::Muddy_Water},
			{"mudshot", MoveName::Mud_Shot},
			{"mudslap", MoveName::Mud_Slap},
			{"mudsport", MoveName::Mud_Sport},
			{"multiattack", MoveName::Multi_Attack},
			{"mysticalfire", MoveName::Mystical_Fire},
			{"nastyplot", MoveName::Nasty_Plot},
			{"naturalgift", MoveName::Natural_Gift},
			{"naturepower", MoveName::Nature_Power},
			{"naturesmadness", MoveName::Natures_Madness},
			{"needlearm", MoveName::Needle_Arm},
			{"neverendingnightmare", MoveName::Never_Ending_Nightmare},
			{"nightdaze", MoveName::Night_Daze},
			{"nightmare", MoveName::Nightmare},
			{"nightshade", MoveName::Night_Shade},
			{"nightslash", MoveName::Night_Slash},
			{"nobleroar", MoveName::Noble_Roar},
			{"noretreat", MoveName::No_Retreat},
			{"noxioustorque", MoveName::Noxious_Torque},
			{"nuzzle", MoveName::Nuzzle},
			{"oblivionwing", MoveName::Oblivion_Wing},
			{"obstruct", MoveName::Obstruct},
			{"oceanicoperetta", MoveName::Oceanic_Operetta},
			{"octazooka", MoveName::Octazooka},
			{"octolock", MoveName::Octolock},
			{"odorsleuth", MoveName::Odor_Sleuth},
			{"ominouswind", MoveName::Ominous_Wind},
			{"orderup", MoveName::Order_Up},
			{"originpulse", MoveName::Origin_Pulse},
			{"outrage", MoveName::Outrage},
			{"overdrive", MoveName::Overdrive},
			{"overheat", MoveName::Overheat},
			{"painsplit", MoveName::Pain_Split},
			{"paraboliccharge", MoveName::Parabolic_Charge},
			{"partingshot", MoveName::Parting_Shot},
			{"pass", MoveName::Pass},
			{"payback", MoveName::Payback},
			{"payday", MoveName::Pay_Day},
			{"peck", MoveName::Peck},
			{"perishsong", MoveName::Perish_Song},
			{"petalblizzard", MoveName::Petal_Blizzard},
			{"petaldance", MoveName::Petal_Dance},
			{"phantomforce", MoveName::Phantom_Force},
			{"photongeyser", MoveName::Photon_Geyser},
			{"pinmissile", MoveName::Pin_Missile},
			{"plasmafists", MoveName::Plasma_Fists},
			{"playnice", MoveName::Play_Nice},
			{"playrough", MoveName::Play_Rough},
			{"pluck", MoveName::Pluck},
			{"poisonfang", MoveName::Poison_Fang},
			{"poisongas", MoveName::Poison_Gas},
			{"poisonjab", MoveName::Poison_Jab},
			{"poisonpowder", MoveName::Poison_Powder},
			{"poisonsting", MoveName::Poison_Sting},
			{"poisontail", MoveName::Poison_Tail},
			{"pollenpuff", MoveName::Pollen_Puff},
			{"poltergeist", MoveName::Poltergeist},
			{"populationbomb", MoveName::Population_Bomb},
			{"pounce", MoveName::Pounce},
			{"pound", MoveName::Pound},
			{"powder", MoveName::Powder},
			{"powdersnow", MoveName::Powder_Snow},
			{"powergem", MoveName::Power_Gem},
			{"powersplit", MoveName::Power_Split},
			{"powerswap", MoveName::Power_Swap},
			{"powertrick", MoveName::Power_Trick},
			{"powertrip", MoveName::Power_Trip},
			{"poweruppunch", MoveName::Power_Up_Punch},
			{"powerwhip", MoveName::Power_Whip},
			{"precipiceblades", MoveName::Precipice_Blades},
			{"present", MoveName::Present},
			{"prismaticlaser", MoveName::Prismatic_Laser},
			{"protect", MoveName::Protect},
			{"psybeam", MoveName::Psybeam},
			{"psyblade", MoveName::Psyblade},
			{"psychic", MoveName::Psychic},
			{"psychicfangs", MoveName::Psychic_Fangs},
			{"psychicnoise", MoveName::Psychic_Noise},
			{"psychicterrain", MoveName::Psychic_Terrain},
			{"psychoboost", MoveName::Psycho_Boost},
			{"psychocut", MoveName::Psycho_Cut},
			{"psychoshift", MoveName::Psycho_Shift},
			{"psychup", MoveName::Psych_Up},
			{"psyshock", MoveName::Psyshock},
			{"psystrike", MoveName::Psystrike},
			{"psywave", MoveName::Psywave},
			{"pulverizingpancake", MoveName::Pulverizing_Pancake},
			{"punishment", MoveName::Punishment},
			{"purify", MoveName::Purify},
			{"pursuit", MoveName::Pursuit},
			{"pyroball", MoveName::Pyro_Ball},
			{"quash", MoveName::Quash},
			{"quickattack", MoveName::Quick_Attack},
			{"quickguard", MoveName::Quick_Guard},
			{"quiverdance", MoveName::Quiver_Dance},
			{"rage", MoveName::Rage},
			{"ragefist", MoveName::Rage_Fist},
			{"ragepowder", MoveName::Rage_Powder},
			{"ragingbull", MoveName::Raging_Bull},
			{"raindance", MoveName::Rain_Dance},
			{"rapidspin", MoveName::Rapid_Spin},
			{"razorleaf", MoveName::Razor_Leaf},
			{"razorshell", MoveName::Razor_Shell},
			{"razorwind", MoveName::Razor_Wind},
			{"recharge", MoveName::Hyper_Beam}, // TODO: fix
			{"recover", MoveName::Recover},
			{"recycle", MoveName::Recycle},
			{"reflect", MoveName::Reflect},
			{"reflecttype", MoveName::Reflect_Type},
			{"refresh", MoveName::Refresh},
			{"relicsong", MoveName::Relic_Song},
			{"rest", MoveName::Rest},
			{"retaliate", MoveName::Retaliate},
			{"return", MoveName::Return},
			{"return102", MoveName::Return},
			{"revelationdance", MoveName::Revelation_Dance},
			{"revenge", MoveName::Revenge},
			{"reversal", MoveName::Reversal},
			{"revivalblessing", MoveName::Revival_Blessing},
			{"risingvoltage", MoveName::Rising_Voltage},
			{"roar", MoveName::Roar},
			{"roaroftime", MoveName::Roar_of_Time},
			{"rockblast", MoveName::Rock_Blast},
			{"rockclimb", MoveName::Rock_Climb},
			{"rockpolish", MoveName::Rock_Polish},
			{"rockslide", MoveName::Rock_Slide},
			{"rocksmash", MoveName::Rock_Smash},
			{"rockthrow", MoveName::Rock_Throw},
			{"rocktomb", MoveName::Rock_Tomb},
			{"rockwrecker", MoveName::Rock_Wrecker},
			{"roleplay", MoveName::Role_Play},
			{"rollingkick", MoveName::Rolling_Kick},
			{"rollout", MoveName::Rollout},
			{"roost", MoveName::Roost},
			{"rototiller", MoveName::Rototiller},
			{"round", MoveName::Round},
			{"ruination", MoveName::Ruination},
			{"sacredfire", MoveName::Sacred_Fire},
			{"sacredsword", MoveName::Sacred_Sword},
			{"safeguard", MoveName::Safeguard},
			{"saltcure", MoveName::Salt_Cure},
			{"sandattack", MoveName::Sand_Attack},
			{"sandstorm", MoveName::Sandstorm},
			{"sandtomb", MoveName::Sand_Tomb},
			{"savagespinout", MoveName::Savage_Spin_Out},
			{"scald", MoveName::Scald},
			{"scaleshot", MoveName::Scale_Shot},
			{"scaryface", MoveName::Scary_Face},
			{"scorchingsands", MoveName::Scorching_Sands},
			{"scratch", MoveName::Scratch},
			{"screech", MoveName::Screech},
			{"searingshot", MoveName::Searing_Shot},
			{"searingsunrazesmash", MoveName::Searing_Sunraze_Smash},
			{"secretpower", MoveName::Secret_Power},
			{"secretsword", MoveName::Secret_Sword},
			{"seedbomb", MoveName::Seed_Bomb},
			{"seedflare", MoveName::Seed_Flare},
			{"seismictoss", MoveName::Seismic_Toss},
			{"selfdestruct", MoveName::Self_Destruct},
			{"shadowball", MoveName::Shadow_Ball},
			{"shadowbone", MoveName::Shadow_Bone},
			{"shadowclaw", MoveName::Shadow_Claw},
			{"shadowforce", MoveName::Shadow_Force},
			{"shadowpunch", MoveName::Shadow_Punch},
			{"shadowsneak", MoveName::Shadow_Sneak},
			{"sharpen", MoveName::Sharpen},
			{"shatteredpsyche", MoveName::Shattered_Psyche},
			{"shedtail", MoveName::Shed_Tail},
			{"sheercold", MoveName::Sheer_Cold},
			{"shellsidearm", MoveName::Shell_Side_Arm},
			{"shellsmash", MoveName::Shell_Smash},
			{"shelltrap", MoveName::Shell_Trap},
			{"shiftgear", MoveName::Shift_Gear},
			{"shockwave", MoveName::Shock_Wave},
			{"shoreup", MoveName::Shore_Up},
			{"signalbeam", MoveName::Signal_Beam},
			{"silktrap", MoveName::Silk_Trap},
			{"silverwind", MoveName::Silver_Wind},
			{"simplebeam", MoveName::Simple_Beam},
			{"sing", MoveName::Sing},
			{"sinisterarrowraid", MoveName::Sinister_Arrow_Raid},
			{"sketch", MoveName::Sketch},
			{"skillswap", MoveName::Skill_Swap},
			{"skittersmack", MoveName::Skitter_Smack},
			{"skullbash", MoveName::Skull_Bash},
			{"skyattack", MoveName::Sky_Attack},
			{"skydrop", MoveName::Sky_Drop},
			{"skyuppercut", MoveName::Sky_Uppercut},
			{"slackoff", MoveName::Slack_Off},
			{"slam", MoveName::Slam},
			{"slash", MoveName::Slash},
			{"sleeppowder", MoveName::Sleep_Powder},
			{"sleeptalk", MoveName::Sleep_Talk},
			{"sludge", MoveName::Sludge},
			{"sludgebomb", MoveName::Sludge_Bomb},
			{"sludgewave", MoveName::Sludge_Wave},
			{"smackdown", MoveName::Smack_Down},
			{"smartstrike", MoveName::Smart_Strike},
			{"smellingsalt", MoveName::Smelling_Salts},
			{"smellingsalts", MoveName::Smelling_Salts},
			{"smog", MoveName::Smog},
			{"smokescreen", MoveName::Smokescreen},
			{"snaptrap", MoveName::Snap_Trap},
			{"snarl", MoveName::Snarl},
			{"snatch", MoveName::Snatch},
			{"snipeshot", MoveName::Snipe_Shot},
			{"snore", MoveName::Snore},
			{"snowscape", MoveName::Snowscape},
			{"soak", MoveName::Soak},
			{"softboiled", MoveName::Soft_Boiled},
			{"solarbeam", MoveName::Solar_Beam},
			{"solarblade", MoveName::Solar_Blade},
			{"sonicboom", MoveName::Sonic_Boom},
			{"soulstealing7starstrike", MoveName::Soul_Stealing_7_Star_Strike},
			{"spacialrend", MoveName::Spacial_Rend},
			{"spark", MoveName::Spark},
			{"sparklingaria", MoveName::Sparkling_Aria},
			{"spectralthief", MoveName::Spectral_Thief},
			{"speedswap", MoveName::Speed_Swap},
			{"spicyextract", MoveName::Spicy_Extract},
			{"spiderweb", MoveName::Spider_Web},
			{"spikecannon", MoveName::Spike_Cannon},
			{"spikes", MoveName::Spikes},
			{"spikyshield", MoveName::Spiky_Shield},
			{"spinout", MoveName::Spin_Out},
			{"spiritbreak", MoveName::Spirit_Break},
			{"spiritshackle", MoveName::Spirit_Shackle},
			{"spite", MoveName::Spite},
			{"spitup", MoveName::Spit_Up},
			{"splash", MoveName::Splash},
			{"splinteredstormshards", MoveName::Splintered_Stormshards},
			{"spore", MoveName::Spore},
			{"spotlight", MoveName::Spotlight},
			{"stealthrock", MoveName::Stealth_Rock},
			{"steameruption", MoveName::Steam_Eruption},
			{"steamroller", MoveName::Steamroller},
			{"steelbeam", MoveName::Steel_Beam},
			{"steelroller", MoveName::Steel_Roller},
			{"steelwing", MoveName::Steel_Wing},
			{"stickyweb", MoveName::Sticky_Web},
			{"stockpile", MoveName::Stockpile},
			{"stokedsparksurfer", MoveName::Stoked_Sparksurfer},
			{"stomp", MoveName::Stomp},
			{"stompingtantrum", MoveName::Stomping_Tantrum},
			{"stoneedge", MoveName::Stone_Edge},
			{"storedpower", MoveName::Stored_Power},
			{"stormthrow", MoveName::Storm_Throw},
			{"strangesteam", MoveName::Strange_Steam},
			{"strength", MoveName::Strength},
			{"strengthsap", MoveName::Strength_Sap},
			{"stringshot", MoveName::String_Shot},
			{"struggle", MoveName::Struggle},
			{"strugglebug", MoveName::Struggle_Bug},
			{"stuffcheeks", MoveName::Stuff_Cheeks},
			{"stunspore", MoveName::Stun_Spore},
			{"submission", MoveName::Submission},
			{"substitute", MoveName::Substitute},
			{"subzeroslammer", MoveName::Subzero_Slammer},
			{"suckerpunch", MoveName::Sucker_Punch},
			{"sunnyday", MoveName::Sunny_Day},
			{"sunsteelstrike", MoveName::Sunsteel_Strike},
			{"superfang", MoveName::Super_Fang},
			{"superpower", MoveName::Superpower},
			{"supersonic", MoveName::Supersonic},
			{"supersonicskystrike", MoveName::Supersonic_Skystrike},
			{"surf", MoveName::Surf},
			{"surgingstrikes", MoveName::Surging_Strikes},
			{"swagger", MoveName::Swagger},
			{"swallow", MoveName::Swallow},
			{"sweetkiss", MoveName::Sweet_Kiss},
			{"sweetscent", MoveName::Sweet_Scent},
			{"swift", MoveName::Swift},
			{"switch0", MoveName::Switch0},
			{"switch1", MoveName::Switch1},
			{"switch2", MoveName::Switch2},
			{"switch3", MoveName::Switch3},
			{"switch4", MoveName::Switch4},
			{"switch5", MoveName::Switch5},
			{"switcheroo", MoveName::Switcheroo},
			{"swordsdance", MoveName::Swords_Dance},
			{"synchronoise", MoveName::Synchronoise},
			{"synthesis", MoveName::Synthesis},
			{"syrupbomb", MoveName::Syrup_Bomb},
			{"tachyoncutter", MoveName::Tachyon_Cutter},
			{"tackle", MoveName::Tackle},
			{"tailglow", MoveName::Tail_Glow},
			{"tailslap", MoveName::Tail_Slap},
			{"tailwhip", MoveName::Tail_Whip},
			{"tailwind", MoveName::Tailwind},
			{"takedown", MoveName::Take_Down},
			{"tarshot", MoveName::Tar_Shot},
			{"taunt", MoveName::Taunt},
			{"tearfullook", MoveName::Tearful_Look},
			{"teatime", MoveName::Teatime},
			{"technoblast", MoveName::Techno_Blast},
			{"tectonicrage", MoveName::Tectonic_Rage},
			{"teeterdance", MoveName::Teeter_Dance},
			{"telekinesis", MoveName::Telekinesis},
			{"teleport", MoveName::Teleport},
			{"terablast", MoveName::Tera_Blast},
			{"terrainpulse", MoveName::Terrain_Pulse},
			{"thief", MoveName::Thief},
			{"thousandarrows", MoveName::Thousand_Arrows},
			{"thousandwaves", MoveName::Thousand_Waves},
			{"thrash", MoveName::Thrash},
			{"throatchop", MoveName::Throat_Chop},
			{"thunder", MoveName::Thunder},
			{"thunderbolt", MoveName::Thunderbolt},
			{"thundercage", MoveName::Thunder_Cage},
			{"thunderclap", MoveName::Thunderclap},
			{"thunderfang", MoveName::Thunder_Fang},
			{"thunderouskick", MoveName::Thunderous_Kick},
			{"thunderpunch", MoveName::Thunder_Punch},
			{"thundershock", MoveName::Thunder_Shock},
			{"thunderwave", MoveName::Thunder_Wave},
			{"tickle", MoveName::Tickle},
			{"tidyup", MoveName::Tidy_Up},
			{"topsyturvy", MoveName::Topsy_Turvy},
			{"torchsong", MoveName::Torch_Song},
			{"torment", MoveName::Torment},
			{"toxic", MoveName::Toxic},
			{"toxicspikes", MoveName::Toxic_Spikes},
			{"toxicthread", MoveName::Toxic_Thread},
			{"trailblaze", MoveName::Trailblaze},
			{"transform", MoveName::Transform},
			{"triattack", MoveName::Tri_Attack},
			{"trick", MoveName::Trick},
			{"trickortreat", MoveName::Trick_or_Treat},
			{"trickroom", MoveName::Trick_Room},
			{"tripleaxel", MoveName::Triple_Axel},
			{"tripledive", MoveName::Triple_Dive},
			{"triplekick", MoveName::Triple_Kick},
			{"tropkick", MoveName::Trop_Kick},
			{"trumpcard", MoveName::Trump_Card},
			{"twinbeam", MoveName::Twin_Beam},
			{"twineedle", MoveName::Twineedle},
			{"twinkletackle", MoveName::Twinkle_Tackle},
			{"twister", MoveName::Twister},
			{"upperhand", MoveName::Upper_Hand},
			{"uproar", MoveName::Uproar},
			{"uturn", MoveName::U_turn},
			{"vacuumwave", MoveName::Vacuum_Wave},
			{"vcreate", MoveName::V_create},
			{"venomdrench", MoveName::Venom_Drench},
			{"venoshock", MoveName::Venoshock},
			{"vicegrip", MoveName::Vise_Grip},
			{"vinewhip", MoveName::Vine_Whip},
			{"visegrip", MoveName::Vise_Grip},
			{"vitalthrow", MoveName::Vital_Throw},
			{"voltswitch", MoveName::Volt_Switch},
			{"volttackle", MoveName::Volt_Tackle},
			{"wakeupslap", MoveName::Wake_Up_Slap},
			{"waterfall", MoveName::Waterfall},
			{"watergun", MoveName::Water_Gun},
			{"waterpledge", MoveName::Water_Pledge},
			{"waterpulse", MoveName::Water_Pulse},
			{"watershuriken", MoveName::Water_Shuriken},
			{"watersport", MoveName::Water_Sport},
			{"waterspout", MoveName::Water_Spout},
			{"weatherball", MoveName::Weather_Ball},
			{"whirlpool", MoveName::Whirlpool},
			{"whirlwind", MoveName::Whirlwind},
			{"wickedblow", MoveName::Wicked_Blow},
			{"wickedtorque", MoveName::Wicked_Torque},
			{"wideguard", MoveName::Wide_Guard},
			{"wildcharge", MoveName::Wild_Charge},
			{"willowisp", MoveName::Will_O_Wisp},
			{"wingattack", MoveName::Wing_Attack},
			{"wish", MoveName::Wish},
			{"withdraw", MoveName::Withdraw},
			{"wonderroom", MoveName::Wonder_Room},
			{"woodhammer", MoveName::Wood_Hammer},
			{"workup", MoveName::Work_Up},
			{"worryseed", MoveName::Worry_Seed},
			{"wrap", MoveName::Wrap},
			{"wringout", MoveName::Wring_Out},
			{"xscissor", MoveName::X_Scissor},
			{"yawn", MoveName::Yawn},
			{"zapcannon", MoveName::Zap_Cannon},
			{"zenheadbutt", MoveName::Zen_Headbutt},
			{"zingzap", MoveName::Zing_Zap},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<25_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Move", str);
	}
	return *result;
}

} // namespace technicalmachine
