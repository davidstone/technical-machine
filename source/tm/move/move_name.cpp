// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.move_name;

import numeric_traits;
import std_module;

namespace technicalmachine {

export enum class MoveName : std::uint16_t {
	// Generation 1
	Pound = 6,
	Karate_Chop,
	Double_Slap,
	Comet_Punch,
	Mega_Punch,
	Pay_Day,
	Fire_Punch,
	Ice_Punch,
	Thunder_Punch,
	Scratch,
	Vise_Grip,
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
	Sonic_Boom,
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
	Bubble_Beam,
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
	Solar_Beam,
	Poison_Powder,
	Stun_Spore,
	Sleep_Powder,
	Petal_Dance,
	String_Shot,
	Dragon_Rage,
	Fire_Spin,
	Thunder_Shock,
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
	Smokescreen,
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
	Self_Destruct,
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
	Soft_Boiled,
	High_Jump_Kick,
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
	Feint_Attack,
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
	Dynamic_Punch,
	Megahorn,
	Dragon_Breath,
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
	Extreme_Speed,
	Ancient_Power,
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
	Smelling_Salts,
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
	Feather_Dance,
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
	Grass_Whistle,
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
	Fusion_Bolt,

	// Generation 6
	Flying_Press,
	Mat_Block,
	Belch,
	Rototiller,
	Sticky_Web,
	Fell_Stinger,
	Phantom_Force,
	Trick_or_Treat,
	Noble_Roar,
	Ion_Deluge,
	Parabolic_Charge,
	Forests_Curse,
	Petal_Blizzard,
	Freeze_Dry,
	Disarming_Voice,
	Parting_Shot,
	Topsy_Turvy,
	Draining_Kiss,
	Crafty_Shield,
	Flower_Shield,
	Grassy_Terrain,
	Misty_Terrain,
	Electrify,
	Play_Rough,
	Fairy_Wind,
	Moonblast,
	Boomburst,
	Fairy_Lock,
	Kings_Shield,
	Play_Nice,
	Confide,
	Diamond_Storm,
	Steam_Eruption,
	Hyperspace_Hole,
	Water_Shuriken,
	Mystical_Fire,
	Spiky_Shield,
	Aromatic_Mist,
	Eerie_Impulse,
	Venom_Drench,
	Powder,
	Geomancy,
	Magnetic_Flux,
	Happy_Hour,
	Electric_Terrain,
	Dazzling_Gleam,
	Celebrate,
	Hold_Hands,
	Baby_Doll_Eyes,
	Nuzzle,
	Hold_Back,
	Infestation,
	Power_Up_Punch,
	Oblivion_Wing,
	Thousand_Arrows,
	Thousand_Waves,
	Lands_Wrath,
	Light_of_Ruin,
	Origin_Pulse,
	Precipice_Blades,
	Dragon_Ascent,
	Hyperspace_Fury,

	// Generation 7
	Breakneck_Blitz,
	All_Out_Pummeling,
	Supersonic_Skystrike,
	Acid_Downpour,
	Tectonic_Rage,
	Continental_Crush,
	Savage_Spin_Out,
	Never_Ending_Nightmare,
	Corkscrew_Crash,
	Inferno_Overdrive,
	Hydro_Vortex,
	Bloom_Doom,
	Gigavolt_Havoc,
	Shattered_Psyche,
	Subzero_Slammer,
	Devastating_Drake,
	Black_Hole_Eclipse,
	Twinkle_Tackle,
	Catastropika,
	Shore_Up,
	First_Impression,
	Baneful_Bunker,
	Spirit_Shackle,
	Darkest_Lariat,
	Sparkling_Aria,
	Ice_Hammer,
	Floral_Healing,
	High_Horsepower,
	Strength_Sap,
	Solar_Blade,
	Leafage,
	Spotlight,
	Toxic_Thread,
	Laser_Focus,
	Gear_Up,
	Throat_Chop,
	Pollen_Puff,
	Anchor_Shot,
	Psychic_Terrain,
	Lunge,
	Fire_Lash,
	Power_Trip,
	Burn_Up,
	Speed_Swap,
	Smart_Strike,
	Purify,
	Revelation_Dance,
	Core_Enforcer,
	Trop_Kick,
	Instruct,
	Beak_Blast,
	Clanging_Scales,
	Dragon_Hammer,
	Brutal_Swing,
	Aurora_Veil,
	Sinister_Arrow_Raid,
	Malicious_Moonsault,
	Oceanic_Operetta,
	Guardian_of_Alola,
	Soul_Stealing_7_Star_Strike,
	Stoked_Sparksurfer,
	Pulverizing_Pancake,
	Extreme_Evoboost,
	Genesis_Supernova,
	Shell_Trap,
	Fleur_Cannon,
	Psychic_Fangs,
	Stomping_Tantrum,
	Shadow_Bone,
	Accelerock,
	Liquidation,
	Prismatic_Laser,
	Spectral_Thief,
	Sunsteel_Strike,
	Moongeist_Beam,
	Tearful_Look,
	Zing_Zap,
	Natures_Madness,
	Multi_Attack,
	m10000000_Volt_Thunderbolt,
	Mind_Blown,
	Plasma_Fists,
	Photon_Geyser,
	Light_That_Burns_the_Sky,
	Searing_Sunraze_Smash,
	Menacing_Moonraze_Maelstrom,
	Lets_Snuggle_Forever,
	Splintered_Stormshards,
	Clangorous_Soulblaze,
	Double_Iron_Bash,

