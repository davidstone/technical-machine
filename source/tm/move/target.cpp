// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/move/target.hpp>

#include <tm/move/moves.hpp>

#include <tm/generation.hpp>

namespace technicalmachine {

auto move_target(Generation const generation, Moves const move) -> Target {
	switch (move) {
		case Moves::Mist:
			return generation <= Generation::two ? Target::user : Target::user_field;
		case Moves::Helping_Hand:
			return generation <= Generation::three ? Target::user : Target::adjacent_ally;
		case Moves::Surf:
			return generation <= Generation::three ? Target::all_adjacent_foes : Target::all_adjacent;
		case Moves::Conversion_2:
			return generation <= Generation::four ? Target::user : Target::adjacent;
		case Moves::Poison_Gas:
			return generation <= Generation::four ? Target::any : Target::all_adjacent_foes;
		case Moves::Cotton_Spore:
			return generation <= Generation::five ? Target::adjacent : Target::all_adjacent_foes;
		case Moves::Nature_Power:
			return generation <= Generation::five ? Target::user : Target::adjacent;
		case Moves::Howl:
			return generation <= Generation::seven ? Target::user : Target::all_allies;
		case Moves::Aromatherapy:
		case Moves::Heal_Bell:
			return Target::user_team;
		case Moves::Gear_Up:
		case Moves::Magnetic_Flux:
			return Target::all_allies;
		case Moves::Aurora_Veil:
		case Moves::Crafty_Shield:
		case Moves::Happy_Hour:
		case Moves::Light_Screen:
		case Moves::Lucky_Chant:
		case Moves::Mat_Block:
		case Moves::Quick_Guard:
		case Moves::Reflect:
		case Moves::Safeguard:
		case Moves::Tailwind:
		case Moves::Wide_Guard:
			return Target::user_field;
		case Moves::Electric_Terrain:
		case Moves::Fairy_Lock:
		case Moves::Grassy_Terrain:
		case Moves::Gravity:
		case Moves::Hail:
		case Moves::Ion_Deluge:
		case Moves::Magic_Room:
		case Moves::Misty_Terrain:
		case Moves::Psychic_Terrain:
		case Moves::Rain_Dance:
		case Moves::Sandstorm:
		case Moves::Sunny_Day:
		case Moves::Trick_Room:
		case Moves::Wonder_Room:
			return Target::field;
		case Moves::Flower_Shield:
		case Moves::Haze:
		case Moves::Mud_Sport:
		case Moves::Perish_Song:
		case Moves::Rototiller:
		case Moves::Water_Sport:
			return Target::all;
		case Moves::Acrobatics:
		case Moves::Aerial_Ace:
		case Moves::Aeroblast:
		case Moves::Air_Slash:
		case Moves::Aura_Sphere:
		case Moves::Bounce:
		case Moves::Brave_Bird:
		case Moves::Chatter:
		case Moves::Dark_Pulse:
		case Moves::Dragon_Ascent:
		case Moves::Dragon_Pulse:
		case Moves::Drill_Peck:
		case Moves::Fly:
		case Moves::Flying_Press:
		case Moves::Gust:
		case Moves::Heal_Pulse:
		case Moves::Hurricane:
		case Moves::Oblivion_Wing:
		case Moves::Peck:
		case Moves::Pluck:
		case Moves::Sky_Attack:
		case Moves::Sky_Drop:
		case Moves::Water_Pulse:
		case Moves::Wing_Attack:
			return Target::any;
		case Moves::Aromatic_Mist:
		case Moves::Hold_Hands:
			return Target::adjacent_ally;
		case Moves::Acupressure:
			return Target::user_or_adjacent_ally;
		case Moves::Spikes:
		case Moves::Stealth_Rock:
		case Moves::Sticky_Web:
		case Moves::Toxic_Spikes:
			return Target::foe_field;
		case Moves::Boomburst:
		case Moves::Brutal_Swing:
		case Moves::Bulldoze:
		case Moves::Discharge:
		case Moves::Earthquake:
		case Moves::Explosion:
		case Moves::Lava_Plume:
		case Moves::Magnitude:
		case Moves::Mind_Blown:
		case Moves::Parabolic_Charge:
		case Moves::Petal_Blizzard:
		case Moves::Searing_Shot:
		case Moves::Self_Destruct:
		case Moves::Sludge_Wave:
		case Moves::Sparkling_Aria:
		case Moves::Synchronoise:
		case Moves::Teeter_Dance:
			return Target::all_adjacent;
		case Moves::Acid:
		case Moves::Air_Cutter:
		case Moves::Blizzard:
		case Moves::Bouncy_Bubble:
		case Moves::Bubble:
		case Moves::Captivate:
		case Moves::Clanging_Scales:
		case Moves::Clangorous_Soulblaze:
		case Moves::Core_Enforcer:
		case Moves::Dark_Void:
		case Moves::Dazzling_Gleam:
		case Moves::Diamond_Storm:
		case Moves::Disarming_Voice:
		case Moves::Electroweb:
		case Moves::Eruption:
		case Moves::Glaciate:
		case Moves::Growl:
		case Moves::Heal_Block:
		case Moves::Heat_Wave:
		case Moves::Hyper_Voice:
		case Moves::Icy_Wind:
		case Moves::Incinerate:
		case Moves::Lands_Wrath:
		case Moves::Leer:
		case Moves::Muddy_Water:
		case Moves::Origin_Pulse:
		case Moves::Powder_Snow:
		case Moves::Precipice_Blades:
		case Moves::Razor_Leaf:
		case Moves::Razor_Wind:
		case Moves::Relic_Song:
		case Moves::Rock_Slide:
		case Moves::Shell_Trap:
		case Moves::Snarl:
		case Moves::Splishy_Splash:
		case Moves::String_Shot:
		case Moves::Struggle_Bug:
		case Moves::Sweet_Scent:
		case Moves::Swift:
		case Moves::Tail_Whip:
		case Moves::Thousand_Arrows:
		case Moves::Thousand_Waves:
		case Moves::Twister:
		case Moves::Venom_Drench:
		case Moves::Water_Spout:
			return Target::all_adjacent_foes;
		case Moves::Me_First:
		case Moves::Struggle:
			return Target::adjacent_foe;
		case Moves::Acid_Armor:
		case Moves::Agility:
		case Moves::Ally_Switch:
		case Moves::Amnesia:
		case Moves::Aqua_Ring:
		case Moves::Assist:
		case Moves::Autotomize:
		case Moves::Baneful_Bunker:
		case Moves::Barrier:
		case Moves::Baton_Pass:
		case Moves::Belly_Drum:
		case Moves::Bide:
		case Moves::Bulk_Up:
		case Moves::Calm_Mind:
		case Moves::Camouflage:
		case Moves::Celebrate:
		case Moves::Charge:
		case Moves::Coil:
		case Moves::Conversion:
		case Moves::Copycat:
		case Moves::Cosmic_Power:
		case Moves::Cotton_Guard:
		case Moves::Counter:
		case Moves::Curse:
		case Moves::Defend_Order:
		case Moves::Defense_Curl:
		case Moves::Destiny_Bond:
		case Moves::Detect:
		case Moves::Double_Team:
		case Moves::Dragon_Dance:
		case Moves::Endure:
		case Moves::Extreme_Evoboost:
		case Moves::Focus_Energy:
		case Moves::Follow_Me:
		case Moves::Geomancy:
		case Moves::Growth:
		case Moves::Grudge:
		case Moves::Harden:
		case Moves::Heal_Order:
		case Moves::Healing_Wish:
		case Moves::Hit_Self:
		case Moves::Hone_Claws:
		case Moves::Imprison:
		case Moves::Ingrain:
		case Moves::Iron_Defense:
		case Moves::Kings_Shield:
		case Moves::Laser_Focus:
		case Moves::Lunar_Dance:
		case Moves::Magic_Coat:
		case Moves::Magnet_Rise:
		case Moves::Meditate:
		case Moves::Metal_Burst:
		case Moves::Metronome:
		case Moves::Milk_Drink:
		case Moves::Minimize:
		case Moves::Mirror_Coat:
		case Moves::Moonlight:
		case Moves::Morning_Sun:
		case Moves::Nasty_Plot:
		case Moves::Outrage:
		case Moves::Pass:
		case Moves::Petal_Dance:
		case Moves::Power_Trick:
		case Moves::Protect:
		case Moves::Quiver_Dance:
		case Moves::Rage_Powder:
		case Moves::Recover:
		case Moves::Recycle:
		case Moves::Refresh:
		case Moves::Rest:
		case Moves::Rock_Polish:
		case Moves::Roost:
		case Moves::Sharpen:
		case Moves::Shell_Smash:
		case Moves::Shift_Gear:
		case Moves::Shore_Up:
		case Moves::Slack_Off:
		case Moves::Sleep_Talk:
		case Moves::Snatch:
		case Moves::Soft_Boiled:
		case Moves::Spiky_Shield:
		case Moves::Splash:
		case Moves::Stockpile:
		case Moves::Substitute:
		case Moves::Swallow:
		case Moves::Switch0:
		case Moves::Switch1:
		case Moves::Switch2:
		case Moves::Switch3:
		case Moves::Switch4:
		case Moves::Switch5:
		case Moves::Swords_Dance:
		case Moves::Synthesis:
		case Moves::Tail_Glow:
		case Moves::Teleport:
		case Moves::Thrash:
		case Moves::Uproar:
		case Moves::Wish:
		case Moves::Withdraw:
		case Moves::Work_Up:
			return Target::user;
		case Moves::m10000000_Volt_Thunderbolt:
		case Moves::Absorb:
		case Moves::Accelerock:
		case Moves::Acid_Downpour:
		case Moves::Acid_Spray:
		case Moves::After_You:
		case Moves::All_Out_Pummeling:
		case Moves::Anchor_Shot:
		case Moves::Ancient_Power:
		case Moves::Aqua_Jet:
		case Moves::Aqua_Tail:
		case Moves::Arm_Thrust:
		case Moves::Assurance:
		case Moves::Astonish:
		case Moves::Attack_Order:
		case Moves::Attract:
		case Moves::Aurora_Beam:
		case Moves::Avalanche:
		case Moves::Baby_Doll_Eyes:
		case Moves::Baddy_Bad:
		case Moves::Barrage:
		case Moves::Beak_Blast:
		case Moves::Beat_Up:
		case Moves::Belch:
		case Moves::Bestow:
		case Moves::Bind:
		case Moves::Bite:
		case Moves::Black_Hole_Eclipse:
		case Moves::Blast_Burn:
		case Moves::Blaze_Kick:
		case Moves::Block:
		case Moves::Bloom_Doom:
		case Moves::Blue_Flare:
		case Moves::Body_Slam:
		case Moves::Bolt_Strike:
		case Moves::Bone_Club:
		case Moves::Bone_Rush:
		case Moves::Bonemerang:
		case Moves::Breakneck_Blitz:
		case Moves::Brick_Break:
		case Moves::Brine:
		case Moves::Bubble_Beam:
		case Moves::Bug_Bite:
		case Moves::Bug_Buzz:
		case Moves::Bullet_Punch:
		case Moves::Bullet_Seed:
		case Moves::Burn_Up:
		case Moves::Buzzy_Buzz:
		case Moves::Catastropika:
		case Moves::Charge_Beam:
		case Moves::Charm:
		case Moves::Chip_Away:
		case Moves::Circle_Throw:
		case Moves::Clamp:
		case Moves::Clear_Smog:
		case Moves::Close_Combat:
		case Moves::Comet_Punch:
		case Moves::Confide:
		case Moves::Confuse_Ray:
		case Moves::Confusion:
		case Moves::Constrict:
		case Moves::Continental_Crush:
		case Moves::Corkscrew_Crash:
		case Moves::Covet:
		case Moves::Crabhammer:
		case Moves::Cross_Chop:
		case Moves::Cross_Poison:
		case Moves::Crunch:
		case Moves::Crush_Claw:
		case Moves::Crush_Grip:
		case Moves::Cut:
		case Moves::Darkest_Lariat:
		case Moves::Defog:
		case Moves::Devastating_Drake:
		case Moves::Dig:
		case Moves::Disable:
		case Moves::Dive:
		case Moves::Dizzy_Punch:
		case Moves::Doom_Desire:
		case Moves::Double_Hit:
		case Moves::Double_Iron_Bash:
		case Moves::Double_Kick:
		case Moves::Double_Slap:
		case Moves::Double_Edge:
		case Moves::Draco_Meteor:
		case Moves::Dragon_Breath:
		case Moves::Dragon_Claw:
		case Moves::Dragon_Hammer:
		case Moves::Dragon_Rage:
		case Moves::Dragon_Rush:
		case Moves::Dragon_Tail:
		case Moves::Drain_Punch:
		case Moves::Draining_Kiss:
		case Moves::Dream_Eater:
		case Moves::Drill_Run:
		case Moves::Dual_Chop:
		case Moves::Dynamic_Punch:
		case Moves::Earth_Power:
		case Moves::Echoed_Voice:
		case Moves::Eerie_Impulse:
		case Moves::Egg_Bomb:
		case Moves::Electrify:
		case Moves::Electro_Ball:
		case Moves::Embargo:
		case Moves::Ember:
		case Moves::Encore:
		case Moves::Endeavor:
		case Moves::Energy_Ball:
		case Moves::Entrainment:
		case Moves::Extrasensory:
		case Moves::Extreme_Speed:
		case Moves::Facade:
		case Moves::Fairy_Wind:
		case Moves::Fake_Out:
		case Moves::Fake_Tears:
		case Moves::False_Swipe:
		case Moves::Feather_Dance:
		case Moves::Feint:
		case Moves::Feint_Attack:
		case Moves::Fell_Stinger:
		case Moves::Fiery_Dance:
		case Moves::Final_Gambit:
		case Moves::Fire_Blast:
		case Moves::Fire_Fang:
		case Moves::Fire_Lash:
		case Moves::Fire_Pledge:
		case Moves::Fire_Punch:
		case Moves::Fire_Spin:
		case Moves::First_Impression:
		case Moves::Fissure:
		case Moves::Flail:
		case Moves::Flame_Burst:
		case Moves::Flame_Charge:
		case Moves::Flame_Wheel:
		case Moves::Flamethrower:
		case Moves::Flare_Blitz:
		case Moves::Flash:
		case Moves::Flash_Cannon:
		case Moves::Flatter:
		case Moves::Fleur_Cannon:
		case Moves::Fling:
		case Moves::Floaty_Fall:
		case Moves::Floral_Healing:
		case Moves::Focus_Blast:
		case Moves::Focus_Punch:
		case Moves::Force_Palm:
		case Moves::Foresight:
		case Moves::Forests_Curse:
		case Moves::Foul_Play:
		case Moves::Freeze_Shock:
		case Moves::Freeze_Dry:
		case Moves::Freezy_Frost:
		case Moves::Frenzy_Plant:
		case Moves::Frost_Breath:
		case Moves::Frustration:
		case Moves::Fury_Attack:
		case Moves::Fury_Cutter:
		case Moves::Fury_Swipes:
		case Moves::Fusion_Bolt:
		case Moves::Fusion_Flare:
		case Moves::Future_Sight:
		case Moves::Gastro_Acid:
		case Moves::Gear_Grind:
		case Moves::Genesis_Supernova:
		case Moves::Giga_Drain:
		case Moves::Giga_Impact:
		case Moves::Gigavolt_Havoc:
		case Moves::Glare:
		case Moves::Glitzy_Glow:
		case Moves::Grass_Knot:
		case Moves::Grass_Pledge:
		case Moves::Grass_Whistle:
		case Moves::Guard_Split:
		case Moves::Guard_Swap:
		case Moves::Guardian_of_Alola:
		case Moves::Guillotine:
		case Moves::Gunk_Shot:
		case Moves::Gyro_Ball:
		case Moves::Hammer_Arm:
		case Moves::Head_Charge:
		case Moves::Head_Smash:
		case Moves::Headbutt:
		case Moves::Heart_Stamp:
		case Moves::Heart_Swap:
		case Moves::Heat_Crash:
		case Moves::Heavy_Slam:
		case Moves::Hex:
		case Moves::Hidden_Power:
		case Moves::High_Horsepower:
		case Moves::High_Jump_Kick:
		case Moves::Hold_Back:
		case Moves::Horn_Attack:
		case Moves::Horn_Drill:
		case Moves::Horn_Leech:
		case Moves::Hydro_Cannon:
		case Moves::Hydro_Pump:
		case Moves::Hydro_Vortex:
		case Moves::Hyper_Beam:
		case Moves::Hyper_Fang:
		case Moves::Hyperspace_Fury:
		case Moves::Hyperspace_Hole:
		case Moves::Hypnosis:
		case Moves::Ice_Ball:
		case Moves::Ice_Beam:
		case Moves::Ice_Burn:
		case Moves::Ice_Fang:
		case Moves::Ice_Hammer:
		case Moves::Ice_Punch:
		case Moves::Ice_Shard:
		case Moves::Icicle_Crash:
		case Moves::Icicle_Spear:
		case Moves::Inferno:
		case Moves::Inferno_Overdrive:
		case Moves::Infestation:
		case Moves::Instruct:
		case Moves::Iron_Head:
		case Moves::Iron_Tail:
		case Moves::Judgment:
		case Moves::Jump_Kick:
		case Moves::Karate_Chop:
		case Moves::Kinesis:
		case Moves::Knock_Off:
		case Moves::Last_Resort:
		case Moves::Leaf_Blade:
		case Moves::Leaf_Storm:
		case Moves::Leaf_Tornado:
		case Moves::Leafage:
		case Moves::Leech_Life:
		case Moves::Leech_Seed:
		case Moves::Lets_Snuggle_Forever:
		case Moves::Lick:
		case Moves::Light_of_Ruin:
		case Moves::Light_That_Burns_the_Sky:
		case Moves::Liquidation:
		case Moves::Lock_On:
		case Moves::Lovely_Kiss:
		case Moves::Low_Kick:
		case Moves::Low_Sweep:
		case Moves::Lunge:
		case Moves::Luster_Purge:
		case Moves::Mach_Punch:
		case Moves::Magical_Leaf:
		case Moves::Magma_Storm:
		case Moves::Magnet_Bomb:
		case Moves::Malicious_Moonsault:
		case Moves::Mean_Look:
		case Moves::Mega_Drain:
		case Moves::Mega_Kick:
		case Moves::Mega_Punch:
		case Moves::Megahorn:
		case Moves::Memento:
		case Moves::Menacing_Moonraze_Maelstrom:
		case Moves::Metal_Claw:
		case Moves::Metal_Sound:
		case Moves::Meteor_Mash:
		case Moves::Mimic:
		case Moves::Mind_Reader:
		case Moves::Miracle_Eye:
		case Moves::Mirror_Move:
		case Moves::Mirror_Shot:
		case Moves::Mist_Ball:
		case Moves::Moonblast:
		case Moves::Moongeist_Beam:
		case Moves::Mud_Bomb:
		case Moves::Mud_Shot:
		case Moves::Mud_Slap:
		case Moves::Multi_Attack:
		case Moves::Mystical_Fire:
		case Moves::Natural_Gift:
		case Moves::Natures_Madness:
		case Moves::Needle_Arm:
		case Moves::Never_Ending_Nightmare:
		case Moves::Night_Daze:
		case Moves::Night_Shade:
		case Moves::Night_Slash:
		case Moves::Nightmare:
		case Moves::Noble_Roar:
		case Moves::Nuzzle:
		case Moves::Oceanic_Operetta:
		case Moves::Octazooka:
		case Moves::Odor_Sleuth:
		case Moves::Ominous_Wind:
		case Moves::Overheat:
		case Moves::Pain_Split:
		case Moves::Parting_Shot:
		case Moves::Pay_Day:
		case Moves::Payback:
		case Moves::Phantom_Force:
		case Moves::Photon_Geyser:
		case Moves::Pika_Papow:
		case Moves::Pin_Missile:
		case Moves::Plasma_Fists:
		case Moves::Play_Nice:
		case Moves::Play_Rough:
		case Moves::Poison_Fang:
		case Moves::Poison_Jab:
		case Moves::Poison_Powder:
		case Moves::Poison_Sting:
		case Moves::Poison_Tail:
		case Moves::Pollen_Puff:
		case Moves::Pound:
		case Moves::Powder:
		case Moves::Power_Gem:
		case Moves::Power_Split:
		case Moves::Power_Swap:
		case Moves::Power_Trip:
		case Moves::Power_Whip:
		case Moves::Power_Up_Punch:
		case Moves::Present:
		case Moves::Prismatic_Laser:
		case Moves::Psybeam:
		case Moves::Psych_Up:
		case Moves::Psychic:
		case Moves::Psychic_Fangs:
		case Moves::Psycho_Boost:
		case Moves::Psycho_Cut:
		case Moves::Psycho_Shift:
		case Moves::Psyshock:
		case Moves::Psystrike:
		case Moves::Psywave:
		case Moves::Pulverizing_Pancake:
		case Moves::Punishment:
		case Moves::Purify:
		case Moves::Pursuit:
		case Moves::Quash:
		case Moves::Quick_Attack:
		case Moves::Rage:
		case Moves::Rapid_Spin:
		case Moves::Razor_Shell:
		case Moves::Reflect_Type:
		case Moves::Retaliate:
		case Moves::Return:
		case Moves::Revelation_Dance:
		case Moves::Revenge:
		case Moves::Reversal:
		case Moves::Roar:
		case Moves::Roar_of_Time:
		case Moves::Rock_Blast:
		case Moves::Rock_Climb:
		case Moves::Rock_Smash:
		case Moves::Rock_Throw:
		case Moves::Rock_Tomb:
		case Moves::Rock_Wrecker:
		case Moves::Role_Play:
		case Moves::Rolling_Kick:
		case Moves::Rollout:
		case Moves::Round:
		case Moves::Sacred_Fire:
		case Moves::Sacred_Sword:
		case Moves::Sand_Attack:
		case Moves::Sand_Tomb:
		case Moves::Sappy_Seed:
		case Moves::Savage_Spin_Out:
		case Moves::Scald:
		case Moves::Scary_Face:
		case Moves::Scratch:
		case Moves::Screech:
		case Moves::Searing_Sunraze_Smash:
		case Moves::Secret_Power:
		case Moves::Secret_Sword:
		case Moves::Seed_Bomb:
		case Moves::Seed_Flare:
		case Moves::Seismic_Toss:
		case Moves::Shadow_Ball:
		case Moves::Shadow_Bone:
		case Moves::Shadow_Claw:
		case Moves::Shadow_Force:
		case Moves::Shadow_Punch:
		case Moves::Shadow_Sneak:
		case Moves::Shattered_Psyche:
		case Moves::Sheer_Cold:
		case Moves::Shock_Wave:
		case Moves::Signal_Beam:
		case Moves::Silver_Wind:
		case Moves::Simple_Beam:
		case Moves::Sing:
		case Moves::Sinister_Arrow_Raid:
		case Moves::Sizzly_Slide:
		case Moves::Sketch:
		case Moves::Skill_Swap:
		case Moves::Skull_Bash:
		case Moves::Sky_Uppercut:
		case Moves::Slam:
		case Moves::Slash:
		case Moves::Sleep_Powder:
		case Moves::Sludge:
		case Moves::Sludge_Bomb:
		case Moves::Smack_Down:
		case Moves::Smart_Strike:
		case Moves::Smelling_Salts:
		case Moves::Smog:
		case Moves::Smokescreen:
		case Moves::Snore:
		case Moves::Soak:
		case Moves::Solar_Beam:
		case Moves::Solar_Blade:
		case Moves::Sonic_Boom:
		case Moves::Soul_Stealing_7_Star_Strike:
		case Moves::Spacial_Rend:
		case Moves::Spark:
		case Moves::Sparkly_Swirl:
		case Moves::Spectral_Thief:
		case Moves::Speed_Swap:
		case Moves::Spider_Web:
		case Moves::Spike_Cannon:
		case Moves::Spirit_Shackle:
		case Moves::Spit_Up:
		case Moves::Spite:
		case Moves::Splintered_Stormshards:
		case Moves::Spore:
		case Moves::Spotlight:
		case Moves::Steam_Eruption:
		case Moves::Steamroller:
		case Moves::Steel_Wing:
		case Moves::Stoked_Sparksurfer:
		case Moves::Stomp:
		case Moves::Stomping_Tantrum:
		case Moves::Stone_Edge:
		case Moves::Stored_Power:
		case Moves::Storm_Throw:
		case Moves::Strength:
		case Moves::Strength_Sap:
		case Moves::Stun_Spore:
		case Moves::Submission:
		case Moves::Subzero_Slammer:
		case Moves::Sucker_Punch:
		case Moves::Sunsteel_Strike:
		case Moves::Super_Fang:
		case Moves::Superpower:
		case Moves::Supersonic:
		case Moves::Supersonic_Skystrike:
		case Moves::Swagger:
		case Moves::Sweet_Kiss:
		case Moves::Switcheroo:
		case Moves::Tackle:
		case Moves::Tail_Slap:
		case Moves::Take_Down:
		case Moves::Taunt:
		case Moves::Tearful_Look:
		case Moves::Techno_Blast:
		case Moves::Tectonic_Rage:
		case Moves::Telekinesis:
		case Moves::Thief:
		case Moves::Throat_Chop:
		case Moves::Thunder:
		case Moves::Thunder_Fang:
		case Moves::Thunder_Punch:
		case Moves::Thunder_Shock:
		case Moves::Thunder_Wave:
		case Moves::Thunderbolt:
		case Moves::Tickle:
		case Moves::Topsy_Turvy:
		case Moves::Torment:
		case Moves::Toxic:
		case Moves::Toxic_Thread:
		case Moves::Transform:
		case Moves::Tri_Attack:
		case Moves::Trick:
		case Moves::Trick_or_Treat:
		case Moves::Triple_Kick:
		case Moves::Trop_Kick:
		case Moves::Trump_Card:
		case Moves::Twineedle:
		case Moves::Twinkle_Tackle:
		case Moves::U_turn:
		case Moves::V_create:
		case Moves::Vacuum_Wave:
		case Moves::Veevee_Volley:
		case Moves::Venoshock:
		case Moves::Vice_Grip:
		case Moves::Vine_Whip:
		case Moves::Vital_Throw:
		case Moves::Volt_Switch:
		case Moves::Volt_Tackle:
		case Moves::Wake_Up_Slap:
		case Moves::Water_Gun:
		case Moves::Water_Pledge:
		case Moves::Water_Shuriken:
		case Moves::Waterfall:
		case Moves::Weather_Ball:
		case Moves::Whirlpool:
		case Moves::Whirlwind:
		case Moves::Wild_Charge:
		case Moves::Will_O_Wisp:
		case Moves::Wood_Hammer:
		case Moves::Worry_Seed:
		case Moves::Wrap:
		case Moves::Wring_Out:
		case Moves::X_Scissor:
		case Moves::Yawn:
		case Moves::Zap_Cannon:
		case Moves::Zen_Headbutt:
		case Moves::Zing_Zap:
		case Moves::Zippy_Zap:
			return Target::adjacent;
	}
}

} // namespace technicalmachine
