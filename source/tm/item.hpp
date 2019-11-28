// Copyright (C) 2015 David Stone
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

#include <tm/status.hpp>

#include <bounded/integer.hpp>

#include <cstdint>

namespace technicalmachine {

enum class Item : uint16_t {
	// Generation 1
	None,

	Antidote,
	Awakening,
	Burn_Heal,
	Calcium,
	Carbos,
	Dire_Hit,
	Dome_Fossil,
	Elixir,
	Escape_Rope,
	Ether,
	Exp_Share,
	Fire_Stone,
	Fresh_Water,
	Full_Heal,
	Full_Restore,
	Great_Ball,
	Guard_Spec,
	Helix_Fossil,
	HP_Up,
	Hyper_Potion,
	Ice_Heal,
	Iron,
	Leaf_Stone,
	Lemonade,
	Master_Ball,
	Max_Elixir,
	Max_Ether,
	Max_Potion,
	Max_Repel,
	Max_Revive,
	Moon_Stone,
	Nugget,
	Old_Amber,
	Paralyze_Heal,
	Poke_Ball,
	Poke_Doll,
	Potion,
	PP_Up,
	Protein,
	Rare_Candy,
	Repel,
	Revive,
	Safari_Ball,
	Soda_Pop,
	Super_Potion,
	Super_Repel,
	Thunder_Stone,
	TM01,
	TM02,
	TM03,
	TM04,
	TM05,
	TM06,
	TM07,
	TM08,
	TM09,
	TM10,
	TM11,
	TM12,
	TM13,
	TM14,
	TM15,
	TM16,
	TM17,
	TM18,
	TM19,
	TM20,
	TM21,
	TM22,
	TM23,
	TM24,
	TM25,
	TM26,
	TM27,
	TM28,
	TM29,
	TM30,
	TM31,
	TM32,
	TM33,
	TM34,
	TM35,
	TM36,
	TM37,
	TM38,
	TM39,
	TM40,
	TM41,
	TM42,
	TM43,
	TM44,
	TM45,
	TM46,
	TM47,
	TM48,
	TM49,
	TM50,
	Ultra_Ball,
	Water_Stone,
	X_Accuracy,
	X_Attack,
	X_Defend,
	X_Special,
	X_Speed,
	
	// Generation 2
	Amulet_Coin,
	Berry_Juice,
	Berry,
	Berserk_Gene,
	Big_Mushroom,
	Big_Pearl,
	Bitter_Berry,
	Black_Apricorn,
	Black_Belt,
	Black_Glasses,
	Blue_Apricorn,
	Bluesky_Mail,
	Brick_Piece,
	Bright_Powder,
	Burnt_Berry,
	Charcoal,
	Cleanse_Tag,
	Dragon_Fang,
	Dragon_Scale,
	Energy_Powder,
	Energy_Root,
	Eon_Mail,
	Everstone,
	Fast_Ball,
	Flower_Mail,
	Focus_Band,
	Friend_Ball,
	Gold_Berry,
	Gold_Leaf,
	Gorgeous_Box,
	Green_Apricorn,
	Hard_Stone,
	Heal_Powder,
	Heavy_Ball,
	Ice_Berry,
	Kings_Rock,
	Leftovers,
	Level_Ball,
	Light_Ball,
	Litebluemail,
	Love_Ball,
	Lovely_Mail,
	Lucky_Egg,
	Lucky_Punch,
	Lure_Ball,
	Magnet,
	Metal_Coat,
	Metal_Powder,
	Mint_Berry,
	Miracle_Seed,
	MiracleBerry,
	Mirage_Mail,
	Moomoo_Milk,
	Moon_Ball,
	Morph_Mail,
	Music_Mail,
	MysteryBerry,
	Mystic_Water,
	Never_Melt_Ice,
	Normal_Box,
	Pearl,
	Pink_Apricorn,
	Pink_Bow,
	Poison_Barb,
	Polkadot_Bow,
	Portraitmail,
	PRZCureBerry,
	PSNCureBerry,
	Quick_Claw,
	Rage_Candy_Bar,
	Red_Apricorn,
	Revival_Herb,
	Sacred_Ash,
	Scope_Lens,
	Sharp_Beak,
	Silver_Leaf,
	Silver_Powder,
	Slowpoke_Tail,
	Smoke_Ball,
	Soft_Sand,
	Spell_Tag,
	Sport_Ball,
	Star_Piece,
	Stardust,
	Stick,
	Sun_Stone,
	Surf_Mail,
	Thick_Club,
	Tiny_Mushroom,
	Twisted_Spoon,
	Up_Grade,
	White_Apricorn,
	Yellow_Apricorn,

