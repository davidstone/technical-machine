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
#include <tm/move/category.hpp>
#include <tm/move/move.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/target.hpp>
#include <tm/move/will_be_recharge_turn.hpp>

#include <tm/ability.hpp>
#include <tm/block.hpp>
#include <tm/end_of_turn.hpp>
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

auto item_can_be_lost(Generation const generation, ActivePokemon const pokemon) {
	return
		pokemon.ability() != Ability::Sticky_Hold or
		(generation >= Generation::five and get_hp(pokemon).current() == 0_bi);
}

// I could potentially treat this as negative recoil
auto absorb_hp(Generation const generation, MutableActivePokemon user, MutableActivePokemon target, Weather const weather, HP::current_type const damage) -> void {
	auto const absorbed = damage / 2_bi * healing_multiplier(user.item(generation, weather));
	if (damages_leechers(target.ability())) {
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
	if (is_type(user, Type::Ghost)) {
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


auto item_can_be_incinerated(Generation const generation, ActivePokemon const target, Weather const weather) -> bool {
	// TODO: Destroy gems
	return item_can_be_lost(generation, target) and berry_power(target.item(generation, weather)) != 0_bi;
}



void recoil(Generation const generation, MutableActivePokemon user, Weather const weather, HP::current_type const damage, bounded::checked_integer<1, 4> const denominator) {
	if (!blocks_recoil(user.ability())) {
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
	return !team.pokemon().ingrained() and !blocks_phazing(team.pokemon().ability()) and size(team.all_pokemon()) > 1_bi;
}

auto phaze(Generation const generation, MutableActivePokemon user, Team & target, Weather & weather, Variable const variable) {
	// TODO: Phazing activates Synchronize if Toxic Spikes cause status before
	// generation 5
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


auto do_side_effects(Generation const generation, Team & user_team, ExecutedMove const executed, Team & other, Weather & weather, HP::current_type const damage) {
	auto user = user_team.pokemon();
	switch (executed.move.name) {
		case Moves::Absorb:
		case Moves::Drain_Punch:
		case Moves::Giga_Drain:
		case Moves::Leech_Life:
		case Moves::Mega_Drain:
			absorb_hp(generation, user, other.pokemon(), weather, damage);
			break;
		case Moves::Acid:
			if (executed.variable.effect_activates()) {
				auto const stat = generation <= Generation::three ? StatNames::DEF : StatNames::SPD;
				other.pokemon().stage()[stat] += -1_bi;
			}
			break;
		case Moves::Acid_Armor:
		case Moves::Barrier:
		case Moves::Iron_Defense:
			user.stage()[StatNames::DEF] += 2_bi;
			break;
		case Moves::Acupressure:
			user.stage()[executed.variable.acupressure_stat_boost()] += 2_bi;
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
		case Moves::Steamroller:
		case Moves::Stomp:
		case Moves::Twister:
		case Moves::Waterfall:
		case Moves::Zen_Headbutt:
			if (executed.variable.effect_activates()) {
				other.pokemon().flinch();
			}
			break;
		case Moves::Amnesia:
			user.stage()[StatNames::SPD] += 2_bi;
			if (generation == Generation::one) {
				user.stage()[StatNames::SPA] += 2_bi;
			}
			break;
		case Moves::Ancient_Power:
		case Moves::Ominous_Wind:
		case Moves::Silver_Wind:
			if (executed.variable.effect_activates()) {
				boost_regular(user.stage(), 1_bi);
			}
			break;
		case Moves::Aqua_Ring:
			user.activate_aqua_ring();
			break;
		case Moves::Aurora_Beam:
			if (executed.variable.effect_activates()) {
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
		case Moves::Scald:
		case Moves::Searing_Shot:
			if (executed.variable.effect_activates()) {
				other.pokemon().apply_status(generation, Statuses::burn, user, weather);
			}
			break;
		case Moves::Blizzard:
		case Moves::Ice_Beam:
		case Moves::Ice_Punch:
		case Moves::Powder_Snow:
			if (executed.variable.effect_activates()) {
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
			if (executed.variable.effect_activates()) {
				other.pokemon().apply_status(generation, Statuses::paralysis, user, weather);
			}
			break;
		case Moves::Bounce:
			if (user.bounce(generation, weather) and executed.variable.effect_activates()) {
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
			if (executed.variable.effect_activates()) {
				other.pokemon().stage()[StatNames::SPE] += -1_bi;
			}
			break;
		case Moves::Bug_Bite:
		case Moves::Pluck: {
			auto const other_pokemon = other.pokemon();
			if (item_can_be_lost(generation, other_pokemon)) {
			}
			break;
		}
		case Moves::Bug_Buzz:
		case Moves::Earth_Power:
		case Moves::Energy_Ball:
		case Moves::Flash_Cannon:
		case Moves::Focus_Blast:
		case Moves::Luster_Purge:
		case Moves::Psychic:
		case Moves::Shadow_Ball:
			if (executed.variable.effect_activates()) {
				other.pokemon().stage()[StatNames::SPD] += -1_bi;
			}
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
			if (executed.variable.effect_activates()) {
				user.stage()[StatNames::SPA] += 1_bi;
			}
			break;
		case Moves::Charm:
		case Moves::Feather_Dance:
			other.pokemon().stage()[StatNames::ATK] += -2_bi;
			break;
		case Moves::Chatter:
			if (can_confuse_with_chatter(get_species(user)) and executed.variable.effect_activates()) {
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
			if (executed.variable.effect_activates()) {
				other.pokemon().confuse(generation, weather);
			}
			break;
		case Moves::Conversion:
			break;
		case Moves::Conversion_2:
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
			if (executed.variable.effect_activates()) {
				other.pokemon().stage()[StatNames::DEF] += -1_bi;
			}
			break;
		case Moves::Cotton_Spore:
		case Moves::Scary_Face:
			other.pokemon().stage()[StatNames::SPE] += -2_bi;
			break;
		case Moves::Covet:
		case Moves::Thief:
			if (item_can_be_lost(generation, other.pokemon())) {
				user.steal_item(generation, other.pokemon());
			}
			break;
		case Moves::Cross_Poison:
		case Moves::Gunk_Shot:
		case Moves::Poison_Jab:
		case Moves::Poison_Sting:
		case Moves::Poison_Tail:
		case Moves::Sludge:
		case Moves::Sludge_Bomb:
		case Moves::Sludge_Wave:
		case Moves::Smog:
			if (executed.variable.effect_activates()) {
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
		case Moves::Doom_Desire:
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
			executed.variable.fang_side_effects(generation, user, other.pokemon(), weather, Statuses::burn);
			break;
		case Moves::Flare_Blitz:
			recoil_status(generation, user, other.pokemon(), weather, damage, executed.variable, Statuses::burn);
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
		case Moves::Follow_Me:
			break;
		case Moves::Foresight:
		case Moves::Odor_Sleuth:
			other.pokemon().identify();
			break;
		case Moves::Gastro_Acid:
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
		case Moves::Grudge:
			break;
		case Moves::Guard_Swap:
			swap_defensive(user.stage(), other.pokemon().stage());
			break;
		case Moves::Hail:
			weather.activate_hail_from_move(extends_hail(user.item(generation, weather)));
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
			cure_all_status(user_team, [=](Pokemon const & pokemon) {
				auto ability = [&]{
					auto const is_active = std::addressof(pokemon) == std::addressof(static_cast<Pokemon const &>(user));
					return is_active ? user.ability() : pokemon.initial_ability();
				};
				return generation == Generation::five or !blocks_sound_moves(ability());
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
		case Moves::Healing_Wish:
			break;
		case Moves::Heart_Swap:
			std::swap(user.stage(), other.pokemon().stage());
			break;
		case Moves::High_Jump_Kick:
		case Moves::Jump_Kick:
			break;
		case Moves::Howl:
		case Moves::Metal_Claw:
		case Moves::Meditate:
		case Moves::Meteor_Mash:
			if (executed.variable.effect_activates()) {
				user.stage()[StatNames::ATK] += 1_bi;
			}
			break;
		case Moves::Ice_Ball:
		case Moves::Rollout:
			break;
		case Moves::Ice_Fang:
			executed.variable.fang_side_effects(generation, user, other.pokemon(), weather, Statuses::freeze);
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
			if (item_can_be_incinerated(generation, other_pokemon, weather)) {
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
		case Moves::Knock_Off: {
			auto const other_pokemon = other.pokemon();
			if (item_can_be_lost(generation, other_pokemon)) {
				other_pokemon.remove_item();
			}
			break;
		}
		case Moves::Leaf_Tornado:
		case Moves::Mirror_Shot:
		case Moves::Mud_Bomb:
		case Moves::Muddy_Water:
		case Moves::Night_Daze:
		case Moves::Octazooka:
			if (executed.variable.effect_activates()) {
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
			user_team.activate_light_screen(generation, weather);
			break;
		case Moves::Lock_On:
		case Moves::Mind_Reader:
			user.use_lock_on();
			break;
		case Moves::Lucky_Chant:
			user_team.activate_lucky_chant();
			break;
		case Moves::Lunar_Dance:
			break;
		case Moves::Magic_Coat:
			break;
		case Moves::Magic_Room:
			weather.activate_magic_room();
			break;
		case Moves::Magnet_Rise:
			user.activate_magnet_rise();
			break;
		case Moves::Me_First:
			break;
		case Moves::Memento:
			boost_offensive(other.pokemon().stage(), -2_bi);
			user.set_hp(generation, weather, 0_bi);
			break;
		case Moves::Mimic:
			break;
		case Moves::Miracle_Eye:
			break;
		case Moves::Mist:
			user_team.activate_mist();
			break;
		case Moves::Mist_Ball:
			if (executed.variable.effect_activates()) {
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
				
				auto const blocks_weather = weather_is_blocked_by_ability(user.ability(), other.pokemon().ability());
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
			user.try_activate_perish_song();
			other.pokemon().try_activate_perish_song();
			break;
		case Moves::Poison_Fang:
			if (executed.variable.effect_activates()) {
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
			if (executed.variable.present_heals()) {
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
		case Moves::Rage:
			break;
		case Moves::Rain_Dance:
			weather.activate_rain_from_move(extends_rain(user.item(generation, weather)));
			break;
		case Moves::Rapid_Spin:
			user_team.clear_field();
			break;
		case Moves::Razor_Wind:
			break;
		case Moves::Recycle:
			user.recycle_item();
			break;
		case Moves::Reflect:
			user_team.activate_reflect(generation, weather);
			break;
		case Moves::Refresh:
			user.clear_status();
			break;
		case Moves::Rest:
			user.rest(generation, weather, other.pokemon().is_uproaring());
			break;
		case Moves::Roar:
		case Moves::Whirlwind:
			phaze(generation, user, other, weather, executed.variable);
			break;
		case Moves::Role_Play:
			break;
		case Moves::Roost:
			user.roost();
			heal(generation, user, weather, rational(1_bi, 2_bi));
			break;
		case Moves::Safeguard:
			user_team.activate_safeguard();
			break;
		case Moves::Sandstorm:
			weather.activate_sand_from_move(extends_sand(user.item(generation, weather)));
			break;
		case Moves::Screech:
			other.pokemon().stage()[StatNames::DEF] += -2_bi;
			break;
		case Moves::Seed_Flare:
			if (executed.variable.effect_activates()) {
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
		case Moves::Shell_Smash:
			for (auto const stat : {StatNames::DEF, StatNames::SPD}) {
				user.stage()[stat] -= 1_bi;
			}
			for (auto const stat : {StatNames::ATK, StatNames::SPA, StatNames::SPE}) {
				user.stage()[stat] += 2_bi;
			}
			break;
		case Moves::Shift_Gear:
			user.stage()[StatNames::ATK] += 1_bi;
			user.stage()[StatNames::SPE] += 2_bi;
			break;
		case Moves::Sketch:
			break;
		case Moves::Skill_Swap:
			break;
		case Moves::Skull_Bash:
			break;
		case Moves::Sky_Attack:
			if (executed.variable.effect_activates()) {
				other.pokemon().flinch();
			}
			break;
		case Moves::Smelling_Salts:
			if (boosts_smellingsalt(get_status(other.pokemon()))) {
				get_status(other.pokemon()) = Status{};
			}
			break;
		case Moves::Snarl:
		case Moves::Struggle_Bug:
			other.pokemon().stage()[StatNames::SPA] -= 1_bi;
			break;
		case Moves::Snatch:
			break;
		case Moves::Solar_Beam:
			if (will_be_recharge_turn(user, executed.move.name, other.pokemon().ability(), weather)) {
				user.use_charge_up_move();
			}
			break;
		case Moves::Spikes:
			other.add_spikes();
			break;
		case Moves::Spit_Up:
			user.release_stockpile();
			break;
		case Moves::Spite:
			break;
		case Moves::Stealth_Rock:
			other.add_stealth_rock();
			break;
		case Moves::Steel_Wing:
			if (executed.variable.effect_activates()) {
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
			weather.activate_sun_from_move(extends_sun(user.item(generation, weather)));
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
			user_team.switch_pokemon(generation, other.pokemon(), weather, to_replacement(executed.move.name));
			break;
		case Moves::Switcheroo:
		case Moves::Trick: {
			auto const other_pokemon = other.pokemon();
			if (item_can_be_lost(generation, other_pokemon)) {
				swap_items(generation, user, other.pokemon());
			}
			break;
		}
		case Moves::Swords_Dance:
			user.stage()[StatNames::ATK] += 2_bi;
			break;
		case Moves::Tailwind:
			user_team.activate_tailwind();
			break;
		case Moves::Taunt:
			other.pokemon().taunt(generation, weather);
			break;
		case Moves::Thunder_Fang:
			executed.variable.fang_side_effects(generation, user, other.pokemon(), weather, Statuses::paralysis);
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
			other.add_toxic_spikes();
			break;
		case Moves::Transform:
			break;
		case Moves::Tri_Attack:
			if (auto const status = executed.variable.tri_attack_status(); status != Statuses::clear) {
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
		case Moves::V_create:
			for (auto const stat : {StatNames::DEF, StatNames::SPD, StatNames::SPE}) {
				user.stage()[stat] -= 1_bi;
			}
			break;
		case Moves::Volt_Tackle:
			recoil_status(generation, user, other.pokemon(), weather, damage, executed.variable, Statuses::paralysis);
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
			user_team.activate_wish();
			break;
		case Moves::Worry_Seed:
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
		other.shatter_screens();
	} else if (is_usable_while_frozen(move)) {
		if (is_frozen(get_status(user))) {
			user.clear_status();
		}
	}
}


constexpr auto move_fails(Moves const move, bool const user_damaged, Ability const other_ability, OtherMove const other_move) {
	switch (move) {
		case Moves::Boomburst:
		case Moves::Bug_Buzz:
		case Moves::Chatter:
		case Moves::Clanging_Scales:
		case Moves::Clangorous_Soulblaze:
		case Moves::Confide:
		case Moves::Disarming_Voice:
		case Moves::Echoed_Voice:
		case Moves::Grass_Whistle:
		case Moves::Growl:
		case Moves::Hyper_Voice:
		case Moves::Metal_Sound:
		case Moves::Noble_Roar:
		case Moves::Parting_Shot:
		case Moves::Relic_Song:
		case Moves::Roar:
		case Moves::Round:
		case Moves::Screech:
		case Moves::Sing:
		case Moves::Snarl:
		case Moves::Snore:
		case Moves::Sparkling_Aria:
		case Moves::Supersonic:
		case Moves::Uproar:
			return blocks_sound_moves(other_ability);
		case Moves::Explosion:
		case Moves::Mind_Blown:
		case Moves::Self_Destruct:
			return other_ability == Ability::Damp;
		case Moves::Focus_Punch:
			return user_damaged;
		case Moves::Sucker_Punch:
			return !other_move.future_move_is_damaging();
		default:
			return false;
	}
}

// Returns whether the attack is weakened by the item
auto activate_when_hit_item(Generation const generation, KnownMove const move, MutableActivePokemon defender, Weather const weather, Effectiveness const effectiveness) -> bool {
	auto substitute = [&] {
		return defender.substitute() and substitute_interaction(generation, move.name) != Substitute::bypassed;
	};
	auto resistance_berry = [&](Type const resisted) {
		if (move.type != resisted or substitute()) {
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
		if (move.type == type) {
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
			if (is_physical(generation, move)) {
				stat_boost(StatNames::DEF);
			}
			return false;
		case Item::Luminous_Moss:
			return stat_boost_move_type(Type::Water, StatNames::SPD);
		case Item::Maranga_Berry:
			if (is_special(generation, move)) {
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

constexpr auto targets_foe_specifically(Target const target) {
	switch (target) {
		case Target::adjacent_foe:
		case Target::adjacent:
		case Target::any:
			return true;
		case Target::user:
		case Target::adjacent_ally:
		case Target::user_or_adjacent_ally:
		case Target::all_allies:
		case Target::user_team:
		case Target::user_field:
		case Target::foe_field:
		case Target::field:
		case Target::all_adjacent_foes:
		case Target::all_adjacent:
		case Target::all:
			return false;
	}
}

auto use_move(Generation const generation, Team & user, ExecutedMove const executed, Target const target, Team & other, OtherMove const other_move, Weather & weather, ActualDamage const actual_damage) -> void {
	auto const user_pokemon = user.pokemon();
	auto const other_pokemon = other.pokemon();
	do_effects_before_moving(executed.move.name, user_pokemon, other);

	auto const effectiveness = Effectiveness(generation, executed.move.type, other_pokemon.types());
	if (targets_foe_specifically(target) and effectiveness.has_no_effect()) {
		return;
	}

	auto const damaging = is_damaging(executed.move.name);
	auto const weakened = damaging and activate_when_hit_item(generation, executed.move, other_pokemon, weather, effectiveness);
	auto const damage = actual_damage.value(generation, user, executed, weakened, other, other_move, weather);
	BOUNDED_ASSERT(damaging or damage == 0_bi);

	auto const had_substitute = static_cast<bool>(other_pokemon.substitute());
	auto const effects = had_substitute ?
		substitute_interaction(generation, executed.move.name) :
		Substitute::bypassed;
	if (effects == Substitute::causes_failure) {
		return;
	}

	// Should this check if we did any damage or if the move is damaging?
	auto const damage_done = damage != 0_bi ?
		other_pokemon.direct_damage(generation, executed.move.name, weather, damage) :
		0_bi;

	// TODO: When are side-effects on the user blocked?
	if (effects == Substitute::absorbs) {
		return;
	}
	do_side_effects(generation, user, executed, other, weather, damage_done);
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

auto absorb_ability_activates(Generation const generation, KnownMove const move, Type const absorbed_type) -> bool {
	if (!move_targets_foe(generation, move.name)) {
		return false;
	}
	if (move.type != absorbed_type) {
		return false;
	}
	return true;
}

auto flash_fire_activates(Generation const generation, KnownMove const move, ActivePokemon const target) -> bool {
	if (generation <= Generation::four and get_status(target).name() == Statuses::freeze) {
		return false;
	}
	return
		absorb_ability_activates(generation, move, Type::Fire) and
		(generation >= Generation::four or move.name != Moves::Will_O_Wisp);
}

auto volt_absorb_activates(Generation const generation, KnownMove const move) -> bool {
	return
		absorb_ability_activates(generation, move, Type::Electric) and
		(generation >= Generation::four or move.name != Moves::Thunder_Wave);
}

auto water_absorb_activates(Generation const generation, KnownMove const move) -> bool {
	return absorb_ability_activates(generation, move, Type::Water);
}

auto handle_ability_blocks_move(Generation const generation, KnownMove const move, MutableActivePokemon const target, Weather const weather) {
	switch (target.ability()) {
		case Ability::Flash_Fire:
			if (!flash_fire_activates(generation, move, target)) {
				return false;
			}
			target.activate_flash_fire();
			return true;
		case Ability::Volt_Absorb:
			if (!volt_absorb_activates(generation, move)) {
				return false;
			}
			heal(generation, target, weather, rational(1_bi, 4_bi));
			return true;
		case Ability::Water_Absorb:
		case Ability::Dry_Skin:
			if (!water_absorb_activates(generation, move)) {
				return false;
			}
			heal(generation, target, weather, rational(1_bi, 4_bi));
			return true;
		case Ability::Wonder_Guard:
			if (!is_damaging(move.name)) {
				return false;
			}
			if (Effectiveness(generation, move.type, target.types()).is_super_effective()) {
				return false;
			}
			switch (move.name) {
				case Moves::Struggle:
					return false;
				case Moves::Beat_Up:
					return generation >= Generation::five;
				case Moves::Fire_Fang:
					return generation != Generation::four;
				default:
					return true;
			}
		case Ability::Motor_Drive:
			if (!absorb_ability_activates(generation, move, Type::Electric)) {
				return false;
			}
			target.stage()[StatNames::SPE] += 1_bi;
			return true;
		default:
			return false;
	}
}

auto try_use_move(Generation const generation, Team & user, UsedMove const move, Team & other, OtherMove const other_move, Weather & weather, bool const clear_status, ActualDamage const actual_damage) -> void {
	if (move.selected == Moves::Pass) {
		return;
	}
	auto user_pokemon = user.pokemon();
	auto const found_move = find_move(all_moves(user_pokemon), move.selected);
	auto other_pokemon = other.pokemon();
	user_pokemon.update_before_move();
	if (!is_switch(move.selected)) {
		user_pokemon.advance_status_from_move(clear_status);
	}
	// Need the side-effect from recharge
	auto const is_recharging = user_pokemon.recharge();
	if (!can_attempt_move_execution(user_pokemon, found_move, other_pokemon)) {
		return;
	}
	user_pokemon.handle_confusion();
	if (move.executed != Moves::Hit_Self and user_pokemon.flinched()) {
		if (boosts_speed_when_flinched(user_pokemon.ability())) {
			user_pokemon.stage()[StatNames::SPE] += 1_bi;
		}
	}
	if (!can_execute_move(user_pokemon, found_move, weather, is_recharging)) {
		return;
	}
	
	auto const other_ability = other_pokemon.ability();

	if (is_regular(move.selected) and move.executed != Moves::Hit_Self and !user_pokemon.is_locked_in_by_move()) {
		auto & move_ref = find_regular_move(all_moves(user_pokemon).regular(), move.selected);
		move_ref.decrement_pp(other_ability);
		activate_pp_restore_berry(generation, move_ref, user_pokemon, weather);
	}
	
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

	auto const target = move_target(generation, move.executed);

	auto const unsuccessful =
		move_fails(move.executed, user_pokemon.damaged(), other_ability, other_move) or
		(get_hp(other_pokemon).current() == 0_bi and fails_against_fainted(target)) or
		(other_pokemon.is_protecting() and blocked_by_protect(target, move.executed));
	if (unsuccessful) {
		user_pokemon.unsuccessfully_use_move(move.executed);
		return;
	}

	auto const known_move = KnownMove{
		move.executed,
		get_type(generation, move.executed, get_hidden_power(user_pokemon).type())
	};
	if (!handle_ability_blocks_move(generation, known_move, other_pokemon, weather)) {
		auto const executed_move = ExecutedMove{
			known_move,
			found_move.pp(),
			move.variable,
			move.critical_hit
		};
		if (executed_move.move.name == Moves::Hit_Self) {
			use_move(generation, user, executed_move, target, user, other_move, weather, actual_damage);
		} else {
			use_move(generation, user, executed_move, target, other, other_move, weather, actual_damage);
		}
	}
	user_pokemon.increment_move_use_counter(move.executed);
}

} // namespace

auto call_move(Generation const generation, Team & user, UsedMove const move, Team & other, OtherMove const other_move, Weather & weather, bool const clear_status, ActualDamage const actual_damage) -> void {
	try_use_move(generation, user, move, other, other_move, weather, clear_status, actual_damage);
	end_of_attack(generation, user, other, weather);
}

} // namespace technicalmachine
