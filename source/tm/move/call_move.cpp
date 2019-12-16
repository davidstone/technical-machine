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
auto absorb_hp(Generation const generation, MutableActivePokemon user, MutableActivePokemon target, Weather const weather, HP::current_type const damage) -> void {
	auto const absorbed = damage / 2_bi * healing_multiplier(user.item(generation, weather));
	if (damages_leechers(get_ability(target))) {
		change_hp(generation, user, weather, -absorbed);
	} else {
		change_hp(generation, user, weather, absorbed);
	}
}


auto cure_all_status(Team & user, auto const & predicate) -> void {
	for (auto & pokemon : user.all_pokemon()) {
		if (predicate(pokemon)) {
			pokemon.set_status(Statuses::clear);
		}
	}
}


auto belly_drum(Generation const generation, MutableActivePokemon user, Weather const weather) {
	auto const hp = get_hp(user);
	if (hp.current() > hp.max() / 2_bi and hp.current() > 1_bi) {
		change_hp(generation, user, weather, -hp.max() / 2_bi);
		user.stage()[StatNames::ATK] += 12_bi;
	}
}


auto can_confuse_with_chatter(Species const pokemon) {
	return pokemon == Species::Chatot;
}


auto curse(Generation const generation, MutableActivePokemon user, MutableActivePokemon target, Weather const weather) {
	if (is_type(user, Type::Ghost) and !blocks_secondary_damage(get_ability(user))) {
		if (!target.is_cursed()) {
			user.indirect_damage(generation, weather, get_hp(user).max() / 2_bi);
			target.curse();
		}
	} else {
		auto & stat_stage = user.stage();
		boost_physical(stat_stage, 1_bi);
		stat_stage[StatNames::SPE] += -1_bi;
	}
}

constexpr bool uproar_does_not_apply(Statuses const status) {
	switch (status) {
		case Statuses::rest:
		case Statuses::sleep:
			return false;
		default:
			return true;
	}
}

