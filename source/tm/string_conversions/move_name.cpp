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
		case MoveName::Pound: return "Pound"_sv;
		case MoveName::Karate_Chop: return "Karate Chop"_sv;
		case MoveName::Double_Slap: return "Double Slap"_sv;
		case MoveName::Comet_Punch: return "Comet Punch"_sv;
		case MoveName::Mega_Punch: return "Mega Punch"_sv;
		case MoveName::Pay_Day: return "Pay Day"_sv;
		case MoveName::Fire_Punch: return "Fire Punch"_sv;
		case MoveName::Ice_Punch: return "Ice Punch"_sv;
		case MoveName::Thunder_Punch: return "Thunder Punch"_sv;
		case MoveName::Scratch: return "Scratch"_sv;
		case MoveName::Vise_Grip: return "Vise Grip"_sv;
		case MoveName::Guillotine: return "Guillotine"_sv;
		case MoveName::Razor_Wind: return "Razor Wind"_sv;
		case MoveName::Swords_Dance: return "Swords Dance"_sv;
		case MoveName::Cut: return "Cut"_sv;
		case MoveName::Gust: return "Gust"_sv;
		case MoveName::Wing_Attack: return "Wing Attack"_sv;
		case MoveName::Whirlwind: return "Whirlwind"_sv;
		case MoveName::Fly: return "Fly"_sv;
		case MoveName::Bind: return "Bind"_sv;
		case MoveName::Slam: return "Slam"_sv;
		case MoveName::Vine_Whip: return "Vine Whip"_sv;
		case MoveName::Stomp: return "Stomp"_sv;
		case MoveName::Double_Kick: return "Double Kick"_sv;
		case MoveName::Mega_Kick: return "Mega Kick"_sv;
		case MoveName::Jump_Kick: return "Jump Kick"_sv;
		case MoveName::Rolling_Kick: return "Rolling Kick"_sv;
		case MoveName::Sand_Attack: return "Sand Attack"_sv;
		case MoveName::Headbutt: return "Headbutt"_sv;
		case MoveName::Horn_Attack: return "Horn Attack"_sv;
		case MoveName::Fury_Attack: return "Fury Attack"_sv;
		case MoveName::Horn_Drill: return "Horn Drill"_sv;
		case MoveName::Tackle: return "Tackle"_sv;
		case MoveName::Body_Slam: return "Body Slam"_sv;
		case MoveName::Wrap: return "Wrap"_sv;
		case MoveName::Take_Down: return "Take Down"_sv;
		case MoveName::Thrash: return "Thrash"_sv;
		case MoveName::Double_Edge: return "Double-Edge"_sv;
		case MoveName::Tail_Whip: return "Tail Whip"_sv;
		case MoveName::Poison_Sting: return "Poison Sting"_sv;
		case MoveName::Twineedle: return "Twineedle"_sv;
		case MoveName::Pin_Missile: return "Pin Missile"_sv;
		case MoveName::Leer: return "Leer"_sv;
		case MoveName::Bite: return "Bite"_sv;
		case MoveName::Growl: return "Growl"_sv;
		case MoveName::Roar: return "Roar"_sv;
		case MoveName::Sing: return "Sing"_sv;
		case MoveName::Supersonic: return "Supersonic"_sv;
		case MoveName::Sonic_Boom: return "Sonic Boom"_sv;
		case MoveName::Disable: return "Disable"_sv;
		case MoveName::Acid: return "Acid"_sv;
		case MoveName::Ember: return "Ember"_sv;
		case MoveName::Flamethrower: return "Flamethrower"_sv;
		case MoveName::Mist: return "Mist"_sv;
		case MoveName::Water_Gun: return "Water Gun"_sv;
		case MoveName::Hydro_Pump: return "Hydro Pump"_sv;
		case MoveName::Surf: return "Surf"_sv;
		case MoveName::Ice_Beam: return "Ice Beam"_sv;
		case MoveName::Blizzard: return "Blizzard"_sv;
		case MoveName::Psybeam: return "Psybeam"_sv;
		case MoveName::Bubble_Beam: return "Bubble Beam"_sv;
		case MoveName::Aurora_Beam: return "Aurora Beam"_sv;
		case MoveName::Hyper_Beam: return "Hyper Beam"_sv;
		case MoveName::Peck: return "Peck"_sv;
		case MoveName::Drill_Peck: return "Drill Peck"_sv;
		case MoveName::Submission: return "Submission"_sv;
		case MoveName::Low_Kick: return "Low Kick"_sv;
		case MoveName::Counter: return "Counter"_sv;
		case MoveName::Seismic_Toss: return "Seismic Toss"_sv;
		case MoveName::Strength: return "Strength"_sv;
		case MoveName::Absorb: return "Absorb"_sv;
		case MoveName::Mega_Drain: return "Mega Drain"_sv;
		case MoveName::Leech_Seed: return "Leech Seed"_sv;
		case MoveName::Growth: return "Growth"_sv;
		case MoveName::Razor_Leaf: return "Razor Leaf"_sv;
		case MoveName::Solar_Beam: return "Solar Beam"_sv;
		case MoveName::Poison_Powder: return "Poison Powder"_sv;
		case MoveName::Stun_Spore: return "Stun Spore"_sv;
		case MoveName::Sleep_Powder: return "Sleep Powder"_sv;
		case MoveName::Petal_Dance: return "Petal Dance"_sv;
		case MoveName::String_Shot: return "String Shot"_sv;
		case MoveName::Dragon_Rage: return "Dragon Rage"_sv;
		case MoveName::Fire_Spin: return "Fire Spin"_sv;
		case MoveName::Thunder_Shock: return "Thunder Shock"_sv;
		case MoveName::Thunderbolt: return "Thunderbolt"_sv;
		case MoveName::Thunder_Wave: return "Thunder Wave"_sv;
		case MoveName::Thunder: return "Thunder"_sv;
		case MoveName::Rock_Throw: return "Rock Throw"_sv;
		case MoveName::Earthquake: return "Earthquake"_sv;
		case MoveName::Fissure: return "Fissure"_sv;
		case MoveName::Dig: return "Dig"_sv;
		case MoveName::Toxic: return "Toxic"_sv;
		case MoveName::Confusion: return "Confusion"_sv;
		case MoveName::Psychic: return "Psychic"_sv;
		case MoveName::Hypnosis: return "Hypnosis"_sv;
		case MoveName::Meditate: return "Meditate"_sv;
		case MoveName::Agility: return "Agility"_sv;
		case MoveName::Quick_Attack: return "Quick Attack"_sv;
		case MoveName::Rage: return "Rage"_sv;
		case MoveName::Teleport: return "Teleport"_sv;
		case MoveName::Night_Shade: return "Night Shade"_sv;
		case MoveName::Mimic: return "Mimic"_sv;
		case MoveName::Screech: return "Screech"_sv;
		case MoveName::Double_Team: return "Double Team"_sv;
		case MoveName::Recover: return "Recover"_sv;
		case MoveName::Harden: return "Harden"_sv;
		case MoveName::Minimize: return "Minimize"_sv;
		case MoveName::Smokescreen: return "Smokescreen"_sv;
		case MoveName::Confuse_Ray: return "Confuse Ray"_sv;
		case MoveName::Withdraw: return "Withdraw"_sv;
		case MoveName::Defense_Curl: return "Defense Curl"_sv;
		case MoveName::Barrier: return "Barrier"_sv;
		case MoveName::Light_Screen: return "Light Screen"_sv;
		case MoveName::Haze: return "Haze"_sv;
		case MoveName::Reflect: return "Reflect"_sv;
		case MoveName::Focus_Energy: return "Focus Energy"_sv;
		case MoveName::Bide: return "Bide"_sv;
		case MoveName::Metronome: return "Metronome"_sv;
		case MoveName::Mirror_Move: return "Mirror Move"_sv;
		case MoveName::Self_Destruct: return "Self-Destruct"_sv;
		case MoveName::Egg_Bomb: return "Egg Bomb"_sv;
		case MoveName::Lick: return "Lick"_sv;
		case MoveName::Smog: return "Smog"_sv;
		case MoveName::Sludge: return "Sludge"_sv;
		case MoveName::Bone_Club: return "Bone Club"_sv;
		case MoveName::Fire_Blast: return "Fire Blast"_sv;
		case MoveName::Waterfall: return "Waterfall"_sv;
		case MoveName::Clamp: return "Clamp"_sv;
		case MoveName::Swift: return "Swift"_sv;
		case MoveName::Skull_Bash: return "Skull Bash"_sv;
		case MoveName::Spike_Cannon: return "Spike Cannon"_sv;
		case MoveName::Constrict: return "Constrict"_sv;
		case MoveName::Amnesia: return "Amnesia"_sv;
		case MoveName::Kinesis: return "Kinesis"_sv;
		case MoveName::Soft_Boiled: return "Soft-Boiled"_sv;
		case MoveName::High_Jump_Kick: return "High Jump Kick"_sv;
		case MoveName::Glare: return "Glare"_sv;
		case MoveName::Dream_Eater: return "Dream Eater"_sv;
		case MoveName::Poison_Gas: return "Poison Gas"_sv;
		case MoveName::Barrage: return "Barrage"_sv;
		case MoveName::Leech_Life: return "Leech Life"_sv;
		case MoveName::Lovely_Kiss: return "Lovely Kiss"_sv;
		case MoveName::Sky_Attack: return "Sky Attack"_sv;
		case MoveName::Transform: return "Transform"_sv;
		case MoveName::Bubble: return "Bubble"_sv;
		case MoveName::Dizzy_Punch: return "Dizzy Punch"_sv;
		case MoveName::Spore: return "Spore"_sv;
		case MoveName::Flash: return "Flash"_sv;
		case MoveName::Psywave: return "Psywave"_sv;
		case MoveName::Splash: return "Splash"_sv;
		case MoveName::Acid_Armor: return "Acid Armor"_sv;
		case MoveName::Crabhammer: return "Crabhammer"_sv;
		case MoveName::Explosion: return "Explosion"_sv;
		case MoveName::Fury_Swipes: return "Fury Swipes"_sv;
		case MoveName::Bonemerang: return "Bonemerang"_sv;
		case MoveName::Rest: return "Rest"_sv;
		case MoveName::Rock_Slide: return "Rock Slide"_sv;
		case MoveName::Hyper_Fang: return "Hyper Fang"_sv;
		case MoveName::Sharpen: return "Sharpen"_sv;
		case MoveName::Conversion: return "Conversion"_sv;
		case MoveName::Tri_Attack: return "Tri Attack"_sv;
		case MoveName::Super_Fang: return "Super Fang"_sv;
		case MoveName::Slash: return "Slash"_sv;
		case MoveName::Substitute: return "Substitute"_sv;
		case MoveName::Struggle: return "Struggle"_sv;
		case MoveName::Sketch: return "Sketch"_sv;
		case MoveName::Triple_Kick: return "Triple Kick"_sv;
		case MoveName::Thief: return "Thief"_sv;
		case MoveName::Spider_Web: return "Spider Web"_sv;
		case MoveName::Mind_Reader: return "Mind Reader"_sv;
		case MoveName::Nightmare: return "Nightmare"_sv;
		case MoveName::Flame_Wheel: return "Flame Wheel"_sv;
		case MoveName::Snore: return "Snore"_sv;
		case MoveName::Curse: return "Curse"_sv;
		case MoveName::Flail: return "Flail"_sv;
		case MoveName::Conversion_2: return "Conversion 2"_sv;
		case MoveName::Aeroblast: return "Aeroblast"_sv;
		case MoveName::Cotton_Spore: return "Cotton Spore"_sv;
		case MoveName::Reversal: return "Reversal"_sv;
		case MoveName::Spite: return "Spite"_sv;
		case MoveName::Powder_Snow: return "Powder Snow"_sv;
		case MoveName::Protect: return "Protect"_sv;
		case MoveName::Mach_Punch: return "Mach Punch"_sv;
		case MoveName::Scary_Face: return "Scary Face"_sv;
		case MoveName::Feint_Attack: return "Feint Attack"_sv;
		case MoveName::Sweet_Kiss: return "Sweet Kiss"_sv;
		case MoveName::Belly_Drum: return "Belly Drum"_sv;
		case MoveName::Sludge_Bomb: return "Sludge Bomb"_sv;
		case MoveName::Mud_Slap: return "Mud-Slap"_sv;
		case MoveName::Octazooka: return "Octazooka"_sv;
		case MoveName::Spikes: return "Spikes"_sv;
		case MoveName::Zap_Cannon: return "Zap Cannon"_sv;
		case MoveName::Foresight: return "Foresight"_sv;
		case MoveName::Destiny_Bond: return "Destiny Bond"_sv;
		case MoveName::Perish_Song: return "Perish Song"_sv;
		case MoveName::Icy_Wind: return "Icy Wind"_sv;
		case MoveName::Detect: return "Detect"_sv;
		case MoveName::Bone_Rush: return "Bone Rush"_sv;
		case MoveName::Lock_On: return "Lock-On"_sv;
		case MoveName::Outrage: return "Outrage"_sv;
		case MoveName::Sandstorm: return "Sandstorm"_sv;
		case MoveName::Giga_Drain: return "Giga Drain"_sv;
		case MoveName::Endure: return "Endure"_sv;
		case MoveName::Charm: return "Charm"_sv;
		case MoveName::Rollout: return "Rollout"_sv;
		case MoveName::False_Swipe: return "False Swipe"_sv;
		case MoveName::Swagger: return "Swagger"_sv;
		case MoveName::Milk_Drink: return "Milk Drink"_sv;
		case MoveName::Spark: return "Spark"_sv;
		case MoveName::Fury_Cutter: return "Fury Cutter"_sv;
		case MoveName::Steel_Wing: return "Steel Wing"_sv;
		case MoveName::Mean_Look: return "Mean Look"_sv;
		case MoveName::Attract: return "Attract"_sv;
		case MoveName::Sleep_Talk: return "Sleep Talk"_sv;
		case MoveName::Heal_Bell: return "Heal Bell"_sv;
		case MoveName::Return: return "Return"_sv;
		case MoveName::Present: return "Present"_sv;
		case MoveName::Frustration: return "Frustration"_sv;
		case MoveName::Safeguard: return "Safeguard"_sv;
		case MoveName::Pain_Split: return "Pain Split"_sv;
		case MoveName::Sacred_Fire: return "Sacred Fire"_sv;
		case MoveName::Magnitude: return "Magnitude"_sv;
		case MoveName::Dynamic_Punch: return "Dynamic Punch"_sv;
		case MoveName::Megahorn: return "Megahorn"_sv;
		case MoveName::Dragon_Breath: return "Dragon Breath"_sv;
		case MoveName::Baton_Pass: return "Baton Pass"_sv;
		case MoveName::Encore: return "Encore"_sv;
		case MoveName::Pursuit: return "Pursuit"_sv;
		case MoveName::Rapid_Spin: return "Rapid Spin"_sv;
		case MoveName::Sweet_Scent: return "Sweet Scent"_sv;
		case MoveName::Iron_Tail: return "Iron Tail"_sv;
		case MoveName::Metal_Claw: return "Metal Claw"_sv;
		case MoveName::Vital_Throw: return "Vital Throw"_sv;
		case MoveName::Morning_Sun: return "Morning Sun"_sv;
		case MoveName::Synthesis: return "Synthesis"_sv;
		case MoveName::Moonlight: return "Moonlight"_sv;
		case MoveName::Hidden_Power: return "Hidden Power"_sv;
		case MoveName::Cross_Chop: return "Cross Chop"_sv;
		case MoveName::Twister: return "Twister"_sv;
		case MoveName::Rain_Dance: return "Rain Dance"_sv;
		case MoveName::Sunny_Day: return "Sunny Day"_sv;
		case MoveName::Crunch: return "Crunch"_sv;
		case MoveName::Mirror_Coat: return "Mirror Coat"_sv;
		case MoveName::Psych_Up: return "Psych Up"_sv;
		case MoveName::Extreme_Speed: return "Extreme Speed"_sv;
		case MoveName::Ancient_Power: return "Ancient Power"_sv;
		case MoveName::Shadow_Ball: return "Shadow Ball"_sv;
		case MoveName::Future_Sight: return "Future Sight"_sv;
		case MoveName::Rock_Smash: return "Rock Smash"_sv;
		case MoveName::Whirlpool: return "Whirlpool"_sv;
		case MoveName::Beat_Up: return "Beat Up"_sv;
		case MoveName::Fake_Out: return "Fake Out"_sv;
		case MoveName::Uproar: return "Uproar"_sv;
		case MoveName::Stockpile: return "Stockpile"_sv;
		case MoveName::Spit_Up: return "Spit Up"_sv;
		case MoveName::Swallow: return "Swallow"_sv;
		case MoveName::Heat_Wave: return "Heat Wave"_sv;
		case MoveName::Hail: return "Hail"_sv;
		case MoveName::Torment: return "Torment"_sv;
		case MoveName::Flatter: return "Flatter"_sv;
		case MoveName::Will_O_Wisp: return "Will-O-Wisp"_sv;
		case MoveName::Memento: return "Memento"_sv;
		case MoveName::Facade: return "Facade"_sv;
		case MoveName::Focus_Punch: return "Focus Punch"_sv;
		case MoveName::Smelling_Salts: return "Smelling Salts"_sv;
		case MoveName::Follow_Me: return "Follow Me"_sv;
		case MoveName::Nature_Power: return "Nature Power"_sv;
		case MoveName::Charge: return "Charge"_sv;
		case MoveName::Taunt: return "Taunt"_sv;
		case MoveName::Helping_Hand: return "Helping Hand"_sv;
		case MoveName::Trick: return "Trick"_sv;
		case MoveName::Role_Play: return "Role Play"_sv;
		case MoveName::Wish: return "Wish"_sv;
		case MoveName::Assist: return "Assist"_sv;
		case MoveName::Ingrain: return "Ingrain"_sv;
		case MoveName::Superpower: return "Superpower"_sv;
		case MoveName::Magic_Coat: return "Magic Coat"_sv;
		case MoveName::Recycle: return "Recycle"_sv;
		case MoveName::Revenge: return "Revenge"_sv;
		case MoveName::Brick_Break: return "Brick Break"_sv;
		case MoveName::Yawn: return "Yawn"_sv;
		case MoveName::Knock_Off: return "Knock Off"_sv;
		case MoveName::Endeavor: return "Endeavor"_sv;
		case MoveName::Eruption: return "Eruption"_sv;
		case MoveName::Skill_Swap: return "Skill Swap"_sv;
		case MoveName::Imprison: return "Imprison"_sv;
		case MoveName::Refresh: return "Refresh"_sv;
		case MoveName::Grudge: return "Grudge"_sv;
		case MoveName::Snatch: return "Snatch"_sv;
		case MoveName::Secret_Power: return "Secret Power"_sv;
		case MoveName::Dive: return "Dive"_sv;
		case MoveName::Arm_Thrust: return "Arm Thrust"_sv;
		case MoveName::Camouflage: return "Camouflage"_sv;
		case MoveName::Tail_Glow: return "Tail Glow"_sv;
		case MoveName::Luster_Purge: return "Luster Purge"_sv;
		case MoveName::Mist_Ball: return "Mist Ball"_sv;
		case MoveName::Feather_Dance: return "Feather Dance"_sv;
		case MoveName::Teeter_Dance: return "Teeter Dance"_sv;
		case MoveName::Blaze_Kick: return "Blaze Kick"_sv;
		case MoveName::Mud_Sport: return "Mud Sport"_sv;
		case MoveName::Ice_Ball: return "Ice Ball"_sv;
		case MoveName::Needle_Arm: return "Needle Arm"_sv;
		case MoveName::Slack_Off: return "Slack Off"_sv;
		case MoveName::Hyper_Voice: return "Hyper Voice"_sv;
		case MoveName::Poison_Fang: return "Poison Fang"_sv;
		case MoveName::Crush_Claw: return "Crush Claw"_sv;
		case MoveName::Blast_Burn: return "Blast Burn"_sv;
		case MoveName::Hydro_Cannon: return "Hydro Cannon"_sv;
		case MoveName::Meteor_Mash: return "Meteor Mash"_sv;
		case MoveName::Astonish: return "Astonish"_sv;
		case MoveName::Weather_Ball: return "Weather Ball"_sv;
		case MoveName::Aromatherapy: return "Aromatherapy"_sv;
		case MoveName::Fake_Tears: return "Fake Tears"_sv;
		case MoveName::Air_Cutter: return "Air Cutter"_sv;
		case MoveName::Overheat: return "Overheat"_sv;
		case MoveName::Odor_Sleuth: return "Odor Sleuth"_sv;
		case MoveName::Rock_Tomb: return "Rock Tomb"_sv;
		case MoveName::Silver_Wind: return "Silver Wind"_sv;
		case MoveName::Metal_Sound: return "Metal Sound"_sv;
		case MoveName::Grass_Whistle: return "Grass Whistle"_sv;
		case MoveName::Tickle: return "Tickle"_sv;
		case MoveName::Cosmic_Power: return "Cosmic Power"_sv;
		case MoveName::Water_Spout: return "Water Spout"_sv;
		case MoveName::Signal_Beam: return "Signal Beam"_sv;
		case MoveName::Shadow_Punch: return "Shadow Punch"_sv;
		case MoveName::Extrasensory: return "Extrasensory"_sv;
		case MoveName::Sky_Uppercut: return "Sky Uppercut"_sv;
		case MoveName::Sand_Tomb: return "Sand Tomb"_sv;
		case MoveName::Sheer_Cold: return "Sheer Cold"_sv;
		case MoveName::Muddy_Water: return "Muddy Water"_sv;
		case MoveName::Bullet_Seed: return "Bullet Seed"_sv;
		case MoveName::Aerial_Ace: return "Aerial Ace"_sv;
		case MoveName::Icicle_Spear: return "Icicle Spear"_sv;
		case MoveName::Iron_Defense: return "Iron Defense"_sv;
		case MoveName::Block: return "Block"_sv;
		case MoveName::Howl: return "Howl"_sv;
		case MoveName::Dragon_Claw: return "Dragon Claw"_sv;
		case MoveName::Frenzy_Plant: return "Frenzy Plant"_sv;
		case MoveName::Bulk_Up: return "Bulk Up"_sv;
		case MoveName::Bounce: return "Bounce"_sv;
		case MoveName::Mud_Shot: return "Mud Shot"_sv;
		case MoveName::Poison_Tail: return "Poison Tail"_sv;
		case MoveName::Covet: return "Covet"_sv;
		case MoveName::Volt_Tackle: return "Volt Tackle"_sv;
		case MoveName::Magical_Leaf: return "Magical Leaf"_sv;
		case MoveName::Water_Sport: return "Water Sport"_sv;
		case MoveName::Calm_Mind: return "Calm Mind"_sv;
		case MoveName::Leaf_Blade: return "Leaf Blade"_sv;
		case MoveName::Dragon_Dance: return "Dragon Dance"_sv;
		case MoveName::Rock_Blast: return "Rock Blast"_sv;
		case MoveName::Shock_Wave: return "Shock Wave"_sv;
		case MoveName::Water_Pulse: return "Water Pulse"_sv;
		case MoveName::Doom_Desire: return "Doom Desire"_sv;
		case MoveName::Psycho_Boost: return "Psycho Boost"_sv;
		case MoveName::Roost: return "Roost"_sv;
		case MoveName::Gravity: return "Gravity"_sv;
		case MoveName::Miracle_Eye: return "Miracle Eye"_sv;
		case MoveName::Wake_Up_Slap: return "Wake-Up Slap"_sv;
		case MoveName::Hammer_Arm: return "Hammer Arm"_sv;
		case MoveName::Gyro_Ball: return "Gyro Ball"_sv;
		case MoveName::Healing_Wish: return "Healing Wish"_sv;
		case MoveName::Brine: return "Brine"_sv;
		case MoveName::Natural_Gift: return "Natural Gift"_sv;
		case MoveName::Feint: return "Feint"_sv;
		case MoveName::Pluck: return "Pluck"_sv;
		case MoveName::Tailwind: return "Tailwind"_sv;
		case MoveName::Acupressure: return "Acupressure"_sv;
		case MoveName::Metal_Burst: return "Metal Burst"_sv;
		case MoveName::U_turn: return "U-turn"_sv;
		case MoveName::Close_Combat: return "Close Combat"_sv;
		case MoveName::Payback: return "Payback"_sv;
		case MoveName::Assurance: return "Assurance"_sv;
		case MoveName::Embargo: return "Embargo"_sv;
		case MoveName::Fling: return "Fling"_sv;
		case MoveName::Psycho_Shift: return "Psycho Shift"_sv;
		case MoveName::Trump_Card: return "Trump Card"_sv;
		case MoveName::Heal_Block: return "Heal Block"_sv;
		case MoveName::Wring_Out: return "Wring Out"_sv;
		case MoveName::Power_Trick: return "Power Trick"_sv;
		case MoveName::Gastro_Acid: return "Gastro Acid"_sv;
		case MoveName::Lucky_Chant: return "Lucky Chant"_sv;
		case MoveName::Me_First: return "Me First"_sv;
		case MoveName::Copycat: return "Copycat"_sv;
		case MoveName::Power_Swap: return "Power Swap"_sv;
		case MoveName::Guard_Swap: return "Guard Swap"_sv;
		case MoveName::Punishment: return "Punishment"_sv;
		case MoveName::Last_Resort: return "Last Resort"_sv;
		case MoveName::Worry_Seed: return "Worry Seed"_sv;
		case MoveName::Sucker_Punch: return "Sucker Punch"_sv;
		case MoveName::Toxic_Spikes: return "Toxic Spikes"_sv;
		case MoveName::Heart_Swap: return "Heart Swap"_sv;
		case MoveName::Aqua_Ring: return "Aqua Ring"_sv;
		case MoveName::Magnet_Rise: return "Magnet Rise"_sv;
		case MoveName::Flare_Blitz: return "Flare Blitz"_sv;
		case MoveName::Force_Palm: return "Force Palm"_sv;
		case MoveName::Aura_Sphere: return "Aura Sphere"_sv;
		case MoveName::Rock_Polish: return "Rock Polish"_sv;
		case MoveName::Poison_Jab: return "Poison Jab"_sv;
		case MoveName::Dark_Pulse: return "Dark Pulse"_sv;
		case MoveName::Night_Slash: return "Night Slash"_sv;
		case MoveName::Aqua_Tail: return "Aqua Tail"_sv;
		case MoveName::Seed_Bomb: return "Seed Bomb"_sv;
		case MoveName::Air_Slash: return "Air Slash"_sv;
		case MoveName::X_Scissor: return "X-Scissor"_sv;
		case MoveName::Bug_Buzz: return "Bug Buzz"_sv;
		case MoveName::Dragon_Pulse: return "Dragon Pulse"_sv;
		case MoveName::Dragon_Rush: return "Dragon Rush"_sv;
		case MoveName::Power_Gem: return "Power Gem"_sv;
		case MoveName::Drain_Punch: return "Drain Punch"_sv;
		case MoveName::Vacuum_Wave: return "Vacuum Wave"_sv;
		case MoveName::Focus_Blast: return "Focus Blast"_sv;
		case MoveName::Energy_Ball: return "Energy Ball"_sv;
		case MoveName::Brave_Bird: return "Brave Bird"_sv;
		case MoveName::Earth_Power: return "Earth Power"_sv;
		case MoveName::Switcheroo: return "Switcheroo"_sv;
		case MoveName::Giga_Impact: return "Giga Impact"_sv;
		case MoveName::Nasty_Plot: return "Nasty Plot"_sv;
		case MoveName::Bullet_Punch: return "Bullet Punch"_sv;
		case MoveName::Avalanche: return "Avalanche"_sv;
		case MoveName::Ice_Shard: return "Ice Shard"_sv;
		case MoveName::Shadow_Claw: return "Shadow Claw"_sv;
		case MoveName::Thunder_Fang: return "Thunder Fang"_sv;
		case MoveName::Ice_Fang: return "Ice Fang"_sv;
		case MoveName::Fire_Fang: return "Fire Fang"_sv;
		case MoveName::Shadow_Sneak: return "Shadow Sneak"_sv;
		case MoveName::Mud_Bomb: return "Mud Bomb"_sv;
		case MoveName::Psycho_Cut: return "Psycho Cut"_sv;
		case MoveName::Zen_Headbutt: return "Zen Headbutt"_sv;
		case MoveName::Mirror_Shot: return "Mirror Shot"_sv;
		case MoveName::Flash_Cannon: return "Flash Cannon"_sv;
		case MoveName::Rock_Climb: return "Rock Climb"_sv;
		case MoveName::Defog: return "Defog"_sv;
		case MoveName::Trick_Room: return "Trick Room"_sv;
		case MoveName::Draco_Meteor: return "Draco Meteor"_sv;
		case MoveName::Discharge: return "Discharge"_sv;
		case MoveName::Lava_Plume: return "Lava Plume"_sv;
		case MoveName::Leaf_Storm: return "Leaf Storm"_sv;
		case MoveName::Power_Whip: return "Power Whip"_sv;
		case MoveName::Rock_Wrecker: return "Rock Wrecker"_sv;
		case MoveName::Cross_Poison: return "Cross Poison"_sv;
		case MoveName::Gunk_Shot: return "Gunk Shot"_sv;
		case MoveName::Iron_Head: return "Iron Head"_sv;
		case MoveName::Magnet_Bomb: return "Magnet Bomb"_sv;
		case MoveName::Stone_Edge: return "Stone Edge"_sv;
		case MoveName::Captivate: return "Captivate"_sv;
		case MoveName::Stealth_Rock: return "Stealth Rock"_sv;
		case MoveName::Grass_Knot: return "Grass Knot"_sv;
		case MoveName::Chatter: return "Chatter"_sv;
		case MoveName::Judgment: return "Judgment"_sv;
		case MoveName::Bug_Bite: return "Bug Bite"_sv;
		case MoveName::Charge_Beam: return "Charge Beam"_sv;
		case MoveName::Wood_Hammer: return "Wood Hammer"_sv;
		case MoveName::Aqua_Jet: return "Aqua Jet"_sv;
		case MoveName::Attack_Order: return "Attack Order"_sv;
		case MoveName::Defend_Order: return "Defend Order"_sv;
		case MoveName::Heal_Order: return "Heal Order"_sv;
		case MoveName::Head_Smash: return "Head Smash"_sv;
		case MoveName::Double_Hit: return "Double Hit"_sv;
		case MoveName::Roar_of_Time: return "Roar of Time"_sv;
		case MoveName::Spacial_Rend: return "Spacial Rend"_sv;
		case MoveName::Lunar_Dance: return "Lunar Dance"_sv;
		case MoveName::Crush_Grip: return "Crush Grip"_sv;
		case MoveName::Magma_Storm: return "Magma Storm"_sv;
		case MoveName::Dark_Void: return "Dark Void"_sv;
		case MoveName::Seed_Flare: return "Seed Flare"_sv;
		case MoveName::Ominous_Wind: return "Ominous Wind"_sv;
		case MoveName::Shadow_Force: return "Shadow Force"_sv;
		case MoveName::Hone_Claws: return "Hone Claws"_sv;
		case MoveName::Wide_Guard: return "Wide Guard"_sv;
		case MoveName::Guard_Split: return "Guard Split"_sv;
		case MoveName::Power_Split: return "Power Split"_sv;
		case MoveName::Wonder_Room: return "Wonder Room"_sv;
		case MoveName::Psyshock: return "Psyshock"_sv;
		case MoveName::Venoshock: return "Venoshock"_sv;
		case MoveName::Autotomize: return "Autotomize"_sv;
		case MoveName::Rage_Powder: return "Rage Powder"_sv;
		case MoveName::Telekinesis: return "Telekinesis"_sv;
		case MoveName::Magic_Room: return "Magic Room"_sv;
		case MoveName::Smack_Down: return "Smack Down"_sv;
		case MoveName::Storm_Throw: return "Storm Throw"_sv;
		case MoveName::Flame_Burst: return "Flame Burst"_sv;
		case MoveName::Sludge_Wave: return "Sludge Wave"_sv;
		case MoveName::Quiver_Dance: return "Quiver Dance"_sv;
		case MoveName::Heavy_Slam: return "Heavy Slam"_sv;
		case MoveName::Synchronoise: return "Synchronoise"_sv;
		case MoveName::Electro_Ball: return "Electro Ball"_sv;
		case MoveName::Soak: return "Soak"_sv;
		case MoveName::Flame_Charge: return "Flame Charge"_sv;
		case MoveName::Coil: return "Coil"_sv;
		case MoveName::Low_Sweep: return "Low Sweep"_sv;
		case MoveName::Acid_Spray: return "Acid Spray"_sv;
		case MoveName::Foul_Play: return "Foul Play"_sv;
		case MoveName::Simple_Beam: return "Simple Beam"_sv;
		case MoveName::Entrainment: return "Entrainment"_sv;
		case MoveName::After_You: return "After You"_sv;
		case MoveName::Round: return "Round"_sv;
		case MoveName::Echoed_Voice: return "Echoed Voice"_sv;
		case MoveName::Chip_Away: return "Chip Away"_sv;
		case MoveName::Clear_Smog: return "Clear Smog"_sv;
		case MoveName::Stored_Power: return "Stored Power"_sv;
		case MoveName::Quick_Guard: return "Quick Guard"_sv;
		case MoveName::Ally_Switch: return "Ally Switch"_sv;
		case MoveName::Scald: return "Scald"_sv;
		case MoveName::Shell_Smash: return "Shell Smash"_sv;
		case MoveName::Heal_Pulse: return "Heal Pulse"_sv;
		case MoveName::Hex: return "Hex"_sv;
		case MoveName::Sky_Drop: return "Sky Drop"_sv;
		case MoveName::Shift_Gear: return "Shift Gear"_sv;
		case MoveName::Circle_Throw: return "Circle Throw"_sv;
		case MoveName::Incinerate: return "Incinerate"_sv;
		case MoveName::Quash: return "Quash"_sv;
		case MoveName::Acrobatics: return "Acrobatics"_sv;
		case MoveName::Reflect_Type: return "Reflect Type"_sv;
		case MoveName::Retaliate: return "Retaliate"_sv;
		case MoveName::Final_Gambit: return "Final Gambit"_sv;
		case MoveName::Bestow: return "Bestow"_sv;
		case MoveName::Inferno: return "Inferno"_sv;
		case MoveName::Water_Pledge: return "Water Pledge"_sv;
		case MoveName::Fire_Pledge: return "Fire Pledge"_sv;
		case MoveName::Grass_Pledge: return "Grass Pledge"_sv;
		case MoveName::Volt_Switch: return "Volt Switch"_sv;
		case MoveName::Struggle_Bug: return "Struggle Bug"_sv;
		case MoveName::Bulldoze: return "Bulldoze"_sv;
		case MoveName::Frost_Breath: return "Frost Breath"_sv;
		case MoveName::Dragon_Tail: return "Dragon Tail"_sv;
		case MoveName::Work_Up: return "Work Up"_sv;
		case MoveName::Electroweb: return "Electroweb"_sv;
		case MoveName::Wild_Charge: return "Wild Charge"_sv;
		case MoveName::Drill_Run: return "Drill Run"_sv;
		case MoveName::Dual_Chop: return "Dual Chop"_sv;
		case MoveName::Heart_Stamp: return "Heart Stamp"_sv;
		case MoveName::Horn_Leech: return "Horn Leech"_sv;
		case MoveName::Sacred_Sword: return "Sacred Sword"_sv;
		case MoveName::Razor_Shell: return "Razor Shell"_sv;
		case MoveName::Heat_Crash: return "Heat Crash"_sv;
		case MoveName::Leaf_Tornado: return "Leaf Tornado"_sv;
		case MoveName::Steamroller: return "Steamroller"_sv;
		case MoveName::Cotton_Guard: return "Cotton Guard"_sv;
		case MoveName::Night_Daze: return "Night Daze"_sv;
		case MoveName::Psystrike: return "Psystrike"_sv;
		case MoveName::Tail_Slap: return "Tail Slap"_sv;
		case MoveName::Hurricane: return "Hurricane"_sv;
		case MoveName::Head_Charge: return "Head Charge"_sv;
		case MoveName::Gear_Grind: return "Gear Grind"_sv;
		case MoveName::Searing_Shot: return "Searing Shot"_sv;
		case MoveName::Techno_Blast: return "Techno Blast"_sv;
		case MoveName::Relic_Song: return "Relic Song"_sv;
		case MoveName::Secret_Sword: return "Secret Sword"_sv;
		case MoveName::Glaciate: return "Glaciate"_sv;
		case MoveName::Bolt_Strike: return "Bolt Strike"_sv;
		case MoveName::Blue_Flare: return "Blue Flare"_sv;
		case MoveName::Fiery_Dance: return "Fiery Dance"_sv;
		case MoveName::Freeze_Shock: return "Freeze Shock"_sv;
		case MoveName::Ice_Burn: return "Ice Burn"_sv;
		case MoveName::Snarl: return "Snarl"_sv;
		case MoveName::Icicle_Crash: return "Icicle Crash"_sv;
		case MoveName::V_create: return "V-create"_sv;
		case MoveName::Fusion_Flare: return "Fusion Flare"_sv;
		case MoveName::Fusion_Bolt: return "Fusion Bolt"_sv;
		case MoveName::Flying_Press: return "Flying Press"_sv;
		case MoveName::Mat_Block: return "Mat Block"_sv;
		case MoveName::Belch: return "Belch"_sv;
		case MoveName::Rototiller: return "Rototiller"_sv;
		case MoveName::Sticky_Web: return "Sticky Web"_sv;
		case MoveName::Fell_Stinger: return "Fell Stinger"_sv;
		case MoveName::Phantom_Force: return "Phantom Force"_sv;
		case MoveName::Trick_or_Treat: return "Trick-or-Treat"_sv;
		case MoveName::Noble_Roar: return "Noble Roar"_sv;
		case MoveName::Ion_Deluge: return "Ion Deluge"_sv;
		case MoveName::Parabolic_Charge: return "Parabolic Charge"_sv;
		case MoveName::Forests_Curse: return "Forest's Curse"_sv;
		case MoveName::Petal_Blizzard: return "Petal Blizzard"_sv;
		case MoveName::Freeze_Dry: return "Freeze-Dry"_sv;
		case MoveName::Disarming_Voice: return "Disarming Voice"_sv;
		case MoveName::Parting_Shot: return "Parting Shot"_sv;
		case MoveName::Topsy_Turvy: return "Topsy-Turvy"_sv;
		case MoveName::Draining_Kiss: return "Draining Kiss"_sv;
		case MoveName::Crafty_Shield: return "Crafty Shield"_sv;
		case MoveName::Flower_Shield: return "Flower Shield"_sv;
		case MoveName::Grassy_Terrain: return "Grassy Terrain"_sv;
		case MoveName::Misty_Terrain: return "Misty Terrain"_sv;
		case MoveName::Electrify: return "Electrify"_sv;
		case MoveName::Play_Rough: return "Play Rough"_sv;
		case MoveName::Fairy_Wind: return "Fairy Wind"_sv;
		case MoveName::Moonblast: return "Moonblast"_sv;
		case MoveName::Boomburst: return "Boomburst"_sv;
		case MoveName::Fairy_Lock: return "Fairy Lock"_sv;
		case MoveName::Kings_Shield: return "King's Shield"_sv;
		case MoveName::Play_Nice: return "Play Nice"_sv;
		case MoveName::Confide: return "Confide"_sv;
		case MoveName::Diamond_Storm: return "Diamond Storm"_sv;
		case MoveName::Steam_Eruption: return "Steam Eruption"_sv;
		case MoveName::Hyperspace_Hole: return "Hyperspace Hole"_sv;
		case MoveName::Water_Shuriken: return "Water Shuriken"_sv;
		case MoveName::Mystical_Fire: return "Mystical Fire"_sv;
		case MoveName::Spiky_Shield: return "Spiky Shield"_sv;
		case MoveName::Aromatic_Mist: return "Aromatic Mist"_sv;
		case MoveName::Eerie_Impulse: return "Eerie Impulse"_sv;
		case MoveName::Venom_Drench: return "Venom Drench"_sv;
		case MoveName::Powder: return "Powder"_sv;
		case MoveName::Geomancy: return "Geomancy"_sv;
		case MoveName::Magnetic_Flux: return "Magnetic Flux"_sv;
		case MoveName::Happy_Hour: return "Happy Hour"_sv;
		case MoveName::Electric_Terrain: return "Electric Terrain"_sv;
		case MoveName::Dazzling_Gleam: return "Dazzling Gleam"_sv;
		case MoveName::Celebrate: return "Celebrate"_sv;
		case MoveName::Hold_Hands: return "Hold Hands"_sv;
		case MoveName::Baby_Doll_Eyes: return "Baby-Doll Eyes"_sv;
		case MoveName::Nuzzle: return "Nuzzle"_sv;
		case MoveName::Hold_Back: return "Hold Back"_sv;
		case MoveName::Infestation: return "Infestation"_sv;
		case MoveName::Power_Up_Punch: return "Power-Up Punch"_sv;
		case MoveName::Oblivion_Wing: return "Oblivion Wing"_sv;
		case MoveName::Thousand_Arrows: return "Thousand Arrows"_sv;
		case MoveName::Thousand_Waves: return "Thousand Waves"_sv;
		case MoveName::Lands_Wrath: return "Land's Wrath"_sv;
		case MoveName::Light_of_Ruin: return "Light of Ruin"_sv;
		case MoveName::Origin_Pulse: return "Origin Pulse"_sv;
		case MoveName::Precipice_Blades: return "Precipice Blades"_sv;
		case MoveName::Dragon_Ascent: return "Dragon Ascent"_sv;
		case MoveName::Hyperspace_Fury: return "Hyperspace Fury"_sv;
		case MoveName::Breakneck_Blitz: return "Breakneck Blitz"_sv;
		case MoveName::All_Out_Pummeling: return "All-Out Pummeling"_sv;
		case MoveName::Supersonic_Skystrike: return "Supersonic Skystrike"_sv;
		case MoveName::Acid_Downpour: return "Acid Downpour"_sv;
		case MoveName::Tectonic_Rage: return "Tectonic Rage"_sv;
		case MoveName::Continental_Crush: return "Continental Crush"_sv;
		case MoveName::Savage_Spin_Out: return "Savage Spin-Out"_sv;
		case MoveName::Never_Ending_Nightmare: return "Never-Ending Nightmare"_sv;
		case MoveName::Corkscrew_Crash: return "Corkscrew Crash"_sv;
		case MoveName::Inferno_Overdrive: return "Inferno Overdrive"_sv;
		case MoveName::Hydro_Vortex: return "Hydro Vortex"_sv;
		case MoveName::Bloom_Doom: return "Bloom Doom"_sv;
		case MoveName::Gigavolt_Havoc: return "Gigavolt Havoc"_sv;
		case MoveName::Shattered_Psyche: return "Shattered Psyche"_sv;
		case MoveName::Subzero_Slammer: return "Subzero Slammer"_sv;
		case MoveName::Devastating_Drake: return "Devastating Drake"_sv;
		case MoveName::Black_Hole_Eclipse: return "Black Hole Eclipse"_sv;
		case MoveName::Twinkle_Tackle: return "Twinkle Tackle"_sv;
		case MoveName::Catastropika: return "Catastropika"_sv;
		case MoveName::Shore_Up: return "Shore Up"_sv;
		case MoveName::First_Impression: return "First Impression"_sv;
		case MoveName::Baneful_Bunker: return "Baneful Bunker"_sv;
		case MoveName::Spirit_Shackle: return "Spirit Shackle"_sv;
		case MoveName::Darkest_Lariat: return "Darkest Lariat"_sv;
		case MoveName::Sparkling_Aria: return "Sparkling Aria"_sv;
		case MoveName::Ice_Hammer: return "Ice Hammer"_sv;
		case MoveName::Floral_Healing: return "Floral Healing"_sv;
		case MoveName::High_Horsepower: return "High Horsepower"_sv;
		case MoveName::Strength_Sap: return "Strength Sap"_sv;
		case MoveName::Solar_Blade: return "Solar Blade"_sv;
		case MoveName::Leafage: return "Leafage"_sv;
		case MoveName::Spotlight: return "Spotlight"_sv;
		case MoveName::Toxic_Thread: return "Toxic Thread"_sv;
		case MoveName::Laser_Focus: return "Laser Focus"_sv;
		case MoveName::Gear_Up: return "Gear Up"_sv;
		case MoveName::Throat_Chop: return "Throat Chop"_sv;
		case MoveName::Pollen_Puff: return "Pollen Puff"_sv;
		case MoveName::Anchor_Shot: return "Anchor Shot"_sv;
		case MoveName::Psychic_Terrain: return "Psychic Terrain"_sv;
		case MoveName::Lunge: return "Lunge"_sv;
		case MoveName::Fire_Lash: return "Fire Lash"_sv;
		case MoveName::Power_Trip: return "Power Trip"_sv;
		case MoveName::Burn_Up: return "Burn Up"_sv;
		case MoveName::Speed_Swap: return "Speed Swap"_sv;
		case MoveName::Smart_Strike: return "Smart Strike"_sv;
		case MoveName::Purify: return "Purify"_sv;
		case MoveName::Revelation_Dance: return "Revelation Dance"_sv;
		case MoveName::Core_Enforcer: return "Core Enforcer"_sv;
		case MoveName::Trop_Kick: return "Trop Kick"_sv;
		case MoveName::Instruct: return "Instruct"_sv;
		case MoveName::Beak_Blast: return "Beak Blast"_sv;
		case MoveName::Clanging_Scales: return "Clanging Scales"_sv;
		case MoveName::Dragon_Hammer: return "Dragon Hammer"_sv;
		case MoveName::Brutal_Swing: return "Brutal Swing"_sv;
		case MoveName::Aurora_Veil: return "Aurora Veil"_sv;
		case MoveName::Sinister_Arrow_Raid: return "Sinister Arrow Raid"_sv;
		case MoveName::Malicious_Moonsault: return "Malicious Moonsault"_sv;
		case MoveName::Oceanic_Operetta: return "Oceanic Operetta"_sv;
		case MoveName::Guardian_of_Alola: return "Guardian of Alola"_sv;
		case MoveName::Soul_Stealing_7_Star_Strike: return "Soul-Stealing 7-Star Strike"_sv;
		case MoveName::Stoked_Sparksurfer: return "Stoked Sparksurfer"_sv;
		case MoveName::Pulverizing_Pancake: return "Pulverizing Pancake"_sv;
		case MoveName::Extreme_Evoboost: return "Extreme Evoboost"_sv;
		case MoveName::Genesis_Supernova: return "Genesis Supernova"_sv;
		case MoveName::Shell_Trap: return "Shell Trap"_sv;
		case MoveName::Fleur_Cannon: return "Fleur Cannon"_sv;
		case MoveName::Psychic_Fangs: return "Psychic Fangs"_sv;
		case MoveName::Stomping_Tantrum: return "Stomping Tantrum"_sv;
		case MoveName::Shadow_Bone: return "Shadow Bone"_sv;
		case MoveName::Accelerock: return "Accelerock"_sv;
		case MoveName::Liquidation: return "Liquidation"_sv;
		case MoveName::Prismatic_Laser: return "Prismatic Laser"_sv;
		case MoveName::Spectral_Thief: return "Spectral Thief"_sv;
		case MoveName::Sunsteel_Strike: return "Sunsteel Strike"_sv;
		case MoveName::Moongeist_Beam: return "Moongeist Beam"_sv;
		case MoveName::Tearful_Look: return "Tearful Look"_sv;
		case MoveName::Zing_Zap: return "Zing Zap"_sv;
		case MoveName::Natures_Madness: return "Nature's Madness"_sv;
		case MoveName::Multi_Attack: return "Multi-Attack"_sv;
		case MoveName::m10000000_Volt_Thunderbolt: return "10,000,000 Volt Thunderbolt"_sv;
		case MoveName::Mind_Blown: return "Mind Blown"_sv;
		case MoveName::Plasma_Fists: return "Plasma Fists"_sv;
		case MoveName::Photon_Geyser: return "Photon Geyser"_sv;
		case MoveName::Light_That_Burns_the_Sky: return "Light That Burns the Sky"_sv;
		case MoveName::Searing_Sunraze_Smash: return "Searing Sunraze Smash"_sv;
		case MoveName::Menacing_Moonraze_Maelstrom: return "Menacing Moonraze Maelstrom"_sv;
		case MoveName::Lets_Snuggle_Forever: return "Let's Snuggle Forever"_sv;
		case MoveName::Splintered_Stormshards: return "Splintered Stormshards"_sv;
		case MoveName::Clangorous_Soulblaze: return "Clangorous Soulblaze"_sv;
		case MoveName::Double_Iron_Bash: return "Double Iron Bash"_sv;
		case MoveName::Max_Guard: return "Max Guard"_sv;
		case MoveName::Dynamax_Cannon: return "Dynamax Cannon"_sv;
		case MoveName::Snipe_Shot: return "Snipe Shot"_sv;
		case MoveName::Jaw_Lock: return "Jaw Lock"_sv;
		case MoveName::Stuff_Cheeks: return "Stuff Cheeks"_sv;
		case MoveName::No_Retreat: return "No Retreat"_sv;
		case MoveName::Tar_Shot: return "Tar Shot"_sv;
		case MoveName::Magic_Powder: return "Magic Powder"_sv;
		case MoveName::Dragon_Darts: return "Dragon Darts"_sv;
		case MoveName::Teatime: return "Teatime"_sv;
		case MoveName::Octolock: return "Octolock"_sv;
		case MoveName::Bolt_Beak: return "Bolt Beak"_sv;
		case MoveName::Fishious_Rend: return "Fishious Rend"_sv;
		case MoveName::Court_Change: return "Court Change"_sv;
		case MoveName::Max_Flare: return "Max Flare"_sv;
		case MoveName::Max_Flutterby: return "Max Flutterby"_sv;
		case MoveName::Max_Lightning: return "Max Lightning"_sv;
		case MoveName::Max_Strike: return "Max Strike"_sv;
		case MoveName::Max_Knuckle: return "Max Knuckle"_sv;
		case MoveName::Max_Phantasm: return "Max Phantasm"_sv;
		case MoveName::Max_Hailstorm: return "Max Hailstorm"_sv;
		case MoveName::Max_Ooze: return "Max Ooze"_sv;
		case MoveName::Max_Geyser: return "Max Geyser"_sv;
		case MoveName::Max_Airstream: return "Max Airstream"_sv;
		case MoveName::Max_Starfall: return "Max Starfall"_sv;
		case MoveName::Max_Wyrmwind: return "Max Wyrmwind"_sv;
		case MoveName::Max_Mindstorm: return "Max Mindstorm"_sv;
		case MoveName::Max_Rockfall: return "Max Rockfall"_sv;
		case MoveName::Max_Quake: return "Max Quake"_sv;
		case MoveName::Max_Darkness: return "Max Darkness"_sv;
		case MoveName::Max_Overgrowth: return "Max Overgrowth"_sv;
		case MoveName::Max_Steelspike: return "Max Steelspike"_sv;
		case MoveName::Clangorous_Soul: return "Clangorous Soul"_sv;
		case MoveName::Body_Press: return "Body Press"_sv;
		case MoveName::Decorate: return "Decorate"_sv;
		case MoveName::Drum_Beating: return "Drum Beating"_sv;
		case MoveName::Snap_Trap: return "Snap Trap"_sv;
		case MoveName::Pyro_Ball: return "Pyro Ball"_sv;
		case MoveName::Behemoth_Blade: return "Behemoth Blade"_sv;
		case MoveName::Behemoth_Bash: return "Behemoth Bash"_sv;
		case MoveName::Aura_Wheel: return "Aura Wheel"_sv;
		case MoveName::Breaking_Swipe: return "Breaking Swipe"_sv;
		case MoveName::Branch_Poke: return "Branch Poke"_sv;
		case MoveName::Overdrive: return "Overdrive"_sv;
		case MoveName::Apple_Acid: return "Apple Acid"_sv;
		case MoveName::Grav_Apple: return "Grav Apple"_sv;
		case MoveName::Spirit_Break: return "Spirit Break"_sv;
		case MoveName::Strange_Steam: return "Strange Steam"_sv;
		case MoveName::Life_Dew: return "Life Dew"_sv;
		case MoveName::Obstruct: return "Obstruct"_sv;
		case MoveName::False_Surrender: return "False Surrender"_sv;
		case MoveName::Meteor_Assault: return "Meteor Assault"_sv;
		case MoveName::Eternabeam: return "Eternabeam"_sv;
		case MoveName::Steel_Beam: return "Steel Beam"_sv;
		case MoveName::Expanding_Force: return "Expanding Force"_sv;
		case MoveName::Steel_Roller: return "Steel Roller"_sv;
		case MoveName::Scale_Shot: return "Scale Shot"_sv;
		case MoveName::Meteor_Beam: return "Meteor Beam"_sv;
		case MoveName::Shell_Side_Arm: return "Shell Side Arm"_sv;
		case MoveName::Misty_Explosion: return "Misty Explosion"_sv;
		case MoveName::Grassy_Glide: return "Grassy Glide"_sv;
		case MoveName::Rising_Voltage: return "Rising Voltage"_sv;
		case MoveName::Terrain_Pulse: return "Terrain Pulse"_sv;
		case MoveName::Skitter_Smack: return "Skitter Smack"_sv;
		case MoveName::Burning_Jealousy: return "Burning Jealousy"_sv;
		case MoveName::Lash_Out: return "Lash Out"_sv;
		case MoveName::Poltergeist: return "Poltergeist"_sv;
		case MoveName::Corrosive_Gas: return "Corrosive Gas"_sv;
		case MoveName::Coaching: return "Coaching"_sv;
		case MoveName::Flip_Turn: return "Flip Turn"_sv;
		case MoveName::Triple_Axel: return "Triple Axel"_sv;
		case MoveName::Dual_Wingbeat: return "Dual Wingbeat"_sv;
		case MoveName::Scorching_Sands: return "Scorching Sands"_sv;
		case MoveName::Jungle_Healing: return "Jungle Healing"_sv;
		case MoveName::Wicked_Blow: return "Wicked Blow"_sv;
		case MoveName::Surging_Strikes: return "Surging Strikes"_sv;
		case MoveName::Thunder_Cage: return "Thunder Cage"_sv;
		case MoveName::Dragon_Energy: return "Dragon Energy"_sv;
		case MoveName::Freezing_Glare: return "Freezing Glare"_sv;
		case MoveName::Fiery_Wrath: return "Fiery Wrath"_sv;
		case MoveName::Thunderous_Kick: return "Thunderous Kick"_sv;
		case MoveName::Glacial_Lance: return "Glacial Lance"_sv;
		case MoveName::Astral_Barrage: return "Astral Barrage"_sv;
		case MoveName::Eerie_Spell: return "Eerie Spell"_sv;
		case MoveName::Tera_Blast: return "Tera Blast"_sv;
		case MoveName::Silk_Trap: return "Silk Trap"_sv;
		case MoveName::Axe_Kick: return "Axe Kick"_sv;
		case MoveName::Last_Respects: return "Last Respects"_sv;
		case MoveName::Lumina_Crash: return "Lumina Crash"_sv;
		case MoveName::Order_Up: return "Order Up"_sv;
		case MoveName::Jet_Punch: return "Jet Punch"_sv;
		case MoveName::Spicy_Extract: return "Spicy Extract"_sv;
		case MoveName::Spin_Out: return "Spin Out"_sv;
		case MoveName::Population_Bomb: return "Population Bomb"_sv;
		case MoveName::Ice_Spinner: return "Ice Spinner"_sv;
		case MoveName::Glaive_Rush: return "Glaive Rush"_sv;
		case MoveName::Revival_Blessing: return "Revival Blessing"_sv;
		case MoveName::Salt_Cure: return "Salt Cure"_sv;
		case MoveName::Triple_Dive: return "Triple Dive"_sv;
		case MoveName::Mortal_Spin: return "Mortal Spin"_sv;
		case MoveName::Doodle: return "Doodle"_sv;
		case MoveName::Fillet_Away: return "Fillet Away"_sv;
		case MoveName::Kowtow_Cleave: return "Kowtow Cleave"_sv;
		case MoveName::Flower_Trick: return "Flower Trick"_sv;
		case MoveName::Torch_Song: return "Torch Song"_sv;
		case MoveName::Aqua_Step: return "Aqua Step"_sv;
		case MoveName::Raging_Bull: return "Raging Bull"_sv;
		case MoveName::Make_It_Rain: return "Make It Rain"_sv;
		case MoveName::Psyblade: return "Psyblade"_sv;
		case MoveName::Hydro_Steam: return "Hydro Steam"_sv;
		case MoveName::Ruination: return "Ruination"_sv;
		case MoveName::Collision_Course: return "Collision Course"_sv;
		case MoveName::Electro_Drift: return "Electro Drift"_sv;
		case MoveName::Shed_Tail: return "Shed Tail"_sv;
		case MoveName::Chilly_Reception: return "Chilly Reception"_sv;
		case MoveName::Tidy_Up: return "Tidy Up"_sv;
		case MoveName::Snowscape: return "Snowscape"_sv;
		case MoveName::Pounce: return "Pounce"_sv;
		case MoveName::Trailblaze: return "Trailblaze"_sv;
		case MoveName::Chilling_Water: return "Chilling Water"_sv;
		case MoveName::Hyper_Drill: return "Hyper Drill"_sv;
		case MoveName::Twin_Beam: return "Twin Beam"_sv;
		case MoveName::Rage_Fist: return "Rage Fist"_sv;
		case MoveName::Armor_Cannon: return "Armor Cannon"_sv;
		case MoveName::Bitter_Blade: return "Bitter Blade"_sv;
		case MoveName::Double_Shock: return "Double Shock"_sv;
		case MoveName::Gigaton_Hammer: return "Gigaton Hammer"_sv;
		case MoveName::Comeuppance: return "Comeuppance"_sv;
		case MoveName::Aqua_Cutter: return "Aqua Cutter"_sv;
		case MoveName::Blazing_Torque: return "Blazing Torque"_sv;
		case MoveName::Wicked_Torque: return "Wicked Torque"_sv;
		case MoveName::Noxious_Torque: return "Noxious Torque"_sv;
		case MoveName::Combat_Torque: return "Combat Torque"_sv;
		case MoveName::Magical_Torque: return "Magical Torque"_sv;
		case MoveName::Blood_Moon: return "Blood Moon"_sv;
		case MoveName::Matcha_Gotcha: return "Matcha Gotcha"_sv;
		case MoveName::Syrup_Bomb: return "Syrup Bomb"_sv;
		case MoveName::Ivy_Cudgel: return "Ivy Cudgel"_sv;
		case MoveName::Electro_Shot: return "Electro Shot"_sv;
		case MoveName::Thunderclap: return "Thunderclap"_sv;
		case MoveName::Tachyon_Cutter: return "Tachyon Cutter"_sv;
		case MoveName::Upper_Hand: return "Upper Hand"_sv;
		case MoveName::Psychic_Noise: return "Psychic Noise"_sv;
	}
}

