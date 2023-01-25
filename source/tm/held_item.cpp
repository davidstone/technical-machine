// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/conditional.hpp>

export module tm.held_item;

import tm.compress;
import tm.generation;
import tm.item;

import bounded;
import tv;

using namespace bounded::literal;

namespace technicalmachine {

// This uses a macro to take advantage of warnings about not handling all
// enumerations in a switch statement.

#define TECHNICALMACHINE_ITEM_CANNOT_BE_LOST_CASES \
	case Item::Ability_Capsule: \
	case Item::Abomasite: \
	case Item::Absolite: \
	case Item::Aerodactylite: \
	case Item::Aggronite: \
	case Item::Air_Mail: \
	case Item::Alakazite: \
	case Item::Aloraichium_Z: \
	case Item::Altarianite: \
	case Item::Ampharosite: \
	case Item::Audinite: \
	case Item::Banettite: \
	case Item::Bead_Mail: \
	case Item::Beast_Ball: \
	case Item::Beedrillite: \
	case Item::Berry: \
	case Item::Berserk_Gene: \
	case Item::Black_Apricorn: \
	case Item::Blastoisinite: \
	case Item::Blazikenite: \
	case Item::Bloom_Mail: \
	case Item::Blue_Apricorn: \
	case Item::Blue_Orb: \
	case Item::Bluesky_Mail: \
	case Item::Brick_Mail: \
	case Item::Brick_Piece: \
	case Item::BridgeMail_D: \
	case Item::BridgeMail_M: \
	case Item::BridgeMail_S: \
	case Item::BridgeMail_T: \
	case Item::BridgeMail_V: \
	case Item::Bubble_Mail: \
	case Item::Bug_Gem: \
	case Item::Buginium_Z: \
	case Item::Cameruptite: \
	case Item::Charizardite_X: \
	case Item::Charizardite_Y: \
	case Item::Cherish_Ball: \
	case Item::Dark_Gem: \
	case Item::Darkinium_Z: \
	case Item::Decidium_Z: \
	case Item::Diancite: \
	case Item::Dive_Ball: \
	case Item::Dragon_Gem: \
	case Item::Dragonium_Z: \
	case Item::Dream_Ball: \
	case Item::Dream_Mail: \
	case Item::Dusk_Ball: \
	case Item::Eevium_Z: \
	case Item::Electric_Gem: \
	case Item::Electrium_Z: \
	case Item::Eon_Mail: \
	case Item::Fab_Mail: \
	case Item::Fairium_Z: \
	case Item::Fairy_Gem: \
	case Item::Fast_Ball: \
	case Item::Favored_Mail: \
	case Item::Festival_Ticket: \
	case Item::Fighting_Gem: \
	case Item::Fightinium_Z: \
	case Item::Fire_Gem: \
	case Item::Firium_Z: \
	case Item::Flame_Mail: \
	case Item::Flower_Mail: \
	case Item::Flying_Gem: \
	case Item::Flyinium_Z: \
	case Item::Friend_Ball: \
	case Item::Galladite: \
	case Item::Garchompite: \
	case Item::Gardevoirite: \
	case Item::Gengarite: \
	case Item::Ghost_Gem: \
	case Item::Ghostium_Z: \
	case Item::Glalitite: \
	case Item::Glitter_Mail: \
	case Item::Gorgeous_Box: \
	case Item::Grass_Gem: \
	case Item::Grass_Mail: \
	case Item::Grassium_Z: \
	case Item::Great_Ball: \
	case Item::Green_Apricorn: \
	case Item::Greet_Mail: \
	case Item::Ground_Gem: \
	case Item::Groundium_Z: \
	case Item::Gyaradosite: \
	case Item::Harbor_Mail: \
	case Item::Heal_Ball: \
	case Item::Heart_Mail: \
	case Item::Heavy_Ball: \
	case Item::Heracronite: \
	case Item::Houndoominite: \
	case Item::Ice_Gem: \
	case Item::Icium_Z: \
	case Item::Incinium_Z: \
	case Item::Inquiry_Mail: \
	case Item::Kangaskhanite: \
	case Item::Kommonium_Z: \
	case Item::Latiasite: \
	case Item::Latiosite: \
	case Item::Level_Ball: \
	case Item::Like_Mail: \
	case Item::Litebluemail: \
	case Item::Lopunnite: \
	case Item::Love_Ball: \
	case Item::Lovely_Mail: \
	case Item::Lucarionite: \
	case Item::Lunalium_Z: \
	case Item::Lure_Ball: \
	case Item::Luxury_Ball: \
	case Item::Lycanium_Z: \
	case Item::Manectite: \
	case Item::Marshadium_Z: \
	case Item::Master_Ball: \
	case Item::Mawilite: \
	case Item::Mech_Mail: \
	case Item::Medichamite: \
	case Item::Metagrossite: \
	case Item::Mewnium_Z: \
	case Item::Mewtwonite_X: \
	case Item::Mewtwonite_Y: \
	case Item::Mimikium_Z: \
	case Item::Mirage_Mail: \
	case Item::Moon_Ball: \
	case Item::Morph_Mail: \
	case Item::Mosaic_Mail: \
	case Item::Music_Mail: \
	case Item::Nest_Ball: \
	case Item::Net_Ball: \
	case Item::None: \
	case Item::Normal_Box: \
	case Item::Normal_Gem: \
	case Item::Normalium_Z: \
	case Item::Orange_Mail: \
	case Item::Park_Ball: \
	case Item::Pidgeotite: \
	case Item::Pikanium_Z: \
	case Item::Pikashunium_Z: \
	case Item::Pink_Apricorn: \
	case Item::Pink_Bow: \
	case Item::Pinsirite: \
	case Item::Poison_Gem: \
	case Item::Poisonium_Z: \
	case Item::Poke_Ball: \
	case Item::Polkadot_Bow: \
	case Item::Portraitmail: \
	case Item::Premier_Ball: \
	case Item::Primarium_Z: \
	case Item::Psychic_Gem: \
	case Item::Psychium_Z: \
	case Item::Quick_Ball: \
	case Item::Red_Apricorn: \
	case Item::Red_Orb: \
	case Item::Repeat_Ball: \
	case Item::Reply_Mail: \
	case Item::Retro_Mail: \
	case Item::Rock_Gem: \
	case Item::Rockium_Z: \
	case Item::RSVP_Mail: \
	case Item::Rusted_Shield: \
	case Item::Rusted_Sword: \
	case Item::Sablenite: \
	case Item::Safari_Ball: \
	case Item::Salamencite: \
	case Item::Sceptilite: \
	case Item::Scizorite: \
	case Item::Shadow_Mail: \
	case Item::Sharpedonite: \
	case Item::Slowbronite: \
	case Item::Slowpoke_Tail: \
	case Item::Snorlium_Z: \
	case Item::Snow_Mail: \
	case Item::Solganium_Z: \
	case Item::Space_Mail: \
	case Item::Sport_Ball: \
	case Item::Steel_Gem: \
	case Item::Steel_Mail: \
	case Item::Steelium_Z: \
	case Item::Steelixite: \
	case Item::Surf_Mail: \
	case Item::Swampertite: \
	case Item::Tapunium_Z: \
	case Item::Thanks_Mail: \
	case Item::Timer_Ball: \
	case Item::Tropic_Mail: \
	case Item::Tunnel_Mail: \
	case Item::Tyranitarite: \
	case Item::Ultra_Ball: \
	case Item::Ultranecrozium_Z: \
	case Item::Venusaurite: \
	case Item::Water_Gem: \
	case Item::Waterium_Z: \
	case Item::Wave_Mail: \
	case Item::White_Apricorn: \
	case Item::Wood_Mail: \
	case Item::Yellow_Apricorn: \
	case Item::Gold_Leaf: \
	case Item::Silver_Leaf: \
	/* not sure about these */ \
	case Item::TM01: \
	case Item::TM02: \
	case Item::TM03: \
	case Item::TM04: \
	case Item::TM05: \
	case Item::TM06: \
	case Item::TM07: \
	case Item::TM08: \
	case Item::TM09: \
	case Item::TM10: \
	case Item::TM11: \
	case Item::TM12: \
	case Item::TM13: \
	case Item::TM14: \
	case Item::TM15: \
	case Item::TM16: \
	case Item::TM17: \
	case Item::TM18: \
	case Item::TM19: \
	case Item::TM20: \
	case Item::TM21: \
	case Item::TM22: \
	case Item::TM23: \
	case Item::TM24: \
	case Item::TM25: \
	case Item::TM26: \
	case Item::TM27: \
	case Item::TM28: \
	case Item::TM29: \
	case Item::TM30: \
	case Item::TM31: \
	case Item::TM32: \
	case Item::TM33: \
	case Item::TM34: \
	case Item::TM35: \
	case Item::TM36: \
	case Item::TM37: \
	case Item::TM38: \
	case Item::TM39: \
	case Item::TM40: \
	case Item::TM41: \
	case Item::TM42: \
	case Item::TM43: \
	case Item::TM44: \
	case Item::TM45: \
	case Item::TM46: \
	case Item::TM47: \
	case Item::TM48: \
	case Item::TM49: \
	case Item::TM50: \
	case Item::TM51: \
	case Item::TM52: \
	case Item::TM53: \
	case Item::TM54: \
	case Item::TM55: \
	case Item::TM56: \
	case Item::TM57: \
	case Item::TM58: \
	case Item::TM59: \
	case Item::TM60: \
	case Item::TM61: \
	case Item::TM62: \
	case Item::TM63: \
	case Item::TM64: \
	case Item::TM65: \
	case Item::TM66: \
	case Item::TM67: \
	case Item::TM68: \
	case Item::TM69: \
	case Item::TM70: \
	case Item::TM71: \
	case Item::TM72: \
	case Item::TM73: \
	case Item::TM74: \
	case Item::TM75: \
	case Item::TM76: \
	case Item::TM77: \
	case Item::TM78: \
	case Item::TM79: \
	case Item::TM80: \
	case Item::TM81: \
	case Item::TM82: \
	case Item::TM83: \
	case Item::TM84: \
	case Item::TM85: \
	case Item::TM86: \
	case Item::TM87: \
	case Item::TM88: \
	case Item::TM89: \
	case Item::TM90: \
	case Item::TM91: \
	case Item::TM92:


export constexpr auto cannot_be_lost(Item const item) -> bool {
	switch (item) {
		TECHNICALMACHINE_ITEM_CANNOT_BE_LOST_CASES
			return true;
		default:
			return false;
	}
}

export constexpr auto fling_power(Item const item) -> bounded::integer<0, 130> {
	switch (item) {
		case Item::Iron_Ball:
		case Item::TR43:
		case Item::TR71:
			return 130_bi;
		case Item::Armor_Fossil:
		case Item::Claw_Fossil:
		case Item::Cover_Fossil:
		case Item::Dome_Fossil:
		case Item::Fossilized_Bird:
		case Item::Fossilized_Dino:
		case Item::Fossilized_Drake:
		case Item::Fossilized_Fish:
		case Item::Hard_Stone:
		case Item::Helix_Fossil:
		case Item::Jaw_Fossil:
		case Item::Old_Amber:
		case Item::Plume_Fossil:
		case Item::Rare_Bone:
		case Item::Room_Service:
		case Item::Root_Fossil:
		case Item::Sail_Fossil:
		case Item::Skull_Fossil:
			return 100_bi;
		case Item::Deep_Sea_Tooth:
		case Item::Draco_Plate:
		case Item::Dread_Plate:
		case Item::Earth_Plate:
		case Item::Fist_Plate:
		case Item::Flame_Plate:
		case Item::Grip_Claw:
		case Item::Icicle_Plate:
		case Item::Insect_Plate:
		case Item::Iron_Plate:
		case Item::Meadow_Plate:
		case Item::Mind_Plate:
		case Item::Pixie_Plate:
		case Item::Sky_Plate:
		case Item::Splash_Plate:
		case Item::Spooky_Plate:
		case Item::Stone_Plate:
		case Item::Thick_Club:
		case Item::Toxic_Plate:
		case Item::Zap_Plate:
			return 90_bi;
		case Item::Assault_Vest:
		case Item::Bachs_Food_Tin:
		case Item::Blunder_Policy:
		case Item::Bobs_Food_Tin:
		case Item::Chipped_Pot:
		case Item::Cracked_Pot:
		case Item::Dawn_Stone:
		case Item::Dusk_Stone:
		case Item::Electirizer:
		case Item::Heavy_Duty_Boots:
		case Item::Magmarizer:
		case Item::Odd_Keystone:
		case Item::Oval_Stone:
		case Item::Protector:
		case Item::Quick_Claw:
		case Item::Razor_Claw:
		case Item::Sachet:
		case Item::Safety_Goggles:
		case Item::Shiny_Stone:
		case Item::Sticky_Barb:
		case Item::Tin_of_Beans:
		case Item::Weakness_Policy:
		case Item::Whipped_Dream:
			return 80_bi;
		case Item::Burn_Drive:
		case Item::Chill_Drive:
		case Item::Douse_Drive:
		case Item::Dragon_Fang:
		case Item::Poison_Barb:
		case Item::Power_Anklet:
		case Item::Power_Band:
		case Item::Power_Belt:
		case Item::Power_Bracer:
		case Item::Power_Lens:
		case Item::Power_Weight:
		case Item::Shock_Drive:
			return 70_bi;
		case Item::Adamant_Orb:
		case Item::Damp_Rock:
		case Item::Griseous_Orb:
		case Item::Heat_Rock:
		case Item::Leek:
		case Item::Lustrous_Orb:
		case Item::Macho_Brace:
		case Item::Rocky_Helmet:
		case Item::Stick:
		case Item::Terrain_Extender:
		case Item::Utility_Umbrella:
			return 60_bi;
		case Item::Bug_Memory:
		case Item::Dark_Memory:
		case Item::Dragon_Memory:
		case Item::Dubious_Disc:
		case Item::Eject_Pack:
		case Item::Electric_Memory:
		case Item::Fairy_Memory:
		case Item::Fighting_Memory:
		case Item::Fire_Memory:
		case Item::Flying_Memory:
		case Item::Ghost_Memory:
		case Item::Gigantamix:
		case Item::Grass_Memory:
		case Item::Ground_Memory:
		case Item::Ice_Memory:
		case Item::Poison_Memory:
		case Item::Psychic_Memory:
		case Item::Rock_Memory:
		case Item::Sharp_Beak:
		case Item::Spice_Mix:
		case Item::Steel_Memory:
		case Item::Water_Memory:
			return 50_bi;
		case Item::Eviolite:
		case Item::Icy_Rock:
		case Item::Lucky_Punch:
			return 40_bi;
		case Item::Absorb_Bulb:
		case Item::Adrenaline_Orb:
		case Item::Amaze_Mulch:
		case Item::Amulet_Coin:
		case Item::Antidote:
		case Item::Awakening:
		case Item::Balm_Mushroom:
		case Item::Berry_Juice:
		case Item::Big_Malasada:
		case Item::Big_Mushroom:
		case Item::Big_Nugget:
		case Item::Big_Pearl:
		case Item::Binding_Band:
		case Item::Black_Belt:
		case Item::Black_Flute:
		case Item::Black_Glasses:
		case Item::Black_Sludge:
		case Item::Blue_Flute:
		case Item::Blue_Shard:
		case Item::Boost_Mulch:
		case Item::Bottle_Cap:
		case Item::Brittle_Bones:
		case Item::Burn_Heal:
		case Item::Calcium:
		case Item::Carbos:
		case Item::Casteliacone:
		case Item::Cell_Battery:
		case Item::Charcoal:
		case Item::Cleanse_Tag:
		case Item::Comet_Shard:
		case Item::Damp_Mulch:
		case Item::Deep_Sea_Scale:
		case Item::Dire_Hit:
		case Item::Dragon_Scale:
		case Item::Eject_Button:
		case Item::Elixir:
		case Item::Energy_Powder:
		case Item::Energy_Root:
		case Item::Escape_Rope:
		case Item::Ether:
		case Item::Everstone:
		case Item::Exp_Share:
		case Item::Fire_Stone:
		case Item::Flame_Orb:
		case Item::Float_Stone:
		case Item::Fluffy_Tail:
		case Item::Fresh_Water:
		case Item::Full_Heal:
		case Item::Full_Restore:
		case Item::Gold_Bottle_Cap:
		case Item::Gooey_Mulch:
		case Item::Green_Shard:
		case Item::Growth_Mulch:
		case Item::Guard_Spec:
		case Item::Heal_Powder:
		case Item::Heart_Scale:
		case Item::Honey:
		case Item::HP_Up:
		case Item::Hyper_Potion:
		case Item::Ice_Heal:
		case Item::Ice_Stone:
		case Item::Iron:
		case Item::Kings_Rock:
		case Item::Lava_Cookie:
		case Item::Leaf_Stone:
		case Item::Lemonade:
		case Item::Life_Orb:
		case Item::Light_Ball:
		case Item::Light_Clay:
		case Item::Lucky_Egg:
		case Item::Luminous_Moss:
		case Item::Lumiose_Galette:
		case Item::Magnet:
		case Item::Max_Elixir:
		case Item::Max_Ether:
		case Item::Max_Potion:
		case Item::Max_Repel:
		case Item::Max_Revive:
		case Item::Metal_Coat:
		case Item::Metronome:
		case Item::Miracle_Seed:
		case Item::Moomoo_Milk:
		case Item::Moon_Stone:
		case Item::Mystic_Water:
		case Item::Never_Melt_Ice:
		case Item::Nugget:
		case Item::Old_Gateau:
		case Item::Paralyze_Heal:
		case Item::Pass_Orb:
		case Item::Pearl_String:
		case Item::Pearl:
		case Item::Pewter_Crunchies:
		case Item::Poke_Doll:
		case Item::Poke_Toy:
		case Item::Potion:
		case Item::PP_Max:
		case Item::PP_Up:
		case Item::Prism_Scale:
		case Item::Protective_Pads:
		case Item::Protein:
		case Item::Rage_Candy_Bar:
		case Item::Rare_Candy:
		case Item::Razor_Fang:
		case Item::Red_Flute:
		case Item::Red_Shard:
		case Item::Relic_Band:
		case Item::Relic_Copper:
		case Item::Relic_Crown:
		case Item::Relic_Gold:
		case Item::Relic_Silver:
		case Item::Relic_Statue:
		case Item::Relic_Vase:
		case Item::Repel:
		case Item::Revival_Herb:
		case Item::Revive:
		case Item::Rich_Mulch:
		case Item::Sacred_Ash:
		case Item::Scope_Lens:
		case Item::Shalour_Sable:
		case Item::Shell_Bell:
		case Item::Shoal_Salt:
		case Item::Shoal_Shell:
		case Item::Smoke_Ball:
		case Item::Snowball:
		case Item::Soda_Pop:
		case Item::Soul_Dew:
		case Item::Spell_Tag:
		case Item::Stable_Mulch:
		case Item::Star_Piece:
		case Item::Stardust:
		case Item::Strange_Souvenir:
		case Item::Sun_Stone:
		case Item::Super_Potion:
		case Item::Super_Repel:
		case Item::Surprise_Mulch:
		case Item::Sweet_Apple:
		case Item::Sweet_Heart:
		case Item::Tart_Apple:
		case Item::Throat_Spray:
		case Item::Thunder_Stone:
		case Item::Tiny_Mushroom:
		case Item::Toxic_Orb:
		case Item::Twisted_Spoon:
		case Item::Up_Grade:
		case Item::Water_Stone:
		case Item::White_Flute:
		case Item::X_Accuracy:
		case Item::X_Attack:
		case Item::X_Defend:
		case Item::X_Defense:
		case Item::X_Sp_Atk:
		case Item::X_Sp_Def:
		case Item::X_Special:
		case Item::X_Speed:
		case Item::Yellow_Flute:
		case Item::Yellow_Shard:
		case Item::Zinc:
			return 30_bi;
		case Item::Boiled_Egg:
		case Item::Clever_Wing:
		case Item::Fancy_Apple:
		case Item::Genius_Wing:
		case Item::Health_Wing:
		case Item::Large_Leek:
		case Item::Moomoo_Cheese:
		case Item::Muscle_Wing:
		case Item::Pretty_Wing:
		case Item::Resist_Wing:
		case Item::Swift_Wing:
			return 20_bi;
		case Item::Adamant_Mint:
		case Item::Aguav_Berry:
		case Item::Air_Balloon:
		case Item::Apicot_Berry:
		case Item::Aspear_Berry:
		case Item::Babiri_Berry:
		case Item::Belue_Berry:
		case Item::Berry_Sweet:
		case Item::Big_Root:
		case Item::Bitter_Berry:
		case Item::Blue_Scarf:
		case Item::Bluk_Berry:
		case Item::Brave_Mint:
		case Item::Bread:
		case Item::Bright_Powder:
		case Item::Bold_Mint:
		case Item::Burnt_Berry:
		case Item::Calm_Mint:
		case Item::Careful_Mint:
		case Item::Charti_Berry:
		case Item::Cheri_Berry:
		case Item::Chesto_Berry:
		case Item::Chilan_Berry:
		case Item::Choice_Band:
		case Item::Choice_Scarf:
		case Item::Choice_Specs:
		case Item::Chople_Berry:
		case Item::Clover_Sweet:
		case Item::Coba_Berry:
		case Item::Coconut_Milk:
		case Item::Colbur_Berry:
		case Item::Cornn_Berry:
		case Item::Custap_Berry:
		case Item::Destiny_Knot:
		case Item::Discount_Coupon:
		case Item::Drash_Berry:
		case Item::Durin_Berry:
		case Item::Eggant_Berry:
		case Item::Electric_Seed:
		case Item::Enigma_Berry:
		case Item::Expert_Belt:
		case Item::Figy_Berry:
		case Item::Flower_Sweet:
		case Item::Focus_Band:
		case Item::Focus_Sash:
		case Item::Fresh_Cream:
		case Item::Fried_Food:
		case Item::Fruit_Bunch:
		case Item::Full_Incense:
		case Item::Ganlon_Berry:
		case Item::Gentle_Mint:
		case Item::Ginema_Berry:
		case Item::Gold_Berry:
		case Item::Grassy_Seed:
		case Item::Green_Scarf:
		case Item::Grepa_Berry:
		case Item::Haban_Berry:
		case Item::Hasty_Mint:
		case Item::Hondew_Berry:
		case Item::Iapapa_Berry:
		case Item::Ice_Berry:
		case Item::Impish_Mint:
		case Item::Instant_Noodles:
		case Item::Jaboca_Berry:
		case Item::Jolly_Mint:
		case Item::Kasib_Berry:
		case Item::Kebia_Berry:
		case Item::Kee_Berry:
		case Item::Kelpsy_Berry:
		case Item::Kuo_Berry:
		case Item::Lagging_Tail:
		case Item::Lansat_Berry:
		case Item::Lax_Incense:
		case Item::Lax_Mint:
		case Item::Leftovers:
		case Item::Leppa_Berry:
		case Item::Liechi_Berry:
		case Item::Lonely_Mint:
		case Item::Love_Sweet:
		case Item::Luck_Incense:
		case Item::Lum_Berry:
		case Item::Mago_Berry:
		case Item::Magost_Berry:
		case Item::Maranga_Berry:
		case Item::Mental_Herb:
		case Item::Metal_Powder:
		case Item::Micle_Berry:
		case Item::Mild_Mint:
		case Item::Mint_Berry:
		case Item::MiracleBerry:
		case Item::Misty_Seed:
		case Item::Mixed_Mushrooms:
		case Item::Modest_Mint:
		case Item::Muscle_Band:
		case Item::MysteryBerry:
		case Item::Naive_Mint:
		case Item::Nanab_Berry:
		case Item::Naughty_Mint:
		case Item::Niniku_Berry:
		case Item::Nomel_Berry:
		case Item::Nutpea_Berry:
		case Item::Occa_Berry:
		case Item::Odd_Incense:
		case Item::Oran_Berry:
		case Item::Pack_of_Potatoes:
		case Item::Packaged_Curry:
		case Item::Pamtre_Berry:
		case Item::Passho_Berry:
		case Item::Pasta:
		case Item::Payapa_Berry:
		case Item::Pecha_Berry:
		case Item::Persim_Berry:
		case Item::Petaya_Berry:
		case Item::Pinap_Berry:
		case Item::Pink_Nectar:
		case Item::Pink_Scarf:
		case Item::Pomeg_Berry:
		case Item::Power_Herb:
		case Item::Precooked_Burger:
		case Item::PRZCureBerry:
		case Item::PSNCureBerry:
		case Item::Psychic_Seed:
		case Item::Pumkin_Berry:
		case Item::Pungent_Root:
		case Item::Pure_Incense:
		case Item::Purple_Nectar:
		case Item::Qualot_Berry:
		case Item::Quick_Powder:
		case Item::Quiet_Mint:
		case Item::Rabuta_Berry:
		case Item::Rash_Mint:
		case Item::Rawst_Berry:
		case Item::Razz_Berry:
		case Item::Reaper_Cloth:
		case Item::Red_Card:
		case Item::Red_Scarf:
		case Item::Relaxed_Mint:
		case Item::Ribbon_Sweet:
		case Item::Rindo_Berry:
		case Item::Ring_Target:
		case Item::Rock_Incense:
		case Item::Rose_Incense:
		case Item::Roseli_Berry:
		case Item::Rowap_Berry:
		case Item::Salac_Berry:
		case Item::Salad_Mix:
		case Item::Sassy_Mint:
		case Item::Sausages:
		case Item::Sea_Incense:
		case Item::Serious_Mint:
		case Item::Shed_Shell:
		case Item::Shuca_Berry:
		case Item::Silk_Scarf:
		case Item::Silver_Powder:
		case Item::Sitrus_Berry:
		case Item::Smoke_Poke_Tail:
		case Item::Smooth_Rock:
		case Item::Soft_Sand:
		case Item::Soothe_Bell:
		case Item::Spelon_Berry:
		case Item::Star_Sweet:
		case Item::Starf_Berry:
		case Item::Strawberry_Sweet:
		case Item::Strib_Berry:
		case Item::Tamato_Berry:
		case Item::Tanga_Berry:
		case Item::Timid_Mint:
		case Item::Topo_Berry:
		case Item::Touga_Berry:
		case Item::TR00:
		case Item::TR01:
		case Item::TR02:
		case Item::TR03:
		case Item::TR04:
		case Item::TR05:
		case Item::TR06:
		case Item::TR07:
		case Item::TR08:
		case Item::TR09:
		case Item::TR10:
		case Item::TR11:
		case Item::TR12:
		case Item::TR13:
		case Item::TR14:
		case Item::TR15:
		case Item::TR16:
		case Item::TR17:
		case Item::TR18:
		case Item::TR19:
		case Item::TR20:
		case Item::TR21:
		case Item::TR22:
		case Item::TR23:
		case Item::TR24:
		case Item::TR25:
		case Item::TR26:
		case Item::TR27:
		case Item::TR28:
		case Item::TR29:
		case Item::TR30:
		case Item::TR31:
		case Item::TR32:
		case Item::TR33:
		case Item::TR34:
		case Item::TR35:
		case Item::TR36:
		case Item::TR37:
		case Item::TR38:
		case Item::TR39:
		case Item::TR40:
		case Item::TR41:
		case Item::TR42:
		case Item::TR44:
		case Item::TR45:
		case Item::TR46:
		case Item::TR47:
		case Item::TR48:
		case Item::TR49:
		case Item::TR50:
		case Item::TR51:
		case Item::TR52:
		case Item::TR53:
		case Item::TR54:
		case Item::TR55:
		case Item::TR56:
		case Item::TR57:
		case Item::TR58:
		case Item::TR59:
		case Item::TR60:
		case Item::TR61:
		case Item::TR62:
		case Item::TR63:
		case Item::TR64:
		case Item::TR65:
		case Item::TR66:
		case Item::TR67:
		case Item::TR68:
		case Item::TR69:
		case Item::TR70:
		case Item::TR72:
		case Item::TR73:
		case Item::TR74:
		case Item::TR75:
		case Item::TR76:
		case Item::TR77:
		case Item::TR78:
		case Item::TR79:
		case Item::TR80:
		case Item::TR81:
		case Item::TR82:
		case Item::TR83:
		case Item::TR84:
		case Item::TR85:
		case Item::TR86:
		case Item::TR87:
		case Item::TR88:
		case Item::TR89:
		case Item::TR90:
		case Item::TR91:
		case Item::TR92:
		case Item::TR93:
		case Item::TR94:
		case Item::TR95:
		case Item::TR96:
		case Item::TR97:
		case Item::TR98:
		case Item::TR99:
		case Item::Wacan_Berry:
		case Item::Watmel_Berry:
		case Item::Wave_Incense:
		case Item::Wepear_Berry:
		case Item::White_Herb:
		case Item::Wide_Lens:
		case Item::Wiki_Berry:
		case Item::Wise_Glasses:
		case Item::Yache_Berry:
		case Item::Yago_Berry:
		case Item::Yellow_Nectar:
		case Item::Yellow_Scarf:
		case Item::Zoom_Lens:
			return 10_bi;
		TECHNICALMACHINE_ITEM_CANNOT_BE_LOST_CASES
			return 0_bi;
	}
}

export struct HeldItem {
	constexpr explicit HeldItem(Item const item):
		m_item(item),
		m_active(true)
	{
	}

