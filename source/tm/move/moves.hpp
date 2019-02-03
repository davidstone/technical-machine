// Moves enum
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

#pragma once

#include <tm/enum.hpp>

#include <bounded/integer.hpp>

#include <cstdint>

namespace technicalmachine {

enum class Moves : uint16_t {
	// Weird moves
	Pass,
	Switch0,
	Switch1,
	Switch2,
	Switch3,
	Switch4,
	Switch5,
	Hit_Self,
	
	// Generation 1
	Regular_Begin,
	Pound = Regular_Begin,
	Karate_Chop,
	DoubleSlap,
	Comet_Punch,
	Mega_Punch,
	Pay_Day,
	Fire_Punch,
	Ice_Punch,
	ThunderPunch,
	Scratch,
	ViceGrip,
	Guillotine,
	Razor_Wind,
	Swords_Dance,
	Cut,
	Gust,
	Wing_Attack,
	Whirlwind,
	Fly,
	Bind,
	Slam,
	Vine_Whip,
	Stomp,
	Double_Kick,
	Mega_Kick,
	Jump_Kick,
	Rolling_Kick,
	Sand_Attack,
	Headbutt,
	Horn_Attack,
	Fury_Attack,
	Horn_Drill,
	Tackle,
	Body_Slam,
	Wrap,
	Take_Down,
	Thrash,
	Double_Edge,
	Tail_Whip,
	Poison_Sting,
	Twineedle,
	Pin_Missile,
	Leer,
	Bite,
	Growl,
	Roar,
	Sing,
	Supersonic,
	SonicBoom,
	Disable,
	Acid,
	Ember,
	Flamethrower,
	Mist,
	Water_Gun,
	Hydro_Pump,
	Surf,
	Ice_Beam,
	Blizzard,
	Psybeam,
	BubbleBeam,
	Aurora_Beam,
	Hyper_Beam,
	Peck,
	Drill_Peck,
	Submission,
	Low_Kick,
	Counter,
	Seismic_Toss,
	Strength,
	Absorb,
	Mega_Drain,
	Leech_Seed,
	Growth,
	Razor_Leaf,
	SolarBeam,
	PoisonPowder,
	Stun_Spore,
	Sleep_Powder,
	Petal_Dance,
	String_Shot,
	Dragon_Rage,
	Fire_Spin,
	ThunderShock,
	Thunderbolt,
	Thunder_Wave,
	Thunder,
	Rock_Throw,
	Earthquake,
	Fissure,
	Dig,
	Toxic,
	Confusion,
	Psychic,
	Hypnosis,
	Meditate,
	Agility,
	Quick_Attack,
	Rage,
	Teleport,
	Night_Shade,
	Mimic,
	Screech,
	Double_Team,
	Recover,
	Harden,
	Minimize,
	SmokeScreen,
	Confuse_Ray,
	Withdraw,
	Defense_Curl,
	Barrier,
	Light_Screen,
	Haze,
	Reflect,
	Focus_Energy,
	Bide,
	Metronome,
	Mirror_Move,
	Selfdestruct,
	Egg_Bomb,
	Lick,
	Smog,
	Sludge,
	Bone_Club,
	Fire_Blast,
	Waterfall,
	Clamp,
	Swift,
	Skull_Bash,
	Spike_Cannon,
	Constrict,
	Amnesia,
	Kinesis,
	Softboiled,
	Hi_Jump_Kick,
	Glare,
	Dream_Eater,
	Poison_Gas,
	Barrage,
	Leech_Life,
	Lovely_Kiss,
	Sky_Attack,
	Transform,
	Bubble,
	Dizzy_Punch,
	Spore,
	Flash,
	Psywave,
	Splash,
	Acid_Armor,
	Crabhammer,
	Explosion,
	Fury_Swipes,
	Bonemerang,
	Rest,
	Rock_Slide,
	Hyper_Fang,
	Sharpen,
	Conversion,
	Tri_Attack,
	Super_Fang,
	Slash,
	Substitute,
	Struggle,
	