export template<>
constexpr auto from_string(containers::string_view const str) -> MoveName {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, MoveName>>({
			{"10000000voltthunderbolt"_sv, MoveName::m10000000_Volt_Thunderbolt},
			{"absorb"_sv, MoveName::Absorb},
			{"accelerock"_sv, MoveName::Accelerock},
			{"acid"_sv, MoveName::Acid},
			{"acidarmor"_sv, MoveName::Acid_Armor},
			{"aciddownpour"_sv, MoveName::Acid_Downpour},
			{"acidspray"_sv, MoveName::Acid_Spray},
			{"acrobatics"_sv, MoveName::Acrobatics},
			{"acupressure"_sv, MoveName::Acupressure},
			{"aerialace"_sv, MoveName::Aerial_Ace},
			{"aeroblast"_sv, MoveName::Aeroblast},
			{"afteryou"_sv, MoveName::After_You},
			{"agility"_sv, MoveName::Agility},
			{"aircutter"_sv, MoveName::Air_Cutter},
			{"airslash"_sv, MoveName::Air_Slash},
			{"alloutpummeling"_sv, MoveName::All_Out_Pummeling},
			{"allyswitch"_sv, MoveName::Ally_Switch},
			{"amnesia"_sv, MoveName::Amnesia},
			{"anchorshot"_sv, MoveName::Anchor_Shot},
			{"ancientpower"_sv, MoveName::Ancient_Power},
			{"appleacid"_sv, MoveName::Apple_Acid},
			{"aquacutter"_sv, MoveName::Aqua_Cutter},
			{"aquajet"_sv, MoveName::Aqua_Jet},
			{"aquaring"_sv, MoveName::Aqua_Ring},
			{"aquastep"_sv, MoveName::Aqua_Step},
			{"aquatail"_sv, MoveName::Aqua_Tail},
			{"armorcannon"_sv, MoveName::Armor_Cannon},
			{"armthrust"_sv, MoveName::Arm_Thrust},
			{"aromatherapy"_sv, MoveName::Aromatherapy},
			{"aromaticmist"_sv, MoveName::Aromatic_Mist},
			{"assist"_sv, MoveName::Assist},
			{"assurance"_sv, MoveName::Assurance},
			{"astonish"_sv, MoveName::Astonish},
			{"astralbarrage"_sv, MoveName::Astral_Barrage},
			{"attackorder"_sv, MoveName::Attack_Order},
			{"attract"_sv, MoveName::Attract},
			{"aurasphere"_sv, MoveName::Aura_Sphere},
			{"aurawheel"_sv, MoveName::Aura_Wheel},
			{"aurorabeam"_sv, MoveName::Aurora_Beam},
			{"auroraveil"_sv, MoveName::Aurora_Veil},
			{"autotomize"_sv, MoveName::Autotomize},
			{"avalanche"_sv, MoveName::Avalanche},
			{"axekick"_sv, MoveName::Axe_Kick},
			{"babydolleyes"_sv, MoveName::Baby_Doll_Eyes},
			{"banefulbunker"_sv, MoveName::Baneful_Bunker},
			{"barrage"_sv, MoveName::Barrage},
			{"barrier"_sv, MoveName::Barrier},
			{"batonpass"_sv, MoveName::Baton_Pass},
			{"beakblast"_sv, MoveName::Beak_Blast},
			{"beatup"_sv, MoveName::Beat_Up},
			{"behemothbash"_sv, MoveName::Behemoth_Bash},
			{"behemothblade"_sv, MoveName::Behemoth_Blade},
			{"belch"_sv, MoveName::Belch},
			{"bellydrum"_sv, MoveName::Belly_Drum},
			{"bestow"_sv, MoveName::Bestow},
			{"bide"_sv, MoveName::Bide},
			{"bind"_sv, MoveName::Bind},
			{"bite"_sv, MoveName::Bite},
			{"bitterblade"_sv, MoveName::Bitter_Blade},
			{"blackholeeclipse"_sv, MoveName::Black_Hole_Eclipse},
			{"blastburn"_sv, MoveName::Blast_Burn},
			{"blazekick"_sv, MoveName::Blaze_Kick},
			{"blazingtorque"_sv, MoveName::Blazing_Torque},
			{"blizzard"_sv, MoveName::Blizzard},
			{"block"_sv, MoveName::Block},
			{"bloodmoon"_sv, MoveName::Blood_Moon},
			{"bloomdoom"_sv, MoveName::Bloom_Doom},
			{"blueflare"_sv, MoveName::Blue_Flare},
			{"bodypress"_sv, MoveName::Body_Press},
			{"bodyslam"_sv, MoveName::Body_Slam},
			{"boltbeak"_sv, MoveName::Bolt_Beak},
			{"boltstrike"_sv, MoveName::Bolt_Strike},
			{"boneclub"_sv, MoveName::Bone_Club},
			{"bonemerang"_sv, MoveName::Bonemerang},
			{"bonerush"_sv, MoveName::Bone_Rush},
			{"boomburst"_sv, MoveName::Boomburst},
			{"bounce"_sv, MoveName::Bounce},
			{"branchpoke"_sv, MoveName::Branch_Poke},
			{"bravebird"_sv, MoveName::Brave_Bird},
			{"breakingswipe"_sv, MoveName::Breaking_Swipe},
			{"breakneckblitz"_sv, MoveName::Breakneck_Blitz},
			{"brickbreak"_sv, MoveName::Brick_Break},
			{"brine"_sv, MoveName::Brine},
			{"brutalswing"_sv, MoveName::Brutal_Swing},
			{"bubble"_sv, MoveName::Bubble},
			{"bubblebeam"_sv, MoveName::Bubble_Beam},
			{"bugbite"_sv, MoveName::Bug_Bite},
			{"bugbuzz"_sv, MoveName::Bug_Buzz},
			{"bulkup"_sv, MoveName::Bulk_Up},
			{"bulldoze"_sv, MoveName::Bulldoze},
			{"bulletpunch"_sv, MoveName::Bullet_Punch},
			{"bulletseed"_sv, MoveName::Bullet_Seed},
			{"burningjealousy"_sv, MoveName::Burning_Jealousy},
			{"burnup"_sv, MoveName::Burn_Up},
			{"calmmind"_sv, MoveName::Calm_Mind},
			{"camouflage"_sv, MoveName::Camouflage},
			{"captivate"_sv, MoveName::Captivate},
			{"catastropika"_sv, MoveName::Catastropika},
			{"celebrate"_sv, MoveName::Celebrate},
			{"charge"_sv, MoveName::Charge},
			{"chargebeam"_sv, MoveName::Charge_Beam},
			{"charm"_sv, MoveName::Charm},
			{"chatter"_sv, MoveName::Chatter},
			{"chillingwater"_sv, MoveName::Chilling_Water},
			{"chillyreception"_sv, MoveName::Chilly_Reception},
			{"chipaway"_sv, MoveName::Chip_Away},
			{"circlethrow"_sv, MoveName::Circle_Throw},
			{"clamp"_sv, MoveName::Clamp},
			{"clangingscales"_sv, MoveName::Clanging_Scales},
			{"clangoroussoul"_sv, MoveName::Clangorous_Soul},
			{"clangoroussoulblaze"_sv, MoveName::Clangorous_Soulblaze},
			{"clearsmog"_sv, MoveName::Clear_Smog},
			{"closecombat"_sv, MoveName::Close_Combat},
			{"coaching"_sv, MoveName::Coaching},
			{"coil"_sv, MoveName::Coil},
			{"collisioncourse"_sv, MoveName::Collision_Course},
			{"combattorque"_sv, MoveName::Combat_Torque},
			{"cometpunch"_sv, MoveName::Comet_Punch},
			{"comeuppance"_sv, MoveName::Comeuppance},
			{"confide"_sv, MoveName::Confide},
			{"confuseray"_sv, MoveName::Confuse_Ray},
			{"confusion"_sv, MoveName::Confusion},
			{"constrict"_sv, MoveName::Constrict},
			{"continentalcrush"_sv, MoveName::Continental_Crush},
			{"conversion"_sv, MoveName::Conversion},
			{"conversion2"_sv, MoveName::Conversion_2},
			{"copycat"_sv, MoveName::Copycat},
			{"coreenforcer"_sv, MoveName::Core_Enforcer},
			{"corkscrewcrash"_sv, MoveName::Corkscrew_Crash},
			{"corrosivegas"_sv, MoveName::Corrosive_Gas},
			{"cosmicpower"_sv, MoveName::Cosmic_Power},
			{"cottonguard"_sv, MoveName::Cotton_Guard},
			{"cottonspore"_sv, MoveName::Cotton_Spore},
			{"counter"_sv, MoveName::Counter},
			{"courtchange"_sv, MoveName::Court_Change},
			{"covet"_sv, MoveName::Covet},
			{"crabhammer"_sv, MoveName::Crabhammer},
			{"craftyshield"_sv, MoveName::Crafty_Shield},
			{"crosschop"_sv, MoveName::Cross_Chop},
			{"crosspoison"_sv, MoveName::Cross_Poison},
			{"crunch"_sv, MoveName::Crunch},
			{"crushclaw"_sv, MoveName::Crush_Claw},
			{"crushgrip"_sv, MoveName::Crush_Grip},
			{"curse"_sv, MoveName::Curse},
			{"cut"_sv, MoveName::Cut},
			{"darkestlariat"_sv, MoveName::Darkest_Lariat},
			{"darkpulse"_sv, MoveName::Dark_Pulse},
			{"darkvoid"_sv, MoveName::Dark_Void},
			{"dazzlinggleam"_sv, MoveName::Dazzling_Gleam},
			{"decorate"_sv, MoveName::Decorate},
			{"defendorder"_sv, MoveName::Defend_Order},
			{"defensecurl"_sv, MoveName::Defense_Curl},
			{"defog"_sv, MoveName::Defog},
			{"destinybond"_sv, MoveName::Destiny_Bond},
			{"detect"_sv, MoveName::Detect},
			{"devastatingdrake"_sv, MoveName::Devastating_Drake},
			{"diamondstorm"_sv, MoveName::Diamond_Storm},
			{"dig"_sv, MoveName::Dig},
			{"disable"_sv, MoveName::Disable},
			{"disarmingvoice"_sv, MoveName::Disarming_Voice},
			{"discharge"_sv, MoveName::Discharge},
			{"dive"_sv, MoveName::Dive},
			{"dizzypunch"_sv, MoveName::Dizzy_Punch},
			{"doodle"_sv, MoveName::Doodle},
			{"doomdesire"_sv, MoveName::Doom_Desire},
			{"doubleedge"_sv, MoveName::Double_Edge},
			{"doublehit"_sv, MoveName::Double_Hit},
			{"doubleironbash"_sv, MoveName::Double_Iron_Bash},
			{"doublekick"_sv, MoveName::Double_Kick},
			{"doubleshock"_sv, MoveName::Double_Shock},
			{"doubleslap"_sv, MoveName::Double_Slap},
			{"doubleteam"_sv, MoveName::Double_Team},
			{"dracometeor"_sv, MoveName::Draco_Meteor},
			{"dragonascent"_sv, MoveName::Dragon_Ascent},
			{"dragonbreath"_sv, MoveName::Dragon_Breath},
			{"dragonclaw"_sv, MoveName::Dragon_Claw},
			{"dragondance"_sv, MoveName::Dragon_Dance},
			{"dragondarts"_sv, MoveName::Dragon_Darts},
			{"dragonenergy"_sv, MoveName::Dragon_Energy},
			{"dragonhammer"_sv, MoveName::Dragon_Hammer},
			{"dragonpulse"_sv, MoveName::Dragon_Pulse},
			{"dragonrage"_sv, MoveName::Dragon_Rage},
			{"dragonrush"_sv, MoveName::Dragon_Rush},
			{"dragontail"_sv, MoveName::Dragon_Tail},
			{"drainingkiss"_sv, MoveName::Draining_Kiss},
			{"drainpunch"_sv, MoveName::Drain_Punch},
			{"dreameater"_sv, MoveName::Dream_Eater},
			{"drillpeck"_sv, MoveName::Drill_Peck},
			{"drillrun"_sv, MoveName::Drill_Run},
			{"drumbeating"_sv, MoveName::Drum_Beating},
			{"dualchop"_sv, MoveName::Dual_Chop},
			{"dualwingbeat"_sv, MoveName::Dual_Wingbeat},
			{"dynamaxcannon"_sv, MoveName::Dynamax_Cannon},
			{"dynamicpunch"_sv, MoveName::Dynamic_Punch},
			{"earthpower"_sv, MoveName::Earth_Power},
			{"earthquake"_sv, MoveName::Earthquake},
			{"echoedvoice"_sv, MoveName::Echoed_Voice},
			{"eerieimpulse"_sv, MoveName::Eerie_Impulse},
			{"eeriespell"_sv, MoveName::Eerie_Spell},
			{"eggbomb"_sv, MoveName::Egg_Bomb},
			{"electricterrain"_sv, MoveName::Electric_Terrain},
			{"electrify"_sv, MoveName::Electrify},
			{"electroball"_sv, MoveName::Electro_Ball},
			{"electrodrift"_sv, MoveName::Electro_Drift},
			{"electroshot"_sv, MoveName::Electro_Shot},
			{"electroweb"_sv, MoveName::Electroweb},
			{"embargo"_sv, MoveName::Embargo},
			{"ember"_sv, MoveName::Ember},
			{"encore"_sv, MoveName::Encore},
			{"endeavor"_sv, MoveName::Endeavor},
			{"endure"_sv, MoveName::Endure},
			{"energyball"_sv, MoveName::Energy_Ball},
			{"entrainment"_sv, MoveName::Entrainment},
			{"eruption"_sv, MoveName::Eruption},
			{"eternabeam"_sv, MoveName::Eternabeam},
			{"expandingforce"_sv, MoveName::Expanding_Force},
			{"explosion"_sv, MoveName::Explosion},
			{"extrasensory"_sv, MoveName::Extrasensory},
			{"extremeevoboost"_sv, MoveName::Extreme_Evoboost},
			{"extremespeed"_sv, MoveName::Extreme_Speed},
			{"facade"_sv, MoveName::Facade},
			{"faintattack"_sv, MoveName::Feint_Attack},
			{"fairylock"_sv, MoveName::Fairy_Lock},
			{"fairywind"_sv, MoveName::Fairy_Wind},
			{"fakeout"_sv, MoveName::Fake_Out},
			{"faketears"_sv, MoveName::Fake_Tears},
			{"falsesurrender"_sv, MoveName::False_Surrender},
			{"falseswipe"_sv, MoveName::False_Swipe},
			{"featherdance"_sv, MoveName::Feather_Dance},
			{"feint"_sv, MoveName::Feint},
			{"feintattack"_sv, MoveName::Feint_Attack},
			{"fellstinger"_sv, MoveName::Fell_Stinger},
			{"fierydance"_sv, MoveName::Fiery_Dance},
			{"fierywrath"_sv, MoveName::Fiery_Wrath},
			{"filletaway"_sv, MoveName::Fillet_Away},
			{"finalgambit"_sv, MoveName::Final_Gambit},
			{"fireblast"_sv, MoveName::Fire_Blast},
			{"firefang"_sv, MoveName::Fire_Fang},
			{"firelash"_sv, MoveName::Fire_Lash},
			{"firepledge"_sv, MoveName::Fire_Pledge},
			{"firepunch"_sv, MoveName::Fire_Punch},
			{"firespin"_sv, MoveName::Fire_Spin},
			{"firstimpression"_sv, MoveName::First_Impression},
			{"fishiousrend"_sv, MoveName::Fishious_Rend},
			{"fissure"_sv, MoveName::Fissure},
			{"flail"_sv, MoveName::Flail},
			{"flameburst"_sv, MoveName::Flame_Burst},
			{"flamecharge"_sv, MoveName::Flame_Charge},
			{"flamethrower"_sv, MoveName::Flamethrower},
			{"flamewheel"_sv, MoveName::Flame_Wheel},
			{"flareblitz"_sv, MoveName::Flare_Blitz},
			{"flash"_sv, MoveName::Flash},
			{"flashcannon"_sv, MoveName::Flash_Cannon},
			{"flatter"_sv, MoveName::Flatter},
			{"fleurcannon"_sv, MoveName::Fleur_Cannon},
			{"fling"_sv, MoveName::Fling},
			{"flipturn"_sv, MoveName::Flip_Turn},
			{"floralhealing"_sv, MoveName::Floral_Healing},
			{"flowershield"_sv, MoveName::Flower_Shield},
			{"flowertrick"_sv, MoveName::Flower_Trick},
			{"fly"_sv, MoveName::Fly},
			{"flyingpress"_sv, MoveName::Flying_Press},
			{"focusblast"_sv, MoveName::Focus_Blast},
			{"focusenergy"_sv, MoveName::Focus_Energy},
			{"focuspunch"_sv, MoveName::Focus_Punch},
			{"followme"_sv, MoveName::Follow_Me},
			{"forcepalm"_sv, MoveName::Force_Palm},
			{"foresight"_sv, MoveName::Foresight},
			{"forestscurse"_sv, MoveName::Forests_Curse},
			{"foulplay"_sv, MoveName::Foul_Play},
			{"freezedry"_sv, MoveName::Freeze_Dry},
			{"freezeshock"_sv, MoveName::Freeze_Shock},
			{"freezingglare"_sv, MoveName::Freezing_Glare},
			{"frenzyplant"_sv, MoveName::Frenzy_Plant},
			{"frostbreath"_sv, MoveName::Frost_Breath},
			{"frustration"_sv, MoveName::Frustration},
			{"frustrationnull"_sv, MoveName::Frustration}, // Pokemon Showdown
			{"furyattack"_sv, MoveName::Fury_Attack},
			{"furycutter"_sv, MoveName::Fury_Cutter},
			{"furyswipes"_sv, MoveName::Fury_Swipes},
			{"fusionbolt"_sv, MoveName::Fusion_Bolt},
			{"fusionflare"_sv, MoveName::Fusion_Flare},
			{"futuresight"_sv, MoveName::Future_Sight},
			{"gastroacid"_sv, MoveName::Gastro_Acid},
			{"geargrind"_sv, MoveName::Gear_Grind},
			{"gearup"_sv, MoveName::Gear_Up},
			{"genesissupernova"_sv, MoveName::Genesis_Supernova},
			{"geomancy"_sv, MoveName::Geomancy},
			{"gigadrain"_sv, MoveName::Giga_Drain},
			{"gigaimpact"_sv, MoveName::Giga_Impact},
			{"gigatonhammer"_sv, MoveName::Gigaton_Hammer},
			{"gigavolthavoc"_sv, MoveName::Gigavolt_Havoc},
			{"glaciallance"_sv, MoveName::Glacial_Lance},
			{"glaciate"_sv, MoveName::Glaciate},
			{"glaiverush"_sv, MoveName::Glaive_Rush},
			{"glare"_sv, MoveName::Glare},
			{"grassknot"_sv, MoveName::Grass_Knot},
			{"grasspledge"_sv, MoveName::Grass_Pledge},
			{"grasswhistle"_sv, MoveName::Grass_Whistle},
			{"grassyglide"_sv, MoveName::Grassy_Glide},
			{"grassyterrain"_sv, MoveName::Grassy_Terrain},
			{"gravapple"_sv, MoveName::Grav_Apple},
			{"gravity"_sv, MoveName::Gravity},
			{"growl"_sv, MoveName::Growl},
			{"growth"_sv, MoveName::Growth},
			{"grudge"_sv, MoveName::Grudge},
			{"guardianofalola"_sv, MoveName::Guardian_of_Alola},
			{"guardsplit"_sv, MoveName::Guard_Split},
			{"guardswap"_sv, MoveName::Guard_Swap},
			{"guillotine"_sv, MoveName::Guillotine},
			{"gunkshot"_sv, MoveName::Gunk_Shot},
			{"gust"_sv, MoveName::Gust},
			{"gyroball"_sv, MoveName::Gyro_Ball},
			{"hail"_sv, MoveName::Hail},
			{"hammerarm"_sv, MoveName::Hammer_Arm},
			{"happyhour"_sv, MoveName::Happy_Hour},
			{"harden"_sv, MoveName::Harden},
			{"haze"_sv, MoveName::Haze},
			{"headbutt"_sv, MoveName::Headbutt},
			{"headcharge"_sv, MoveName::Head_Charge},
			{"headsmash"_sv, MoveName::Head_Smash},
			{"healbell"_sv, MoveName::Heal_Bell},
			{"healblock"_sv, MoveName::Heal_Block},
			{"healingwish"_sv, MoveName::Healing_Wish},
			{"healorder"_sv, MoveName::Heal_Order},
			{"healpulse"_sv, MoveName::Heal_Pulse},
			{"heartstamp"_sv, MoveName::Heart_Stamp},
			{"heartswap"_sv, MoveName::Heart_Swap},
			{"heatcrash"_sv, MoveName::Heat_Crash},
			{"heatwave"_sv, MoveName::Heat_Wave},
			{"heavyslam"_sv, MoveName::Heavy_Slam},
			{"helpinghand"_sv, MoveName::Helping_Hand},
			{"hex"_sv, MoveName::Hex},
			{"hiddenpower"_sv, MoveName::Hidden_Power},
			{"hiddenpowerbug"_sv, MoveName::Hidden_Power},
			{"hiddenpowerdark"_sv, MoveName::Hidden_Power},
			{"hiddenpowerdragon"_sv, MoveName::Hidden_Power},
			{"hiddenpowerelectric"_sv, MoveName::Hidden_Power},
			{"hiddenpowerfighting"_sv, MoveName::Hidden_Power},
			{"hiddenpowerfire"_sv, MoveName::Hidden_Power},
			{"hiddenpowerflying"_sv, MoveName::Hidden_Power},
			{"hiddenpowerghost"_sv, MoveName::Hidden_Power},
			{"hiddenpowergrass"_sv, MoveName::Hidden_Power},
			{"hiddenpowerground"_sv, MoveName::Hidden_Power},
			{"hiddenpowerice"_sv, MoveName::Hidden_Power},
			{"hiddenpowerpoison"_sv, MoveName::Hidden_Power},
			{"hiddenpowerpsychic"_sv, MoveName::Hidden_Power},
			{"hiddenpowerrock"_sv, MoveName::Hidden_Power},
			{"hiddenpowersteel"_sv, MoveName::Hidden_Power},
			{"hiddenpowerwater"_sv, MoveName::Hidden_Power},
			{"highhorsepower"_sv, MoveName::High_Horsepower},
			{"highjumpkick"_sv, MoveName::High_Jump_Kick},
			{"hijumpkick"_sv, MoveName::High_Jump_Kick},
			{"holdback"_sv, MoveName::Hold_Back},
			{"holdhands"_sv, MoveName::Hold_Hands},
			{"honeclaws"_sv, MoveName::Hone_Claws},
			{"hornattack"_sv, MoveName::Horn_Attack},
			{"horndrill"_sv, MoveName::Horn_Drill},
			{"hornleech"_sv, MoveName::Horn_Leech},
			{"howl"_sv, MoveName::Howl},
			{"hpbug"_sv, MoveName::Hidden_Power},
			{"hpdark"_sv, MoveName::Hidden_Power},
			{"hpdragon"_sv, MoveName::Hidden_Power},
			{"hpelectric"_sv, MoveName::Hidden_Power},
			{"hpfighting"_sv, MoveName::Hidden_Power},
			{"hpfire"_sv, MoveName::Hidden_Power},
			{"hpflying"_sv, MoveName::Hidden_Power},
			{"hpghost"_sv, MoveName::Hidden_Power},
			{"hpgrass"_sv, MoveName::Hidden_Power},
			{"hpground"_sv, MoveName::Hidden_Power},
			{"hpice"_sv, MoveName::Hidden_Power},
			{"hppoison"_sv, MoveName::Hidden_Power},
			{"hppsychic"_sv, MoveName::Hidden_Power},
			{"hprock"_sv, MoveName::Hidden_Power},
			{"hpsteel"_sv, MoveName::Hidden_Power},
			{"hpwater"_sv, MoveName::Hidden_Power},
			{"hurricane"_sv, MoveName::Hurricane},
			{"hydrocannon"_sv, MoveName::Hydro_Cannon},
			{"hydropump"_sv, MoveName::Hydro_Pump},
			{"hydrosteam"_sv, MoveName::Hydro_Steam},
			{"hydrovortex"_sv, MoveName::Hydro_Vortex},
			{"hyperbeam"_sv, MoveName::Hyper_Beam},
			{"hyperdrill"_sv, MoveName::Hyper_Drill},
			{"hyperfang"_sv, MoveName::Hyper_Fang},
			{"hyperspacefury"_sv, MoveName::Hyperspace_Fury},
			{"hyperspacehole"_sv, MoveName::Hyperspace_Hole},
			{"hypervoice"_sv, MoveName::Hyper_Voice},
			{"hypnosis"_sv, MoveName::Hypnosis},
			{"iceball"_sv, MoveName::Ice_Ball},
			{"icebeam"_sv, MoveName::Ice_Beam},
			{"iceburn"_sv, MoveName::Ice_Burn},
			{"icefang"_sv, MoveName::Ice_Fang},
			{"icehammer"_sv, MoveName::Ice_Hammer},
			{"icepunch"_sv, MoveName::Ice_Punch},
			{"iceshard"_sv, MoveName::Ice_Shard},
			{"icespinner"_sv, MoveName::Ice_Spinner},
			{"iciclecrash"_sv, MoveName::Icicle_Crash},
			{"iciclespear"_sv, MoveName::Icicle_Spear},
			{"icywind"_sv, MoveName::Icy_Wind},
			{"imprison"_sv, MoveName::Imprison},
			{"incinerate"_sv, MoveName::Incinerate},
			{"inferno"_sv, MoveName::Inferno},
			{"infernooverdrive"_sv, MoveName::Inferno_Overdrive},
			{"infestation"_sv, MoveName::Infestation},
			{"ingrain"_sv, MoveName::Ingrain},
			{"instruct"_sv, MoveName::Instruct},
			{"iondeluge"_sv, MoveName::Ion_Deluge},
			{"irondefense"_sv, MoveName::Iron_Defense},
			{"ironhead"_sv, MoveName::Iron_Head},
			{"irontail"_sv, MoveName::Iron_Tail},
			{"ivycudgel"_sv, MoveName::Ivy_Cudgel},
			{"jawlock"_sv, MoveName::Jaw_Lock},
			{"jetpunch"_sv, MoveName::Jet_Punch},
			{"judgment"_sv, MoveName::Judgment},
			{"jumpkick"_sv, MoveName::Jump_Kick},
			{"junglehealing"_sv, MoveName::Jungle_Healing},
			{"karatechop"_sv, MoveName::Karate_Chop},
			{"kinesis"_sv, MoveName::Kinesis},
			{"kingsshield"_sv, MoveName::Kings_Shield},
			{"knockoff"_sv, MoveName::Knock_Off},
			{"kowtowcleave"_sv, MoveName::Kowtow_Cleave},
			{"landswrath"_sv, MoveName::Lands_Wrath},
			{"laserfocus"_sv, MoveName::Laser_Focus},
			{"lashout"_sv, MoveName::Lash_Out},
			{"lastresort"_sv, MoveName::Last_Resort},
			{"lastrespects"_sv, MoveName::Last_Respects},
			{"lavaplume"_sv, MoveName::Lava_Plume},
			{"leafage"_sv, MoveName::Leafage},
			{"leafblade"_sv, MoveName::Leaf_Blade},
			{"leafstorm"_sv, MoveName::Leaf_Storm},
			{"leaftornado"_sv, MoveName::Leaf_Tornado},
			{"leechlife"_sv, MoveName::Leech_Life},
			{"leechseed"_sv, MoveName::Leech_Seed},
			{"leer"_sv, MoveName::Leer},
			{"letssnuggleforever"_sv, MoveName::Lets_Snuggle_Forever},
			{"lick"_sv, MoveName::Lick},
			{"lifedew"_sv, MoveName::Life_Dew},
			{"lightofruin"_sv, MoveName::Light_of_Ruin},
			{"lightscreen"_sv, MoveName::Light_Screen},
			{"lightthatburnsthesky"_sv, MoveName::Light_That_Burns_the_Sky},
			{"liquidation"_sv, MoveName::Liquidation},
			{"lockon"_sv, MoveName::Lock_On},
			{"lovelykiss"_sv, MoveName::Lovely_Kiss},
			{"lowkick"_sv, MoveName::Low_Kick},
			{"lowsweep"_sv, MoveName::Low_Sweep},
			{"luckychant"_sv, MoveName::Lucky_Chant},
			{"luminacrash"_sv, MoveName::Lumina_Crash},
			{"lunardance"_sv, MoveName::Lunar_Dance},
			{"lunge"_sv, MoveName::Lunge},
			{"lusterpurge"_sv, MoveName::Luster_Purge},
			{"machpunch"_sv, MoveName::Mach_Punch},
			{"magicalleaf"_sv, MoveName::Magical_Leaf},
			{"magicaltorque"_sv, MoveName::Magical_Torque},
			{"magiccoat"_sv, MoveName::Magic_Coat},
			{"magicpowder"_sv, MoveName::Magic_Powder},
			{"magicroom"_sv, MoveName::Magic_Room},
			{"magmastorm"_sv, MoveName::Magma_Storm},
			{"magnetbomb"_sv, MoveName::Magnet_Bomb},
			{"magneticflux"_sv, MoveName::Magnetic_Flux},
			{"magnetrise"_sv, MoveName::Magnet_Rise},
			{"magnitude"_sv, MoveName::Magnitude},
			{"makeitrain"_sv, MoveName::Make_It_Rain},
			{"maliciousmoonsault"_sv, MoveName::Malicious_Moonsault},
			{"matblock"_sv, MoveName::Mat_Block},
			{"matchagotcha"_sv, MoveName::Matcha_Gotcha},
			{"maxairstream"_sv, MoveName::Max_Airstream},
			{"maxdarkness"_sv, MoveName::Max_Darkness},
			{"maxflare"_sv, MoveName::Max_Flare},
			{"maxflutterby"_sv, MoveName::Max_Flutterby},
			{"maxgeyser"_sv, MoveName::Max_Geyser},
			{"maxguard"_sv, MoveName::Max_Guard},
			{"maxhailstorm"_sv, MoveName::Max_Hailstorm},
			{"maxknuckle"_sv, MoveName::Max_Knuckle},
			{"maxlightning"_sv, MoveName::Max_Lightning},
			{"maxmindstorm"_sv, MoveName::Max_Mindstorm},
			{"maxooze"_sv, MoveName::Max_Ooze},
			{"maxovergrowth"_sv, MoveName::Max_Overgrowth},
			{"maxphantasm"_sv, MoveName::Max_Phantasm},
			{"maxquake"_sv, MoveName::Max_Quake},
			{"maxrockfall"_sv, MoveName::Max_Rockfall},
			{"maxstarfall"_sv, MoveName::Max_Starfall},
			{"maxsteelspike"_sv, MoveName::Max_Steelspike},
			{"maxstrike"_sv, MoveName::Max_Strike},
			{"maxwyrmwind"_sv, MoveName::Max_Wyrmwind},
			{"meanlook"_sv, MoveName::Mean_Look},
			{"meditate"_sv, MoveName::Meditate},
			{"mefirst"_sv, MoveName::Me_First},
			{"megadrain"_sv, MoveName::Mega_Drain},
			{"megahorn"_sv, MoveName::Megahorn},
			{"megakick"_sv, MoveName::Mega_Kick},
			{"megapunch"_sv, MoveName::Mega_Punch},
			{"memento"_sv, MoveName::Memento},
			{"menacingmoonrazemaelstrom"_sv, MoveName::Menacing_Moonraze_Maelstrom},
			{"metalburst"_sv, MoveName::Metal_Burst},
			{"metalclaw"_sv, MoveName::Metal_Claw},
			{"metalsound"_sv, MoveName::Metal_Sound},
			{"meteorassault"_sv, MoveName::Meteor_Assault},
			{"meteorbeam"_sv, MoveName::Meteor_Beam},
			{"meteormash"_sv, MoveName::Meteor_Mash},
			{"metronome"_sv, MoveName::Metronome},
			{"milkdrink"_sv, MoveName::Milk_Drink},
			{"mimic"_sv, MoveName::Mimic},
			{"mindblown"_sv, MoveName::Mind_Blown},
			{"mindreader"_sv, MoveName::Mind_Reader},
			{"minimize"_sv, MoveName::Minimize},
			{"miracleeye"_sv, MoveName::Miracle_Eye},
			{"mirrorcoat"_sv, MoveName::Mirror_Coat},
			{"mirrormove"_sv, MoveName::Mirror_Move},
			{"mirrorshot"_sv, MoveName::Mirror_Shot},
			{"mist"_sv, MoveName::Mist},
			{"mistball"_sv, MoveName::Mist_Ball},
			{"mistyexplosion"_sv, MoveName::Misty_Explosion},
			{"mistyterrain"_sv, MoveName::Misty_Terrain},
			{"moonblast"_sv, MoveName::Moonblast},
			{"moongeistbeam"_sv, MoveName::Moongeist_Beam},
			{"moonlight"_sv, MoveName::Moonlight},
			{"morningsun"_sv, MoveName::Morning_Sun},
			{"mortalspin"_sv, MoveName::Mortal_Spin},
			{"mudbomb"_sv, MoveName::Mud_Bomb},
			{"muddywater"_sv, MoveName::Muddy_Water},
			{"mudshot"_sv, MoveName::Mud_Shot},
			{"mudslap"_sv, MoveName::Mud_Slap},
			{"mudsport"_sv, MoveName::Mud_Sport},
			{"multiattack"_sv, MoveName::Multi_Attack},
			{"mysticalfire"_sv, MoveName::Mystical_Fire},
			{"nastyplot"_sv, MoveName::Nasty_Plot},
			{"naturalgift"_sv, MoveName::Natural_Gift},
			{"naturepower"_sv, MoveName::Nature_Power},
			{"naturesmadness"_sv, MoveName::Natures_Madness},
			{"needlearm"_sv, MoveName::Needle_Arm},
			{"neverendingnightmare"_sv, MoveName::Never_Ending_Nightmare},
			{"nightdaze"_sv, MoveName::Night_Daze},
			{"nightmare"_sv, MoveName::Nightmare},
			{"nightshade"_sv, MoveName::Night_Shade},
			{"nightslash"_sv, MoveName::Night_Slash},
			{"nobleroar"_sv, MoveName::Noble_Roar},
			{"noretreat"_sv, MoveName::No_Retreat},
			{"noxioustorque"_sv, MoveName::Noxious_Torque},
			{"nuzzle"_sv, MoveName::Nuzzle},
			{"oblivionwing"_sv, MoveName::Oblivion_Wing},
			{"obstruct"_sv, MoveName::Obstruct},
			{"oceanicoperetta"_sv, MoveName::Oceanic_Operetta},
			{"octazooka"_sv, MoveName::Octazooka},
			{"octolock"_sv, MoveName::Octolock},
			{"odorsleuth"_sv, MoveName::Odor_Sleuth},
			{"ominouswind"_sv, MoveName::Ominous_Wind},
			{"orderup"_sv, MoveName::Order_Up},
			{"originpulse"_sv, MoveName::Origin_Pulse},
			{"outrage"_sv, MoveName::Outrage},
			{"overdrive"_sv, MoveName::Overdrive},
			{"overheat"_sv, MoveName::Overheat},
			{"painsplit"_sv, MoveName::Pain_Split},
			{"paraboliccharge"_sv, MoveName::Parabolic_Charge},
			{"partingshot"_sv, MoveName::Parting_Shot},
			{"payback"_sv, MoveName::Payback},
			{"payday"_sv, MoveName::Pay_Day},
			{"peck"_sv, MoveName::Peck},
			{"perishsong"_sv, MoveName::Perish_Song},
			{"petalblizzard"_sv, MoveName::Petal_Blizzard},
			{"petaldance"_sv, MoveName::Petal_Dance},
			{"phantomforce"_sv, MoveName::Phantom_Force},
			{"photongeyser"_sv, MoveName::Photon_Geyser},
			{"pinmissile"_sv, MoveName::Pin_Missile},
			{"plasmafists"_sv, MoveName::Plasma_Fists},
			{"playnice"_sv, MoveName::Play_Nice},
			{"playrough"_sv, MoveName::Play_Rough},
			{"pluck"_sv, MoveName::Pluck},
			{"poisonfang"_sv, MoveName::Poison_Fang},
			{"poisongas"_sv, MoveName::Poison_Gas},
			{"poisonjab"_sv, MoveName::Poison_Jab},
			{"poisonpowder"_sv, MoveName::Poison_Powder},
			{"poisonsting"_sv, MoveName::Poison_Sting},
			{"poisontail"_sv, MoveName::Poison_Tail},
			{"pollenpuff"_sv, MoveName::Pollen_Puff},
			{"poltergeist"_sv, MoveName::Poltergeist},
			{"populationbomb"_sv, MoveName::Population_Bomb},
			{"pounce"_sv, MoveName::Pounce},
			{"pound"_sv, MoveName::Pound},
			{"powder"_sv, MoveName::Powder},
			{"powdersnow"_sv, MoveName::Powder_Snow},
			{"powergem"_sv, MoveName::Power_Gem},
			{"powersplit"_sv, MoveName::Power_Split},
			{"powerswap"_sv, MoveName::Power_Swap},
			{"powertrick"_sv, MoveName::Power_Trick},
			{"powertrip"_sv, MoveName::Power_Trip},
			{"poweruppunch"_sv, MoveName::Power_Up_Punch},
			{"powerwhip"_sv, MoveName::Power_Whip},
			{"precipiceblades"_sv, MoveName::Precipice_Blades},
			{"present"_sv, MoveName::Present},
			{"prismaticlaser"_sv, MoveName::Prismatic_Laser},
			{"protect"_sv, MoveName::Protect},
			{"psybeam"_sv, MoveName::Psybeam},
			{"psyblade"_sv, MoveName::Psyblade},
			{"psychic"_sv, MoveName::Psychic},
			{"psychicfangs"_sv, MoveName::Psychic_Fangs},
			{"psychicnoise"_sv, MoveName::Psychic_Noise},
			{"psychicterrain"_sv, MoveName::Psychic_Terrain},
			{"psychoboost"_sv, MoveName::Psycho_Boost},
			{"psychocut"_sv, MoveName::Psycho_Cut},
			{"psychoshift"_sv, MoveName::Psycho_Shift},
			{"psychup"_sv, MoveName::Psych_Up},
			{"psyshock"_sv, MoveName::Psyshock},
			{"psystrike"_sv, MoveName::Psystrike},
			{"psywave"_sv, MoveName::Psywave},
			{"pulverizingpancake"_sv, MoveName::Pulverizing_Pancake},
			{"punishment"_sv, MoveName::Punishment},
			{"purify"_sv, MoveName::Purify},
			{"pursuit"_sv, MoveName::Pursuit},
			{"pyroball"_sv, MoveName::Pyro_Ball},
			{"quash"_sv, MoveName::Quash},
			{"quickattack"_sv, MoveName::Quick_Attack},
			{"quickguard"_sv, MoveName::Quick_Guard},
			{"quiverdance"_sv, MoveName::Quiver_Dance},
			{"rage"_sv, MoveName::Rage},
			{"ragefist"_sv, MoveName::Rage_Fist},
			{"ragepowder"_sv, MoveName::Rage_Powder},
			{"ragingbull"_sv, MoveName::Raging_Bull},
			{"raindance"_sv, MoveName::Rain_Dance},
			{"rapidspin"_sv, MoveName::Rapid_Spin},
			{"razorleaf"_sv, MoveName::Razor_Leaf},
			{"razorshell"_sv, MoveName::Razor_Shell},
			{"razorwind"_sv, MoveName::Razor_Wind},
			{"recharge"_sv, MoveName::Hyper_Beam}, // TODO: fix
			{"recover"_sv, MoveName::Recover},
			{"recycle"_sv, MoveName::Recycle},
			{"reflect"_sv, MoveName::Reflect},
			{"reflecttype"_sv, MoveName::Reflect_Type},
			{"refresh"_sv, MoveName::Refresh},
			{"relicsong"_sv, MoveName::Relic_Song},
			{"rest"_sv, MoveName::Rest},
			{"retaliate"_sv, MoveName::Retaliate},
			{"return"_sv, MoveName::Return},
			{"return102"_sv, MoveName::Return},
			{"revelationdance"_sv, MoveName::Revelation_Dance},
			{"revenge"_sv, MoveName::Revenge},
			{"reversal"_sv, MoveName::Reversal},
			{"revivalblessing"_sv, MoveName::Revival_Blessing},
			{"risingvoltage"_sv, MoveName::Rising_Voltage},
			{"roar"_sv, MoveName::Roar},
			{"roaroftime"_sv, MoveName::Roar_of_Time},
			{"rockblast"_sv, MoveName::Rock_Blast},
			{"rockclimb"_sv, MoveName::Rock_Climb},
			{"rockpolish"_sv, MoveName::Rock_Polish},
			{"rockslide"_sv, MoveName::Rock_Slide},
			{"rocksmash"_sv, MoveName::Rock_Smash},
			{"rockthrow"_sv, MoveName::Rock_Throw},
			{"rocktomb"_sv, MoveName::Rock_Tomb},
			{"rockwrecker"_sv, MoveName::Rock_Wrecker},
			{"roleplay"_sv, MoveName::Role_Play},
			{"rollingkick"_sv, MoveName::Rolling_Kick},
			{"rollout"_sv, MoveName::Rollout},
			{"roost"_sv, MoveName::Roost},
			{"rototiller"_sv, MoveName::Rototiller},
			{"round"_sv, MoveName::Round},
			{"ruination"_sv, MoveName::Ruination},
			{"sacredfire"_sv, MoveName::Sacred_Fire},
			{"sacredsword"_sv, MoveName::Sacred_Sword},
			{"safeguard"_sv, MoveName::Safeguard},
			{"saltcure"_sv, MoveName::Salt_Cure},
			{"sandattack"_sv, MoveName::Sand_Attack},
			{"sandstorm"_sv, MoveName::Sandstorm},
			{"sandtomb"_sv, MoveName::Sand_Tomb},
			{"savagespinout"_sv, MoveName::Savage_Spin_Out},
			{"scald"_sv, MoveName::Scald},
			{"scaleshot"_sv, MoveName::Scale_Shot},
			{"scaryface"_sv, MoveName::Scary_Face},
			{"scorchingsands"_sv, MoveName::Scorching_Sands},
			{"scratch"_sv, MoveName::Scratch},
			{"screech"_sv, MoveName::Screech},
			{"searingshot"_sv, MoveName::Searing_Shot},
			{"searingsunrazesmash"_sv, MoveName::Searing_Sunraze_Smash},
			{"secretpower"_sv, MoveName::Secret_Power},
			{"secretsword"_sv, MoveName::Secret_Sword},
			{"seedbomb"_sv, MoveName::Seed_Bomb},
			{"seedflare"_sv, MoveName::Seed_Flare},
			{"seismictoss"_sv, MoveName::Seismic_Toss},
			{"selfdestruct"_sv, MoveName::Self_Destruct},
			{"shadowball"_sv, MoveName::Shadow_Ball},
			{"shadowbone"_sv, MoveName::Shadow_Bone},
			{"shadowclaw"_sv, MoveName::Shadow_Claw},
			{"shadowforce"_sv, MoveName::Shadow_Force},
			{"shadowpunch"_sv, MoveName::Shadow_Punch},
			{"shadowsneak"_sv, MoveName::Shadow_Sneak},
			{"sharpen"_sv, MoveName::Sharpen},
			{"shatteredpsyche"_sv, MoveName::Shattered_Psyche},
			{"shedtail"_sv, MoveName::Shed_Tail},
			{"sheercold"_sv, MoveName::Sheer_Cold},
			{"shellsidearm"_sv, MoveName::Shell_Side_Arm},
			{"shellsmash"_sv, MoveName::Shell_Smash},
			{"shelltrap"_sv, MoveName::Shell_Trap},
			{"shiftgear"_sv, MoveName::Shift_Gear},
			{"shockwave"_sv, MoveName::Shock_Wave},
			{"shoreup"_sv, MoveName::Shore_Up},
			{"signalbeam"_sv, MoveName::Signal_Beam},
			{"silktrap"_sv, MoveName::Silk_Trap},
			{"silverwind"_sv, MoveName::Silver_Wind},
			{"simplebeam"_sv, MoveName::Simple_Beam},
			{"sing"_sv, MoveName::Sing},
			{"sinisterarrowraid"_sv, MoveName::Sinister_Arrow_Raid},
			{"sketch"_sv, MoveName::Sketch},
			{"skillswap"_sv, MoveName::Skill_Swap},
			{"skittersmack"_sv, MoveName::Skitter_Smack},
			{"skullbash"_sv, MoveName::Skull_Bash},
			{"skyattack"_sv, MoveName::Sky_Attack},
			{"skydrop"_sv, MoveName::Sky_Drop},
			{"skyuppercut"_sv, MoveName::Sky_Uppercut},
			{"slackoff"_sv, MoveName::Slack_Off},
			{"slam"_sv, MoveName::Slam},
			{"slash"_sv, MoveName::Slash},
			{"sleeppowder"_sv, MoveName::Sleep_Powder},
			{"sleeptalk"_sv, MoveName::Sleep_Talk},
			{"sludge"_sv, MoveName::Sludge},
			{"sludgebomb"_sv, MoveName::Sludge_Bomb},
			{"sludgewave"_sv, MoveName::Sludge_Wave},
			{"smackdown"_sv, MoveName::Smack_Down},
			{"smartstrike"_sv, MoveName::Smart_Strike},
			{"smellingsalt"_sv, MoveName::Smelling_Salts},
			{"smellingsalts"_sv, MoveName::Smelling_Salts},
			{"smog"_sv, MoveName::Smog},
			{"smokescreen"_sv, MoveName::Smokescreen},
			{"snaptrap"_sv, MoveName::Snap_Trap},
			{"snarl"_sv, MoveName::Snarl},
			{"snatch"_sv, MoveName::Snatch},
			{"snipeshot"_sv, MoveName::Snipe_Shot},
			{"snore"_sv, MoveName::Snore},
			{"snowscape"_sv, MoveName::Snowscape},
			{"soak"_sv, MoveName::Soak},
			{"softboiled"_sv, MoveName::Soft_Boiled},
			{"solarbeam"_sv, MoveName::Solar_Beam},
			{"solarblade"_sv, MoveName::Solar_Blade},
			{"sonicboom"_sv, MoveName::Sonic_Boom},
			{"soulstealing7starstrike"_sv, MoveName::Soul_Stealing_7_Star_Strike},
			{"spacialrend"_sv, MoveName::Spacial_Rend},
			{"spark"_sv, MoveName::Spark},
			{"sparklingaria"_sv, MoveName::Sparkling_Aria},
			{"spectralthief"_sv, MoveName::Spectral_Thief},
			{"speedswap"_sv, MoveName::Speed_Swap},
			{"spicyextract"_sv, MoveName::Spicy_Extract},
			{"spiderweb"_sv, MoveName::Spider_Web},
			{"spikecannon"_sv, MoveName::Spike_Cannon},
			{"spikes"_sv, MoveName::Spikes},
			{"spikyshield"_sv, MoveName::Spiky_Shield},
			{"spinout"_sv, MoveName::Spin_Out},
			{"spiritbreak"_sv, MoveName::Spirit_Break},
			{"spiritshackle"_sv, MoveName::Spirit_Shackle},
			{"spite"_sv, MoveName::Spite},
			{"spitup"_sv, MoveName::Spit_Up},
			{"splash"_sv, MoveName::Splash},
			{"splinteredstormshards"_sv, MoveName::Splintered_Stormshards},
			{"spore"_sv, MoveName::Spore},
			{"spotlight"_sv, MoveName::Spotlight},
			{"stealthrock"_sv, MoveName::Stealth_Rock},
			{"steameruption"_sv, MoveName::Steam_Eruption},
			{"steamroller"_sv, MoveName::Steamroller},
			{"steelbeam"_sv, MoveName::Steel_Beam},
			{"steelroller"_sv, MoveName::Steel_Roller},
			{"steelwing"_sv, MoveName::Steel_Wing},
			{"stickyweb"_sv, MoveName::Sticky_Web},
			{"stockpile"_sv, MoveName::Stockpile},
			{"stokedsparksurfer"_sv, MoveName::Stoked_Sparksurfer},
			{"stomp"_sv, MoveName::Stomp},
			{"stompingtantrum"_sv, MoveName::Stomping_Tantrum},
			{"stoneedge"_sv, MoveName::Stone_Edge},
			{"storedpower"_sv, MoveName::Stored_Power},
			{"stormthrow"_sv, MoveName::Storm_Throw},
			{"strangesteam"_sv, MoveName::Strange_Steam},
			{"strength"_sv, MoveName::Strength},
			{"strengthsap"_sv, MoveName::Strength_Sap},
			{"stringshot"_sv, MoveName::String_Shot},
			{"struggle"_sv, MoveName::Struggle},
			{"strugglebug"_sv, MoveName::Struggle_Bug},
			{"stuffcheeks"_sv, MoveName::Stuff_Cheeks},
			{"stunspore"_sv, MoveName::Stun_Spore},
			{"submission"_sv, MoveName::Submission},
			{"substitute"_sv, MoveName::Substitute},
			{"subzeroslammer"_sv, MoveName::Subzero_Slammer},
			{"suckerpunch"_sv, MoveName::Sucker_Punch},
			{"sunnyday"_sv, MoveName::Sunny_Day},
			{"sunsteelstrike"_sv, MoveName::Sunsteel_Strike},
			{"superfang"_sv, MoveName::Super_Fang},
			{"superpower"_sv, MoveName::Superpower},
			{"supersonic"_sv, MoveName::Supersonic},
			{"supersonicskystrike"_sv, MoveName::Supersonic_Skystrike},
			{"surf"_sv, MoveName::Surf},
			{"surgingstrikes"_sv, MoveName::Surging_Strikes},
			{"swagger"_sv, MoveName::Swagger},
			{"swallow"_sv, MoveName::Swallow},
			{"sweetkiss"_sv, MoveName::Sweet_Kiss},
			{"sweetscent"_sv, MoveName::Sweet_Scent},
			{"swift"_sv, MoveName::Swift},
			{"switcheroo"_sv, MoveName::Switcheroo},
			{"swordsdance"_sv, MoveName::Swords_Dance},
			{"synchronoise"_sv, MoveName::Synchronoise},
			{"synthesis"_sv, MoveName::Synthesis},
			{"syrupbomb"_sv, MoveName::Syrup_Bomb},
			{"tachyoncutter"_sv, MoveName::Tachyon_Cutter},
			{"tackle"_sv, MoveName::Tackle},
			{"tailglow"_sv, MoveName::Tail_Glow},
			{"tailslap"_sv, MoveName::Tail_Slap},
			{"tailwhip"_sv, MoveName::Tail_Whip},
			{"tailwind"_sv, MoveName::Tailwind},
			{"takedown"_sv, MoveName::Take_Down},
			{"tarshot"_sv, MoveName::Tar_Shot},
			{"taunt"_sv, MoveName::Taunt},
			{"tearfullook"_sv, MoveName::Tearful_Look},
			{"teatime"_sv, MoveName::Teatime},
			{"technoblast"_sv, MoveName::Techno_Blast},
			{"tectonicrage"_sv, MoveName::Tectonic_Rage},
			{"teeterdance"_sv, MoveName::Teeter_Dance},
			{"telekinesis"_sv, MoveName::Telekinesis},
			{"teleport"_sv, MoveName::Teleport},
			{"terablast"_sv, MoveName::Tera_Blast},
			{"terrainpulse"_sv, MoveName::Terrain_Pulse},
			{"thief"_sv, MoveName::Thief},
			{"thousandarrows"_sv, MoveName::Thousand_Arrows},
			{"thousandwaves"_sv, MoveName::Thousand_Waves},
			{"thrash"_sv, MoveName::Thrash},
			{"throatchop"_sv, MoveName::Throat_Chop},
			{"thunder"_sv, MoveName::Thunder},
			{"thunderbolt"_sv, MoveName::Thunderbolt},
			{"thundercage"_sv, MoveName::Thunder_Cage},
			{"thunderclap"_sv, MoveName::Thunderclap},
			{"thunderfang"_sv, MoveName::Thunder_Fang},
			{"thunderouskick"_sv, MoveName::Thunderous_Kick},
			{"thunderpunch"_sv, MoveName::Thunder_Punch},
			{"thundershock"_sv, MoveName::Thunder_Shock},
			{"thunderwave"_sv, MoveName::Thunder_Wave},
			{"tickle"_sv, MoveName::Tickle},
			{"tidyup"_sv, MoveName::Tidy_Up},
			{"topsyturvy"_sv, MoveName::Topsy_Turvy},
			{"torchsong"_sv, MoveName::Torch_Song},
			{"torment"_sv, MoveName::Torment},
			{"toxic"_sv, MoveName::Toxic},
			{"toxicspikes"_sv, MoveName::Toxic_Spikes},
			{"toxicthread"_sv, MoveName::Toxic_Thread},
			{"trailblaze"_sv, MoveName::Trailblaze},
			{"transform"_sv, MoveName::Transform},
			{"triattack"_sv, MoveName::Tri_Attack},
			{"trick"_sv, MoveName::Trick},
			{"trickortreat"_sv, MoveName::Trick_or_Treat},
			{"trickroom"_sv, MoveName::Trick_Room},
			{"tripleaxel"_sv, MoveName::Triple_Axel},
			{"tripledive"_sv, MoveName::Triple_Dive},
			{"triplekick"_sv, MoveName::Triple_Kick},
			{"tropkick"_sv, MoveName::Trop_Kick},
			{"trumpcard"_sv, MoveName::Trump_Card},
			{"twinbeam"_sv, MoveName::Twin_Beam},
			{"twineedle"_sv, MoveName::Twineedle},
			{"twinkletackle"_sv, MoveName::Twinkle_Tackle},
			{"twister"_sv, MoveName::Twister},
			{"upperhand"_sv, MoveName::Upper_Hand},
			{"uproar"_sv, MoveName::Uproar},
			{"uturn"_sv, MoveName::U_turn},
			{"vacuumwave"_sv, MoveName::Vacuum_Wave},
			{"vcreate"_sv, MoveName::V_create},
			{"venomdrench"_sv, MoveName::Venom_Drench},
			{"venoshock"_sv, MoveName::Venoshock},
			{"vicegrip"_sv, MoveName::Vise_Grip},
			{"vinewhip"_sv, MoveName::Vine_Whip},
			{"visegrip"_sv, MoveName::Vise_Grip},
			{"vitalthrow"_sv, MoveName::Vital_Throw},
			{"voltswitch"_sv, MoveName::Volt_Switch},
			{"volttackle"_sv, MoveName::Volt_Tackle},
			{"wakeupslap"_sv, MoveName::Wake_Up_Slap},
			{"waterfall"_sv, MoveName::Waterfall},
			{"watergun"_sv, MoveName::Water_Gun},
			{"waterpledge"_sv, MoveName::Water_Pledge},
			{"waterpulse"_sv, MoveName::Water_Pulse},
			{"watershuriken"_sv, MoveName::Water_Shuriken},
			{"watersport"_sv, MoveName::Water_Sport},
			{"waterspout"_sv, MoveName::Water_Spout},
			{"weatherball"_sv, MoveName::Weather_Ball},
			{"whirlpool"_sv, MoveName::Whirlpool},
			{"whirlwind"_sv, MoveName::Whirlwind},
			{"wickedblow"_sv, MoveName::Wicked_Blow},
			{"wickedtorque"_sv, MoveName::Wicked_Torque},
			{"wideguard"_sv, MoveName::Wide_Guard},
			{"wildcharge"_sv, MoveName::Wild_Charge},
			{"willowisp"_sv, MoveName::Will_O_Wisp},
			{"wingattack"_sv, MoveName::Wing_Attack},
			{"wish"_sv, MoveName::Wish},
			{"withdraw"_sv, MoveName::Withdraw},
			{"wonderroom"_sv, MoveName::Wonder_Room},
			{"woodhammer"_sv, MoveName::Wood_Hammer},
			{"workup"_sv, MoveName::Work_Up},
			{"worryseed"_sv, MoveName::Worry_Seed},
			{"wrap"_sv, MoveName::Wrap},
			{"wringout"_sv, MoveName::Wring_Out},
			{"xscissor"_sv, MoveName::X_Scissor},
			{"yawn"_sv, MoveName::Yawn},
			{"zapcannon"_sv, MoveName::Zap_Cannon},
			{"zenheadbutt"_sv, MoveName::Zen_Headbutt},
			{"zingzap"_sv, MoveName::Zing_Zap},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<25_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Move"_sv, str);
	}
	return *result;
}

} // namespace technicalmachine
