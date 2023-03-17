// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <tm/for_each_generation.hpp>

#include <bounded/conditional.hpp>

export module tm.move.side_effects;

import tm.move.healing_move_fails_in_generation_1;
import tm.move.move_name;
import tm.move.no_effect_function;
import tm.move.will_be_recharge_turn;
import tm.move.side_effect_function;

import tm.pokemon.active_pokemon;
import tm.pokemon.any_pokemon;
import tm.pokemon.change_hp;
import tm.pokemon.faint;
import tm.pokemon.is_type;
import tm.pokemon.known_pokemon;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.pokemon;
import tm.pokemon.seen_pokemon;
import tm.pokemon.species;
import tm.pokemon.stockpile;

import tm.stat.stage;
import tm.stat.stat_names;

import tm.status.blocks_status;
import tm.status.status;
import tm.status.status_name;

import tm.type.type;

import tm.ability;
import tm.any_team;
import tm.associated_team;
import tm.gender;
import tm.generation;
import tm.heal;
import tm.item;
import tm.other_team;
import tm.rational;
import tm.team;
import tm.weather;

import bounded;
import containers;
import numeric_traits;
import tv;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

template<any_team UserTeam>
struct SideEffect {
	double probability;
	SideEffectFunction<UserTeam> function;
};

template<any_team UserTeam>
using SideEffects = containers::static_vector<SideEffect<UserTeam>, 15_bi>;

template<any_team UserTeam>
constexpr auto guaranteed_effect(SideEffectFunction<UserTeam> function) {
	return SideEffects<UserTeam>({
		SideEffect<UserTeam>{
			1.0,
			function
		}
	});
}

constexpr auto reflected_status(Generation const generation, StatusName const status) -> tv::optional<StatusName> {
	switch (status) {
		case StatusName::burn:
		case StatusName::paralysis:
		case StatusName::poison:
			return status;
		case StatusName::toxic:
			return generation <= Generation::four ? StatusName::poison : StatusName::toxic;
		case StatusName::freeze:
		case StatusName::sleep:
			return tv::none;
		case StatusName::clear:
		case StatusName::rest:
			std::unreachable();
	}
}

template<any_mutable_active_pokemon UserPokemon>
auto apply_status(StatusName const status, UserPokemon const user, OtherMutableActivePokemon<UserPokemon> const target, Weather const weather) {
	target.set_status(status, weather);
	auto const reflected = reflected_status(generation_from<UserPokemon>, status);
	if (reflected and reflects_status(target.ability())) {
		user.set_status(*reflected, weather);
	}
}

template<any_team UserTeam>
constexpr auto no_effect = guaranteed_effect<UserTeam>(no_effect_function);

template<any_team UserTeam>
constexpr auto basic_probability(double const probability, auto function) {
	return probability == 1.0 ?
		guaranteed_effect<UserTeam>(function) :
		SideEffects<UserTeam>({
			SideEffect<UserTeam>{1.0 - probability, no_effect_function},
			SideEffect<UserTeam>{probability, function},
		});
}

template<any_team UserTeam>
constexpr auto absorb_effect = guaranteed_effect<UserTeam>([](auto & user, auto & other, auto & weather, auto const damage) {
	auto const user_pokemon = user.pokemon();
	auto const other_pokemon = other.pokemon();
	auto const absorbed = bounded::max(damage / 2_bi * healing_multiplier(user_pokemon.item(weather)), 1_bi);
	if (damages_leechers(other_pokemon.ability())) {
		change_hp(user_pokemon, weather, -absorbed);
	} else {
		change_hp(user_pokemon, weather, absorbed);
	}
});

template<typename Denominator>
struct RecoilEffect {
	static_assert(numeric_traits::min_value<Denominator> == numeric_traits::max_value<Denominator>);

	RecoilEffect() = default;
	constexpr explicit RecoilEffect(Denominator denominator):
		m_denominator(denominator)
	{
	}

	constexpr auto operator()(any_team auto & user, any_team auto &, Weather & weather, auto const damage) const {
		auto const user_pokemon = user.pokemon();
		if (!blocks_recoil(user_pokemon.ability())) {
			change_hp(user_pokemon, weather, -bounded::max(damage / m_denominator, 1_bi));
		}
	}

private:
	[[no_unique_address]] Denominator m_denominator;
};

template<any_active_pokemon TargetPokemon>
constexpr auto confusion_effect(double const probability, TargetPokemon const original_target, auto const... maybe_immune_type) {
	using UserTeam = OtherTeam<AssociatedTeam<TargetPokemon>>;
	return (... or is_type(original_target, maybe_immune_type)) ?
		no_effect<UserTeam> :
		basic_probability<UserTeam>(probability, [](auto &, auto & target, auto & weather, auto) {
			target.pokemon().confuse(weather);
		});
}

struct flinch_t {
	static constexpr auto operator()(auto &, auto & target, auto &, auto) -> void {
		target.pokemon().flinch();
	}
};
constexpr auto flinch = flinch_t();

template<BoostableStat stat, int stages>
constexpr auto boost_user_stat = [](auto & user, auto &, auto &, auto) {
	user.pokemon().stages()[stat] += bounded::constant<stages>;
};

template<BoostableStat stat, int stages>
constexpr auto boost_target_stat = [](auto &, auto & target, auto &, auto) {
	target.pokemon().stages()[stat] += bounded::constant<stages>;
};

constexpr auto status_is_clausable(StatusName const status) {
	switch (status) {
		case StatusName::burn:
		case StatusName::paralysis:
		case StatusName::poison:
		case StatusName::toxic:
		case StatusName::rest:
		case StatusName::clear:
			return false;
		case StatusName::freeze:
		case StatusName::sleep:
			return true;
	}
}


constexpr auto status_can_apply_ignoring_current_status(StatusName const status, any_active_pokemon auto const user, any_team auto const & target, Weather const weather, auto const... immune_types) {
	auto const target_pokemon = target.pokemon();
	return
		!blocks_status(target_pokemon.ability(), user.ability(), status, weather) and
		(... and !is_type(target_pokemon, immune_types)) and
		(!status_is_clausable(status) or !team_has_status(target, status)) and
		(status != StatusName::sleep or (!user.last_used_move().is_uproaring() and !target_pokemon.last_used_move().is_uproaring()));
}

constexpr auto status_can_apply(StatusName const status, any_active_pokemon auto const user, any_team auto const & target, Weather const weather, auto const... immune_types) {
	return
		is_clear(target.pokemon().status()) and
		status_can_apply_ignoring_current_status(status, user, target, weather, immune_types...);
}


template<StatusName status>
constexpr auto set_status_function = [](any_team auto & user, any_team auto & target, auto & weather, auto) {
	apply_status(status, user.pokemon(), target.pokemon(), weather);
};

struct clear_status_function_t {
	static constexpr auto operator()(any_team auto &, any_team auto & target, auto &, auto) -> void {
		target.pokemon().clear_status();
	}
};
constexpr auto clear_status_function = clear_status_function_t();

template<StatusName status, any_active_pokemon UserPokemon>
constexpr auto status_effect(double const probability, UserPokemon const original_user, OtherTeam<AssociatedTeam<UserPokemon>> const & original_target, Weather const original_weather, auto const... immune_types) {
	using UserTeam = AssociatedTeam<UserPokemon>;
	return status_can_apply(status, original_user, original_target, original_weather, immune_types...) ?
		basic_probability<UserTeam>(probability, set_status_function<status>) :
		no_effect<UserTeam>;
}


template<any_active_pokemon UserPokemon>
constexpr auto thaw_and_burn_effect(double const probability, UserPokemon const original_user, OtherTeam<AssociatedTeam<UserPokemon>> const & original_target, Weather const original_weather) {
	auto const target_status = original_target.pokemon().status().name();
	auto const will_thaw = target_status == StatusName::freeze;
	auto const can_burn =
		(target_status == StatusName::clear or will_thaw) and
		status_can_apply_ignoring_current_status(StatusName::burn, original_user, original_target, original_weather, Type::Fire);

	using UserTeam = AssociatedTeam<UserPokemon>;
	return
		can_burn and probability == 1.0 ? guaranteed_effect<UserTeam>(set_status_function<StatusName::burn>) :
		can_burn and will_thaw ? SideEffects<UserTeam>({
			SideEffect<UserTeam>{1.0 - probability, clear_status_function},
			SideEffect<UserTeam>{probability, set_status_function<StatusName::burn>},
		}) :
		can_burn ? basic_probability<UserTeam>(probability, set_status_function<StatusName::burn>) :
		will_thaw ? guaranteed_effect<UserTeam>(clear_status_function) :
		no_effect<UserTeam>;
}

template<any_team UserTeam, BoostableStat stat, int stages>
constexpr auto confusing_stat_boost = guaranteed_effect<UserTeam>([](auto &, auto & other, auto & weather, auto) {
	auto target = other.pokemon();
	auto & stage = target.stages()[stat];
	if (generation_from<UserTeam> <= Generation::two and stage == numeric_traits::max_value<Stage::value_type>) {
		return;
	}
	stage += bounded::constant<stages>;
	target.confuse(weather);
});

template<StatusName status, any_active_pokemon UserPokemon>
constexpr auto fang_effects(UserPokemon const original_user, OtherTeam<AssociatedTeam<UserPokemon>> const & original_target, Weather const original_weather, Type const immune_type) {
	using UserTeam = AssociatedTeam<UserPokemon>;
	constexpr auto status_and_flinch_function = [](auto & user, auto & target, auto & weather, auto const damage) {
		set_status_function<status>(user, target, weather, damage);
		flinch(user, target, weather, damage);
	};
	return status_can_apply(status, original_user, original_target, original_weather, immune_type) ?
		SideEffects<UserTeam>({
			{0.81, no_effect_function},
			{0.09, set_status_function<status>},
			{0.09, flinch},
			{0.01, status_and_flinch_function}
		}) :
		basic_probability<UserTeam>(0.1, flinch);
}

template<StatusName status, any_active_pokemon UserPokemon>
auto recoil_status(UserPokemon const original_user, OtherTeam<AssociatedTeam<UserPokemon>> const & original_target, Weather const original_weather, Type const immune_type) {
	using UserTeam = AssociatedTeam<UserPokemon>;
	constexpr auto recoil_and_status = [](auto & user, auto & target, auto & weather, auto const damage) {
		set_status_function<status>(user, target, weather, damage);
		RecoilEffect(3_bi)(user, target, weather, damage);
	};
	return status_can_apply(status, original_user, original_target, original_weather, immune_type) ?
		SideEffects<UserTeam>({
			{0.9, RecoilEffect(3_bi)},
			{0.1, recoil_and_status}
		}) :
		guaranteed_effect<UserTeam>(RecoilEffect(3_bi));
}

template<StatusName const status>
auto try_apply_status(auto & user, auto & target, auto & weather, auto const damage, auto const... immune_types) {
	if (status_can_apply(status, user.pokemon().as_const(), target, weather, immune_types...)) {
		set_status_function<status>(user, target, weather, damage);
	}
}