	// Generation 8
	Max_Guard,
	Dynamax_Cannon,
	Snipe_Shot,
	Jaw_Lock,
	Stuff_Cheeks,
	No_Retreat,
	Tar_Shot,
	Magic_Powder,
	Dragon_Darts,
	Teatime,
	Octolock,
	Bolt_Beak,
	Fishious_Rend,
	Court_Change,
	Max_Flare,
	Max_Flutterby,
	Max_Lightning,
	Max_Strike,
	Max_Knuckle,
	Max_Phantasm,
	Max_Hailstorm,
	Max_Ooze,
	Max_Geyser,
	Max_Airstream,
	Max_Starfall,
	Max_Wyrmwind,
	Max_Mindstorm,
	Max_Rockfall,
	Max_Quake,
	Max_Darkness,
	Max_Overgrowth,
	Max_Steelspike,
	Clangorous_Soul,
	Body_Press,
	Decorate,
	Drum_Beating,
	Snap_Trap,
	Pyro_Ball,
	Behemoth_Blade,
	Behemoth_Bash,
	Aura_Wheel,
	Breaking_Swipe,
	Branch_Poke,
	Overdrive,
	Apple_Acid,
	Grav_Apple,
	Spirit_Break,
	Strange_Steam,
	Life_Dew,
	Obstruct,
	False_Surrender,
	Meteor_Assault,
	Eternabeam,
	Steel_Beam,
	Expanding_Force,
	Steel_Roller,
	Scale_Shot,
	Meteor_Beam,
	Shell_Side_Arm,
	Misty_Explosion,
	Grassy_Glide,
	Rising_Voltage,
	Terrain_Pulse,
	Skitter_Smack,
	Burning_Jealousy,
	Lash_Out,
	Poltergeist,
	Corrosive_Gas,
	Coaching,
	Flip_Turn,
	Triple_Axel,
	Dual_Wingbeat,
	Scorching_Sands,
	Jungle_Healing,
	Wicked_Blow,
	Surging_Strikes,
	Thunder_Cage,
	Dragon_Energy,
	Freezing_Glare,
	Fiery_Wrath,
	Thunderous_Kick,
	Glacial_Lance,
	Astral_Barrage,
	Eerie_Spell,

	// Generation 9
	Tera_Blast,
	Silk_Trap,
	Axe_Kick,
	Last_Respects,
	Lumina_Crash,
	Order_Up,
	Jet_Punch,
	Spicy_Extract,
	Spin_Out,
	Population_Bomb,
	Ice_Spinner,
	Glaive_Rush,
	Revival_Blessing,
	Salt_Cure,
	Triple_Dive,
	Mortal_Spin,
	Doodle,
	Fillet_Away,
	Kowtow_Cleave,
	Flower_Trick,
	Torch_Song,
	Aqua_Step,
	Raging_Bull,
	Make_It_Rain,
	Psyblade,
	Hydro_Steam,
	Ruination,
	Collision_Course,
	Electro_Drift,
	Shed_Tail,
	Chilly_Reception,
	Tidy_Up,
	Snowscape,
	Pounce,
	Trailblaze,
	Chilling_Water,
	Hyper_Drill,
	Twin_Beam,
	Rage_Fist,
	Armor_Cannon,
	Bitter_Blade,
	Double_Shock,
	Gigaton_Hammer,
	Comeuppance,
	Aqua_Cutter,
	Blazing_Torque,
	Wicked_Torque,
	Noxious_Torque,
	Combat_Torque,
	Magical_Torque,
	Blood_Moon,
	Matcha_Gotcha,
	Syrup_Bomb,
	Ivy_Cudgel,
	Electro_Shot,
	Thunderclap,
	Tachyon_Cutter,
	Upper_Hand,
	Psychic_Noise,	
};

} // namespace technicalmachine

template<>
constexpr auto numeric_traits::min_value<technicalmachine::MoveName> = technicalmachine::MoveName::Pound;

template<>
constexpr auto numeric_traits::max_value<technicalmachine::MoveName> = technicalmachine::MoveName::Eerie_Spell;

namespace technicalmachine {

export constexpr auto is_regular(MoveName const move) -> bool {
	switch (move) {
		case MoveName::Struggle:
			return false;
		default:
			return true;
	}
}

export constexpr auto is_phaze(MoveName const move) -> bool {
	switch (move) {
		case MoveName::Circle_Throw:
		case MoveName::Dragon_Tail:
		case MoveName::Roar:
		case MoveName::Whirlwind:
			return true;
		default:
			return false;
	}
}

export constexpr auto thaws_user(MoveName const move) -> bool {
	switch (move) {
		case MoveName::Flame_Wheel:
		case MoveName::Pyro_Ball:
		case MoveName::Sacred_Fire:
		case MoveName::Scald:
		case MoveName::Scorching_Sands:
			return true;
		default:
			return false;
	}
}

} // namespace technicalmachine