// Move string conversions
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/string_conversions/move.hpp>

#include <tm/string_conversions/invalid_string_conversion.hpp>
#include <tm/string_conversions/lowercase_alphanumeric.hpp>

#include <tm/move/moves.hpp>

#include <containers/flat_map.hpp>
#include <containers/array.hpp>
#include <containers/begin_end.hpp>
#include <containers/lookup.hpp>

namespace technicalmachine {

using namespace bounded::literal;

auto to_string(Moves const move) -> std::string_view {
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
		case Moves::Vise_Grip: return "Vise Grip";
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
		case Moves::Double_Iron_Bash: return "Double Iron Bash";
		case Moves::Max_Guard: return "Max Guard";
		case Moves::Dynamax_Cannon: return "Dynamax Cannon";
		case Moves::Snipe_Shot: return "Snipe Shot";
		case Moves::Jaw_Lock: return "Jaw Lock";
		case Moves::Stuff_Cheeks: return "Stuff Cheeks";
		case Moves::No_Retreat: return "No Retreat";
		case Moves::Tar_Shot: return "Tar Shot";
		case Moves::Magic_Powder: return "Magic Powder";
		case Moves::Dragon_Darts: return "Dragon Darts";
		case Moves::Teatime: return "Teatime";
		case Moves::Octolock: return "Octolock";
		case Moves::Bolt_Beak: return "Bolt Beak";
		case Moves::Fishious_Rend: return "Fishious Rend";
		case Moves::Court_Change: return "Court Change";
		case Moves::Max_Flare: return "Max Flare";
		case Moves::Max_Flutterby: return "Max Flutterby";
		case Moves::Max_Lightning: return "Max Lightning";
		case Moves::Max_Strike: return "Max Strike";
		case Moves::Max_Knuckle: return "Max Knuckle";
		case Moves::Max_Phantasm: return "Max Phantasm";
		case Moves::Max_Hailstorm: return "Max Hailstorm";
		case Moves::Max_Ooze: return "Max Ooze";
		case Moves::Max_Geyser: return "Max Geyser";
		case Moves::Max_Airstream: return "Max Airstream";
		case Moves::Max_Starfall: return "Max Starfall";
		case Moves::Max_Wyrmwind: return "Max Wyrmwind";
		case Moves::Max_Mindstorm: return "Max Mindstorm";
		case Moves::Max_Rockfall: return "Max Rockfall";
		case Moves::Max_Quake: return "Max Quake";
		case Moves::Max_Darkness: return "Max Darkness";
		case Moves::Max_Overgrowth: return "Max Overgrowth";
		case Moves::Max_Steelspike: return "Max Steelspike";
		case Moves::Clangorous_Soul: return "Clangorous Soul";
		case Moves::Body_Press: return "Body Press";
		case Moves::Decorate: return "Decorate";
		case Moves::Drum_Beating: return "Drum Beating";
		case Moves::Snap_Trap: return "Snap Trap";
		case Moves::Pyro_Ball: return "Pyro Ball";
		case Moves::Behemoth_Blade: return "Behemoth Blade";
		case Moves::Behemoth_Bash: return "Behemoth Bash";
		case Moves::Aura_Wheel: return "Aura Wheel";
		case Moves::Breaking_Swipe: return "Breaking Swipe";
		case Moves::Branch_Poke: return "Branch Poke";
		case Moves::Overdrive: return "Overdrive";
		case Moves::Apple_Acid: return "Apple Acid";
		case Moves::Grav_Apple: return "Grav Apple";
		case Moves::Spirit_Break: return "Spirit Break";
		case Moves::Strange_Steam: return "Strange Steam";
		case Moves::Life_Dew: return "Life Dew";
		case Moves::Obstruct: return "Obstruct";
		case Moves::False_Surrender: return "False Surrender";
		case Moves::Meteor_Assault: return "Meteor Assault";
		case Moves::Eternabeam: return "Eternabeam";
		case Moves::Steel_Beam: return "Steel Beam";
		case Moves::Expanding_Force: return "Expanding Force";
		case Moves::Steel_Roller: return "Steel Roller";
		case Moves::Scale_Shot: return "Scale Shot";
		case Moves::Meteor_Beam: return "Meteor Beam";
		case Moves::Shell_Side_Arm: return "Shell Side Arm";
		case Moves::Misty_Explosion: return "Misty Explosion";
		case Moves::Grassy_Glide: return "Grassy Glide";
		case Moves::Rising_Voltage: return "Rising Voltage";
		case Moves::Terrain_Pulse: return "Terrain Pulse";
		case Moves::Skitter_Smack: return "Skitter Smack";
		case Moves::Burning_Jealousy: return "Burning Jealousy";
		case Moves::Lash_Out: return "Lash Out";
		case Moves::Poltergeist: return "Poltergeist";
		case Moves::Corrosive_Gas: return "Corrosive Gas";
		case Moves::Coaching: return "Coaching";
		case Moves::Flip_Turn: return "Flip Turn";
		case Moves::Triple_Axel: return "Triple Axel";
		case Moves::Dual_Wingbeat: return "Dual Wingbeat";
		case Moves::Scorching_Sands: return "Scorching Sands";
		case Moves::Jungle_Healing: return "Jungle Healing";
		case Moves::Wicked_Blow: return "Wicked Blow";
		case Moves::Surging_Strikes: return "Surging Strikes";
		case Moves::Thunder_Cage: return "Thunder Cage";
		case Moves::Dragon_Energy: return "Dragon Energy";
		case Moves::Freezing_Glare: return "Freezing Glare";
		case Moves::Fiery_Wrath: return "Fiery Wrath";
		case Moves::Thunderous_Kick: return "Thunderous Kick";
		case Moves::Glacial_Lance: return "Glacial Lance";
		case Moves::Astral_Barrage: return "Astral Barrage";
		case Moves::Eerie_Spell: return "Eerie Spell";
	}
}

