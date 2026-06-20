// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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
using namespace containers::string_literals;

export constexpr auto to_string(MoveName const move) -> containers::string_view {
	switch (move) {
		case MoveName::Pound: return "Pound"_s;
		case MoveName::Karate_Chop: return "Karate Chop"_s;
		case MoveName::Double_Slap: return "Double Slap"_s;
		case MoveName::Comet_Punch: return "Comet Punch"_s;
		case MoveName::Mega_Punch: return "Mega Punch"_s;
		case MoveName::Pay_Day: return "Pay Day"_s;
		case MoveName::Fire_Punch: return "Fire Punch"_s;
		case MoveName::Ice_Punch: return "Ice Punch"_s;
		case MoveName::Thunder_Punch: return "Thunder Punch"_s;
		case MoveName::Scratch: return "Scratch"_s;
		case MoveName::Vise_Grip: return "Vise Grip"_s;
		case MoveName::Guillotine: return "Guillotine"_s;
		case MoveName::Razor_Wind: return "Razor Wind"_s;
		case MoveName::Swords_Dance: return "Swords Dance"_s;
		case MoveName::Cut: return "Cut"_s;
		case MoveName::Gust: return "Gust"_s;
		case MoveName::Wing_Attack: return "Wing Attack"_s;
		case MoveName::Whirlwind: return "Whirlwind"_s;
		case MoveName::Fly: return "Fly"_s;
		case MoveName::Bind: return "Bind"_s;
		case MoveName::Slam: return "Slam"_s;
		case MoveName::Vine_Whip: return "Vine Whip"_s;
		case MoveName::Stomp: return "Stomp"_s;
		case MoveName::Double_Kick: return "Double Kick"_s;
		case MoveName::Mega_Kick: return "Mega Kick"_s;
		case MoveName::Jump_Kick: return "Jump Kick"_s;
		case MoveName::Rolling_Kick: return "Rolling Kick"_s;
		case MoveName::Sand_Attack: return "Sand Attack"_s;
		case MoveName::Headbutt: return "Headbutt"_s;
		case MoveName::Horn_Attack: return "Horn Attack"_s;
		case MoveName::Fury_Attack: return "Fury Attack"_s;
		case MoveName::Horn_Drill: return "Horn Drill"_s;
		case MoveName::Tackle: return "Tackle"_s;
		case MoveName::Body_Slam: return "Body Slam"_s;
		case MoveName::Wrap: return "Wrap"_s;
		case MoveName::Take_Down: return "Take Down"_s;
		case MoveName::Thrash: return "Thrash"_s;
		case MoveName::Double_Edge: return "Double-Edge"_s;
		case MoveName::Tail_Whip: return "Tail Whip"_s;
		case MoveName::Poison_Sting: return "Poison Sting"_s;
		case MoveName::Twineedle: return "Twineedle"_s;
		case MoveName::Pin_Missile: return "Pin Missile"_s;
		case MoveName::Leer: return "Leer"_s;
		case MoveName::Bite: return "Bite"_s;
		case MoveName::Growl: return "Growl"_s;
		case MoveName::Roar: return "Roar"_s;
		case MoveName::Sing: return "Sing"_s;
		case MoveName::Supersonic: return "Supersonic"_s;
		case MoveName::Sonic_Boom: return "Sonic Boom"_s;
		case MoveName::Disable: return "Disable"_s;
		case MoveName::Acid: return "Acid"_s;
		case MoveName::Ember: return "Ember"_s;
		case MoveName::Flamethrower: return "Flamethrower"_s;
		case MoveName::Mist: return "Mist"_s;
		case MoveName::Water_Gun: return "Water Gun"_s;
		case MoveName::Hydro_Pump: return "Hydro Pump"_s;
		case MoveName::Surf: return "Surf"_s;
		case MoveName::Ice_Beam: return "Ice Beam"_s;
		case MoveName::Blizzard: return "Blizzard"_s;
		case MoveName::Psybeam: return "Psybeam"_s;
		case MoveName::Bubble_Beam: return "Bubble Beam"_s;
		case MoveName::Aurora_Beam: return "Aurora Beam"_s;
		case MoveName::Hyper_Beam: return "Hyper Beam"_s;
		case MoveName::Peck: return "Peck"_s;
		case MoveName::Drill_Peck: return "Drill Peck"_s;
		case MoveName::Submission: return "Submission"_s;
		case MoveName::Low_Kick: return "Low Kick"_s;
		case MoveName::Counter: return "Counter"_s;
		case MoveName::Seismic_Toss: return "Seismic Toss"_s;
		case MoveName::Strength: return "Strength"_s;
		case MoveName::Absorb: return "Absorb"_s;
		case MoveName::Mega_Drain: return "Mega Drain"_s;
		case MoveName::Leech_Seed: return "Leech Seed"_s;
		case MoveName::Growth: return "Growth"_s;
		case MoveName::Razor_Leaf: return "Razor Leaf"_s;
		case MoveName::Solar_Beam: return "Solar Beam"_s;
		case MoveName::Poison_Powder: return "Poison Powder"_s;
		case MoveName::Stun_Spore: return "Stun Spore"_s;
		case MoveName::Sleep_Powder: return "Sleep Powder"_s;
		case MoveName::Petal_Dance: return "Petal Dance"_s;
		case MoveName::String_Shot: return "String Shot"_s;
		case MoveName::Dragon_Rage: return "Dragon Rage"_s;
		case MoveName::Fire_Spin: return "Fire Spin"_s;
		case MoveName::Thunder_Shock: return "Thunder Shock"_s;
		case MoveName::Thunderbolt: return "Thunderbolt"_s;
		case MoveName::Thunder_Wave: return "Thunder Wave"_s;
		case MoveName::Thunder: return "Thunder"_s;
		case MoveName::Rock_Throw: return "Rock Throw"_s;
		case MoveName::Earthquake: return "Earthquake"_s;
		case MoveName::Fissure: return "Fissure"_s;
		case MoveName::Dig: return "Dig"_s;
		case MoveName::Toxic: return "Toxic"_s;
		case MoveName::Confusion: return "Confusion"_s;
		case MoveName::Psychic: return "Psychic"_s;
		case MoveName::Hypnosis: return "Hypnosis"_s;
		case MoveName::Meditate: return "Meditate"_s;
		case MoveName::Agility: return "Agility"_s;
		case MoveName::Quick_Attack: return "Quick Attack"_s;
		case MoveName::Rage: return "Rage"_s;
		case MoveName::Teleport: return "Teleport"_s;
		case MoveName::Night_Shade: return "Night Shade"_s;
		case MoveName::Mimic: return "Mimic"_s;
		case MoveName::Screech: return "Screech"_s;
		case MoveName::Double_Team: return "Double Team"_s;
		case MoveName::Recover: return "Recover"_s;
		case MoveName::Harden: return "Harden"_s;
		case MoveName::Minimize: return "Minimize"_s;
		case MoveName::Smokescreen: return "Smokescreen"_s;
		case MoveName::Confuse_Ray: return "Confuse Ray"_s;
		case MoveName::Withdraw: return "Withdraw"_s;
		case MoveName::Defense_Curl: return "Defense Curl"_s;
		case MoveName::Barrier: return "Barrier"_s;
		case MoveName::Light_Screen: return "Light Screen"_s;
		case MoveName::Haze: return "Haze"_s;
		case MoveName::Reflect: return "Reflect"_s;
		case MoveName::Focus_Energy: return "Focus Energy"_s;
		case MoveName::Bide: return "Bide"_s;
		case MoveName::Metronome: return "Metronome"_s;
		case MoveName::Mirror_Move: return "Mirror Move"_s;
		case MoveName::Self_Destruct: return "Self-Destruct"_s;
		case MoveName::Egg_Bomb: return "Egg Bomb"_s;
		case MoveName::Lick: return "Lick"_s;
		case MoveName::Smog: return "Smog"_s;
		case MoveName::Sludge: return "Sludge"_s;
		case MoveName::Bone_Club: return "Bone Club"_s;
		case MoveName::Fire_Blast: return "Fire Blast"_s;
		case MoveName::Waterfall: return "Waterfall"_s;
		case MoveName::Clamp: return "Clamp"_s;
		case MoveName::Swift: return "Swift"_s;
		case MoveName::Skull_Bash: return "Skull Bash"_s;
		case MoveName::Spike_Cannon: return "Spike Cannon"_s;
		case MoveName::Constrict: return "Constrict"_s;
		case MoveName::Amnesia: return "Amnesia"_s;
		case MoveName::Kinesis: return "Kinesis"_s;
		case MoveName::Soft_Boiled: return "Soft-Boiled"_s;
		case MoveName::High_Jump_Kick: return "High Jump Kick"_s;
		case MoveName::Glare: return "Glare"_s;
		case MoveName::Dream_Eater: return "Dream Eater"_s;
		case MoveName::Poison_Gas: return "Poison Gas"_s;
		case MoveName::Barrage: return "Barrage"_s;
		case MoveName::Leech_Life: return "Leech Life"_s;
		case MoveName::Lovely_Kiss: return "Lovely Kiss"_s;
		case MoveName::Sky_Attack: return "Sky Attack"_s;
		case MoveName::Transform: return "Transform"_s;
		case MoveName::Bubble: return "Bubble"_s;
		case MoveName::Dizzy_Punch: return "Dizzy Punch"_s;
		case MoveName::Spore: return "Spore"_s;
		case MoveName::Flash: return "Flash"_s;
		case MoveName::Psywave: return "Psywave"_s;
		case MoveName::Splash: return "Splash"_s;
		case MoveName::Acid_Armor: return "Acid Armor"_s;
		case MoveName::Crabhammer: return "Crabhammer"_s;
		case MoveName::Explosion: return "Explosion"_s;
		case MoveName::Fury_Swipes: return "Fury Swipes"_s;
		case MoveName::Bonemerang: return "Bonemerang"_s;
		case MoveName::Rest: return "Rest"_s;
		case MoveName::Rock_Slide: return "Rock Slide"_s;
		case MoveName::Hyper_Fang: return "Hyper Fang"_s;
		case MoveName::Sharpen: return "Sharpen"_s;
		case MoveName::Conversion: return "Conversion"_s;
		case MoveName::Tri_Attack: return "Tri Attack"_s;
		case MoveName::Super_Fang: return "Super Fang"_s;
		case MoveName::Slash: return "Slash"_s;
		case MoveName::Substitute: return "Substitute"_s;
		case MoveName::Struggle: return "Struggle"_s;
		case MoveName::Sketch: return "Sketch"_s;
		case MoveName::Triple_Kick: return "Triple Kick"_s;
		case MoveName::Thief: return "Thief"_s;
		case MoveName::Spider_Web: return "Spider Web"_s;
		case MoveName::Mind_Reader: return "Mind Reader"_s;
		case MoveName::Nightmare: return "Nightmare"_s;
		case MoveName::Flame_Wheel: return "Flame Wheel"_s;
		case MoveName::Snore: return "Snore"_s;
		case MoveName::Curse: return "Curse"_s;
		case MoveName::Flail: return "Flail"_s;
		case MoveName::Conversion_2: return "Conversion 2"_s;
		case MoveName::Aeroblast: return "Aeroblast"_s;
		case MoveName::Cotton_Spore: return "Cotton Spore"_s;
		case MoveName::Reversal: return "Reversal"_s;
		case MoveName::Spite: return "Spite"_s;
		case MoveName::Powder_Snow: return "Powder Snow"_s;
		case MoveName::Protect: return "Protect"_s;
		case MoveName::Mach_Punch: return "Mach Punch"_s;
		case MoveName::Scary_Face: return "Scary Face"_s;
		case MoveName::Feint_Attack: return "Feint Attack"_s;
		case MoveName::Sweet_Kiss: return "Sweet Kiss"_s;
		case MoveName::Belly_Drum: return "Belly Drum"_s;
		case MoveName::Sludge_Bomb: return "Sludge Bomb"_s;
		case MoveName::Mud_Slap: return "Mud-Slap"_s;
		case MoveName::Octazooka: return "Octazooka"_s;
		case MoveName::Spikes: return "Spikes"_s;
		case MoveName::Zap_Cannon: return "Zap Cannon"_s;
		case MoveName::Foresight: return "Foresight"_s;
		case MoveName::Destiny_Bond: return "Destiny Bond"_s;
		case MoveName::Perish_Song: return "Perish Song"_s;
		case MoveName::Icy_Wind: return "Icy Wind"_s;
		case MoveName::Detect: return "Detect"_s;
		case MoveName::Bone_Rush: return "Bone Rush"_s;
		case MoveName::Lock_On: return "Lock-On"_s;
		case MoveName::Outrage: return "Outrage"_s;
		case MoveName::Sandstorm: return "Sandstorm"_s;
		case MoveName::Giga_Drain: return "Giga Drain"_s;
		case MoveName::Endure: return "Endure"_s;
		case MoveName::Charm: return "Charm"_s;
		case MoveName::Rollout: return "Rollout"_s;
		case MoveName::False_Swipe: return "False Swipe"_s;
		case MoveName::Swagger: return "Swagger"_s;
		case MoveName::Milk_Drink: return "Milk Drink"_s;
		case MoveName::Spark: return "Spark"_s;
		case MoveName::Fury_Cutter: return "Fury Cutter"_s;
		case MoveName::Steel_Wing: return "Steel Wing"_s;
		case MoveName::Mean_Look: return "Mean Look"_s;
		case MoveName::Attract: return "Attract"_s;
		case MoveName::Sleep_Talk: return "Sleep Talk"_s;
		case MoveName::Heal_Bell: return "Heal Bell"_s;
		case MoveName::Return: return "Return"_s;
		case MoveName::Present: return "Present"_s;
		case MoveName::Frustration: return "Frustration"_s;
		case MoveName::Safeguard: return "Safeguard"_s;
		case MoveName::Pain_Split: return "Pain Split"_s;
		case MoveName::Sacred_Fire: return "Sacred Fire"_s;
		case MoveName::Magnitude: return "Magnitude"_s;
		case MoveName::Dynamic_Punch: return "Dynamic Punch"_s;
		case MoveName::Megahorn: return "Megahorn"_s;
		case MoveName::Dragon_Breath: return "Dragon Breath"_s;
		case MoveName::Baton_Pass: return "Baton Pass"_s;
		case MoveName::Encore: return "Encore"_s;
		case MoveName::Pursuit: return "Pursuit"_s;
		case MoveName::Rapid_Spin: return "Rapid Spin"_s;
		case MoveName::Sweet_Scent: return "Sweet Scent"_s;
		case MoveName::Iron_Tail: return "Iron Tail"_s;
		case MoveName::Metal_Claw: return "Metal Claw"_s;
		case MoveName::Vital_Throw: return "Vital Throw"_s;
		case MoveName::Morning_Sun: return "Morning Sun"_s;
		case MoveName::Synthesis: return "Synthesis"_s;
		case MoveName::Moonlight: return "Moonlight"_s;
		case MoveName::Hidden_Power: return "Hidden Power"_s;
		case MoveName::Cross_Chop: return "Cross Chop"_s;
		case MoveName::Twister: return "Twister"_s;
		case MoveName::Rain_Dance: return "Rain Dance"_s;
		case MoveName::Sunny_Day: return "Sunny Day"_s;
		case MoveName::Crunch: return "Crunch"_s;
		case MoveName::Mirror_Coat: return "Mirror Coat"_s;
		case MoveName::Psych_Up: return "Psych Up"_s;
		case MoveName::Extreme_Speed: return "Extreme Speed"_s;
		case MoveName::Ancient_Power: return "Ancient Power"_s;
		case MoveName::Shadow_Ball: return "Shadow Ball"_s;
		case MoveName::Future_Sight: return "Future Sight"_s;
		case MoveName::Rock_Smash: return "Rock Smash"_s;
		case MoveName::Whirlpool: return "Whirlpool"_s;
		case MoveName::Beat_Up: return "Beat Up"_s;
		case MoveName::Fake_Out: return "Fake Out"_s;
		case MoveName::Uproar: return "Uproar"_s;
		case MoveName::Stockpile: return "Stockpile"_s;
		case MoveName::Spit_Up: return "Spit Up"_s;
		case MoveName::Swallow: return "Swallow"_s;
		case MoveName::Heat_Wave: return "Heat Wave"_s;
		case MoveName::Hail: return "Hail"_s;
		case MoveName::Torment: return "Torment"_s;
		case MoveName::Flatter: return "Flatter"_s;
		case MoveName::Will_O_Wisp: return "Will-O-Wisp"_s;
		case MoveName::Memento: return "Memento"_s;
		case MoveName::Facade: return "Facade"_s;
		case MoveName::Focus_Punch: return "Focus Punch"_s;
		case MoveName::Smelling_Salts: return "Smelling Salts"_s;
		case MoveName::Follow_Me: return "Follow Me"_s;
		case MoveName::Nature_Power: return "Nature Power"_s;
		case MoveName::Charge: return "Charge"_s;
		case MoveName::Taunt: return "Taunt"_s;
		case MoveName::Helping_Hand: return "Helping Hand"_s;
		case MoveName::Trick: return "Trick"_s;
		case MoveName::Role_Play: return "Role Play"_s;
		case MoveName::Wish: return "Wish"_s;
		case MoveName::Assist: return "Assist"_s;
		case MoveName::Ingrain: return "Ingrain"_s;
		case MoveName::Superpower: return "Superpower"_s;
		case MoveName::Magic_Coat: return "Magic Coat"_s;
		case MoveName::Recycle: return "Recycle"_s;
		case MoveName::Revenge: return "Revenge"_s;
		case MoveName::Brick_Break: return "Brick Break"_s;
		case MoveName::Yawn: return "Yawn"_s;
		case MoveName::Knock_Off: return "Knock Off"_s;
		case MoveName::Endeavor: return "Endeavor"_s;
		case MoveName::Eruption: return "Eruption"_s;
		case MoveName::Skill_Swap: return "Skill Swap"_s;
		case MoveName::Imprison: return "Imprison"_s;
		case MoveName::Refresh: return "Refresh"_s;
		case MoveName::Grudge: return "Grudge"_s;
		case MoveName::Snatch: return "Snatch"_s;
		case MoveName::Secret_Power: return "Secret Power"_s;
		case MoveName::Dive: return "Dive"_s;
		case MoveName::Arm_Thrust: return "Arm Thrust"_s;
		case MoveName::Camouflage: return "Camouflage"_s;
		case MoveName::Tail_Glow: return "Tail Glow"_s;
		case MoveName::Luster_Purge: return "Luster Purge"_s;
		case MoveName::Mist_Ball: return "Mist Ball"_s;
		case MoveName::Feather_Dance: return "Feather Dance"_s;
		case MoveName::Teeter_Dance: return "Teeter Dance"_s;
		case MoveName::Blaze_Kick: return "Blaze Kick"_s;
		case MoveName::Mud_Sport: return "Mud Sport"_s;
		case MoveName::Ice_Ball: return "Ice Ball"_s;
		case MoveName::Needle_Arm: return "Needle Arm"_s;
		case MoveName::Slack_Off: return "Slack Off"_s;
		case MoveName::Hyper_Voice: return "Hyper Voice"_s;
		case MoveName::Poison_Fang: return "Poison Fang"_s;
		case MoveName::Crush_Claw: return "Crush Claw"_s;
		case MoveName::Blast_Burn: return "Blast Burn"_s;
		case MoveName::Hydro_Cannon: return "Hydro Cannon"_s;
		case MoveName::Meteor_Mash: return "Meteor Mash"_s;
		case MoveName::Astonish: return "Astonish"_s;
		case MoveName::Weather_Ball: return "Weather Ball"_s;
		case MoveName::Aromatherapy: return "Aromatherapy"_s;
		case MoveName::Fake_Tears: return "Fake Tears"_s;
		case MoveName::Air_Cutter: return "Air Cutter"_s;
		case MoveName::Overheat: return "Overheat"_s;
		case MoveName::Odor_Sleuth: return "Odor Sleuth"_s;
		case MoveName::Rock_Tomb: return "Rock Tomb"_s;
		case MoveName::Silver_Wind: return "Silver Wind"_s;
		case MoveName::Metal_Sound: return "Metal Sound"_s;
		case MoveName::Grass_Whistle: return "Grass Whistle"_s;
		case MoveName::Tickle: return "Tickle"_s;
		case MoveName::Cosmic_Power: return "Cosmic Power"_s;
		case MoveName::Water_Spout: return "Water Spout"_s;
		case MoveName::Signal_Beam: return "Signal Beam"_s;
		case MoveName::Shadow_Punch: return "Shadow Punch"_s;
		case MoveName::Extrasensory: return "Extrasensory"_s;
		case MoveName::Sky_Uppercut: return "Sky Uppercut"_s;
		case MoveName::Sand_Tomb: return "Sand Tomb"_s;
		case MoveName::Sheer_Cold: return "Sheer Cold"_s;
		case MoveName::Muddy_Water: return "Muddy Water"_s;
		case MoveName::Bullet_Seed: return "Bullet Seed"_s;
		case MoveName::Aerial_Ace: return "Aerial Ace"_s;
		case MoveName::Icicle_Spear: return "Icicle Spear"_s;
		case MoveName::Iron_Defense: return "Iron Defense"_s;
		case MoveName::Block: return "Block"_s;
		case MoveName::Howl: return "Howl"_s;
		case MoveName::Dragon_Claw: return "Dragon Claw"_s;
		case MoveName::Frenzy_Plant: return "Frenzy Plant"_s;
		case MoveName::Bulk_Up: return "Bulk Up"_s;
		case MoveName::Bounce: return "Bounce"_s;
		case MoveName::Mud_Shot: return "Mud Shot"_s;
		case MoveName::Poison_Tail: return "Poison Tail"_s;
		case MoveName::Covet: return "Covet"_s;
		case MoveName::Volt_Tackle: return "Volt Tackle"_s;
		case MoveName::Magical_Leaf: return "Magical Leaf"_s;
		case MoveName::Water_Sport: return "Water Sport"_s;
		case MoveName::Calm_Mind: return "Calm Mind"_s;
		case MoveName::Leaf_Blade: return "Leaf Blade"_s;
		case MoveName::Dragon_Dance: return "Dragon Dance"_s;
		case MoveName::Rock_Blast: return "Rock Blast"_s;
		case MoveName::Shock_Wave: return "Shock Wave"_s;
		case MoveName::Water_Pulse: return "Water Pulse"_s;
		case MoveName::Doom_Desire: return "Doom Desire"_s;
		case MoveName::Psycho_Boost: return "Psycho Boost"_s;
		case MoveName::Roost: return "Roost"_s;
		case MoveName::Gravity: return "Gravity"_s;
		case MoveName::Miracle_Eye: return "Miracle Eye"_s;
		case MoveName::Wake_Up_Slap: return "Wake-Up Slap"_s;
		case MoveName::Hammer_Arm: return "Hammer Arm"_s;
		case MoveName::Gyro_Ball: return "Gyro Ball"_s;
		case MoveName::Healing_Wish: return "Healing Wish"_s;
		case MoveName::Brine: return "Brine"_s;
		case MoveName::Natural_Gift: return "Natural Gift"_s;
		case MoveName::Feint: return "Feint"_s;
		case MoveName::Pluck: return "Pluck"_s;
		case MoveName::Tailwind: return "Tailwind"_s;
		case MoveName::Acupressure: return "Acupressure"_s;
		case MoveName::Metal_Burst: return "Metal Burst"_s;
		case MoveName::U_turn: return "U-turn"_s;
		case MoveName::Close_Combat: return "Close Combat"_s;
		case MoveName::Payback: return "Payback"_s;
		case MoveName::Assurance: return "Assurance"_s;
		case MoveName::Embargo: return "Embargo"_s;
		case MoveName::Fling: return "Fling"_s;
		case MoveName::Psycho_Shift: return "Psycho Shift"_s;
		case MoveName::Trump_Card: return "Trump Card"_s;
		case MoveName::Heal_Block: return "Heal Block"_s;
		case MoveName::Wring_Out: return "Wring Out"_s;
		case MoveName::Power_Trick: return "Power Trick"_s;
		case MoveName::Gastro_Acid: return "Gastro Acid"_s;
		case MoveName::Lucky_Chant: return "Lucky Chant"_s;
		case MoveName::Me_First: return "Me First"_s;
		case MoveName::Copycat: return "Copycat"_s;
		case MoveName::Power_Swap: return "Power Swap"_s;
		case MoveName::Guard_Swap: return "Guard Swap"_s;
		case MoveName::Punishment: return "Punishment"_s;
		case MoveName::Last_Resort: return "Last Resort"_s;
		case MoveName::Worry_Seed: return "Worry Seed"_s;
		case MoveName::Sucker_Punch: return "Sucker Punch"_s;
		case MoveName::Toxic_Spikes: return "Toxic Spikes"_s;
		case MoveName::Heart_Swap: return "Heart Swap"_s;
		case MoveName::Aqua_Ring: return "Aqua Ring"_s;
		case MoveName::Magnet_Rise: return "Magnet Rise"_s;
		case MoveName::Flare_Blitz: return "Flare Blitz"_s;
		case MoveName::Force_Palm: return "Force Palm"_s;
		case MoveName::Aura_Sphere: return "Aura Sphere"_s;
		case MoveName::Rock_Polish: return "Rock Polish"_s;
		case MoveName::Poison_Jab: return "Poison Jab"_s;
		case MoveName::Dark_Pulse: return "Dark Pulse"_s;
		case MoveName::Night_Slash: return "Night Slash"_s;
		case MoveName::Aqua_Tail: return "Aqua Tail"_s;
		case MoveName::Seed_Bomb: return "Seed Bomb"_s;
		case MoveName::Air_Slash: return "Air Slash"_s;
		case MoveName::X_Scissor: return "X-Scissor"_s;
		case MoveName::Bug_Buzz: return "Bug Buzz"_s;
		case MoveName::Dragon_Pulse: return "Dragon Pulse"_s;
		case MoveName::Dragon_Rush: return "Dragon Rush"_s;
		case MoveName::Power_Gem: return "Power Gem"_s;
		case MoveName::Drain_Punch: return "Drain Punch"_s;
		case MoveName::Vacuum_Wave: return "Vacuum Wave"_s;
		case MoveName::Focus_Blast: return "Focus Blast"_s;
		case MoveName::Energy_Ball: return "Energy Ball"_s;
		case MoveName::Brave_Bird: return "Brave Bird"_s;
		case MoveName::Earth_Power: return "Earth Power"_s;
		case MoveName::Switcheroo: return "Switcheroo"_s;
		case MoveName::Giga_Impact: return "Giga Impact"_s;
		case MoveName::Nasty_Plot: return "Nasty Plot"_s;
		case MoveName::Bullet_Punch: return "Bullet Punch"_s;
		case MoveName::Avalanche: return "Avalanche"_s;
		case MoveName::Ice_Shard: return "Ice Shard"_s;
		case MoveName::Shadow_Claw: return "Shadow Claw"_s;
		case MoveName::Thunder_Fang: return "Thunder Fang"_s;
		case MoveName::Ice_Fang: return "Ice Fang"_s;
		case MoveName::Fire_Fang: return "Fire Fang"_s;
		case MoveName::Shadow_Sneak: return "Shadow Sneak"_s;
		case MoveName::Mud_Bomb: return "Mud Bomb"_s;
		case MoveName::Psycho_Cut: return "Psycho Cut"_s;
		case MoveName::Zen_Headbutt: return "Zen Headbutt"_s;
		case MoveName::Mirror_Shot: return "Mirror Shot"_s;
		case MoveName::Flash_Cannon: return "Flash Cannon"_s;
		case MoveName::Rock_Climb: return "Rock Climb"_s;
		case MoveName::Defog: return "Defog"_s;
		case MoveName::Trick_Room: return "Trick Room"_s;
		case MoveName::Draco_Meteor: return "Draco Meteor"_s;
		case MoveName::Discharge: return "Discharge"_s;
		case MoveName::Lava_Plume: return "Lava Plume"_s;
		case MoveName::Leaf_Storm: return "Leaf Storm"_s;
		case MoveName::Power_Whip: return "Power Whip"_s;
		case MoveName::Rock_Wrecker: return "Rock Wrecker"_s;
		case MoveName::Cross_Poison: return "Cross Poison"_s;
		case MoveName::Gunk_Shot: return "Gunk Shot"_s;
		case MoveName::Iron_Head: return "Iron Head"_s;
		case MoveName::Magnet_Bomb: return "Magnet Bomb"_s;
		case MoveName::Stone_Edge: return "Stone Edge"_s;
		case MoveName::Captivate: return "Captivate"_s;
		case MoveName::Stealth_Rock: return "Stealth Rock"_s;
		case MoveName::Grass_Knot: return "Grass Knot"_s;
		case MoveName::Chatter: return "Chatter"_s;
		case MoveName::Judgment: return "Judgment"_s;
		case MoveName::Bug_Bite: return "Bug Bite"_s;
		case MoveName::Charge_Beam: return "Charge Beam"_s;
		case MoveName::Wood_Hammer: return "Wood Hammer"_s;
		case MoveName::Aqua_Jet: return "Aqua Jet"_s;
		case MoveName::Attack_Order: return "Attack Order"_s;
		case MoveName::Defend_Order: return "Defend Order"_s;
		case MoveName::Heal_Order: return "Heal Order"_s;
		case MoveName::Head_Smash: return "Head Smash"_s;
		case MoveName::Double_Hit: return "Double Hit"_s;
		case MoveName::Roar_of_Time: return "Roar of Time"_s;
		case MoveName::Spacial_Rend: return "Spacial Rend"_s;
		case MoveName::Lunar_Dance: return "Lunar Dance"_s;
		case MoveName::Crush_Grip: return "Crush Grip"_s;
		case MoveName::Magma_Storm: return "Magma Storm"_s;
		case MoveName::Dark_Void: return "Dark Void"_s;
		case MoveName::Seed_Flare: return "Seed Flare"_s;
		case MoveName::Ominous_Wind: return "Ominous Wind"_s;
		case MoveName::Shadow_Force: return "Shadow Force"_s;
		case MoveName::Hone_Claws: return "Hone Claws"_s;
		case MoveName::Wide_Guard: return "Wide Guard"_s;
		case MoveName::Guard_Split: return "Guard Split"_s;
		case MoveName::Power_Split: return "Power Split"_s;
		case MoveName::Wonder_Room: return "Wonder Room"_s;
		case MoveName::Psyshock: return "Psyshock"_s;
		case MoveName::Venoshock: return "Venoshock"_s;
		case MoveName::Autotomize: return "Autotomize"_s;
		case MoveName::Rage_Powder: return "Rage Powder"_s;
		case MoveName::Telekinesis: return "Telekinesis"_s;
		case MoveName::Magic_Room: return "Magic Room"_s;
		case MoveName::Smack_Down: return "Smack Down"_s;
		case MoveName::Storm_Throw: return "Storm Throw"_s;
		case MoveName::Flame_Burst: return "Flame Burst"_s;
		case MoveName::Sludge_Wave: return "Sludge Wave"_s;
		case MoveName::Quiver_Dance: return "Quiver Dance"_s;
		case MoveName::Heavy_Slam: return "Heavy Slam"_s;
		case MoveName::Synchronoise: return "Synchronoise"_s;
		case MoveName::Electro_Ball: return "Electro Ball"_s;
		case MoveName::Soak: return "Soak"_s;
		case MoveName::Flame_Charge: return "Flame Charge"_s;
		case MoveName::Coil: return "Coil"_s;
		case MoveName::Low_Sweep: return "Low Sweep"_s;
		case MoveName::Acid_Spray: return "Acid Spray"_s;
		case MoveName::Foul_Play: return "Foul Play"_s;
		case MoveName::Simple_Beam: return "Simple Beam"_s;
		case MoveName::Entrainment: return "Entrainment"_s;
		case MoveName::After_You: return "After You"_s;
		case MoveName::Round: return "Round"_s;
		case MoveName::Echoed_Voice: return "Echoed Voice"_s;
		case MoveName::Chip_Away: return "Chip Away"_s;
		case MoveName::Clear_Smog: return "Clear Smog"_s;
		case MoveName::Stored_Power: return "Stored Power"_s;
		case MoveName::Quick_Guard: return "Quick Guard"_s;
		case MoveName::Ally_Switch: return "Ally Switch"_s;
		case MoveName::Scald: return "Scald"_s;
		case MoveName::Shell_Smash: return "Shell Smash"_s;
		case MoveName::Heal_Pulse: return "Heal Pulse"_s;
		case MoveName::Hex: return "Hex"_s;
		case MoveName::Sky_Drop: return "Sky Drop"_s;
		case MoveName::Shift_Gear: return "Shift Gear"_s;
		case MoveName::Circle_Throw: return "Circle Throw"_s;
		case MoveName::Incinerate: return "Incinerate"_s;
		case MoveName::Quash: return "Quash"_s;
		case MoveName::Acrobatics: return "Acrobatics"_s;
		case MoveName::Reflect_Type: return "Reflect Type"_s;
		case MoveName::Retaliate: return "Retaliate"_s;
		case MoveName::Final_Gambit: return "Final Gambit"_s;
		case MoveName::Bestow: return "Bestow"_s;
		case MoveName::Inferno: return "Inferno"_s;
		case MoveName::Water_Pledge: return "Water Pledge"_s;
		case MoveName::Fire_Pledge: return "Fire Pledge"_s;
		case MoveName::Grass_Pledge: return "Grass Pledge"_s;
		case MoveName::Volt_Switch: return "Volt Switch"_s;
		case MoveName::Struggle_Bug: return "Struggle Bug"_s;
		case MoveName::Bulldoze: return "Bulldoze"_s;
		case MoveName::Frost_Breath: return "Frost Breath"_s;
		case MoveName::Dragon_Tail: return "Dragon Tail"_s;
		case MoveName::Work_Up: return "Work Up"_s;
		case MoveName::Electroweb: return "Electroweb"_s;
		case MoveName::Wild_Charge: return "Wild Charge"_s;
		case MoveName::Drill_Run: return "Drill Run"_s;
		case MoveName::Dual_Chop: return "Dual Chop"_s;
		case MoveName::Heart_Stamp: return "Heart Stamp"_s;
		case MoveName::Horn_Leech: return "Horn Leech"_s;
		case MoveName::Sacred_Sword: return "Sacred Sword"_s;
		case MoveName::Razor_Shell: return "Razor Shell"_s;
		case MoveName::Heat_Crash: return "Heat Crash"_s;
		case MoveName::Leaf_Tornado: return "Leaf Tornado"_s;
		case MoveName::Steamroller: return "Steamroller"_s;
		case MoveName::Cotton_Guard: return "Cotton Guard"_s;
		case MoveName::Night_Daze: return "Night Daze"_s;
		case MoveName::Psystrike: return "Psystrike"_s;
		case MoveName::Tail_Slap: return "Tail Slap"_s;
		case MoveName::Hurricane: return "Hurricane"_s;
		case MoveName::Head_Charge: return "Head Charge"_s;
		case MoveName::Gear_Grind: return "Gear Grind"_s;
		case MoveName::Searing_Shot: return "Searing Shot"_s;
		case MoveName::Techno_Blast: return "Techno Blast"_s;
		case MoveName::Relic_Song: return "Relic Song"_s;
		case MoveName::Secret_Sword: return "Secret Sword"_s;
		case MoveName::Glaciate: return "Glaciate"_s;
		case MoveName::Bolt_Strike: return "Bolt Strike"_s;
		case MoveName::Blue_Flare: return "Blue Flare"_s;
		case MoveName::Fiery_Dance: return "Fiery Dance"_s;
		case MoveName::Freeze_Shock: return "Freeze Shock"_s;
		case MoveName::Ice_Burn: return "Ice Burn"_s;
		case MoveName::Snarl: return "Snarl"_s;
		case MoveName::Icicle_Crash: return "Icicle Crash"_s;
		case MoveName::V_create: return "V-create"_s;
		case MoveName::Fusion_Flare: return "Fusion Flare"_s;
		case MoveName::Fusion_Bolt: return "Fusion Bolt"_s;
		case MoveName::Flying_Press: return "Flying Press"_s;
		case MoveName::Mat_Block: return "Mat Block"_s;
		case MoveName::Belch: return "Belch"_s;
		case MoveName::Rototiller: return "Rototiller"_s;
		case MoveName::Sticky_Web: return "Sticky Web"_s;
		case MoveName::Fell_Stinger: return "Fell Stinger"_s;
		case MoveName::Phantom_Force: return "Phantom Force"_s;
		case MoveName::Trick_or_Treat: return "Trick-or-Treat"_s;
		case MoveName::Noble_Roar: return "Noble Roar"_s;
		case MoveName::Ion_Deluge: return "Ion Deluge"_s;
		case MoveName::Parabolic_Charge: return "Parabolic Charge"_s;
		case MoveName::Forests_Curse: return "Forest's Curse"_s;
		case MoveName::Petal_Blizzard: return "Petal Blizzard"_s;
		case MoveName::Freeze_Dry: return "Freeze-Dry"_s;
		case MoveName::Disarming_Voice: return "Disarming Voice"_s;
		case MoveName::Parting_Shot: return "Parting Shot"_s;
		case MoveName::Topsy_Turvy: return "Topsy-Turvy"_s;
		case MoveName::Draining_Kiss: return "Draining Kiss"_s;
		case MoveName::Crafty_Shield: return "Crafty Shield"_s;
		case MoveName::Flower_Shield: return "Flower Shield"_s;
		case MoveName::Grassy_Terrain: return "Grassy Terrain"_s;
		case MoveName::Misty_Terrain: return "Misty Terrain"_s;
		case MoveName::Electrify: return "Electrify"_s;
		case MoveName::Play_Rough: return "Play Rough"_s;
		case MoveName::Fairy_Wind: return "Fairy Wind"_s;
		case MoveName::Moonblast: return "Moonblast"_s;
		case MoveName::Boomburst: return "Boomburst"_s;
		case MoveName::Fairy_Lock: return "Fairy Lock"_s;
		case MoveName::Kings_Shield: return "King's Shield"_s;
		case MoveName::Play_Nice: return "Play Nice"_s;
		case MoveName::Confide: return "Confide"_s;
		case MoveName::Diamond_Storm: return "Diamond Storm"_s;
		case MoveName::Steam_Eruption: return "Steam Eruption"_s;
		case MoveName::Hyperspace_Hole: return "Hyperspace Hole"_s;
		case MoveName::Water_Shuriken: return "Water Shuriken"_s;
		case MoveName::Mystical_Fire: return "Mystical Fire"_s;
		case MoveName::Spiky_Shield: return "Spiky Shield"_s;
		case MoveName::Aromatic_Mist: return "Aromatic Mist"_s;
		case MoveName::Eerie_Impulse: return "Eerie Impulse"_s;
		case MoveName::Venom_Drench: return "Venom Drench"_s;
		case MoveName::Powder: return "Powder"_s;
		case MoveName::Geomancy: return "Geomancy"_s;
		case MoveName::Magnetic_Flux: return "Magnetic Flux"_s;
		case MoveName::Happy_Hour: return "Happy Hour"_s;
		case MoveName::Electric_Terrain: return "Electric Terrain"_s;
		case MoveName::Dazzling_Gleam: return "Dazzling Gleam"_s;
		case MoveName::Celebrate: return "Celebrate"_s;
		case MoveName::Hold_Hands: return "Hold Hands"_s;
		case MoveName::Baby_Doll_Eyes: return "Baby-Doll Eyes"_s;
		case MoveName::Nuzzle: return "Nuzzle"_s;
		case MoveName::Hold_Back: return "Hold Back"_s;
		case MoveName::Infestation: return "Infestation"_s;
		case MoveName::Power_Up_Punch: return "Power-Up Punch"_s;
		case MoveName::Oblivion_Wing: return "Oblivion Wing"_s;
		case MoveName::Thousand_Arrows: return "Thousand Arrows"_s;
		case MoveName::Thousand_Waves: return "Thousand Waves"_s;
		case MoveName::Lands_Wrath: return "Land's Wrath"_s;
		case MoveName::Light_of_Ruin: return "Light of Ruin"_s;
		case MoveName::Origin_Pulse: return "Origin Pulse"_s;
		case MoveName::Precipice_Blades: return "Precipice Blades"_s;
		case MoveName::Dragon_Ascent: return "Dragon Ascent"_s;
		case MoveName::Hyperspace_Fury: return "Hyperspace Fury"_s;
		case MoveName::Breakneck_Blitz: return "Breakneck Blitz"_s;
		case MoveName::All_Out_Pummeling: return "All-Out Pummeling"_s;
		case MoveName::Supersonic_Skystrike: return "Supersonic Skystrike"_s;
		case MoveName::Acid_Downpour: return "Acid Downpour"_s;
		case MoveName::Tectonic_Rage: return "Tectonic Rage"_s;
		case MoveName::Continental_Crush: return "Continental Crush"_s;
		case MoveName::Savage_Spin_Out: return "Savage Spin-Out"_s;
		case MoveName::Never_Ending_Nightmare: return "Never-Ending Nightmare"_s;
		case MoveName::Corkscrew_Crash: return "Corkscrew Crash"_s;
		case MoveName::Inferno_Overdrive: return "Inferno Overdrive"_s;
		case MoveName::Hydro_Vortex: return "Hydro Vortex"_s;
		case MoveName::Bloom_Doom: return "Bloom Doom"_s;
		case MoveName::Gigavolt_Havoc: return "Gigavolt Havoc"_s;
		case MoveName::Shattered_Psyche: return "Shattered Psyche"_s;
		case MoveName::Subzero_Slammer: return "Subzero Slammer"_s;
		case MoveName::Devastating_Drake: return "Devastating Drake"_s;
		case MoveName::Black_Hole_Eclipse: return "Black Hole Eclipse"_s;
		case MoveName::Twinkle_Tackle: return "Twinkle Tackle"_s;
		case MoveName::Catastropika: return "Catastropika"_s;
		case MoveName::Shore_Up: return "Shore Up"_s;
		case MoveName::First_Impression: return "First Impression"_s;
		case MoveName::Baneful_Bunker: return "Baneful Bunker"_s;
		case MoveName::Spirit_Shackle: return "Spirit Shackle"_s;
		case MoveName::Darkest_Lariat: return "Darkest Lariat"_s;
		case MoveName::Sparkling_Aria: return "Sparkling Aria"_s;
		case MoveName::Ice_Hammer: return "Ice Hammer"_s;
		case MoveName::Floral_Healing: return "Floral Healing"_s;
		case MoveName::High_Horsepower: return "High Horsepower"_s;
		case MoveName::Strength_Sap: return "Strength Sap"_s;
		case MoveName::Solar_Blade: return "Solar Blade"_s;
		case MoveName::Leafage: return "Leafage"_s;
		case MoveName::Spotlight: return "Spotlight"_s;
		case MoveName::Toxic_Thread: return "Toxic Thread"_s;
		case MoveName::Laser_Focus: return "Laser Focus"_s;
		case MoveName::Gear_Up: return "Gear Up"_s;
		case MoveName::Throat_Chop: return "Throat Chop"_s;
		case MoveName::Pollen_Puff: return "Pollen Puff"_s;
		case MoveName::Anchor_Shot: return "Anchor Shot"_s;
		case MoveName::Psychic_Terrain: return "Psychic Terrain"_s;
		case MoveName::Lunge: return "Lunge"_s;
		case MoveName::Fire_Lash: return "Fire Lash"_s;
		case MoveName::Power_Trip: return "Power Trip"_s;
		case MoveName::Burn_Up: return "Burn Up"_s;
		case MoveName::Speed_Swap: return "Speed Swap"_s;
		case MoveName::Smart_Strike: return "Smart Strike"_s;
		case MoveName::Purify: return "Purify"_s;
		case MoveName::Revelation_Dance: return "Revelation Dance"_s;
		case MoveName::Core_Enforcer: return "Core Enforcer"_s;
		case MoveName::Trop_Kick: return "Trop Kick"_s;
		case MoveName::Instruct: return "Instruct"_s;
		case MoveName::Beak_Blast: return "Beak Blast"_s;
		case MoveName::Clanging_Scales: return "Clanging Scales"_s;
		case MoveName::Dragon_Hammer: return "Dragon Hammer"_s;
		case MoveName::Brutal_Swing: return "Brutal Swing"_s;
		case MoveName::Aurora_Veil: return "Aurora Veil"_s;
		case MoveName::Sinister_Arrow_Raid: return "Sinister Arrow Raid"_s;
		case MoveName::Malicious_Moonsault: return "Malicious Moonsault"_s;
		case MoveName::Oceanic_Operetta: return "Oceanic Operetta"_s;
		case MoveName::Guardian_of_Alola: return "Guardian of Alola"_s;
		case MoveName::Soul_Stealing_7_Star_Strike: return "Soul-Stealing 7-Star Strike"_s;
		case MoveName::Stoked_Sparksurfer: return "Stoked Sparksurfer"_s;
		case MoveName::Pulverizing_Pancake: return "Pulverizing Pancake"_s;
		case MoveName::Extreme_Evoboost: return "Extreme Evoboost"_s;
		case MoveName::Genesis_Supernova: return "Genesis Supernova"_s;
		case MoveName::Shell_Trap: return "Shell Trap"_s;
		case MoveName::Fleur_Cannon: return "Fleur Cannon"_s;
		case MoveName::Psychic_Fangs: return "Psychic Fangs"_s;
		case MoveName::Stomping_Tantrum: return "Stomping Tantrum"_s;
		case MoveName::Shadow_Bone: return "Shadow Bone"_s;
		case MoveName::Accelerock: return "Accelerock"_s;
		case MoveName::Liquidation: return "Liquidation"_s;
		case MoveName::Prismatic_Laser: return "Prismatic Laser"_s;
		case MoveName::Spectral_Thief: return "Spectral Thief"_s;
		case MoveName::Sunsteel_Strike: return "Sunsteel Strike"_s;
		case MoveName::Moongeist_Beam: return "Moongeist Beam"_s;
		case MoveName::Tearful_Look: return "Tearful Look"_s;
		case MoveName::Zing_Zap: return "Zing Zap"_s;
		case MoveName::Natures_Madness: return "Nature's Madness"_s;
		case MoveName::Multi_Attack: return "Multi-Attack"_s;
		case MoveName::m10000000_Volt_Thunderbolt: return "10,000,000 Volt Thunderbolt"_s;
		case MoveName::Mind_Blown: return "Mind Blown"_s;
		case MoveName::Plasma_Fists: return "Plasma Fists"_s;
		case MoveName::Photon_Geyser: return "Photon Geyser"_s;
		case MoveName::Light_That_Burns_the_Sky: return "Light That Burns the Sky"_s;
		case MoveName::Searing_Sunraze_Smash: return "Searing Sunraze Smash"_s;
		case MoveName::Menacing_Moonraze_Maelstrom: return "Menacing Moonraze Maelstrom"_s;
		case MoveName::Lets_Snuggle_Forever: return "Let's Snuggle Forever"_s;
		case MoveName::Splintered_Stormshards: return "Splintered Stormshards"_s;
		case MoveName::Clangorous_Soulblaze: return "Clangorous Soulblaze"_s;
		case MoveName::Double_Iron_Bash: return "Double Iron Bash"_s;
		case MoveName::Max_Guard: return "Max Guard"_s;
		case MoveName::Dynamax_Cannon: return "Dynamax Cannon"_s;
		case MoveName::Snipe_Shot: return "Snipe Shot"_s;
		case MoveName::Jaw_Lock: return "Jaw Lock"_s;
		case MoveName::Stuff_Cheeks: return "Stuff Cheeks"_s;
		case MoveName::No_Retreat: return "No Retreat"_s;
		case MoveName::Tar_Shot: return "Tar Shot"_s;
		case MoveName::Magic_Powder: return "Magic Powder"_s;
		case MoveName::Dragon_Darts: return "Dragon Darts"_s;
		case MoveName::Teatime: return "Teatime"_s;
		case MoveName::Octolock: return "Octolock"_s;
		case MoveName::Bolt_Beak: return "Bolt Beak"_s;
		case MoveName::Fishious_Rend: return "Fishious Rend"_s;
		case MoveName::Court_Change: return "Court Change"_s;
		case MoveName::Max_Flare: return "Max Flare"_s;
		case MoveName::Max_Flutterby: return "Max Flutterby"_s;
		case MoveName::Max_Lightning: return "Max Lightning"_s;
		case MoveName::Max_Strike: return "Max Strike"_s;
		case MoveName::Max_Knuckle: return "Max Knuckle"_s;
		case MoveName::Max_Phantasm: return "Max Phantasm"_s;
		case MoveName::Max_Hailstorm: return "Max Hailstorm"_s;
		case MoveName::Max_Ooze: return "Max Ooze"_s;
		case MoveName::Max_Geyser: return "Max Geyser"_s;
		case MoveName::Max_Airstream: return "Max Airstream"_s;
		case MoveName::Max_Starfall: return "Max Starfall"_s;
		case MoveName::Max_Wyrmwind: return "Max Wyrmwind"_s;
		case MoveName::Max_Mindstorm: return "Max Mindstorm"_s;
		case MoveName::Max_Rockfall: return "Max Rockfall"_s;
		case MoveName::Max_Quake: return "Max Quake"_s;
		case MoveName::Max_Darkness: return "Max Darkness"_s;
		case MoveName::Max_Overgrowth: return "Max Overgrowth"_s;
		case MoveName::Max_Steelspike: return "Max Steelspike"_s;
		case MoveName::Clangorous_Soul: return "Clangorous Soul"_s;
		case MoveName::Body_Press: return "Body Press"_s;
		case MoveName::Decorate: return "Decorate"_s;
		case MoveName::Drum_Beating: return "Drum Beating"_s;
		case MoveName::Snap_Trap: return "Snap Trap"_s;
		case MoveName::Pyro_Ball: return "Pyro Ball"_s;
		case MoveName::Behemoth_Blade: return "Behemoth Blade"_s;
		case MoveName::Behemoth_Bash: return "Behemoth Bash"_s;
		case MoveName::Aura_Wheel: return "Aura Wheel"_s;
		case MoveName::Breaking_Swipe: return "Breaking Swipe"_s;
		case MoveName::Branch_Poke: return "Branch Poke"_s;
		case MoveName::Overdrive: return "Overdrive"_s;
		case MoveName::Apple_Acid: return "Apple Acid"_s;
		case MoveName::Grav_Apple: return "Grav Apple"_s;
		case MoveName::Spirit_Break: return "Spirit Break"_s;
		case MoveName::Strange_Steam: return "Strange Steam"_s;
		case MoveName::Life_Dew: return "Life Dew"_s;
		case MoveName::Obstruct: return "Obstruct"_s;
		case MoveName::False_Surrender: return "False Surrender"_s;
		case MoveName::Meteor_Assault: return "Meteor Assault"_s;
		case MoveName::Eternabeam: return "Eternabeam"_s;
		case MoveName::Steel_Beam: return "Steel Beam"_s;
		case MoveName::Expanding_Force: return "Expanding Force"_s;
		case MoveName::Steel_Roller: return "Steel Roller"_s;
		case MoveName::Scale_Shot: return "Scale Shot"_s;
		case MoveName::Meteor_Beam: return "Meteor Beam"_s;
		case MoveName::Shell_Side_Arm: return "Shell Side Arm"_s;
		case MoveName::Misty_Explosion: return "Misty Explosion"_s;
		case MoveName::Grassy_Glide: return "Grassy Glide"_s;
		case MoveName::Rising_Voltage: return "Rising Voltage"_s;
		case MoveName::Terrain_Pulse: return "Terrain Pulse"_s;
		case MoveName::Skitter_Smack: return "Skitter Smack"_s;
		case MoveName::Burning_Jealousy: return "Burning Jealousy"_s;
		case MoveName::Lash_Out: return "Lash Out"_s;
		case MoveName::Poltergeist: return "Poltergeist"_s;
		case MoveName::Corrosive_Gas: return "Corrosive Gas"_s;
		case MoveName::Coaching: return "Coaching"_s;
		case MoveName::Flip_Turn: return "Flip Turn"_s;
		case MoveName::Triple_Axel: return "Triple Axel"_s;
		case MoveName::Dual_Wingbeat: return "Dual Wingbeat"_s;
		case MoveName::Scorching_Sands: return "Scorching Sands"_s;
		case MoveName::Jungle_Healing: return "Jungle Healing"_s;
		case MoveName::Wicked_Blow: return "Wicked Blow"_s;
		case MoveName::Surging_Strikes: return "Surging Strikes"_s;
		case MoveName::Thunder_Cage: return "Thunder Cage"_s;
		case MoveName::Dragon_Energy: return "Dragon Energy"_s;
		case MoveName::Freezing_Glare: return "Freezing Glare"_s;
		case MoveName::Fiery_Wrath: return "Fiery Wrath"_s;
		case MoveName::Thunderous_Kick: return "Thunderous Kick"_s;
		case MoveName::Glacial_Lance: return "Glacial Lance"_s;
		case MoveName::Astral_Barrage: return "Astral Barrage"_s;
		case MoveName::Eerie_Spell: return "Eerie Spell"_s;
		case MoveName::Tera_Blast: return "Tera Blast"_s;
		case MoveName::Silk_Trap: return "Silk Trap"_s;
		case MoveName::Axe_Kick: return "Axe Kick"_s;
		case MoveName::Last_Respects: return "Last Respects"_s;
		case MoveName::Lumina_Crash: return "Lumina Crash"_s;
		case MoveName::Order_Up: return "Order Up"_s;
		case MoveName::Jet_Punch: return "Jet Punch"_s;
		case MoveName::Spicy_Extract: return "Spicy Extract"_s;
		case MoveName::Spin_Out: return "Spin Out"_s;
		case MoveName::Population_Bomb: return "Population Bomb"_s;
		case MoveName::Ice_Spinner: return "Ice Spinner"_s;
		case MoveName::Glaive_Rush: return "Glaive Rush"_s;
		case MoveName::Revival_Blessing: return "Revival Blessing"_s;
		case MoveName::Salt_Cure: return "Salt Cure"_s;
		case MoveName::Triple_Dive: return "Triple Dive"_s;
		case MoveName::Mortal_Spin: return "Mortal Spin"_s;
		case MoveName::Doodle: return "Doodle"_s;
		case MoveName::Fillet_Away: return "Fillet Away"_s;
		case MoveName::Kowtow_Cleave: return "Kowtow Cleave"_s;
		case MoveName::Flower_Trick: return "Flower Trick"_s;
		case MoveName::Torch_Song: return "Torch Song"_s;
		case MoveName::Aqua_Step: return "Aqua Step"_s;
		case MoveName::Raging_Bull: return "Raging Bull"_s;
		case MoveName::Make_It_Rain: return "Make It Rain"_s;
		case MoveName::Psyblade: return "Psyblade"_s;
		case MoveName::Hydro_Steam: return "Hydro Steam"_s;
		case MoveName::Ruination: return "Ruination"_s;
		case MoveName::Collision_Course: return "Collision Course"_s;
		case MoveName::Electro_Drift: return "Electro Drift"_s;
		case MoveName::Shed_Tail: return "Shed Tail"_s;
		case MoveName::Chilly_Reception: return "Chilly Reception"_s;
		case MoveName::Tidy_Up: return "Tidy Up"_s;
		case MoveName::Snowscape: return "Snowscape"_s;
		case MoveName::Pounce: return "Pounce"_s;
		case MoveName::Trailblaze: return "Trailblaze"_s;
		case MoveName::Chilling_Water: return "Chilling Water"_s;
		case MoveName::Hyper_Drill: return "Hyper Drill"_s;
		case MoveName::Twin_Beam: return "Twin Beam"_s;
		case MoveName::Rage_Fist: return "Rage Fist"_s;
		case MoveName::Armor_Cannon: return "Armor Cannon"_s;
		case MoveName::Bitter_Blade: return "Bitter Blade"_s;
		case MoveName::Double_Shock: return "Double Shock"_s;
		case MoveName::Gigaton_Hammer: return "Gigaton Hammer"_s;
		case MoveName::Comeuppance: return "Comeuppance"_s;
		case MoveName::Aqua_Cutter: return "Aqua Cutter"_s;
		case MoveName::Blazing_Torque: return "Blazing Torque"_s;
		case MoveName::Wicked_Torque: return "Wicked Torque"_s;
		case MoveName::Noxious_Torque: return "Noxious Torque"_s;
		case MoveName::Combat_Torque: return "Combat Torque"_s;
		case MoveName::Magical_Torque: return "Magical Torque"_s;
		case MoveName::Blood_Moon: return "Blood Moon"_s;
		case MoveName::Matcha_Gotcha: return "Matcha Gotcha"_s;
		case MoveName::Syrup_Bomb: return "Syrup Bomb"_s;
		case MoveName::Ivy_Cudgel: return "Ivy Cudgel"_s;
		case MoveName::Electro_Shot: return "Electro Shot"_s;
		case MoveName::Thunderclap: return "Thunderclap"_s;
		case MoveName::Tachyon_Cutter: return "Tachyon Cutter"_s;
		case MoveName::Upper_Hand: return "Upper Hand"_s;
		case MoveName::Psychic_Noise: return "Psychic Noise"_s;
	}
}