	// Generation 3
	Aguav_Berry,
	Apicot_Berry,
	Aspear_Berry,
	Bead_Mail,
	Belue_Berry,
	Black_Flute,
	Blue_Flute,
	Blue_Orb,
	Blue_Scarf,
	Blue_Shard,
	Bluk_Berry,
	Cheri_Berry,
	Chesto_Berry,
	Chilan_Berry,
	Choice_Band,
	Claw_Fossil,
	Cornn_Berry,
	Deep_Sea_Scale,
	Deep_Sea_Tooth,
	Dive_Ball,
	Drash_Berry,
	Dream_Mail,
	Durin_Berry,
	Eggant_Berry,
	Enigma_Berry,
	Fab_Mail,
	Figy_Berry,
	Fluffy_Tail,
	Ganlon_Berry,
	Ginema_Berry,
	Glitter_Mail,
	Green_Scarf,
	Green_Shard,
	Grepa_Berry,
	Harbor_Mail,
	Heart_Scale,
	Hondew_Berry,
	Iapapa_Berry,
	Kelpsy_Berry,
	Kuo_Berry,
	Lansat_Berry,
	Lava_Cookie,
	Lax_Incense,
	Leppa_Berry,
	Liechi_Berry,
	Lum_Berry,
	Luxury_Ball,
	Macho_Brace,
	Mago_Berry,
	Magost_Berry,
	Mech_Mail,
	Mental_Herb,
	Nanab_Berry,
	Nest_Ball,
	Net_Ball,
	Niniku_Berry,
	Nomel_Berry,
	Nutpea_Berry,
	Oran_Berry,
	Orange_Mail,
	Pamtre_Berry,
	Pecha_Berry,
	Persim_Berry,
	Petaya_Berry,
	Pinap_Berry,
	Pink_Scarf,
	Pomeg_Berry,
	PP_Max,
	Premier_Ball,
	Pumkin_Berry,
	Qualot_Berry,
	Rabuta_Berry,
	Rawst_Berry,
	Razz_Berry,
	Red_Flute,
	Red_Orb,
	Red_Scarf,
	Red_Shard,
	Repeat_Ball,
	Retro_Mail,
	Root_Fossil,
	Salac_Berry,
	Sea_Incense,
	Shadow_Mail,
	Shell_Bell,
	Shoal_Salt,
	Shoal_Shell,
	Silk_Scarf,
	Sitrus_Berry,
	Soothe_Bell,
	Soul_Dew,
	Spelon_Berry,
	Starf_Berry,
	Strib_Berry,
	Tamato_Berry,
	Timer_Ball,
	Topo_Berry,
	Touga_Berry,
	Tropic_Mail,
	Watmel_Berry,
	Wave_Mail,
	Wepear_Berry,
	White_Flute,
	White_Herb,
	Wiki_Berry,
	Wood_Mail,
	Yago_Berry,
	Yellow_Flute,
	Yellow_Scarf,
	Yellow_Shard,
	Zinc,