template<any_active_pokemon UserPokemon>
constexpr auto tri_attack_effect(UserPokemon const original_user, OtherTeam<AssociatedTeam<UserPokemon>> const & original_target, Weather const original_weather) {
	using UserTeam = AssociatedTeam<UserPokemon>;
	// TODO: Foresight, Wonder Guard, Scrappy
	if (is_type(original_target.pokemon(), Type::Ghost)) {
		return no_effect<UserTeam>;
	}
	constexpr auto generation = generation_from<UserPokemon>;
	constexpr auto burn = SideEffect<UserTeam>({1.0 / 15.0, set_status_function<StatusName::burn>});
	constexpr auto freeze = SideEffect<UserTeam>({1.0 / 15.0, set_status_function<StatusName::freeze>});
	constexpr auto paralysis = SideEffect<UserTeam>({1.0 / 15.0, set_status_function<StatusName::paralysis>});

	constexpr auto burn_freeze_paralysis_probabilities = SideEffects<UserTeam>({{12.0 / 15.0, no_effect_function}, burn, freeze, paralysis});
	constexpr auto burn_freeze_probabilities = SideEffects<UserTeam>({{13.0 / 15.0, no_effect_function}, burn, freeze});
	constexpr auto burn_paralysis_probabilities = SideEffects<UserTeam>({{13.0 / 15.0, no_effect_function}, burn, paralysis});
	constexpr auto freeze_paralysis_probabilities = SideEffects<UserTeam>({{13.0 / 15.0, no_effect_function}, freeze, paralysis});
	constexpr auto burn_probabilities = SideEffects<UserTeam>({{14.0 / 15.0, no_effect_function}, burn});
	constexpr auto freeze_probabilities = SideEffects<UserTeam>({{14.0 / 15.0, no_effect_function}, freeze});
	constexpr auto paralysis_probabilities = SideEffects<UserTeam>({{14.0 / 15.0, no_effect_function}, paralysis});

	switch (generation) {
		case Generation::one:
			return no_effect<UserTeam>;
		case Generation::two: {
			constexpr auto thaw = SideEffect<UserTeam>{4.0 / 15.0, [](auto &, auto & target, auto &, auto) {
				target.pokemon().clear_status();
			}};
			auto const freeze_claused = team_has_status(original_target, StatusName::freeze);
			switch (original_target.pokemon().status().name()) {
				case StatusName::clear:
					return freeze_claused ?
						burn_paralysis_probabilities :
						burn_freeze_paralysis_probabilities;
				case StatusName::freeze:
					return SideEffects<UserTeam>({{10.0 / 15.0, no_effect_function}, thaw, burn});
				default:
					return no_effect<UserTeam>;
			}
		}
		default: {
			auto const can_burn = status_can_apply(StatusName::burn, original_user, original_target, original_weather, Type::Fire);
			auto const can_freeze = status_can_apply(StatusName::freeze, original_user, original_target, original_weather, Type::Ice);
			auto const can_paralyze = generation <= Generation::five ?
				status_can_apply(StatusName::paralysis, original_user, original_target, original_weather) :
				status_can_apply(StatusName::paralysis, original_user, original_target, original_weather, Type::Electric);

			return
				can_burn and can_freeze and can_paralyze ? burn_freeze_paralysis_probabilities :
				can_burn and can_freeze ? burn_freeze_probabilities :
				can_burn and can_paralyze ? burn_paralysis_probabilities :
				can_freeze and can_paralyze ? freeze_paralysis_probabilities :
				can_burn ? burn_probabilities :
				can_freeze ? freeze_probabilities :
				can_paralyze ? paralysis_probabilities :
				no_effect<UserTeam>;
		}
	}
}

constexpr auto cure_all_status(any_team auto & user, auto const & predicate) -> void {
	for (auto & pokemon : user.all_pokemon()) {
		if (predicate(pokemon)) {
			pokemon.set_status(StatusName::clear);
		}
	}
}

constexpr auto do_aromatherapy(any_team auto & user) -> void {
	cure_all_status(user, [=](any_pokemon auto const &) { return true; });
}

template<any_team UserTeam>
constexpr auto do_heal_bell(UserTeam & user) -> void {
	cure_all_status(user, [=](any_pokemon auto const & pokemon) {
		switch (generation_from<UserTeam>) {
			case Generation::one:
			case Generation::two:
				return true;
			case Generation::three:
			case Generation::four: {
				auto const is_active = std::addressof(pokemon) == std::addressof(user.all_pokemon()());
				auto const ability = is_active ? user.pokemon().ability() : pokemon.initial_ability();
				return !blocks_sound_moves(ability);
			}
			case Generation::five:
				return true;
			case Generation::six:
			case Generation::seven: {
				if (std::addressof(pokemon) != std::addressof(user.all_pokemon()())) {
					return true;
				}
				return !blocks_sound_moves(user.pokemon().ability());
			}
			case Generation::eight:
				return true;
		}
	});
}

struct fling_effects_t {
	static constexpr auto operator()(auto & user_team, auto & target_team, auto & weather, auto const damage) -> void {
		auto user = user_team.pokemon();
		auto target = target_team.pokemon();
		// TODO: Activate berry
		switch (user.item(weather)) {
			case Item::Flame_Orb:
				try_apply_status<StatusName::burn>(user_team, target_team, weather, damage, Type::Fire);
				break;
			case Item::Kings_Rock:
			case Item::Razor_Fang:
				target.flinch();
				break;
			case Item::Light_Ball:
				try_apply_status<StatusName::paralysis>(user_team, target_team, weather, damage);
				break;
			case Item::Mental_Herb:
				target.apply_mental_herb();
				break;
			case Item::Poison_Barb:
				try_apply_status<StatusName::poison>(user_team, target_team, weather, damage, Type::Poison, Type::Steel);
				break;
			case Item::Toxic_Orb:
				try_apply_status<StatusName::toxic>(user_team, target_team, weather, damage, Type::Poison, Type::Steel);
				break;
			case Item::White_Herb:
				apply_white_herb(target);
				break;
			default:
				break;
		}
		user.remove_item();
	}
};
constexpr auto fling_effects = fling_effects_t();

// Not a lambda because of https://github.com/llvm/llvm-project/issues/59513
struct recover_half_t {
	static constexpr auto operator()(auto & user, auto &, auto & weather, auto) -> void {
		heal(user.pokemon(), weather, rational(1_bi, 2_bi));
	}
};
constexpr auto recover_half = recover_half_t();

constexpr auto stat_can_boost(Stage const stage) {
	return stage.value() != numeric_traits::max_value<Stage::value_type>;
}

template<auto...>
struct sequence {};

template<any_active_pokemon TargetPokemon>
constexpr auto acupressure_effect(TargetPokemon const target) {
	using UserTeam = AssociatedTeam<TargetPokemon>;
	auto result = SideEffects<UserTeam>();
	auto const stages = target.stages();
	auto const probability = 1.0 / double(containers::count_if(stages, stat_can_boost));

	auto add_stat = [&]<BoostableStat stat>(std::integral_constant<BoostableStat, stat>) {
		if (stat_can_boost(stages[stat])) {
			containers::push_back(result, SideEffect<UserTeam>{probability, boost_user_stat<stat, 2>});
		}
	};
	auto add_stats = [&]<BoostableStat... stats>(sequence<stats...>) {
		(..., add_stat(std::integral_constant<BoostableStat, stats>()));
	};
	add_stats(sequence<
		BoostableStat::atk,
		BoostableStat::def,
		BoostableStat::spa,
		BoostableStat::spd,
		BoostableStat::spe,
		BoostableStat::acc,
		BoostableStat::eva
	>());
	return result;
}

constexpr auto active_pokemon_can_be_phazed(any_team auto const & team) {
	return !team.pokemon().ingrained() and !blocks_phazing(team.pokemon().ability()) and team.size() > 1_bi;
}

template<auto index>
constexpr auto phaze = [](any_team auto & user, any_team auto & target, Weather & weather, auto) {
	target.switch_pokemon(user.pokemon(), weather, index);
};

template<any_team TargetTeam>
constexpr auto phaze_effect(TargetTeam const & target) {
	using UserTeam = OtherTeam<TargetTeam>;
	if (!active_pokemon_can_be_phazed(target)) {
		return no_effect<UserTeam>;
	}
	// TODO: Phazing activates Synchronize if Toxic Spikes cause status before
	// generation 5
	auto is_not_active = [&](TeamIndex const index) {
		return index != target.all_pokemon().index();
	};

	auto const probability = 1.0 / double(target.size() - 1_bi);
	auto result = SideEffects<UserTeam>();
	auto add_one = [&](auto const index) {
		if (is_not_active(index) and index < target.size()) {
			containers::push_back(result, SideEffect<UserTeam>{probability, phaze<index>});
		}
	};
	auto add_all = [&]<std::size_t... indexes>(std::index_sequence<indexes...>) {
		(..., add_one(bounded::constant<indexes>));
	};
	add_all(bounded::make_index_sequence(numeric_traits::max_value<TeamSize>));
	return result;
}

template<auto reduction>
constexpr auto reduce_pp = [](any_team auto & user, any_team auto &, Weather & weather, auto) {
	auto const pokemon = user.pokemon();
	pokemon.reduce_pp(pokemon.last_used_move().name(), weather, reduction);
};

template<int, typename>
struct increase_by;

template<int minimum, int... values>
struct increase_by<minimum, std::integer_sequence<int, values...>> {
	using type = std::integer_sequence<int, (values + minimum)...>;
};

template<int minimum, int maximum>
using make_integer_sequence = typename increase_by<minimum, std::make_integer_sequence<int, maximum - minimum + 1>>::type;

template<any_team UserTeam>
constexpr auto random_spite = [] {
	constexpr auto min_reduction = 2;
	constexpr auto max_reduction = 5;
	constexpr auto probability = 1.0 / double(max_reduction - min_reduction + 1);
	auto result = SideEffects<UserTeam>();
	auto add_one = [&](auto const index) {
		containers::push_back(result, SideEffect<UserTeam>{probability, reduce_pp<index>});
	};
	auto add_all = [&]<int... indexes>(std::integer_sequence<int, indexes...>) {
		(..., add_one(bounded::constant<indexes>));
	};
	add_all(make_integer_sequence<min_reduction, max_reduction>());
	return result;
}();

template<any_team UserTeam, int index>
constexpr auto switch_effect = guaranteed_effect<UserTeam>([](UserTeam & user, OtherTeam<UserTeam> & other, Weather & weather, auto) {
	user.switch_pokemon(other.pokemon(), weather, bounded::constant<index>);
});

template<any_active_pokemon UserPokemon>
constexpr auto charge_up_move(
	MoveName const move_name,
	UserPokemon const original_user,
	OtherActivePokemon<UserPokemon> const original_other,
	Weather const original_weather,
	SideEffects<AssociatedTeam<UserPokemon>> when_used
) {
	return will_be_recharge_turn(original_user, move_name, original_other.ability(), original_weather) ?
		guaranteed_effect<AssociatedTeam<UserPokemon>>([](auto & user, auto &, auto &, auto) {
			user.pokemon().use_charge_up_move();
		}) :
		std::move(when_used);
}

template<any_active_pokemon PokemonType>
auto item_can_be_lost(PokemonType const pokemon) {
	return
		pokemon.ability() != Ability::Sticky_Hold or
		(generation_from<PokemonType> >= Generation::five and pokemon.hp().current() == 0_bi);
}

auto item_can_be_incinerated(any_active_pokemon auto const target, Weather const weather) -> bool {
	// TODO: Destroy gems
	return item_can_be_lost(target) and berry_power(target.item(weather)) != 0_bi;
}

struct equalize_hp_t {
	constexpr auto operator()(auto & user, auto & other, auto & weather, auto) -> void {
		auto const lhs = user.pokemon();
		auto const rhs = other.pokemon();
		auto const average = (lhs.hp().current() + rhs.hp().current()) / 2_bi;
		lhs.set_hp(weather, average);
		rhs.set_hp(weather, average);
	}
};
constexpr auto equalize_hp = equalize_hp_t();