	// Generation 2
	Sketch,
	Triple_Kick,
	Thief,
	Spider_Web,
	Mind_Reader,
	Nightmare,
	Flame_Wheel,
	Snore,
	Curse,
	Flail,
	Conversion_2,
	Aeroblast,
	Cotton_Spore,
	Reversal,
	Spite,
	Powder_Snow,
	Protect,
	Mach_Punch,
	Scary_Face,
	Faint_Attack,
	Sweet_Kiss,
	Belly_Drum,
	Sludge_Bomb,
	Mud_Slap,
	Octazooka,
	Spikes,
	Zap_Cannon,
	Foresight,
	Destiny_Bond,
	Perish_Song,
	Icy_Wind,
	Detect,
	Bone_Rush,
	Lock_On,
	Outrage,
	Sandstorm,
	Giga_Drain,
	Endure,
	Charm,
	Rollout,
	False_Swipe,
	Swagger,
	Milk_Drink,
	Spark,
	Fury_Cutter,
	Steel_Wing,
	Mean_Look,
	Attract,
	Sleep_Talk,
	Heal_Bell,
	Return,
	Present,
	Frustration,
	Safeguard,
	Pain_Split,
	Sacred_Fire,
	Magnitude,
	DynamicPunch,
	Megahorn,
	DragonBreath,
	Baton_Pass,
	Encore,
	Pursuit,
	Rapid_Spin,
	Sweet_Scent,
	Iron_Tail,
	Metal_Claw,
	Vital_Throw,
	Morning_Sun,
	Synthesis,
	Moonlight,
	Hidden_Power,
	Cross_Chop,
	Twister,
	Rain_Dance,
	Sunny_Day,
	Crunch,
	Mirror_Coat,
	Psych_Up,
	ExtremeSpeed,
	AncientPower,
	Shadow_Ball,
	Future_Sight,
	Rock_Smash,
	Whirlpool,
	Beat_Up,
	
	// Generation 3
	Fake_Out,
	Uproar,
	Stockpile,
	Spit_Up,
	Swallow,
	Heat_Wave,
	Hail,
	Torment,
	Flatter,
	Will_O_Wisp,
	Memento,
	Facade,
	Focus_Punch,
	SmellingSalt,
	Follow_Me,
	Nature_Power,
	Charge,
	Taunt,
	Helping_Hand,
	Trick,
	Role_Play,
	Wish,
	Assist,
	Ingrain,
	Superpower,
	Magic_Coat,
	Recycle,
	Revenge,
	Brick_Break,
	Yawn,
	Knock_Off,
	Endeavor,
	Eruption,
	Skill_Swap,
	Imprison,
	Refresh,
	Grudge,
	Snatch,
	Secret_Power,
	Dive,
	Arm_Thrust,
	Camouflage,
	Tail_Glow,
	Luster_Purge,
	Mist_Ball,
	FeatherDance,
	Teeter_Dance,
	Blaze_Kick,
	Mud_Sport,
	Ice_Ball,
	Needle_Arm,
	Slack_Off,
	Hyper_Voice,
	Poison_Fang,
	Crush_Claw,
	Blast_Burn,
	Hydro_Cannon,
	Meteor_Mash,
	Astonish,
	Weather_Ball,
	Aromatherapy,
	Fake_Tears,
	Air_Cutter,
	Overheat,
	Odor_Sleuth,
	Rock_Tomb,
	Silver_Wind,
	Metal_Sound,
	GrassWhistle,
	Tickle,
	Cosmic_Power,
	Water_Spout,
	Signal_Beam,
	Shadow_Punch,
	Extrasensory,
	Sky_Uppercut,
	Sand_Tomb,
	Sheer_Cold,
	Muddy_Water,
	Bullet_Seed,
	Aerial_Ace,
	Icicle_Spear,
	Iron_Defense,
	Block,
	Howl,
	Dragon_Claw,
	Frenzy_Plant,
	Bulk_Up,
	Bounce,
	Mud_Shot,
	Poison_Tail,
	Covet,
	Volt_Tackle,
	Magical_Leaf,
	Water_Sport,
	Calm_Mind,
	Leaf_Blade,
	Dragon_Dance,
	Rock_Blast,
	Shock_Wave,
	Water_Pulse,
	Doom_Desire,
	Psycho_Boost,
	