	// Generation 4
	Adamant_Orb,
	Air_Mail,
	Armor_Fossil,
	Babiri_Berry,
	Big_Root,
	Black_Sludge,
	Bloom_Mail,
	Brick_Mail,
	Bubble_Mail,
	Charti_Berry,
	Cherish_Ball,
	Choice_Scarf,
	Choice_Specs,
	Chople_Berry,
	Coba_Berry,
	Colbur_Berry,
	Custap_Berry,
	Damp_Mulch,
	Damp_Rock,
	Dawn_Stone,
	Destiny_Knot,
	Draco_Plate,
	Dread_Plate,
	Dubious_Disc,
	Dusk_Ball,
	Dusk_Stone,
	Earth_Plate,
	Electirizer,
	Expert_Belt,
	Fist_Plate,
	Flame_Mail,
	Flame_Orb,
	Flame_Plate,
	Focus_Sash,
	Full_Incense,
	Gooey_Mulch,
	Grass_Mail,
	Grip_Claw,
	Griseous_Orb,
	Growth_Mulch,
	Haban_Berry,
	Heal_Ball,
	Heart_Mail,
	Heat_Rock,
	Honey,
	Icicle_Plate,
	Icy_Rock,
	Insect_Plate,
	Iron_Ball,
	Iron_Plate,
	Jaboca_Berry,
	Kasib_Berry,
	Kebia_Berry,
	Lagging_Tail,
	Life_Orb,
	Light_Clay,
	Luck_Incense,
	Lustrous_Orb,
	Magmarizer,
	Meadow_Plate,
	Metronome,
	Micle_Berry,
	Mind_Plate,
	Mosaic_Mail,
	Muscle_Band,
	Occa_Berry,
	Odd_Incense,
	Odd_Keystone,
	Old_Gateau,
	Oval_Stone,
	Park_Ball,
	Passho_Berry,
	Payapa_Berry,
	Power_Anklet,
	Power_Band,
	Power_Belt,
	Power_Bracer,
	Power_Herb,
	Power_Lens,
	Power_Weight,
	Protector,
	Pure_Incense,
	Quick_Ball,
	Quick_Powder,
	Rare_Bone,
	Razor_Claw,
	Razor_Fang,
	Reaper_Cloth,
	Rindo_Berry,
	Rock_Incense,
	Rose_Incense,
	Rowap_Berry,
	Shed_Shell,
	Shiny_Stone,
	Shuca_Berry,
	Skull_Fossil,
	Sky_Plate,
	Smooth_Rock,
	Snow_Mail,
	Space_Mail,
	Splash_Plate,
	Spooky_Plate,
	Stable_Mulch,
	Steel_Mail,
	Sticky_Barb,
	Stone_Plate,
	Tanga_Berry,
	TM51,
	TM52,
	TM53,
	TM54,
	TM55,
	TM56,
	TM57,
	TM58,
	TM59,
	TM60,
	TM61,
	TM62,
	TM63,
	TM64,
	TM65,
	TM66,
	TM67,
	TM68,
	TM69,
	TM70,
	TM71,
	TM72,
	TM73,
	TM74,
	TM75,
	TM76,
	TM77,
	TM78,
	TM79,
	TM80,
	TM81,
	TM82,
	TM83,
	TM84,
	TM85,
	TM86,
	TM87,
	TM88,
	TM89,
	TM90,
	TM91,
	TM92,
	Toxic_Orb,
	Toxic_Plate,
	Tunnel_Mail,
	Wacan_Berry,
	Wave_Incense,
	Wide_Lens,
	Wise_Glasses,
	X_Sp_Def,
	Yache_Berry,
	Zap_Plate,
	Zoom_Lens,

	// Generation 5
	Absorb_Bulb,
	Air_Balloon,
	Balm_Mushroom,
	Big_Nugget,
	Binding_Band,
	BridgeMail_D,
	BridgeMail_M,
	BridgeMail_S,
	BridgeMail_T,
	BridgeMail_V,
	Bug_Gem,
	Burn_Drive,
	Casteliacone,
	Cell_Battery,
	Chill_Drive,
	Clever_Wing,
	Comet_Shard,
	Cover_Fossil,
	Dark_Gem,
	Douse_Drive,
	Dragon_Gem,
	Dream_Ball,
	Eject_Button,
	Electric_Gem,
	Eviolite,
	Favored_Mail,
	Fighting_Gem,
	Fire_Gem,
	Float_Stone,
	Flying_Gem,
	Genius_Wing,
	Ghost_Gem,
	Grass_Gem,
	Greet_Mail,
	Ground_Gem,
	Health_Wing,
	Ice_Gem,
	Inquiry_Mail,
	Like_Mail,
	Muscle_Wing,
	Normal_Gem,
	Pass_Orb,
	Pearl_String,
	Plume_Fossil,
	Poison_Gem,
	Poke_Toy,
	Pretty_Wing,
	Prism_Scale,
	Psychic_Gem,
	Red_Card,
	Relic_Band,
	Relic_Copper,
	Relic_Crown,
	Relic_Gold,
	Relic_Silver,
	Relic_Statue,
	Relic_Vase,
	Reply_Mail,
	Resist_Wing,
	Ring_Target,
	Rock_Gem,
	Rocky_Helmet,
	RSVP_Mail,
	Shock_Drive,
	Steel_Gem,
	Sweet_Heart,
	Swift_Wing,
	Thanks_Mail,
	Water_Gem,

