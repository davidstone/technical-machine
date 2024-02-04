// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.target;

import tm.move.move_name;

import tm.generation;

namespace technicalmachine {

export enum class Target {
	user,
	adjacent_ally,
	user_or_adjacent_ally,
	all_allies,
	user_and_all_allies,
	user_team,
	user_field,
	adjacent_foe,
	all_adjacent_foes,
	foe_field,
	adjacent,
	any,
	all_adjacent,
	all,
	field,
};

export constexpr auto move_target(Generation const generation, MoveName const move) -> Target {
	switch (move) {
		case MoveName::Mist:
			return generation <= Generation::two ? Target::user : Target::user_field;
		case MoveName::Helping_Hand:
			return generation <= Generation::three ? Target::user : Target::adjacent_ally;
		case MoveName::Surf:
			return generation <= Generation::three ? Target::all_adjacent_foes : Target::all_adjacent;
		case MoveName::Conversion_2:
			return generation <= Generation::four ? Target::user : Target::adjacent;
		case MoveName::Poison_Gas:
			return generation <= Generation::four ? Target::any : Target::all_adjacent_foes;
		case MoveName::Cotton_Spore:
			return generation <= Generation::five ? Target::adjacent : Target::all_adjacent_foes;
		case MoveName::Nature_Power:
			return generation <= Generation::five ? Target::user : Target::adjacent;
		case MoveName::Howl:
			return generation <= Generation::seven ? Target::user : Target::user_and_all_allies;
		case MoveName::Aromatherapy:
		case MoveName::Heal_Bell:
			return Target::user_team;
		case MoveName::Coaching:
			return Target::all_allies;
		case MoveName::Gear_Up:
		case MoveName::Jungle_Healing:
		case MoveName::Life_Dew:
		case MoveName::Magnetic_Flux:
			return Target::user_and_all_allies;
		case MoveName::Aurora_Veil:
		case MoveName::Crafty_Shield:
		case MoveName::Happy_Hour:
		case MoveName::Light_Screen:
		case MoveName::Lucky_Chant:
		case MoveName::Mat_Block:
		case MoveName::Quick_Guard:
		case MoveName::Reflect:
		case MoveName::Safeguard:
		case MoveName::Tailwind:
		case MoveName::Wide_Guard:
			return Target::user_field;
		case MoveName::Chilly_Reception:
		case MoveName::Court_Change:
		case MoveName::Electric_Terrain:
		case MoveName::Fairy_Lock:
		case MoveName::Grassy_Terrain:
		case MoveName::Gravity:
		case MoveName::Hail:
		case MoveName::Ion_Deluge:
		case MoveName::Magic_Room:
		case MoveName::Misty_Terrain:
		case MoveName::Psychic_Terrain:
		case MoveName::Rain_Dance:
		case MoveName::Sandstorm:
		case MoveName::Snowscape:
		case MoveName::Sunny_Day:
		case MoveName::Trick_Room:
		case MoveName::Wonder_Room:
			return Target::field;
		case MoveName::Flower_Shield:
		case MoveName::Haze:
		case MoveName::Mud_Sport:
		case MoveName::Perish_Song:
		case MoveName::Rototiller:
		case MoveName::Teatime:
		case MoveName::Water_Sport:
			return Target::all;
		case MoveName::Acrobatics:
		case MoveName::Aerial_Ace:
		case MoveName::Aeroblast:
		case MoveName::Air_Slash:
		case MoveName::Aura_Sphere:
		case MoveName::Bounce:
		case MoveName::Brave_Bird:
		case MoveName::Chatter:
		case MoveName::Dark_Pulse:
		case MoveName::Dragon_Pulse:
		case MoveName::Drill_Peck:
		case MoveName::Fly:
		case MoveName::Flying_Press:
		case MoveName::Gust:
		case MoveName::Heal_Pulse:
		case MoveName::Hurricane:
		case MoveName::Oblivion_Wing:
		case MoveName::Peck:
		case MoveName::Pluck:
		case MoveName::Sky_Attack:
		case MoveName::Sky_Drop:
		case MoveName::Water_Pulse:
		case MoveName::Wing_Attack:
			return Target::any;
		case MoveName::Aromatic_Mist:
		case MoveName::Hold_Hands:
			return Target::adjacent_ally;
		case MoveName::Acupressure:
			return Target::user_or_adjacent_ally;
		case MoveName::Spikes:
		case MoveName::Stealth_Rock:
		case MoveName::Sticky_Web:
		case MoveName::Toxic_Spikes:
			return Target::foe_field;
		case MoveName::Boomburst:
		case MoveName::Brutal_Swing:
		case MoveName::Bulldoze:
		case MoveName::Corrosive_Gas:
		case MoveName::Discharge:
		case MoveName::Earthquake:
		case MoveName::Explosion:
		case MoveName::Lava_Plume:
		case MoveName::Magnitude:
		case MoveName::Mind_Blown:
		case MoveName::Misty_Explosion:
		case MoveName::Parabolic_Charge:
		case MoveName::Petal_Blizzard:
		case MoveName::Searing_Shot:
		case MoveName::Self_Destruct:
		case MoveName::Sludge_Wave:
		case MoveName::Sparkling_Aria:
		case MoveName::Synchronoise:
		case MoveName::Teeter_Dance:
			return Target::all_adjacent;
		case MoveName::Acid:
		case MoveName::Air_Cutter:
		case MoveName::Astral_Barrage:
		case MoveName::Blizzard:
		case MoveName::Breaking_Swipe:
		case MoveName::Bubble:
		case MoveName::Burning_Jealousy:
		case MoveName::Captivate:
		case MoveName::Clanging_Scales:
		case MoveName::Clangorous_Soulblaze:
		case MoveName::Core_Enforcer:
		case MoveName::Dark_Void:
		case MoveName::Dazzling_Gleam:
		case MoveName::Diamond_Storm:
		case MoveName::Disarming_Voice:
		case MoveName::Dragon_Energy:
		case MoveName::Electroweb:
		case MoveName::Eruption:
		case MoveName::Fiery_Wrath:
		case MoveName::Glacial_Lance:
		case MoveName::Glaciate:
		case MoveName::Growl:
		case MoveName::Heal_Block:
		case MoveName::Heat_Wave:
		case MoveName::Hyper_Voice:
		case MoveName::Icy_Wind:
		case MoveName::Incinerate:
		case MoveName::Lands_Wrath:
		case MoveName::Leer:
		case MoveName::Make_It_Rain:
		case MoveName::Matcha_Gotcha:
		case MoveName::Mortal_Spin:
		case MoveName::Muddy_Water:
		case MoveName::Origin_Pulse:
		case MoveName::Overdrive:
		case MoveName::Powder_Snow:
		case MoveName::Precipice_Blades:
		case MoveName::Razor_Leaf:
		case MoveName::Razor_Wind:
		case MoveName::Relic_Song:
		case MoveName::Rock_Slide:
		case MoveName::Shell_Trap:
		case MoveName::Snarl:
		case MoveName::String_Shot:
		case MoveName::Struggle_Bug:
		case MoveName::Sweet_Scent:
		case MoveName::Swift:
		case MoveName::Tail_Whip:
		case MoveName::Thousand_Arrows:
		case MoveName::Thousand_Waves:
		case MoveName::Twister:
		case MoveName::Venom_Drench:
		case MoveName::Water_Spout:
			return Target::all_adjacent_foes;
		case MoveName::Doodle:
		case MoveName::Max_Airstream:
		case MoveName::Max_Darkness:
		case MoveName::Max_Flare:
		case MoveName::Max_Flutterby:
		case MoveName::Max_Geyser:
		case MoveName::Max_Hailstorm:
		case MoveName::Max_Knuckle:
		case MoveName::Max_Lightning:
		case MoveName::Max_Mindstorm:
		case MoveName::Max_Ooze:
		case MoveName::Max_Overgrowth:
		case MoveName::Max_Phantasm:
		case MoveName::Max_Quake:
		case MoveName::Max_Rockfall:
		case MoveName::Max_Starfall:
		case MoveName::Max_Steelspike:
		case MoveName::Max_Strike:
		case MoveName::Max_Wyrmwind:
		case MoveName::Me_First:
		case MoveName::Struggle:
			return Target::adjacent_foe;
		case MoveName::Acid_Armor:
		case MoveName::Agility:
		case MoveName::Ally_Switch:
		case MoveName::Amnesia:
		case MoveName::Aqua_Ring:
		case MoveName::Assist:
		case MoveName::Autotomize:
		case MoveName::Baneful_Bunker:
		case MoveName::Barrier:
		case MoveName::Baton_Pass:
		case MoveName::Belly_Drum:
		case MoveName::Bide:
		case MoveName::Bulk_Up:
		case MoveName::Calm_Mind:
		case MoveName::Camouflage:
		case MoveName::Celebrate:
		case MoveName::Charge:
		case MoveName::Clangorous_Soul:
		case MoveName::Coil:
		case MoveName::Comeuppance:
		case MoveName::Conversion:
		case MoveName::Copycat:
		case MoveName::Cosmic_Power:
		case MoveName::Cotton_Guard:
		case MoveName::Counter:
		case MoveName::Curse:
		case MoveName::Defend_Order:
		case MoveName::Defense_Curl:
		case MoveName::Destiny_Bond:
		case MoveName::Detect:
		case MoveName::Double_Team:
		case MoveName::Dragon_Dance:
		case MoveName::Endure:
		case MoveName::Extreme_Evoboost:
		case MoveName::Fillet_Away:
		case MoveName::Focus_Energy:
		case MoveName::Follow_Me:
		case MoveName::Geomancy:
		case MoveName::Growth:
		case MoveName::Grudge:
		case MoveName::Harden:
		case MoveName::Heal_Order:
		case MoveName::Healing_Wish:
		case MoveName::Hone_Claws:
		case MoveName::Imprison:
		case MoveName::Ingrain:
		case MoveName::Iron_Defense:
		case MoveName::Kings_Shield:
		case MoveName::Laser_Focus:
		case MoveName::Lunar_Dance:
		case MoveName::Magic_Coat:
		case MoveName::Magnet_Rise:
		case MoveName::Max_Guard:
		case MoveName::Meditate:
		case MoveName::Metal_Burst:
		case MoveName::Metronome:
		case MoveName::Milk_Drink:
		case MoveName::Minimize:
		case MoveName::Mirror_Coat:
		case MoveName::Moonlight:
		case MoveName::Morning_Sun:
		case MoveName::Nasty_Plot:
		case MoveName::No_Retreat:
		case MoveName::Obstruct:
		case MoveName::Outrage:
		case MoveName::Petal_Dance:
		case MoveName::Power_Trick:
		case MoveName::Protect:
		case MoveName::Quiver_Dance:
		case MoveName::Rage_Powder:
		case MoveName::Recover:
		case MoveName::Recycle:
		case MoveName::Refresh:
		case MoveName::Rest:
		case MoveName::Revival_Blessing:
		case MoveName::Rock_Polish:
		case MoveName::Roost:
		case MoveName::Sharpen:
		case MoveName::Shed_Tail:
		case MoveName::Shell_Smash:
		case MoveName::Shift_Gear:
		case MoveName::Shore_Up:
		case MoveName::Silk_Trap:
		case MoveName::Slack_Off:
		case MoveName::Sleep_Talk:
		case MoveName::Snatch:
		case MoveName::Soft_Boiled:
		case MoveName::Spiky_Shield:
		case MoveName::Splash:
		case MoveName::Stockpile:
		case MoveName::Stuff_Cheeks:
		case MoveName::Substitute:
		case MoveName::Swallow:
		case MoveName::Swords_Dance:
		case MoveName::Synthesis:
		case MoveName::Tail_Glow:
		case MoveName::Teleport:
		case MoveName::Thrash:
		case MoveName::Tidy_Up:
		case MoveName::Uproar:
		case MoveName::Wish:
		case MoveName::Withdraw:
		case MoveName::Work_Up:
			return Target::user;
		case MoveName::m10000000_Volt_Thunderbolt:
		case MoveName::Absorb:
		case MoveName::Accelerock:
		case MoveName::Acid_Downpour:
		case MoveName::Acid_Spray:
		case MoveName::After_You:
		case MoveName::All_Out_Pummeling:
		case MoveName::Anchor_Shot:
		case MoveName::Ancient_Power:
		case MoveName::Apple_Acid:
		case MoveName::Aqua_Cutter:
		case MoveName::Aqua_Jet:
		case MoveName::Aqua_Step:
		case MoveName::Aqua_Tail:
		case MoveName::Arm_Thrust:
		case MoveName::Armor_Cannon:
		case MoveName::Assurance:
		case MoveName::Astonish:
		case MoveName::Attack_Order:
		case MoveName::Attract:
		case MoveName::Aura_Wheel:
		case MoveName::Aurora_Beam:
		case MoveName::Avalanche:
		case MoveName::Axe_Kick:
		case MoveName::Baby_Doll_Eyes:
		case MoveName::Barrage:
		case MoveName::Beak_Blast:
		case MoveName::Beat_Up:
		case MoveName::Behemoth_Bash:
		case MoveName::Behemoth_Blade:
		case MoveName::Belch:
		case MoveName::Bestow:
		case MoveName::Bind:
		case MoveName::Bite:
		case MoveName::Bitter_Blade:
		case MoveName::Black_Hole_Eclipse:
		case MoveName::Blast_Burn:
		case MoveName::Blaze_Kick:
		case MoveName::Blazing_Torque:
		case MoveName::Block:
		case MoveName::Blood_Moon:
		case MoveName::Bloom_Doom:
		case MoveName::Blue_Flare:
		case MoveName::Body_Press:
		case MoveName::Body_Slam:
		case MoveName::Bolt_Beak:
		case MoveName::Bolt_Strike:
		case MoveName::Bone_Club:
		case MoveName::Bone_Rush:
		case MoveName::Bonemerang:
		case MoveName::Branch_Poke:
		case MoveName::Breakneck_Blitz:
		case MoveName::Brick_Break:
		case MoveName::Brine:
		case MoveName::Bubble_Beam:
		case MoveName::Bug_Bite:
		case MoveName::Bug_Buzz:
		case MoveName::Bullet_Punch:
		case MoveName::Bullet_Seed:
		case MoveName::Burn_Up:
		case MoveName::Catastropika:
		case MoveName::Charge_Beam:
		case MoveName::Charm:
		case MoveName::Chilling_Water:
		case MoveName::Chip_Away:
		case MoveName::Circle_Throw:
		case MoveName::Clamp:
		case MoveName::Clear_Smog:
		case MoveName::Close_Combat:
		case MoveName::Collision_Course:
		case MoveName::Combat_Torque:
		case MoveName::Comet_Punch:
		case MoveName::Confide:
		case MoveName::Confuse_Ray:
		case MoveName::Confusion:
		case MoveName::Constrict:
		case MoveName::Continental_Crush:
		case MoveName::Corkscrew_Crash:
		case MoveName::Covet:
		case MoveName::Crabhammer:
		case MoveName::Cross_Chop:
		case MoveName::Cross_Poison:
		case MoveName::Crunch:
		case MoveName::Crush_Claw:
		case MoveName::Crush_Grip:
		case MoveName::Cut:
		case MoveName::Darkest_Lariat:
		case MoveName::Decorate:
		case MoveName::Defog:
		case MoveName::Devastating_Drake:
		case MoveName::Dig:
		case MoveName::Disable:
		case MoveName::Dive:
		case MoveName::Dizzy_Punch:
		case MoveName::Doom_Desire:
		case MoveName::Double_Edge:
		case MoveName::Double_Hit:
		case MoveName::Double_Iron_Bash:
		case MoveName::Double_Kick:
		case MoveName::Double_Shock:
		case MoveName::Double_Slap:
		case MoveName::Draco_Meteor:
		case MoveName::Dragon_Ascent:
		case MoveName::Dragon_Breath:
		case MoveName::Dragon_Claw:
		case MoveName::Dragon_Darts:
		case MoveName::Dragon_Hammer:
		case MoveName::Dragon_Rage:
		case MoveName::Dragon_Rush:
		case MoveName::Dragon_Tail:
		case MoveName::Drain_Punch:
		case MoveName::Draining_Kiss:
		case MoveName::Dream_Eater:
		case MoveName::Drill_Run:
		case MoveName::Drum_Beating:
		case MoveName::Dual_Chop:
		case MoveName::Dual_Wingbeat:
		case MoveName::Dynamax_Cannon:
		case MoveName::Dynamic_Punch:
		case MoveName::Earth_Power:
		case MoveName::Echoed_Voice:
		case MoveName::Eerie_Impulse:
		case MoveName::Eerie_Spell:
		case MoveName::Egg_Bomb:
		case MoveName::Electrify:
		case MoveName::Electro_Ball:
		case MoveName::Electro_Drift:
		case MoveName::Electro_Shot:
		case MoveName::Embargo:
		case MoveName::Ember:
		case MoveName::Encore:
		case MoveName::Endeavor:
		case MoveName::Energy_Ball:
		case MoveName::Entrainment:
		case MoveName::Eternabeam:
		case MoveName::Expanding_Force:
		case MoveName::Extrasensory:
		case MoveName::Extreme_Speed:
		case MoveName::Facade:
		case MoveName::Fairy_Wind:
		case MoveName::Fake_Out:
		case MoveName::Fake_Tears:
		case MoveName::False_Surrender:
		case MoveName::False_Swipe:
		case MoveName::Feather_Dance:
		case MoveName::Feint:
		case MoveName::Feint_Attack:
		case MoveName::Fell_Stinger:
		case MoveName::Fiery_Dance:
		case MoveName::Final_Gambit:
		case MoveName::Fire_Blast:
		case MoveName::Fire_Fang:
		case MoveName::Fire_Lash:
		case MoveName::Fire_Pledge:
		case MoveName::Fire_Punch:
		case MoveName::Fire_Spin:
		case MoveName::First_Impression:
		case MoveName::Fishious_Rend:
		case MoveName::Fissure:
		case MoveName::Flail:
		case MoveName::Flame_Burst:
		case MoveName::Flame_Charge:
		case MoveName::Flame_Wheel:
		case MoveName::Flamethrower:
		case MoveName::Flare_Blitz:
		case MoveName::Flash:
		case MoveName::Flash_Cannon:
		case MoveName::Flatter:
		case MoveName::Fleur_Cannon:
		case MoveName::Fling:
		case MoveName::Flip_Turn:
		case MoveName::Floral_Healing:
		case MoveName::Flower_Trick:
		case MoveName::Focus_Blast:
		case MoveName::Focus_Punch:
		case MoveName::Force_Palm:
		case MoveName::Foresight:
		case MoveName::Forests_Curse:
		case MoveName::Foul_Play:
		case MoveName::Freeze_Dry:
		case MoveName::Freeze_Shock:
		case MoveName::Freezing_Glare:
		case MoveName::Frenzy_Plant:
		case MoveName::Frost_Breath:
		case MoveName::Frustration:
		case MoveName::Fury_Attack:
		case MoveName::Fury_Cutter:
		case MoveName::Fury_Swipes:
		case MoveName::Fusion_Bolt:
		case MoveName::Fusion_Flare:
		case MoveName::Future_Sight:
		case MoveName::Gastro_Acid:
		case MoveName::Gear_Grind:
		case MoveName::Genesis_Supernova:
		case MoveName::Giga_Drain:
		case MoveName::Giga_Impact:
		case MoveName::Gigaton_Hammer:
		case MoveName::Gigavolt_Havoc:
		case MoveName::Glaive_Rush:
		case MoveName::Glare:
		case MoveName::Grass_Knot:
		case MoveName::Grass_Pledge:
		case MoveName::Grass_Whistle:
		case MoveName::Grassy_Glide:
		case MoveName::Grav_Apple:
		case MoveName::Guard_Split:
		case MoveName::Guard_Swap:
		case MoveName::Guardian_of_Alola:
		case MoveName::Guillotine:
		case MoveName::Gunk_Shot:
		case MoveName::Gyro_Ball:
		case MoveName::Hammer_Arm:
		case MoveName::Head_Charge:
		case MoveName::Head_Smash:
		case MoveName::Headbutt:
		case MoveName::Heart_Stamp:
		case MoveName::Heart_Swap:
		case MoveName::Heat_Crash:
		case MoveName::Heavy_Slam:
		case MoveName::Hex:
		case MoveName::Hidden_Power:
		case MoveName::High_Horsepower:
		case MoveName::High_Jump_Kick:
		case MoveName::Hold_Back:
		case MoveName::Horn_Attack:
		case MoveName::Horn_Drill:
		case MoveName::Horn_Leech:
		case MoveName::Hydro_Cannon:
		case MoveName::Hydro_Pump:
		case MoveName::Hydro_Steam:
		case MoveName::Hydro_Vortex:
		case MoveName::Hyper_Beam:
		case MoveName::Hyper_Drill:
		case MoveName::Hyper_Fang:
		case MoveName::Hyperspace_Fury:
		case MoveName::Hyperspace_Hole:
		case MoveName::Hypnosis:
		case MoveName::Ice_Ball:
		case MoveName::Ice_Beam:
		case MoveName::Ice_Burn:
		case MoveName::Ice_Fang:
		case MoveName::Ice_Hammer:
		case MoveName::Ice_Punch:
		case MoveName::Ice_Shard:
		case MoveName::Ice_Spinner:
		case MoveName::Icicle_Crash:
		case MoveName::Icicle_Spear:
		case MoveName::Inferno:
		case MoveName::Inferno_Overdrive:
		case MoveName::Infestation:
		case MoveName::Instruct:
		case MoveName::Iron_Head:
		case MoveName::Iron_Tail:
		case MoveName::Ivy_Cudgel:
		case MoveName::Jaw_Lock:
		case MoveName::Jet_Punch:
		case MoveName::Judgment:
		case MoveName::Jump_Kick:
		case MoveName::Karate_Chop:
		case MoveName::Kinesis:
		case MoveName::Knock_Off:
		case MoveName::Kowtow_Cleave:
		case MoveName::Lash_Out:
		case MoveName::Last_Resort:
		case MoveName::Last_Respects:
		case MoveName::Leaf_Blade:
		case MoveName::Leaf_Storm:
		case MoveName::Leaf_Tornado:
		case MoveName::Leafage:
		case MoveName::Leech_Life:
		case MoveName::Leech_Seed:
		case MoveName::Lets_Snuggle_Forever:
		case MoveName::Lick:
		case MoveName::Light_of_Ruin:
		case MoveName::Light_That_Burns_the_Sky:
		case MoveName::Liquidation:
		case MoveName::Lock_On:
		case MoveName::Lovely_Kiss:
		case MoveName::Low_Kick:
		case MoveName::Low_Sweep:
		case MoveName::Lumina_Crash:
		case MoveName::Lunge:
		case MoveName::Luster_Purge:
		case MoveName::Mach_Punch:
		case MoveName::Magic_Powder:
		case MoveName::Magical_Leaf:
		case MoveName::Magical_Torque:
		case MoveName::Magma_Storm:
		case MoveName::Magnet_Bomb:
		case MoveName::Malicious_Moonsault:
		case MoveName::Mean_Look:
		case MoveName::Mega_Drain:
		case MoveName::Mega_Kick:
		case MoveName::Mega_Punch:
		case MoveName::Megahorn:
		case MoveName::Memento:
		case MoveName::Menacing_Moonraze_Maelstrom:
		case MoveName::Metal_Claw:
		case MoveName::Metal_Sound:
		case MoveName::Meteor_Assault:
		case MoveName::Meteor_Beam:
		case MoveName::Meteor_Mash:
		case MoveName::Mimic:
		case MoveName::Mind_Reader:
		case MoveName::Miracle_Eye:
		case MoveName::Mirror_Move:
		case MoveName::Mirror_Shot:
		case MoveName::Mist_Ball:
		case MoveName::Moonblast:
		case MoveName::Moongeist_Beam:
		case MoveName::Mud_Bomb:
		case MoveName::Mud_Shot:
		case MoveName::Mud_Slap:
		case MoveName::Multi_Attack:
		case MoveName::Mystical_Fire:
		case MoveName::Natural_Gift:
		case MoveName::Natures_Madness:
		case MoveName::Needle_Arm:
		case MoveName::Never_Ending_Nightmare:
		case MoveName::Night_Daze:
		case MoveName::Night_Shade:
		case MoveName::Night_Slash:
		case MoveName::Nightmare:
		case MoveName::Noble_Roar:
		case MoveName::Noxious_Torque:
		case MoveName::Nuzzle:
		case MoveName::Oceanic_Operetta:
		case MoveName::Octazooka:
		case MoveName::Octolock:
		case MoveName::Odor_Sleuth:
		case MoveName::Ominous_Wind:
		case MoveName::Order_Up:
		case MoveName::Overheat:
		case MoveName::Pain_Split:
		case MoveName::Parting_Shot:
		case MoveName::Pay_Day:
		case MoveName::Payback:
		case MoveName::Phantom_Force:
		case MoveName::Photon_Geyser:
		case MoveName::Pin_Missile:
		case MoveName::Plasma_Fists:
		case MoveName::Play_Nice:
		case MoveName::Play_Rough:
		case MoveName::Poison_Fang:
		case MoveName::Poison_Jab:
		case MoveName::Poison_Powder:
		case MoveName::Poison_Sting:
		case MoveName::Poison_Tail:
		case MoveName::Pollen_Puff:
		case MoveName::Poltergeist:
		case MoveName::Population_Bomb:
		case MoveName::Pounce:
		case MoveName::Pound:
		case MoveName::Powder:
		case MoveName::Power_Gem:
		case MoveName::Power_Split:
		case MoveName::Power_Swap:
		case MoveName::Power_Trip:
		case MoveName::Power_Up_Punch:
		case MoveName::Power_Whip:
		case MoveName::Present:
		case MoveName::Prismatic_Laser:
		case MoveName::Psybeam:
		case MoveName::Psyblade:
		case MoveName::Psych_Up:
		case MoveName::Psychic:
		case MoveName::Psychic_Fangs:
		case MoveName::Psychic_Noise:
		case MoveName::Psycho_Boost:
		case MoveName::Psycho_Cut:
		case MoveName::Psycho_Shift:
		case MoveName::Psyshock:
		case MoveName::Psystrike:
		case MoveName::Psywave:
		case MoveName::Pulverizing_Pancake:
		case MoveName::Punishment:
		case MoveName::Purify:
		case MoveName::Pursuit:
		case MoveName::Pyro_Ball:
		case MoveName::Quash:
		case MoveName::Quick_Attack:
		case MoveName::Rage_Fist:
		case MoveName::Rage:
		case MoveName::Raging_Bull:
		case MoveName::Rapid_Spin:
		case MoveName::Razor_Shell:
		case MoveName::Reflect_Type:
		case MoveName::Retaliate:
		case MoveName::Return:
		case MoveName::Revelation_Dance:
		case MoveName::Revenge:
		case MoveName::Reversal:
		case MoveName::Rising_Voltage:
		case MoveName::Roar:
		case MoveName::Roar_of_Time:
		case MoveName::Rock_Blast:
		case MoveName::Rock_Climb:
		case MoveName::Rock_Smash:
		case MoveName::Rock_Throw:
		case MoveName::Rock_Tomb:
		case MoveName::Rock_Wrecker:
		case MoveName::Role_Play:
		case MoveName::Rolling_Kick:
		case MoveName::Rollout:
		case MoveName::Round:
		case MoveName::Ruination:
		case MoveName::Sacred_Fire:
		case MoveName::Sacred_Sword:
		case MoveName::Salt_Cure:
		case MoveName::Sand_Attack:
		case MoveName::Sand_Tomb:
		case MoveName::Savage_Spin_Out:
		case MoveName::Scald:
		case MoveName::Scale_Shot:
		case MoveName::Scary_Face:
		case MoveName::Scorching_Sands:
		case MoveName::Scratch:
		case MoveName::Screech:
		case MoveName::Searing_Sunraze_Smash:
		case MoveName::Secret_Power:
		case MoveName::Secret_Sword:
		case MoveName::Seed_Bomb:
		case MoveName::Seed_Flare:
		case MoveName::Seismic_Toss:
		case MoveName::Shadow_Ball:
		case MoveName::Shadow_Bone:
		case MoveName::Shadow_Claw:
		case MoveName::Shadow_Force:
		case MoveName::Shadow_Punch:
		case MoveName::Shadow_Sneak:
		case MoveName::Shattered_Psyche:
		case MoveName::Sheer_Cold:
		case MoveName::Shell_Side_Arm:
		case MoveName::Shock_Wave:
		case MoveName::Signal_Beam:
		case MoveName::Silver_Wind:
		case MoveName::Simple_Beam:
		case MoveName::Sing:
		case MoveName::Sinister_Arrow_Raid:
		case MoveName::Sketch:
		case MoveName::Skill_Swap:
		case MoveName::Skitter_Smack:
		case MoveName::Skull_Bash:
		case MoveName::Sky_Uppercut:
		case MoveName::Slam:
		case MoveName::Slash:
		case MoveName::Sleep_Powder:
		case MoveName::Sludge:
		case MoveName::Sludge_Bomb:
		case MoveName::Smack_Down:
		case MoveName::Smart_Strike:
		case MoveName::Smelling_Salts:
		case MoveName::Smog:
		case MoveName::Smokescreen:
		case MoveName::Snap_Trap:
		case MoveName::Snipe_Shot:
		case MoveName::Snore:
		case MoveName::Soak:
		case MoveName::Solar_Beam:
		case MoveName::Solar_Blade:
		case MoveName::Sonic_Boom:
		case MoveName::Soul_Stealing_7_Star_Strike:
		case MoveName::Spacial_Rend:
		case MoveName::Spark:
		case MoveName::Spectral_Thief:
		case MoveName::Speed_Swap:
		case MoveName::Spicy_Extract:
		case MoveName::Spider_Web:
		case MoveName::Spike_Cannon:
		case MoveName::Spin_Out:
		case MoveName::Spirit_Break:
		case MoveName::Spirit_Shackle:
		case MoveName::Spit_Up:
		case MoveName::Spite:
		case MoveName::Splintered_Stormshards:
		case MoveName::Spore:
		case MoveName::Spotlight:
		case MoveName::Steam_Eruption:
		case MoveName::Steamroller:
		case MoveName::Steel_Beam:
		case MoveName::Steel_Roller:
		case MoveName::Steel_Wing:
		case MoveName::Stoked_Sparksurfer:
		case MoveName::Stomp:
		case MoveName::Stomping_Tantrum:
		case MoveName::Stone_Edge:
		case MoveName::Stored_Power:
		case MoveName::Storm_Throw:
		case MoveName::Strange_Steam:
		case MoveName::Strength:
		case MoveName::Strength_Sap:
		case MoveName::Stun_Spore:
		case MoveName::Submission:
		case MoveName::Subzero_Slammer:
		case MoveName::Sucker_Punch:
		case MoveName::Sunsteel_Strike:
		case MoveName::Super_Fang:
		case MoveName::Superpower:
		case MoveName::Supersonic:
		case MoveName::Supersonic_Skystrike:
		case MoveName::Surging_Strikes:
		case MoveName::Swagger:
		case MoveName::Sweet_Kiss:
		case MoveName::Switcheroo:
		case MoveName::Syrup_Bomb:
		case MoveName::Tachyon_Cutter:
		case MoveName::Tackle:
		case MoveName::Tail_Slap:
		case MoveName::Take_Down:
		case MoveName::Tar_Shot:
		case MoveName::Taunt:
		case MoveName::Tearful_Look:
		case MoveName::Techno_Blast:
		case MoveName::Tectonic_Rage:
		case MoveName::Telekinesis:
		case MoveName::Tera_Blast:
		case MoveName::Terrain_Pulse:
		case MoveName::Thief:
		case MoveName::Throat_Chop:
		case MoveName::Thunder:
		case MoveName::Thunder_Cage:
		case MoveName::Thunder_Fang:
		case MoveName::Thunder_Punch:
		case MoveName::Thunder_Shock:
		case MoveName::Thunder_Wave:
		case MoveName::Thunderbolt:
		case MoveName::Thunderclap:
		case MoveName::Thunderous_Kick:
		case MoveName::Tickle:
		case MoveName::Topsy_Turvy:
		case MoveName::Torch_Song:
		case MoveName::Torment:
		case MoveName::Toxic:
		case MoveName::Toxic_Thread:
		case MoveName::Trailblaze:
		case MoveName::Transform:
		case MoveName::Tri_Attack:
		case MoveName::Trick:
		case MoveName::Trick_or_Treat:
		case MoveName::Triple_Axel:
		case MoveName::Triple_Dive:
		case MoveName::Triple_Kick:
		case MoveName::Trop_Kick:
		case MoveName::Trump_Card:
		case MoveName::Twin_Beam:
		case MoveName::Twineedle:
		case MoveName::Twinkle_Tackle:
		case MoveName::U_turn:
		case MoveName::Upper_Hand:
		case MoveName::V_create:
		case MoveName::Vacuum_Wave:
		case MoveName::Venoshock:
		case MoveName::Vine_Whip:
		case MoveName::Vise_Grip:
		case MoveName::Vital_Throw:
		case MoveName::Volt_Switch:
		case MoveName::Volt_Tackle:
		case MoveName::Wake_Up_Slap:
		case MoveName::Water_Gun:
		case MoveName::Water_Pledge:
		case MoveName::Water_Shuriken:
		case MoveName::Waterfall:
		case MoveName::Weather_Ball:
		case MoveName::Whirlpool:
		case MoveName::Whirlwind:
		case MoveName::Wicked_Blow:
		case MoveName::Wicked_Torque:
		case MoveName::Wild_Charge:
		case MoveName::Will_O_Wisp:
		case MoveName::Wood_Hammer:
		case MoveName::Worry_Seed:
		case MoveName::Wrap:
		case MoveName::Wring_Out:
		case MoveName::X_Scissor:
		case MoveName::Yawn:
		case MoveName::Zap_Cannon:
		case MoveName::Zen_Headbutt:
		case MoveName::Zing_Zap:
			return Target::adjacent;
	}
}

export constexpr auto move_targets_foe(Generation const generation, MoveName const move) {
	switch (move_target(generation, move)) {
		case Target::user:
		case Target::adjacent_ally:
		case Target::user_or_adjacent_ally:
		case Target::all_allies:
		case Target::user_and_all_allies:
		case Target::user_team:
		case Target::user_field:
		case Target::foe_field:
		case Target::field:
			return false;
		case Target::adjacent_foe:
		case Target::all_adjacent_foes:
		case Target::adjacent:
		case Target::any:
		case Target::all_adjacent:
		case Target::all:
			return true;
	}
}

} // namespace technicalmachine