export template<>
constexpr auto from_string(containers::string_view const str) -> MoveName {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, MoveName>>({
			{"10000000voltthunderbolt"_s, MoveName::m10000000_Volt_Thunderbolt},
			{"absorb"_s, MoveName::Absorb},
			{"accelerock"_s, MoveName::Accelerock},
			{"acid"_s, MoveName::Acid},
			{"acidarmor"_s, MoveName::Acid_Armor},
			{"aciddownpour"_s, MoveName::Acid_Downpour},
			{"acidspray"_s, MoveName::Acid_Spray},
			{"acrobatics"_s, MoveName::Acrobatics},
			{"acupressure"_s, MoveName::Acupressure},
			{"aerialace"_s, MoveName::Aerial_Ace},
			{"aeroblast"_s, MoveName::Aeroblast},
			{"afteryou"_s, MoveName::After_You},
			{"agility"_s, MoveName::Agility},
			{"aircutter"_s, MoveName::Air_Cutter},
			{"airslash"_s, MoveName::Air_Slash},
			{"alloutpummeling"_s, MoveName::All_Out_Pummeling},
			{"allyswitch"_s, MoveName::Ally_Switch},
			{"amnesia"_s, MoveName::Amnesia},
			{"anchorshot"_s, MoveName::Anchor_Shot},
			{"ancientpower"_s, MoveName::Ancient_Power},
			{"appleacid"_s, MoveName::Apple_Acid},
			{"aquacutter"_s, MoveName::Aqua_Cutter},
			{"aquajet"_s, MoveName::Aqua_Jet},
			{"aquaring"_s, MoveName::Aqua_Ring},
			{"aquastep"_s, MoveName::Aqua_Step},
			{"aquatail"_s, MoveName::Aqua_Tail},
			{"armorcannon"_s, MoveName::Armor_Cannon},
			{"armthrust"_s, MoveName::Arm_Thrust},
			{"aromatherapy"_s, MoveName::Aromatherapy},
			{"aromaticmist"_s, MoveName::Aromatic_Mist},
			{"assist"_s, MoveName::Assist},
			{"assurance"_s, MoveName::Assurance},
			{"astonish"_s, MoveName::Astonish},
			{"astralbarrage"_s, MoveName::Astral_Barrage},
			{"attackorder"_s, MoveName::Attack_Order},
			{"attract"_s, MoveName::Attract},
			{"aurasphere"_s, MoveName::Aura_Sphere},
			{"aurawheel"_s, MoveName::Aura_Wheel},
			{"aurorabeam"_s, MoveName::Aurora_Beam},
			{"auroraveil"_s, MoveName::Aurora_Veil},
			{"autotomize"_s, MoveName::Autotomize},
			{"avalanche"_s, MoveName::Avalanche},
			{"axekick"_s, MoveName::Axe_Kick},
			{"babydolleyes"_s, MoveName::Baby_Doll_Eyes},
			{"banefulbunker"_s, MoveName::Baneful_Bunker},
			{"barrage"_s, MoveName::Barrage},
			{"barrier"_s, MoveName::Barrier},
			{"batonpass"_s, MoveName::Baton_Pass},
			{"beakblast"_s, MoveName::Beak_Blast},
			{"beatup"_s, MoveName::Beat_Up},
			{"behemothbash"_s, MoveName::Behemoth_Bash},
			{"behemothblade"_s, MoveName::Behemoth_Blade},
			{"belch"_s, MoveName::Belch},
			{"bellydrum"_s, MoveName::Belly_Drum},
			{"bestow"_s, MoveName::Bestow},
			{"bide"_s, MoveName::Bide},
			{"bind"_s, MoveName::Bind},
			{"bite"_s, MoveName::Bite},
			{"bitterblade"_s, MoveName::Bitter_Blade},
			{"blackholeeclipse"_s, MoveName::Black_Hole_Eclipse},
			{"blastburn"_s, MoveName::Blast_Burn},
			{"blazekick"_s, MoveName::Blaze_Kick},
			{"blazingtorque"_s, MoveName::Blazing_Torque},
			{"blizzard"_s, MoveName::Blizzard},
			{"block"_s, MoveName::Block},
			{"bloodmoon"_s, MoveName::Blood_Moon},
			{"bloomdoom"_s, MoveName::Bloom_Doom},
			{"blueflare"_s, MoveName::Blue_Flare},
			{"bodypress"_s, MoveName::Body_Press},
			{"bodyslam"_s, MoveName::Body_Slam},
			{"boltbeak"_s, MoveName::Bolt_Beak},
			{"boltstrike"_s, MoveName::Bolt_Strike},
			{"boneclub"_s, MoveName::Bone_Club},
			{"bonemerang"_s, MoveName::Bonemerang},
			{"bonerush"_s, MoveName::Bone_Rush},
			{"boomburst"_s, MoveName::Boomburst},
			{"bounce"_s, MoveName::Bounce},
			{"branchpoke"_s, MoveName::Branch_Poke},
			{"bravebird"_s, MoveName::Brave_Bird},
			{"breakingswipe"_s, MoveName::Breaking_Swipe},
			{"breakneckblitz"_s, MoveName::Breakneck_Blitz},
			{"brickbreak"_s, MoveName::Brick_Break},
			{"brine"_s, MoveName::Brine},
			{"brutalswing"_s, MoveName::Brutal_Swing},
			{"bubble"_s, MoveName::Bubble},
			{"bubblebeam"_s, MoveName::Bubble_Beam},
			{"bugbite"_s, MoveName::Bug_Bite},
			{"bugbuzz"_s, MoveName::Bug_Buzz},
			{"bulkup"_s, MoveName::Bulk_Up},
			{"bulldoze"_s, MoveName::Bulldoze},
			{"bulletpunch"_s, MoveName::Bullet_Punch},
			{"bulletseed"_s, MoveName::Bullet_Seed},
			{"burningjealousy"_s, MoveName::Burning_Jealousy},
			{"burnup"_s, MoveName::Burn_Up},
			{"calmmind"_s, MoveName::Calm_Mind},
			{"camouflage"_s, MoveName::Camouflage},
			{"captivate"_s, MoveName::Captivate},
			{"catastropika"_s, MoveName::Catastropika},
			{"celebrate"_s, MoveName::Celebrate},
			{"charge"_s, MoveName::Charge},
			{"chargebeam"_s, MoveName::Charge_Beam},
			{"charm"_s, MoveName::Charm},
			{"chatter"_s, MoveName::Chatter},
			{"chillingwater"_s, MoveName::Chilling_Water},
			{"chillyreception"_s, MoveName::Chilly_Reception},
			{"chipaway"_s, MoveName::Chip_Away},
			{"circlethrow"_s, MoveName::Circle_Throw},
			{"clamp"_s, MoveName::Clamp},
			{"clangingscales"_s, MoveName::Clanging_Scales},
			{"clangoroussoul"_s, MoveName::Clangorous_Soul},
			{"clangoroussoulblaze"_s, MoveName::Clangorous_Soulblaze},
			{"clearsmog"_s, MoveName::Clear_Smog},
			{"closecombat"_s, MoveName::Close_Combat},
			{"coaching"_s, MoveName::Coaching},
			{"coil"_s, MoveName::Coil},
			{"collisioncourse"_s, MoveName::Collision_Course},
			{"combattorque"_s, MoveName::Combat_Torque},
			{"cometpunch"_s, MoveName::Comet_Punch},
			{"comeuppance"_s, MoveName::Comeuppance},
			{"confide"_s, MoveName::Confide},
			{"confuseray"_s, MoveName::Confuse_Ray},
			{"confusion"_s, MoveName::Confusion},
			{"constrict"_s, MoveName::Constrict},
			{"continentalcrush"_s, MoveName::Continental_Crush},
			{"conversion"_s, MoveName::Conversion},
			{"conversion2"_s, MoveName::Conversion_2},
			{"copycat"_s, MoveName::Copycat},
			{"coreenforcer"_s, MoveName::Core_Enforcer},
			{"corkscrewcrash"_s, MoveName::Corkscrew_Crash},
			{"corrosivegas"_s, MoveName::Corrosive_Gas},
			{"cosmicpower"_s, MoveName::Cosmic_Power},
			{"cottonguard"_s, MoveName::Cotton_Guard},
			{"cottonspore"_s, MoveName::Cotton_Spore},
			{"counter"_s, MoveName::Counter},
			{"courtchange"_s, MoveName::Court_Change},
			{"covet"_s, MoveName::Covet},
			{"crabhammer"_s, MoveName::Crabhammer},
			{"craftyshield"_s, MoveName::Crafty_Shield},
			{"crosschop"_s, MoveName::Cross_Chop},
			{"crosspoison"_s, MoveName::Cross_Poison},
			{"crunch"_s, MoveName::Crunch},
			{"crushclaw"_s, MoveName::Crush_Claw},
			{"crushgrip"_s, MoveName::Crush_Grip},
			{"curse"_s, MoveName::Curse},
			{"cut"_s, MoveName::Cut},
			{"darkestlariat"_s, MoveName::Darkest_Lariat},
			{"darkpulse"_s, MoveName::Dark_Pulse},
			{"darkvoid"_s, MoveName::Dark_Void},
			{"dazzlinggleam"_s, MoveName::Dazzling_Gleam},
			{"decorate"_s, MoveName::Decorate},
			{"defendorder"_s, MoveName::Defend_Order},
			{"defensecurl"_s, MoveName::Defense_Curl},
			{"defog"_s, MoveName::Defog},
			{"destinybond"_s, MoveName::Destiny_Bond},
			{"detect"_s, MoveName::Detect},
			{"devastatingdrake"_s, MoveName::Devastating_Drake},
			{"diamondstorm"_s, MoveName::Diamond_Storm},
			{"dig"_s, MoveName::Dig},
			{"disable"_s, MoveName::Disable},
			{"disarmingvoice"_s, MoveName::Disarming_Voice},
			{"discharge"_s, MoveName::Discharge},
			{"dive"_s, MoveName::Dive},
			{"dizzypunch"_s, MoveName::Dizzy_Punch},
			{"doodle"_s, MoveName::Doodle},
			{"doomdesire"_s, MoveName::Doom_Desire},
			{"doubleedge"_s, MoveName::Double_Edge},
			{"doublehit"_s, MoveName::Double_Hit},
			{"doubleironbash"_s, MoveName::Double_Iron_Bash},
			{"doublekick"_s, MoveName::Double_Kick},
			{"doubleshock"_s, MoveName::Double_Shock},
			{"doubleslap"_s, MoveName::Double_Slap},
			{"doubleteam"_s, MoveName::Double_Team},
			{"dracometeor"_s, MoveName::Draco_Meteor},
			{"dragonascent"_s, MoveName::Dragon_Ascent},
			{"dragonbreath"_s, MoveName::Dragon_Breath},
			{"dragonclaw"_s, MoveName::Dragon_Claw},
			{"dragondance"_s, MoveName::Dragon_Dance},
			{"dragondarts"_s, MoveName::Dragon_Darts},
			{"dragonenergy"_s, MoveName::Dragon_Energy},
			{"dragonhammer"_s, MoveName::Dragon_Hammer},
			{"dragonpulse"_s, MoveName::Dragon_Pulse},
			{"dragonrage"_s, MoveName::Dragon_Rage},
			{"dragonrush"_s, MoveName::Dragon_Rush},
			{"dragontail"_s, MoveName::Dragon_Tail},
			{"drainingkiss"_s, MoveName::Draining_Kiss},
			{"drainpunch"_s, MoveName::Drain_Punch},
			{"dreameater"_s, MoveName::Dream_Eater},
			{"drillpeck"_s, MoveName::Drill_Peck},
			{"drillrun"_s, MoveName::Drill_Run},
			{"drumbeating"_s, MoveName::Drum_Beating},
			{"dualchop"_s, MoveName::Dual_Chop},
			{"dualwingbeat"_s, MoveName::Dual_Wingbeat},
			{"dynamaxcannon"_s, MoveName::Dynamax_Cannon},
			{"dynamicpunch"_s, MoveName::Dynamic_Punch},
			{"earthpower"_s, MoveName::Earth_Power},
			{"earthquake"_s, MoveName::Earthquake},
			{"echoedvoice"_s, MoveName::Echoed_Voice},
			{"eerieimpulse"_s, MoveName::Eerie_Impulse},
			{"eeriespell"_s, MoveName::Eerie_Spell},
			{"eggbomb"_s, MoveName::Egg_Bomb},
			{"electricterrain"_s, MoveName::Electric_Terrain},
			{"electrify"_s, MoveName::Electrify},
			{"electroball"_s, MoveName::Electro_Ball},
			{"electrodrift"_s, MoveName::Electro_Drift},
			{"electroshot"_s, MoveName::Electro_Shot},
			{"electroweb"_s, MoveName::Electroweb},
			{"embargo"_s, MoveName::Embargo},
			{"ember"_s, MoveName::Ember},
			{"encore"_s, MoveName::Encore},
			{"endeavor"_s, MoveName::Endeavor},
			{"endure"_s, MoveName::Endure},
			{"energyball"_s, MoveName::Energy_Ball},
			{"entrainment"_s, MoveName::Entrainment},
			{"eruption"_s, MoveName::Eruption},
			{"eternabeam"_s, MoveName::Eternabeam},
			{"expandingforce"_s, MoveName::Expanding_Force},
			{"explosion"_s, MoveName::Explosion},
			{"extrasensory"_s, MoveName::Extrasensory},
			{"extremeevoboost"_s, MoveName::Extreme_Evoboost},
			{"extremespeed"_s, MoveName::Extreme_Speed},
			{"facade"_s, MoveName::Facade},
			{"faintattack"_s, MoveName::Feint_Attack},
			{"fairylock"_s, MoveName::Fairy_Lock},
			{"fairywind"_s, MoveName::Fairy_Wind},
			{"fakeout"_s, MoveName::Fake_Out},
			{"faketears"_s, MoveName::Fake_Tears},
			{"falsesurrender"_s, MoveName::False_Surrender},
			{"falseswipe"_s, MoveName::False_Swipe},
			{"featherdance"_s, MoveName::Feather_Dance},
			{"feint"_s, MoveName::Feint},
			{"feintattack"_s, MoveName::Feint_Attack},
			{"fellstinger"_s, MoveName::Fell_Stinger},
			{"fierydance"_s, MoveName::Fiery_Dance},
			{"fierywrath"_s, MoveName::Fiery_Wrath},
			{"filletaway"_s, MoveName::Fillet_Away},
			{"finalgambit"_s, MoveName::Final_Gambit},
			{"fireblast"_s, MoveName::Fire_Blast},
			{"firefang"_s, MoveName::Fire_Fang},
			{"firelash"_s, MoveName::Fire_Lash},
			{"firepledge"_s, MoveName::Fire_Pledge},
			{"firepunch"_s, MoveName::Fire_Punch},
			{"firespin"_s, MoveName::Fire_Spin},
			{"firstimpression"_s, MoveName::First_Impression},
			{"fishiousrend"_s, MoveName::Fishious_Rend},
			{"fissure"_s, MoveName::Fissure},
			{"flail"_s, MoveName::Flail},
			{"flameburst"_s, MoveName::Flame_Burst},
			{"flamecharge"_s, MoveName::Flame_Charge},
			{"flamethrower"_s, MoveName::Flamethrower},
			{"flamewheel"_s, MoveName::Flame_Wheel},
			{"flareblitz"_s, MoveName::Flare_Blitz},
			{"flash"_s, MoveName::Flash},
			{"flashcannon"_s, MoveName::Flash_Cannon},
			{"flatter"_s, MoveName::Flatter},
			{"fleurcannon"_s, MoveName::Fleur_Cannon},
			{"fling"_s, MoveName::Fling},
			{"flipturn"_s, MoveName::Flip_Turn},
			{"floralhealing"_s, MoveName::Floral_Healing},
			{"flowershield"_s, MoveName::Flower_Shield},
			{"flowertrick"_s, MoveName::Flower_Trick},
			{"fly"_s, MoveName::Fly},
			{"flyingpress"_s, MoveName::Flying_Press},
			{"focusblast"_s, MoveName::Focus_Blast},
			{"focusenergy"_s, MoveName::Focus_Energy},
			{"focuspunch"_s, MoveName::Focus_Punch},
			{"followme"_s, MoveName::Follow_Me},
			{"forcepalm"_s, MoveName::Force_Palm},
			{"foresight"_s, MoveName::Foresight},
			{"forestscurse"_s, MoveName::Forests_Curse},
			{"foulplay"_s, MoveName::Foul_Play},
			{"freezedry"_s, MoveName::Freeze_Dry},
			{"freezeshock"_s, MoveName::Freeze_Shock},
			{"freezingglare"_s, MoveName::Freezing_Glare},
			{"frenzyplant"_s, MoveName::Frenzy_Plant},
			{"frostbreath"_s, MoveName::Frost_Breath},
			{"frustration"_s, MoveName::Frustration},
			{"frustrationnull"_s, MoveName::Frustration}, // Pokemon Showdown
			{"furyattack"_s, MoveName::Fury_Attack},
			{"furycutter"_s, MoveName::Fury_Cutter},
			{"furyswipes"_s, MoveName::Fury_Swipes},
			{"fusionbolt"_s, MoveName::Fusion_Bolt},
			{"fusionflare"_s, MoveName::Fusion_Flare},
			{"futuresight"_s, MoveName::Future_Sight},
			{"gastroacid"_s, MoveName::Gastro_Acid},
			{"geargrind"_s, MoveName::Gear_Grind},
			{"gearup"_s, MoveName::Gear_Up},
			{"genesissupernova"_s, MoveName::Genesis_Supernova},
			{"geomancy"_s, MoveName::Geomancy},
			{"gigadrain"_s, MoveName::Giga_Drain},
			{"gigaimpact"_s, MoveName::Giga_Impact},
			{"gigatonhammer"_s, MoveName::Gigaton_Hammer},
			{"gigavolthavoc"_s, MoveName::Gigavolt_Havoc},
			{"glaciallance"_s, MoveName::Glacial_Lance},
			{"glaciate"_s, MoveName::Glaciate},
			{"glaiverush"_s, MoveName::Glaive_Rush},
			{"glare"_s, MoveName::Glare},
			{"grassknot"_s, MoveName::Grass_Knot},
			{"grasspledge"_s, MoveName::Grass_Pledge},
			{"grasswhistle"_s, MoveName::Grass_Whistle},
			{"grassyglide"_s, MoveName::Grassy_Glide},
			{"grassyterrain"_s, MoveName::Grassy_Terrain},
			{"gravapple"_s, MoveName::Grav_Apple},
			{"gravity"_s, MoveName::Gravity},
			{"growl"_s, MoveName::Growl},
			{"growth"_s, MoveName::Growth},
			{"grudge"_s, MoveName::Grudge},
			{"guardianofalola"_s, MoveName::Guardian_of_Alola},
			{"guardsplit"_s, MoveName::Guard_Split},
			{"guardswap"_s, MoveName::Guard_Swap},
			{"guillotine"_s, MoveName::Guillotine},
			{"gunkshot"_s, MoveName::Gunk_Shot},
			{"gust"_s, MoveName::Gust},
			{"gyroball"_s, MoveName::Gyro_Ball},
			{"hail"_s, MoveName::Hail},
			{"hammerarm"_s, MoveName::Hammer_Arm},
			{"happyhour"_s, MoveName::Happy_Hour},
			{"harden"_s, MoveName::Harden},
			{"haze"_s, MoveName::Haze},
			{"headbutt"_s, MoveName::Headbutt},
			{"headcharge"_s, MoveName::Head_Charge},
			{"headsmash"_s, MoveName::Head_Smash},
			{"healbell"_s, MoveName::Heal_Bell},
			{"healblock"_s, MoveName::Heal_Block},
			{"healingwish"_s, MoveName::Healing_Wish},
			{"healorder"_s, MoveName::Heal_Order},
			{"healpulse"_s, MoveName::Heal_Pulse},
			{"heartstamp"_s, MoveName::Heart_Stamp},
			{"heartswap"_s, MoveName::Heart_Swap},
			{"heatcrash"_s, MoveName::Heat_Crash},
			{"heatwave"_s, MoveName::Heat_Wave},
			{"heavyslam"_s, MoveName::Heavy_Slam},
			{"helpinghand"_s, MoveName::Helping_Hand},
			{"hex"_s, MoveName::Hex},
			{"hiddenpower"_s, MoveName::Hidden_Power},
			{"hiddenpowerbug"_s, MoveName::Hidden_Power},
			{"hiddenpowerdark"_s, MoveName::Hidden_Power},
			{"hiddenpowerdragon"_s, MoveName::Hidden_Power},
			{"hiddenpowerelectric"_s, MoveName::Hidden_Power},
			{"hiddenpowerfighting"_s, MoveName::Hidden_Power},
			{"hiddenpowerfire"_s, MoveName::Hidden_Power},
			{"hiddenpowerflying"_s, MoveName::Hidden_Power},
			{"hiddenpowerghost"_s, MoveName::Hidden_Power},
			{"hiddenpowergrass"_s, MoveName::Hidden_Power},
			{"hiddenpowerground"_s, MoveName::Hidden_Power},
			{"hiddenpowerice"_s, MoveName::Hidden_Power},
			{"hiddenpowerpoison"_s, MoveName::Hidden_Power},
			{"hiddenpowerpsychic"_s, MoveName::Hidden_Power},
			{"hiddenpowerrock"_s, MoveName::Hidden_Power},
			{"hiddenpowersteel"_s, MoveName::Hidden_Power},
			{"hiddenpowerwater"_s, MoveName::Hidden_Power},
			{"highhorsepower"_s, MoveName::High_Horsepower},
			{"highjumpkick"_s, MoveName::High_Jump_Kick},
			{"hijumpkick"_s, MoveName::High_Jump_Kick},
			{"holdback"_s, MoveName::Hold_Back},
			{"holdhands"_s, MoveName::Hold_Hands},
			{"honeclaws"_s, MoveName::Hone_Claws},
			{"hornattack"_s, MoveName::Horn_Attack},
			{"horndrill"_s, MoveName::Horn_Drill},
			{"hornleech"_s, MoveName::Horn_Leech},
			{"howl"_s, MoveName::Howl},
			{"hpbug"_s, MoveName::Hidden_Power},
			{"hpdark"_s, MoveName::Hidden_Power},
			{"hpdragon"_s, MoveName::Hidden_Power},
			{"hpelectric"_s, MoveName::Hidden_Power},
			{"hpfighting"_s, MoveName::Hidden_Power},
			{"hpfire"_s, MoveName::Hidden_Power},
			{"hpflying"_s, MoveName::Hidden_Power},
			{"hpghost"_s, MoveName::Hidden_Power},
			{"hpgrass"_s, MoveName::Hidden_Power},
			{"hpground"_s, MoveName::Hidden_Power},
			{"hpice"_s, MoveName::Hidden_Power},
			{"hppoison"_s, MoveName::Hidden_Power},
			{"hppsychic"_s, MoveName::Hidden_Power},
			{"hprock"_s, MoveName::Hidden_Power},
			{"hpsteel"_s, MoveName::Hidden_Power},
			{"hpwater"_s, MoveName::Hidden_Power},
			{"hurricane"_s, MoveName::Hurricane},
			{"hydrocannon"_s, MoveName::Hydro_Cannon},
			{"hydropump"_s, MoveName::Hydro_Pump},
			{"hydrosteam"_s, MoveName::Hydro_Steam},
			{"hydrovortex"_s, MoveName::Hydro_Vortex},
			{"hyperbeam"_s, MoveName::Hyper_Beam},
			{"hyperdrill"_s, MoveName::Hyper_Drill},
			{"hyperfang"_s, MoveName::Hyper_Fang},
			{"hyperspacefury"_s, MoveName::Hyperspace_Fury},
			{"hyperspacehole"_s, MoveName::Hyperspace_Hole},
			{"hypervoice"_s, MoveName::Hyper_Voice},
			{"hypnosis"_s, MoveName::Hypnosis},
			{"iceball"_s, MoveName::Ice_Ball},
			{"icebeam"_s, MoveName::Ice_Beam},
			{"iceburn"_s, MoveName::Ice_Burn},
			{"icefang"_s, MoveName::Ice_Fang},
			{"icehammer"_s, MoveName::Ice_Hammer},
			{"icepunch"_s, MoveName::Ice_Punch},
			{"iceshard"_s, MoveName::Ice_Shard},
			{"icespinner"_s, MoveName::Ice_Spinner},
			{"iciclecrash"_s, MoveName::Icicle_Crash},
			{"iciclespear"_s, MoveName::Icicle_Spear},
			{"icywind"_s, MoveName::Icy_Wind},
			{"imprison"_s, MoveName::Imprison},
			{"incinerate"_s, MoveName::Incinerate},
			{"inferno"_s, MoveName::Inferno},
			{"infernooverdrive"_s, MoveName::Inferno_Overdrive},
			{"infestation"_s, MoveName::Infestation},
			{"ingrain"_s, MoveName::Ingrain},
			{"instruct"_s, MoveName::Instruct},
			{"iondeluge"_s, MoveName::Ion_Deluge},
			{"irondefense"_s, MoveName::Iron_Defense},
			{"ironhead"_s, MoveName::Iron_Head},
			{"irontail"_s, MoveName::Iron_Tail},
			{"ivycudgel"_s, MoveName::Ivy_Cudgel},
			{"jawlock"_s, MoveName::Jaw_Lock},
			{"jetpunch"_s, MoveName::Jet_Punch},
			{"judgment"_s, MoveName::Judgment},
			{"jumpkick"_s, MoveName::Jump_Kick},
			{"junglehealing"_s, MoveName::Jungle_Healing},
			{"karatechop"_s, MoveName::Karate_Chop},
			{"kinesis"_s, MoveName::Kinesis},
			{"kingsshield"_s, MoveName::Kings_Shield},
			{"knockoff"_s, MoveName::Knock_Off},
			{"kowtowcleave"_s, MoveName::Kowtow_Cleave},
			{"landswrath"_s, MoveName::Lands_Wrath},
			{"laserfocus"_s, MoveName::Laser_Focus},
			{"lashout"_s, MoveName::Lash_Out},
			{"lastresort"_s, MoveName::Last_Resort},
			{"lastrespects"_s, MoveName::Last_Respects},
			{"lavaplume"_s, MoveName::Lava_Plume},
			{"leafage"_s, MoveName::Leafage},
			{"leafblade"_s, MoveName::Leaf_Blade},
			{"leafstorm"_s, MoveName::Leaf_Storm},
			{"leaftornado"_s, MoveName::Leaf_Tornado},
			{"leechlife"_s, MoveName::Leech_Life},
			{"leechseed"_s, MoveName::Leech_Seed},
			{"leer"_s, MoveName::Leer},
			{"letssnuggleforever"_s, MoveName::Lets_Snuggle_Forever},
			{"lick"_s, MoveName::Lick},
			{"lifedew"_s, MoveName::Life_Dew},
			{"lightofruin"_s, MoveName::Light_of_Ruin},
			{"lightscreen"_s, MoveName::Light_Screen},
			{"lightthatburnsthesky"_s, MoveName::Light_That_Burns_the_Sky},
			{"liquidation"_s, MoveName::Liquidation},
			{"lockon"_s, MoveName::Lock_On},
			{"lovelykiss"_s, MoveName::Lovely_Kiss},
			{"lowkick"_s, MoveName::Low_Kick},
			{"lowsweep"_s, MoveName::Low_Sweep},
			{"luckychant"_s, MoveName::Lucky_Chant},
			{"luminacrash"_s, MoveName::Lumina_Crash},
			{"lunardance"_s, MoveName::Lunar_Dance},
			{"lunge"_s, MoveName::Lunge},
			{"lusterpurge"_s, MoveName::Luster_Purge},
			{"machpunch"_s, MoveName::Mach_Punch},
			{"magicalleaf"_s, MoveName::Magical_Leaf},
			{"magicaltorque"_s, MoveName::Magical_Torque},
			{"magiccoat"_s, MoveName::Magic_Coat},
			{"magicpowder"_s, MoveName::Magic_Powder},
			{"magicroom"_s, MoveName::Magic_Room},
			{"magmastorm"_s, MoveName::Magma_Storm},
			{"magnetbomb"_s, MoveName::Magnet_Bomb},
			{"magneticflux"_s, MoveName::Magnetic_Flux},
			{"magnetrise"_s, MoveName::Magnet_Rise},
			{"magnitude"_s, MoveName::Magnitude},
			{"makeitrain"_s, MoveName::Make_It_Rain},
			{"maliciousmoonsault"_s, MoveName::Malicious_Moonsault},
			{"matblock"_s, MoveName::Mat_Block},
			{"matchagotcha"_s, MoveName::Matcha_Gotcha},
			{"maxairstream"_s, MoveName::Max_Airstream},
			{"maxdarkness"_s, MoveName::Max_Darkness},
			{"maxflare"_s, MoveName::Max_Flare},
			{"maxflutterby"_s, MoveName::Max_Flutterby},
			{"maxgeyser"_s, MoveName::Max_Geyser},
			{"maxguard"_s, MoveName::Max_Guard},
			{"maxhailstorm"_s, MoveName::Max_Hailstorm},
			{"maxknuckle"_s, MoveName::Max_Knuckle},
			{"maxlightning"_s, MoveName::Max_Lightning},
			{"maxmindstorm"_s, MoveName::Max_Mindstorm},
			{"maxooze"_s, MoveName::Max_Ooze},
			{"maxovergrowth"_s, MoveName::Max_Overgrowth},
			{"maxphantasm"_s, MoveName::Max_Phantasm},
			{"maxquake"_s, MoveName::Max_Quake},
			{"maxrockfall"_s, MoveName::Max_Rockfall},
			{"maxstarfall"_s, MoveName::Max_Starfall},
			{"maxsteelspike"_s, MoveName::Max_Steelspike},
			{"maxstrike"_s, MoveName::Max_Strike},
			{"maxwyrmwind"_s, MoveName::Max_Wyrmwind},
			{"meanlook"_s, MoveName::Mean_Look},
			{"meditate"_s, MoveName::Meditate},
			{"mefirst"_s, MoveName::Me_First},
			{"megadrain"_s, MoveName::Mega_Drain},
			{"megahorn"_s, MoveName::Megahorn},
			{"megakick"_s, MoveName::Mega_Kick},
			{"megapunch"_s, MoveName::Mega_Punch},
			{"memento"_s, MoveName::Memento},
			{"menacingmoonrazemaelstrom"_s, MoveName::Menacing_Moonraze_Maelstrom},
			{"metalburst"_s, MoveName::Metal_Burst},
			{"metalclaw"_s, MoveName::Metal_Claw},
			{"metalsound"_s, MoveName::Metal_Sound},
			{"meteorassault"_s, MoveName::Meteor_Assault},
			{"meteorbeam"_s, MoveName::Meteor_Beam},
			{"meteormash"_s, MoveName::Meteor_Mash},
			{"metronome"_s, MoveName::Metronome},
			{"milkdrink"_s, MoveName::Milk_Drink},
			{"mimic"_s, MoveName::Mimic},
			{"mindblown"_s, MoveName::Mind_Blown},
			{"mindreader"_s, MoveName::Mind_Reader},
			{"minimize"_s, MoveName::Minimize},
			{"miracleeye"_s, MoveName::Miracle_Eye},
			{"mirrorcoat"_s, MoveName::Mirror_Coat},
			{"mirrormove"_s, MoveName::Mirror_Move},
			{"mirrorshot"_s, MoveName::Mirror_Shot},
			{"mist"_s, MoveName::Mist},
			{"mistball"_s, MoveName::Mist_Ball},
			{"mistyexplosion"_s, MoveName::Misty_Explosion},
			{"mistyterrain"_s, MoveName::Misty_Terrain},
			{"moonblast"_s, MoveName::Moonblast},
			{"moongeistbeam"_s, MoveName::Moongeist_Beam},
			{"moonlight"_s, MoveName::Moonlight},
			{"morningsun"_s, MoveName::Morning_Sun},
			{"mortalspin"_s, MoveName::Mortal_Spin},
			{"mudbomb"_s, MoveName::Mud_Bomb},
			{"muddywater"_s, MoveName::Muddy_Water},
			{"mudshot"_s, MoveName::Mud_Shot},
			{"mudslap"_s, MoveName::Mud_Slap},
			{"mudsport"_s, MoveName::Mud_Sport},
			{"multiattack"_s, MoveName::Multi_Attack},
			{"mysticalfire"_s, MoveName::Mystical_Fire},
			{"nastyplot"_s, MoveName::Nasty_Plot},
			{"naturalgift"_s, MoveName::Natural_Gift},
			{"naturepower"_s, MoveName::Nature_Power},
			{"naturesmadness"_s, MoveName::Natures_Madness},
			{"needlearm"_s, MoveName::Needle_Arm},
			{"neverendingnightmare"_s, MoveName::Never_Ending_Nightmare},
			{"nightdaze"_s, MoveName::Night_Daze},
			{"nightmare"_s, MoveName::Nightmare},
			{"nightshade"_s, MoveName::Night_Shade},
			{"nightslash"_s, MoveName::Night_Slash},
			{"nobleroar"_s, MoveName::Noble_Roar},
			{"noretreat"_s, MoveName::No_Retreat},
			{"noxioustorque"_s, MoveName::Noxious_Torque},
			{"nuzzle"_s, MoveName::Nuzzle},
			{"oblivionwing"_s, MoveName::Oblivion_Wing},
			{"obstruct"_s, MoveName::Obstruct},
			{"oceanicoperetta"_s, MoveName::Oceanic_Operetta},
			{"octazooka"_s, MoveName::Octazooka},
			{"octolock"_s, MoveName::Octolock},
			{"odorsleuth"_s, MoveName::Odor_Sleuth},
			{"ominouswind"_s, MoveName::Ominous_Wind},
			{"orderup"_s, MoveName::Order_Up},
			{"originpulse"_s, MoveName::Origin_Pulse},
			{"outrage"_s, MoveName::Outrage},
			{"overdrive"_s, MoveName::Overdrive},
			{"overheat"_s, MoveName::Overheat},
			{"painsplit"_s, MoveName::Pain_Split},
			{"paraboliccharge"_s, MoveName::Parabolic_Charge},
			{"partingshot"_s, MoveName::Parting_Shot},
			{"payback"_s, MoveName::Payback},
			{"payday"_s, MoveName::Pay_Day},
			{"peck"_s, MoveName::Peck},
			{"perishsong"_s, MoveName::Perish_Song},
			{"petalblizzard"_s, MoveName::Petal_Blizzard},
			{"petaldance"_s, MoveName::Petal_Dance},
			{"phantomforce"_s, MoveName::Phantom_Force},
			{"photongeyser"_s, MoveName::Photon_Geyser},
			{"pinmissile"_s, MoveName::Pin_Missile},
			{"plasmafists"_s, MoveName::Plasma_Fists},
			{"playnice"_s, MoveName::Play_Nice},
			{"playrough"_s, MoveName::Play_Rough},
			{"pluck"_s, MoveName::Pluck},
			{"poisonfang"_s, MoveName::Poison_Fang},
			{"poisongas"_s, MoveName::Poison_Gas},
			{"poisonjab"_s, MoveName::Poison_Jab},
			{"poisonpowder"_s, MoveName::Poison_Powder},
			{"poisonsting"_s, MoveName::Poison_Sting},
			{"poisontail"_s, MoveName::Poison_Tail},
			{"pollenpuff"_s, MoveName::Pollen_Puff},
			{"poltergeist"_s, MoveName::Poltergeist},
			{"populationbomb"_s, MoveName::Population_Bomb},
			{"pounce"_s, MoveName::Pounce},
			{"pound"_s, MoveName::Pound},
			{"powder"_s, MoveName::Powder},
			{"powdersnow"_s, MoveName::Powder_Snow},
			{"powergem"_s, MoveName::Power_Gem},
			{"powersplit"_s, MoveName::Power_Split},
			{"powerswap"_s, MoveName::Power_Swap},
			{"powertrick"_s, MoveName::Power_Trick},
			{"powertrip"_s, MoveName::Power_Trip},
			{"poweruppunch"_s, MoveName::Power_Up_Punch},
			{"powerwhip"_s, MoveName::Power_Whip},
			{"precipiceblades"_s, MoveName::Precipice_Blades},
			{"present"_s, MoveName::Present},
			{"prismaticlaser"_s, MoveName::Prismatic_Laser},
			{"protect"_s, MoveName::Protect},
			{"psybeam"_s, MoveName::Psybeam},
			{"psyblade"_s, MoveName::Psyblade},
			{"psychic"_s, MoveName::Psychic},
			{"psychicfangs"_s, MoveName::Psychic_Fangs},
			{"psychicnoise"_s, MoveName::Psychic_Noise},
			{"psychicterrain"_s, MoveName::Psychic_Terrain},
			{"psychoboost"_s, MoveName::Psycho_Boost},
			{"psychocut"_s, MoveName::Psycho_Cut},
			{"psychoshift"_s, MoveName::Psycho_Shift},
			{"psychup"_s, MoveName::Psych_Up},
			{"psyshock"_s, MoveName::Psyshock},
			{"psystrike"_s, MoveName::Psystrike},
			{"psywave"_s, MoveName::Psywave},
			{"pulverizingpancake"_s, MoveName::Pulverizing_Pancake},
			{"punishment"_s, MoveName::Punishment},
			{"purify"_s, MoveName::Purify},
			{"pursuit"_s, MoveName::Pursuit},
			{"pyroball"_s, MoveName::Pyro_Ball},
			{"quash"_s, MoveName::Quash},
			{"quickattack"_s, MoveName::Quick_Attack},
			{"quickguard"_s, MoveName::Quick_Guard},
			{"quiverdance"_s, MoveName::Quiver_Dance},
			{"rage"_s, MoveName::Rage},
			{"ragefist"_s, MoveName::Rage_Fist},
			{"ragepowder"_s, MoveName::Rage_Powder},
			{"ragingbull"_s, MoveName::Raging_Bull},
			{"raindance"_s, MoveName::Rain_Dance},
			{"rapidspin"_s, MoveName::Rapid_Spin},
			{"razorleaf"_s, MoveName::Razor_Leaf},
			{"razorshell"_s, MoveName::Razor_Shell},
			{"razorwind"_s, MoveName::Razor_Wind},
			{"recharge"_s, MoveName::Hyper_Beam}, // TODO: fix
			{"recover"_s, MoveName::Recover},
			{"recycle"_s, MoveName::Recycle},
			{"reflect"_s, MoveName::Reflect},
			{"reflecttype"_s, MoveName::Reflect_Type},
			{"refresh"_s, MoveName::Refresh},
			{"relicsong"_s, MoveName::Relic_Song},
			{"rest"_s, MoveName::Rest},
			{"retaliate"_s, MoveName::Retaliate},
			{"return"_s, MoveName::Return},
			{"return102"_s, MoveName::Return},
			{"revelationdance"_s, MoveName::Revelation_Dance},
			{"revenge"_s, MoveName::Revenge},
			{"reversal"_s, MoveName::Reversal},
			{"revivalblessing"_s, MoveName::Revival_Blessing},
			{"risingvoltage"_s, MoveName::Rising_Voltage},
			{"roar"_s, MoveName::Roar},
			{"roaroftime"_s, MoveName::Roar_of_Time},
			{"rockblast"_s, MoveName::Rock_Blast},
			{"rockclimb"_s, MoveName::Rock_Climb},
			{"rockpolish"_s, MoveName::Rock_Polish},
			{"rockslide"_s, MoveName::Rock_Slide},
			{"rocksmash"_s, MoveName::Rock_Smash},
			{"rockthrow"_s, MoveName::Rock_Throw},
			{"rocktomb"_s, MoveName::Rock_Tomb},
			{"rockwrecker"_s, MoveName::Rock_Wrecker},
			{"roleplay"_s, MoveName::Role_Play},
			{"rollingkick"_s, MoveName::Rolling_Kick},
			{"rollout"_s, MoveName::Rollout},
			{"roost"_s, MoveName::Roost},
			{"rototiller"_s, MoveName::Rototiller},
			{"round"_s, MoveName::Round},
			{"ruination"_s, MoveName::Ruination},
			{"sacredfire"_s, MoveName::Sacred_Fire},
			{"sacredsword"_s, MoveName::Sacred_Sword},
			{"safeguard"_s, MoveName::Safeguard},
			{"saltcure"_s, MoveName::Salt_Cure},
			{"sandattack"_s, MoveName::Sand_Attack},
			{"sandstorm"_s, MoveName::Sandstorm},
			{"sandtomb"_s, MoveName::Sand_Tomb},
			{"savagespinout"_s, MoveName::Savage_Spin_Out},
			{"scald"_s, MoveName::Scald},
			{"scaleshot"_s, MoveName::Scale_Shot},
			{"scaryface"_s, MoveName::Scary_Face},
			{"scorchingsands"_s, MoveName::Scorching_Sands},
			{"scratch"_s, MoveName::Scratch},
			{"screech"_s, MoveName::Screech},
			{"searingshot"_s, MoveName::Searing_Shot},
			{"searingsunrazesmash"_s, MoveName::Searing_Sunraze_Smash},
			{"secretpower"_s, MoveName::Secret_Power},
			{"secretsword"_s, MoveName::Secret_Sword},
			{"seedbomb"_s, MoveName::Seed_Bomb},
			{"seedflare"_s, MoveName::Seed_Flare},
			{"seismictoss"_s, MoveName::Seismic_Toss},
			{"selfdestruct"_s, MoveName::Self_Destruct},
			{"shadowball"_s, MoveName::Shadow_Ball},
			{"shadowbone"_s, MoveName::Shadow_Bone},
			{"shadowclaw"_s, MoveName::Shadow_Claw},
			{"shadowforce"_s, MoveName::Shadow_Force},
			{"shadowpunch"_s, MoveName::Shadow_Punch},
			{"shadowsneak"_s, MoveName::Shadow_Sneak},
			{"sharpen"_s, MoveName::Sharpen},
			{"shatteredpsyche"_s, MoveName::Shattered_Psyche},
			{"shedtail"_s, MoveName::Shed_Tail},
			{"sheercold"_s, MoveName::Sheer_Cold},
			{"shellsidearm"_s, MoveName::Shell_Side_Arm},
			{"shellsmash"_s, MoveName::Shell_Smash},
			{"shelltrap"_s, MoveName::Shell_Trap},
			{"shiftgear"_s, MoveName::Shift_Gear},
			{"shockwave"_s, MoveName::Shock_Wave},
			{"shoreup"_s, MoveName::Shore_Up},
			{"signalbeam"_s, MoveName::Signal_Beam},
			{"silktrap"_s, MoveName::Silk_Trap},
			{"silverwind"_s, MoveName::Silver_Wind},
			{"simplebeam"_s, MoveName::Simple_Beam},
			{"sing"_s, MoveName::Sing},
			{"sinisterarrowraid"_s, MoveName::Sinister_Arrow_Raid},
			{"sketch"_s, MoveName::Sketch},
			{"skillswap"_s, MoveName::Skill_Swap},
			{"skittersmack"_s, MoveName::Skitter_Smack},
			{"skullbash"_s, MoveName::Skull_Bash},
			{"skyattack"_s, MoveName::Sky_Attack},
			{"skydrop"_s, MoveName::Sky_Drop},
			{"skyuppercut"_s, MoveName::Sky_Uppercut},
			{"slackoff"_s, MoveName::Slack_Off},
			{"slam"_s, MoveName::Slam},
			{"slash"_s, MoveName::Slash},
			{"sleeppowder"_s, MoveName::Sleep_Powder},
			{"sleeptalk"_s, MoveName::Sleep_Talk},
			{"sludge"_s, MoveName::Sludge},
			{"sludgebomb"_s, MoveName::Sludge_Bomb},
			{"sludgewave"_s, MoveName::Sludge_Wave},
			{"smackdown"_s, MoveName::Smack_Down},
			{"smartstrike"_s, MoveName::Smart_Strike},
			{"smellingsalt"_s, MoveName::Smelling_Salts},
			{"smellingsalts"_s, MoveName::Smelling_Salts},
			{"smog"_s, MoveName::Smog},
			{"smokescreen"_s, MoveName::Smokescreen},
			{"snaptrap"_s, MoveName::Snap_Trap},
			{"snarl"_s, MoveName::Snarl},
			{"snatch"_s, MoveName::Snatch},
			{"snipeshot"_s, MoveName::Snipe_Shot},
			{"snore"_s, MoveName::Snore},
			{"snowscape"_s, MoveName::Snowscape},
			{"soak"_s, MoveName::Soak},
			{"softboiled"_s, MoveName::Soft_Boiled},
			{"solarbeam"_s, MoveName::Solar_Beam},
			{"solarblade"_s, MoveName::Solar_Blade},
			{"sonicboom"_s, MoveName::Sonic_Boom},
			{"soulstealing7starstrike"_s, MoveName::Soul_Stealing_7_Star_Strike},
			{"spacialrend"_s, MoveName::Spacial_Rend},
			{"spark"_s, MoveName::Spark},
			{"sparklingaria"_s, MoveName::Sparkling_Aria},
			{"spectralthief"_s, MoveName::Spectral_Thief},
			{"speedswap"_s, MoveName::Speed_Swap},
			{"spicyextract"_s, MoveName::Spicy_Extract},
			{"spiderweb"_s, MoveName::Spider_Web},
			{"spikecannon"_s, MoveName::Spike_Cannon},
			{"spikes"_s, MoveName::Spikes},
			{"spikyshield"_s, MoveName::Spiky_Shield},
			{"spinout"_s, MoveName::Spin_Out},
			{"spiritbreak"_s, MoveName::Spirit_Break},
			{"spiritshackle"_s, MoveName::Spirit_Shackle},
			{"spite"_s, MoveName::Spite},
			{"spitup"_s, MoveName::Spit_Up},
			{"splash"_s, MoveName::Splash},
			{"splinteredstormshards"_s, MoveName::Splintered_Stormshards},
			{"spore"_s, MoveName::Spore},
			{"spotlight"_s, MoveName::Spotlight},
			{"stealthrock"_s, MoveName::Stealth_Rock},
			{"steameruption"_s, MoveName::Steam_Eruption},
			{"steamroller"_s, MoveName::Steamroller},
			{"steelbeam"_s, MoveName::Steel_Beam},
			{"steelroller"_s, MoveName::Steel_Roller},
			{"steelwing"_s, MoveName::Steel_Wing},
			{"stickyweb"_s, MoveName::Sticky_Web},
			{"stockpile"_s, MoveName::Stockpile},
			{"stokedsparksurfer"_s, MoveName::Stoked_Sparksurfer},
			{"stomp"_s, MoveName::Stomp},
			{"stompingtantrum"_s, MoveName::Stomping_Tantrum},
			{"stoneedge"_s, MoveName::Stone_Edge},
			{"storedpower"_s, MoveName::Stored_Power},
			{"stormthrow"_s, MoveName::Storm_Throw},
			{"strangesteam"_s, MoveName::Strange_Steam},
			{"strength"_s, MoveName::Strength},
			{"strengthsap"_s, MoveName::Strength_Sap},
			{"stringshot"_s, MoveName::String_Shot},
			{"struggle"_s, MoveName::Struggle},
			{"strugglebug"_s, MoveName::Struggle_Bug},
			{"stuffcheeks"_s, MoveName::Stuff_Cheeks},
			{"stunspore"_s, MoveName::Stun_Spore},
			{"submission"_s, MoveName::Submission},
			{"substitute"_s, MoveName::Substitute},
			{"subzeroslammer"_s, MoveName::Subzero_Slammer},
			{"suckerpunch"_s, MoveName::Sucker_Punch},
			{"sunnyday"_s, MoveName::Sunny_Day},
			{"sunsteelstrike"_s, MoveName::Sunsteel_Strike},
			{"superfang"_s, MoveName::Super_Fang},
			{"superpower"_s, MoveName::Superpower},
			{"supersonic"_s, MoveName::Supersonic},
			{"supersonicskystrike"_s, MoveName::Supersonic_Skystrike},
			{"surf"_s, MoveName::Surf},
			{"surgingstrikes"_s, MoveName::Surging_Strikes},
			{"swagger"_s, MoveName::Swagger},
			{"swallow"_s, MoveName::Swallow},
			{"sweetkiss"_s, MoveName::Sweet_Kiss},
			{"sweetscent"_s, MoveName::Sweet_Scent},
			{"swift"_s, MoveName::Swift},
			{"switcheroo"_s, MoveName::Switcheroo},
			{"swordsdance"_s, MoveName::Swords_Dance},
			{"synchronoise"_s, MoveName::Synchronoise},
			{"synthesis"_s, MoveName::Synthesis},
			{"syrupbomb"_s, MoveName::Syrup_Bomb},
			{"tachyoncutter"_s, MoveName::Tachyon_Cutter},
			{"tackle"_s, MoveName::Tackle},
			{"tailglow"_s, MoveName::Tail_Glow},
			{"tailslap"_s, MoveName::Tail_Slap},
			{"tailwhip"_s, MoveName::Tail_Whip},
			{"tailwind"_s, MoveName::Tailwind},
			{"takedown"_s, MoveName::Take_Down},
			{"tarshot"_s, MoveName::Tar_Shot},
			{"taunt"_s, MoveName::Taunt},
			{"tearfullook"_s, MoveName::Tearful_Look},
			{"teatime"_s, MoveName::Teatime},
			{"technoblast"_s, MoveName::Techno_Blast},
			{"tectonicrage"_s, MoveName::Tectonic_Rage},
			{"teeterdance"_s, MoveName::Teeter_Dance},
			{"telekinesis"_s, MoveName::Telekinesis},
			{"teleport"_s, MoveName::Teleport},
			{"terablast"_s, MoveName::Tera_Blast},
			{"terrainpulse"_s, MoveName::Terrain_Pulse},
			{"thief"_s, MoveName::Thief},
			{"thousandarrows"_s, MoveName::Thousand_Arrows},
			{"thousandwaves"_s, MoveName::Thousand_Waves},
			{"thrash"_s, MoveName::Thrash},
			{"throatchop"_s, MoveName::Throat_Chop},
			{"thunder"_s, MoveName::Thunder},
			{"thunderbolt"_s, MoveName::Thunderbolt},
			{"thundercage"_s, MoveName::Thunder_Cage},
			{"thunderclap"_s, MoveName::Thunderclap},
			{"thunderfang"_s, MoveName::Thunder_Fang},
			{"thunderouskick"_s, MoveName::Thunderous_Kick},
			{"thunderpunch"_s, MoveName::Thunder_Punch},
			{"thundershock"_s, MoveName::Thunder_Shock},
			{"thunderwave"_s, MoveName::Thunder_Wave},
			{"tickle"_s, MoveName::Tickle},
			{"tidyup"_s, MoveName::Tidy_Up},
			{"topsyturvy"_s, MoveName::Topsy_Turvy},
			{"torchsong"_s, MoveName::Torch_Song},
			{"torment"_s, MoveName::Torment},
			{"toxic"_s, MoveName::Toxic},
			{"toxicspikes"_s, MoveName::Toxic_Spikes},
			{"toxicthread"_s, MoveName::Toxic_Thread},
			{"trailblaze"_s, MoveName::Trailblaze},
			{"transform"_s, MoveName::Transform},
			{"triattack"_s, MoveName::Tri_Attack},
			{"trick"_s, MoveName::Trick},
			{"trickortreat"_s, MoveName::Trick_or_Treat},
			{"trickroom"_s, MoveName::Trick_Room},
			{"tripleaxel"_s, MoveName::Triple_Axel},
			{"tripledive"_s, MoveName::Triple_Dive},
			{"triplekick"_s, MoveName::Triple_Kick},
			{"tropkick"_s, MoveName::Trop_Kick},
			{"trumpcard"_s, MoveName::Trump_Card},
			{"twinbeam"_s, MoveName::Twin_Beam},
			{"twineedle"_s, MoveName::Twineedle},
			{"twinkletackle"_s, MoveName::Twinkle_Tackle},
			{"twister"_s, MoveName::Twister},
			{"upperhand"_s, MoveName::Upper_Hand},
			{"uproar"_s, MoveName::Uproar},
			{"uturn"_s, MoveName::U_turn},
			{"vacuumwave"_s, MoveName::Vacuum_Wave},
			{"vcreate"_s, MoveName::V_create},
			{"venomdrench"_s, MoveName::Venom_Drench},
			{"venoshock"_s, MoveName::Venoshock},
			{"vicegrip"_s, MoveName::Vise_Grip},
			{"vinewhip"_s, MoveName::Vine_Whip},
			{"visegrip"_s, MoveName::Vise_Grip},
			{"vitalthrow"_s, MoveName::Vital_Throw},
			{"voltswitch"_s, MoveName::Volt_Switch},
			{"volttackle"_s, MoveName::Volt_Tackle},
			{"wakeupslap"_s, MoveName::Wake_Up_Slap},
			{"waterfall"_s, MoveName::Waterfall},
			{"watergun"_s, MoveName::Water_Gun},
			{"waterpledge"_s, MoveName::Water_Pledge},
			{"waterpulse"_s, MoveName::Water_Pulse},
			{"watershuriken"_s, MoveName::Water_Shuriken},
			{"watersport"_s, MoveName::Water_Sport},
			{"waterspout"_s, MoveName::Water_Spout},
			{"weatherball"_s, MoveName::Weather_Ball},
			{"whirlpool"_s, MoveName::Whirlpool},
			{"whirlwind"_s, MoveName::Whirlwind},
			{"wickedblow"_s, MoveName::Wicked_Blow},
			{"wickedtorque"_s, MoveName::Wicked_Torque},
			{"wideguard"_s, MoveName::Wide_Guard},
			{"wildcharge"_s, MoveName::Wild_Charge},
			{"willowisp"_s, MoveName::Will_O_Wisp},
			{"wingattack"_s, MoveName::Wing_Attack},
			{"wish"_s, MoveName::Wish},
			{"withdraw"_s, MoveName::Withdraw},
			{"wonderroom"_s, MoveName::Wonder_Room},
			{"woodhammer"_s, MoveName::Wood_Hammer},
			{"workup"_s, MoveName::Work_Up},
			{"worryseed"_s, MoveName::Worry_Seed},
			{"wrap"_s, MoveName::Wrap},
			{"wringout"_s, MoveName::Wring_Out},
			{"xscissor"_s, MoveName::X_Scissor},
			{"yawn"_s, MoveName::Yawn},
			{"zapcannon"_s, MoveName::Zap_Cannon},
			{"zenheadbutt"_s, MoveName::Zen_Headbutt},
			{"zingzap"_s, MoveName::Zing_Zap},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<25_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Move"_s, str);
	}
	return *result;
}

} // namespace technicalmachine