	// Generation 4
	Roost,
	Gravity,
	Miracle_Eye,
	Wake_Up_Slap,
	Hammer_Arm,
	Gyro_Ball,
	Healing_Wish,
	Brine,
	Natural_Gift,
	Feint,
	Pluck,
	Tailwind,
	Acupressure,
	Metal_Burst,
	U_turn,
	Close_Combat,
	Payback,
	Assurance,
	Embargo,
	Fling,
	Psycho_Shift,
	Trump_Card,
	Heal_Block,
	Wring_Out,
	Power_Trick,
	Gastro_Acid,
	Lucky_Chant,
	Me_First,
	Copycat,
	Power_Swap,
	Guard_Swap,
	Punishment,
	Last_Resort,
	Worry_Seed,
	Sucker_Punch,
	Toxic_Spikes,
	Heart_Swap,
	Aqua_Ring,
	Magnet_Rise,
	Flare_Blitz,
	Force_Palm,
	Aura_Sphere,
	Rock_Polish,
	Poison_Jab,
	Dark_Pulse,
	Night_Slash,
	Aqua_Tail,
	Seed_Bomb,
	Air_Slash,
	X_Scissor,
	Bug_Buzz,
	Dragon_Pulse,
	Dragon_Rush,
	Power_Gem,
	Drain_Punch,
	Vacuum_Wave,
	Focus_Blast,
	Energy_Ball,
	Brave_Bird,
	Earth_Power,
	Switcheroo,
	Giga_Impact,
	Nasty_Plot,
	Bullet_Punch,
	Avalanche,
	Ice_Shard,
	Shadow_Claw,
	Thunder_Fang,
	Ice_Fang,
	Fire_Fang,
	Shadow_Sneak,
	Mud_Bomb,
	Psycho_Cut,
	Zen_Headbutt,
	Mirror_Shot,
	Flash_Cannon,
	Rock_Climb,
	Defog,
	Trick_Room,
	Draco_Meteor,
	Discharge,
	Lava_Plume,
	Leaf_Storm,
	Power_Whip,
	Rock_Wrecker,
	Cross_Poison,
	Gunk_Shot,
	Iron_Head,
	Magnet_Bomb,
	Stone_Edge,
	Captivate,
	Stealth_Rock,
	Grass_Knot,
	Chatter,
	Judgment,
	Bug_Bite,
	Charge_Beam,
	Wood_Hammer,
	Aqua_Jet,
	Attack_Order,
	Defend_Order,
	Heal_Order,
	Head_Smash,
	Double_Hit,
	Roar_of_Time,
	Spacial_Rend,
	Lunar_Dance,
	Crush_Grip,
	Magma_Storm,
	Dark_Void,
	Seed_Flare,
	Ominous_Wind,
	Shadow_Force,
	
	// Generation 5
	Hone_Claws,
	Wide_Guard,
	Guard_Split,
	Power_Split,
	Wonder_Room,
	Psyshock,
	Venoshock,
	Autotomize,
	Rage_Powder,
	Telekinesis,
	Magic_Room,
	Smack_Down,
	Storm_Throw,
	Flame_Burst,
	Sludge_Wave,
	Quiver_Dance,
	Heavy_Slam,
	Synchronoise,
	Electro_Ball,
	Soak,
	Flame_Charge,
	Coil,
	Low_Sweep,
	Acid_Spray,
	Foul_Play,
	Simple_Beam,
	Entrainment,
	After_You,
	Round,
	Echoed_Voice,
	Chip_Away,
	Clear_Smog,
	Stored_Power,
	Quick_Guard,
	Ally_Switch,
	Scald,
	Shell_Smash,
	Heal_Pulse,
	Hex,
	Sky_Drop,
	Shift_Gear,
	Circle_Throw,
	Incinerate,
	Quash,
	Acrobatics,
	Reflect_Type,
	Retaliate,
	Final_Gambit,
	Bestow,
	Inferno,
	Water_Pledge,
	Fire_Pledge,
	Grass_Pledge,
	Volt_Switch,
	Struggle_Bug,
	Bulldoze,
	Frost_Breath,
	Dragon_Tail,
	Work_Up,
	Electroweb,
	Wild_Charge,
	Drill_Run,
	Dual_Chop,
	Heart_Stamp,
	Horn_Leech,
	Sacred_Sword,
	Razor_Shell,
	Heat_Crash,
	Leaf_Tornado,
	Steamroller,
	Cotton_Guard,
	Night_Daze,
	Psystrike,
	Tail_Slap,
	Hurricane,
	Head_Charge,
	Gear_Grind,
	Searing_Shot,
	Techno_Blast,
	Relic_Song,
	Secret_Sword,
	Glaciate,
	Bolt_Strike,
	Blue_Flare,
	Fiery_Dance,
	Freeze_Shock,
	Ice_Burn,
	Snarl,
	Icicle_Crash,
	V_create,
	Fusion_Flare,
	Fusion_Bolt
};

}	// namespace technicalmachine

namespace std {

template<>
struct numeric_limits<technicalmachine::Moves> : technicalmachine::enum_numeric_limits<technicalmachine::Moves::Fusion_Bolt> {};

}	// namespace std