constexpr auto can_confuse_with_chatter(Species const pokemon) {
	return pokemon == Species::Chatot;
}

export template<any_active_pokemon UserPokemon>
auto possible_side_effects(MoveName const move, UserPokemon const original_user, OtherTeam<AssociatedTeam<UserPokemon>> const & original_other, Weather const original_weather) -> SideEffects<AssociatedTeam<UserPokemon>> {
	using UserTeam = AssociatedTeam<UserPokemon>;
	constexpr auto generation = generation_from<UserPokemon>;
	switch (move) {
		case MoveName::Absorb:
		case MoveName::Drain_Punch:
		case MoveName::Giga_Drain:
		case MoveName::Leech_Life:
		case MoveName::Mega_Drain:
			return absorb_effect<UserTeam>;
		case MoveName::Dream_Eater:
			return is_sleeping(original_other.pokemon().status()) ? absorb_effect<UserTeam> : no_effect<UserTeam>;
		case MoveName::Brave_Bird:
		case MoveName::Double_Edge:
		case MoveName::Wood_Hammer:
			return generation <= Generation::two ?
				guaranteed_effect<UserTeam>(RecoilEffect(4_bi)) :
				guaranteed_effect<UserTeam>(RecoilEffect(3_bi));
		case MoveName::Head_Smash:
			return guaranteed_effect<UserTeam>(RecoilEffect(2_bi));
		case MoveName::Submission:
		case MoveName::Take_Down:
			return guaranteed_effect<UserTeam>(RecoilEffect(4_bi));

		case MoveName::Metal_Claw:
			return basic_probability<UserTeam>(0.1, boost_user_stat<BoostableStat::atk, 1>);
		case MoveName::Meteor_Mash:
			return basic_probability<UserTeam>(0.2, boost_user_stat<BoostableStat::atk, 1>);
		case MoveName::Howl:
		case MoveName::Meditate:
		case MoveName::Sharpen:
			return guaranteed_effect<UserTeam>(boost_user_stat<BoostableStat::atk, 1>);
		case MoveName::Swords_Dance:
			return guaranteed_effect<UserTeam>(boost_user_stat<BoostableStat::atk, 2>);
		case MoveName::Charm:
		case MoveName::Feather_Dance:
			return guaranteed_effect<UserTeam>(boost_target_stat<BoostableStat::atk, -2>);
		case MoveName::Aurora_Beam:
			return basic_probability<UserTeam>(generation == Generation::one ? 0.332 : 0.1, boost_target_stat<BoostableStat::atk, -1>);
		case MoveName::Breaking_Swipe:
		case MoveName::Growl:
			return guaranteed_effect<UserTeam>(boost_target_stat<BoostableStat::atk, -1>);

		case MoveName::Steel_Wing:
			return basic_probability<UserTeam>(0.1, boost_user_stat<BoostableStat::def, 1>);
		case MoveName::Harden:
		case MoveName::Withdraw:
			return guaranteed_effect<UserTeam>(boost_user_stat<BoostableStat::def, 1>);
		case MoveName::Acid_Armor:
		case MoveName::Barrier:
		case MoveName::Iron_Defense:
			return guaranteed_effect<UserTeam>(boost_user_stat<BoostableStat::def, 2>);
		case MoveName::Screech:
			return guaranteed_effect<UserTeam>(boost_target_stat<BoostableStat::def, -2>);
		case MoveName::Iron_Tail:
			return basic_probability<UserTeam>(0.3, boost_target_stat<BoostableStat::def, -1>);
		case MoveName::Crunch: {
			constexpr auto stat = generation <= Generation::three ? BoostableStat::spd : BoostableStat::def;
			return basic_probability<UserTeam>(0.2, boost_target_stat<stat, -1>);
		}
		case MoveName::Crush_Claw:
		case MoveName::Razor_Shell:
		case MoveName::Rock_Smash:
			return basic_probability<UserTeam>(0.5, boost_target_stat<BoostableStat::def, -1>);
		case MoveName::Grav_Apple:
		case MoveName::Leer:
		case MoveName::Tail_Whip:
		case MoveName::Thunderous_Kick:
			return guaranteed_effect<UserTeam>(boost_target_stat<BoostableStat::def, -1>);

		case MoveName::Draco_Meteor:
		case MoveName::Leaf_Storm:
		case MoveName::Overheat:
		case MoveName::Psycho_Boost:
			return guaranteed_effect<UserTeam>(boost_user_stat<BoostableStat::spa, -2>);
		case MoveName::Mist_Ball:
			return basic_probability<UserTeam>(0.5, boost_user_stat<BoostableStat::spa, 1>);
		case MoveName::Charge_Beam:
			return basic_probability<UserTeam>(0.7, boost_user_stat<BoostableStat::spa, 1>);
		case MoveName::Nasty_Plot:
		case MoveName::Tail_Glow:
			return guaranteed_effect<UserTeam>(boost_user_stat<BoostableStat::spa, 2>);
		case MoveName::Skitter_Smack:
		case MoveName::Snarl:
		case MoveName::Spirit_Break:
		case MoveName::Struggle_Bug:
			return guaranteed_effect<UserTeam>(boost_target_stat<BoostableStat::spa, -1>);

		case MoveName::Seed_Flare:
			return basic_probability<UserTeam>(0.4, boost_target_stat<BoostableStat::spd, -2>);
		case MoveName::Fake_Tears:
		case MoveName::Metal_Sound:
			return guaranteed_effect<UserTeam>(boost_target_stat<BoostableStat::spd, -2>);
		case MoveName::Captivate:
			return multiplier(original_user.gender(), original_other.pokemon().gender()) == -1_bi ?
				guaranteed_effect<UserTeam>(boost_target_stat<BoostableStat::spd, -2>) :
				no_effect<UserTeam>;
		case MoveName::Bug_Buzz:
		case MoveName::Earth_Power:
		case MoveName::Energy_Ball:
		case MoveName::Flash_Cannon:
		case MoveName::Focus_Blast:
			return basic_probability<UserTeam>(0.1, boost_target_stat<BoostableStat::spd, -1>);
		case MoveName::Shadow_Ball:
			return basic_probability<UserTeam>(0.2, boost_target_stat<BoostableStat::spd, -1>);
		case MoveName::Apple_Acid:
		case MoveName::Luster_Purge:
			return guaranteed_effect<UserTeam>(boost_target_stat<BoostableStat::spd, -1>);

		case MoveName::Aura_Wheel:
			return guaranteed_effect<UserTeam>(boost_user_stat<BoostableStat::spe, 1>);
		case MoveName::Hammer_Arm:
			return guaranteed_effect<UserTeam>(boost_user_stat<BoostableStat::spe, -1>);
		case MoveName::Agility:
		case MoveName::Rock_Polish:
			return guaranteed_effect<UserTeam>(boost_user_stat<BoostableStat::spe, 2>);
		case MoveName::Scary_Face:
			return guaranteed_effect<UserTeam>(boost_target_stat<BoostableStat::spe, -2>);
		case MoveName::Cotton_Spore:
			return (generation <= Generation::five or !is_type(original_other.pokemon(), Type::Grass)) ?
				guaranteed_effect<UserTeam>(boost_target_stat<BoostableStat::spe, -2>) :
				no_effect<UserTeam>;
		case MoveName::Bubble:
		case MoveName::Bubble_Beam:
		case MoveName::Constrict:
			return basic_probability<UserTeam>(
				generation == Generation::one ? 0.332 : 0.1,
				boost_target_stat<BoostableStat::spe, -1>
			);
		case MoveName::Drum_Beating:
		case MoveName::Icy_Wind:
		case MoveName::Low_Sweep:
		case MoveName::Mud_Shot:
		case MoveName::Rock_Tomb:
			return generation <= Generation::two ?
				basic_probability<UserTeam>(0.996, boost_target_stat<BoostableStat::spe, -1>) :
				guaranteed_effect<UserTeam>(boost_target_stat<BoostableStat::spe, -1>);

		case MoveName::Leaf_Tornado:
		case MoveName::Mirror_Shot:
		case MoveName::Mud_Bomb:
		case MoveName::Muddy_Water:
			return basic_probability<UserTeam>(0.3, boost_target_stat<BoostableStat::acc, -1>);
		case MoveName::Night_Daze:
			return basic_probability<UserTeam>(0.4, boost_target_stat<BoostableStat::acc, -1>);
		case MoveName::Octazooka:
			return basic_probability<UserTeam>(0.5, boost_target_stat<BoostableStat::acc, -1>);
		case MoveName::Flash:
		case MoveName::Kinesis:
		case MoveName::Sand_Attack:
		case MoveName::Smokescreen:
			return guaranteed_effect<UserTeam>(boost_target_stat<BoostableStat::acc, -1>);
		case MoveName::Mud_Slap:
			return generation <= Generation::two ?
				basic_probability<UserTeam>(0.996, boost_target_stat<BoostableStat::acc, -1>) :
				guaranteed_effect<UserTeam>(boost_target_stat<BoostableStat::acc, -1>);

		case MoveName::Double_Team:
			return guaranteed_effect<UserTeam>(boost_user_stat<BoostableStat::eva, 1>);
		case MoveName::Sweet_Scent:
			return generation <= Generation::five ?
				guaranteed_effect<UserTeam>(boost_target_stat<BoostableStat::eva, -1>) :
				guaranteed_effect<UserTeam>(boost_target_stat<BoostableStat::eva, -2>);

		case MoveName::Acid: {
			constexpr auto probability = generation == Generation::one ? 0.332 : 0.1;
			constexpr auto stat = generation <= Generation::three ? BoostableStat::def : BoostableStat::spd;
			return basic_probability<UserTeam>(probability, boost_target_stat<stat, -1>);
		}
		case MoveName::Acupressure:
			// TODO: Probability not correct due to the maxing out behavior
			return acupressure_effect(original_user);
		case MoveName::Amnesia:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				auto pokemon = user.pokemon();
				pokemon.stages()[BoostableStat::spd] += 2_bi;
				if (generation == Generation::one) {
					pokemon.stages()[BoostableStat::spa] += 2_bi;
				}
			});
		case MoveName::Ancient_Power:
		case MoveName::Ominous_Wind:
		case MoveName::Silver_Wind:
			return basic_probability<UserTeam>(0.1, [](auto & user, auto &, auto &, auto) {
				boost_regular(user.pokemon().stages(), 1_bi);
			});
		case MoveName::Belly_Drum:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto & weather, auto) {
				auto const user_pokemon = user.pokemon();
				auto const hp = user_pokemon.hp();
				if (hp.current() > hp.max() / 2_bi and hp.current() > 1_bi) {
					change_hp(user_pokemon, weather, -hp.max() / 2_bi);
					user_pokemon.stages()[BoostableStat::atk] = Stage(6_bi);
				} else if constexpr (generation == Generation::two) {
					user_pokemon.stages()[BoostableStat::atk] += 2_bi;
				}
			});
		case MoveName::Clangorous_Soul:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto & weather, auto) {
				auto const user_pokemon = user.pokemon();
				auto const hp = user_pokemon.hp();
				if (hp.current() > hp.max() / 3_bi and hp.current() > 1_bi) {
					change_hp(user_pokemon, weather, -hp.max() / 3_bi);
					boost_regular(user.pokemon().stages(), 1_bi);
				}
			});
		case MoveName::Close_Combat:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				boost_physical(user.pokemon().stages(), -1_bi);
			});
		case MoveName::Bulk_Up:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				boost_physical(user.pokemon().stages(), 1_bi);
			});
		case MoveName::Calm_Mind:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				boost_special(user.pokemon().stages(), 1_bi);
			});
		case MoveName::Cosmic_Power:
		case MoveName::Defend_Order:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				boost_defensive(user.pokemon().stages(), 1_bi);
			});
		case MoveName::Decorate:
			return guaranteed_effect<UserTeam>([](auto &, auto & target, auto &, auto) {
				boost_offensive(target.pokemon().stages(), 2_bi);
			});
		case MoveName::Dragon_Dance:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				auto & stages = user.pokemon().stages();
				stages[BoostableStat::atk] += 1_bi;
				stages[BoostableStat::spe] += 1_bi;
			});
		case MoveName::Growth:
			return guaranteed_effect<UserTeam>([](auto & user, auto & other, auto & weather, auto) {
				auto const user_pokemon = user.pokemon();
				auto & stages = user_pokemon.stages();
				switch (generation) {
					case Generation::one:
						for (auto const stat : {BoostableStat::spa, BoostableStat::spd}) {
							stages[stat] += 1_bi;
						}
						break;
					case Generation::two:
					case Generation::three:
					case Generation::four:
						stages[BoostableStat::spa] += 1_bi;
						break;
					default: {
						auto const boost = BOUNDED_CONDITIONAL(
							weather.sun(weather_is_blocked_by_ability(user_pokemon.ability(), other.pokemon().ability())),
							2_bi,
							1_bi
						);
						for (auto const stat : {BoostableStat::atk, BoostableStat::spa}) {
							stages[stat] += boost;
						}
						break;
					}
				}
			});
		case MoveName::Psychic:
			return generation == Generation::one ?
				basic_probability<UserTeam>(0.332, [](auto &, auto & other, auto &, auto) {
					auto & stages = other.pokemon().stages();
					stages[BoostableStat::spa] -= 1_bi;
					stages[BoostableStat::spd] -= 1_bi;
				}) :
				basic_probability<UserTeam>(0.1, boost_target_stat<BoostableStat::spd, -1>);
		case MoveName::Quiver_Dance:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				auto & stages = user.pokemon().stages();
				for (auto const stat : {BoostableStat::spa, BoostableStat::spd, BoostableStat::spe}) {
					stages[stat] += 1_bi;
				}
			});
		case MoveName::Scale_Shot:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				auto & stages = user.pokemon().stages();
				stages[BoostableStat::def] -= 1_bi;
				stages[BoostableStat::spe] += 1_bi;
			});
		case MoveName::Shell_Smash:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				auto & stages = user.pokemon().stages();
				for (auto const stat : {BoostableStat::def, BoostableStat::spd}) {
					stages[stat] -= 1_bi;
				}
				for (auto const stat : {BoostableStat::atk, BoostableStat::spa, BoostableStat::spe}) {
					stages[stat] += 2_bi;
				}
			});
		case MoveName::Shift_Gear:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				auto & stages = user.pokemon().stages();
				stages[BoostableStat::atk] += 1_bi;
				stages[BoostableStat::spe] += 2_bi;
			});
		case MoveName::String_Shot:
			return generation <= Generation::five ?
				guaranteed_effect<UserTeam>(boost_target_stat<BoostableStat::spe, -1>) :
				guaranteed_effect<UserTeam>(boost_target_stat<BoostableStat::spe, -2>);
		case MoveName::Superpower:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				boost_physical(user.pokemon().stages(), -1_bi);
			});
		case MoveName::Tickle:
			return guaranteed_effect<UserTeam>([](auto &, auto & other, auto &, auto) {
				boost_physical(other.pokemon().stages(), -1_bi);
			});
		case MoveName::V_create:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				auto & stages = user.pokemon().stages();
				for (auto const stat : {BoostableStat::def, BoostableStat::spd, BoostableStat::spe}) {
					stages[stat] -= 1_bi;
				}
			});

		case MoveName::Haze:
			return guaranteed_effect<UserTeam>([](auto & user, auto & other, auto &, auto) {
				user.pokemon().stages() = Stages();
				other.pokemon().stages() = Stages();
				if (generation == Generation::one) {
					user.shatter_screens();
					other.shatter_screens();
				}
			});
		case MoveName::Power_Swap:
			return guaranteed_effect<UserTeam>([](auto & user, auto & other, auto &, auto) {
				swap_offensive(user.pokemon().stages(), other.pokemon().stages());
			});
		case MoveName::Psych_Up:
			return generation >= Generation::three or containers::any(original_other.pokemon().stages(), [](Stage const stage) { return stage != 0_bi; }) ?
				guaranteed_effect<UserTeam>([](auto & user, auto & other, auto &, auto) {
					user.pokemon().stages() = other.pokemon().stages();
				}) :
				no_effect<UserTeam>;

		case MoveName::Charge:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				auto const pokemon = user.pokemon();
				pokemon.charge();
				pokemon.stages()[BoostableStat::spd] += 1_bi;
			});
		case MoveName::Defense_Curl:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				auto const pokemon = user.pokemon();
				pokemon.stages()[BoostableStat::def] += 1_bi;
				pokemon.defense_curl();
			});

		case MoveName::Bone_Club:
		case MoveName::Extrasensory:
		case MoveName::Headbutt:
		case MoveName::Hyper_Fang:
		case MoveName::Stomp:
			return basic_probability<UserTeam>(0.1, flinch);
		case MoveName::Dark_Pulse:
		case MoveName::Dragon_Rush:
		case MoveName::Fiery_Wrath:
		case MoveName::Twister:
		case MoveName::Zen_Headbutt:
			return basic_probability<UserTeam>(0.2, flinch);
		case MoveName::Air_Slash:
		case MoveName::Astonish:
		case MoveName::Icicle_Crash:
		case MoveName::Iron_Head:
		case MoveName::Rolling_Kick:
		case MoveName::Snore:
		case MoveName::Steamroller:
		case MoveName::Needle_Arm:
			return basic_probability<UserTeam>(0.3, flinch);
		case MoveName::Fake_Out:
			return guaranteed_effect<UserTeam>(flinch);
		case MoveName::Bite:
			return basic_probability<UserTeam>(generation <= Generation::one ? 0.1 : 0.3, flinch);
		case MoveName::Rock_Slide:
			return generation == Generation::one ? no_effect<UserTeam> : basic_probability<UserTeam>(0.3, flinch);
		case MoveName::Waterfall:
			return generation <= Generation::three ? no_effect<UserTeam> : basic_probability<UserTeam>(0.2, flinch);
		case MoveName::Low_Kick:
			return generation <= Generation::two ? basic_probability<UserTeam>(0.3, flinch) : no_effect<UserTeam>;

		case MoveName::Aromatherapy:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				do_aromatherapy(user);
			});
		case MoveName::Heal_Bell:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				do_heal_bell(user);
			});
		case MoveName::Psycho_Shift:
			return guaranteed_effect<UserTeam>([](auto & user, auto & other, auto & weather, auto const damage) {
				auto const other_pokemon = other.pokemon();
				if (!is_clear(other_pokemon.status())) {
					return;
				}
				auto const user_pokemon = user.pokemon();
				auto const status_name = user_pokemon.status().name();
				switch (status_name) {
					case StatusName::burn:
						try_apply_status<StatusName::burn>(user, other, weather, damage, Type::Fire);
						break;
					case StatusName::paralysis:
						try_apply_status<StatusName::paralysis>(user, other, weather, damage);
						break;
					case StatusName::poison:
						try_apply_status<StatusName::poison>(user, other, weather, damage, Type::Poison, Type::Steel);
						break;
					case StatusName::toxic:
						try_apply_status<StatusName::toxic>(user, other, weather, damage, Type::Poison, Type::Steel);
						break;
					case StatusName::sleep: // TODO: Sleep Clause
					case StatusName::rest: // TODO: How does Rest shift?
						try_apply_status<StatusName::sleep>(user, other, weather, damage);
						break;
					case StatusName::clear:
					case StatusName::freeze:
						break;
				}
				user_pokemon.clear_status();
			});
		case MoveName::Refresh:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				user.pokemon().clear_status();
			});
		case MoveName::Jungle_Healing:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto & weather, auto) {
				auto const pokemon = user.pokemon();
				heal(pokemon, weather, rational(1_bi, 4_bi));
				pokemon.clear_status();
			});
		case MoveName::Wake_Up_Slap:
			return guaranteed_effect<UserTeam>([](auto &, auto & other, auto &, auto) {
				auto const pokemon = other.pokemon();
				if (is_sleeping(pokemon.status())) {
					pokemon.clear_status();
				}
			});

		case MoveName::Blaze_Kick:
		case MoveName::Ember:
		case MoveName::Fire_Punch:
		case MoveName::Flame_Wheel:
		case MoveName::Flamethrower:
		case MoveName::Heat_Wave:
		case MoveName::Pyro_Ball:
			return thaw_and_burn_effect(0.1, original_user, original_other, original_weather);
		case MoveName::Lava_Plume:
		case MoveName::Scald:
		case MoveName::Scorching_Sands:
		case MoveName::Searing_Shot:
			return thaw_and_burn_effect(0.3, original_user, original_other, original_weather);
		case MoveName::Sacred_Fire:
			return thaw_and_burn_effect(0.5, original_user, original_other, original_weather);
		case MoveName::Inferno:
		case MoveName::Will_O_Wisp:
			return thaw_and_burn_effect(1.0, original_user, original_other, original_weather);
		case MoveName::Fire_Blast:
			return thaw_and_burn_effect(generation == Generation::one ? 0.3 : 0.1, original_user, original_other, original_weather);

		case MoveName::Blizzard:
		case MoveName::Freezing_Glare:
		case MoveName::Ice_Beam:
		case MoveName::Ice_Punch:
		case MoveName::Powder_Snow:
			return status_effect<StatusName::freeze>(0.1, original_user, original_other, original_weather, Type::Ice);

		case MoveName::Dragon_Breath:
			return status_effect<StatusName::paralysis>(0.3, original_user, original_other, original_weather);
		case MoveName::Force_Palm:
		case MoveName::Secret_Power:
			return status_effect<StatusName::paralysis>(0.3, original_user, original_other, original_weather, Type::Ghost);
		case MoveName::Discharge:
		case MoveName::Spark:
			return status_effect<StatusName::paralysis>(0.3, original_user, original_other, original_weather, Type::Ground);
		case MoveName::Thunder_Wave:
		case MoveName::Zap_Cannon:
			return generation <= Generation::two ?
				status_effect<StatusName::paralysis>(0.996, original_user, original_other, original_weather, Type::Ground) :
				status_effect<StatusName::paralysis>(1.0, original_user, original_other, original_weather, Type::Ground);
		case MoveName::Body_Slam:
			return generation == Generation::one ?
				status_effect<StatusName::paralysis>(0.3, original_user, original_other, original_weather, Type::Ghost, Type::Normal) :
				status_effect<StatusName::paralysis>(0.3, original_user, original_other, original_weather, Type::Ghost);
		case MoveName::Lick:
			return generation == Generation::one ?
				status_effect<StatusName::paralysis>(0.3, original_user, original_other, original_weather, Type::Normal, Type::Ghost) :
				status_effect<StatusName::paralysis>(0.3, original_user, original_other, original_weather, Type::Normal);
		case MoveName::Thunderbolt:
		case MoveName::Thunder_Punch:
		case MoveName::Thunder_Shock:
			return generation == Generation::one ?
				status_effect<StatusName::paralysis>(0.1, original_user, original_other, original_weather, Type::Ground, Type::Electric) :
				status_effect<StatusName::paralysis>(0.1, original_user, original_other, original_weather, Type::Ground);
		case MoveName::Thunder:
			return generation == Generation::one ?
				status_effect<StatusName::paralysis>(0.1, original_user, original_other, original_weather, Type::Ground, Type::Electric) :
				status_effect<StatusName::paralysis>(0.3, original_user, original_other, original_weather, Type::Ground);
		case MoveName::Stun_Spore:
			return generation <= Generation::five ?
				status_effect<StatusName::paralysis>(1.0, original_user, original_other, original_weather) :
				status_effect<StatusName::paralysis>(1.0, original_user, original_other, original_weather, Type::Grass);
		case MoveName::Glare:
			switch (generation) {
				case Generation::one:
					return status_effect<StatusName::paralysis>(1.0, original_user, original_other, original_weather);
				case Generation::two:
				case Generation::three:
					return status_effect<StatusName::paralysis>(1.0, original_user, original_other, original_weather, Type::Ghost);
				case Generation::four:
				case Generation::five:
					return status_effect<StatusName::paralysis>(1.0, original_user, original_other, original_weather);
				case Generation::six:
				case Generation::seven:
				case Generation::eight:
					return status_effect<StatusName::paralysis>(1.0, original_user, original_other, original_weather, Type::Electric);
			}

		case MoveName::Cross_Poison:
		case MoveName::Poison_Tail:
		case MoveName::Sludge_Wave:
			return status_effect<StatusName::poison>(0.1, original_user, original_other, original_weather, Type::Poison, Type::Steel);
		case MoveName::Shell_Side_Arm:
			return status_effect<StatusName::poison>(0.2, original_user, original_other, original_weather, Type::Poison, Type::Steel);
		case MoveName::Gunk_Shot:
		case MoveName::Poison_Jab:
		case MoveName::Sludge_Bomb:
			return status_effect<StatusName::poison>(0.3, original_user, original_other, original_weather, Type::Poison, Type::Steel);
		case MoveName::Smog:
			return status_effect<StatusName::poison>(0.4, original_user, original_other, original_weather, Type::Poison, Type::Steel);
		case MoveName::Poison_Gas:
		case MoveName::Poison_Powder:
			return generation <= Generation::five ?
				status_effect<StatusName::poison>(1.0, original_user, original_other, original_weather, Type::Poison, Type::Steel) :
				status_effect<StatusName::poison>(1.0, original_user, original_other, original_weather, Type::Grass, Type::Poison, Type::Steel);
		case MoveName::Poison_Sting:
			return status_effect<StatusName::poison>(generation == Generation::one ? 0.2 : 0.3, original_user, original_other, original_weather, Type::Poison, Type::Steel);
		case MoveName::Sludge:
			return status_effect<StatusName::poison>(generation == Generation::one ? 0.4 : 0.3, original_user, original_other, original_weather, Type::Poison, Type::Steel);
		case MoveName::Twineedle:
			return generation == Generation::two ?
				status_effect<StatusName::poison>(0.2, original_user, original_other, original_weather, Type::Poison) :
				status_effect<StatusName::poison>(0.2, original_user, original_other, original_weather, Type::Poison, Type::Steel);

		case MoveName::Poison_Fang: {
			constexpr auto probability = generation <= Generation::five ? 0.3 : 0.5;
			return status_effect<StatusName::toxic>(probability, original_user, original_other, original_weather, Type::Poison, Type::Steel);
		}
		case MoveName::Toxic:
			return status_effect<StatusName::toxic>(1.0, original_user, original_other, original_weather, Type::Poison, Type::Steel);

		case MoveName::Dark_Void:
		case MoveName::Grass_Whistle:
		case MoveName::Hypnosis:
		case MoveName::Lovely_Kiss:
		case MoveName::Sing:
			return status_effect<StatusName::sleep>(1.0, original_user, original_other, original_weather);
		case MoveName::Sleep_Powder:
		case MoveName::Spore:
			return generation <= Generation::five ?
				status_effect<StatusName::sleep>(1.0, original_user, original_other, original_weather) :
				status_effect<StatusName::sleep>(1.0, original_user, original_other, original_weather, Type::Grass);
		case MoveName::Yawn:
			return guaranteed_effect<UserTeam>([](auto &, auto & other, auto &, auto) {
				other.pokemon().hit_with_yawn();
			});

		case MoveName::Flare_Blitz:
			return recoil_status<StatusName::burn>(original_user, original_other, original_weather, Type::Fire);
		case MoveName::Volt_Tackle:
			return generation <= Generation::three ?
				guaranteed_effect<UserTeam>(RecoilEffect(3_bi)) :
				recoil_status<StatusName::paralysis>(original_user, original_other, original_weather, Type::Ground);

		case MoveName::Fire_Fang:
			return fang_effects<StatusName::burn>(original_user, original_other, original_weather, Type::Fire);
		case MoveName::Ice_Fang:
			return fang_effects<StatusName::freeze>(original_user, original_other, original_weather, Type::Ice);
		case MoveName::Thunder_Fang:
			return fang_effects<StatusName::paralysis>(original_user, original_other, original_weather, Type::Ground);

		case MoveName::Tri_Attack:
			return tri_attack_effect(original_user, original_other, original_weather);

		case MoveName::Signal_Beam:
			return confusion_effect(0.1, original_other.pokemon());
		case MoveName::Confusion:
		case MoveName::Psybeam:
			return confusion_effect(0.1, original_other.pokemon(), Type::Dark);
		case MoveName::Rock_Climb:
		case MoveName::Strange_Steam:
		case MoveName::Water_Pulse:
			return confusion_effect(0.2, original_other.pokemon());
		case MoveName::Confuse_Ray:
		case MoveName::Supersonic:
		case MoveName::Sweet_Kiss:
		case MoveName::Teeter_Dance:
			return confusion_effect(1.0, original_other.pokemon());
		case MoveName::Dynamic_Punch:
			if constexpr (generation <= Generation::two) {
				return confusion_effect(0.996, original_other.pokemon(), Type::Ghost);
			} else {
				return confusion_effect(1.0, original_other.pokemon(), Type::Ghost);
			}
		case MoveName::Dizzy_Punch:
			return generation == Generation::one ?
				no_effect<UserTeam> :
				confusion_effect(0.2, original_other.pokemon(), Type::Ghost);
		case MoveName::Chatter: {
			constexpr auto probability = generation <= Generation::five ? 0.31 : 1.0;
			return can_confuse_with_chatter(original_user.species()) ?
				confusion_effect(probability, original_other.pokemon(), Type::Ghost) :
				no_effect<UserTeam>;
		}

		case MoveName::Rest:
			return guaranteed_effect<UserTeam>([](auto & user, auto & other, auto & weather, auto) {
				user.pokemon().rest(weather, other.pokemon().last_used_move().is_uproaring());
			});

		case MoveName::Smelling_Salts:
			return boosts_smellingsalt(original_other.pokemon().status()) ?
				guaranteed_effect<UserTeam>([](auto &, auto & other, auto &, auto) {
					other.pokemon().status() = Status{};
				}) :
				no_effect<UserTeam>;

		case MoveName::Flatter:
			return confusing_stat_boost<UserTeam, BoostableStat::spa, 1>;
		case MoveName::Swagger:
			return confusing_stat_boost<UserTeam, BoostableStat::atk, 2>;

		case MoveName::Bounce:
			// TODO: Paralysis
		case MoveName::Dig:
		case MoveName::Dive:
		case MoveName::Fly:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto & weather, auto) {
				user.pokemon().use_vanish_move(weather);
			});
		case MoveName::Shadow_Force:
			return guaranteed_effect<UserTeam>([](auto & user, auto & other, auto & weather, auto) {
				auto const hit = user.pokemon().use_vanish_move(weather);
				if (hit) {
					other.pokemon().break_protect();
				}
			});

		case MoveName::Razor_Wind:
		case MoveName::Solar_Beam:
			return charge_up_move(
				move,
				original_user,
				original_other.pokemon(),
				original_weather,
				no_effect<UserTeam>
			);
		case MoveName::Meteor_Beam:
			return charge_up_move(
				move,
				original_user,
				original_other.pokemon(),
				original_weather,
				guaranteed_effect<UserTeam>(boost_user_stat<BoostableStat::spa, 1>)
			);
		case MoveName::Skull_Bash:
			return charge_up_move(
				move,
				original_user,
				original_other.pokemon(),
				original_weather,
				generation >= Generation::two ? guaranteed_effect<UserTeam>(boost_user_stat<BoostableStat::def, 1>) : no_effect<UserTeam>
			);
		case MoveName::Sky_Attack:
			return charge_up_move(
				move,
				original_user,
				original_other.pokemon(),
				original_weather,
				SideEffects<UserTeam>({
					SideEffect<UserTeam>{0.7, no_effect_function},
					SideEffect<UserTeam>{0.3, flinch}
				})
			);

		case MoveName::Gravity:
			return guaranteed_effect<UserTeam>([](auto &, auto &, auto & weather, auto) {
				weather.activate_gravity();
			});
		case MoveName::Hail:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto & weather, auto) {
				weather.activate_hail_from_move(extends_hail(user.pokemon().item(weather)));
			});
		case MoveName::Rain_Dance:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto & weather, auto) {
				weather.activate_rain_from_move(extends_rain(user.pokemon().item(weather)));
			});
		case MoveName::Sandstorm:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto & weather, auto) {
				weather.activate_sand_from_move(extends_sand(user.pokemon().item(weather)));
			});
		case MoveName::Sunny_Day:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto & weather, auto) {
				weather.activate_sun_from_move(extends_sun(user.pokemon().item(weather)));
			});
		case MoveName::Trick_Room:
			return guaranteed_effect<UserTeam>([](auto &, auto &, auto & weather, auto) {
				weather.activate_trick_room();
			});

		case MoveName::Light_Screen:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto & weather, auto) {
				user.activate_light_screen(weather);
			});
		case MoveName::Lucky_Chant:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				user.activate_lucky_chant();
			});
		case MoveName::Mist:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				user.activate_mist();
			});
		case MoveName::Reflect:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto & weather, auto) {
				user.activate_reflect(weather);
			});
		case MoveName::Safeguard:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				user.activate_safeguard();
			});
		case MoveName::Tailwind:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				user.activate_tailwind();
			});

		case MoveName::Heal_Order:
		case MoveName::Milk_Drink:
		case MoveName::Recover:
		case MoveName::Roost:
		case MoveName::Slack_Off:
		case MoveName::Soft_Boiled:
			return generation == Generation::one and healing_move_fails_in_generation_1(original_user.hp()) ?
				no_effect<UserTeam> :
				guaranteed_effect<UserTeam>(recover_half);
		case MoveName::Moonlight:
		case MoveName::Morning_Sun:
		case MoveName::Synthesis:
			return guaranteed_effect<UserTeam>([](auto & user, auto & other, auto & weather, auto) {
				auto const amount = [&]() {
					using Numerator = bounded::integer<1, 2>;
					using Denominator = bounded::integer<2, 4>;
					using Result = rational<Numerator, Denominator>;

					auto const blocks_weather = weather_is_blocked_by_ability(user.pokemon().ability(), other.pokemon().ability());
					if (weather.sun(blocks_weather)) {
						return Result(2_bi, 3_bi);
					} else if (weather.hail(blocks_weather) or weather.rain(blocks_weather) or weather.sand(blocks_weather)) {
						return Result(1_bi, 4_bi);
					} else {
						return Result(1_bi, 2_bi);
					}
				}();
				heal(user.pokemon(), weather, amount);
			});
		case MoveName::Life_Dew:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto & weather, auto) {
				heal(user.pokemon(), weather, rational(1_bi, 4_bi));
			});
		case MoveName::Wish:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				user.activate_wish();
			});

		case MoveName::Spikes:
			return guaranteed_effect<UserTeam>([](auto &, auto & other, auto &, auto) {
				other.add_spikes();
			});
		case MoveName::Stealth_Rock:
			return guaranteed_effect<UserTeam>([](auto &, auto & other, auto &, auto) {
				other.add_stealth_rock();
			});
		case MoveName::Toxic_Spikes:
			return guaranteed_effect<UserTeam>([](auto &, auto & other, auto &, auto) {
				other.add_toxic_spikes();
			});

		case MoveName::Magnitude:
			return no_effect<UserTeam>;