	// Generation 6
	Ability_Capsule,
	Abomasite,
	Absolite,
	Aerodactylite,
	Aggronite,
	Alakazite,
	Altarianite,
	Amaze_Mulch,
	Ampharosite,
	Assault_Vest,
	Audinite,
	Banettite,
	Beedrillite,
	Blastoisinite,
	Blazikenite,
	Boost_Mulch,
	Cameruptite,
	Charizardite_X,
	Charizardite_Y,
	Diancite,
	Discount_Coupon,
	Fairy_Gem,
	Galladite,
	Garchompite,
	Gardevoirite,
	Gengarite,
	Glalitite,
	Gyaradosite,
	Heracronite,
	Houndoominite,
	Jaw_Fossil,
	Kangaskhanite,
	Kee_Berry,
	Latiasite,
	Latiosite,
	Lopunnite,
	Lucarionite,
	Luminous_Moss,
	Lumiose_Galette,
	Manectite,
	Maranga_Berry,
	Mawilite,
	Medichamite,
	Metagrossite,
	Mewtwonite_X,
	Mewtwonite_Y,
	Pidgeotite,
	Pinsirite,
	Pixie_Plate,
	Rich_Mulch,
	Roseli_Berry,
	Sablenite,
	Sachet,
	Safety_Goggles,
	Sail_Fossil,
	Salamencite,
	Sceptilite,
	Scizorite,
	Shalour_Sable,
	Sharpedonite,
	Slowbronite,
	Snowball,
	Steelixite,
	Strange_Souvenir,
	Surprise_Mulch,
	Swampertite,
	Tyranitarite,
	Venusaurite,
	Weakness_Policy,
	Whipped_Dream,
	X_Defense,
	X_Sp_Atk,

	// Generation 7
	Adrenaline_Orb,
	Aloraichium_Z,
	Beast_Ball,
	Big_Malasada,
	Bottle_Cap,
	Bug_Memory,
	Buginium_Z,
	Dark_Memory,
	Darkinium_Z,
	Decidium_Z,
	Dragon_Memory,
	Dragonium_Z,
	Eevium_Z,
	Electric_Memory,
	Electric_Seed,
	Electrium_Z,
	Fairium_Z,
	Fairy_Memory,
	Festival_Ticket,
	Fighting_Memory,
	Fightinium_Z,
	Fire_Memory,
	Firium_Z,
	Flying_Memory,
	Flyinium_Z,
	Ghost_Memory,
	Ghostium_Z,
	Gold_Bottle_Cap,
	Grass_Memory,
	Grassium_Z,
	Grassy_Seed,
	Ground_Memory,
	Groundium_Z,
	Ice_Memory,
	Ice_Stone,
	Icium_Z,
	Incinium_Z,
	Kommonium_Z,
	Lunalium_Z,
	Lycanium_Z,
	Marshadium_Z,
	Mewnium_Z,
	Mimikium_Z,
	Misty_Seed,
	Normalium_Z,
	Pewter_Crunchies,
	Pikanium_Z,
	Pikashunium_Z,
	Pink_Nectar,
	Poison_Memory,
	Poisonium_Z,
	Primarium_Z,
	Protective_Pads,
	Psychic_Memory,
	Psychic_Seed,
	Psychium_Z,
	Purple_Nectar,
	Rock_Memory,
	Rockium_Z,
	Snorlium_Z,
	Solganium_Z,
	Steel_Memory,
	Steelium_Z,
	Tapunium_Z,
	Terrain_Extender,
	Ultranecrozium_Z,
	Water_Memory,
	Waterium_Z,
	Yellow_Nectar,

