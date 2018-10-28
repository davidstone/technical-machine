// Random effects of moves
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

#include "variable.hpp"
#include "phazing_in_same_pokemon.hpp"
#include "team.hpp"
#include "move/moves.hpp"
#include "move/is_switch.hpp"
#include "pokemon/pokemon_not_found.hpp"

#include <containers/integer_range.hpp>

#include <cassert>

namespace technicalmachine {
using namespace bounded::literal;

namespace {

auto find_index(PokemonCollection const & collection, Species const species) {
	auto const it = containers::find(collection, species);
	if (it == end(collection)) {
		throw PokemonNotFound(species);
	}
	return TeamIndex(it - begin(collection));
}

auto get_phaze_index(Team const & team, Species const species) {
	assert(team.size() > 1_bi);
	auto const & all = team.all_pokemon();
	auto const pokemon_index = all.index();
	auto const new_index = find_index(all, species);
	if (new_index == pokemon_index) {
		throw PhazingInSamePokemon(new_index);
	}
	return (new_index < pokemon_index) ? TeamIndex(new_index) : TeamIndex(new_index - 1_bi);
}

}	// namespace

auto set_phaze_index(Variable & variable, Team const & team, Species const species) -> void {
	variable.value = get_phaze_index(team, species);
}

auto set_flinch(Variable & variable, bool const set) -> void {
	variable.value = BOUNDED_CONDITIONAL(set, 1_bi, 0_bi);
}

auto effect_activates(Variable const variable) -> bool {
	return variable.value != 0_bi;
}

auto present_heals(Variable const variable) -> bool {
	return variable.value != 0_bi;
}

namespace {

constexpr auto get_magnitude(Variable::Magnitude const magnitude) -> bounded::integer<10, 150> {
	switch (magnitude.value()) {
		case 4: return 10_bi;
		case 5: return 30_bi;
		case 6: return 50_bi;
		case 7: return 70_bi;
		case 8: return 90_bi;
		case 9: return 110_bi;
		default: return 150_bi;
	}
}

}	// namespace

auto set_magnitude(Variable & variable, Variable::Magnitude const magnitude) -> void {
	variable.value = get_magnitude(magnitude);
}

namespace {

constexpr auto magnitude_variables = Probabilities{
	{ 10_bi, 0.05 },
	{ 30_bi, 0.1 },
	{ 50_bi, 0.2 },
	{ 70_bi, 0.3 },
	{ 90_bi, 0.2 },
	{ 110_bi, 0.1 },
	{ 150_bi, 0.05 }
};

constexpr auto present_variables = Probabilities{
	{0_bi, 0.25 },
	{40_bi, 0.25 },
	{80_bi, 0.25 },
	{120_bi, 0.25 }
};

constexpr auto psywave_variables = []{
	Probabilities probabilities;
	constexpr auto min = 50_bi;
	constexpr auto max = 150_bi + 1_bi;
	for (auto const n : containers::integer_range(min, max)) {
		probabilities.emplace_back(n, 1.0 / static_cast<double>(max - min));
	}
	return probabilities;
}();

template<std::size_t... indexes, typename... Args>
constexpr auto generic_probability(std::index_sequence<indexes...>, Args... probabilities) noexcept {
	return Probabilities{ { bounded::constant<indexes>, probabilities }... };
}

template<typename... Args>
constexpr auto generic_probability(Args... probabilities) noexcept {
	return generic_probability(bounded::make_index_sequence(bounded::constant<sizeof...(probabilities)>), probabilities...);
}

template<typename Count>
constexpr auto constant_probability(Count const count) noexcept {
	Probabilities probabilities;
	for (auto const n : containers::integer_range(count)) {
		probabilities.emplace_back(n, 1.0 / static_cast<double>(count));
	}
	return probabilities;
}

constexpr auto guaranteed = generic_probability(1.0);

constexpr auto phaze_probability(TeamSize const foe_size) {
	switch(foe_size.value()) {
		case 1:
		case 2:
			return guaranteed;
		default:
			return constant_probability(bounded::integer<3, TeamSize::max().value()>(foe_size) - 1_bi);
	}
}

}	// namespace

auto all_probabilities(Moves const move, TeamSize const foe_size) -> Probabilities {
	switch (move) {
		case Moves::Absorb:
		case Moves::Acid_Armor:
		case Moves::Acid_Spray:
		case Moves::Acrobatics:
		case Moves::Aerial_Ace:
		case Moves::Aeroblast:
		case Moves::After_You:
		case Moves::Agility:
		case Moves::Air_Cutter:
		case Moves::Ally_Switch:
		case Moves::Amnesia:
		case Moves::Aqua_Jet:
		case Moves::Aqua_Ring:
		case Moves::Aqua_Tail:
		case Moves::Arm_Thrust:
		case Moves::Aromatherapy:
		case Moves::Assist:
		case Moves::Assurance:
		case Moves::Attack_Order:
		case Moves::Attract:
		case Moves::Aura_Sphere:
		case Moves::Autotomize:
		case Moves::Avalanche:
		case Moves::Barrage:
		case Moves::Barrier:
		case Moves::Baton_Pass:
		case Moves::Beat_Up:
		case Moves::Belly_Drum:
		case Moves::Bestow:
		case Moves::Bide:
		case Moves::Bind:
		case Moves::Blast_Burn:
		case Moves::Block:
		case Moves::Bonemerang:
		case Moves::Bone_Rush:
		case Moves::Brave_Bird:
		case Moves::Brick_Break:
		case Moves::Brine:
		case Moves::Bug_Bite:
		case Moves::Bulk_Up:
		case Moves::Bulldoze:
		case Moves::Bullet_Punch:
		case Moves::Bullet_Seed:
		case Moves::Calm_Mind:
		case Moves::Camouflage:
		case Moves::Captivate:
		case Moves::Charge:
		case Moves::Charm:
		case Moves::Chatter:
		case Moves::Chip_Away:
		case Moves::Circle_Throw:
		case Moves::Clamp:
		case Moves::Clear_Smog:
		case Moves::Close_Combat:
		case Moves::Coil:
		case Moves::Comet_Punch:
		case Moves::Confuse_Ray:
		case Moves::Conversion:
		case Moves::Conversion_2:
		case Moves::Copycat:
		case Moves::Cosmic_Power:
		case Moves::Cotton_Guard:
		case Moves::Cotton_Spore:
		case Moves::Counter:
		case Moves::Covet:
		case Moves::Crabhammer:
		case Moves::Cross_Chop:
		case Moves::Crush_Grip:
		case Moves::Curse:
		case Moves::Cut:
		case Moves::Dark_Void:
		case Moves::Defend_Order:
		case Moves::Defense_Curl:
		case Moves::Defog:
		case Moves::Destiny_Bond:
		case Moves::Detect:
		case Moves::Dig:
		case Moves::Disable:
		case Moves::Dive:
		case Moves::Doom_Desire:
		case Moves::Double_Edge:
		case Moves::Double_Hit:
		case Moves::Double_Kick:
		case Moves::DoubleSlap:
		case Moves::Double_Team:
		case Moves::Draco_Meteor:
		case Moves::Dragon_Claw:
		case Moves::Dragon_Dance:
		case Moves::Dragon_Pulse:
		case Moves::Dragon_Rage:
		case Moves::Dragon_Tail:
		case Moves::Drain_Punch:
		case Moves::Dream_Eater:
		case Moves::Drill_Peck:
		case Moves::Drill_Run:
		case Moves::Dual_Chop:
		case Moves::Earthquake:
		case Moves::Echoed_Voice:
		case Moves::Egg_Bomb:
		case Moves::Electro_Ball:
		case Moves::Electroweb:
		case Moves::Embargo:
		case Moves::Encore:
		case Moves::Endeavor:
		case Moves::Endure:
		case Moves::Entrainment:
		case Moves::Eruption:
		case Moves::Explosion:
		case Moves::ExtremeSpeed:
		case Moves::Facade:
		case Moves::Faint_Attack:
		case Moves::Fake_Out:
		case Moves::Fake_Tears:
		case Moves::False_Swipe:
		case Moves::FeatherDance:
		case Moves::Feint:
		case Moves::Fiery_Dance:
		case Moves::Final_Gambit:
		case Moves::Fire_Pledge:
		case Moves::Fire_Spin:
		case Moves::Fissure:
		case Moves::Flail:
		case Moves::Flame_Burst:
		case Moves::Flame_Charge:
		case Moves::Flash:
		case Moves::Flatter:
		case Moves::Fling:
		case Moves::Fly:
		case Moves::Focus_Energy:
		case Moves::Focus_Punch:
		case Moves::Follow_Me:
		case Moves::Foresight:
		case Moves::Foul_Play:
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
		case Moves::Giga_Drain:
		case Moves::Giga_Impact:
		case Moves::Glaciate:
		case Moves::Glare:
		case Moves::Grass_Knot:
		case Moves::Grass_Pledge:
		case Moves::GrassWhistle:
		case Moves::Gravity:
		case Moves::Growl:
		case Moves::Growth:
		case Moves::Grudge:
		case Moves::Guard_Split:
		case Moves::Guard_Swap:
		case Moves::Guillotine:
		case Moves::Gust:
		case Moves::Gyro_Ball:
		case Moves::Hail:
		case Moves::Hammer_Arm:
		case Moves::Harden:
		case Moves::Haze:
		case Moves::Head_Charge:
		case Moves::Head_Smash:
		case Moves::Heal_Bell:
		case Moves::Heal_Block:
		case Moves::Healing_Wish:
		case Moves::Heal_Order:
		case Moves::Heal_Pulse:
		case Moves::Heart_Swap:
		case Moves::Heat_Crash:
		case Moves::Heavy_Slam:
		case Moves::Helping_Hand:
		case Moves::Hex:
		case Moves::Hidden_Power:
		case Moves::Hi_Jump_Kick:
		case Moves::Hit_Self:
		case Moves::Hone_Claws:
		case Moves::Horn_Attack:
		case Moves::Horn_Drill:
		case Moves::Horn_Leech:
		case Moves::Howl:
		case Moves::Hydro_Cannon:
		case Moves::Hydro_Pump:
		case Moves::Hyper_Beam:
		case Moves::Hyper_Voice:
		case Moves::Hypnosis:
		case Moves::Ice_Ball:
		case Moves::Ice_Shard:
		case Moves::Icicle_Spear:
		case Moves::Icy_Wind:
		case Moves::Imprison:
		case Moves::Incinerate:
		case Moves::Ingrain:
		case Moves::Iron_Defense:
		case Moves::Judgment:
		case Moves::Jump_Kick:
		case Moves::Karate_Chop:
		case Moves::Kinesis:
		case Moves::Knock_Off:
		case Moves::Last_Resort:
		case Moves::Leaf_Blade:
		case Moves::Leaf_Storm:
		case Moves::Leech_Life:
		case Moves::Leech_Seed:
		case Moves::Leer:
		case Moves::Light_Screen:
		case Moves::Lock_On:
		case Moves::Lovely_Kiss:
		case Moves::Low_Kick:
		case Moves::Low_Sweep:
		case Moves::Lucky_Chant:
		case Moves::Lunar_Dance:
		case Moves::Luster_Purge:
		case Moves::Mach_Punch:
		case Moves::Magical_Leaf:
		case Moves::Magic_Coat:
		case Moves::Magic_Room:
		case Moves::Magma_Storm:
		case Moves::Magnet_Bomb:
		case Moves::Magnet_Rise:
		case Moves::Mean_Look:
		case Moves::Meditate:
		case Moves::Me_First:
		case Moves::Mega_Drain:
		case Moves::Megahorn:
		case Moves::Mega_Kick:
		case Moves::Mega_Punch:
		case Moves::Memento:
		case Moves::Metal_Burst:
		case Moves::Metal_Sound:
		case Moves::Metronome:
		case Moves::Milk_Drink:
		case Moves::Mimic:
		case Moves::Mind_Reader:
		case Moves::Minimize:
		case Moves::Miracle_Eye:
		case Moves::Mirror_Coat:
		case Moves::Mirror_Move:
		case Moves::Mist:
		case Moves::Moonlight:
		case Moves::Morning_Sun:
		case Moves::Mud_Shot:
		case Moves::Mud_Slap:
		case Moves::Mud_Sport:
		case Moves::Nasty_Plot:
		case Moves::Natural_Gift:
		case Moves::Nature_Power:
		case Moves::Nightmare:
		case Moves::Night_Shade:
		case Moves::Night_Slash:
		case Moves::Odor_Sleuth:
		case Moves::Outrage:
		case Moves::Overheat:
		case Moves::Pain_Split:
		case Moves::Pass:
		case Moves::Payback:
		case Moves::Pay_Day:
		case Moves::Peck:
		case Moves::Perish_Song:
		case Moves::Petal_Dance:
		case Moves::Pin_Missile:
		case Moves::Pluck:
		case Moves::Poison_Gas:
		case Moves::PoisonPowder:
		case Moves::Pound:
		case Moves::Power_Gem:
		case Moves::Power_Split:
		case Moves::Power_Swap:
		case Moves::Power_Trick:
		case Moves::Power_Whip:
		case Moves::Protect:
		case Moves::Psycho_Boost:
		case Moves::Psycho_Cut:
		case Moves::Psycho_Shift:
		case Moves::Psych_Up:
		case Moves::Psyshock:
		case Moves::Psystrike:
		case Moves::Punishment:
		case Moves::Pursuit:
		case Moves::Quash:
		case Moves::Quick_Attack:
		case Moves::Quick_Guard:
		case Moves::Quiver_Dance:
		case Moves::Rage:
		case Moves::Rage_Powder:
		case Moves::Rain_Dance:
		case Moves::Rapid_Spin:
		case Moves::Razor_Leaf:
		case Moves::Razor_Wind:
		case Moves::Recover:
		case Moves::Recycle:
		case Moves::Reflect:
		case Moves::Reflect_Type:
		case Moves::Refresh:
		case Moves::Rest:
		case Moves::Retaliate:
		case Moves::Return:
		case Moves::Revenge:
		case Moves::Reversal:
		case Moves::Roar_of_Time:
		case Moves::Rock_Blast:
		case Moves::Rock_Polish:
		case Moves::Rock_Throw:
		case Moves::Rock_Tomb:
		case Moves::Rock_Wrecker:
		case Moves::Role_Play:
		case Moves::Rollout:
		case Moves::Roost:
		case Moves::Round:
		case Moves::Sacred_Sword:
		case Moves::Safeguard:
		case Moves::Sand_Attack:
		case Moves::Sandstorm:
		case Moves::Sand_Tomb:
		case Moves::Scary_Face:
		case Moves::Scratch:
		case Moves::Screech:
		case Moves::Secret_Sword:
		case Moves::Seed_Bomb:
		case Moves::Seismic_Toss:
		case Moves::Selfdestruct:
		case Moves::Shadow_Claw:
		case Moves::Shadow_Force:
		case Moves::Shadow_Punch:
		case Moves::Shadow_Sneak:
		case Moves::Sharpen:
		case Moves::Sheer_Cold:
		case Moves::Shell_Smash:
		case Moves::Shift_Gear:
		case Moves::Shock_Wave:
		case Moves::Simple_Beam:
		case Moves::Sing:
		case Moves::Sketch:
		case Moves::Skill_Swap:
		case Moves::Skull_Bash:
		case Moves::Sky_Drop:
		case Moves::Sky_Uppercut:
		case Moves::Slack_Off:
		case Moves::Slam:
		case Moves::Slash:
		case Moves::Sleep_Powder:
		case Moves::Sleep_Talk:
		case Moves::Smack_Down:
		case Moves::SmellingSalt:
		case Moves::SmokeScreen:
		case Moves::Snarl:
		case Moves::Snatch:
		case Moves::Soak:
		case Moves::Softboiled:
		case Moves::SolarBeam:
		case Moves::SonicBoom:
		case Moves::Spacial_Rend:
		case Moves::Spider_Web:
		case Moves::Spike_Cannon:
		case Moves::Spikes:
		case Moves::Spite:
		case Moves::Spit_Up:
		case Moves::Splash:
		case Moves::Spore:
		case Moves::Stealth_Rock:
		case Moves::Stockpile:
		case Moves::Stone_Edge:
		case Moves::Stored_Power:
		case Moves::Storm_Throw:
		case Moves::Strength:
		case Moves::String_Shot:
		case Moves::Struggle:
		case Moves::Struggle_Bug:
		case Moves::Stun_Spore:
		case Moves::Submission:
		case Moves::Substitute:
		case Moves::Sucker_Punch:
		case Moves::Sunny_Day:
		case Moves::Super_Fang:
		case Moves::Superpower:
		case Moves::Supersonic:
		case Moves::Surf:
		case Moves::Swagger:
		case Moves::Swallow:
		case Moves::Sweet_Kiss:
		case Moves::Sweet_Scent:
		case Moves::Swift:
		case Moves::Switch0:
		case Moves::Switch1:
		case Moves::Switch2:
		case Moves::Switch3:
		case Moves::Switch4:
		case Moves::Switch5:
		case Moves::Switcheroo:
		case Moves::Swords_Dance:
		case Moves::Synchronoise:
		case Moves::Synthesis:
		case Moves::Tackle:
		case Moves::Tail_Glow:
		case Moves::Tail_Slap:
		case Moves::Tail_Whip:
		case Moves::Tailwind:
		case Moves::Take_Down:
		case Moves::Taunt:
		case Moves::Techno_Blast:
		case Moves::Teeter_Dance:
		case Moves::Telekinesis:
		case Moves::Teleport:
		case Moves::Thief:
		case Moves::Thrash:
		case Moves::Thunder_Wave:
		case Moves::Tickle:
		case Moves::Torment:
		case Moves::Toxic:
		case Moves::Toxic_Spikes:
		case Moves::Transform:
		case Moves::Trick:
		case Moves::Trick_Room:
		case Moves::Triple_Kick:
		case Moves::Trump_Card:
		case Moves::Uproar:
		case Moves::U_turn:
		case Moves::Vacuum_Wave:
		case Moves::V_create:
		case Moves::Venoshock:
		case Moves::ViceGrip:
		case Moves::Vine_Whip:
		case Moves::Vital_Throw:
		case Moves::Volt_Switch:
		case Moves::Wake_Up_Slap:
		case Moves::Water_Gun:
		case Moves::Water_Pledge:
		case Moves::Water_Sport:
		case Moves::Water_Spout:
		case Moves::Weather_Ball:
		case Moves::Whirlpool:
		case Moves::Wide_Guard:
		case Moves::Wild_Charge:
		case Moves::Will_O_Wisp:
		case Moves::Wing_Attack:
		case Moves::Wish:
		case Moves::Withdraw:
		case Moves::Wonder_Room:
		case Moves::Wood_Hammer:
		case Moves::Work_Up:
		case Moves::Worry_Seed:
		case Moves::Wrap:
		case Moves::Wring_Out:
		case Moves::X_Scissor:
		case Moves::Yawn:
			return guaranteed;
		case Moves::Acid:
		case Moves::AncientPower:
		case Moves::Aurora_Beam:
		case Moves::Bite:
		case Moves::Blaze_Kick:
		case Moves::Blizzard:
		case Moves::Body_Slam:
		case Moves::Bone_Club:
		case Moves::Bubble:
		case Moves::BubbleBeam:
		case Moves::Bug_Buzz:
		case Moves::Confusion:
		case Moves::Constrict:
		case Moves::Cross_Poison:
		case Moves::Earth_Power:
		case Moves::Ember:
		case Moves::Energy_Ball:
		case Moves::Extrasensory:
		case Moves::Fire_Blast:
		case Moves::Fire_Punch:
		case Moves::Flamethrower:
		case Moves::Flame_Wheel:
		case Moves::Flare_Blitz:
		case Moves::Flash_Cannon:
		case Moves::Focus_Blast:
		case Moves::Headbutt:
		case Moves::Heat_Wave:
		case Moves::Hyper_Fang:
		case Moves::Ice_Beam:
		case Moves::Ice_Punch:
		case Moves::Iron_Tail:
		case Moves::Metal_Claw:
		case Moves::Ominous_Wind:
		case Moves::Poison_Sting:
		case Moves::Poison_Tail:
		case Moves::Powder_Snow:
		case Moves::Psybeam:
		case Moves::Psychic:
		case Moves::Relic_Song:
		case Moves::Rolling_Kick:
		case Moves::Signal_Beam:
		case Moves::Silver_Wind:
		case Moves::Sludge_Wave:
		case Moves::Steel_Wing:
		case Moves::Stomp:
		case Moves::Thunderbolt:
		case Moves::ThunderPunch:
		case Moves::ThunderShock:
		case Moves::Volt_Tackle:
			return generic_probability(0.9, 0.1);
		case Moves::Blue_Flare:
		case Moves::Bolt_Strike:
		case Moves::Crunch:
		case Moves::Dark_Pulse:
		case Moves::Dizzy_Punch:
		case Moves::Dragon_Rush:
		case Moves::Meteor_Mash:
		case Moves::Rock_Climb:
		case Moves::Shadow_Ball:
		case Moves::Twineedle:
		case Moves::Twister:
		case Moves::Waterfall:
		case Moves::Water_Pulse:
		case Moves::Zen_Headbutt:
			return generic_probability(0.8, 0.2);
		case Moves::Air_Slash:
		case Moves::Astonish:
		case Moves::Bounce:
		case Moves::Discharge:
		case Moves::DragonBreath:
		case Moves::Force_Palm:
		case Moves::Freeze_Shock:
		case Moves::Gunk_Shot:
		case Moves::Heart_Stamp:
		case Moves::Hurricane:
		case Moves::Ice_Burn:
		case Moves::Icicle_Crash:
		case Moves::Iron_Head:
		case Moves::Lava_Plume:
		case Moves::Leaf_Tornado:
		case Moves::Lick:
		case Moves::Mirror_Shot:
		case Moves::Mud_Bomb:
		case Moves::Muddy_Water:
		case Moves::Needle_Arm:
		case Moves::Poison_Fang:
		case Moves::Poison_Jab:
		case Moves::Rock_Slide:
		case Moves::Scald:
		case Moves::Searing_Shot:
		case Moves::Secret_Power:
		case Moves::Sky_Attack:
		case Moves::Sludge:
		case Moves::Sludge_Bomb:
		case Moves::Snore:
		case Moves::Spark:
		case Moves::Steamroller:
		case Moves::Thunder:
			return generic_probability(0.7, 0.3);
		case Moves::Night_Daze:
		case Moves::Seed_Flare:
		case Moves::Smog:
			return generic_probability(0.6, 0.4);
		case Moves::Crush_Claw:
		case Moves::Mist_Ball:
		case Moves::Octazooka:
		case Moves::Razor_Shell:
		case Moves::Rock_Smash:
		case Moves::Sacred_Fire:
			return generic_probability(0.5, 0.5);
		case Moves::Charge_Beam:
			return generic_probability(0.3, 0.7);
		case Moves::DynamicPunch:
		case Moves::Inferno:
		case Moves::Zap_Cannon:
			return generic_probability(0.0, 1.0);
		case Moves::Fire_Fang:
		case Moves::Ice_Fang:
		case Moves::Thunder_Fang:
			return generic_probability(0.79, 0.10, 0.10, 0.01);
		case Moves::Tri_Attack:
			return generic_probability(12.0 / 15.0, 1.0 / 15.0, 1.0 / 15.0, 1.0 / 15.0);
		case Moves::Magnitude:
			return magnitude_variables;
		case Moves::Present:
			return present_variables;
		case Moves::Psywave:
			return psywave_variables;
		case Moves::Acupressure:
			// Possibly not correct due to the maxing out behavior
			return constant_probability(7_bi);
		case Moves::Roar:
		case Moves::Whirlwind:
			return phaze_probability(foe_size);
	}
}
}	// namespace technicalmachine