#if 0
			return Probabilities{
				{10_bi, 0.05}, // Magnitude 4
				{30_bi, 0.1}, // Magnitude 5
				{50_bi, 0.2}, // Magnitude 6
				{70_bi, 0.3}, // Magnitude 7
				{90_bi, 0.2}, // Magnitude 8
				{110_bi, 0.1}, // Magnitude 9
				{150_bi, 0.05} // Magnitude 10
			};
#endif
		case MoveName::Present:
			// TODO: Figure out how to represent variable damage
			// heal 80: 0.25
			// 40 power: 0.25
			// 80 power: 0.25
			// 120 power: 0.25
			return no_effect<UserTeam>;
		case MoveName::Psywave:
			return no_effect<UserTeam>;
#if 0
			auto compute = [](auto const min_value, auto const max_value) {
				auto const range = containers::inclusive_integer_range(min_value, max_value);
				Probabilities probabilities;
				for (auto const n : range) {
					containers::emplace_back(probabilities, Variable{n}, 1.0 / static_cast<double>(containers::size(range)));
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
#endif

		case MoveName::Spit_Up:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				user.pokemon().release_stockpile();
			});
		case MoveName::Stockpile:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				user.pokemon().increment_stockpile();
			});
		case MoveName::Swallow:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto & weather, auto) {
				auto const pokemon = user.pokemon();
				auto const stockpiles = pokemon.release_stockpile();
				if (stockpiles == 0_bi) {
					return;
				}
				heal(pokemon, weather, swallow_healing(stockpiles));
			});

		case MoveName::Mud_Sport:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				user.pokemon().activate_mud_sport();
			});
		case MoveName::Water_Sport:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				user.pokemon().activate_water_sport();
			});

		case MoveName::Aqua_Ring:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				user.pokemon().activate_aqua_ring();
			});
		case MoveName::Attract:
			return multiplier(original_user.gender(), original_other.pokemon().gender()) == -1_bi ?
				guaranteed_effect<UserTeam>([](auto & user, auto & other, auto & weather, auto) {
					other.pokemon().attract(user.pokemon(), weather);
				}) :
				no_effect<UserTeam>;
		case MoveName::Bide:
			return guaranteed_effect<UserTeam>([](auto & user, auto & other, auto & weather, auto) {
				user.pokemon().use_bide(other.pokemon(), weather);
			});
		case MoveName::Bind:
		case MoveName::Clamp:
		case MoveName::Sand_Tomb:
		case MoveName::Snap_Trap:
		case MoveName::Thunder_Cage:
		case MoveName::Whirlpool:
		case MoveName::Wrap:
			return guaranteed_effect<UserTeam>([](auto &, auto & other, auto &, auto) {
				other.pokemon().partially_trap();
			});
		case MoveName::Fire_Spin:
		case MoveName::Magma_Storm:
			return guaranteed_effect<UserTeam>([](auto &, auto & other, auto &, auto) {
				auto other_pokemon = other.pokemon();
				if (other_pokemon.status().name() == StatusName::freeze) {
					other_pokemon.clear_status();
				}
				other_pokemon.partially_trap();
			});
		case MoveName::Block:
		case MoveName::Mean_Look:
		case MoveName::Spider_Web:
			return guaranteed_effect<UserTeam>([](auto &, auto & other, auto &, auto) {
				other.pokemon().fully_trap();
			});
		case MoveName::Jaw_Lock:
			return original_user.fully_trapped() or original_other.pokemon().fully_trapped() ?
				no_effect<UserTeam> :
				guaranteed_effect<UserTeam>([](auto & user, auto & other, auto &, auto) {
					user.pokemon().fully_trap();
					other.pokemon().fully_trap();
				});
		case MoveName::No_Retreat:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				user.pokemon().fully_trap();
				boost_regular(user.pokemon().stages(), 1_bi);
			});
		case MoveName::Octolock:
			return guaranteed_effect<UserTeam>([](auto &, auto & other, auto &, auto) {
				other.pokemon().fully_trap();
			});
		case MoveName::Blast_Burn:
		case MoveName::Eternabeam:
		case MoveName::Frenzy_Plant:
		case MoveName::Giga_Impact:
		case MoveName::Hydro_Cannon:
		case MoveName::Hyper_Beam:
		case MoveName::Meteor_Assault:
		case MoveName::Roar_of_Time:
		case MoveName::Rock_Wrecker:
			return guaranteed_effect<UserTeam>([](auto & user, auto & other, auto &, auto) {
				if (generation >= Generation::two or other.pokemon().hp().current() != 0_bi) {
					user.pokemon().use_recharge_move();
				}
			});
		case MoveName::Bug_Bite:
		case MoveName::Pluck:
			return guaranteed_effect<UserTeam>([](auto & user, auto & other, auto &, auto) {
				static_cast<void>(user);
				auto const other_pokemon = other.pokemon();
				if (item_can_be_lost(other_pokemon.as_const())) {
				}
			});
		case MoveName::Camouflage:
			return no_effect<UserTeam>;
		case MoveName::Conversion:
			return no_effect<UserTeam>;
		case MoveName::Conversion_2:
			return no_effect<UserTeam>;
		case MoveName::Covet:
		case MoveName::Thief:
			return guaranteed_effect<UserTeam>([](auto & user, auto & other, auto &, auto) {
				if (item_can_be_lost(other.pokemon().as_const())) {
					user.pokemon().steal_item(other.pokemon());
				}
			});
		case MoveName::Curse:
			return guaranteed_effect<UserTeam>([](auto & user, auto & other, auto & weather, auto) {
				auto const user_pokemon = user.pokemon();
				if (is_type(user_pokemon.as_const(), Type::Ghost)) {
					auto const other_pokemon = other.pokemon();
					if (!other_pokemon.is_cursed()) {
						user_pokemon.indirect_damage(weather, user_pokemon.hp().max() / 2_bi);
						other_pokemon.curse();
					}
				} else {
					auto & stages = user_pokemon.stages();
					if constexpr (generation == Generation::two) {
						constexpr auto max = numeric_traits::max_value<Stage::value_type>;
						if (stages[BoostableStat::atk] == max and stages[BoostableStat::def] == max) {
							return;
						}
					}
					stages[BoostableStat::atk] += 1_bi;
					stages[BoostableStat::def] += 1_bi;
					stages[BoostableStat::spe] -= 1_bi;
				}
			});
		case MoveName::Defog:
			return guaranteed_effect<UserTeam>([](auto &, auto & other, auto & weather, auto) {
				weather.deactivate_fog();
				other.pokemon().stages()[BoostableStat::eva] -= 1_bi;
			});
		case MoveName::Detect:
		case MoveName::Protect:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				user.pokemon().protect();
			});
		case MoveName::Disable:
			return guaranteed_effect<UserTeam>([](auto &, auto & other, auto & weather, auto) {
				auto pokemon = other.pokemon();
				pokemon.disable(pokemon.last_used_move().name(), weather);
			});
		case MoveName::Doom_Desire:
		case MoveName::Future_Sight:
			return no_effect<UserTeam>;
		case MoveName::Embargo:
			return guaranteed_effect<UserTeam>([](auto &, auto & other, auto &, auto) {
				other.pokemon().activate_embargo();
			});
		case MoveName::Encore:
			return guaranteed_effect<UserTeam>([](auto &, auto & other, auto & weather, auto) {
				other.pokemon().activate_encore(weather);
			});
		case MoveName::Explosion:
		case MoveName::Misty_Explosion:
		case MoveName::Self_Destruct:
			return (generation == Generation::one and original_other.pokemon().substitute()) ?
				guaranteed_effect<UserTeam>([](auto & user, auto & other, auto &, auto) {
					if (other.pokemon().substitute()) {
						faint(user.pokemon());
					}
				}) :
				guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
					faint(user.pokemon());
				});
		case MoveName::Feint:
		case MoveName::Hyperspace_Fury:
		case MoveName::Hyperspace_Hole:
		case MoveName::Phantom_Force:
			return guaranteed_effect<UserTeam>([](auto &, auto & other, auto &, auto) {
				other.pokemon().break_protect();
			});
		case MoveName::Fling:
			return guaranteed_effect<UserTeam>(fling_effects);
		case MoveName::Focus_Energy:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				user.pokemon().focus_energy();
			});
		case MoveName::Follow_Me:
			return no_effect<UserTeam>;
		case MoveName::Foresight:
		case MoveName::Odor_Sleuth:
			return guaranteed_effect<UserTeam>([](auto &, auto & other, auto &, auto) {
				other.pokemon().identify();
			});
		case MoveName::Gastro_Acid:
			return no_effect<UserTeam>;
		case MoveName::Grudge:
			return no_effect<UserTeam>;
		case MoveName::Guard_Swap:
			return guaranteed_effect<UserTeam>([](auto & user, auto & other, auto &, auto) {
				swap_defensive(user.pokemon().stages(), other.pokemon().stages());
			});
		case MoveName::Heal_Block:
			return guaranteed_effect<UserTeam>([](auto &, auto & other, auto &, auto) {
				other.pokemon().activate_heal_block();
			});
		case MoveName::Healing_Wish:
			return no_effect<UserTeam>;
		case MoveName::Heart_Swap:
			return guaranteed_effect<UserTeam>([](auto & user, auto & other, auto &, auto) {
				std::swap(user.pokemon().stages(), other.pokemon().stages());
			});
		case MoveName::High_Jump_Kick:
		case MoveName::Jump_Kick:
			return no_effect<UserTeam>;
		case MoveName::Ice_Ball:
		case MoveName::Rollout:
			return no_effect<UserTeam>;
		case MoveName::Imprison:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				user.pokemon().use_imprison();
			});
		case MoveName::Incinerate:
			return guaranteed_effect<UserTeam>([](auto &, auto & other, auto & weather, auto) {
				auto other_pokemon = other.pokemon();
				if (item_can_be_incinerated(other_pokemon.as_const(), weather)) {
					other_pokemon.destroy_item();
				}
			});
		case MoveName::Ingrain:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				user.pokemon().ingrain();
			});
		case MoveName::Corrosive_Gas:
		case MoveName::Knock_Off:
			return guaranteed_effect<UserTeam>([](auto &, auto & other, auto &, auto) {
				auto const other_pokemon = other.pokemon();
				if (item_can_be_lost(other_pokemon.as_const())) {
					other_pokemon.remove_item();
				}
			});
		case MoveName::Leech_Seed:
			return guaranteed_effect<UserTeam>([](auto &, auto & other, auto &, auto) {
				other.pokemon().hit_with_leech_seed();
			});
		case MoveName::Lunar_Dance:
			return no_effect<UserTeam>;
		case MoveName::Magic_Coat:
			return no_effect<UserTeam>;
		case MoveName::Magic_Room:
			return guaranteed_effect<UserTeam>([](auto &, auto &, auto & weather, auto) {
				weather.activate_magic_room();
			});
		case MoveName::Magnet_Rise:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				user.pokemon().activate_magnet_rise();
			});
		case MoveName::Me_First:
			return no_effect<UserTeam>;
		case MoveName::Memento:
			return guaranteed_effect<UserTeam>([](auto & user, auto & other, auto &, auto) {
				boost_offensive(other.pokemon().stages(), -2_bi);
				faint(user.pokemon());
			});
		case MoveName::Mimic:
			return no_effect<UserTeam>;
		case MoveName::Minimize:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				user.pokemon().minimize();
			});
		case MoveName::Miracle_Eye:
			return no_effect<UserTeam>;
		case MoveName::Nightmare:
			return guaranteed_effect<UserTeam>([](auto &, auto & other, auto &, auto) {
				other.pokemon().try_to_give_nightmares();
			});
		case MoveName::Outrage:
		case MoveName::Petal_Dance:
		case MoveName::Thrash:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				user.pokemon().activate_rampage();
			});
		case MoveName::Pain_Split:
			return guaranteed_effect<UserTeam>(equalize_hp);
		case MoveName::Perish_Song:
			return guaranteed_effect<UserTeam>([](auto & user, auto & other, auto &, auto) {
				auto const user_pokemon = user.pokemon();
				if (generation >= Generation::eight or !blocks_sound_moves(user_pokemon.ability())) {
					user_pokemon.activate_perish_song();
				}
				auto const other_pokemon = other.pokemon();
				if (!blocks_sound_moves(other_pokemon.ability())) {
					other_pokemon.activate_perish_song();
				}
			});
		case MoveName::Power_Trick:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				user.pokemon().activate_power_trick();
			});
		case MoveName::Rage:
			return no_effect<UserTeam>;
		case MoveName::Rapid_Spin:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				user.clear_field();
				if constexpr (generation >= Generation::eight) {
					user.pokemon().stages()[BoostableStat::spe] += 1_bi;
				}
			});
		case MoveName::Recycle:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				user.pokemon().recycle_item();
			});
		case MoveName::Roar:
		case MoveName::Whirlwind:
			switch (generation) {
				case Generation::one:
					return no_effect<UserTeam>;
				case Generation::two:
					return original_other.pokemon().last_used_move().moved_this_turn() ? phaze_effect(original_other) : no_effect<UserTeam>;
				default:
					return phaze_effect(original_other);
			}
		case MoveName::Role_Play:
			return no_effect<UserTeam>;
		case MoveName::Sketch:
			return no_effect<UserTeam>;
		case MoveName::Skill_Swap:
			return no_effect<UserTeam>;
		case MoveName::Snatch:
			return no_effect<UserTeam>;
		case MoveName::Spite: {
			// TODO: Spite fails if the last used move has no PP
			auto const regular_move = containers::maybe_find(original_user.regular_moves(), original_user.last_used_move().name());
			if (!regular_move) {
				return no_effect<UserTeam>;
			}
			auto const remaining_pp = regular_move->pp().remaining();
			if (!remaining_pp or *remaining_pp == 0_bi) {
				return no_effect<UserTeam>;
			}
			switch (generation) {
				case Generation::two:
					return random_spite<UserTeam>;
				case Generation::three:
					if (*remaining_pp == 1_bi) {
						return no_effect<UserTeam>;
					}
					return random_spite<UserTeam>;
				default:
					return guaranteed_effect<UserTeam>(reduce_pp<4_bi>);
			}
		}
		case MoveName::Eerie_Spell:
			return guaranteed_effect<UserTeam>(reduce_pp<3_bi>);
		case MoveName::Struggle:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto & weather, auto const damage) {
				switch (generation) {
					case Generation::one:
						change_hp(user.pokemon(), weather, -bounded::max(damage / 2_bi, 1_bi));
						break;
					case Generation::two:
					case Generation::three:
						change_hp(user.pokemon(), weather, -bounded::max(damage / 4_bi, 1_bi));
						break;
					case Generation::four:
					case Generation::five:
					case Generation::six:
					case Generation::seven:
					case Generation::eight:
						change_hp(user.pokemon(), weather, -bounded::max(user.pokemon().hp().max() / 4_bi, 1_bi));
				}
			});
		case MoveName::Substitute:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto & weather, auto) {
				user.pokemon().use_substitute(weather);
			});
		case MoveName::Switch0:
			return switch_effect<UserTeam, 0>;
		case MoveName::Switch1:
			return switch_effect<UserTeam, 1>;
		case MoveName::Switch2:
			return switch_effect<UserTeam, 2>;
		case MoveName::Switch3:
			return switch_effect<UserTeam, 3>;
		case MoveName::Switch4:
			return switch_effect<UserTeam, 4>;
		case MoveName::Switch5:
			return switch_effect<UserTeam, 5>;
		case MoveName::Switcheroo:
		case MoveName::Trick:
			return guaranteed_effect<UserTeam>([](auto & user, auto & other, auto &, auto) {
				auto const other_pokemon = other.pokemon();
				if (item_can_be_lost(other_pokemon.as_const())) {
					swap_items(user.pokemon(), other.pokemon());
				}
			});
		case MoveName::Taunt:
			return guaranteed_effect<UserTeam>([](auto &, auto & other, auto & weather, auto) {
				other.pokemon().taunt(weather);
			});
		case MoveName::Torment:
			return guaranteed_effect<UserTeam>([](auto &, auto & other, auto & weather, auto) {
				other.pokemon().torment(weather);
			});
		case MoveName::Transform:
			return no_effect<UserTeam>;
		case MoveName::Uproar:
			return guaranteed_effect<UserTeam>([](auto & user, auto &, auto &, auto) {
				user.pokemon().use_uproar();
			});
		case MoveName::Worry_Seed:
			return no_effect<UserTeam>;

		case MoveName::Acid_Spray:
		case MoveName::Acrobatics:
		case MoveName::Aerial_Ace:
		case MoveName::Aeroblast:
		case MoveName::After_You:
		case MoveName::Air_Cutter:
		case MoveName::Ally_Switch:
		case MoveName::Aqua_Jet:
		case MoveName::Aqua_Tail:
		case MoveName::Arm_Thrust:
		case MoveName::Assist:
		case MoveName::Assurance:
		case MoveName::Astral_Barrage:
		case MoveName::Attack_Order:
		case MoveName::Aura_Sphere:
		case MoveName::Autotomize:
		case MoveName::Avalanche:
		case MoveName::Barrage:
		case MoveName::Baton_Pass:
		case MoveName::Beat_Up:
		case MoveName::Behemoth_Bash:
		case MoveName::Behemoth_Blade:
		case MoveName::Bestow:
		case MoveName::Body_Press:
		case MoveName::Bolt_Beak:
		case MoveName::Bone_Rush:
		case MoveName::Bonemerang:
		case MoveName::Branch_Poke:
		case MoveName::Brick_Break:
		case MoveName::Brine:
		case MoveName::Bulldoze:
		case MoveName::Bullet_Punch:
		case MoveName::Bullet_Seed:
		case MoveName::Burning_Jealousy:
		case MoveName::Chip_Away:
		case MoveName::Circle_Throw:
		case MoveName::Clear_Smog:
		case MoveName::Coaching:
		case MoveName::Coil:
		case MoveName::Comet_Punch:
		case MoveName::Copycat:
		case MoveName::Cotton_Guard:
		case MoveName::Counter:
		case MoveName::Court_Change:
		case MoveName::Crabhammer:
		case MoveName::Cross_Chop:
		case MoveName::Crush_Grip:
		case MoveName::Cut:
		case MoveName::Destiny_Bond:
		case MoveName::Double_Hit:
		case MoveName::Double_Kick:
		case MoveName::Double_Slap:
		case MoveName::Dragon_Claw:
		case MoveName::Dragon_Darts:
		case MoveName::Dragon_Energy:
		case MoveName::Dragon_Pulse:
		case MoveName::Dragon_Rage:
		case MoveName::Dragon_Tail:
		case MoveName::Drill_Peck:
		case MoveName::Drill_Run:
		case MoveName::Dual_Chop:
		case MoveName::Dual_Wingbeat:
		case MoveName::Dynamax_Cannon:
		case MoveName::Earthquake:
		case MoveName::Echoed_Voice:
		case MoveName::Egg_Bomb:
		case MoveName::Electro_Ball:
		case MoveName::Electroweb:
		case MoveName::Endeavor:
		case MoveName::Endure:
		case MoveName::Entrainment:
		case MoveName::Eruption:
		case MoveName::Expanding_Force:
		case MoveName::Extreme_Speed:
		case MoveName::Facade:
		case MoveName::False_Surrender:
		case MoveName::False_Swipe:
		case MoveName::Feint_Attack:
		case MoveName::Fiery_Dance:
		case MoveName::Final_Gambit:
		case MoveName::Fire_Pledge:
		case MoveName::Fishious_Rend:
		case MoveName::Fissure:
		case MoveName::Flail:
		case MoveName::Flame_Burst:
		case MoveName::Flame_Charge:
		case MoveName::Flip_Turn:
		case MoveName::Focus_Punch:
		case MoveName::Foul_Play:
		case MoveName::Frost_Breath:
		case MoveName::Frustration:
		case MoveName::Fury_Attack:
		case MoveName::Fury_Cutter:
		case MoveName::Fury_Swipes:
		case MoveName::Fusion_Bolt:
		case MoveName::Fusion_Flare:
		case MoveName::Gear_Grind:
		case MoveName::Glacial_Lance:
		case MoveName::Glaciate:
		case MoveName::Grass_Knot:
		case MoveName::Grass_Pledge:
		case MoveName::Grassy_Glide:
		case MoveName::Guard_Split:
		case MoveName::Guillotine:
		case MoveName::Gust:
		case MoveName::Gyro_Ball:
		case MoveName::Head_Charge:
		case MoveName::Heal_Pulse:
		case MoveName::Heat_Crash:
		case MoveName::Heavy_Slam:
		case MoveName::Helping_Hand:
		case MoveName::Hex:
		case MoveName::Hidden_Power:
		case MoveName::Hit_Self:
		case MoveName::Hone_Claws:
		case MoveName::Horn_Attack:
		case MoveName::Horn_Drill:
		case MoveName::Horn_Leech:
		case MoveName::Hydro_Pump:
		case MoveName::Hyper_Voice:
		case MoveName::Ice_Shard:
		case MoveName::Icicle_Spear:
		case MoveName::Judgment:
		case MoveName::Karate_Chop:
		case MoveName::Lash_Out:
		case MoveName::Last_Resort:
		case MoveName::Leaf_Blade:
		case MoveName::Lock_On:
		case MoveName::Mach_Punch:
		case MoveName::Magic_Powder:
		case MoveName::Magical_Leaf:
		case MoveName::Magnet_Bomb:
		case MoveName::Max_Airstream:
		case MoveName::Max_Darkness:
		case MoveName::Max_Flare:
		case MoveName::Max_Flutterby:
		case MoveName::Max_Geyser:
		case MoveName::Max_Guard:
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
		case MoveName::Mega_Kick:
		case MoveName::Mega_Punch:
		case MoveName::Megahorn:
		case MoveName::Metal_Burst:
		case MoveName::Metronome:
		case MoveName::Mind_Reader:
		case MoveName::Mirror_Coat:
		case MoveName::Mirror_Move:
		case MoveName::Natural_Gift:
		case MoveName::Nature_Power:
		case MoveName::Night_Shade:
		case MoveName::Night_Slash:
		case MoveName::Obstruct:
		case MoveName::Overdrive:
		case MoveName::Pass:
		case MoveName::Pay_Day:
		case MoveName::Payback:
		case MoveName::Peck:
		case MoveName::Pin_Missile:
		case MoveName::Poltergeist:
		case MoveName::Pound:
		case MoveName::Power_Gem:
		case MoveName::Power_Split:
		case MoveName::Power_Whip:
		case MoveName::Psycho_Cut:
		case MoveName::Psyshock:
		case MoveName::Psystrike:
		case MoveName::Punishment:
		case MoveName::Pursuit:
		case MoveName::Quash:
		case MoveName::Quick_Attack:
		case MoveName::Quick_Guard:
		case MoveName::Rage_Powder:
		case MoveName::Razor_Leaf:
		case MoveName::Reflect_Type:
		case MoveName::Retaliate:
		case MoveName::Return:
		case MoveName::Revenge:
		case MoveName::Reversal:
		case MoveName::Rising_Voltage:
		case MoveName::Rock_Blast:
		case MoveName::Rock_Throw:
		case MoveName::Round:
		case MoveName::Sacred_Sword:
		case MoveName::Scratch:
		case MoveName::Secret_Sword:
		case MoveName::Seed_Bomb:
		case MoveName::Seismic_Toss:
		case MoveName::Shadow_Claw:
		case MoveName::Shadow_Punch:
		case MoveName::Shadow_Sneak:
		case MoveName::Sheer_Cold:
		case MoveName::Shock_Wave:
		case MoveName::Simple_Beam:
		case MoveName::Sky_Drop:
		case MoveName::Sky_Uppercut:
		case MoveName::Slam:
		case MoveName::Slash:
		case MoveName::Sleep_Talk:
		case MoveName::Smack_Down:
		case MoveName::Snipe_Shot:
		case MoveName::Soak:
		case MoveName::Sonic_Boom:
		case MoveName::Spacial_Rend:
		case MoveName::Spike_Cannon:
		case MoveName::Splash:
		case MoveName::Steel_Beam:
		case MoveName::Steel_Roller:
		case MoveName::Stone_Edge:
		case MoveName::Stored_Power:
		case MoveName::Storm_Throw:
		case MoveName::Strength:
		case MoveName::Stuff_Cheeks:
		case MoveName::Sucker_Punch:
		case MoveName::Super_Fang:
		case MoveName::Surf:
		case MoveName::Surging_Strikes:
		case MoveName::Swift:
		case MoveName::Synchronoise:
		case MoveName::Tackle:
		case MoveName::Tail_Slap:
		case MoveName::Tar_Shot:
		case MoveName::Teatime:
		case MoveName::Techno_Blast:
		case MoveName::Telekinesis:
		case MoveName::Teleport:
		case MoveName::Terrain_Pulse:
		case MoveName::Triple_Axel:
		case MoveName::Triple_Kick:
		case MoveName::Trump_Card:
		case MoveName::U_turn:
		case MoveName::Vacuum_Wave:
		case MoveName::Venoshock:
		case MoveName::Vine_Whip:
		case MoveName::Vise_Grip:
		case MoveName::Vital_Throw:
		case MoveName::Volt_Switch:
		case MoveName::Water_Gun:
		case MoveName::Water_Pledge:
		case MoveName::Water_Spout:
		case MoveName::Weather_Ball:
		case MoveName::Wicked_Blow:
		case MoveName::Wide_Guard:
		case MoveName::Wild_Charge:
		case MoveName::Wing_Attack:
		case MoveName::Wonder_Room:
		case MoveName::Work_Up:
		case MoveName::Wring_Out:
		case MoveName::X_Scissor:
			return no_effect<UserTeam>;

		case MoveName::Blue_Flare:
		case MoveName::Bolt_Strike:
		case MoveName::Relic_Song:
		case MoveName::Freeze_Shock:
		case MoveName::Heart_Stamp:
		case MoveName::Hurricane:
		case MoveName::Ice_Burn:
		case MoveName::Flying_Press:
		case MoveName::Mat_Block:
		case MoveName::Belch:
		case MoveName::Rototiller:
		case MoveName::Sticky_Web:
		case MoveName::Fell_Stinger:
		case MoveName::Trick_or_Treat:
		case MoveName::Noble_Roar:
		case MoveName::Ion_Deluge:
		case MoveName::Parabolic_Charge:
		case MoveName::Forests_Curse:
		case MoveName::Petal_Blizzard:
		case MoveName::Freeze_Dry:
		case MoveName::Disarming_Voice:
		case MoveName::Parting_Shot:
		case MoveName::Topsy_Turvy:
		case MoveName::Draining_Kiss:
		case MoveName::Crafty_Shield:
		case MoveName::Flower_Shield:
		case MoveName::Grassy_Terrain:
		case MoveName::Misty_Terrain:
		case MoveName::Electrify:
		case MoveName::Play_Rough:
		case MoveName::Fairy_Wind:
		case MoveName::Moonblast:
		case MoveName::Boomburst:
		case MoveName::Fairy_Lock:
		case MoveName::Kings_Shield:
		case MoveName::Play_Nice:
		case MoveName::Confide:
		case MoveName::Diamond_Storm:
		case MoveName::Steam_Eruption:
		case MoveName::Water_Shuriken:
		case MoveName::Mystical_Fire:
		case MoveName::Spiky_Shield:
		case MoveName::Aromatic_Mist:
		case MoveName::Eerie_Impulse:
		case MoveName::Venom_Drench:
		case MoveName::Powder:
		case MoveName::Geomancy:
		case MoveName::Magnetic_Flux:
		case MoveName::Happy_Hour:
		case MoveName::Electric_Terrain:
		case MoveName::Dazzling_Gleam:
		case MoveName::Celebrate:
		case MoveName::Hold_Hands:
		case MoveName::Baby_Doll_Eyes:
		case MoveName::Nuzzle:
		case MoveName::Hold_Back:
		case MoveName::Infestation:
		case MoveName::Power_Up_Punch:
		case MoveName::Oblivion_Wing:
		case MoveName::Thousand_Arrows:
		case MoveName::Thousand_Waves:
		case MoveName::Lands_Wrath:
		case MoveName::Light_of_Ruin:
		case MoveName::Origin_Pulse:
		case MoveName::Precipice_Blades:
		case MoveName::Dragon_Ascent:
		case MoveName::Breakneck_Blitz:
		case MoveName::All_Out_Pummeling:
		case MoveName::Supersonic_Skystrike:
		case MoveName::Acid_Downpour:
		case MoveName::Tectonic_Rage:
		case MoveName::Continental_Crush:
		case MoveName::Savage_Spin_Out:
		case MoveName::Never_Ending_Nightmare:
		case MoveName::Corkscrew_Crash:
		case MoveName::Inferno_Overdrive:
		case MoveName::Hydro_Vortex:
		case MoveName::Bloom_Doom:
		case MoveName::Gigavolt_Havoc:
		case MoveName::Shattered_Psyche:
		case MoveName::Subzero_Slammer:
		case MoveName::Devastating_Drake:
		case MoveName::Black_Hole_Eclipse:
		case MoveName::Twinkle_Tackle:
		case MoveName::Catastropika:
		case MoveName::Shore_Up:
		case MoveName::First_Impression:
		case MoveName::Baneful_Bunker:
		case MoveName::Spirit_Shackle:
		case MoveName::Darkest_Lariat:
		case MoveName::Sparkling_Aria:
		case MoveName::Ice_Hammer:
		case MoveName::Floral_Healing:
		case MoveName::High_Horsepower:
		case MoveName::Strength_Sap:
		case MoveName::Solar_Blade:
		case MoveName::Leafage:
		case MoveName::Spotlight:
		case MoveName::Toxic_Thread:
		case MoveName::Laser_Focus:
		case MoveName::Gear_Up:
		case MoveName::Throat_Chop:
		case MoveName::Pollen_Puff:
		case MoveName::Anchor_Shot:
		case MoveName::Psychic_Terrain:
		case MoveName::Lunge:
		case MoveName::Fire_Lash:
		case MoveName::Power_Trip:
		case MoveName::Burn_Up:
		case MoveName::Speed_Swap:
		case MoveName::Smart_Strike:
		case MoveName::Purify:
		case MoveName::Revelation_Dance:
		case MoveName::Core_Enforcer:
		case MoveName::Trop_Kick:
		case MoveName::Instruct:
		case MoveName::Beak_Blast:
		case MoveName::Clanging_Scales:
		case MoveName::Dragon_Hammer:
		case MoveName::Brutal_Swing:
		case MoveName::Aurora_Veil:
		case MoveName::Sinister_Arrow_Raid:
		case MoveName::Malicious_Moonsault:
		case MoveName::Oceanic_Operetta:
		case MoveName::Guardian_of_Alola:
		case MoveName::Soul_Stealing_7_Star_Strike:
		case MoveName::Stoked_Sparksurfer:
		case MoveName::Pulverizing_Pancake:
		case MoveName::Extreme_Evoboost:
		case MoveName::Genesis_Supernova:
		case MoveName::Shell_Trap:
		case MoveName::Fleur_Cannon:
		case MoveName::Psychic_Fangs:
		case MoveName::Stomping_Tantrum:
		case MoveName::Shadow_Bone:
		case MoveName::Accelerock:
		case MoveName::Liquidation:
		case MoveName::Prismatic_Laser:
		case MoveName::Spectral_Thief:
		case MoveName::Sunsteel_Strike:
		case MoveName::Moongeist_Beam:
		case MoveName::Tearful_Look:
		case MoveName::Zing_Zap:
		case MoveName::Natures_Madness:
		case MoveName::Multi_Attack:
		case MoveName::m10000000_Volt_Thunderbolt:
		case MoveName::Mind_Blown:
		case MoveName::Plasma_Fists:
		case MoveName::Photon_Geyser:
		case MoveName::Light_That_Burns_the_Sky:
		case MoveName::Searing_Sunraze_Smash:
		case MoveName::Menacing_Moonraze_Maelstrom:
		case MoveName::Lets_Snuggle_Forever:
		case MoveName::Splintered_Stormshards:
		case MoveName::Clangorous_Soulblaze:
		case MoveName::Double_Iron_Bash:
			return no_effect<UserTeam>;
	}
}

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(UserPokemon) \
	template auto possible_side_effects(MoveName, UserPokemon, OtherTeam<AssociatedTeam<UserPokemon>> const &, Weather) -> SideEffects<AssociatedTeam<UserPokemon>>

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION(generation) \
	TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(AnyActivePokemon<Pokemon<generation>>); \
	TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(AnyActivePokemon<SeenPokemon<generation>>); \
	TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(AnyActivePokemon<KnownPokemon<generation>>)

TECHNICALMACHINE_FOR_EACH_GENERATION(TECHNICALMACHINE_EXPLICIT_INSTANTIATION);

} // namespace technicalmachine