	// Generation 8
	Adamant_Mint,
	Bachs_Food_Tin,
	Berry_Sweet,
	Blunder_Policy,
	Bobs_Food_Tin,
	Boiled_Egg,
	Bold_Mint,
	Brave_Mint,
	Bread,
	Brittle_Bones,
	Calm_Mint,
	Careful_Mint,
	Chipped_Pot,
	Clover_Sweet,
	Coconut_Milk,
	Cracked_Pot,
	Eject_Pack,
	Fancy_Apple,
	Flower_Sweet,
	Fresh_Cream,
	Fried_Food,
	Fruit_Bunch,
	Gentle_Mint,
	Gigantamix,
	Hasty_Mint,
	Heavy_Duty_Boots,
	Impish_Mint,
	Instant_Noodles,
	Jolly_Mint,
	Large_Leek,
	Lax_Mint,
	Leek,
	Lonely_Mint,
	Love_Sweet,
	Mild_Mint,
	Mixed_Mushrooms,
	Modest_Mint,
	Moomoo_Cheese,
	Naive_Mint,
	Naughty_Mint,
	Pack_of_Potatoes,
	Packaged_Curry,
	Pasta,
	Precooked_Burger,
	Pungent_Root,
	Quiet_Mint,
	Rash_Mint,
	Relaxed_Mint,
	Ribbon_Sweet,
	Room_Service,
	Salad_Mix,
	Sassy_Mint,
	Sausages,
	Serious_Mint,
	Smoke_Poke_Tail,
	Spice_Mix,
	Star_Sweet,
	Strawberry_Sweet,
	Sweet_Apple,
	Tart_Apple,
	Throat_Spray,
	Timid_Mint,
	Tin_of_Beans,
	Utility_Umbrella,
};

} // namespace technicalmachine
namespace bounded {

template<>
inline constexpr auto min_value<technicalmachine::Item> = technicalmachine::Item();

template<>
inline constexpr auto max_value<technicalmachine::Item> = technicalmachine::Item::Utility_Umbrella;

}	// namespace bounded
namespace technicalmachine {

constexpr bool allows_switching(Item const item) {
	return item == Item::Shed_Shell;
}
constexpr bool boosts_super_effective_moves(Item const item) {
	return item == Item::Expert_Belt;
}
constexpr bool causes_recoil(Item const item) {
	return item == Item::Life_Orb;
}
constexpr bool grounds(Item const item) {
	return item == Item::Iron_Ball;
}
constexpr bool is_choice_item(Item const item) {
	switch (item) {
		case Item::Choice_Band:
		case Item::Choice_Scarf:
		case Item::Choice_Specs:
			return true;
		default:
			return false;
	}
}
constexpr bool was_lost(Item const) {
	// TODO
	return false;
}
constexpr bool extends_hail(Item const item) {
	return item == Item::Icy_Rock;
}
constexpr bool extends_rain(Item const item) {
	return item == Item::Damp_Rock;
}
constexpr bool extends_sand(Item const item) {
	return item == Item::Smooth_Rock;
}
constexpr bool extends_sun(Item const item) {
	return item == Item::Heat_Rock;
}
constexpr bool extends_light_screen(Item const item) {
	return item == Item::Light_Clay;
}
constexpr bool extends_reflect(Item const item) {
	return item == Item::Light_Clay;
}

bounded::integer<0, 80> berry_power(Item item);		// Returns 0 for non-berries
bounded::integer<0, 130> fling_power(Item item);
bool blocks_trick(Item item);

constexpr auto clears_status(Item const item, Statuses const status) -> bool {
	switch (item) {
		case Item::Ice_Berry:
		case Item::Rawst_Berry:
			return status == Statuses::burn;
		case Item::Aspear_Berry:
		case Item::Burnt_Berry:
			return status == Statuses::freeze;
		case Item::Cheri_Berry:
		case Item::PRZCureBerry:
			return status == Statuses::paralysis;
		case Item::Chesto_Berry:
		case Item::Mint_Berry:
			return status == Statuses::rest or status == Statuses::sleep;
		case Item::Pecha_Berry:
		case Item::PSNCureBerry:
			return status == Statuses::poison or status == Statuses::toxic;
		case Item::Lum_Berry:
		case Item::MiracleBerry:
			return true;
		default:
			return false;
	}
}

constexpr auto clears_confusion(Item const item) -> bool {
	switch (item) {
		case Item::Bitter_Berry:
		case Item::Lum_Berry:
		case Item::MiracleBerry:
		case Item::Persim_Berry:
			return true;
		default:
			return false;
	}
}

}	// namespace technicalmachine
