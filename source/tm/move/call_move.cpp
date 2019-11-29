// Copyright (C) 2019 David Stone
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

#include <tm/move/call_move.hpp>

#include <tm/move/is_switch.hpp>
#include <tm/move/move.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/target.hpp>
#include <tm/move/will_be_recharge_turn.hpp>

#include <tm/ability.hpp>
#include <tm/block.hpp>
#include <tm/heal.hpp>
#include <tm/rational.hpp>
#include <tm/status.hpp>
#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <tm/pokemon/active_pokemon.hpp>
#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/species.hpp>

#include <tm/stat/stat.hpp>

#include <tm/type/effectiveness.hpp>
#include <tm/type/type.hpp>

#include <bounded/assert.hpp>

#include <containers/algorithms/maybe_find.hpp>

#include <utility>

namespace technicalmachine {
namespace {

// I could potentially treat this as negative recoil
auto absorb_hp(Pokemon & user, Pokemon const & target, HP::current_type const damage) -> void {
	auto const absorbed = damage / 2_bi;
	if (damages_leechers(get_ability(target))) {
		change_hp(user, -absorbed);
	} else {
		change_hp(user, absorbed);
	}
}


auto cure_all_status(Team & user, auto const & predicate) -> void {
	for (auto & pokemon : user.all_pokemon()) {
		if (predicate(pokemon)) {
			clear_status(pokemon);
		}
	}
}


auto belly_drum(MutableActivePokemon user) {
	auto const hp = get_hp(user);
	if (hp.current() > hp.max() / 2_bi and hp.current() > 1_bi) {
		change_hp(user, -hp.max() / 2_bi);
		boost(user.stage(), StatNames::ATK, 12_bi);
	}
}


auto can_confuse_with_chatter(Species const pokemon) {
	return pokemon == Species::Chatot;
}


auto curse(MutableActivePokemon user, MutableActivePokemon target) {
	if (is_type(user, Type::Ghost, user.is_roosting()) and !blocks_secondary_damage(get_ability(user))) {
		if (!target.is_cursed()) {
			user.indirect_damage(get_hp(user).max() / 2_bi);
			target.curse();
		}
	} else {
		auto & stat_stage = user.stage();
		boost_physical(stat_stage, 1_bi);
		boost(stat_stage, StatNames::SPE, -1_bi);
	}
}


auto fling_side_effects(Generation const generation, Pokemon & user, MutableActivePokemon target, Weather const weather) {
	// TODO: Activate berry
	auto apply_status = [&](Statuses const status) {
		// Uproar is irrelevant in this function
		constexpr auto uproaring = false;
		apply(status, user, target, weather, uproaring);
	};
	switch (get_item(user)) {
		case Item::Flame_Orb:
			apply_status(Statuses::burn);
			break;
		case Item::Kings_Rock:
		case Item::Razor_Fang:
			target.flinch();
			break;
		case Item::Light_Ball:
			apply_status(Statuses::paralysis);
			break;
		case Item::Mental_Herb:
			apply_mental_herb(generation, target);
			break;
		case Item::Poison_Barb:
			apply_status(Statuses::poison);
			break;
		case Item::Toxic_Orb:
			apply_status(Statuses::toxic);
			break;
		case Item::White_Herb:
			apply_white_herb(target);
			break;
		default:
			break;
	}
	// TODO: Some items cannot be lost
	set_item(user, Item::None);
}


void recoil(Pokemon & user, HP::current_type const damage, bounded::checked_integer<1, 4> const denominator) {
	if (!blocks_recoil(get_ability(user))) {
		change_hp(user, -bounded::max(damage / denominator, 1_bi));
	}
}

auto recoil_status(Pokemon & user, Pokemon & target, Weather const weather, HP::current_type const damage, Variable const variable, Statuses const status) {
	// Uproar is irrelevant in this function
	constexpr auto uproaring = false;
	recoil(user, damage, 3_bi);
	if (variable.effect_activates()) {
		apply(status, user, target, weather, uproaring);
	}
}


auto confusing_stat_boost(MutableActivePokemon target, StatNames const stat, bounded::checked_integer<1, 2> const stages) {
	boost(target.stage(), stat, stages);
	target.confuse();
}


auto equalize_hp(Pokemon & lhs, Pokemon & rhs) {
	auto const average = (get_hp(lhs).current() + get_hp(rhs).current()) / 2_bi;
	set_hp(lhs, average);
	set_hp(rhs, average);
}


auto active_pokemon_can_be_phazed(Team const & team) {
	return !team.pokemon().ingrained() and !blocks_phazing(get_ability(team.pokemon())) and size(team.all_pokemon()) > 1_bi;
}

auto phaze(Generation const generation, Team & user, Team & target, Weather & weather, Variable const variable) {
	if (active_pokemon_can_be_phazed(target)) {
		target.switch_pokemon(generation, user, weather, variable.phaze_index());
		target.pokemon().update_before_move();
	}
}


auto struggle(Pokemon & user) {
	change_hp(user, -get_hp(user).max() / 4_bi);
}


auto use_swallow(MutableActivePokemon user) {
	auto const stockpiles = user.release_stockpile();
	if (stockpiles == 0_bi) {
		return;
	}
	heal(user, swallow_healing(bounded::integer<1, Stockpile::max>(stockpiles)));
}


auto swap_items(Pokemon & user, Pokemon & other) {
	// Add support for abilities that block Trick / Switcheroo
	if (!blocks_trick(get_item(user)) and !blocks_trick(get_item(other))) {
		auto const user_item = get_item(user);
		auto const other_item = get_item(other);
		set_item(user, other_item);
		set_item(other, user_item);
	}
}


auto do_side_effects(Generation const generation, Team & user_team, ExecutedMove const move, Type const move_type, Team & other, Weather & weather, HP::current_type const damage) {
	auto user = user_team.pokemon();
	switch (move.name) {
		case Moves::Absorb:
		case Moves::Drain_Punch:
		case Moves::Giga_Drain:
		case Moves::Leech_Life:
		case Moves::Mega_Drain:
			absorb_hp(user, other.pokemon(), damage);
			break;
		case Moves::Acid:
		case Moves::Bug_Buzz:
		case Moves::Earth_Power:
		case Moves::Energy_Ball:
		case Moves::Flash_Cannon:
		case Moves::Focus_Blast:
		case Moves::Luster_Purge:
		case Moves::Psychic:
		case Moves::Shadow_Ball:
			if (move.variable.effect_activates()) {
				boost(other.pokemon().stage(), StatNames::SPD, -1_bi);
			}
			break;
		case Moves::Acid_Armor:
		case Moves::Barrier:
		case Moves::Iron_Defense:
			boost(user.stage(), StatNames::DEF, 2_bi);
			break;
		case Moves::Acupressure:
			boost(user.stage(), move.variable.acupressure_stat_boost(), 2_bi);
			break;
		case Moves::Agility:
		case Moves::Rock_Polish:
			boost(user.stage(), StatNames::SPE, 2_bi);
			break;
		case Moves::Air_Slash:
		case Moves::Astonish:
		case Moves::Bite:
		case Moves::Bone_Club:
		case Moves::Dark_Pulse:
		case Moves::Dragon_Rush:
		case Moves::Extrasensory:
		case Moves::Fake_Out:
		case Moves::Headbutt:
		case Moves::Hyper_Fang:
		case Moves::Iron_Head:
		case Moves::Needle_Arm:
		case Moves::Rock_Slide:
		case Moves::Rolling_Kick:
		case Moves::Snore:
		case Moves::Stomp:
		case Moves::Twister:
		case Moves::Waterfall:
		case Moves::Zen_Headbutt:
			if (move.variable.effect_activates()) {
				other.pokemon().flinch();
			}
			break;
		case Moves::Amnesia:
			boost(user.stage(), StatNames::SPD, 2_bi);
			break;
		case Moves::Ancient_Power:
		case Moves::Ominous_Wind:
		case Moves::Silver_Wind:
			if (move.variable.effect_activates()) {
				boost_regular(user.stage(), 1_bi);
			}
			break;
		case Moves::Aqua_Ring:
			user.activate_aqua_ring();
			break;
		case Moves::Aurora_Beam:
			if (move.variable.effect_activates()) {
				boost(other.pokemon().stage(), StatNames::ATK, -1_bi);
			}
			break;
		case Moves::Aromatherapy:
			cure_all_status(user_team, [](Pokemon const &) { return true; });
			break;
		case Moves::Attract:
			if (multiplier(get_gender(user), get_gender(other.pokemon())) == -1_bi) {
				other.pokemon().attract(generation);
			}
			break;
		case Moves::Baton_Pass:
			if (user_team.size() > 1_bi) {
				user.baton_pass();
			}
			break;
		case Moves::Belly_Drum:
			belly_drum(user);
			break;
		case Moves::Bide:
			user.use_bide(other.pokemon());
			break;
		case Moves::Bind:
		case Moves::Clamp:
		case Moves::Fire_Spin:
		case Moves::Magma_Storm:
		case Moves::Sand_Tomb:
		case Moves::Whirlpool:
		case Moves::Wrap:
			other.pokemon().partially_trap();
			break;
		case Moves::Blast_Burn:
		case Moves::Frenzy_Plant:
		case Moves::Giga_Impact:
		case Moves::Hydro_Cannon:
		case Moves::Hyper_Beam:
		case Moves::Roar_of_Time:
		case Moves::Rock_Wrecker:
			user.use_recharge_move();
			break;
		case Moves::Blaze_Kick:
		case Moves::Ember:
		case Moves::Fire_Blast:
		case Moves::Fire_Punch:
		case Moves::Flame_Wheel:
		case Moves::Flamethrower:
		case Moves::Heat_Wave:
		case Moves::Lava_Plume:
		case Moves::Sacred_Fire:
			if (move.variable.effect_activates()) {
				constexpr auto uproar = false;
				apply(Statuses::burn, user, other.pokemon(), weather, uproar);
			}
			break;
		case Moves::Blizzard:
		case Moves::Ice_Beam:
		case Moves::Ice_Punch:
		case Moves::Powder_Snow:
			if (move.variable.effect_activates()) {
				constexpr auto uproar = false;
				apply(Statuses::freeze, user, other.pokemon(), weather, uproar);
			}
			break;
		case Moves::Block:
		case Moves::Mean_Look:
		case Moves::Spider_Web:
			other.pokemon().fully_trap();
			break;
		case Moves::Body_Slam:
		case Moves::Discharge:
		case Moves::Dragon_Breath:
		case Moves::Force_Palm:
		case Moves::Lick:
		case Moves::Secret_Power:
		case Moves::Spark:
		case Moves::Thunder:
		case Moves::Thunderbolt:
		case Moves::Thunder_Punch:
		case Moves::Thunder_Shock:
			if (move.variable.effect_activates()) {
				constexpr auto uproar = false;
				apply(Statuses::paralysis, user, other.pokemon(), weather, uproar);
			}
			break;
		case Moves::Bounce:
			if (user.bounce() and move.variable.effect_activates()) {
				constexpr auto uproar = false;
				apply(Statuses::paralysis, user, other.pokemon(), weather, uproar);
			}
			break;
		case Moves::Brave_Bird:
		case Moves::Double_Edge:
		case Moves::Wood_Hammer:
			recoil(user, damage, 3_bi);
			break;
		case Moves::Bubble:
		case Moves::Bubble_Beam:
		case Moves::Constrict:
			if (move.variable.effect_activates()) {
				boost(other.pokemon().stage(), StatNames::SPE, -1_bi);
			}
			break;
		case Moves::Bug_Bite:			// Fix
		case Moves::Pluck:
			break;
		case Moves::Bulk_Up:
			boost_physical(user.stage(), 1_bi);
			break;
		case Moves::Calm_Mind:
			boost_special(user.stage(), 1_bi);
			break;
		case Moves::Camouflage:
			break;
		case Moves::Captivate:
			if (multiplier(get_gender(user), get_gender(other.pokemon())) == -1_bi) {
				boost(other.pokemon().stage(), StatNames::SPD, -2_bi);
			}
			break;
		case Moves::Charge:
			user.charge();
			boost(user.stage(), StatNames::SPD, 1_bi);
			break;
		case Moves::Charge_Beam:
			if (move.variable.effect_activates()) {
				boost(user.stage(), StatNames::SPA, 1_bi);
			}
			break;
		case Moves::Charm:
		case Moves::Feather_Dance:
			boost(other.pokemon().stage(), StatNames::ATK, -2_bi);
			break;
		case Moves::Chatter:
			if (can_confuse_with_chatter(get_species(user)) and move.variable.effect_activates()) {
				other.pokemon().confuse();
			}
			break;
		case Moves::Close_Combat:
			boost_physical(user.stage(), -1_bi);
			break;
		case Moves::Confuse_Ray:
		case Moves::Supersonic:
		case Moves::Sweet_Kiss:
		case Moves::Teeter_Dance:
			other.pokemon().confuse();
			break;
		case Moves::Confusion:
		case Moves::Dizzy_Punch:
		case Moves::Dynamic_Punch:
		case Moves::Psybeam:
		case Moves::Rock_Climb:
		case Moves::Signal_Beam:
		case Moves::Water_Pulse:
			if (move.variable.effect_activates()) {
				other.pokemon().confuse();
			}
			break;
		case Moves::Conversion:		// Fix
			break;
		case Moves::Conversion_2:	// Fix
			break;
		case Moves::Cosmic_Power:
		case Moves::Defend_Order:
			boost_defensive(user.stage(), 1_bi);
			break;
		case Moves::Crunch:
		case Moves::Crush_Claw:
		case Moves::Iron_Tail:
		case Moves::Rock_Smash:
			if (move.variable.effect_activates()) {
				boost(other.pokemon().stage(), StatNames::DEF, -1_bi);
			}
			break;
		case Moves::Cotton_Spore:
		case Moves::Scary_Face:
			boost(other.pokemon().stage(), StatNames::SPE, -2_bi);
			break;
		case Moves::Covet:
		case Moves::Thief: {
			// TODO: Sticky Hold
			auto const user_item = get_item(user);
			auto const other_item = get_item(other.pokemon());
			if (user_item == Item::None and !blocks_trick(other_item)) {
				set_item(user, other_item);
				set_item(other.pokemon(), Item::None);
			}
			break;
		}
		case Moves::Cross_Poison:
		case Moves::Gunk_Shot:
		case Moves::Poison_Jab:
		case Moves::Poison_Sting:
		case Moves::Poison_Tail:
		case Moves::Sludge:
		case Moves::Sludge_Bomb:
		case Moves::Smog:
			if (move.variable.effect_activates()) {
				constexpr auto uproar = false;
				apply(Statuses::poison, user, other.pokemon(), weather, uproar);
			}
			break;
		case Moves::Curse:
			curse(user, other.pokemon());
			break;
		case Moves::Dark_Void:
		case Moves::Grass_Whistle:
		case Moves::Hypnosis:
		case Moves::Lovely_Kiss:
		case Moves::Sing:
		case Moves::Sleep_Powder:
		case Moves::Spore:
			apply(Statuses::sleep, user, other.pokemon(), weather, user.is_uproaring() or other.pokemon().is_uproaring());
			break;
		case Moves::Defense_Curl:
			boost(user.stage(), StatNames::DEF, 1_bi);
			user.defense_curl();
			break;
		case Moves::Defog:
			weather.deactivate_fog();
			boost(other.pokemon().stage(), StatNames::EVA, -1_bi);
			break;
		case Moves::Destiny_Bond:
			user.use_destiny_bond();
			break;
		case Moves::Detect:
		case Moves::Protect:
			user.protect();
			break;
		case Moves::Dig:
			user.dig();
			break;
		case Moves::Disable:
			other.pokemon().disable(generation, other.pokemon().last_used_move().name());
			break;
		case Moves::Dive:
			user.dive();
			break;
		case Moves::Doom_Desire:	// Fix
		case Moves::Future_Sight:
			break;
		case Moves::Double_Team:
		case Moves::Minimize:
			boost(user.stage(), StatNames::EVA, 1_bi);
			break;
		case Moves::Draco_Meteor:
		case Moves::Leaf_Storm:
		case Moves::Overheat:
		case Moves::Psycho_Boost:
			boost(user.stage(), StatNames::SPA, -2_bi);
			break;
		case Moves::Dragon_Dance:
			boost(user.stage(), StatNames::ATK, 1_bi);
			boost(user.stage(), StatNames::SPE, 1_bi);
			break;
		case Moves::Dream_Eater:
			if (is_sleeping(get_status(other.pokemon()))) {
				absorb_hp(user, other.pokemon(), damage);
			}
			break;
		case Moves::Embargo:
			other.pokemon().activate_embargo();
			break;
		case Moves::Encore:
			other.pokemon().activate_encore(generation);
			break;
		case Moves::Endure:
			user.endure();
			break;
		case Moves::Explosion:
		case Moves::Self_Destruct:
			set_hp(user, 0_bi);
			break;
		case Moves::Fake_Tears:
		case Moves::Metal_Sound:
			boost(other.pokemon().stage(), StatNames::SPD, -2_bi);
			break;
		case Moves::Feint:
		case Moves::Hyperspace_Fury:
		case Moves::Hyperspace_Hole:
		case Moves::Phantom_Force:
			other.pokemon().break_protect();
			break;
		case Moves::Fire_Fang:
			move.variable.fang_side_effects(user, other.pokemon(), weather, Statuses::burn);
			break;
		case Moves::Flare_Blitz:
			recoil_status(user, other.pokemon(), weather, damage, move.variable, Statuses::burn);
			break;
		case Moves::Flash:
		case Moves::Kinesis:
		case Moves::Mud_Slap:
		case Moves::Sand_Attack:
		case Moves::Smokescreen:
			boost(other.pokemon().stage(), StatNames::ACC, -1_bi);
			break;
		case Moves::Flatter:
			confusing_stat_boost(other.pokemon(), StatNames::SPA, 1_bi);
			break;
		case Moves::Fling:
			fling_side_effects(generation, user, other.pokemon(), weather);
			break;
		case Moves::Fly:
			user.fly();
			break;
		case Moves::Focus_Energy:
			user.focus_energy();
			break;
		case Moves::Follow_Me:		// Fix
			break;
		case Moves::Foresight:
		case Moves::Odor_Sleuth:
			other.pokemon().identify();
			break;
		case Moves::Gastro_Acid:		// Fix
			break;
		case Moves::Glare:
		case Moves::Stun_Spore:
		case Moves::Thunder_Wave:
		case Moves::Zap_Cannon: {
			constexpr auto uproar = false;
			apply(Statuses::paralysis, user, other.pokemon(), weather, uproar);
			break;
		}
		case Moves::Gravity:
			weather.activate_gravity();
			break;
		case Moves::Growl:
			boost(other.pokemon().stage(), StatNames::ATK, -1_bi);
			break;
		case Moves::Growth:
			boost(user.stage(), StatNames::SPA, 1_bi);
			break;
		case Moves::Grudge:		// Fix
			break;
		case Moves::Guard_Swap:
			swap_defensive(user.stage(), other.pokemon().stage());
			break;
		case Moves::Hail:
			weather.activate_hail(extends_hail(get_item(user)));
			break;
		case Moves::Hammer_Arm:
			boost(user.stage(), StatNames::SPE, -1_bi);
			break;
		case Moves::Harden:
		case Moves::Withdraw:
			boost(user.stage(), StatNames::DEF, 1_bi);
			break;
		case Moves::Haze:
			user.stage() = Stage{};
			other.pokemon().stage() = Stage{};
			break;
		case Moves::Head_Smash:
			recoil(user, damage, 2_bi);
			break;
		case Moves::Heal_Bell:
			cure_all_status(user_team, [](Pokemon const & pokemon) {
				return !blocks_sound_moves(get_ability(pokemon));
			});
			break;
		case Moves::Heal_Block:
			other.pokemon().activate_heal_block();
			break;
		case Moves::Heal_Order:
		case Moves::Milk_Drink:
		case Moves::Recover:
		case Moves::Slack_Off:
		case Moves::Soft_Boiled:
			heal(user, rational(1_bi, 2_bi));
			break;
		case Moves::Healing_Wish:		// Fix
			break;
		case Moves::Heart_Swap:
			std::swap(user.stage(), other.pokemon().stage());
			break;
		case Moves::High_Jump_Kick:		// Fix
		case Moves::Jump_Kick:
			break;
		case Moves::Howl:
		case Moves::Metal_Claw:
		case Moves::Meditate:
		case Moves::Meteor_Mash:
			if (move.variable.effect_activates()) {
				boost(user.stage(), StatNames::ATK, 1_bi);
			}
			break;
		case Moves::Ice_Ball:		// Fix
		case Moves::Rollout:
			break;
		case Moves::Ice_Fang:
			move.variable.fang_side_effects(user, other.pokemon(), weather, Statuses::freeze);
			break;
		case Moves::Icy_Wind:
		case Moves::Mud_Shot:
		case Moves::Rock_Tomb:
		case Moves::String_Shot:
			boost(other.pokemon().stage(), StatNames::SPE, -1_bi);
			break;
		case Moves::Imprison:
			user.use_imprison();
			break;
		case Moves::Ingrain:
			user.ingrain();
			break;
		case Moves::Knock_Off:		// Fix
			break;
		case Moves::Leech_Seed:
			other.pokemon().hit_with_leech_seed();
			break;
		case Moves::Leer:
		case Moves::Tail_Whip:
			boost(other.pokemon().stage(), StatNames::DEF, -1_bi);
			break;
		case Moves::Light_Screen:
			user_team.screens.activate_light_screen(extends_light_screen(get_item(user)));
			break;
		case Moves::Lock_On:
		case Moves::Mind_Reader:
			user.use_lock_on();
			break;
		case Moves::Lucky_Chant:
			user_team.screens.activate_lucky_chant();
			break;
		case Moves::Lunar_Dance:		// Fix
			break;
		case Moves::Magic_Coat:		// Fix
			break;
		case Moves::Magnet_Rise:
			user.activate_magnet_rise();
			break;
		case Moves::Me_First:		// Fix
			break;
		case Moves::Memento:
			boost_offensive(other.pokemon().stage(), -2_bi);
			set_hp(user, 0_bi);
			break;
		case Moves::Mimic:		// Fix
			break;
		case Moves::Miracle_Eye:		// Fix
			break;
		case Moves::Mirror_Shot:
		case Moves::Mud_Bomb:
		case Moves::Muddy_Water:
		case Moves::Octazooka:
			if (move.variable.effect_activates()) {
				boost(other.pokemon().stage(), StatNames::ACC, -1_bi);
			}
			break;
		case Moves::Mist:
			user_team.screens.activate_mist();
			break;
		case Moves::Mist_Ball:
			if (move.variable.effect_activates()) {
				boost(other.pokemon().stage(), StatNames::SPA, -1_bi);
			}
			break;
		case Moves::Moonlight:
		case Moves::Morning_Sun:
		case Moves::Synthesis: {
			auto const amount = [weather]() {
				using Numerator = bounded::integer<1, 2>;
				using Denominator = bounded::integer<2, 4>;
				using Result = rational<Numerator, Denominator>;
				if (weather.sun()) {
					return Result(2_bi, 3_bi);
				} else if (weather.hail() or weather.rain() or weather.sand()) {
					return Result(1_bi, 4_bi);
				} else {
					return Result(1_bi, 2_bi);
				}
			};
			heal(user, amount());
			break;
		}
		case Moves::Mud_Sport:
			user.activate_mud_sport();
			break;
		case Moves::Nasty_Plot:
		case Moves::Tail_Glow:
			boost(user.stage(), StatNames::SPA, 2_bi);
			break;
		case Moves::Nightmare:
			other.pokemon().give_nightmares();
			break;
		case Moves::Outrage:
		case Moves::Petal_Dance:
		case Moves::Thrash:
			user.activate_rampage();
			break;
		case Moves::Pain_Split:
			equalize_hp(user, other.pokemon());
			break;
		case Moves::Perish_Song:
			user.activate_perish_song();
			other.pokemon().activate_perish_song();
			break;
		case Moves::Poison_Fang:
			if (move.variable.effect_activates()) {
				constexpr auto uproar = false;
				apply(Statuses::toxic, user, other.pokemon(), weather, uproar);
			}
			break;
		case Moves::Poison_Gas:
		case Moves::Poison_Powder: {
			constexpr auto uproar = false;
			apply(Statuses::poison, user, other.pokemon(), weather, uproar);
			break;
		}
		case Moves::Power_Swap:
			swap_offensive(user.stage(), other.pokemon().stage());
			break;
		case Moves::Power_Trick:
			user.activate_power_trick();
			break;
		case Moves::Present:
			if (move.variable.present_heals()) {
				change_hp(other.pokemon(), 80_bi);
			}
			break;
		case Moves::Psych_Up:
			user.stage() = other.pokemon().stage();
			break;
		case Moves::Psycho_Shift:
			if (is_clear(get_status(other.pokemon()))) {
				shift_status(user, other.pokemon(), weather);
			}
			break;
		case Moves::Rage:		// Fix
			break;
		case Moves::Rain_Dance:
			weather.activate_rain(extends_rain(get_item(user)));
			break;
		case Moves::Rapid_Spin:
			if (!Effectiveness(generation, move_type, other.pokemon()).has_no_effect()) {
				user_team.clear_field();
			}
			break;
		case Moves::Razor_Wind:	// Fix
			break;
		case Moves::Recycle:		// Fix
			break;
		case Moves::Reflect:
			user_team.screens.activate_reflect(extends_reflect(get_item(user)));
			break;
		case Moves::Refresh:
			clear_status(user);
			break;
		case Moves::Rest:
			rest(user, other.pokemon().is_uproaring());
			break;
		case Moves::Roar:
		case Moves::Whirlwind:
			phaze(generation, user_team, other, weather, move.variable);
			break;
		case Moves::Role_Play:		// Fix
			break;
		case Moves::Roost:
			user.roost();
			heal(user, rational(1_bi, 2_bi));
			break;
		case Moves::Safeguard:
			user_team.screens.activate_safeguard();
			break;
		case Moves::Sandstorm:
			weather.activate_sand(extends_sand(get_item(user)));
			break;
		case Moves::Screech:
			boost(other.pokemon().stage(), StatNames::DEF, -2_bi);
			break;
		case Moves::Seed_Flare:
			if (move.variable.effect_activates()) {
				boost(other.pokemon().stage(), StatNames::SPD, -2_bi);
			}
			break;
		case Moves::Shadow_Force:
			user.shadow_force();
			// TODO: other.pokemon().break_protect();
			break;
		case Moves::Sharpen:
			boost(user.stage(), StatNames::ATK, 1_bi);
			break;
		case Moves::Sketch:		// Fix
			break;
		case Moves::Skill_Swap:		// Fix
			break;
		case Moves::Skull_Bash: // Fix
			break;
		case Moves::Sky_Attack:
			if (move.variable.effect_activates()) {
				other.pokemon().flinch();
			}
			break;
		case Moves::Smelling_Salts:
			if (boosts_smellingsalt(get_status(other.pokemon()))) {
				get_status(other.pokemon()) = Status{};
			}
			break;
		case Moves::Snatch:	// Fix
			break;
		case Moves::Solar_Beam:
			// TODO: Support Power Herb
			if (will_be_recharge_turn(user, move.name, weather)) {
				user.use_charge_up_move();
			}
			break;
		case Moves::Spikes:
			other.entry_hazards.add_spikes();
			break;
		case Moves::Spit_Up:
			user.release_stockpile();
			break;
		case Moves::Spite:		// Fix
			break;
		case Moves::Stealth_Rock:
			other.entry_hazards.add_stealth_rock();
			break;
		case Moves::Steel_Wing:
			if (move.variable.effect_activates()) {
				boost(user.stage(), StatNames::DEF, 1_bi);
			}
			break;
		case Moves::Stockpile:
			user.increment_stockpile();
			break;
		case Moves::Struggle:
			struggle(user);
			break;
		case Moves::Submission:
		case Moves::Take_Down:
			recoil(user, damage, 4_bi);
			break;
		case Moves::Substitute:
			user.use_substitute();		
			break;
		case Moves::Sunny_Day:
			weather.activate_sun(extends_sun(get_item(user)));
			break;
		case Moves::Superpower:
			boost_physical(user.stage(), -1_bi);
			break;
		case Moves::Swagger:
			confusing_stat_boost(other.pokemon(), StatNames::ATK, 2_bi);
			break;
		case Moves::Swallow:
			use_swallow(user);
			break;
		case Moves::Sweet_Scent:
			boost(other.pokemon().stage(), StatNames::EVA, -1_bi);
			break;
		case Moves::Switch0:
		case Moves::Switch1:
		case Moves::Switch2:
		case Moves::Switch3:
		case Moves::Switch4:
		case Moves::Switch5:
			user_team.switch_pokemon(generation, other, weather, to_replacement(move.name));
			break;
		case Moves::Switcheroo:
		case Moves::Trick:
			swap_items(user, other.pokemon());
			break;
		case Moves::Swords_Dance:
			boost(user.stage(), StatNames::ATK, 2_bi);
			break;
		case Moves::Tailwind:
			user_team.screens.activate_tailwind();
			break;
		case Moves::Taunt:
			other.pokemon().taunt(generation);
			break;
		case Moves::Thunder_Fang:
			move.variable.fang_side_effects(user, other.pokemon(), weather, Statuses::paralysis);
			break;
		case Moves::Tickle:
			boost_physical(other.pokemon().stage(), -1_bi);
			break;
		case Moves::Torment:
			other.pokemon().torment(generation);
			break;
		case Moves::Toxic: {
			constexpr auto uproar = false;
			apply(Statuses::toxic, user, other.pokemon(), weather, uproar);
			break;
		}
		case Moves::Toxic_Spikes:
			other.entry_hazards.add_toxic_spikes();
			break;
		case Moves::Transform:		// Fix
			break;
		case Moves::Tri_Attack:
			if (auto const status = move.variable.tri_attack_status(); status != Statuses::clear) {
				constexpr auto uproar = false;
				apply(status, user, other.pokemon(), weather, uproar);
			}
			break;
		case Moves::Trick_Room:
			weather.activate_trick_room();
			break;
		case Moves::U_turn:
			if (user_team.size() > 1_bi) {
				user.u_turn();
			}
			break;
		case Moves::Uproar:
			user.use_uproar();
			break;
		case Moves::Volt_Tackle:
			recoil_status(user, other.pokemon(), weather, damage, move.variable, Statuses::paralysis);
			break;
		case Moves::Wake_Up_Slap:
			if (is_sleeping(get_status(other.pokemon()))) {
				get_status(other.pokemon()) = Status{};
			}
			break;
		case Moves::Water_Sport:
			user.activate_water_sport();
			break;
		case Moves::Will_O_Wisp: {
			constexpr auto uproar = false;
			apply(Statuses::burn, user, other.pokemon(), weather, uproar);
			break;
		}
		case Moves::Wish:
			user_team.wish.activate();
			break;
		case Moves::Worry_Seed:		// Fix
			break;
		case Moves::Yawn:
			other.pokemon().hit_with_yawn();
			break;
		default:
			break;
	}
}


constexpr auto breaks_screens(Moves const move) {
	return move == Moves::Brick_Break;
}

auto do_effects_before_moving(Moves const move, Pokemon & user, Team & other) {
	if (breaks_screens(move)) {
		other.screens.shatter();
	} else if (is_usable_while_frozen(move)) {
		if (is_frozen(get_status(user))) {
			clear_status(user);
		}
	}
}


constexpr auto move_fails(Moves const move, bool const user_damaged, Ability const other_ability, OtherMove const other_move) {
	switch (move) {
		case Moves::Bug_Buzz:
		case Moves::Chatter:
		case Moves::Grass_Whistle:
		case Moves::Growl:
		case Moves::Hyper_Voice:
		case Moves::Metal_Sound:
		case Moves::Roar:
		case Moves::Screech:
		case Moves::Sing:
		case Moves::Snore:
		case Moves::Supersonic:
		case Moves::Uproar:
			return blocks_sound_moves(other_ability);
		case Moves::Focus_Punch:
			return user_damaged;
		case Moves::Sucker_Punch:
			return !other_move.future_move_is_damaging();
		default:
			return false;
	}
}


auto use_move(Generation const generation, Team & user, ExecutedMove const move, Type const move_type, Team & other, OtherMove const other_move, Weather & weather, ActualDamage const actual_damage) -> void {
	auto const user_pokemon = user.pokemon();
	auto const other_pokemon = other.pokemon();
	do_effects_before_moving(move.name, user_pokemon, other);

	auto const damage = actual_damage.value(generation, user, move, other, other_move, weather);

	auto const substitute = other_pokemon.substitute();
	if (damage != 0_bi) {
		other_pokemon.direct_damage(damage);
	}
	if (!substitute or !blocked_by_substitute(generation, move.name)) {
		do_side_effects(generation, user, move, move_type, other, weather, damage);
	}
	auto const item = get_item(user_pokemon);
	if (!static_cast<bool>(substitute) and causes_recoil(item)) {
		heal(user_pokemon, rational(-1_bi, 10_bi));
	} else if (item == Item::Shell_Bell) {
		change_hp(user_pokemon, bounded::max(damage / 8_bi, 1_bi));
	}
}

auto find_move(MoveContainer const container, Moves const move_name) -> Move {
	auto const maybe_move = containers::maybe_find(container, move_name);
	BOUNDED_ASSERT(maybe_move);
	return *maybe_move;
}

auto find_regular_move(RegularMoveContainer & container, Moves const move_name) -> Move & {
	auto const move_ptr = containers::maybe_find(container, move_name);
	BOUNDED_ASSERT_OR_ASSUME(move_ptr);
	return *move_ptr;
}

constexpr auto blocked_by_protect(Target const target, Moves const move) {
	switch (target) {
		case Target::user:
		case Target::all_allies:
		case Target::all:
		case Target::field:
		case Target::user_team:
		case Target::user_field:
		case Target::foe_field:
			return false;
		case Target::adjacent_ally:
		case Target::user_or_adjacent_ally:
		case Target::adjacent_foe:
		case Target::all_adjacent_foes:
		case Target::any:
		case Target::all_adjacent:
			return true;
		case Target::adjacent:
			switch (move) {
				case Moves::Feint:
				case Moves::Hyperspace_Fury:
				case Moves::Hyperspace_Hole:
				case Moves::Phantom_Force:
				case Moves::Shadow_Force:
					return false;
				default:
					return true;
			}
	}
}

constexpr auto fails_against_fainted(Target const target) {
	switch (target) {
		case Target::user:
		case Target::all_allies:
		case Target::all:
		case Target::field:
		case Target::user_team:
		case Target::user_field:
		case Target::all_adjacent_foes:
		case Target::foe_field:
		case Target::all_adjacent:
			return false;
		case Target::adjacent_ally:
		case Target::user_or_adjacent_ally:
		case Target::adjacent_foe:
		case Target::adjacent:
		case Target::any:
			return true;
	}
}

auto activate_pp_restore_berry(Move & move, Pokemon & pokemon) {
	if (move.pp().is_empty()) {
		switch (get_item(pokemon)) {
			case Item::Leppa_Berry:
				move.restore_pp(10_bi);
				set_item(pokemon, Item::None);
				break;
			case Item::MysteryBerry:
				move.restore_pp(5_bi);
				set_item(pokemon, Item::None);
				break;
			default:
				break;
		}
	}
}

}	// namespace

auto call_move(Generation const generation, Team & user, UsedMove const move, Team & other, OtherMove const other_move, Weather & weather, bool const clear_status, ActualDamage const actual_damage) -> void {
	if (move.selected == Moves::Pass) {
		return;
	}
	auto user_pokemon = user.pokemon();
	auto const found_move = find_move(all_moves(user_pokemon), move.selected);
	auto other_pokemon = other.pokemon();
	user_pokemon.update_before_move();
	if (!is_switch(move.selected)) {
		advance_status_from_move(user_pokemon, clear_status);
	}
	// Need the side-effect from recharge
	auto const is_recharging = user_pokemon.recharge();
	if (!can_attempt_move_execution(user_pokemon, found_move, other_pokemon)) {
		return;
	}
	user_pokemon.handle_confusion();
	if (move.executed != Moves::Hit_Self and user_pokemon.flinched()) {
		if (boosts_speed_when_flinched(get_ability(user_pokemon))) {
			boost(user_pokemon.stage(), StatNames::SPE, 1_bi);
		}
	}
	if (!can_execute_move(user_pokemon, found_move, weather, is_recharging)) {
		return;
	}
	
	auto const other_ability = get_ability(other_pokemon);

	if (is_regular(move.selected) and move.executed != Moves::Hit_Self and !user_pokemon.is_locked_in_by_move()) {
		auto & move_ref = find_regular_move(all_moves(user_pokemon).regular(), move.selected);
		move_ref.decrement_pp(other_ability);
		activate_pp_restore_berry(move_ref, user_pokemon);
	}
	
	auto const target = move_target(generation, move.executed);
	// TODO: What happens if we Sleep Talk Trump Card?
	auto const unsuccessful =
		move.miss or
		move_fails(move.executed, user_pokemon.damaged(), other_ability, other_move) or
		(get_hp(other_pokemon).current() == 0_bi and fails_against_fainted(target)) or
		(other_pokemon.is_protecting() and blocked_by_protect(target, move.executed));
	if (unsuccessful) {
		user_pokemon.unsuccessfully_use_move(move.selected);
		return;
	}

	// TODO: Add targeting information
	auto const move_type = get_type(generation, move.executed, get_hidden_power(user_pokemon).type());
	if (move_type == Type::Fire and other_ability == Ability::Flash_Fire) {
		other.pokemon().activate_flash_fire();
	} else {
		auto const executed_move = ExecutedMove{
			move.executed,
			found_move.pp(),
			move.variable,
			move.critical_hit
		};
		use_move(generation, user, executed_move, move_type, other, other_move, weather, actual_damage);
	}
	user_pokemon.increment_move_use_counter(move.selected);
}

}	// namespace technicalmachine