	constexpr auto get(Generation const generation, bool const embargo, bool const magic_room) const {
		if (!m_active or (embargo and affected_by_embargo(generation)) or magic_room) {
			return Item::None;
		}
		return m_item;
	}

	// Remove the item in a way that can be recovered with Recycle
	constexpr auto remove() & -> tv::optional<Item> {
		if (!m_active or cannot_be_lost(m_item)) {
			return tv::none;
		}
		m_active = false;
		// Need to explicitly copy m_item because otherwise we would bind a
		// reference to a bitfield
		return Item(m_item);
	}
	// Remove the item in a way that cannot be recovered with Recycle
	constexpr auto destroy() & -> bool {
		if (!m_active or m_item == Item::None) {
			return false;
		}
		m_item = Item::None;
		return true;
	}

	constexpr auto recycle() & -> void {
		m_active = true;
	}

	friend auto operator==(HeldItem, HeldItem) -> bool = default;
	friend constexpr auto compress(HeldItem const value) {
		return compress_combine(value.m_item, value.m_active);
	}

private:
	constexpr auto affected_by_embargo(Generation const generation) const -> bool {
		// Iron_Ball is disabled in Generation 4 for the Speed check but not for
		// the grounding check
		switch (m_item) {
			case Item::Iron_Ball:
			case Item::Macho_Brace:
			case Item::Power_Anklet:
			case Item::Power_Band:
			case Item::Power_Belt:
			case Item::Power_Bracer:
			case Item::Power_Lens:
			case Item::Power_Weight:
				return generation <= Generation::four;
			default:
				return true;
		}
	}
	Item m_item : bounded::representation_bits<Item>.value();
	bool m_active : 1;
	bool m_tombstone : 1 = false;

	constexpr explicit HeldItem(bounded::tombstone_tag):
		m_item(),
		m_active(),
		m_tombstone(true)
	{
	}
	friend bounded::tombstone_traits<HeldItem>;
};

} // namespace technicalmachine

template<>
struct bounded::tombstone_traits<technicalmachine::HeldItem> {
	static constexpr auto spare_representations = 1_bi;

	static constexpr auto make(bounded::constant_t<0>) noexcept -> technicalmachine::HeldItem {
		return technicalmachine::HeldItem(tombstone_tag());
	}
	static constexpr auto index(technicalmachine::HeldItem const & value) noexcept {
		return BOUNDED_CONDITIONAL(value.m_tombstone, 0_bi, -1_bi);
	}
};
