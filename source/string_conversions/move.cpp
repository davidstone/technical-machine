// Move string conversions
// Copyright (C) 2012 David Stone
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

#include "conversion.hpp"

#include <cassert>
#include <map>
#include <string>

#include <boost/algorithm/string/case_conv.hpp>

#include "invalid_string_conversion.hpp"

#include "../move/move.hpp"
#include "../move/moves.hpp"

namespace technicalmachine {

template<>
std::string to_string(Moves const name) {
	assert(name <= Moves::END);
	static std::string const name_to_string [] = {
		// Generation 1
		"Switch0", "Switch1", "Switch2", "Switch3", "Switch4",
		"Switch5", "Hit self in confusion", "Pound", "Karate Chop", "DoubleSlap",
		"Comet Punch", "Mega Punch", "Pay Day", "Fire Punch", "Ice Punch",
		"ThunderPunch", "Scratch", "ViceGrip", "Guillotine", "Razor Wind",
		"Swords Dance", "Cut", "Gust", "Wing Attack", "Whirlwind",
		"Fly", "Bind", "Slam", "Vine Whip", "Stomp",
		"Double Kick", "Mega Kick", "Jump Kick", "Rolling Kick", "Sand-Attack",
		"Headbutt", "Horn Attack", "Fury Attack", "Horn Drill", "Tackle",
		"Body Slam", "Wrap", "Take Down", "Thrash", "Double-Edge",
		"Tail Whip", "Poison Sting", "Twineedle", "Pin Missile", "Leer",
		"Bite", "Growl", "Roar", "Sing", "Supersonic",
		"SonicBoom", "Disable", "Acid", "Ember", "Flamethrower",
		"Mist", "Water Gun", "Hydro Pump", "Surf", "Ice Beam",
		"Blizzard", "Psybeam", "BubbleBeam", "Aurora Beam", "Hyper Beam",
		"Peck", "Drill Peck", "Submission", "Low Kick", "Counter",
		"Seismic Toss", "Strength", "Absorb", "Mega Drain", "Leech Seed",
		"Growth", "Razor Leaf", "SolarBeam", "PoisonPowder", "Stun Spore",
		"Sleep Powder", "Petal Dance", "String Shot", "Dragon Rage", "Fire Spin",
		"ThunderShock", "Thunderbolt", "Thunder Wave", "Thunder", "Rock Throw",
		"Earthquake", "Fissure", "Dig", "Toxic", "Confusion",
		"Psychic", "Hypnosis", "Meditate", "Agility", "Quick Attack",
		"Rage", "Teleport", "Night Shade", "Mimic", "Screech",
		"Double Team", "Recover", "Harden", "Minimize", "SmokeScreen",
		"Confuse Ray", "Withdraw", "Defense Curl", "Barrier", "Light Screen",
		"Haze", "Reflect", "Focus Energy", "Bide", "Metronome",
		"Mirror Move", "Selfdestruct", "Egg Bomb", "Lick", "Smog",
		"Sludge", "Bone Club", "Fire Blast", "Waterfall", "Clamp",
		"Swift", "Skull Bash", "Spike Cannon", "Constrict", "Amnesia",
		"Kinesis", "Softboiled", "Hi Jump Kick", "Glare", "Dream Eater",
		"Poison Gas", "Barrage", "Leech Life", "Lovely Kiss", "Sky Attack",
		"Transform", "Bubble", "Dizzy Punch", "Spore", "Flash",
		"Psywave", "Splash", "Acid Armor", "Crabhammer", "Explosion",
		"Fury Swipes", "Bonemerang", "Rest", "Rock Slide", "Hyper Fang",
		"Sharpen", "Conversion", "Tri Attack", "Super Fang", "Slash",
		"Substitute", "Struggle",

		// Generation 2
		"Sketch", "Triple Kick", "Thief", "Spider Web", "Mind Reader",
		"Nightmare", "Flame Wheel", "Snore", "Curse", "Flail",
		"Conversion 2", "Aeroblast", "Cotton Spore", "Reversal", "Spite",
		"Powder Snow", "Protect", "Mach Punch", "Scary Face", "Faint Attack",
		"Sweet Kiss", "Belly Drum", "Sludge Bomb", "Mud-Slap", "Octazooka",
		"Spikes", "Zap Cannon", "Foresight", "Destiny Bond", "Perish Song",
		"Icy Wind", "Detect", "Bone Rush", "Lock-On", "Outrage",
		"Sandstorm", "Giga Drain", "Endure", "Charm", "Rollout",
		"False Swipe", "Swagger", "Milk Drink", "Spark", "Fury Cutter",
		"Steel Wing", "Mean Look", "Attract", "Sleep Talk", "Heal Bell",
		"Return", "Present", "Frustration", "Safeguard", "Pain Split",
		"Sacred Fire", "Magnitude", "DynamicPunch", "Megahorn", "DragonBreath",
		"Baton Pass", "Encore", "Pursuit", "Rapid Spin", "Sweet Scent",
		"Iron Tail", "Metal Claw", "Vital Throw", "Morning Sun", "Synthesis",
		"Moonlight", "Hidden Power", "Cross Chop", "Twister", "Rain Dance",
		"Sunny Day", "Crunch", "Mirror Coat", "Psych Up", "ExtremeSpeed",
		"AncientPower", "Shadow Ball", "Future Sight", "Rock Smash", "Whirlpool",
		"Beat Up",

		// Generation 3
		"Fake Out", "Uproar", "Stockpile", "Spit Up", "Swallow",
		"Heat Wave", "Hail", "Torment", "Flatter", "Will-O-Wisp",
		"Memento", "Facade", "Focus Punch", "SmellingSalt", "Follow Me",
		"Nature Power", "Charge", "Taunt", "Helping Hand", "Trick",
		"Role Play", "Wish", "Assist", "Ingrain", "Superpower",
		"Magic Coat", "Recycle", "Revenge", "Brick Break", "Yawn",
		"Knock Off", "Endeavor", "Eruption", "Skill Swap", "Imprison",
		"Refresh", "Grudge", "Snatch", "Secret Power", "Dive",
		"Arm Thrust", "Camouflage", "Tail Glow", "Luster Purge", "Mist Ball",
		"FeatherDance", "Teeter Dance", "Blaze Kick", "Mud Sport", "Ice Ball",
		"Needle Arm", "Slack Off", "Hyper Voice", "Poison Fang", "Crush Claw",
		"Blast Burn", "Hydro Cannon", "Meteor Mash", "Astonish", "Weather Ball",
		"Aromatherapy", "Fake Tears", "Air Cutter", "Overheat", "Odor Sleuth",
		"Rock Tomb", "Silver Wind", "Metal Sound", "GrassWhistle", "Tickle",
		"Cosmic Power", "Water Spout", "Signal Beam", "Shadow Punch", "Extrasensory",
		"Sky Uppercut", "Sand Tomb", "Sheer Cold", "Muddy Water", "Bullet Seed",
		"Aerial Ace", "Icicle Spear", "Iron Defense", "Block", "Howl",
		"Dragon Claw", "Frenzy Plant", "Bulk Up", "Bounce", "Mud Shot",
		"Poison Tail", "Covet", "Volt Tackle", "Magical Leaf", "Water Sport",
		"Calm Mind", "Leaf Blade", "Dragon Dance", "Rock Blast", "Shock Wave",
		"Water Pulse", "Doom Desire", "Psycho Boost",

		// Generation 4
		"Roost", "Gravity", "Miracle Eye", "Wake-Up Slap", "Hammer Arm",
		"Gyro Ball", "Healing Wish", "Brine", "Natural Gift", "Feint",
		"Pluck", "Tailwind", "Acupressure", "Metal Burst", "U-turn",
		"Close Combat", "Payback", "Assurance", "Embargo", "Fling",
		"Psycho Shift", "Trump Card", "Heal Block", "Wring Out", "Power Trick",
		"Gastro Acid", "Lucky Chant", "Me First", "Copycat", "Power Swap",
		"Guard Swap", "Punishment", "Last Resort", "Worry Seed", "Sucker Punch",
		"Toxic Spikes", "Heart Swap", "Aqua Ring", "Magnet Rise", "Flare Blitz",
		"Force Palm", "Aura Sphere", "Rock Polish", "Poison Jab", "Dark Pulse",
		"Night Slash", "Aqua Tail", "Seed Bomb", "Air Slash", "X-Scissor",
		"Bug Buzz", "Dragon Pulse", "Dragon Rush", "Power Gem", "Drain Punch",
		"Vacuum Wave", "Focus Blast", "Energy Ball", "Brave Bird", "Earth Power",
		"Switcheroo", "Giga Impact", "Nasty Plot", "Bullet Punch", "Avalanche",
		"Ice Shard", "Shadow Claw", "Thunder Fang", "Ice Fang", "Fire Fang",
		"Shadow Sneak", "Mud Bomb", "Psycho Cut", "Zen Headbutt", "Mirror Shot",
		"Flash Cannon", "Rock Climb", "Defog", "Trick Room", "Draco Meteor",
		"Discharge", "Lava Plume", "Leaf Storm", "Power Whip", "Rock Wrecker",
		"Cross Poison", "Gunk Shot", "Iron Head", "Magnet Bomb", "Stone Edge",
		"Captivate", "Stealth Rock", "Grass Knot", "Chatter", "Judgment",
		"Bug Bite", "Charge Beam", "Wood Hammer", "Aqua Jet", "Attack Order",
		"Defend Order", "Heal Order", "Head Smash", "Double Hit",
		"Roar of Time", "Spacial Rend", "Lunar Dance", "Crush Grip", "Magma Storm",
		"Dark Void", "Seed Flare", "Ominous Wind", "Shadow Force",

		// Generation 5
		"Hone Claws", "Wide Guard", "Guard Split", "Power Split", "Wonder Room",
		"Psyshock", "Venoshock", "Autotomize", "Rage Powder", "Telekinesis",
		"Magic Room", "Smack Down", "Storm Throw", "Flame Burst", "Sludge Wave",
		"Quiver Dance", "Heavy Slam", "Synchronoise", "Electro Ball", "Soak",
		"Flame Charge", "Coil", "Low Sweep", "Acid Spray", "Foul Play",
		"Simple Beam", "Entrainment", "After You", "Round", "Echoed Voice",
		"Chip Away", "Clear Smog", "Stored Power", "Quick Guard", "Ally Switch",
		"Scald", "Shell Smash", "Heal Pulse", "Hex", "Sky Drop",
		"Shift Gear", "Circle Throw", "Incinerate", "Quash", "Acrobatics",
		"Reflect Type", "Retaliate", "Final Gambit", "Bestow", "Inferno",
		"Water Pledge", "Fire Pledge", "Grass Pledge", "Volt Switch", "Struggle Bug",
		"Bulldoze", "Frost Breath", "Dragon Tail", "Work Up", "Electroweb",
		"Wild Charge", "Drill Run", "Dual Chop", "Heart Stamp", "Horn Leech",
		"Sacred Sword", "Razor Shell", "Heat Crash", "Leaf Tornado", "Steamroller",
		"Cotton Guard", "Night Daze", "Psystrike", "Tail Slap", "Hurricane",
		"Head Charge", "Gear Grind", "Searing Shot", "Techno Blast", "Relic Song",
		"Secret Sword", "Glaciate", "Bolt Strike", "Blue Flare", "Fiery Dance",
		"Freeze Shock", "Ice Burn", "Snarl", "Icicle Crash", "V-create",
		"Fusion Flare", "Fusion Bolt",

		"END_MOVE"
	};
	return name_to_string[static_cast<size_t>(name)];
}

template<>
Moves from_string(std::string const & str) {
	static std::map<std::string, Moves> const converter {
		{ "switch0", Moves::Switch0 },
		{ "switch1", Moves::Switch1 },
		{ "switch2", Moves::Switch2 },
		{ "switch3", Moves::Switch3 },
		{ "switch4", Moves::Switch4 },
		{ "switch5", Moves::Switch5 },
		{ "hit self in confusion", Moves::Hit_Self },
		{ "pound", Moves::Pound },
		{ "karate chop", Moves::Karate_Chop },
		{ "doubleslap", Moves::DoubleSlap },
		{ "comet punch", Moves::Comet_Punch },
		{ "mega punch", Moves::Mega_Punch },
		{ "pay day", Moves::Pay_Day },
		{ "fire punch", Moves::Fire_Punch },
		{ "ice punch", Moves::Ice_Punch },
		{ "thunderpunch", Moves::ThunderPunch },
		{ "scratch", Moves::Scratch },
		{ "vicegrip", Moves::ViceGrip },
		{ "guillotine", Moves::Guillotine },
		{ "razor wind", Moves::Razor_Wind },
		{ "swords dance", Moves::Swords_Dance },
		{ "cut", Moves::Cut },
		{ "gust", Moves::Gust },
		{ "wing attack", Moves::Wing_Attack },
		{ "whirlwind", Moves::Whirlwind },
		{ "fly", Moves::Fly },
		{ "bind", Moves::Bind },
		{ "slam", Moves::Slam },
		{ "vine whip", Moves::Vine_Whip },
		{ "stomp", Moves::Stomp },
		{ "double kick", Moves::Double_Kick },
		{ "mega kick", Moves::Mega_Kick },
		{ "jump kick", Moves::Jump_Kick },
		{ "rolling kick", Moves::Rolling_Kick },
		{ "sand-attack", Moves::Sand_Attack },
		{ "headbutt", Moves::Headbutt },
		{ "horn attack", Moves::Horn_Attack },
		{ "fury attack", Moves::Fury_Attack },
		{ "horn drill", Moves::Horn_Drill },
		{ "tackle", Moves::Tackle },
		{ "body slam", Moves::Body_Slam },
		{ "wrap", Moves::Wrap },
		{ "take down", Moves::Take_Down },
		{ "thrash", Moves::Thrash },
		{ "double-edge", Moves::Double_Edge },
		{ "tail whip", Moves::Tail_Whip },
		{ "poison sting", Moves::Poison_Sting },
		{ "twineedle", Moves::Twineedle },
		{ "pin missile", Moves::Pin_Missile },
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
		{ "hydro pump", Moves::Hydro_Pump },
		{ "surf", Moves::Surf },
		{ "ice beam", Moves::Ice_Beam },
		{ "blizzard", Moves::Blizzard },
		{ "psybeam", Moves::Psybeam },
		{ "bubblebeam", Moves::BubbleBeam },
		{ "aurora beam", Moves::Aurora_Beam },
		{ "hyper beam", Moves::Hyper_Beam },
		{ "peck", Moves::Peck },
		{ "drill peck", Moves::Drill_Peck },
		{ "submission", Moves::Submission },
		{ "low kick", Moves::Low_Kick },
		{ "counter", Moves::Counter },
		{ "seismic toss", Moves::Seismic_Toss },
		{ "strength", Moves::Strength },
		{ "absorb", Moves::Absorb },
		{ "mega drain", Moves::Mega_Drain },
		{ "leech seed", Moves::Leech_Seed },
		{ "growth", Moves::Growth },
		{ "razor leaf", Moves::Razor_Leaf },
		{ "solarbeam", Moves::SolarBeam },
		{ "poisonpowder", Moves::PoisonPowder },
		{ "stun spore", Moves::Stun_Spore },
		{ "sleep powder", Moves::Sleep_Powder },
		{ "petal dance", Moves::Petal_Dance },
		{ "string shot", Moves::String_Shot },
		{ "dragon rage", Moves::Dragon_Rage },
		{ "fire spin", Moves::Fire_Spin },
		{ "thundershock", Moves::ThunderShock },
		{ "thunderbolt", Moves::Thunderbolt },
		{ "thunder wave", Moves::Thunder_Wave },
		{ "thunder", Moves::Thunder },
		{ "rock throw", Moves::Rock_Throw },
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
		{ "rage", Moves::Rage },
		{ "teleport", Moves::Teleport },
		{ "night shade", Moves::Night_Shade },
		{ "mimic", Moves::Mimic },
		{ "screech", Moves::Screech },
		{ "double team", Moves::Double_Team },
		{ "recover", Moves::Recover },
		{ "harden", Moves::Harden },
		{ "minimize", Moves::Minimize },
		{ "smokescreen", Moves::SmokeScreen },
		{ "confuse ray", Moves::Confuse_Ray },
		{ "withdraw", Moves::Withdraw },
		{ "defense curl", Moves::Defense_Curl },
		{ "barrier", Moves::Barrier },
		{ "light screen", Moves::Light_Screen },
		{ "haze", Moves::Haze },
		{ "reflect", Moves::Reflect },
		{ "focus energy", Moves::Focus_Energy },
		{ "bide", Moves::Bide },
		{ "metronome", Moves::Metronome },
		{ "mirror move", Moves::Mirror_Move },
		{ "selfdestruct", Moves::Selfdestruct },
		{ "egg bomb", Moves::Egg_Bomb },
		{ "lick", Moves::Lick },
		{ "smog", Moves::Smog },
		{ "sludge", Moves::Sludge },
		{ "bone club", Moves::Bone_Club },
		{ "fire blast", Moves::Fire_Blast },
		{ "waterfall", Moves::Waterfall },
		{ "clamp", Moves::Clamp },
		{ "swift", Moves::Swift },
		{ "skull bash", Moves::Skull_Bash },
		{ "spike cannon", Moves::Spike_Cannon },
		{ "constrict", Moves::Constrict },
		{ "amnesia", Moves::Amnesia },
		{ "kinesis", Moves::Kinesis },
		{ "softboiled", Moves::Softboiled },
		{ "hi jump kick", Moves::Hi_Jump_Kick },
		{ "glare", Moves::Glare },
		{ "dream eater", Moves::Dream_Eater },
		{ "poison gas", Moves::Poison_Gas },
		{ "barrage", Moves::Barrage },
		{ "leech life", Moves::Leech_Life },
		{ "lovely kiss", Moves::Lovely_Kiss },
		{ "sky attack", Moves::Sky_Attack },
		{ "transform", Moves::Transform },
		{ "bubble", Moves::Bubble },
		{ "dizzy punch", Moves::Dizzy_Punch },
		{ "spore", Moves::Spore },
		{ "flash", Moves::Flash },
		{ "psywave", Moves::Psywave },
		{ "splash", Moves::Splash },
		{ "acid armor", Moves::Acid_Armor },
		{ "crabhammer", Moves::Crabhammer },
		{ "explosion", Moves::Explosion },
		{ "fury swipes", Moves::Fury_Swipes },
		{ "bonemerang", Moves::Bonemerang },
		{ "rest", Moves::Rest },
		{ "rock slide", Moves::Rock_Slide },
		{ "hyper fang", Moves::Hyper_Fang },
		{ "sharpen", Moves::Sharpen },
		{ "conversion", Moves::Conversion },
		{ "tri attack", Moves::Tri_Attack },
		{ "super fang", Moves::Super_Fang },
		{ "slash", Moves::Slash },
		{ "substitute", Moves::Substitute },
		{ "struggle", Moves::Struggle },
		{ "sketch", Moves::Sketch },
		{ "triple kick", Moves::Triple_Kick },
		{ "thief", Moves::Thief },
		{ "spider web", Moves::Spider_Web },
		{ "mind reader", Moves::Mind_Reader },
		{ "nightmare", Moves::Nightmare },
		{ "flame wheel", Moves::Flame_Wheel },
		{ "snore", Moves::Snore },
		{ "curse", Moves::Curse },
		{ "flail", Moves::Flail },
		{ "conversion 2", Moves::Conversion_2 },
		{ "aeroblast", Moves::Aeroblast },
		{ "cotton spore", Moves::Cotton_Spore },
		{ "reversal", Moves::Reversal },
		{ "spite", Moves::Spite },
		{ "powder snow", Moves::Powder_Snow },
		{ "protect", Moves::Protect },
		{ "mach punch", Moves::Mach_Punch },
		{ "scary face", Moves::Scary_Face },
		{ "faint attack", Moves::Faint_Attack },
		{ "sweet kiss", Moves::Sweet_Kiss },
		{ "belly drum", Moves::Belly_Drum },
		{ "sludge bomb", Moves::Sludge_Bomb },
		{ "mud-slap", Moves::Mud_Slap },
		{ "octazooka", Moves::Octazooka },
		{ "spikes", Moves::Spikes },
		{ "zap cannon", Moves::Zap_Cannon },
		{ "foresight", Moves::Foresight },
		{ "destiny bond", Moves::Destiny_Bond },
		{ "perish song", Moves::Perish_Song },
		{ "icy wind", Moves::Icy_Wind },
		{ "detect", Moves::Detect },
		{ "bone rush", Moves::Bone_Rush },
		{ "lock-on", Moves::Lock_On },
		{ "outrage", Moves::Outrage },
		{ "sandstorm", Moves::Sandstorm },
		{ "giga drain", Moves::Giga_Drain },
		{ "endure", Moves::Endure },
		{ "charm", Moves::Charm },
		{ "rollout", Moves::Rollout },
		{ "false swipe", Moves::False_Swipe },
		{ "swagger", Moves::Swagger },
		{ "milk drink", Moves::Milk_Drink },
		{ "spark", Moves::Spark },
		{ "fury cutter", Moves::Fury_Cutter },
		{ "steel wing", Moves::Steel_Wing },
		{ "mean look", Moves::Mean_Look },
		{ "attract", Moves::Attract },
		{ "sleep talk", Moves::Sleep_Talk },
		{ "heal bell", Moves::Heal_Bell },
		{ "return", Moves::Return },
		{ "present", Moves::Present },
		{ "frustration", Moves::Frustration },
		{ "safeguard", Moves::Safeguard },
		{ "pain split", Moves::Pain_Split },
		{ "sacred fire", Moves::Sacred_Fire },
		{ "magnitude", Moves::Magnitude },
		{ "dynamicpunch", Moves::DynamicPunch },
		{ "megahorn", Moves::Megahorn },
		{ "dragonbreath", Moves::DragonBreath },
		{ "baton pass", Moves::Baton_Pass },
		{ "encore", Moves::Encore },
		{ "pursuit", Moves::Pursuit },
		{ "rapid spin", Moves::Rapid_Spin },
		{ "sweet scent", Moves::Sweet_Scent },
		{ "iron tail", Moves::Iron_Tail },
		{ "metal claw", Moves::Metal_Claw },
		{ "vital throw", Moves::Vital_Throw },
		{ "morning sun", Moves::Morning_Sun },
		{ "synthesis", Moves::Synthesis },
		{ "moonlight", Moves::Moonlight },
		{ "hidden power", Moves::Hidden_Power },
		{ "hidden power bug", Moves::Hidden_Power },
		{ "hidden power dark", Moves::Hidden_Power },
		{ "hidden power dragon", Moves::Hidden_Power },
		{ "hidden power electric", Moves::Hidden_Power },
		{ "hidden power fighting", Moves::Hidden_Power },
		{ "hidden power fire", Moves::Hidden_Power },
		{ "hidden power flying", Moves::Hidden_Power },
		{ "hidden power ghost", Moves::Hidden_Power },
		{ "hidden power grass", Moves::Hidden_Power },
		{ "hidden power ground", Moves::Hidden_Power },
		{ "hidden power ice", Moves::Hidden_Power },
		{ "hidden power poison", Moves::Hidden_Power },
		{ "hidden power psychic", Moves::Hidden_Power },
		{ "hidden power rock", Moves::Hidden_Power },
		{ "hidden power steel", Moves::Hidden_Power },
		{ "hidden power water", Moves::Hidden_Power },
		{ "cross chop", Moves::Cross_Chop },
		{ "twister", Moves::Twister },
		{ "rain dance", Moves::Rain_Dance },
		{ "sunny day", Moves::Sunny_Day },
		{ "crunch", Moves::Crunch },
		{ "mirror coat", Moves::Mirror_Coat },
		{ "psych up", Moves::Psych_Up },
		{ "extremespeed", Moves::ExtremeSpeed },
		{ "ancientpower", Moves::AncientPower },
		{ "shadow ball", Moves::Shadow_Ball },
		{ "future sight", Moves::Future_Sight },
		{ "rock smash", Moves::Rock_Smash },
		{ "whirlpool", Moves::Whirlpool },
		{ "beat up", Moves::Beat_Up },
		{ "fake out", Moves::Fake_Out },
		{ "uproar", Moves::Uproar },
		{ "stockpile", Moves::Stockpile },
		{ "spit up", Moves::Spit_Up },
		{ "swallow", Moves::Swallow },
		{ "heat wave", Moves::Heat_Wave },
		{ "hail", Moves::Hail },
		{ "torment", Moves::Torment },
		{ "flatter", Moves::Flatter },
		{ "will-o-wisp", Moves::Will_O_Wisp },
		{ "memento", Moves::Memento },
		{ "facade", Moves::Facade },
		{ "focus punch", Moves::Focus_Punch },
		{ "smellingsalt", Moves::SmellingSalt },
		{ "follow me", Moves::Follow_Me },
		{ "nature power", Moves::Nature_Power },
		{ "charge", Moves::Charge },
		{ "taunt", Moves::Taunt },
		{ "helping hand", Moves::Helping_Hand },
		{ "trick", Moves::Trick },
		{ "role play", Moves::Role_Play },
		{ "wish", Moves::Wish },
		{ "assist", Moves::Assist },
		{ "ingrain", Moves::Ingrain },
		{ "superpower", Moves::Superpower },
		{ "magic coat", Moves::Magic_Coat },
		{ "recycle", Moves::Recycle },
		{ "revenge", Moves::Revenge },
		{ "brick break", Moves::Brick_Break },
		{ "yawn", Moves::Yawn },
		{ "knock off", Moves::Knock_Off },
		{ "endeavor", Moves::Endeavor },
		{ "eruption", Moves::Eruption },
		{ "skill swap", Moves::Skill_Swap },
		{ "imprison", Moves::Imprison },
		{ "refresh", Moves::Refresh },
		{ "grudge", Moves::Grudge },
		{ "snatch", Moves::Snatch },
		{ "secret power", Moves::Secret_Power },
		{ "dive", Moves::Dive },
		{ "arm thrust", Moves::Arm_Thrust },
		{ "camouflage", Moves::Camouflage },
		{ "tail glow", Moves::Tail_Glow },
		{ "luster purge", Moves::Luster_Purge },
		{ "mist ball", Moves::Mist_Ball },
		{ "featherdance", Moves::FeatherDance },
		{ "teeter dance", Moves::Teeter_Dance },
		{ "blaze kick", Moves::Blaze_Kick },
		{ "mud sport", Moves::Mud_Sport },
		{ "ice ball", Moves::Ice_Ball },
		{ "needle arm", Moves::Needle_Arm },
		{ "slack off", Moves::Slack_Off },
		{ "hyper voice", Moves::Hyper_Voice },
		{ "poison fang", Moves::Poison_Fang },
		{ "crush claw", Moves::Crush_Claw },
		{ "blast burn", Moves::Blast_Burn },
		{ "hydro cannon", Moves::Hydro_Cannon },
		{ "meteor mash", Moves::Meteor_Mash },
		{ "astonish", Moves::Astonish },
		{ "weather ball", Moves::Weather_Ball },
		{ "aromatherapy", Moves::Aromatherapy },
		{ "fake tears", Moves::Fake_Tears },
		{ "air cutter", Moves::Air_Cutter },
		{ "overheat", Moves::Overheat },
		{ "odor sleuth", Moves::Odor_Sleuth },
		{ "rock tomb", Moves::Rock_Tomb },
		{ "silver wind", Moves::Silver_Wind },
		{ "metal sound", Moves::Metal_Sound },
		{ "grasswhistle", Moves::GrassWhistle },
		{ "tickle", Moves::Tickle },
		{ "cosmic power", Moves::Cosmic_Power },
		{ "water spout", Moves::Water_Spout },
		{ "signal beam", Moves::Signal_Beam },
		{ "shadow punch", Moves::Shadow_Punch },
		{ "extrasensory", Moves::Extrasensory },
		{ "sky uppercut", Moves::Sky_Uppercut },
		{ "sand tomb", Moves::Sand_Tomb },
		{ "sheer cold", Moves::Sheer_Cold },
		{ "muddy water", Moves::Muddy_Water },
		{ "bullet seed", Moves::Bullet_Seed },
		{ "aerial ace", Moves::Aerial_Ace },
		{ "icicle spear", Moves::Icicle_Spear },
		{ "iron defense", Moves::Iron_Defense },
		{ "block", Moves::Block },
		{ "howl", Moves::Howl },
		{ "dragon claw", Moves::Dragon_Claw },
		{ "frenzy plant", Moves::Frenzy_Plant },
		{ "bulk up", Moves::Bulk_Up },
		{ "bounce", Moves::Bounce },
		{ "mud shot", Moves::Mud_Shot },
		{ "poison tail", Moves::Poison_Tail },
		{ "covet", Moves::Covet },
		{ "volt tackle", Moves::Volt_Tackle },
		{ "magical leaf", Moves::Magical_Leaf },
		{ "water sport", Moves::Water_Sport },
		{ "calm mind", Moves::Calm_Mind },
		{ "leaf blade", Moves::Leaf_Blade },
		{ "dragon dance", Moves::Dragon_Dance },
		{ "rock blast", Moves::Rock_Blast },
		{ "shock wave", Moves::Shock_Wave },
		{ "water pulse", Moves::Water_Pulse },
		{ "doom desire", Moves::Doom_Desire },
		{ "psycho boost", Moves::Psycho_Boost },
		{ "roost", Moves::Roost },
		{ "gravity", Moves::Gravity },
		{ "miracle eye", Moves::Miracle_Eye },
		{ "wake-up slap", Moves::Wake_Up_Slap },
		{ "hammer arm", Moves::Hammer_Arm },
		{ "gyro ball", Moves::Gyro_Ball },
		{ "healing wish", Moves::Healing_Wish },
		{ "brine", Moves::Brine },
		{ "natural gift", Moves::Natural_Gift },
		{ "feint", Moves::Feint },
		{ "pluck", Moves::Pluck },
		{ "tailwind", Moves::Tailwind },
		{ "acupressure", Moves::Acupressure },
		{ "metal burst", Moves::Metal_Burst },
		{ "u-turn", Moves::U_turn },
		{ "close combat", Moves::Close_Combat },
		{ "payback", Moves::Payback },
		{ "assurance", Moves::Assurance },
		{ "embargo", Moves::Embargo },
		{ "fling", Moves::Fling },
		{ "psycho shift", Moves::Psycho_Shift },
		{ "trump card", Moves::Trump_Card },
		{ "heal block", Moves::Heal_Block },
		{ "wring out", Moves::Wring_Out },
		{ "power trick", Moves::Power_Trick },
		{ "gastro acid", Moves::Gastro_Acid },
		{ "lucky chant", Moves::Lucky_Chant },
		{ "me first", Moves::Me_First },
		{ "copycat", Moves::Copycat },
		{ "power swap", Moves::Power_Swap },
		{ "guard swap", Moves::Guard_Swap },
		{ "punishment", Moves::Punishment },
		{ "last resort", Moves::Last_Resort },
		{ "worry seed", Moves::Worry_Seed },
		{ "sucker punch", Moves::Sucker_Punch },
		{ "toxic spikes", Moves::Toxic_Spikes },
		{ "heart swap", Moves::Heart_Swap },
		{ "aqua ring", Moves::Aqua_Ring },
		{ "magnet rise", Moves::Magnet_Rise },
		{ "flare blitz", Moves::Flare_Blitz },
		{ "force palm", Moves::Force_Palm },
		{ "aura sphere", Moves::Aura_Sphere },
		{ "rock polish", Moves::Rock_Polish },
		{ "poison jab", Moves::Poison_Jab },
		{ "dark pulse", Moves::Dark_Pulse },
		{ "night slash", Moves::Night_Slash },
		{ "aqua tail", Moves::Aqua_Tail },
		{ "seed bomb", Moves::Seed_Bomb },
		{ "air slash", Moves::Air_Slash },
		{ "x-scissor", Moves::X_Scissor },
		{ "bug buzz", Moves::Bug_Buzz },
		{ "dragon pulse", Moves::Dragon_Pulse },
		{ "dragon rush", Moves::Dragon_Rush },
		{ "power gem", Moves::Power_Gem },
		{ "drain punch", Moves::Drain_Punch },
		{ "vacuum wave", Moves::Vacuum_Wave },
		{ "focus blast", Moves::Focus_Blast },
		{ "energy ball", Moves::Energy_Ball },
		{ "brave bird", Moves::Brave_Bird },
		{ "earth power", Moves::Earth_Power },
		{ "switcheroo", Moves::Switcheroo },
		{ "giga impact", Moves::Giga_Impact },
		{ "nasty plot", Moves::Nasty_Plot },
		{ "bullet punch", Moves::Bullet_Punch },
		{ "avalanche", Moves::Avalanche },
		{ "ice shard", Moves::Ice_Shard },
		{ "shadow claw", Moves::Shadow_Claw },
		{ "thunder fang", Moves::Thunder_Fang },
		{ "ice fang", Moves::Ice_Fang },
		{ "fire fang", Moves::Fire_Fang },
		{ "shadow sneak", Moves::Shadow_Sneak },
		{ "mud bomb", Moves::Mud_Bomb },
		{ "psycho cut", Moves::Psycho_Cut },
		{ "zen headbutt", Moves::Zen_Headbutt },
		{ "mirror shot", Moves::Mirror_Shot },
		{ "flash cannon", Moves::Flash_Cannon },
		{ "rock climb", Moves::Rock_Climb },
		{ "defog", Moves::Defog },
		{ "trick room", Moves::Trick_Room },
		{ "draco meteor", Moves::Draco_Meteor },
		{ "discharge", Moves::Discharge },
		{ "lava plume", Moves::Lava_Plume },
		{ "leaf storm", Moves::Leaf_Storm },
		{ "power whip", Moves::Power_Whip },
		{ "rock wrecker", Moves::Rock_Wrecker },
		{ "cross poison", Moves::Cross_Poison },
		{ "gunk shot", Moves::Gunk_Shot },
		{ "iron head", Moves::Iron_Head },
		{ "magnet bomb", Moves::Magnet_Bomb },
		{ "stone edge", Moves::Stone_Edge },
		{ "captivate", Moves::Captivate },
		{ "stealth rock", Moves::Stealth_Rock },
		{ "grass knot", Moves::Grass_Knot },
		{ "chatter", Moves::Chatter },
		{ "judgment", Moves::Judgment },
		{ "bug bite", Moves::Bug_Bite },
		{ "charge beam", Moves::Charge_Beam },
		{ "wood hammer", Moves::Wood_Hammer },
		{ "aqua jet", Moves::Aqua_Jet },
		{ "attack order", Moves::Attack_Order },
		{ "defend order", Moves::Defend_Order },
		{ "heal order", Moves::Heal_Order },
		{ "head smash", Moves::Head_Smash },
		{ "double hit", Moves::Double_Hit },
		{ "roar of time", Moves::Roar_of_Time },
		{ "spacial rend", Moves::Spacial_Rend },
		{ "lunar dance", Moves::Lunar_Dance },
		{ "crush grip", Moves::Crush_Grip },
		{ "magma storm", Moves::Magma_Storm },
		{ "dark void", Moves::Dark_Void },
		{ "seed flare", Moves::Seed_Flare },
		{ "ominous wind", Moves::Ominous_Wind },
		{ "shadow force", Moves::Shadow_Force },
		{ "hone claws", Moves::Hone_Claws },
		{ "wide guard", Moves::Wide_Guard },
		{ "guard split", Moves::Guard_Split },
		{ "power split", Moves::Power_Split },
		{ "wonder room", Moves::Wonder_Room },
		{ "psyshock", Moves::Psyshock },
		{ "venoshock", Moves::Venoshock },
		{ "autotomize", Moves::Autotomize },
		{ "rage powder", Moves::Rage_Powder },
		{ "telekinesis", Moves::Telekinesis },
		{ "magic room", Moves::Magic_Room },
		{ "smack down", Moves::Smack_Down },
		{ "storm throw", Moves::Storm_Throw },
		{ "flame burst", Moves::Flame_Burst },
		{ "sludge wave", Moves::Sludge_Wave },
		{ "quiver dance", Moves::Quiver_Dance },
		{ "heavy slam", Moves::Heavy_Slam },
		{ "synchronoise", Moves::Synchronoise },
		{ "electro ball", Moves::Electro_Ball },
		{ "soak", Moves::Soak },
		{ "flame charge", Moves::Flame_Charge },
		{ "coil", Moves::Coil },
		{ "low sweep", Moves::Low_Sweep },
		{ "acid spray", Moves::Acid_Spray },
		{ "foul play", Moves::Foul_Play },
		{ "simple beam", Moves::Simple_Beam },
		{ "entrainment", Moves::Entrainment },
		{ "after you", Moves::After_You },
		{ "round", Moves::Round },
		{ "echoed voice", Moves::Echoed_Voice },
		{ "chip away", Moves::Chip_Away },
		{ "clear smog", Moves::Clear_Smog },
		{ "stored power", Moves::Stored_Power },
		{ "quick guard", Moves::Quick_Guard },
		{ "ally switch", Moves::Ally_Switch },
		{ "scald", Moves::Scald },
		{ "shell smash", Moves::Shell_Smash },
		{ "heal pulse", Moves::Heal_Pulse },
		{ "hex", Moves::Hex },
		{ "sky drop", Moves::Sky_Drop },
		{ "shift gear", Moves::Shift_Gear },
		{ "circle throw", Moves::Circle_Throw },
		{ "incinerate", Moves::Incinerate },
		{ "quash", Moves::Quash },
		{ "acrobatics", Moves::Acrobatics },
		{ "reflect type", Moves::Reflect_Type },
		{ "retaliate", Moves::Retaliate },
		{ "final gambit", Moves::Final_Gambit },
		{ "bestow", Moves::Bestow },
		{ "inferno", Moves::Inferno },
		{ "water pledge", Moves::Water_Pledge },
		{ "fire pledge", Moves::Fire_Pledge },
		{ "grass pledge", Moves::Grass_Pledge },
		{ "volt switch", Moves::Volt_Switch },
		{ "struggle bug", Moves::Struggle_Bug },
		{ "bulldoze", Moves::Bulldoze },
		{ "frost breath", Moves::Frost_Breath },
		{ "dragon tail", Moves::Dragon_Tail },
		{ "work up", Moves::Work_Up },
		{ "electroweb", Moves::Electroweb },
		{ "wild charge", Moves::Wild_Charge },
		{ "drill run", Moves::Drill_Run },
		{ "dual chop", Moves::Dual_Chop },
		{ "heart stamp", Moves::Heart_Stamp },
		{ "horn leech", Moves::Horn_Leech },
		{ "sacred sword", Moves::Sacred_Sword },
		{ "razor shell", Moves::Razor_Shell },
		{ "heat crash", Moves::Heat_Crash },
		{ "leaf tornado", Moves::Leaf_Tornado },
		{ "steamroller", Moves::Steamroller },
		{ "cotton guard", Moves::Cotton_Guard },
		{ "night daze", Moves::Night_Daze },
		{ "psystrike", Moves::Psystrike },
		{ "tail slap", Moves::Tail_Slap },
		{ "hurricane", Moves::Hurricane },
		{ "head charge", Moves::Head_Charge },
		{ "gear grind", Moves::Gear_Grind },
		{ "searing shot", Moves::Searing_Shot },
		{ "techno blast", Moves::Techno_Blast },
		{ "relic song", Moves::Relic_Song },
		{ "secret sword", Moves::Secret_Sword },
		{ "glaciate", Moves::Glaciate },
		{ "bolt strike", Moves::Bolt_Strike },
		{ "blue flare", Moves::Blue_Flare },
		{ "fiery dance", Moves::Fiery_Dance },
		{ "freeze shock", Moves::Freeze_Shock },
		{ "ice burn", Moves::Ice_Burn },
		{ "snarl", Moves::Snarl },
		{ "icicle crash", Moves::Icicle_Crash },
		{ "v-create", Moves::V_create },
		{ "v-create", Moves::V_create },
		{ "fusion flare", Moves::Fusion_Flare },
		{ "fusion bolt", Moves::Fusion_Bolt },
		{ "end_move", Moves::END }
	};
	auto const it = converter.find(boost::algorithm::to_lower_copy(str));
	if (it != converter.end ())
		return it->second;
	else
		throw InvalidFromStringConversion ("Move", str);
}

}	// namespace technicalmachine
