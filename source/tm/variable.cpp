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

#include <tm/variable.hpp>
#include <tm/phazing_in_same_pokemon.hpp>
#include <tm/team.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/is_switch.hpp>
#include <tm/pokemon/pokemon_not_found.hpp>

#include <bounded/assert.hpp>

#include <containers/emplace_back.hpp>
#include <containers/integer_range.hpp>

#include <cassert>

namespace technicalmachine {
using namespace bounded::literal;

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

auto Variable::set_magnitude(Magnitude const magnitude) -> void {
	m_value = get_magnitude(magnitude);
}

namespace {

template<std::size_t... indexes>
constexpr auto generic_probability(std::index_sequence<indexes...>, auto... probabilities) {
	BOUNDED_ASSERT((... and (probabilities != 0.0)));
	return Probabilities{{Variable{bounded::constant<indexes>}, probabilities}...};
}

constexpr auto generic_probability(auto... probabilities) {
	return generic_probability(bounded::make_index_sequence(bounded::constant<sizeof...(probabilities)>), probabilities...);
}

constexpr auto single_probability(double const probability) {
	return
		probability == 1.0 ? Probabilities{{Variable{bounded::constant<1>}, 1.0}} :
		probability == 0.0 ? generic_probability(1.0) :
		generic_probability(1.0 - probability, probability);
}

constexpr auto constant_probability(auto const count) {
	Probabilities probabilities;
	for (auto const n : containers::integer_range(count)) {
		containers::emplace_back(probabilities, Variable{n}, 1.0 / static_cast<double>(count));
	}
	return probabilities;
}

auto phaze_probability(TeamSize const target_size) {
	switch(target_size.value()) {
		case 1:
		case 2:
			return generic_probability(1.0);
		default:
			return constant_probability(bounded::integer<3, bounded::detail::builtin_max_value<TeamSize>>(target_size) - 1_bi);
	}
}

template<Generation generation>
constexpr auto generation_one_status_side_effect(double const probability, Team<generation> const & target, Type const immune_type, Type const move_type) {
	return
		(generation == Generation::one and is_type(target.pokemon(), move_type)) ? generic_probability(1.0) :
		is_type(target.pokemon(), immune_type) ? generic_probability(1.0) :
		single_probability(probability);
}

template<Generation generation>
constexpr auto status_side_effect(double const probability, Team<generation> const & target, auto const... immune_types) {
	return
		is_type(target.pokemon(), immune_types...) ? generic_probability(1.0) :
		single_probability(probability);
}

template<Generation generation>
constexpr auto freeze_probability(Team<generation> const & target) {
	auto const freeze_claused = team_has_status(target, Statuses::freeze);
	return freeze_claused ? single_probability(0.0) : status_side_effect(0.1, target, Type::Ice);
}

template<Generation generation>
constexpr auto tri_attack_probabilities(Team<generation> const & target) {
	constexpr auto no_status = Variable{0_bi};
	auto status = [](auto const index) {
		return VariableProbability{Variable{index}, 1.0 / 15.0};
	};

	constexpr auto burn = status(1_bi);
	constexpr auto freeze = status(2_bi);
	constexpr auto paralysis = status(3_bi);

	constexpr auto burn_freeze_paralysis_probabilities = Probabilities{{no_status, 12.0 / 15.0}, burn, freeze, paralysis};
	constexpr auto burn_freeze_probabilities = Probabilities{{no_status, 13.0 / 15.0}, burn, freeze};
	constexpr auto burn_paralysis_probabilities = Probabilities{{no_status, 13.0 / 15.0}, burn, paralysis};
	constexpr auto freeze_paralysis_probabilities = Probabilities{{no_status, 13.0 / 15.0}, freeze, paralysis};
	constexpr auto burn_probabilities = Probabilities{{no_status, 14.0 / 15.0}, burn};
	constexpr auto freeze_probabilities = Probabilities{{no_status, 14.0 / 15.0}, freeze};
	constexpr auto paralysis_probabilities = Probabilities{{no_status, 14.0 / 15.0}, paralysis};
	constexpr auto none_probabilities = Probabilities{{no_status, 14.0 / 15.0}};

	auto const freeze_claused = team_has_status(target, Statuses::freeze);

	constexpr auto ice_probabilities = burn_paralysis_probabilities;
	auto const other_probabilities = freeze_claused ? burn_paralysis_probabilities : burn_freeze_paralysis_probabilities;

	switch (generation) {
		case Generation::one:
			return single_probability(0.0);
		case Generation::two:
			// TODO: Can thaw 1/3 of the time
			return other_probabilities;
		case Generation::three:
		case Generation::four:
		case Generation::five: {
			auto is = [&](Type const type) {
				return is_type(target.pokemon(), type);
			};
			return
				is(Type::Fire) ? (freeze_claused ? paralysis_probabilities : freeze_paralysis_probabilities) :
				is(Type::Ice) ? ice_probabilities :
				other_probabilities;
		}
		case Generation::six:
		case Generation::seven:
		case Generation::eight: {
			auto const is_fire = is_type(target.pokemon(), Type::Fire);
			auto const is_ice = is_type(target.pokemon(), Type::Ice);
			auto const is_electric = is_type(target.pokemon(), Type::Electric);

			return
				is_fire and is_ice ? paralysis_probabilities :
				is_fire and is_electric ? (freeze_claused ? none_probabilities : freeze_probabilities) :
				is_ice and is_electric ? burn_probabilities :
				is_fire ? (freeze_claused ? paralysis_probabilities : freeze_paralysis_probabilities) :
				is_ice ? ice_probabilities :
				is_electric ? (freeze_claused ? burn_probabilities : burn_freeze_probabilities) :
				other_probabilities;
		}
	}
}

template<Generation generation>
constexpr auto fang_side_effect(Team<generation> const & target, Type const immune_type, bool status_claused = false) {
	return is_type(target.pokemon(), immune_type) or status_claused ?
		Probabilities{{Variable{bounded::constant<0>}, 0.9}, {Variable{bounded::constant<2>}, 0.1}} :
		generic_probability(0.81, 0.09, 0.09, 0.01);
}

template<Generation generation>
constexpr auto sleep_can_apply(ActivePokemon<generation> const user, Team<generation> const & target) {
	return !(user.is_uproaring() or target.pokemon().is_uproaring() or team_has_status(target, Statuses::sleep));
}

} // namespace

template<Generation generation>
auto all_probabilities(Moves const move, ActivePokemon<generation> const user, Team<generation> const & target) -> Probabilities {
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
		case Moves::Double_Slap:
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
		case Moves::Extreme_Speed:
		case Moves::Facade:
		case Moves::Feint_Attack:
		case Moves::Fake_Out:
		case Moves::Fake_Tears:
		case Moves::False_Swipe:
		case Moves::Feather_Dance:
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
		case Moves::Grass_Knot:
		case Moves::Grass_Pledge:
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
		case Moves::High_Jump_Kick:
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
		case Moves::Self_Destruct:
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
		case Moves::Sketch:
		case Moves::Skill_Swap:
		case Moves::Skull_Bash:
		case Moves::Sky_Drop:
		case Moves::Sky_Uppercut:
		case Moves::Slack_Off:
		case Moves::Slam:
		case Moves::Slash:
		case Moves::Sleep_Talk:
		case Moves::Smack_Down:
		case Moves::Smelling_Salts:
		case Moves::Smokescreen:
		case Moves::Snarl:
		case Moves::Snatch:
		case Moves::Soak:
		case Moves::Soft_Boiled:
		case Moves::Solar_Beam:
		case Moves::Sonic_Boom:
		case Moves::Spacial_Rend:
		case Moves::Spider_Web:
		case Moves::Spike_Cannon:
		case Moves::Spikes:
		case Moves::Spite:
		case Moves::Spit_Up:
		case Moves::Splash:
		case Moves::Stealth_Rock:
		case Moves::Stockpile:
		case Moves::Stone_Edge:
		case Moves::Stored_Power:
		case Moves::Storm_Throw:
		case Moves::Strength:
		case Moves::String_Shot:
		case Moves::Struggle:
		case Moves::Struggle_Bug:
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
		case Moves::Tickle:
		case Moves::Torment:
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
		case Moves::Vice_Grip:
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
			return generic_probability(1.0);
		case Moves::Ancient_Power:
		case Moves::Bone_Club:
		case Moves::Bug_Buzz:
		case Moves::Confusion:
		case Moves::Earth_Power:
		case Moves::Energy_Ball:
		case Moves::Extrasensory:
		case Moves::Flash_Cannon:
		case Moves::Focus_Blast:
		case Moves::Headbutt:
		case Moves::Hyper_Fang:
		case Moves::Iron_Tail:
		case Moves::Metal_Claw:
		case Moves::Ominous_Wind:
		case Moves::Psybeam:
		case Moves::Relic_Song:
		case Moves::Signal_Beam:
		case Moves::Silver_Wind:
		case Moves::Steel_Wing:
		case Moves::Stomp:
			return single_probability(0.1);
		case Moves::Blue_Flare:
		case Moves::Bolt_Strike:
		case Moves::Crunch:
		case Moves::Dark_Pulse:
		case Moves::Dragon_Rush:
		case Moves::Meteor_Mash:
		case Moves::Rock_Climb:
		case Moves::Shadow_Ball:
		case Moves::Twister:
		case Moves::Water_Pulse:
		case Moves::Zen_Headbutt:
			return single_probability(0.2);
		case Moves::Air_Slash:
		case Moves::Astonish:
		case Moves::Bounce:
		case Moves::Dragon_Breath:
		case Moves::Freeze_Shock:
		case Moves::Heart_Stamp:
		case Moves::Hurricane:
		case Moves::Ice_Burn:
		case Moves::Icicle_Crash:
		case Moves::Iron_Head:
		case Moves::Leaf_Tornado:
		case Moves::Mirror_Shot:
		case Moves::Mud_Bomb:
		case Moves::Muddy_Water:
		case Moves::Needle_Arm:
		case Moves::Rolling_Kick:
		case Moves::Snore:
		case Moves::Steamroller:
			return single_probability(0.3);
		case Moves::Night_Daze:
		case Moves::Seed_Flare:
			return single_probability(0.4);
		case Moves::Crush_Claw:
		case Moves::Mist_Ball:
		case Moves::Octazooka:
		case Moves::Razor_Shell:
		case Moves::Rock_Smash:
			return single_probability(0.5);
		case Moves::Charge_Beam:
			return single_probability(0.7);
		case Moves::Dynamic_Punch:
			return generic_probability(1.0);
		case Moves::Blizzard:
		case Moves::Ice_Beam:
		case Moves::Ice_Punch:
		case Moves::Powder_Snow:
			return freeze_probability(target);
		case Moves::Blaze_Kick:
		case Moves::Ember:
		case Moves::Fire_Punch:
		case Moves::Flame_Wheel:
		case Moves::Flamethrower:
		case Moves::Flare_Blitz:
		case Moves::Heat_Wave:
			return status_side_effect(0.1, target, Type::Fire);
		case Moves::Lava_Plume:
		case Moves::Scald:
		case Moves::Searing_Shot:
			return status_side_effect(0.3, target, Type::Fire);
		case Moves::Sacred_Fire:
			return status_side_effect(0.5, target, Type::Fire);
		case Moves::Inferno:
		case Moves::Will_O_Wisp:
			return status_side_effect(1.0, target, Type::Fire);
		case Moves::Cross_Poison:
		case Moves::Poison_Tail:
		case Moves::Sludge_Wave:
			return status_side_effect(0.1, target, Type::Poison, Type::Steel);
		case Moves::Gunk_Shot:
		case Moves::Poison_Fang:
		case Moves::Poison_Jab:
		case Moves::Sludge_Bomb:
			return status_side_effect(0.3, target, Type::Poison, Type::Steel);
		case Moves::Poison_Sting:
			return status_side_effect(
				generation == Generation::one ? 0.2 : 0.3,
				target,
				Type::Poison,
				Type::Steel
			);
		case Moves::Sludge:
			return status_side_effect(
				generation == Generation::one ? 0.4 : 0.3,
				target,
				Type::Poison,
				Type::Steel
			);
		case Moves::Smog:
			return status_side_effect(0.4, target, Type::Poison, Type::Steel);
		case Moves::Twineedle:
			return generation == Generation::two ?
				status_side_effect(0.2, target, Type::Poison) :
				status_side_effect(0.2, target, Type::Poison, Type::Steel);
		case Moves::Toxic:
			return status_side_effect(1.0, target, Type::Poison, Type::Steel);
		case Moves::Poison_Gas:
		case Moves::Poison_Powder:
			return generation <= Generation::five ?
				status_side_effect(1.0, target, Type::Poison, Type::Steel) :
				status_side_effect(1.0, target, Type::Grass, Type::Poison, Type::Steel);
		case Moves::Dark_Void:
		case Moves::Grass_Whistle:
		case Moves::Hypnosis:
		case Moves::Lovely_Kiss:
		case Moves::Sing:
			return sleep_can_apply(user, target) ? single_probability(0.0) : single_probability(1.0);
		case Moves::Sleep_Powder:
		case Moves::Spore:
			if (sleep_can_apply(user, target)) {
				return single_probability(0.0);
			}
			return generation <= Generation::five ?
				single_probability(1.0) :
				status_side_effect(1.0, target, Type::Grass);
		case Moves::Stun_Spore:
			return generation <= Generation::five ?
				single_probability(1.0) :
				status_side_effect(1.0, target, Type::Grass);
		case Moves::Body_Slam:
			return generation_one_status_side_effect(0.3, target, Type::Ghost, Type::Normal);
		case Moves::Force_Palm:
		case Moves::Secret_Power:
			return status_side_effect(0.3, target, Type::Ghost);
		case Moves::Lick:
			return generation_one_status_side_effect(0.3, target, Type::Normal, Type::Ghost);
		case Moves::Thunder:
			return generation_one_status_side_effect(
				generation == Generation::one ? 0.1 : 0.3,
				target,
				Type::Ground,
				Type::Electric
			);
		case Moves::Thunderbolt:
		case Moves::Thunder_Punch:
		case Moves::Thunder_Shock:
			return generation_one_status_side_effect(0.1, target, Type::Ground, Type::Electric);
		case Moves::Volt_Tackle:
			return status_side_effect(0.1, target, Type::Ground);
		case Moves::Discharge:
		case Moves::Spark:
			return status_side_effect(0.3, target, Type::Ground);
		case Moves::Thunder_Wave:
		case Moves::Zap_Cannon:
			return status_side_effect(1.0, target, Type::Ground);
		case Moves::Glare:
			switch (generation) {
				case Generation::one:
					return single_probability(1.0);
				case Generation::two:
				case Generation::three:
					return status_side_effect(1.0, target, Type::Ghost);
				case Generation::four:
				case Generation::five:
					return single_probability(1.0);
				case Generation::six:
				case Generation::seven:
				case Generation::eight:
					return status_side_effect(1.0, target, Type::Electric);
			}
		case Moves::Acid:
		case Moves::Aurora_Beam:
		case Moves::Bubble:
		case Moves::Bubble_Beam:
		case Moves::Constrict:
		case Moves::Psychic:
			return generation <= Generation::one ? single_probability(0.332) : single_probability(0.1);
		case Moves::Bite:
			return generation <= Generation::one ? single_probability(0.1) : single_probability(0.3);
		case Moves::Dizzy_Punch:
			return generation <= Generation::one ? generic_probability(1.0) : single_probability(0.2);
		case Moves::Fire_Blast:
			return generation <= Generation::one ?
				status_side_effect(0.3, target, Type::Fire) :
				status_side_effect(0.1, target, Type::Fire);
		case Moves::Fire_Fang:
			return fang_side_effect(target, Type::Fire);
		case Moves::Ice_Fang:
			return fang_side_effect(target, Type::Ice, team_has_status(target, Statuses::freeze));
		case Moves::Thunder_Fang:
			return fang_side_effect(target, Type::Ground);
		case Moves::Low_Kick:
			return generation <= Generation::two ? single_probability(0.3) : generic_probability(1.0);
		case Moves::Tri_Attack:
			return tri_attack_probabilities(target);
		case Moves::Waterfall:
			return generation <= Generation::three ? generic_probability(1.0) : single_probability(0.2);
		case Moves::Magnitude:
			return Probabilities{
				{Variable{10_bi}, 0.05},
				{Variable{30_bi}, 0.1},
				{Variable{50_bi}, 0.2},
				{Variable{70_bi}, 0.3},
				{Variable{90_bi}, 0.2},
				{Variable{110_bi}, 0.1},
				{Variable{150_bi}, 0.05}
			};
		case Moves::Present:
			return Probabilities{
				{Variable{0_bi}, 0.25},
				{Variable{40_bi}, 0.25},
				{Variable{80_bi}, 0.25},
				{Variable{120_bi}, 0.25}
			};
		case Moves::Psywave: {
			auto compute = [](auto const min_value, auto const max_value) {
				auto const range = containers::inclusive_integer_range(min_value, max_value);
				Probabilities probabilities;
				for (auto const n : range) {
					containers::emplace_back(probabilities, Variable{n}, 1.0 / static_cast<double>(size(range)));
				}
				return probabilities;
			};
			switch (generation) {
				case Generation::one:
					// Close enough
				case Generation::two:
					// Technically, every vallue from 1 to 150 is possible. To
					// save time and space, pretend like it's a multiple of 10
					return compute(1_bi, 15_bi);
				case Generation::three:
				case Generation::four:
					return compute(5_bi, 15_bi);
				case Generation::five:
				case Generation::six:
				case Generation::seven:
				case Generation::eight:
					// Technically, every vallue from 1 to 100 is possible. To
					// save time and space, pretend like it's a multiple of 10
					return compute(5_bi, 15_bi);
			}
		}
		case Moves::Rock_Slide:
			return generation == Generation::one ? generic_probability(1.0) : single_probability(0.3);
		case Moves::Sky_Attack:
			return generation <= Generation::two ? generic_probability(1.0) : single_probability(0.3);
		case Moves::Acupressure:
			// Possibly not correct due to the maxing out behavior
			return constant_probability(7_bi);
		case Moves::Roar:
		case Moves::Whirlwind:
			return phaze_probability(target.size());
		case Moves::Flying_Press:
		case Moves::Mat_Block:
		case Moves::Belch:
		case Moves::Rototiller:
		case Moves::Sticky_Web:
		case Moves::Fell_Stinger:
		case Moves::Phantom_Force:
		case Moves::Trick_or_Treat:
		case Moves::Noble_Roar:
		case Moves::Ion_Deluge:
		case Moves::Parabolic_Charge:
		case Moves::Forests_Curse:
		case Moves::Petal_Blizzard:
		case Moves::Freeze_Dry:
		case Moves::Disarming_Voice:
		case Moves::Parting_Shot:
		case Moves::Topsy_Turvy:
		case Moves::Draining_Kiss:
		case Moves::Crafty_Shield:
		case Moves::Flower_Shield:
		case Moves::Grassy_Terrain:
		case Moves::Misty_Terrain:
		case Moves::Electrify:
		case Moves::Play_Rough:
		case Moves::Fairy_Wind:
		case Moves::Moonblast:
		case Moves::Boomburst:
		case Moves::Fairy_Lock:
		case Moves::Kings_Shield:
		case Moves::Play_Nice:
		case Moves::Confide:
		case Moves::Diamond_Storm:
		case Moves::Steam_Eruption:
		case Moves::Hyperspace_Hole:
		case Moves::Water_Shuriken:
		case Moves::Mystical_Fire:
		case Moves::Spiky_Shield:
		case Moves::Aromatic_Mist:
		case Moves::Eerie_Impulse:
		case Moves::Venom_Drench:
		case Moves::Powder:
		case Moves::Geomancy:
		case Moves::Magnetic_Flux:
		case Moves::Happy_Hour:
		case Moves::Electric_Terrain:
		case Moves::Dazzling_Gleam:
		case Moves::Celebrate:
		case Moves::Hold_Hands:
		case Moves::Baby_Doll_Eyes:
		case Moves::Nuzzle:
		case Moves::Hold_Back:
		case Moves::Infestation:
		case Moves::Power_Up_Punch:
		case Moves::Oblivion_Wing:
		case Moves::Thousand_Arrows:
		case Moves::Thousand_Waves:
		case Moves::Lands_Wrath:
		case Moves::Light_of_Ruin:
		case Moves::Origin_Pulse:
		case Moves::Precipice_Blades:
		case Moves::Dragon_Ascent:
		case Moves::Hyperspace_Fury:
		case Moves::Breakneck_Blitz:
		case Moves::All_Out_Pummeling:
		case Moves::Supersonic_Skystrike:
		case Moves::Acid_Downpour:
		case Moves::Tectonic_Rage:
		case Moves::Continental_Crush:
		case Moves::Savage_Spin_Out:
		case Moves::Never_Ending_Nightmare:
		case Moves::Corkscrew_Crash:
		case Moves::Inferno_Overdrive:
		case Moves::Hydro_Vortex:
		case Moves::Bloom_Doom:
		case Moves::Gigavolt_Havoc:
		case Moves::Shattered_Psyche:
		case Moves::Subzero_Slammer:
		case Moves::Devastating_Drake:
		case Moves::Black_Hole_Eclipse:
		case Moves::Twinkle_Tackle:
		case Moves::Catastropika:
		case Moves::Shore_Up:
		case Moves::First_Impression:
		case Moves::Baneful_Bunker:
		case Moves::Spirit_Shackle:
		case Moves::Darkest_Lariat:
		case Moves::Sparkling_Aria:
		case Moves::Ice_Hammer:
		case Moves::Floral_Healing:
		case Moves::High_Horsepower:
		case Moves::Strength_Sap:
		case Moves::Solar_Blade:
		case Moves::Leafage:
		case Moves::Spotlight:
		case Moves::Toxic_Thread:
		case Moves::Laser_Focus:
		case Moves::Gear_Up:
		case Moves::Throat_Chop:
		case Moves::Pollen_Puff:
		case Moves::Anchor_Shot:
		case Moves::Psychic_Terrain:
		case Moves::Lunge:
		case Moves::Fire_Lash:
		case Moves::Power_Trip:
		case Moves::Burn_Up:
		case Moves::Speed_Swap:
		case Moves::Smart_Strike:
		case Moves::Purify:
		case Moves::Revelation_Dance:
		case Moves::Core_Enforcer:
		case Moves::Trop_Kick:
		case Moves::Instruct:
		case Moves::Beak_Blast:
		case Moves::Clanging_Scales:
		case Moves::Dragon_Hammer:
		case Moves::Brutal_Swing:
		case Moves::Aurora_Veil:
		case Moves::Sinister_Arrow_Raid:
		case Moves::Malicious_Moonsault:
		case Moves::Oceanic_Operetta:
		case Moves::Guardian_of_Alola:
		case Moves::Soul_Stealing_7_Star_Strike:
		case Moves::Stoked_Sparksurfer:
		case Moves::Pulverizing_Pancake:
		case Moves::Extreme_Evoboost:
		case Moves::Genesis_Supernova:
		case Moves::Shell_Trap:
		case Moves::Fleur_Cannon:
		case Moves::Psychic_Fangs:
		case Moves::Stomping_Tantrum:
		case Moves::Shadow_Bone:
		case Moves::Accelerock:
		case Moves::Liquidation:
		case Moves::Prismatic_Laser:
		case Moves::Spectral_Thief:
		case Moves::Sunsteel_Strike:
		case Moves::Moongeist_Beam:
		case Moves::Tearful_Look:
		case Moves::Zing_Zap:
		case Moves::Natures_Madness:
		case Moves::Multi_Attack:
		case Moves::m10000000_Volt_Thunderbolt:
		case Moves::Mind_Blown:
		case Moves::Plasma_Fists:
		case Moves::Photon_Geyser:
		case Moves::Light_That_Burns_the_Sky:
		case Moves::Searing_Sunraze_Smash:
		case Moves::Menacing_Moonraze_Maelstrom:
		case Moves::Lets_Snuggle_Forever:
		case Moves::Splintered_Stormshards:
		case Moves::Clangorous_Soulblaze:
		case Moves::Zippy_Zap:
		case Moves::Splishy_Splash:
		case Moves::Floaty_Fall:
		case Moves::Pika_Papow:
		case Moves::Bouncy_Bubble:
		case Moves::Buzzy_Buzz:
		case Moves::Sizzly_Slide:
		case Moves::Glitzy_Glow:
		case Moves::Baddy_Bad:
		case Moves::Sappy_Seed:
		case Moves::Freezy_Frost:
		case Moves::Sparkly_Swirl:
		case Moves::Veevee_Volley:
		case Moves::Double_Iron_Bash:
			return generic_probability(1.0);
	}
}

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION(generation) \
	template auto all_probabilities<generation>(Moves, ActivePokemon<generation> const user, Team<generation> const & other) -> Probabilities

TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::one);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::two);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::three);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::four);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::five);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::six);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::seven);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::eight);

} // namespace technicalmachine