template<>
auto from_string(std::string_view const str) -> Moves {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<std::string_view, Moves>>({
			{ "10000000voltthunderbolt", Moves::m10000000_Volt_Thunderbolt },
			{ "absorb", Moves::Absorb },
			{ "accelerock", Moves::Accelerock },
			{ "acid", Moves::Acid },
			{ "acidarmor", Moves::Acid_Armor },
			{ "aciddownpour", Moves::Acid_Downpour },
			{ "acidspray", Moves::Acid_Spray },
			{ "acrobatics", Moves::Acrobatics },
			{ "acupressure", Moves::Acupressure },
			{ "aerialace", Moves::Aerial_Ace },
			{ "aeroblast", Moves::Aeroblast },
			{ "afteryou", Moves::After_You },
			{ "agility", Moves::Agility },
			{ "aircutter", Moves::Air_Cutter },
			{ "airslash", Moves::Air_Slash },
			{ "alloutpummeling", Moves::All_Out_Pummeling },
			{ "allyswitch", Moves::Ally_Switch },
			{ "amnesia", Moves::Amnesia },
			{ "anchorshot", Moves::Anchor_Shot },
			{ "ancientpower", Moves::Ancient_Power },
			{ "appleacid", Moves::Apple_Acid },
			{ "aquajet", Moves::Aqua_Jet },
			{ "aquaring", Moves::Aqua_Ring },
			{ "aquatail", Moves::Aqua_Tail },
			{ "armthrust", Moves::Arm_Thrust },
			{ "aromatherapy", Moves::Aromatherapy },
			{ "aromaticmist", Moves::Aromatic_Mist },
			{ "assist", Moves::Assist },
			{ "assurance", Moves::Assurance },
			{ "astonish", Moves::Astonish },
			{ "astralbarrage", Moves::Astral_Barrage },
			{ "attackorder", Moves::Attack_Order },
			{ "attract", Moves::Attract },
			{ "aurasphere", Moves::Aura_Sphere },
			{ "aurawheel", Moves::Aura_Wheel },
			{ "aurorabeam", Moves::Aurora_Beam },
			{ "auroraveil", Moves::Aurora_Veil },
			{ "autotomize", Moves::Autotomize },
			{ "avalanche", Moves::Avalanche },
			{ "babydolleyes", Moves::Baby_Doll_Eyes },
			{ "banefulbunker", Moves::Baneful_Bunker },
			{ "barrage", Moves::Barrage },
			{ "barrier", Moves::Barrier },
			{ "batonpass", Moves::Baton_Pass },
			{ "beakblast", Moves::Beak_Blast },
			{ "beatup", Moves::Beat_Up },
			{ "behemothbash", Moves::Behemoth_Bash },
			{ "behemothblade", Moves::Behemoth_Blade },
			{ "belch", Moves::Belch },
			{ "bellydrum", Moves::Belly_Drum },
			{ "bestow", Moves::Bestow },
			{ "bide", Moves::Bide },
			{ "bind", Moves::Bind },
			{ "bite", Moves::Bite },
			{ "blackholeeclipse", Moves::Black_Hole_Eclipse },
			{ "blastburn", Moves::Blast_Burn },
			{ "blazekick", Moves::Blaze_Kick },
			{ "blizzard", Moves::Blizzard },
			{ "block", Moves::Block },
			{ "bloomdoom", Moves::Bloom_Doom },
			{ "blueflare", Moves::Blue_Flare },
			{ "bodypress", Moves::Body_Press },
			{ "bodyslam", Moves::Body_Slam },
			{ "boltbeak", Moves::Bolt_Beak },
			{ "boltstrike", Moves::Bolt_Strike },
			{ "boneclub", Moves::Bone_Club },
			{ "bonemerang", Moves::Bonemerang },
			{ "bonerush", Moves::Bone_Rush },
			{ "boomburst", Moves::Boomburst },
			{ "bounce", Moves::Bounce },
			{ "branchpoke", Moves::Branch_Poke },
			{ "bravebird", Moves::Brave_Bird },
			{ "breakingswipe", Moves::Breaking_Swipe },
			{ "breakneckblitz", Moves::Breakneck_Blitz },
			{ "brickbreak", Moves::Brick_Break },
			{ "brine", Moves::Brine },
			{ "brutalswing", Moves::Brutal_Swing },
			{ "bubble", Moves::Bubble },
			{ "bubblebeam", Moves::Bubble_Beam },
			{ "bugbite", Moves::Bug_Bite },
			{ "bugbuzz", Moves::Bug_Buzz },
			{ "bulkup", Moves::Bulk_Up },
			{ "bulldoze", Moves::Bulldoze },
			{ "bulletpunch", Moves::Bullet_Punch },
			{ "bulletseed", Moves::Bullet_Seed },
			{ "burningjealousy", Moves::Burning_Jealousy },
			{ "burnup", Moves::Burn_Up },
			{ "calmmind", Moves::Calm_Mind },
			{ "camouflage", Moves::Camouflage },
			{ "captivate", Moves::Captivate },
			{ "catastropika", Moves::Catastropika },
			{ "celebrate", Moves::Celebrate },
			{ "charge", Moves::Charge },
			{ "chargebeam", Moves::Charge_Beam },
			{ "charm", Moves::Charm },
			{ "chatter", Moves::Chatter },
			{ "chipaway", Moves::Chip_Away },
			{ "circlethrow", Moves::Circle_Throw },
			{ "clamp", Moves::Clamp },
			{ "clangingscales", Moves::Clanging_Scales },
			{ "clangoroussoul", Moves::Clangorous_Soul },
			{ "clangoroussoulblaze", Moves::Clangorous_Soulblaze },
			{ "clearsmog", Moves::Clear_Smog },
			{ "closecombat", Moves::Close_Combat },
			{ "coaching", Moves::Coaching },
			{ "coil", Moves::Coil },
			{ "cometpunch", Moves::Comet_Punch },
			{ "confide", Moves::Confide },
			{ "confuseray", Moves::Confuse_Ray },
			{ "confusion", Moves::Confusion },
			{ "constrict", Moves::Constrict },
			{ "continentalcrush", Moves::Continental_Crush },
			{ "conversion", Moves::Conversion },
			{ "conversion2", Moves::Conversion_2 },
			{ "copycat", Moves::Copycat },
			{ "coreenforcer", Moves::Core_Enforcer },
			{ "corkscrewcrash", Moves::Corkscrew_Crash },
			{ "corrosivegas", Moves::Corrosive_Gas },
			{ "cosmicpower", Moves::Cosmic_Power },
			{ "cottonguard", Moves::Cotton_Guard },
			{ "cottonspore", Moves::Cotton_Spore },
			{ "counter", Moves::Counter },
			{ "courtchange", Moves::Court_Change },
			{ "covet", Moves::Covet },
			{ "crabhammer", Moves::Crabhammer },
			{ "craftyshield", Moves::Crafty_Shield },
			{ "crosschop", Moves::Cross_Chop },
			{ "crosspoison", Moves::Cross_Poison },
			{ "crunch", Moves::Crunch },
			{ "crushclaw", Moves::Crush_Claw },
			{ "crushgrip", Moves::Crush_Grip },
			{ "curse", Moves::Curse },
			{ "cut", Moves::Cut },
			{ "darkestlariat", Moves::Darkest_Lariat },
			{ "darkpulse", Moves::Dark_Pulse },
			{ "darkvoid", Moves::Dark_Void },
			{ "dazzlinggleam", Moves::Dazzling_Gleam },
			{ "decorate", Moves::Decorate },
			{ "defendorder", Moves::Defend_Order },
			{ "defensecurl", Moves::Defense_Curl },
			{ "defog", Moves::Defog },
			{ "destinybond", Moves::Destiny_Bond },
			{ "detect", Moves::Detect },
			{ "devastatingdrake", Moves::Devastating_Drake },
			{ "diamondstorm", Moves::Diamond_Storm },
			{ "dig", Moves::Dig },
			{ "disable", Moves::Disable },
			{ "disarmingvoice", Moves::Disarming_Voice },
			{ "discharge", Moves::Discharge },
			{ "dive", Moves::Dive },
			{ "dizzypunch", Moves::Dizzy_Punch },
			{ "doomdesire", Moves::Doom_Desire },
			{ "doubleedge", Moves::Double_Edge },
			{ "doublehit", Moves::Double_Hit },
			{ "doubleironbash", Moves::Double_Iron_Bash },
			{ "doublekick", Moves::Double_Kick },
			{ "doubleslap", Moves::Double_Slap },
			{ "doubleteam", Moves::Double_Team },
			{ "dracometeor", Moves::Draco_Meteor },
			{ "dragonascent", Moves::Dragon_Ascent },
			{ "dragonbreath", Moves::Dragon_Breath },
			{ "dragonclaw", Moves::Dragon_Claw },
			{ "dragondance", Moves::Dragon_Dance },
			{ "dragondarts", Moves::Dragon_Darts },
			{ "dragonenergy", Moves::Dragon_Energy },
			{ "dragonhammer", Moves::Dragon_Hammer },
			{ "dragonpulse", Moves::Dragon_Pulse },
			{ "dragonrage", Moves::Dragon_Rage },
			{ "dragonrush", Moves::Dragon_Rush },
			{ "dragontail", Moves::Dragon_Tail },
			{ "drainingkiss", Moves::Draining_Kiss },
			{ "drainpunch", Moves::Drain_Punch },
			{ "dreameater", Moves::Dream_Eater },
			{ "drillpeck", Moves::Drill_Peck },
			{ "drillrun", Moves::Drill_Run },
			{ "drumbeating", Moves::Drum_Beating },
			{ "dualchop", Moves::Dual_Chop },
			{ "dualwingbeat", Moves::Dual_Wingbeat },
			{ "dynamaxcannon", Moves::Dynamax_Cannon },
			{ "dynamicpunch", Moves::Dynamic_Punch },
			{ "earthpower", Moves::Earth_Power },
			{ "earthquake", Moves::Earthquake },
			{ "echoedvoice", Moves::Echoed_Voice },
			{ "eerieimpulse", Moves::Eerie_Impulse },
			{ "eeriespell", Moves::Eerie_Spell },
			{ "eggbomb", Moves::Egg_Bomb },
			{ "electricterrain", Moves::Electric_Terrain },
			{ "electrify", Moves::Electrify },
			{ "electroball", Moves::Electro_Ball },
			{ "electroweb", Moves::Electroweb },
			{ "embargo", Moves::Embargo },
			{ "ember", Moves::Ember },
			{ "encore", Moves::Encore },
			{ "endeavor", Moves::Endeavor },
			{ "endure", Moves::Endure },
			{ "energyball", Moves::Energy_Ball },
			{ "entrainment", Moves::Entrainment },
			{ "eruption", Moves::Eruption },
			{ "eternabeam", Moves::Eternabeam },
			{ "expandingforce", Moves::Expanding_Force },
			{ "explosion", Moves::Explosion },
			{ "extrasensory", Moves::Extrasensory },
			{ "extremeevoboost", Moves::Extreme_Evoboost },
			{ "extremespeed", Moves::Extreme_Speed },
			{ "facade", Moves::Facade },
			{ "faintattack", Moves::Feint_Attack },
			{ "fairylock", Moves::Fairy_Lock },
			{ "fairywind", Moves::Fairy_Wind },
			{ "fakeout", Moves::Fake_Out },
			{ "faketears", Moves::Fake_Tears },
			{ "falsesurrender", Moves::False_Surrender },
			{ "falseswipe", Moves::False_Swipe },
			{ "featherdance", Moves::Feather_Dance },
			{ "feint", Moves::Feint },
			{ "feintattack", Moves::Feint_Attack },
			{ "fellstinger", Moves::Fell_Stinger },
			{ "fierydance", Moves::Fiery_Dance },
			{ "fierywrath", Moves::Fiery_Wrath },
			{ "finalgambit", Moves::Final_Gambit },
			{ "fireblast", Moves::Fire_Blast },
			{ "firefang", Moves::Fire_Fang },
			{ "firelash", Moves::Fire_Lash },
			{ "firepledge", Moves::Fire_Pledge },
			{ "firepunch", Moves::Fire_Punch },
			{ "firespin", Moves::Fire_Spin },
			{ "firstimpression", Moves::First_Impression },
			{ "fishiousrend", Moves::Fishious_Rend },
			{ "fissure", Moves::Fissure },
			{ "flail", Moves::Flail },
			{ "flameburst", Moves::Flame_Burst },
			{ "flamecharge", Moves::Flame_Charge },
			{ "flamethrower", Moves::Flamethrower },
			{ "flamewheel", Moves::Flame_Wheel },
			{ "flareblitz", Moves::Flare_Blitz },
			{ "flash", Moves::Flash },
			{ "flashcannon", Moves::Flash_Cannon },
			{ "flatter", Moves::Flatter },
			{ "fleurcannon", Moves::Fleur_Cannon },
			{ "fling", Moves::Fling },
			{ "flipturn", Moves::Flip_Turn },
			{ "floralhealing", Moves::Floral_Healing },
			{ "flowershield", Moves::Flower_Shield },
			{ "fly", Moves::Fly },
			{ "flyingpress", Moves::Flying_Press },
			{ "focusblast", Moves::Focus_Blast },
			{ "focusenergy", Moves::Focus_Energy },
			{ "focuspunch", Moves::Focus_Punch },
			{ "followme", Moves::Follow_Me },
			{ "forcepalm", Moves::Force_Palm },
			{ "foresight", Moves::Foresight },
			{ "forestscurse", Moves::Forests_Curse },
			{ "foulplay", Moves::Foul_Play },
			{ "freezedry", Moves::Freeze_Dry },
			{ "freezeshock", Moves::Freeze_Shock },
			{ "freezingglare", Moves::Freezing_Glare },
			{ "frenzyplant", Moves::Frenzy_Plant },
			{ "frostbreath", Moves::Frost_Breath },
			{ "frustration", Moves::Frustration },
			{ "furyattack", Moves::Fury_Attack },
			{ "furycutter", Moves::Fury_Cutter },
			{ "furyswipes", Moves::Fury_Swipes },
			{ "fusionbolt", Moves::Fusion_Bolt },
			{ "fusionflare", Moves::Fusion_Flare },
			{ "futuresight", Moves::Future_Sight },
			{ "gastroacid", Moves::Gastro_Acid },
			{ "geargrind", Moves::Gear_Grind },
			{ "gearup", Moves::Gear_Up },
			{ "genesissupernova", Moves::Genesis_Supernova },
			{ "geomancy", Moves::Geomancy },
			{ "gigadrain", Moves::Giga_Drain },
			{ "gigaimpact", Moves::Giga_Impact },
			{ "gigavolthavoc", Moves::Gigavolt_Havoc },
			{ "glaciallance", Moves::Glacial_Lance },
			{ "glaciate", Moves::Glaciate },
			{ "glare", Moves::Glare },
			{ "grassknot", Moves::Grass_Knot },
			{ "grasspledge", Moves::Grass_Pledge },
			{ "grasswhistle", Moves::Grass_Whistle },
			{ "grassyglide", Moves::Grassy_Glide },
			{ "grassyterrain", Moves::Grassy_Terrain },
			{ "gravapple", Moves::Grav_Apple },
			{ "gravity", Moves::Gravity },
			{ "growl", Moves::Growl },
			{ "growth", Moves::Growth },
			{ "grudge", Moves::Grudge },
			{ "guardianofalola", Moves::Guardian_of_Alola },
			{ "guardsplit", Moves::Guard_Split },
			{ "guardswap", Moves::Guard_Swap },
			{ "guillotine", Moves::Guillotine },
			{ "gunkshot", Moves::Gunk_Shot },
			{ "gust", Moves::Gust },
			{ "gyroball", Moves::Gyro_Ball },
			{ "hail", Moves::Hail },
			{ "hammerarm", Moves::Hammer_Arm },
			{ "happyhour", Moves::Happy_Hour },
			{ "harden", Moves::Harden },
			{ "haze", Moves::Haze },
			{ "headbutt", Moves::Headbutt },
			{ "headcharge", Moves::Head_Charge },
			{ "headsmash", Moves::Head_Smash },
			{ "healbell", Moves::Heal_Bell },
			{ "healblock", Moves::Heal_Block },
			{ "healingwish", Moves::Healing_Wish },
			{ "healorder", Moves::Heal_Order },
			{ "healpulse", Moves::Heal_Pulse },
			{ "heartstamp", Moves::Heart_Stamp },
			{ "heartswap", Moves::Heart_Swap },
			{ "heatcrash", Moves::Heat_Crash },
			{ "heatwave", Moves::Heat_Wave },
			{ "heavyslam", Moves::Heavy_Slam },
			{ "helpinghand", Moves::Helping_Hand },
			{ "hex", Moves::Hex },
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
			{ "highhorsepower", Moves::High_Horsepower },
			{ "highjumpkick", Moves::High_Jump_Kick },
			{ "hijumpkick", Moves::High_Jump_Kick },
			{ "hitselfinconfusion", Moves::Hit_Self },
			{ "holdback", Moves::Hold_Back },
			{ "holdhands", Moves::Hold_Hands },
			{ "honeclaws", Moves::Hone_Claws },
			{ "hornattack", Moves::Horn_Attack },
			{ "horndrill", Moves::Horn_Drill },
			{ "hornleech", Moves::Horn_Leech },
			{ "howl", Moves::Howl },
			{ "hurricane", Moves::Hurricane },
			{ "hydrocannon", Moves::Hydro_Cannon },
			{ "hydropump", Moves::Hydro_Pump },
			{ "hydrovortex", Moves::Hydro_Vortex },
			{ "hyperbeam", Moves::Hyper_Beam },
			{ "hyperfang", Moves::Hyper_Fang },
			{ "hyperspacefury", Moves::Hyperspace_Fury },
			{ "hyperspacehole", Moves::Hyperspace_Hole },
			{ "hypervoice", Moves::Hyper_Voice },
			{ "hypnosis", Moves::Hypnosis },
			{ "iceball", Moves::Ice_Ball },
			{ "icebeam", Moves::Ice_Beam },
			{ "iceburn", Moves::Ice_Burn },
			{ "icefang", Moves::Ice_Fang },
			{ "icehammer", Moves::Ice_Hammer },
			{ "icepunch", Moves::Ice_Punch },
			{ "iceshard", Moves::Ice_Shard },
			{ "iciclecrash", Moves::Icicle_Crash },
			{ "iciclespear", Moves::Icicle_Spear },
			{ "icywind", Moves::Icy_Wind },
			{ "imprison", Moves::Imprison },
			{ "incinerate", Moves::Incinerate },
			{ "inferno", Moves::Inferno },
			{ "infernooverdrive", Moves::Inferno_Overdrive },
			{ "infestation", Moves::Infestation },
			{ "ingrain", Moves::Ingrain },
			{ "instruct", Moves::Instruct },
			{ "iondeluge", Moves::Ion_Deluge },
			{ "irondefense", Moves::Iron_Defense },
			{ "ironhead", Moves::Iron_Head },
			{ "irontail", Moves::Iron_Tail },
			{ "jawlock", Moves::Jaw_Lock },
			{ "judgment", Moves::Judgment },
			{ "jumpkick", Moves::Jump_Kick },
			{ "junglehealing", Moves::Jungle_Healing },
			{ "karatechop", Moves::Karate_Chop },
			{ "kinesis", Moves::Kinesis },
			{ "kingsshield", Moves::Kings_Shield },
			{ "knockoff", Moves::Knock_Off },
			{ "landswrath", Moves::Lands_Wrath },
			{ "laserfocus", Moves::Laser_Focus },
			{ "lashout", Moves::Lash_Out },
			{ "lastresort", Moves::Last_Resort },
			{ "lavaplume", Moves::Lava_Plume },
			{ "leafage", Moves::Leafage },
			{ "leafblade", Moves::Leaf_Blade },
			{ "leafstorm", Moves::Leaf_Storm },
			{ "leaftornado", Moves::Leaf_Tornado },
			{ "leechlife", Moves::Leech_Life },
			{ "leechseed", Moves::Leech_Seed },
			{ "leer", Moves::Leer },
			{ "letssnuggleforever", Moves::Lets_Snuggle_Forever },
			{ "lick", Moves::Lick },
			{ "lifedew", Moves::Life_Dew },
			{ "lightofruin", Moves::Light_of_Ruin },
			{ "lightscreen", Moves::Light_Screen },
			{ "lightthatburnsthesky", Moves::Light_That_Burns_the_Sky },
			{ "liquidation", Moves::Liquidation },
			{ "lockon", Moves::Lock_On },
			{ "lovelykiss", Moves::Lovely_Kiss },
			{ "lowkick", Moves::Low_Kick },
			{ "lowsweep", Moves::Low_Sweep },
			{ "luckychant", Moves::Lucky_Chant },
			{ "lunardance", Moves::Lunar_Dance },
			{ "lunge", Moves::Lunge },
			{ "lusterpurge", Moves::Luster_Purge },
			{ "machpunch", Moves::Mach_Punch },
			{ "magicalleaf", Moves::Magical_Leaf },
			{ "magiccoat", Moves::Magic_Coat },
			{ "magicpowder", Moves::Magic_Powder },
			{ "magicroom", Moves::Magic_Room },
			{ "magmastorm", Moves::Magma_Storm },
			{ "magnetbomb", Moves::Magnet_Bomb },
			{ "magneticflux", Moves::Magnetic_Flux },
			{ "magnetrise", Moves::Magnet_Rise },
			{ "magnitude", Moves::Magnitude },
			{ "maliciousmoonsault", Moves::Malicious_Moonsault },
			{ "matblock", Moves::Mat_Block },
			{ "maxairstream", Moves::Max_Airstream },
			{ "maxdarkness", Moves::Max_Darkness },
			{ "maxflare", Moves::Max_Flare },
			{ "maxflutterby", Moves::Max_Flutterby },
			{ "maxgeyser", Moves::Max_Geyser },
			{ "maxguard", Moves::Max_Guard },
			{ "maxhailstorm", Moves::Max_Hailstorm },
			{ "maxknuckle", Moves::Max_Knuckle },
			{ "maxlightning", Moves::Max_Lightning },
			{ "maxmindstorm", Moves::Max_Mindstorm },
			{ "maxooze", Moves::Max_Ooze },
			{ "maxovergrowth", Moves::Max_Overgrowth },
			{ "maxphantasm", Moves::Max_Phantasm },
			{ "maxquake", Moves::Max_Quake },
			{ "maxrockfall", Moves::Max_Rockfall },
			{ "maxstarfall", Moves::Max_Starfall },
			{ "maxsteelspike", Moves::Max_Steelspike },
			{ "maxstrike", Moves::Max_Strike },
			{ "maxwyrmwind", Moves::Max_Wyrmwind },
			{ "meanlook", Moves::Mean_Look },
			{ "meditate", Moves::Meditate },
			{ "mefirst", Moves::Me_First },
			{ "megadrain", Moves::Mega_Drain },
			{ "megahorn", Moves::Megahorn },
			{ "megakick", Moves::Mega_Kick },
			{ "megapunch", Moves::Mega_Punch },
			{ "memento", Moves::Memento },
			{ "menacingmoonrazemaelstrom", Moves::Menacing_Moonraze_Maelstrom },
			{ "metalburst", Moves::Metal_Burst },
			{ "metalclaw", Moves::Metal_Claw },
			{ "metalsound", Moves::Metal_Sound },
			{ "meteorassault", Moves::Meteor_Assault },
			{ "meteorbeam", Moves::Meteor_Beam },
			{ "meteormash", Moves::Meteor_Mash },
			{ "metronome", Moves::Metronome },
			{ "milkdrink", Moves::Milk_Drink },
			{ "mimic", Moves::Mimic },
			{ "mindblown", Moves::Mind_Blown },
			{ "mindreader", Moves::Mind_Reader },
			{ "minimize", Moves::Minimize },
			{ "miracleeye", Moves::Miracle_Eye },
			{ "mirrorcoat", Moves::Mirror_Coat },
			{ "mirrormove", Moves::Mirror_Move },
			{ "mirrorshot", Moves::Mirror_Shot },
			{ "mist", Moves::Mist },
			{ "mistball", Moves::Mist_Ball },
			{ "mistyexplosion", Moves::Misty_Explosion },
			{ "mistyterrain", Moves::Misty_Terrain },
			{ "moonblast", Moves::Moonblast },
			{ "moongeistbeam", Moves::Moongeist_Beam },
			{ "moonlight", Moves::Moonlight },
			{ "morningsun", Moves::Morning_Sun },
			{ "mudbomb", Moves::Mud_Bomb },
			{ "muddywater", Moves::Muddy_Water },
			{ "mudshot", Moves::Mud_Shot },
			{ "mudslap", Moves::Mud_Slap },
			{ "mudsport", Moves::Mud_Sport },
			{ "multiattack", Moves::Multi_Attack },
			{ "mysticalfire", Moves::Mystical_Fire },
			{ "nastyplot", Moves::Nasty_Plot },
			{ "naturalgift", Moves::Natural_Gift },
			{ "naturepower", Moves::Nature_Power },
			{ "naturesmadness", Moves::Natures_Madness },
			{ "needlearm", Moves::Needle_Arm },
			{ "neverendingnightmare", Moves::Never_Ending_Nightmare },
			{ "nightdaze", Moves::Night_Daze },
			{ "nightmare", Moves::Nightmare },
			{ "nightshade", Moves::Night_Shade },
			{ "nightslash", Moves::Night_Slash },
			{ "nobleroar", Moves::Noble_Roar },
			{ "noretreat", Moves::No_Retreat },
			{ "nuzzle", Moves::Nuzzle },
			{ "oblivionwing", Moves::Oblivion_Wing },
			{ "obstruct", Moves::Obstruct },
			{ "oceanicoperetta", Moves::Oceanic_Operetta },
			{ "octazooka", Moves::Octazooka },
			{ "octolock", Moves::Octolock },
			{ "odorsleuth", Moves::Odor_Sleuth },
			{ "ominouswind", Moves::Ominous_Wind },
			{ "originpulse", Moves::Origin_Pulse },
			{ "outrage", Moves::Outrage },
			{ "overdrive", Moves::Overdrive },
			{ "overheat", Moves::Overheat },
			{ "painsplit", Moves::Pain_Split },
			{ "paraboliccharge", Moves::Parabolic_Charge },
			{ "partingshot", Moves::Parting_Shot },
			{ "pass", Moves::Pass },
			{ "payback", Moves::Payback },
			{ "payday", Moves::Pay_Day },
			{ "peck", Moves::Peck },
			{ "perishsong", Moves::Perish_Song },
			{ "petalblizzard", Moves::Petal_Blizzard },
			{ "petaldance", Moves::Petal_Dance },
			{ "phantomforce", Moves::Phantom_Force },
			{ "photongeyser", Moves::Photon_Geyser },
			{ "pinmissile", Moves::Pin_Missile },
			{ "plasmafists", Moves::Plasma_Fists },
			{ "playnice", Moves::Play_Nice },
			{ "playrough", Moves::Play_Rough },
			{ "pluck", Moves::Pluck },
			{ "poisonfang", Moves::Poison_Fang },
			{ "poisongas", Moves::Poison_Gas },
			{ "poisonjab", Moves::Poison_Jab },
			{ "poisonpowder", Moves::Poison_Powder },
			{ "poisonsting", Moves::Poison_Sting },
			{ "poisontail", Moves::Poison_Tail },
			{ "pollenpuff", Moves::Pollen_Puff },
			{ "poltergeist", Moves::Poltergeist },
			{ "pound", Moves::Pound },
			{ "powder", Moves::Powder },
			{ "powdersnow", Moves::Powder_Snow },
			{ "powergem", Moves::Power_Gem },
			{ "powersplit", Moves::Power_Split },
			{ "powerswap", Moves::Power_Swap },
			{ "powertrick", Moves::Power_Trick },
			{ "powertrip", Moves::Power_Trip },
			{ "poweruppunch", Moves::Power_Up_Punch },
			{ "powerwhip", Moves::Power_Whip },
			{ "precipiceblades", Moves::Precipice_Blades },
			{ "present", Moves::Present },
			{ "prismaticlaser", Moves::Prismatic_Laser },
			{ "protect", Moves::Protect },
			{ "psybeam", Moves::Psybeam },
			{ "psychic", Moves::Psychic },
			{ "psychicfangs", Moves::Psychic_Fangs },
			{ "psychicterrain", Moves::Psychic_Terrain },
			{ "psychoboost", Moves::Psycho_Boost },
			{ "psychocut", Moves::Psycho_Cut },
			{ "psychoshift", Moves::Psycho_Shift },
			{ "psychup", Moves::Psych_Up },
			{ "psyshock", Moves::Psyshock },
			{ "psystrike", Moves::Psystrike },
			{ "psywave", Moves::Psywave },
			{ "pulverizingpancake", Moves::Pulverizing_Pancake },
			{ "punishment", Moves::Punishment },
			{ "purify", Moves::Purify },
			{ "pursuit", Moves::Pursuit },
			{ "pyroball", Moves::Pyro_Ball },
			{ "quash", Moves::Quash },
			{ "quickattack", Moves::Quick_Attack },
			{ "quickguard", Moves::Quick_Guard },
			{ "quiverdance", Moves::Quiver_Dance },
			{ "rage", Moves::Rage },
			{ "ragepowder", Moves::Rage_Powder },
			{ "raindance", Moves::Rain_Dance },
			{ "rapidspin", Moves::Rapid_Spin },
			{ "razorleaf", Moves::Razor_Leaf },
			{ "razorshell", Moves::Razor_Shell },
			{ "razorwind", Moves::Razor_Wind },
			{ "recover", Moves::Recover },
			{ "recycle", Moves::Recycle },
			{ "reflect", Moves::Reflect },
			{ "reflecttype", Moves::Reflect_Type },
			{ "refresh", Moves::Refresh },
			{ "relicsong", Moves::Relic_Song },
			{ "rest", Moves::Rest },
			{ "retaliate", Moves::Retaliate },
			{ "return", Moves::Return },
			{ "return102", Moves::Return },
			{ "revelationdance", Moves::Revelation_Dance },
			{ "revenge", Moves::Revenge },
			{ "reversal", Moves::Reversal },
			{ "risingvoltage", Moves::Rising_Voltage },
			{ "roar", Moves::Roar },
			{ "roaroftime", Moves::Roar_of_Time },
			{ "rockblast", Moves::Rock_Blast },
			{ "rockclimb", Moves::Rock_Climb },
			{ "rockpolish", Moves::Rock_Polish },
			{ "rockslide", Moves::Rock_Slide },
			{ "rocksmash", Moves::Rock_Smash },
			{ "rockthrow", Moves::Rock_Throw },
			{ "rocktomb", Moves::Rock_Tomb },
			{ "rockwrecker", Moves::Rock_Wrecker },
			{ "roleplay", Moves::Role_Play },
			{ "rollingkick", Moves::Rolling_Kick },
			{ "rollout", Moves::Rollout },
			{ "roost", Moves::Roost },
			{ "rototiller", Moves::Rototiller },
			{ "round", Moves::Round },
			{ "sacredfire", Moves::Sacred_Fire },
			{ "sacredsword", Moves::Sacred_Sword },
			{ "safeguard", Moves::Safeguard },
			{ "sandattack", Moves::Sand_Attack },
			{ "sandstorm", Moves::Sandstorm },
			{ "sandtomb", Moves::Sand_Tomb },
			{ "savagespinout", Moves::Savage_Spin_Out },
			{ "scald", Moves::Scald },
			{ "scaleshot", Moves::Scale_Shot },
			{ "scaryface", Moves::Scary_Face },
			{ "scorchingsands", Moves::Scorching_Sands },
			{ "scratch", Moves::Scratch },
			{ "screech", Moves::Screech },
			{ "searingshot", Moves::Searing_Shot },
			{ "searingsunrazesmash", Moves::Searing_Sunraze_Smash },
			{ "secretpower", Moves::Secret_Power },
			{ "secretsword", Moves::Secret_Sword },
			{ "seedbomb", Moves::Seed_Bomb },
			{ "seedflare", Moves::Seed_Flare },
			{ "seismictoss", Moves::Seismic_Toss },
			{ "selfdestruct", Moves::Self_Destruct },
			{ "shadowball", Moves::Shadow_Ball },
			{ "shadowbone", Moves::Shadow_Bone },
			{ "shadowclaw", Moves::Shadow_Claw },
			{ "shadowforce", Moves::Shadow_Force },
			{ "shadowpunch", Moves::Shadow_Punch },
			{ "shadowsneak", Moves::Shadow_Sneak },
			{ "sharpen", Moves::Sharpen },
			{ "shatteredpsyche", Moves::Shattered_Psyche },
			{ "sheercold", Moves::Sheer_Cold },
			{ "shellsidearm", Moves::Shell_Side_Arm },
			{ "shellsmash", Moves::Shell_Smash },
			{ "shelltrap", Moves::Shell_Trap },
			{ "shiftgear", Moves::Shift_Gear },
			{ "shockwave", Moves::Shock_Wave },
			{ "shoreup", Moves::Shore_Up },
			{ "signalbeam", Moves::Signal_Beam },
			{ "silverwind", Moves::Silver_Wind },
			{ "simplebeam", Moves::Simple_Beam },
			{ "sing", Moves::Sing },
			{ "sinisterarrowraid", Moves::Sinister_Arrow_Raid },
			{ "sketch", Moves::Sketch },
			{ "skillswap", Moves::Skill_Swap },
			{ "skittersmack", Moves::Skitter_Smack },
			{ "skullbash", Moves::Skull_Bash },
			{ "skyattack", Moves::Sky_Attack },
			{ "skydrop", Moves::Sky_Drop },
			{ "skyuppercut", Moves::Sky_Uppercut },
			{ "slackoff", Moves::Slack_Off },
			{ "slam", Moves::Slam },
			{ "slash", Moves::Slash },
			{ "sleeppowder", Moves::Sleep_Powder },
			{ "sleeptalk", Moves::Sleep_Talk },
			{ "sludge", Moves::Sludge },
			{ "sludgebomb", Moves::Sludge_Bomb },
			{ "sludgewave", Moves::Sludge_Wave },
			{ "smackdown", Moves::Smack_Down },
			{ "smartstrike", Moves::Smart_Strike },
			{ "smellingsalt", Moves::Smelling_Salts },
			{ "smellingsalts", Moves::Smelling_Salts },
			{ "smog", Moves::Smog },
			{ "smokescreen", Moves::Smokescreen },
			{ "snaptrap", Moves::Snap_Trap },
			{ "snarl", Moves::Snarl },
			{ "snatch", Moves::Snatch },
			{ "snipeshot", Moves::Snipe_Shot },
			{ "snore", Moves::Snore },
			{ "soak", Moves::Soak },
			{ "softboiled", Moves::Soft_Boiled },
			{ "solarbeam", Moves::Solar_Beam },
			{ "solarblade", Moves::Solar_Blade },
			{ "sonicboom", Moves::Sonic_Boom },
			{ "soulstealing7starstrike", Moves::Soul_Stealing_7_Star_Strike },
			{ "spacialrend", Moves::Spacial_Rend },
			{ "spark", Moves::Spark },
			{ "sparklingaria", Moves::Sparkling_Aria },
			{ "spectralthief", Moves::Spectral_Thief },
			{ "speedswap", Moves::Speed_Swap },
			{ "spiderweb", Moves::Spider_Web },
			{ "spikecannon", Moves::Spike_Cannon },
			{ "spikes", Moves::Spikes },
			{ "spikyshield", Moves::Spiky_Shield },
			{ "spiritbreak", Moves::Spirit_Break },
			{ "spiritshackle", Moves::Spirit_Shackle },
			{ "spite", Moves::Spite },
			{ "spitup", Moves::Spit_Up },
			{ "splash", Moves::Splash },
			{ "splinteredstormshards", Moves::Splintered_Stormshards },
			{ "spore", Moves::Spore },
			{ "spotlight", Moves::Spotlight },
			{ "stealthrock", Moves::Stealth_Rock },
			{ "steameruption", Moves::Steam_Eruption },
			{ "steamroller", Moves::Steamroller },
			{ "steelbeam", Moves::Steel_Beam },
			{ "steelroller", Moves::Steel_Roller },
			{ "steelwing", Moves::Steel_Wing },
			{ "stickyweb", Moves::Sticky_Web },
			{ "stockpile", Moves::Stockpile },
			{ "stokedsparksurfer", Moves::Stoked_Sparksurfer },
			{ "stomp", Moves::Stomp },
			{ "stompingtantrum", Moves::Stomping_Tantrum },
			{ "stoneedge", Moves::Stone_Edge },
			{ "storedpower", Moves::Stored_Power },
			{ "stormthrow", Moves::Storm_Throw },
			{ "strangesteam", Moves::Strange_Steam },
			{ "strength", Moves::Strength },
			{ "strengthsap", Moves::Strength_Sap },
			{ "stringshot", Moves::String_Shot },
			{ "struggle", Moves::Struggle },
			{ "strugglebug", Moves::Struggle_Bug },
			{ "stuffcheeks", Moves::Stuff_Cheeks },
			{ "stunspore", Moves::Stun_Spore },
			{ "submission", Moves::Submission },
			{ "substitute", Moves::Substitute },
			{ "subzeroslammer", Moves::Subzero_Slammer },
			{ "suckerpunch", Moves::Sucker_Punch },
			{ "sunnyday", Moves::Sunny_Day },
			{ "sunsteelstrike", Moves::Sunsteel_Strike },
			{ "superfang", Moves::Super_Fang },
			{ "superpower", Moves::Superpower },
			{ "supersonic", Moves::Supersonic },
			{ "supersonicskystrike", Moves::Supersonic_Skystrike },
			{ "surf", Moves::Surf },
			{ "surgingstrikes", Moves::Surging_Strikes },
			{ "swagger", Moves::Swagger },
			{ "swallow", Moves::Swallow },
			{ "sweetkiss", Moves::Sweet_Kiss },
			{ "sweetscent", Moves::Sweet_Scent },
			{ "swift", Moves::Swift },
			{ "switch0", Moves::Switch0 },
			{ "switch1", Moves::Switch1 },
			{ "switch2", Moves::Switch2 },
			{ "switch3", Moves::Switch3 },
			{ "switch4", Moves::Switch4 },
			{ "switch5", Moves::Switch5 },
			{ "switcheroo", Moves::Switcheroo },
			{ "swordsdance", Moves::Swords_Dance },
			{ "synchronoise", Moves::Synchronoise },
			{ "synthesis", Moves::Synthesis },
			{ "tackle", Moves::Tackle },
			{ "tailglow", Moves::Tail_Glow },
			{ "tailslap", Moves::Tail_Slap },
			{ "tailwhip", Moves::Tail_Whip },
			{ "tailwind", Moves::Tailwind },
			{ "takedown", Moves::Take_Down },
			{ "tarshot", Moves::Tar_Shot },
			{ "taunt", Moves::Taunt },
			{ "tearfullook", Moves::Tearful_Look },
			{ "teatime", Moves::Teatime },
			{ "technoblast", Moves::Techno_Blast },
			{ "tectonicrage", Moves::Tectonic_Rage },
			{ "teeterdance", Moves::Teeter_Dance },
			{ "telekinesis", Moves::Telekinesis },
			{ "teleport", Moves::Teleport },
			{ "terrainpulse", Moves::Terrain_Pulse },
			{ "thief", Moves::Thief },
			{ "thousandarrows", Moves::Thousand_Arrows },
			{ "thousandwaves", Moves::Thousand_Waves },
			{ "thrash", Moves::Thrash },
			{ "throatchop", Moves::Throat_Chop },
			{ "thunder", Moves::Thunder },
			{ "thunderbolt", Moves::Thunderbolt },
			{ "thundercage", Moves::Thunder_Cage },
			{ "thunderfang", Moves::Thunder_Fang },
			{ "thunderouskick", Moves::Thunderous_Kick },
			{ "thunderpunch", Moves::Thunder_Punch },
			{ "thundershock", Moves::Thunder_Shock },
			{ "thunderwave", Moves::Thunder_Wave },
			{ "tickle", Moves::Tickle },
			{ "topsyturvy", Moves::Topsy_Turvy },
			{ "torment", Moves::Torment },
			{ "toxic", Moves::Toxic },
			{ "toxicspikes", Moves::Toxic_Spikes },
			{ "toxicthread", Moves::Toxic_Thread },
			{ "transform", Moves::Transform },
			{ "triattack", Moves::Tri_Attack },
			{ "trick", Moves::Trick },
			{ "trickortreat", Moves::Trick_or_Treat },
			{ "trickroom", Moves::Trick_Room },
			{ "tripleaxel", Moves::Triple_Axel },
			{ "triplekick", Moves::Triple_Kick },
			{ "tropkick", Moves::Trop_Kick },
			{ "trumpcard", Moves::Trump_Card },
			{ "twineedle", Moves::Twineedle },
			{ "twinkletackle", Moves::Twinkle_Tackle },
			{ "twister", Moves::Twister },
			{ "uproar", Moves::Uproar },
			{ "uturn", Moves::U_turn },
			{ "vacuumwave", Moves::Vacuum_Wave },
			{ "vcreate", Moves::V_create },
			{ "venomdrench", Moves::Venom_Drench },
			{ "venoshock", Moves::Venoshock },
			{ "vicegrip", Moves::Vise_Grip },
			{ "vinewhip", Moves::Vine_Whip },
			{ "visegrip", Moves::Vise_Grip },
			{ "vitalthrow", Moves::Vital_Throw },
			{ "voltswitch", Moves::Volt_Switch },
			{ "volttackle", Moves::Volt_Tackle },
			{ "wakeupslap", Moves::Wake_Up_Slap },
			{ "waterfall", Moves::Waterfall },
			{ "watergun", Moves::Water_Gun },
			{ "waterpledge", Moves::Water_Pledge },
			{ "waterpulse", Moves::Water_Pulse },
			{ "watershuriken", Moves::Water_Shuriken },
			{ "watersport", Moves::Water_Sport },
			{ "waterspout", Moves::Water_Spout },
			{ "weatherball", Moves::Weather_Ball },
			{ "whirlpool", Moves::Whirlpool },
			{ "whirlwind", Moves::Whirlwind },
			{ "wickedblow", Moves::Wicked_Blow },
			{ "wideguard", Moves::Wide_Guard },
			{ "wildcharge", Moves::Wild_Charge },
			{ "willowisp", Moves::Will_O_Wisp },
			{ "wingattack", Moves::Wing_Attack },
			{ "wish", Moves::Wish },
			{ "withdraw", Moves::Withdraw },
			{ "wonderroom", Moves::Wonder_Room },
			{ "woodhammer", Moves::Wood_Hammer },
			{ "workup", Moves::Work_Up },
			{ "worryseed", Moves::Worry_Seed },
			{ "wrap", Moves::Wrap },
			{ "wringout", Moves::Wring_Out },
			{ "xscissor", Moves::X_Scissor },
			{ "yawn", Moves::Yawn },
			{ "zapcannon", Moves::Zap_Cannon },
			{ "zenheadbutt", Moves::Zen_Headbutt },
			{ "zingzap", Moves::Zing_Zap },
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<25_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Move", str);
	}
	return *result;
}

}	// namespace technicalmachine