auto fling_side_effects(Generation const generation, MutableActivePokemon user, MutableActivePokemon target, Weather const weather) {
	// TODO: Activate berry
	auto apply_status = [&](Statuses const status) {
		BOUNDED_ASSERT(uproar_does_not_apply(status));
		target.apply_status(generation, status, user, weather);
	};
	switch (user.item(generation, weather)) {
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
	user.remove_item();
}


constexpr bool can_be_incinerated(Item const item, Ability const holder_ability) {
	// TODO: Destroy gems
	return holder_ability != Ability::Sticky_Hold and berry_power(item) != 0_bi;
}



void recoil(Generation const generation, MutableActivePokemon user, Weather const weather, HP::current_type const damage, bounded::checked_integer<1, 4> const denominator) {
	if (!blocks_recoil(get_ability(user))) {
		change_hp(generation, user, weather, -bounded::max(damage / denominator, 1_bi));
	}
}

auto recoil_status(Generation const generation, MutableActivePokemon user, MutableActivePokemon target, Weather const weather, HP::current_type const damage, Variable const variable, Statuses const status) {
	BOUNDED_ASSERT(uproar_does_not_apply(status));
	recoil(generation, user, weather, damage, 3_bi);
	if (variable.effect_activates()) {
		target.apply_status(generation, status, user, weather);
	}
}


auto confusing_stat_boost(Generation const generation, MutableActivePokemon target, Weather const weather, StatNames const stat, bounded::checked_integer<1, 2> const stages) {
	target.stage()[stat] += stages;
	target.confuse(generation, weather);
}


auto equalize_hp(Generation const generation, MutableActivePokemon lhs, MutableActivePokemon rhs, Weather const weather) {
	auto const average = (get_hp(lhs).current() + get_hp(rhs).current()) / 2_bi;
	lhs.set_hp(generation, weather, average);
	rhs.set_hp(generation, weather, average);
}


auto active_pokemon_can_be_phazed(Team const & team) {
	return !team.pokemon().ingrained() and !blocks_phazing(get_ability(team.pokemon())) and size(team.all_pokemon()) > 1_bi;
}

auto phaze(Generation const generation, MutableActivePokemon user, Team & target, Weather & weather, Variable const variable) {
	if (active_pokemon_can_be_phazed(target)) {
		target.switch_pokemon(generation, user, weather, variable.phaze_index());
		target.pokemon().update_before_move();
	}
}


auto struggle(Generation const generation, MutableActivePokemon user, Weather const weather) {
	change_hp(generation, user, weather, -get_hp(user).max() / 4_bi);
}


auto use_swallow(Generation const generation, MutableActivePokemon user, Weather const weather) {
	auto const stockpiles = user.release_stockpile();
	if (stockpiles == 0_bi) {
		return;
	}
	heal(generation, user, weather, swallow_healing(bounded::integer<1, Stockpile::max>(stockpiles)));
}


auto do_side_effects(Generation const generation, Team & user_team, ExecutedMove const move, Type const move_type, Team & other, Weather & weather, HP::current_type const damage) {
	auto user = user_team.pokemon();
	switch (move.name) {
		case Moves::Absorb:
		case Moves::Drain_Punch:
		case Moves::Giga_Drain:
		case Moves::Leech_Life:
		case Moves::Mega_Drain:
			absorb_hp(generation, user, other.pokemon(), weather, damage);
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
				other.pokemon().stage()[StatNames::SPD] += -1_bi;
			}
			break;
		case Moves::Acid_Armor:
		case Moves::Barrier:
		case Moves::Iron_Defense:
			user.stage()[StatNames::DEF] += 2_bi;
			break;
		case Moves::Acupressure:
			user.stage()[move.variable.acupressure_stat_boost()] += 2_bi;
			break;
		case Moves::Agility:
		case Moves::Rock_Polish:
			user.stage()[StatNames::SPE] += 2_bi;
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
		case Moves::Icicle_Crash:
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
			user.stage()[StatNames::SPD] += 2_bi;
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
				other.pokemon().stage()[StatNames::ATK] += -1_bi;
			}
			break;
		case Moves::Aromatherapy:
			cure_all_status(user_team, [](Pokemon const &) { return true; });
			break;
		case Moves::Attract:
			if (multiplier(get_gender(user), get_gender(other.pokemon())) == -1_bi) {
				other.pokemon().attract(generation, user, weather);
			}
			break;
		case Moves::Baton_Pass:
			if (user_team.size() > 1_bi) {
				user.baton_pass();
			}
			break;
		case Moves::Belly_Drum:
			belly_drum(generation, user, weather);
			break;
		case Moves::Bide:
			user.use_bide(generation, other.pokemon(), weather);
			break;
		case Moves::Bind:
		case Moves::Clamp:
		case Moves::Fire_Spin:
		case Moves::Magma_Storm:
		case Moves::Sand_Tomb:
		case Moves::Whirlpool:
		case Moves::Wrap:
			// TODO: Handle Grip_Claw
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
				other.pokemon().apply_status(generation, Statuses::burn, user, weather);
			}
			break;
		case Moves::Blizzard:
		case Moves::Ice_Beam:
		case Moves::Ice_Punch:
		case Moves::Powder_Snow:
			if (move.variable.effect_activates()) {
				other.pokemon().apply_status(generation, Statuses::freeze, user, weather);
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
				other.pokemon().apply_status(generation, Statuses::paralysis, user, weather);
			}
			break;
		case Moves::Bounce:
			if (user.bounce(generation, weather) and move.variable.effect_activates()) {
				other.pokemon().apply_status(generation, Statuses::paralysis, user, weather);
			}
			break;
		case Moves::Brave_Bird:
		case Moves::Double_Edge:
		case Moves::Wood_Hammer:
			recoil(generation, user, weather, damage, 3_bi);
			break;
		case Moves::Bubble:
		case Moves::Bubble_Beam:
		case Moves::Constrict:
			if (move.variable.effect_activates()) {
				other.pokemon().stage()[StatNames::SPE] += -1_bi;
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
				other.pokemon().stage()[StatNames::SPD] += -2_bi;
			}
			break;
		case Moves::Charge:
			user.charge();
			user.stage()[StatNames::SPD] += 1_bi;
			break;
		case Moves::Charge_Beam:
			if (move.variable.effect_activates()) {
				user.stage()[StatNames::SPA] += 1_bi;
			}
			break;
		case Moves::Charm:
		case Moves::Feather_Dance:
			other.pokemon().stage()[StatNames::ATK] += -2_bi;
			break;
		case Moves::Chatter:
			if (can_confuse_with_chatter(get_species(user)) and move.variable.effect_activates()) {
				other.pokemon().confuse(generation, weather);
			}
			break;
		case Moves::Close_Combat:
			boost_physical(user.stage(), -1_bi);
			break;
		case Moves::Confuse_Ray:
		case Moves::Supersonic:
		case Moves::Sweet_Kiss:
		case Moves::Teeter_Dance:
			other.pokemon().confuse(generation, weather);
			break;
		case Moves::Confusion:
		case Moves::Dizzy_Punch:
		case Moves::Dynamic_Punch:
		case Moves::Psybeam:
		case Moves::Rock_Climb:
		case Moves::Signal_Beam:
		case Moves::Water_Pulse:
			if (move.variable.effect_activates()) {
				other.pokemon().confuse(generation, weather);
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
		case Moves::Razor_Shell:
		case Moves::Rock_Smash:
			if (move.variable.effect_activates()) {
				other.pokemon().stage()[StatNames::DEF] += -1_bi;
			}
			break;
		case Moves::Cotton_Spore:
		case Moves::Scary_Face:
			other.pokemon().stage()[StatNames::SPE] += -2_bi;
			break;
		case Moves::Covet:
		case Moves::Thief:
			user.steal_item(generation, other.pokemon());
			break;
		case Moves::Cross_Poison:
		case Moves::Gunk_Shot:
		case Moves::Poison_Jab:
		case Moves::Poison_Sting:
		case Moves::Poison_Tail:
		case Moves::Sludge:
		case Moves::Sludge_Bomb:
		case Moves::Smog:
			if (move.variable.effect_activates()) {
				other.pokemon().apply_status(generation, Statuses::poison, user, weather);
			}
			break;
		case Moves::Curse:
			curse(generation, user, other.pokemon(), weather);
			break;
		case Moves::Dark_Void:
		case Moves::Grass_Whistle:
		case Moves::Hypnosis:
		case Moves::Lovely_Kiss:
		case Moves::Sing:
		case Moves::Sleep_Powder:
		case Moves::Spore:
			other.pokemon().apply_status(generation, Statuses::sleep, user, weather, user.is_uproaring() or other.pokemon().is_uproaring());
			break;
		case Moves::Defense_Curl:
			user.stage()[StatNames::DEF] += 1_bi;
			user.defense_curl();
			break;
		case Moves::Defog:
			weather.deactivate_fog();
			other.pokemon().stage()[StatNames::EVA] += -1_bi;
			break;
		case Moves::Destiny_Bond:
			user.use_destiny_bond();
			break;
		case Moves::Detect:
		case Moves::Protect:
			user.protect();
			break;
		case Moves::Dig:
			user.dig(generation, weather);
			break;
		case Moves::Disable:
			other.pokemon().disable(generation, other.pokemon().last_used_move().name(), weather);
			break;
		case Moves::Dive:
			user.dive(generation, weather);
			break;
		case Moves::Doom_Desire:	// Fix
		case Moves::Future_Sight:
			break;
		case Moves::Double_Team:
		case Moves::Minimize:
			user.stage()[StatNames::EVA] += 1_bi;
			break;
		case Moves::Draco_Meteor:
		case Moves::Leaf_Storm:
		case Moves::Overheat:
		case Moves::Psycho_Boost:
			user.stage()[StatNames::SPA] += -2_bi;
			break;
		case Moves::Dragon_Dance:
			user.stage()[StatNames::ATK] += 1_bi;
			user.stage()[StatNames::SPE] += 1_bi;
			break;
		case Moves::Dream_Eater:
			if (is_sleeping(get_status(other.pokemon()))) {
				absorb_hp(generation, user, other.pokemon(), weather, damage);
			}
			break;
		case Moves::Embargo:
			other.pokemon().activate_embargo();
			break;
		case Moves::Encore:
			other.pokemon().activate_encore(generation, weather);
			break;
		case Moves::Endure:
			user.endure();
			break;
		case Moves::Explosion:
		case Moves::Self_Destruct:
			user.set_hp(generation, weather, 0_bi);
			break;
		case Moves::Fake_Tears:
		case Moves::Metal_Sound:
			other.pokemon().stage()[StatNames::SPD] += -2_bi;
			break;
		case Moves::Feint:
		case Moves::Hyperspace_Fury:
		case Moves::Hyperspace_Hole:
		case Moves::Phantom_Force:
			other.pokemon().break_protect();
			break;
		case Moves::Fire_Fang:
			move.variable.fang_side_effects(generation, user, other.pokemon(), weather, Statuses::burn);
			break;
		case Moves::Flare_Blitz:
			recoil_status(generation, user, other.pokemon(), weather, damage, move.variable, Statuses::burn);
			break;
		case Moves::Flash:
		case Moves::Kinesis:
		case Moves::Mud_Slap:
		case Moves::Sand_Attack:
		case Moves::Smokescreen:
			other.pokemon().stage()[StatNames::ACC] += -1_bi;
			break;
		case Moves::Flatter:
			confusing_stat_boost(generation, other.pokemon(), weather, StatNames::SPA, 1_bi);
			break;
		case Moves::Fling:
			fling_side_effects(generation, user, other.pokemon(), weather);
			break;
		case Moves::Fly:
			user.fly(generation, weather);
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
		case Moves::Zap_Cannon:
			other.pokemon().apply_status(generation, Statuses::paralysis, user, weather);
			break;
		case Moves::Gravity:
			weather.activate_gravity();
			break;
		case Moves::Growl:
			other.pokemon().stage()[StatNames::ATK] += -1_bi;
			break;
		case Moves::Growth:
			user.stage()[StatNames::SPA] += 1_bi;
			break;
		case Moves::Grudge:		// Fix
			break;
		case Moves::Guard_Swap:
			swap_defensive(user.stage(), other.pokemon().stage());
			break;
		case Moves::Hail:
			weather.activate_hail(extends_hail(user.item(generation, weather)));
			break;
		case Moves::Hammer_Arm:
			user.stage()[StatNames::SPE] += -1_bi;
			break;
		case Moves::Harden:
		case Moves::Withdraw:
			user.stage()[StatNames::DEF] += 1_bi;
			break;
		case Moves::Haze:
			user.stage() = Stage{};
			other.pokemon().stage() = Stage{};
			break;
		case Moves::Head_Smash:
			recoil(generation, user, weather, damage, 2_bi);
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
			heal(generation, user, weather, rational(1_bi, 2_bi));
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
				user.stage()[StatNames::ATK] += 1_bi;
			}
			break;
		case Moves::Ice_Ball:		// Fix
		case Moves::Rollout:
			break;
		case Moves::Ice_Fang:
			move.variable.fang_side_effects(generation, user, other.pokemon(), weather, Statuses::freeze);
			break;
		case Moves::Icy_Wind:
		case Moves::Low_Sweep:
		case Moves::Mud_Shot:
		case Moves::Rock_Tomb:
		case Moves::String_Shot:
			other.pokemon().stage()[StatNames::SPE] += -1_bi;
			break;
		case Moves::Imprison:
			user.use_imprison();
			break;
		case Moves::Incinerate: {
			auto other_pokemon = other.pokemon();
			if (can_be_incinerated(other_pokemon.item(generation, weather), get_ability(other_pokemon))) {
				other_pokemon.destroy_item();
			}
			break;
		}
		case Moves::Inferno:
			other.pokemon().apply_status(generation, Statuses::burn, user, weather);
			break;
		case Moves::Ingrain:
			user.ingrain();
			break;
		case Moves::Knock_Off:
			other.pokemon().remove_item();
			break;
		case Moves::Leaf_Tornado:
		case Moves::Mirror_Shot:
		case Moves::Mud_Bomb:
		case Moves::Muddy_Water:
		case Moves::Night_Daze:
		case Moves::Octazooka:
			if (move.variable.effect_activates()) {
				other.pokemon().stage()[StatNames::ACC] -= 1_bi;
			}
			break;
		case Moves::Leech_Seed:
			other.pokemon().hit_with_leech_seed();
			break;
		case Moves::Leer:
		case Moves::Tail_Whip:
			other.pokemon().stage()[StatNames::DEF] += -1_bi;
			break;
		case Moves::Light_Screen:
			user_team.screens.activate_light_screen(extends_light_screen(user.item(generation, weather)));
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
		case Moves::Magic_Room:
			weather.activate_magic_room();
			break;
		case Moves::Magnet_Rise:
			user.activate_magnet_rise();
			break;
		case Moves::Me_First:		// Fix
			break;
		case Moves::Memento:
			boost_offensive(other.pokemon().stage(), -2_bi);
			user.set_hp(generation, weather, 0_bi);
			break;
		case Moves::Mimic:		// Fix
			break;
		case Moves::Miracle_Eye:		// Fix
			break;
		case Moves::Mist:
			user_team.screens.activate_mist();
			break;
		case Moves::Mist_Ball:
			if (move.variable.effect_activates()) {
				other.pokemon().stage()[StatNames::SPA] += -1_bi;
			}
			break;
		case Moves::Moonlight:
		case Moves::Morning_Sun:
		case Moves::Synthesis: {
			auto const amount = [&]() {
				using Numerator = bounded::integer<1, 2>;
				using Denominator = bounded::integer<2, 4>;
				using Result = rational<Numerator, Denominator>;
				
				auto const blocks_weather = weather_is_blocked_by_ability(get_ability(user), get_ability(other.pokemon()));
				if (weather.sun(blocks_weather)) {
					return Result(2_bi, 3_bi);
				} else if (weather.hail(blocks_weather) or weather.rain(blocks_weather) or weather.sand(blocks_weather)) {
					return Result(1_bi, 4_bi);
				} else {
					return Result(1_bi, 2_bi);
				}
			};
			heal(generation, user, weather, amount());
			break;
		}
		case Moves::Mud_Sport:
			user.activate_mud_sport();
			break;
		case Moves::Nasty_Plot:
		case Moves::Tail_Glow:
			user.stage()[StatNames::SPA] += 2_bi;
			break;
		case Moves::Nightmare:
			other.pokemon().try_to_give_nightmares();
			break;
		case Moves::Outrage:
		case Moves::Petal_Dance:
		case Moves::Thrash:
			user.activate_rampage();
			break;
		case Moves::Pain_Split:
			equalize_hp(generation, user, other.pokemon(), weather);
			break;
		case Moves::Perish_Song:
			user.activate_perish_song();
			other.pokemon().activate_perish_song();
			break;
		case Moves::Poison_Fang:
			if (move.variable.effect_activates()) {
				other.pokemon().apply_status(generation, Statuses::toxic, user, weather);
			}
			break;
		case Moves::Poison_Gas:
		case Moves::Poison_Powder: {
			other.pokemon().apply_status(generation, Statuses::poison, user, weather);
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
				change_hp(generation, other.pokemon(), weather, 80_bi);
			}
			break;
		case Moves::Psych_Up:
			user.stage() = other.pokemon().stage();
			break;
		case Moves::Psycho_Shift:
			if (is_clear(get_status(other.pokemon()))) {
				shift_status(generation, user, other.pokemon(), weather);
			}
			break;
		case Moves::Quiver_Dance:
			for (auto const stat : {StatNames::SPA, StatNames::SPD, StatNames::SPE}) {
				user.stage()[stat] += 1_bi;
			}
			break;
		case Moves::Rage:		// Fix
			break;
		case Moves::Rain_Dance:
			weather.activate_rain(extends_rain(user.item(generation, weather)));
			break;
		case Moves::Rapid_Spin:
			if (!Effectiveness(generation, move_type, other.pokemon().types()).has_no_effect()) {
				user_team.clear_field();
			}
			break;
		case Moves::Razor_Wind:	// Fix
			break;
		case Moves::Recycle:
			user.recycle_item();
			break;
		case Moves::Reflect:
			user_team.screens.activate_reflect(extends_reflect(user.item(generation, weather)));
			break;
		case Moves::Refresh:
			user.clear_status();
			break;
		case Moves::Rest:
			user.rest(generation, weather, other.pokemon().is_uproaring());
			break;
		case Moves::Roar:
		case Moves::Whirlwind:
			phaze(generation, user, other, weather, move.variable);
			break;
		case Moves::Role_Play:		// Fix
			break;
		case Moves::Roost:
			user.roost();
			heal(generation, user, weather, rational(1_bi, 2_bi));
			break;
		case Moves::Safeguard:
			user_team.screens.activate_safeguard();
			break;
		case Moves::Sandstorm:
			weather.activate_sand(extends_sand(user.item(generation, weather)));
			break;
		case Moves::Screech:
			other.pokemon().stage()[StatNames::DEF] += -2_bi;
			break;
		case Moves::Seed_Flare:
			if (move.variable.effect_activates()) {
				other.pokemon().stage()[StatNames::SPD] += -2_bi;
			}
			break;
		case Moves::Shadow_Force:
			user.shadow_force(generation, weather);
			// TODO: other.pokemon().break_protect();
			break;
		case Moves::Sharpen:
			user.stage()[StatNames::ATK] += 1_bi;
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
			if (will_be_recharge_turn(user, move.name, get_ability(other.pokemon()), weather)) {
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
				user.stage()[StatNames::DEF] += 1_bi;
			}
			break;
		case Moves::Stockpile:
			user.increment_stockpile();
			break;
		case Moves::Struggle:
			struggle(generation, user, weather);
			break;
		case Moves::Submission:
		case Moves::Take_Down:
			recoil(generation, user, weather, damage, 4_bi);
			break;
		case Moves::Substitute:
			user.use_substitute(generation, weather);		
			break;
		case Moves::Sunny_Day:
			weather.activate_sun(extends_sun(user.item(generation, weather)));
			break;
		case Moves::Superpower:
			boost_physical(user.stage(), -1_bi);
			break;
		case Moves::Swagger:
			confusing_stat_boost(generation, other.pokemon(), weather, StatNames::ATK, 2_bi);
			break;
		case Moves::Swallow:
			use_swallow(generation, user, weather);
			break;
		case Moves::Sweet_Scent:
			other.pokemon().stage()[StatNames::EVA] += -1_bi;
			break;
		case Moves::Switch0:
		case Moves::Switch1:
		case Moves::Switch2:
		case Moves::Switch3:
		case Moves::Switch4:
		case Moves::Switch5:
			user_team.switch_pokemon(generation, other.pokemon(), weather, to_replacement(move.name));
			break;
		case Moves::Switcheroo:
		case Moves::Trick:
			swap_items(generation, user, other.pokemon());
			break;
		case Moves::Swords_Dance:
			user.stage()[StatNames::ATK] += 2_bi;
			break;
		case Moves::Tailwind:
			user_team.screens.activate_tailwind();
			break;
		case Moves::Taunt:
			other.pokemon().taunt(generation, weather);
			break;
		case Moves::Thunder_Fang:
			move.variable.fang_side_effects(generation, user, other.pokemon(), weather, Statuses::paralysis);
			break;
		case Moves::Tickle:
			boost_physical(other.pokemon().stage(), -1_bi);
			break;
		case Moves::Torment:
			other.pokemon().torment(generation, weather);
			break;
		case Moves::Toxic: {
			other.pokemon().apply_status(generation, Statuses::toxic, user, weather);
			break;
		}
		case Moves::Toxic_Spikes:
			other.entry_hazards.add_toxic_spikes();
			break;
		case Moves::Transform:		// Fix
			break;
		case Moves::Tri_Attack:
			if (auto const status = move.variable.tri_attack_status(); status != Statuses::clear) {
				BOUNDED_ASSERT(uproar_does_not_apply(status));
				other.pokemon().apply_status(generation, status, user, weather);
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
			recoil_status(generation, user, other.pokemon(), weather, damage, move.variable, Statuses::paralysis);
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
			other.pokemon().apply_status(generation, Statuses::burn, user, weather);
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

auto do_effects_before_moving(Moves const move, MutableActivePokemon user, Team & other) {
	if (breaks_screens(move)) {
		other.screens.shatter();
	} else if (is_usable_while_frozen(move)) {
		if (is_frozen(get_status(user))) {
			user.clear_status();
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

// TODO: This is probably duplicating some logic that I have elsewhere. For
// instance, this also needs to check against things like Water Absorb
// activating. I probably want to guard code that calls this with checking that
// the move affected the Pokemon, rather than having this function try to guard
// itself.

// Returns whether the attack is weakened by the item
auto activate_when_hit_item(Generation const generation, Moves const move, Type const move_type, MutableActivePokemon defender, Weather const weather, Effectiveness const effectiveness) -> bool {
	if (!is_damaging(move) or effectiveness.has_no_effect()) {
		return false;
	}
	auto substitute = [&] {
		return defender.substitute() and blocked_by_substitute(generation, move);
	};
	auto resistance_berry = [&](Type const resisted) {
		if (move_type != resisted or substitute()) {
			return false;
		}
		defender.remove_item();
		return true;
	};
	auto se_resistance_berry = [&](Type const resisted) {
		if (!effectiveness.is_super_effective()) {
			return false;
		}
		return resistance_berry(resisted);
	};
	constexpr auto max_stage = bounded::max_value<Stage::value_type>;
	auto stat_boost = [&](StatNames const stat) {
		auto & stage = defender.stage()[stat];
		if (stage != max_stage and !substitute()) {
			stage += 1_bi;
			defender.remove_item();
		}
	};
	auto stat_boost_move_type = [&](Type const type, StatNames const stat) {
		if (move_type == type) {
			stat_boost(stat);
		}
		return false;
	};
	switch (defender.item(generation, weather)) {
		case Item::Absorb_Bulb:
			return stat_boost_move_type(Type::Water, StatNames::SPA);
		case Item::Air_Balloon:
			defender.remove_item();
			return false;
		case Item::Babiri_Berry:
			return se_resistance_berry(Type::Steel);
		case Item::Cell_Battery:
			return stat_boost_move_type(Type::Electric, StatNames::ATK);
		case Item::Charti_Berry:
			return se_resistance_berry(Type::Rock);
		case Item::Chople_Berry:
			return se_resistance_berry(Type::Fighting);
		case Item::Chilan_Berry:
			return resistance_berry(Type::Normal);
		case Item::Coba_Berry:
			return se_resistance_berry(Type::Flying);
		case Item::Colbur_Berry:
			return se_resistance_berry(Type::Dark);
		case Item::Enigma_Berry:
			if (effectiveness.is_super_effective() and generation >= Generation::four and !substitute()) {
				defender.remove_item();
				heal(generation, defender, weather, rational(1_bi, 4_bi));
			}
			return false;
		case Item::Haban_Berry:
			return se_resistance_berry(Type::Dragon);
		case Item::Kasib_Berry:
			return se_resistance_berry(Type::Ghost);
		case Item::Kebia_Berry:
			return se_resistance_berry(Type::Poison);
		case Item::Kee_Berry:
			if (is_physical(move)) {
				stat_boost(StatNames::DEF);
			}
			return false;
		case Item::Luminous_Moss:
			return stat_boost_move_type(Type::Water, StatNames::SPD);
		case Item::Maranga_Berry:
			if (is_special(move)) {
				stat_boost(StatNames::SPD);
			}
			return false;
		case Item::Occa_Berry:
			return se_resistance_berry(Type::Fire);
		case Item::Passho_Berry:
			return se_resistance_berry(Type::Water);
		case Item::Payapa_Berry:
			return se_resistance_berry(Type::Psychic);
		case Item::Rindo_Berry:
			return se_resistance_berry(Type::Grass);
		case Item::Roseli_Berry:
			return se_resistance_berry(Type::Fairy);
		case Item::Shuca_Berry:
			return se_resistance_berry(Type::Ground);
		case Item::Snowball:
			return stat_boost_move_type(Type::Ice, StatNames::ATK);
		case Item::Tanga_Berry:
			return se_resistance_berry(Type::Bug);
		case Item::Wacan_Berry:
			return se_resistance_berry(Type::Electric);
		case Item::Weakness_Policy:
			if (effectiveness.is_super_effective() and !substitute()) {
				auto & stage = defender.stage();
				if (stage[StatNames::ATK] != max_stage and stage[StatNames::SPA] != max_stage) {
					boost_offensive(stage, 2_bi);
					defender.remove_item();
				}
			}
			return false;
		case Item::Yache_Berry:
			return se_resistance_berry(Type::Ice);
		default:
			return false;
	}
}

auto use_move(Generation const generation, Team & user, ExecutedMove const move, Type const move_type, Team & other, OtherMove const other_move, Weather & weather, ActualDamage const actual_damage) -> void {
	auto const user_pokemon = user.pokemon();
	auto const other_pokemon = other.pokemon();
	do_effects_before_moving(move.name, user_pokemon, other);

	auto const effectiveness = Effectiveness(generation, move_type, other_pokemon.types());
	auto const weakened = activate_when_hit_item(generation, move.name, move_type, other_pokemon, weather, effectiveness);
	auto const damage = actual_damage.value(generation, user, move, weakened, other, other_move, weather);

	auto const had_substitute = static_cast<bool>(other_pokemon.substitute());
	// Should this check if we did any damage or if the move is damaging?
	auto const damage_done = damage != 0_bi ?
		other_pokemon.direct_damage(generation, move.name, weather, damage) :
		0_bi;
	if (!had_substitute or !blocked_by_substitute(generation, move.name)) {
		do_side_effects(generation, user, move, move_type, other, weather, damage_done);
		// Should this check if we did any damage or if the move is damaging?
		if (damage_done != 0_bi) {
			auto const item = user_pokemon.item(generation, weather);
			// TODO: Doom Desire / Future Sight are not handled correctly
			if (item == Item::Life_Orb) {
				heal(generation, user_pokemon, weather, rational(-1_bi, 10_bi));
			} else if (item == Item::Shell_Bell) {
				change_hp(generation, user_pokemon, weather, bounded::max(damage_done / 8_bi, 1_bi));
			}
		}
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

auto activate_pp_restore_berry(Generation const generation, Move & move, MutableActivePokemon pokemon, Weather const weather) {
	if (move.pp().is_empty()) {
		switch (pokemon.item(generation, weather)) {
			case Item::Leppa_Berry:
				move.restore_pp(10_bi);
				pokemon.remove_item();
				break;
			case Item::MysteryBerry:
				move.restore_pp(5_bi);
				pokemon.remove_item();
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
			user_pokemon.stage()[StatNames::SPE] += 1_bi;
		}
	}
	if (!can_execute_move(user_pokemon, found_move, weather, is_recharging)) {
		return;
	}
	
	auto const other_ability = get_ability(other_pokemon);

	if (is_regular(move.selected) and move.executed != Moves::Hit_Self and !user_pokemon.is_locked_in_by_move()) {
		auto & move_ref = find_regular_move(all_moves(user_pokemon).regular(), move.selected);
		move_ref.decrement_pp(other_ability);
		activate_pp_restore_berry(generation, move_ref, user_pokemon, weather);
	}
	
	auto const target = move_target(generation, move.executed);
	// TODO: What happens if we Sleep Talk Trump Card?
	// TODO: Make sure this does not happen because we missed due to a vanishing
	// state
	if (move.miss) {
		user_pokemon.unsuccessfully_use_move(move.executed);
		if (user_pokemon.item(generation, weather) == Item::Blunder_Policy) {
			user_pokemon.stage()[StatNames::SPE] += 2_bi;
			user_pokemon.remove_item();
		}
		return;
	}
	auto const unsuccessful =
		move_fails(move.executed, user_pokemon.damaged(), other_ability, other_move) or
		(get_hp(other_pokemon).current() == 0_bi and fails_against_fainted(target)) or
		(other_pokemon.is_protecting() and blocked_by_protect(target, move.executed));
	if (unsuccessful) {
		user_pokemon.unsuccessfully_use_move(move.executed);
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
	user_pokemon.increment_move_use_counter(move.executed);
}

}	// namespace technicalmachine
