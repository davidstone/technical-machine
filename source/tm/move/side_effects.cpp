// Random effects of moves
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/move/side_effects.hpp>

#include <tm/move/moves.hpp>
#include <tm/move/is_switch.hpp>
#include <tm/move/will_be_recharge_turn.hpp>

#include <tm/team.hpp>

#include <bounded/assert.hpp>

#include <containers/algorithms/count.hpp>
#include <containers/algorithms/filter_iterator.hpp>
#include <containers/emplace_back.hpp>
#include <containers/integer_range.hpp>
#include <containers/push_back.hpp>
#include <containers/size.hpp>

namespace technicalmachine {
using namespace bounded::literal;

namespace {

template<Generation generation>
constexpr auto guaranteed_effect(auto function) {
	return SideEffects<generation>({
		SideEffect<generation>{
			1.0,
			function
		}
	});
}

template<Generation generation>
constexpr auto no_effect = guaranteed_effect<generation>(no_effect_function);

template<Generation generation>
constexpr auto basic_probability(double const probability, auto function) {
	return probability == 1.0 ?
		guaranteed_effect<generation>(function) :
		SideEffects<generation>({
			SideEffect<generation>{1.0 - probability, no_effect_function},
			SideEffect<generation>{probability, function},
		});
}

template<Generation generation>
constexpr auto absorb_effect = guaranteed_effect<generation>([](auto & user, auto & other, auto & weather, auto const damage) {
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
	static_assert(bounded::min_value<Denominator> == bounded::max_value<Denominator>);

	RecoilEffect() = default;
	constexpr explicit RecoilEffect(Denominator denominator):
		m_denominator(denominator)
	{
	}

	template<Generation generation>
	constexpr auto operator()(Team<generation> & user, Team<generation> &, Weather & weather, auto const damage) const {
		auto const user_pokemon = user.pokemon();
		if (!blocks_recoil(user_pokemon.ability())) {
			change_hp(user_pokemon, weather, -bounded::max(damage / m_denominator, 1_bi));
		}
	}

private:
	[[no_unique_address]] Denominator m_denominator;
};

template<Generation generation>
constexpr auto confusion_effect(double const probability, ActivePokemon<generation> const original_target, auto const... maybe_immune_type) {
	return (... or is_type(original_target, maybe_immune_type)) ?
		no_effect<generation> :
		basic_probability<generation>(probability, [](auto &, auto & target, auto & weather, auto) {
			target.pokemon().confuse(weather);
		});
}

constexpr auto flinch = [](auto &, auto & target, auto &, auto) {
	target.pokemon().flinch();
};

template<BoostableStat stat, int stages>
constexpr auto boost_user_stat = [](auto & user, auto &, auto &, auto) {
	user.pokemon().stage()[stat] += bounded::constant<stages>;
};

template<BoostableStat stat, int stages>
constexpr auto boost_target_stat = [](auto &, auto & target, auto &, auto) {
	target.pokemon().stage()[stat] += bounded::constant<stages>;
};

constexpr auto status_is_clausable(Statuses const status) {
	switch (status) {
		case Statuses::burn:
		case Statuses::paralysis:
		case Statuses::poison:
		case Statuses::toxic:
		case Statuses::rest:
		case Statuses::clear:
			return false;
		case Statuses::freeze:
		case Statuses::sleep:
			return true;
	}
}

template<Generation generation>
constexpr auto status_can_apply(Statuses const status, ActivePokemon<generation> const user, Team<generation> const & target, Weather const weather, auto const... immune_types) {
	auto const target_pokemon = target.pokemon();
	return
		is_clear(target_pokemon.status()) and
		!blocks_status(target_pokemon.ability(), user.ability(), status, weather) and
		(... and !is_type(target_pokemon, immune_types)) and
		(!status_is_clausable(status) or !team_has_status(target, status)) and
		(status != Statuses::sleep or (!user.is_uproaring() and !target_pokemon.is_uproaring()));
}


template<Statuses status>
constexpr auto set_status_function = []<Generation generation>(Team<generation> & user, Team<generation> & target, auto & weather, auto) {
	apply_status(status, user.pokemon(), target.pokemon(), weather);
};

template<Statuses status, Generation generation>
constexpr auto status_effect(double const probability, ActivePokemon<generation> const original_user, Team<generation> const & original_target, Weather const original_weather, auto const... immune_types) {
	return status_can_apply(status, original_user, original_target, original_weather, immune_types...) ?
		basic_probability<generation>(probability, set_status_function<status>) :
		no_effect<generation>;
}

template<Generation generation, BoostableStat stat, int stages>
constexpr auto confusing_stat_boost = guaranteed_effect<generation>([](auto &, auto & other, auto & weather, auto) {
	auto target = other.pokemon();
	target.stage()[stat] += bounded::constant<stages>;
	target.confuse(weather);
});

template<Statuses status, Generation generation>
constexpr auto fang_effects(ActivePokemon<generation> const original_user, Team<generation> const & original_target, Weather const original_weather, Type const immune_type) {
	constexpr auto status_and_flinch_function = [](auto & user, auto & target, auto & weather, auto const damage) {
		set_status_function<status>(user, target, weather, damage);
		flinch(user, target, weather, damage);
	};
	return status_can_apply(status, original_user, original_target, original_weather, immune_type) ?
		SideEffects<generation>({
			{0.81, no_effect_function},
			{0.09, set_status_function<status>},
			{0.09, flinch},
			{0.01, status_and_flinch_function}
		}) :
		basic_probability<generation>(0.1, flinch);
}

template<Statuses status, Generation generation>
auto recoil_status(ActivePokemon<generation> const original_user, Team<generation> const & original_target, Weather const original_weather, Type const immune_type) {
	constexpr auto recoil_and_status = [](auto & user, auto & target, auto & weather, auto const damage) {
		set_status_function<status>(user, target, weather, damage);
		RecoilEffect(3_bi)(user, target, weather, damage);
	};
	return status_can_apply(status, original_user, original_target, original_weather, immune_type) ?
		SideEffects<generation>({
			{0.9, RecoilEffect(3_bi)},
			{0.1, recoil_and_status}
		}) :
		guaranteed_effect<generation>(RecoilEffect(3_bi));
}

template<Statuses const status>
auto try_apply_status(auto & user, auto & target, auto & weather, auto const damage, auto const... immune_types) {
	if (status_can_apply(status, as_const(user.pokemon()), target, weather, immune_types...)) {
		set_status_function<status>(user, target, weather, damage);
	}
}

template<Generation generation>
constexpr auto tri_attack_effect(ActivePokemon<generation> const original_user, Team<generation> const & original_target, Weather const original_weather) {
	// TODO: Foresight, Wonder Guard, Scrappy
	if (is_type(original_target.pokemon(), Type::Ghost)) {
		return no_effect<generation>;
	}
	constexpr auto burn = SideEffect<generation>({1.0 / 15.0, set_status_function<Statuses::burn>});
	constexpr auto freeze = SideEffect<generation>({1.0 / 15.0, set_status_function<Statuses::freeze>});
	constexpr auto paralysis = SideEffect<generation>({1.0 / 15.0, set_status_function<Statuses::paralysis>});

	constexpr auto burn_freeze_paralysis_probabilities = SideEffects<generation>({{12.0 / 15.0, no_effect_function}, burn, freeze, paralysis});
	constexpr auto burn_freeze_probabilities = SideEffects<generation>({{13.0 / 15.0, no_effect_function}, burn, freeze});
	constexpr auto burn_paralysis_probabilities = SideEffects<generation>({{13.0 / 15.0, no_effect_function}, burn, paralysis});
	constexpr auto freeze_paralysis_probabilities = SideEffects<generation>({{13.0 / 15.0, no_effect_function}, freeze, paralysis});
	constexpr auto burn_probabilities = SideEffects<generation>({{14.0 / 15.0, no_effect_function}, burn});
	constexpr auto freeze_probabilities = SideEffects<generation>({{14.0 / 15.0, no_effect_function}, freeze});
	constexpr auto paralysis_probabilities = SideEffects<generation>({{14.0 / 15.0, no_effect_function}, paralysis});

	switch (generation) {
		case Generation::one:
			return no_effect<generation>;
		case Generation::two: {
			constexpr auto thaw = SideEffect<generation>{4.0 / 15.0, [](auto &, auto & target, auto &, auto) {
				target.pokemon().clear_status();
			}};
			auto const freeze_claused = team_has_status(original_target, Statuses::freeze);
			switch (original_target.pokemon().status().name()) {
				case Statuses::clear:
					return freeze_claused ?
						burn_paralysis_probabilities :
						burn_freeze_paralysis_probabilities;
				case Statuses::freeze:
					return SideEffects<generation>({{10.0 / 15.0, no_effect_function}, thaw, burn});
				default:
					return no_effect<generation>;
			}
		}
		default: {
			auto const can_burn = status_can_apply(Statuses::burn, original_user, original_target, original_weather, Type::Fire);
			auto const can_freeze = status_can_apply(Statuses::freeze, original_user, original_target, original_weather, Type::Ice);
			auto const can_paralyze = generation <= Generation::five ?
				status_can_apply(Statuses::paralysis, original_user, original_target, original_weather) :
				status_can_apply(Statuses::paralysis, original_user, original_target, original_weather, Type::Electric);

			return
				can_burn and can_freeze and can_paralyze ? burn_freeze_paralysis_probabilities :
				can_burn and can_freeze ? burn_freeze_probabilities :
				can_burn and can_paralyze ? burn_paralysis_probabilities :
				can_freeze and can_paralyze ? freeze_paralysis_probabilities :
				can_burn ? burn_probabilities :
				can_freeze ? freeze_probabilities :
				can_paralyze ? paralysis_probabilities :
				no_effect<generation>;
		}
	}
}

template<Generation generation>
auto cure_all_status(Team<generation> & user, auto const & predicate) -> void {
	for (auto & pokemon : user.all_pokemon()) {
		if (predicate(pokemon)) {
			pokemon.set_status(Statuses::clear);
		}
	}
}

constexpr auto fling_effects = [](auto & user_team, auto & target_team, auto & weather, auto const damage) {
	auto user = user_team.pokemon();
	auto target = target_team.pokemon();
	// TODO: Activate berry
	switch (user.item(weather)) {
		case Item::Flame_Orb:
			try_apply_status<Statuses::burn>(user_team, target_team, weather, damage, Type::Fire);
			break;
		case Item::Kings_Rock:
		case Item::Razor_Fang:
			target.flinch();
			break;
		case Item::Light_Ball:
			try_apply_status<Statuses::paralysis>(user_team, target_team, weather, damage);
			break;
		case Item::Mental_Herb:
			apply_mental_herb(target);
			break;
		case Item::Poison_Barb:
			try_apply_status<Statuses::poison>(user_team, target_team, weather, damage, Type::Poison, Type::Steel);
			break;
		case Item::Toxic_Orb:
			try_apply_status<Statuses::toxic>(user_team, target_team, weather, damage, Type::Poison, Type::Steel);
			break;
		case Item::White_Herb:
			apply_white_herb(target);
			break;
		default:
			break;
	}
	user.remove_item();
};

constexpr auto recover_half = [](auto & user, auto &, auto & weather, auto) {
	heal(user.pokemon(), weather, rational(1_bi, 2_bi));
};

constexpr auto stat_can_boost = [](Stage::value_type const stage) {
	return stage != bounded::max_value<Stage::value_type>;
};

template<auto...>
struct sequence {};

template<Generation generation>
constexpr auto acupressure_effect(ActivePokemon<generation> const target) {
	auto result = SideEffects<generation>();
	auto const stage = target.stage();
	auto const probability = 1.0 / double(containers::count_if(stage, stat_can_boost));

	auto add_stat = [&]<BoostableStat stat>(std::integral_constant<BoostableStat, stat>) {
		if (stat_can_boost(stage[stat])) {
			containers::push_back(result, SideEffect<generation>{probability, boost_user_stat<stat, 2>});
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

template<Generation generation>
constexpr auto active_pokemon_can_be_phazed(Team<generation> const & team) {
	return !team.pokemon().ingrained() and !blocks_phazing(team.pokemon().ability()) and containers::size(team.all_pokemon()) > 1_bi;
}

template<Generation generation, int index>
constexpr auto phaze = [](Team<generation> & user, Team<generation> & target, Weather & weather, auto) {
	target.switch_pokemon(user.pokemon(), weather, bounded::constant<index>);
};

template<Generation generation>
constexpr auto phaze_effect(Team<generation> const & target) {
	if (!active_pokemon_can_be_phazed(target)) {
		return no_effect<generation>;
	}
	// TODO: Phazing activates Synchronize if Toxic Spikes cause status before
	// generation 5
	auto is_not_active = [&](TeamIndex const index) {
		return index != target.all_pokemon().index();
	};

	auto const probability = 1.0 / double(target.size() - 1_bi);
	auto result = SideEffects<generation>();
	auto add_one = [&](auto const index) {
		if (is_not_active(index) and index < target.size()) {
			containers::push_back(result, SideEffect<generation>{probability, phaze<generation, index.value()>});
		}
	};
	auto add_all = [&]<std::size_t... indexes>(std::index_sequence<indexes...>) {
		(..., add_one(bounded::constant<indexes>));
	};
	add_all(std::make_index_sequence<bounded::max_value<TeamSize>.value()>());
	return result;
}

template<Generation generation, int index>
constexpr auto switch_effect = guaranteed_effect<generation>([](Team<generation> & user, Team<generation> & other, Weather & weather, auto) {
	user.switch_pokemon(other.pokemon(), weather, bounded::constant<index>);
});

template<Generation generation>
constexpr auto charge_up_move(
	Moves const move_name,
	ActivePokemon<generation> const original_user,
	ActivePokemon<generation> const original_other,
	Weather const original_weather,
	SideEffects<generation> when_used
) {
	return will_be_recharge_turn(original_user, move_name, original_other.ability(), original_weather) ?
		guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
			user.pokemon().use_charge_up_move();
		}) :
		std::move(when_used);
}

template<Generation generation>
auto item_can_be_lost(ActivePokemon<generation> const pokemon) {
	return
		pokemon.ability() != Ability::Sticky_Hold or
		(generation >= Generation::five and pokemon.hp().current() == 0_bi);
}

template<Generation generation>
auto item_can_be_incinerated(ActivePokemon<generation> const target, Weather const weather) -> bool {
	// TODO: Destroy gems
	return item_can_be_lost(target) and berry_power(target.item(weather)) != 0_bi;
}

constexpr auto equalize_hp = [](auto & user, auto & other, auto & weather, auto) {
	auto const lhs = user.pokemon();
	auto const rhs = other.pokemon();
	auto const average = (lhs.hp().current() + rhs.hp().current()) / 2_bi;
	lhs.set_hp(weather, average);
	rhs.set_hp(weather, average);
};

constexpr auto can_confuse_with_chatter(Species const pokemon) {
	return pokemon == Species::Chatot;
}

} // namespace

template<Generation generation>
auto possible_side_effects(Moves const move, ActivePokemon<generation> const original_user, Team<generation> const & original_other, Weather const original_weather) -> SideEffects<generation> {
	switch (move) {
		case Moves::Absorb:
		case Moves::Drain_Punch:
		case Moves::Giga_Drain:
		case Moves::Leech_Life:
		case Moves::Mega_Drain:
			return absorb_effect<generation>;
		case Moves::Dream_Eater:
			return is_sleeping(original_other.pokemon().status()) ? absorb_effect<generation> : no_effect<generation>;
		case Moves::Brave_Bird:
		case Moves::Double_Edge:
		case Moves::Wood_Hammer:
			return generation <= Generation::two ?
				guaranteed_effect<generation>(RecoilEffect(4_bi)) :
				guaranteed_effect<generation>(RecoilEffect(3_bi));
		case Moves::Head_Smash:
			return guaranteed_effect<generation>(RecoilEffect(2_bi));
		case Moves::Submission:
		case Moves::Take_Down:
			return guaranteed_effect<generation>(RecoilEffect(4_bi));

		case Moves::Metal_Claw:
			return basic_probability<generation>(0.1, boost_user_stat<BoostableStat::atk, 1>);
		case Moves::Meteor_Mash:
			return basic_probability<generation>(0.2, boost_user_stat<BoostableStat::atk, 1>);
		case Moves::Howl:
		case Moves::Meditate:
		case Moves::Sharpen:
			return guaranteed_effect<generation>(boost_user_stat<BoostableStat::atk, 1>);
		case Moves::Swords_Dance:
			return guaranteed_effect<generation>(boost_user_stat<BoostableStat::atk, 2>);
		case Moves::Charm:
		case Moves::Feather_Dance:
			return guaranteed_effect<generation>(boost_target_stat<BoostableStat::atk, -2>);
		case Moves::Aurora_Beam:
			return basic_probability<generation>(generation == Generation::one ? 0.332 : 0.1, boost_target_stat<BoostableStat::atk, -1>);
		case Moves::Growl:
			return guaranteed_effect<generation>(boost_target_stat<BoostableStat::atk, -1>);

		case Moves::Steel_Wing:
			return basic_probability<generation>(0.1, boost_user_stat<BoostableStat::def, 1>);
		case Moves::Harden:
		case Moves::Withdraw:
			return guaranteed_effect<generation>(boost_user_stat<BoostableStat::def, 1>);
		case Moves::Acid_Armor:
		case Moves::Barrier:
		case Moves::Iron_Defense:
			return guaranteed_effect<generation>(boost_user_stat<BoostableStat::def, 2>);
		case Moves::Screech:
			return guaranteed_effect<generation>(boost_target_stat<BoostableStat::def, -2>);
		case Moves::Iron_Tail:
			return basic_probability<generation>(0.1, boost_target_stat<BoostableStat::def, -1>);
		case Moves::Crunch: {
			constexpr auto stat = generation <= Generation::three ? BoostableStat::spd : BoostableStat::def;
			return basic_probability<generation>(0.2, boost_target_stat<stat, -1>);
		}
		case Moves::Crush_Claw:
		case Moves::Razor_Shell:
		case Moves::Rock_Smash:
			return basic_probability<generation>(0.5, boost_target_stat<BoostableStat::def, -1>);
		case Moves::Leer:
		case Moves::Tail_Whip:
			return guaranteed_effect<generation>(boost_target_stat<BoostableStat::def, -1>);

		case Moves::Draco_Meteor:
		case Moves::Leaf_Storm:
		case Moves::Overheat:
		case Moves::Psycho_Boost:
			return guaranteed_effect<generation>(boost_user_stat<BoostableStat::spa, -2>);
		case Moves::Mist_Ball:
			return basic_probability<generation>(0.5, boost_user_stat<BoostableStat::spa, 1>);
		case Moves::Charge_Beam:
			return basic_probability<generation>(0.7, boost_user_stat<BoostableStat::spa, 1>);
		case Moves::Nasty_Plot:
		case Moves::Tail_Glow:
			return guaranteed_effect<generation>(boost_user_stat<BoostableStat::spa, 2>);
		case Moves::Snarl:
		case Moves::Struggle_Bug:
			return guaranteed_effect<generation>(boost_target_stat<BoostableStat::spa, -1>);

		case Moves::Seed_Flare:
			return basic_probability<generation>(0.4, boost_target_stat<BoostableStat::spd, -2>);
		case Moves::Fake_Tears:
		case Moves::Metal_Sound:
			return guaranteed_effect<generation>(boost_target_stat<BoostableStat::spd, -2>);
		case Moves::Captivate:
			return multiplier(original_user.gender(), original_other.pokemon().gender()) == -1_bi ?
				guaranteed_effect<generation>(boost_target_stat<BoostableStat::spd, -2>) :
				no_effect<generation>;
		case Moves::Bug_Buzz:
		case Moves::Earth_Power:
		case Moves::Energy_Ball:
		case Moves::Flash_Cannon:
		case Moves::Focus_Blast:
			return basic_probability<generation>(0.1, boost_target_stat<BoostableStat::spd, -1>);
		case Moves::Shadow_Ball:
			return basic_probability<generation>(0.2, boost_target_stat<BoostableStat::spd, -1>);
		case Moves::Luster_Purge:
			return guaranteed_effect<generation>(boost_target_stat<BoostableStat::spd, -1>);

		case Moves::Hammer_Arm:
			return guaranteed_effect<generation>(boost_user_stat<BoostableStat::spe, -1>);
		case Moves::Agility:
		case Moves::Rock_Polish:
			return guaranteed_effect<generation>(boost_user_stat<BoostableStat::spe, 2>);
		case Moves::Scary_Face:
			return guaranteed_effect<generation>(boost_target_stat<BoostableStat::spe, -2>);
		case Moves::Cotton_Spore:
			return (generation <= Generation::five or !is_type(original_other.pokemon(), Type::Grass)) ?
				guaranteed_effect<generation>(boost_target_stat<BoostableStat::spe, -2>) :
				no_effect<generation>;
		case Moves::Bubble:
		case Moves::Bubble_Beam:
		case Moves::Constrict:
			return basic_probability<generation>(
				generation == Generation::one ? 0.332 : 0.1,
				boost_target_stat<BoostableStat::spe, -1>
			);
		case Moves::Icy_Wind:
		case Moves::Low_Sweep:
		case Moves::Mud_Shot:
		case Moves::Rock_Tomb:
			return guaranteed_effect<generation>(boost_target_stat<BoostableStat::spe, -1>);

		case Moves::Leaf_Tornado:
		case Moves::Mirror_Shot:
		case Moves::Mud_Bomb:
		case Moves::Muddy_Water:
			return basic_probability<generation>(0.3, boost_target_stat<BoostableStat::acc, -1>);
		case Moves::Night_Daze:
			return basic_probability<generation>(0.4, boost_target_stat<BoostableStat::acc, -1>);
		case Moves::Octazooka:
			return basic_probability<generation>(0.5, boost_target_stat<BoostableStat::acc, -1>);
		case Moves::Flash:
		case Moves::Kinesis:
		case Moves::Mud_Slap:
		case Moves::Sand_Attack:
		case Moves::Smokescreen:
			return guaranteed_effect<generation>(boost_target_stat<BoostableStat::acc, -1>);

		case Moves::Double_Team:
			return guaranteed_effect<generation>(boost_user_stat<BoostableStat::eva, 1>);
		case Moves::Sweet_Scent:
			return guaranteed_effect<generation>(boost_target_stat<BoostableStat::eva, -1>);

		case Moves::Acid: {
			constexpr auto probability = generation == Generation::one ? 0.332 : 0.1;
			constexpr auto stat = generation <= Generation::three ? BoostableStat::def : BoostableStat::spd;
			return basic_probability<generation>(probability, boost_target_stat<stat, -1>);
		}
		case Moves::Acupressure:
			// TODO: Probability not correct due to the maxing out behavior
			return acupressure_effect<generation>(original_user);
		case Moves::Amnesia:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				auto pokemon = user.pokemon();
				pokemon.stage()[BoostableStat::spd] += 2_bi;
				if (generation == Generation::one) {
					pokemon.stage()[BoostableStat::spa] += 2_bi;
				}
			});
		case Moves::Ancient_Power:
		case Moves::Ominous_Wind:
		case Moves::Silver_Wind:
			return basic_probability<generation>(0.1, [](auto & user, auto &, auto &, auto) {
				boost_regular(user.pokemon().stage(), 1_bi);
			});
		case Moves::Belly_Drum:
			return guaranteed_effect<generation>([](auto & user, auto &, auto & weather, auto) {
				auto const user_pokemon = user.pokemon();
				auto const hp = user_pokemon.hp();
				if (hp.current() > hp.max() / 2_bi and hp.current() > 1_bi) {
					change_hp(user_pokemon, weather, -hp.max() / 2_bi);
					user_pokemon.stage()[BoostableStat::atk] = 6_bi;
				} else if constexpr (generation == Generation::two) {
					user_pokemon.stage()[BoostableStat::atk] += 2_bi;
				}
			});
		case Moves::Close_Combat:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				boost_physical(user.pokemon().stage(), -1_bi);
			});
		case Moves::Bulk_Up:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				boost_physical(user.pokemon().stage(), 1_bi);
			});
		case Moves::Calm_Mind:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				boost_special(user.pokemon().stage(), 1_bi);
			});
		case Moves::Cosmic_Power:
		case Moves::Defend_Order:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				boost_defensive(user.pokemon().stage(), 1_bi);
			});
		case Moves::Dragon_Dance:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				auto & stage = user.pokemon().stage();
				stage[BoostableStat::atk] += 1_bi;
				stage[BoostableStat::spe] += 1_bi;
			});
		case Moves::Growth:
			return guaranteed_effect<generation>([](auto & user, auto & other, auto & weather, auto) {
				auto const user_pokemon = user.pokemon();
				auto & stage = user_pokemon.stage();
				switch (generation) {
					case Generation::one:
						for (auto const stat : {BoostableStat::spa, BoostableStat::spd}) {
							stage[stat] += 1_bi;
						}
						break;
					case Generation::two:
					case Generation::three:
					case Generation::four:
						stage[BoostableStat::spa] += 1_bi;
						break;
					default: {
						auto const boost = BOUNDED_CONDITIONAL(
							weather.sun(weather_is_blocked_by_ability(user_pokemon.ability(), other.pokemon().ability())),
							2_bi,
							1_bi
						);
						for (auto const stat : {BoostableStat::atk, BoostableStat::spa}) {
							stage[stat] += boost;
						}
						break;
					}
				}
			});
		case Moves::Psychic:
			return generation == Generation::one ?
				basic_probability<generation>(0.332, [](auto &, auto & other, auto &, auto) {
					auto & stage = other.pokemon().stage();
					stage[BoostableStat::spa] -= 1_bi;
					stage[BoostableStat::spd] -= 1_bi;
				}) :
				basic_probability<generation>(0.1, boost_target_stat<BoostableStat::spd, -1>);
		case Moves::Quiver_Dance:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				auto & stage = user.pokemon().stage();
				for (auto const stat : {BoostableStat::spa, BoostableStat::spd, BoostableStat::spe}) {
					stage[stat] += 1_bi;
				}
			});
		case Moves::Shell_Smash:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				auto & stage = user.pokemon().stage();
				for (auto const stat : {BoostableStat::def, BoostableStat::spd}) {
					stage[stat] -= 1_bi;
				}
				for (auto const stat : {BoostableStat::atk, BoostableStat::spa, BoostableStat::spe}) {
					stage[stat] += 2_bi;
				}
			});
		case Moves::Shift_Gear:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				auto & stage = user.pokemon().stage();
				stage[BoostableStat::atk] += 1_bi;
				stage[BoostableStat::spe] += 2_bi;
			});
		case Moves::String_Shot:
			return generation <= Generation::five ?
				guaranteed_effect<generation>(boost_target_stat<BoostableStat::spe, -1>) :
				guaranteed_effect<generation>(boost_target_stat<BoostableStat::spe, -2>);
		case Moves::Superpower:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				boost_physical(user.pokemon().stage(), -1_bi);
			});
		case Moves::Tickle:
			return guaranteed_effect<generation>([](auto &, auto & other, auto &, auto) {
				boost_physical(other.pokemon().stage(), -1_bi);
			});
		case Moves::V_create:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				auto & stage = user.pokemon().stage();
				for (auto const stat : {BoostableStat::def, BoostableStat::spd, BoostableStat::spe}) {
					stage[stat] -= 1_bi;
				}
			});

		case Moves::Haze:
			return guaranteed_effect<generation>([](auto & user, auto & other, auto &, auto) {
				user.pokemon().stage() = Stage{};
				other.pokemon().stage() = Stage{};
				if (generation == Generation::one) {
					user.shatter_screens();
					other.shatter_screens();
				}
			});
		case Moves::Power_Swap:
			return guaranteed_effect<generation>([](auto & user, auto & other, auto &, auto) {
				swap_offensive(user.pokemon().stage(), other.pokemon().stage());
			});
		case Moves::Psych_Up:
			return guaranteed_effect<generation>([](auto & user, auto & other, auto &, auto) {
				user.pokemon().stage() = other.pokemon().stage();
			});

		case Moves::Charge:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				auto const pokemon = user.pokemon();
				pokemon.charge();
				pokemon.stage()[BoostableStat::spd] += 1_bi;
			});
		case Moves::Defense_Curl:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				auto const pokemon = user.pokemon();
				pokemon.stage()[BoostableStat::def] += 1_bi;
				pokemon.defense_curl();
			});

		case Moves::Bone_Club:
		case Moves::Extrasensory:
		case Moves::Headbutt:
		case Moves::Hyper_Fang:
		case Moves::Stomp:
			return basic_probability<generation>(0.1, flinch);
		case Moves::Dark_Pulse:
		case Moves::Dragon_Rush:
		case Moves::Twister:
		case Moves::Zen_Headbutt:
			return basic_probability<generation>(0.2, flinch);
		case Moves::Air_Slash:
		case Moves::Astonish:
		case Moves::Icicle_Crash:
		case Moves::Iron_Head:
		case Moves::Rolling_Kick:
		case Moves::Snore:
		case Moves::Steamroller:
		case Moves::Needle_Arm:
			return basic_probability<generation>(0.3, flinch);
		case Moves::Fake_Out:
			return guaranteed_effect<generation>(flinch);
		case Moves::Bite:
			return basic_probability<generation>(generation <= Generation::one ? 0.1 : 0.3, flinch);
		case Moves::Rock_Slide:
			return generation == Generation::one ? no_effect<generation> : basic_probability<generation>(0.3, flinch);
		case Moves::Waterfall:
			return generation <= Generation::three ? no_effect<generation> : basic_probability<generation>(0.2, flinch);
		case Moves::Low_Kick:
			return generation <= Generation::two ? basic_probability<generation>(0.3, flinch) : no_effect<generation>;

		case Moves::Aromatherapy:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				cure_all_status(user, [](Pokemon<generation> const &) { return true; });
			});
		case Moves::Heal_Bell:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				cure_all_status(user, [=](Pokemon<generation> const & pokemon) {
					if constexpr (generation == Generation::five) {
						return true;
					} else {
						auto const is_active = std::addressof(pokemon) == std::addressof(user.all_pokemon()());
						auto const ability = is_active ? user.pokemon().ability() : pokemon.initial_ability();
						return !blocks_sound_moves(ability);
					}
				});
			});
		case Moves::Psycho_Shift:
			return guaranteed_effect<generation>([](auto & user, auto & other, auto & weather, auto const damage) {
				auto const other_pokemon = other.pokemon();
				if (!is_clear(other_pokemon.status())) {
					return;
				}
				auto const user_pokemon = user.pokemon();
				auto const status_name = user_pokemon.status().name();
				switch (status_name) {
					case Statuses::burn:
						try_apply_status<Statuses::burn>(user, other, weather, damage, Type::Fire);
						break;
					case Statuses::paralysis:
						try_apply_status<Statuses::paralysis>(user, other, weather, damage);
						break;
					case Statuses::poison:
						try_apply_status<Statuses::poison>(user, other, weather, damage, Type::Poison, Type::Steel);
						break;
					case Statuses::toxic:
						try_apply_status<Statuses::toxic>(user, other, weather, damage, Type::Poison, Type::Steel);
						break;
					case Statuses::sleep: // TODO: Sleep Clause
					case Statuses::rest: // TODO: How does Rest shift?
						try_apply_status<Statuses::sleep>(user, other, weather, damage);
						break;
					case Statuses::clear:
					case Statuses::freeze:
						break;
				}
				user_pokemon.clear_status();
			});
		case Moves::Refresh:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				user.pokemon().clear_status();
			});
		case Moves::Wake_Up_Slap:
			return guaranteed_effect<generation>([](auto &, auto & other, auto &, auto) {
				auto const pokemon = other.pokemon();
				if (is_sleeping(pokemon.status())) {
					pokemon.clear_status();
				}
			});

		case Moves::Blaze_Kick:
		case Moves::Ember:
		case Moves::Fire_Punch:
		case Moves::Flame_Wheel:
		case Moves::Flamethrower:
		case Moves::Heat_Wave:
			return status_effect<Statuses::burn>(0.1, original_user, original_other, original_weather, Type::Fire);
		case Moves::Lava_Plume:
		case Moves::Scald:
		case Moves::Searing_Shot:
			return status_effect<Statuses::burn>(0.3, original_user, original_other, original_weather, Type::Fire);
		case Moves::Sacred_Fire:
			return status_effect<Statuses::burn>(0.5, original_user, original_other, original_weather, Type::Fire);
		case Moves::Inferno:
		case Moves::Will_O_Wisp:
			return status_effect<Statuses::burn>(1.0, original_user, original_other, original_weather, Type::Fire);
		case Moves::Fire_Blast:
			return status_effect<Statuses::burn>(generation == Generation::one ? 0.3 : 0.1, original_user, original_other, original_weather, Type::Fire);

		case Moves::Blizzard:
		case Moves::Ice_Beam:
		case Moves::Ice_Punch:
		case Moves::Powder_Snow:
			return status_effect<Statuses::freeze>(0.1, original_user, original_other, original_weather, Type::Ice);

		case Moves::Dragon_Breath:
			return status_effect<Statuses::paralysis>(0.3, original_user, original_other, original_weather);
		case Moves::Force_Palm:
		case Moves::Secret_Power:
			return status_effect<Statuses::paralysis>(0.3, original_user, original_other, original_weather, Type::Ghost);
		case Moves::Discharge:
		case Moves::Spark:
			return status_effect<Statuses::paralysis>(0.3, original_user, original_other, original_weather, Type::Ground);
		case Moves::Thunder_Wave:
		case Moves::Zap_Cannon:
			return status_effect<Statuses::paralysis>(1.0, original_user, original_other, original_weather, Type::Ground);
		case Moves::Body_Slam:
			return generation == Generation::one ?
				status_effect<Statuses::paralysis>(0.3, original_user, original_other, original_weather, Type::Ghost, Type::Normal) :
				status_effect<Statuses::paralysis>(0.3, original_user, original_other, original_weather, Type::Ghost);
		case Moves::Lick:
			return generation == Generation::one ?
				status_effect<Statuses::paralysis>(0.3, original_user, original_other, original_weather, Type::Normal, Type::Ghost) :
				status_effect<Statuses::paralysis>(0.3, original_user, original_other, original_weather, Type::Normal);
		case Moves::Thunderbolt:
		case Moves::Thunder_Punch:
		case Moves::Thunder_Shock:
			return generation == Generation::one ?
				status_effect<Statuses::paralysis>(0.1, original_user, original_other, original_weather, Type::Ground, Type::Electric) :
				status_effect<Statuses::paralysis>(0.1, original_user, original_other, original_weather, Type::Ground);
		case Moves::Thunder:
			return generation == Generation::one ?
				status_effect<Statuses::paralysis>(0.1, original_user, original_other, original_weather, Type::Ground, Type::Electric) :
				status_effect<Statuses::paralysis>(0.3, original_user, original_other, original_weather, Type::Ground);
		case Moves::Stun_Spore:
			return generation <= Generation::five ?
				status_effect<Statuses::paralysis>(1.0, original_user, original_other, original_weather) :
				status_effect<Statuses::paralysis>(1.0, original_user, original_other, original_weather, Type::Grass);
		case Moves::Glare:
			switch (generation) {
				case Generation::one:
					return status_effect<Statuses::paralysis>(1.0, original_user, original_other, original_weather);
				case Generation::two:
				case Generation::three:
					return status_effect<Statuses::paralysis>(1.0, original_user, original_other, original_weather, Type::Ghost);
				case Generation::four:
				case Generation::five:
					return status_effect<Statuses::paralysis>(1.0, original_user, original_other, original_weather);
				case Generation::six:
				case Generation::seven:
				case Generation::eight:
					return status_effect<Statuses::paralysis>(1.0, original_user, original_other, original_weather, Type::Electric);
			}

		case Moves::Cross_Poison:
		case Moves::Poison_Tail:
		case Moves::Sludge_Wave:
			return status_effect<Statuses::poison>(0.1, original_user, original_other, original_weather, Type::Poison, Type::Steel);
		case Moves::Gunk_Shot:
		case Moves::Poison_Jab:
		case Moves::Sludge_Bomb:
			return status_effect<Statuses::poison>(0.3, original_user, original_other, original_weather, Type::Poison, Type::Steel);
		case Moves::Smog:
			return status_effect<Statuses::poison>(0.4, original_user, original_other, original_weather, Type::Poison, Type::Steel);
		case Moves::Poison_Gas:
		case Moves::Poison_Powder:
			return generation <= Generation::five ?
				status_effect<Statuses::poison>(1.0, original_user, original_other, original_weather, Type::Poison, Type::Steel) :
				status_effect<Statuses::poison>(1.0, original_user, original_other, original_weather, Type::Grass, Type::Poison, Type::Steel);
		case Moves::Poison_Sting:
			return status_effect<Statuses::poison>(generation == Generation::one ? 0.2 : 0.3, original_user, original_other, original_weather, Type::Poison, Type::Steel);
		case Moves::Sludge:
			return status_effect<Statuses::poison>(generation == Generation::one ? 0.4 : 0.3, original_user, original_other, original_weather, Type::Poison, Type::Steel);
		case Moves::Twineedle:
			return generation == Generation::two ?
				status_effect<Statuses::poison>(0.2, original_user, original_other, original_weather, Type::Poison) :
				status_effect<Statuses::poison>(0.2, original_user, original_other, original_weather, Type::Poison, Type::Steel);

		case Moves::Poison_Fang: {
			constexpr auto probability = generation <= Generation::five ? 0.3 : 0.5;
			return status_effect<Statuses::toxic>(probability, original_user, original_other, original_weather, Type::Poison, Type::Steel);
		}
		case Moves::Toxic:
			return status_effect<Statuses::toxic>(1.0, original_user, original_other, original_weather, Type::Poison, Type::Steel);

		case Moves::Dark_Void:
		case Moves::Grass_Whistle:
		case Moves::Hypnosis:
		case Moves::Lovely_Kiss:
		case Moves::Sing:
			return status_effect<Statuses::sleep>(1.0, original_user, original_other, original_weather);
		case Moves::Sleep_Powder:
		case Moves::Spore:
			return generation <= Generation::five ?
				status_effect<Statuses::sleep>(1.0, original_user, original_other, original_weather) :
				status_effect<Statuses::sleep>(1.0, original_user, original_other, original_weather, Type::Grass);
		case Moves::Yawn:
			return guaranteed_effect<generation>([](auto &, auto & other, auto &, auto) {
				other.pokemon().hit_with_yawn();
			});

		case Moves::Flare_Blitz:
			return recoil_status<Statuses::burn>(original_user, original_other, original_weather, Type::Fire);
		case Moves::Volt_Tackle:
			return generation <= Generation::three ?
				guaranteed_effect<generation>(RecoilEffect(3_bi)) :
				recoil_status<Statuses::paralysis>(original_user, original_other, original_weather, Type::Ground);

		case Moves::Fire_Fang:
			return fang_effects<Statuses::burn>(original_user, original_other, original_weather, Type::Fire);
		case Moves::Ice_Fang:
			return fang_effects<Statuses::freeze>(original_user, original_other, original_weather, Type::Ice);
		case Moves::Thunder_Fang:
			return fang_effects<Statuses::paralysis>(original_user, original_other, original_weather, Type::Ground);

		case Moves::Tri_Attack:
			return tri_attack_effect(original_user, original_other, original_weather);

		case Moves::Signal_Beam:
			return confusion_effect(0.1, original_other.pokemon());
		case Moves::Confusion:
		case Moves::Psybeam:
			return confusion_effect(0.1, original_other.pokemon(), Type::Dark);
		case Moves::Rock_Climb:
		case Moves::Water_Pulse:
			return confusion_effect(0.2, original_other.pokemon());
		case Moves::Confuse_Ray:
		case Moves::Supersonic:
		case Moves::Sweet_Kiss:
		case Moves::Teeter_Dance:
			return confusion_effect(1.0, original_other.pokemon());
		case Moves::Dynamic_Punch:
			if constexpr (generation <= Generation::two) {
				return confusion_effect(0.996, original_other.pokemon(), Type::Ghost);
			} else {
				return confusion_effect(1.0, original_other.pokemon(), Type::Ghost);
			}
		case Moves::Dizzy_Punch:
			return generation == Generation::one ?
				no_effect<generation> :
				confusion_effect(0.2, original_other.pokemon(), Type::Ghost);
		case Moves::Chatter: {
			constexpr auto probability = generation <= Generation::five ? 0.31 : 1.0;
			return can_confuse_with_chatter(original_user.species()) ?
				confusion_effect(probability, original_other.pokemon(), Type::Ghost) :
				no_effect<generation>;
		}

		case Moves::Rest:
			return guaranteed_effect<generation>([](auto & user, auto & other, auto & weather, auto) {
				user.pokemon().rest(weather, other.pokemon().is_uproaring());
			});

		case Moves::Smelling_Salts:
			return boosts_smellingsalt(original_other.pokemon().status()) ?
				guaranteed_effect<generation>([](auto &, auto & other, auto &, auto) {
					other.pokemon().status() = Status{};
				}) :
				no_effect<generation>;

		case Moves::Flatter:
			return confusing_stat_boost<generation, BoostableStat::spa, 1>;
		case Moves::Swagger:
			return confusing_stat_boost<generation, BoostableStat::atk, 2>;

		case Moves::Bounce:
			// TODO: Paralysis
		case Moves::Dig:
		case Moves::Dive:
		case Moves::Fly:
			return guaranteed_effect<generation>([](auto & user, auto &, auto & weather, auto) {
				user.pokemon().use_vanish_move(weather);
			});
		case Moves::Shadow_Force:
			return guaranteed_effect<generation>([](auto & user, auto & other, auto & weather, auto) {
				auto const hit = user.pokemon().use_vanish_move(weather);
				if (hit) {
					other.pokemon().break_protect();
				}
			});

		case Moves::Razor_Wind:
		case Moves::Solar_Beam:
			return charge_up_move(
				move,
				original_user,
				original_other.pokemon(),
				original_weather,
				no_effect<generation>
			);
		case Moves::Skull_Bash:
			return charge_up_move(
				move,
				original_user,
				original_other.pokemon(),
				original_weather,
				generation >= Generation::two ? guaranteed_effect<generation>(boost_user_stat<BoostableStat::def, 1>) : no_effect<generation>
			);
		case Moves::Sky_Attack:
			return charge_up_move(
				move,
				original_user,
				original_other.pokemon(),
				original_weather,
				SideEffects<generation>({
					SideEffect<generation>{0.7, no_effect_function},
					SideEffect<generation>{0.3, flinch}
				})
			);

		case Moves::Gravity:
			return guaranteed_effect<generation>([](auto &, auto &, auto & weather, auto) {
				weather.activate_gravity();
			});
		case Moves::Hail:
			return guaranteed_effect<generation>([](auto & user, auto &, auto & weather, auto) {
				weather.activate_hail_from_move(extends_hail(user.pokemon().item(weather)));
			});
		case Moves::Rain_Dance:
			return guaranteed_effect<generation>([](auto & user, auto &, auto & weather, auto) {
				weather.activate_rain_from_move(extends_rain(user.pokemon().item(weather)));
			});
		case Moves::Sandstorm:
			return guaranteed_effect<generation>([](auto & user, auto &, auto & weather, auto) {
				weather.activate_sand_from_move(extends_sand(user.pokemon().item(weather)));
			});
		case Moves::Sunny_Day:
			return guaranteed_effect<generation>([](auto & user, auto &, auto & weather, auto) {
				weather.activate_sun_from_move(extends_sun(user.pokemon().item(weather)));
			});
		case Moves::Trick_Room:
			return guaranteed_effect<generation>([](auto &, auto &, auto & weather, auto) {
				weather.activate_trick_room();
			});
	
		case Moves::Light_Screen:
			return guaranteed_effect<generation>([](auto & user, auto &, auto & weather, auto) {
				user.activate_light_screen(weather);
			});
		case Moves::Lucky_Chant:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				user.activate_lucky_chant();
			});
		case Moves::Mist:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				user.activate_mist();
			});
		case Moves::Reflect:
			return guaranteed_effect<generation>([](auto & user, auto &, auto & weather, auto) {
				user.activate_reflect(weather);
			});
		case Moves::Safeguard:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				user.activate_safeguard();
			});
		case Moves::Tailwind:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				user.activate_tailwind();
			});

		case Moves::Heal_Order:
		case Moves::Milk_Drink:
		case Moves::Recover:
		case Moves::Roost:
		case Moves::Slack_Off:
		case Moves::Soft_Boiled:
			return generation == Generation::one and healing_move_fails_in_generation_1(original_user.hp()) ?
				no_effect<generation> :
				guaranteed_effect<generation>(recover_half);
		case Moves::Moonlight:
		case Moves::Morning_Sun:
		case Moves::Synthesis:
			return guaranteed_effect<generation>([](auto & user, auto & other, auto & weather, auto) {
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
		case Moves::Wish:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				user.activate_wish();
			});

		case Moves::Spikes:
			return guaranteed_effect<generation>([](auto &, auto & other, auto &, auto) {
				other.add_spikes();
			});
		case Moves::Stealth_Rock:
			return guaranteed_effect<generation>([](auto &, auto & other, auto &, auto) {
				other.add_stealth_rock();
			});
		case Moves::Toxic_Spikes:
			return guaranteed_effect<generation>([](auto &, auto & other, auto &, auto) {
				other.add_toxic_spikes();
			});

		case Moves::Magnitude:
			return no_effect<generation>;
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
		case Moves::Present:
			// TODO: Figure out how to represent variable damage
			// heal 80: 0.25
			// 40 power: 0.25
			// 80 power: 0.25
			// 120 power: 0.25
			return no_effect<generation>;
		case Moves::Psywave:
			return no_effect<generation>;
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

		case Moves::Spit_Up:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				user.pokemon().release_stockpile();
			});
		case Moves::Stockpile:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				user.pokemon().increment_stockpile();
			});
		case Moves::Swallow:
			return guaranteed_effect<generation>([](auto & user, auto &, auto & weather, auto) {
				auto const pokemon = user.pokemon();
				auto const stockpiles = pokemon.release_stockpile();
				if (stockpiles == 0_bi) {
					return;
				}
				heal(pokemon, weather, swallow_healing(stockpiles));
			});

		case Moves::Mud_Sport:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				user.pokemon().activate_mud_sport();
			});
		case Moves::Water_Sport:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				user.pokemon().activate_water_sport();
			});

		case Moves::Aqua_Ring:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				user.pokemon().activate_aqua_ring();
			});
		case Moves::Attract:
			return multiplier(original_user.gender(), original_other.pokemon().gender()) == -1_bi ?
				guaranteed_effect<generation>([](auto & user, auto & other, auto & weather, auto) {
					other.pokemon().attract(user.pokemon(), weather);
				}) :
				no_effect<generation>;
		case Moves::Bide:
			return guaranteed_effect<generation>([](auto & user, auto & other, auto & weather, auto) {
				user.pokemon().use_bide(other.pokemon(), weather);
			});
		case Moves::Bind:
		case Moves::Clamp:
		case Moves::Fire_Spin:
		case Moves::Magma_Storm:
		case Moves::Sand_Tomb:
		case Moves::Whirlpool:
		case Moves::Wrap:
			return guaranteed_effect<generation>([](auto &, auto & other, auto &, auto) {
				other.pokemon().partially_trap();
			});
		case Moves::Block:
		case Moves::Mean_Look:
		case Moves::Spider_Web:
			return guaranteed_effect<generation>([](auto &, auto & other, auto &, auto) {
				other.pokemon().fully_trap();
			});
		case Moves::Blast_Burn:
		case Moves::Frenzy_Plant:
		case Moves::Giga_Impact:
		case Moves::Hydro_Cannon:
		case Moves::Hyper_Beam:
		case Moves::Roar_of_Time:
		case Moves::Rock_Wrecker:
			return guaranteed_effect<generation>([](auto & user, auto & other, auto &, auto) {
				if (generation >= Generation::two or other.pokemon().hp().current() != 0_bi) {
					user.pokemon().use_recharge_move();
				}
			});
		case Moves::Bug_Bite:
		case Moves::Pluck:
			return guaranteed_effect<generation>([](auto & user, auto & other, auto &, auto) {
				static_cast<void>(user);
				auto const other_pokemon = other.pokemon();
				if (item_can_be_lost(as_const(other_pokemon))) {
				}
			});
		case Moves::Camouflage:
			return no_effect<generation>;
		case Moves::Conversion:
			return no_effect<generation>;
		case Moves::Conversion_2:
			return no_effect<generation>;
		case Moves::Covet:
		case Moves::Thief:
			return guaranteed_effect<generation>([](auto & user, auto & other, auto &, auto) {
				if (item_can_be_lost(as_const(other.pokemon()))) {
					user.pokemon().steal_item(other.pokemon());
				}
			});
		case Moves::Curse:
			return guaranteed_effect<generation>([](auto & user, auto & other, auto & weather, auto) {
				auto const user_pokemon = user.pokemon();
				if (is_type(as_const(user_pokemon), Type::Ghost)) {
					auto const other_pokemon = other.pokemon();
					if (!other_pokemon.is_cursed()) {
						user_pokemon.indirect_damage(weather, user_pokemon.hp().max() / 2_bi);
						other_pokemon.curse();
					}
				} else {
					auto & stat_stage = user_pokemon.stage();
					if constexpr (generation == Generation::two) {
						constexpr auto max = bounded::max_value<Stage::value_type>;
						if (stat_stage[BoostableStat::atk] == max and stat_stage[BoostableStat::def] == max) {
							return;
						}
					}
					stat_stage[BoostableStat::atk] += 1_bi;
					stat_stage[BoostableStat::def] += 1_bi;
					stat_stage[BoostableStat::spe] -= 1_bi;
				}
			});
		case Moves::Defog:
			return guaranteed_effect<generation>([](auto &, auto & other, auto & weather, auto) {
				weather.deactivate_fog();
				other.pokemon().stage()[BoostableStat::eva] -= 1_bi;
			});
		case Moves::Detect:
		case Moves::Protect:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				user.pokemon().protect();
			});
		case Moves::Disable:
			return guaranteed_effect<generation>([](auto &, auto & other, auto & weather, auto) {
				auto pokemon = other.pokemon();
				pokemon.disable(pokemon.last_used_move().name(), weather);
			});
		case Moves::Doom_Desire:
		case Moves::Future_Sight:
			return no_effect<generation>;
		case Moves::Embargo:
			return guaranteed_effect<generation>([](auto &, auto & other, auto &, auto) {
				other.pokemon().activate_embargo();
			});
		case Moves::Encore:
			return guaranteed_effect<generation>([](auto &, auto & other, auto & weather, auto) {
				other.pokemon().activate_encore(weather);
			});
		case Moves::Explosion:
		case Moves::Self_Destruct:
			return (generation == Generation::one and original_other.pokemon().substitute()) ?
				guaranteed_effect<generation>([](auto & user, auto & other, auto & weather, auto) {
					if (other.pokemon().substitute()) {
						user.pokemon().set_hp(weather, 0_bi);
					}
				}) :
				guaranteed_effect<generation>([](auto & user, auto &, auto & weather, auto) {
					user.pokemon().set_hp(weather, 0_bi);
				});
		case Moves::Feint:
		case Moves::Hyperspace_Fury:
		case Moves::Hyperspace_Hole:
		case Moves::Phantom_Force:
			return guaranteed_effect<generation>([](auto &, auto & other, auto &, auto) {
				other.pokemon().break_protect();
			});
		case Moves::Fling:
			return guaranteed_effect<generation>(fling_effects);
		case Moves::Focus_Energy:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				user.pokemon().focus_energy();
			});
		case Moves::Follow_Me:
			return no_effect<generation>;
		case Moves::Foresight:
		case Moves::Odor_Sleuth:
			return guaranteed_effect<generation>([](auto &, auto & other, auto &, auto) {
				other.pokemon().identify();
			});
		case Moves::Gastro_Acid:
			return no_effect<generation>;
		case Moves::Grudge:
			return no_effect<generation>;
		case Moves::Guard_Swap:
			return guaranteed_effect<generation>([](auto & user, auto & other, auto &, auto) {
				swap_defensive(user.pokemon().stage(), other.pokemon().stage());
			});
		case Moves::Heal_Block:
			return guaranteed_effect<generation>([](auto &, auto & other, auto &, auto) {
				other.pokemon().activate_heal_block();
			});
		case Moves::Healing_Wish:
			return no_effect<generation>;
		case Moves::Heart_Swap:
			return guaranteed_effect<generation>([](auto & user, auto & other, auto &, auto) {
				std::swap(user.pokemon().stage(), other.pokemon().stage());
			});
		case Moves::High_Jump_Kick:
		case Moves::Jump_Kick:
			return no_effect<generation>;
		case Moves::Ice_Ball:
		case Moves::Rollout:
			return no_effect<generation>;
		case Moves::Imprison:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				user.pokemon().use_imprison();
			});
		case Moves::Incinerate:
			return guaranteed_effect<generation>([](auto &, auto & other, auto & weather, auto) {
				auto other_pokemon = other.pokemon();
				if (item_can_be_incinerated(as_const(other_pokemon), weather)) {
					other_pokemon.destroy_item();
				}
			});
		case Moves::Ingrain:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				user.pokemon().ingrain();
			});
		case Moves::Knock_Off:
			return guaranteed_effect<generation>([](auto &, auto & other, auto &, auto) {
				auto const other_pokemon = other.pokemon();
				if (item_can_be_lost(as_const(other_pokemon))) {
					other_pokemon.remove_item();
				}
			});
		case Moves::Leech_Seed:
			return guaranteed_effect<generation>([](auto &, auto & other, auto &, auto) {
				other.pokemon().hit_with_leech_seed();
			});
		case Moves::Lunar_Dance:
			return no_effect<generation>;
		case Moves::Magic_Coat:
			return no_effect<generation>;
		case Moves::Magic_Room:
			return guaranteed_effect<generation>([](auto &, auto &, auto & weather, auto) {
				weather.activate_magic_room();
			});
		case Moves::Magnet_Rise:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				user.pokemon().activate_magnet_rise();
			});
		case Moves::Me_First:
			return no_effect<generation>;
		case Moves::Memento:
			return guaranteed_effect<generation>([](auto & user, auto & other, auto & weather, auto) {
				boost_offensive(other.pokemon().stage(), -2_bi);
				user.pokemon().set_hp(weather, 0_bi);
			});
		case Moves::Mimic:
			return no_effect<generation>;
		case Moves::Minimize:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				user.pokemon().minimize();
			});
		case Moves::Miracle_Eye:
			return no_effect<generation>;
		case Moves::Nightmare:
			return guaranteed_effect<generation>([](auto &, auto & other, auto &, auto) {
				other.pokemon().try_to_give_nightmares();
			});
		case Moves::Outrage:
		case Moves::Petal_Dance:
		case Moves::Thrash:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				user.pokemon().activate_rampage();
			});
		case Moves::Pain_Split:
			return guaranteed_effect<generation>(equalize_hp);
		case Moves::Perish_Song:
			return guaranteed_effect<generation>([](auto & user, auto & other, auto &, auto) {
				user.pokemon().try_activate_perish_song();
				other.pokemon().try_activate_perish_song();
			});
		case Moves::Power_Trick:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				user.pokemon().activate_power_trick();
			});
		case Moves::Rage:
			return no_effect<generation>;
		case Moves::Rapid_Spin:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				user.clear_field();
				if constexpr (generation >= Generation::eight) {
					user.pokemon().stage()[BoostableStat::spe] += 1_bi;
				}
			});
		case Moves::Recycle:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				user.pokemon().recycle_item();
			});
		case Moves::Roar:
		case Moves::Whirlwind:
			switch (generation) {
				case Generation::one:
					return no_effect<generation>;
				case Generation::two:
					return original_other.pokemon().moved() ? phaze_effect<generation>(original_other) : no_effect<generation>;
				default:
					return phaze_effect<generation>(original_other);
			}
		case Moves::Role_Play:
			return no_effect<generation>;
		case Moves::Sketch:
			return no_effect<generation>;
		case Moves::Skill_Swap:
			return no_effect<generation>;
		case Moves::Snatch:
			return no_effect<generation>;
		case Moves::Spite:
			return no_effect<generation>;
		case Moves::Struggle:
			return guaranteed_effect<generation>([](auto & user, auto &, auto & weather, auto const damage) {
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
		case Moves::Substitute:
			return guaranteed_effect<generation>([](auto & user, auto &, auto & weather, auto) {
				user.pokemon().use_substitute(weather);
			});
		case Moves::Switch0:
			return switch_effect<generation, 0>;
		case Moves::Switch1:
			return switch_effect<generation, 1>;
		case Moves::Switch2:
			return switch_effect<generation, 2>;
		case Moves::Switch3:
			return switch_effect<generation, 3>;
		case Moves::Switch4:
			return switch_effect<generation, 4>;
		case Moves::Switch5:
			return switch_effect<generation, 5>;
		case Moves::Switcheroo:
		case Moves::Trick:
			return guaranteed_effect<generation>([](auto & user, auto & other, auto &, auto) {
				auto const other_pokemon = other.pokemon();
				if (item_can_be_lost(as_const(other_pokemon))) {
					swap_items(user.pokemon(), other.pokemon());
				}
			});
		case Moves::Taunt:
			return guaranteed_effect<generation>([](auto &, auto & other, auto & weather, auto) {
				other.pokemon().taunt(weather);
			});
		case Moves::Torment:
			return guaranteed_effect<generation>([](auto &, auto & other, auto & weather, auto) {
				other.pokemon().torment(weather);
			});
		case Moves::Transform:
			return no_effect<generation>;
		case Moves::Uproar:
			return guaranteed_effect<generation>([](auto & user, auto &, auto &, auto) {
				user.pokemon().use_uproar();
			});
		case Moves::Worry_Seed:
			return no_effect<generation>;

		case Moves::Acid_Spray:
		case Moves::Acrobatics:
		case Moves::Aerial_Ace:
		case Moves::Aeroblast:
		case Moves::After_You:
		case Moves::Air_Cutter:
		case Moves::Ally_Switch:
		case Moves::Aqua_Jet:
		case Moves::Aqua_Tail:
		case Moves::Arm_Thrust:
		case Moves::Assist:
		case Moves::Assurance:
		case Moves::Attack_Order:
		case Moves::Aura_Sphere:
		case Moves::Autotomize:
		case Moves::Avalanche:
		case Moves::Barrage:
		case Moves::Baton_Pass:
		case Moves::Beat_Up:
		case Moves::Bestow:
		case Moves::Bonemerang:
		case Moves::Bone_Rush:
		case Moves::Brick_Break:
		case Moves::Brine:
		case Moves::Bulldoze:
		case Moves::Bullet_Punch:
		case Moves::Bullet_Seed:
		case Moves::Chip_Away:
		case Moves::Circle_Throw:
		case Moves::Clear_Smog:
		case Moves::Coil:
		case Moves::Comet_Punch:
		case Moves::Copycat:
		case Moves::Cotton_Guard:
		case Moves::Counter:
		case Moves::Crabhammer:
		case Moves::Cross_Chop:
		case Moves::Crush_Grip:
		case Moves::Cut:
		case Moves::Destiny_Bond:
		case Moves::Double_Hit:
		case Moves::Double_Kick:
		case Moves::Double_Slap:
		case Moves::Dragon_Claw:
		case Moves::Dragon_Pulse:
		case Moves::Dragon_Rage:
		case Moves::Dragon_Tail:
		case Moves::Drill_Peck:
		case Moves::Drill_Run:
		case Moves::Dual_Chop:
		case Moves::Earthquake:
		case Moves::Echoed_Voice:
		case Moves::Egg_Bomb:
		case Moves::Electro_Ball:
		case Moves::Electroweb:
		case Moves::Endeavor:
		case Moves::Endure:
		case Moves::Entrainment:
		case Moves::Eruption:
		case Moves::Extreme_Speed:
		case Moves::Facade:
		case Moves::Feint_Attack:
		case Moves::False_Swipe:
		case Moves::Fiery_Dance:
		case Moves::Final_Gambit:
		case Moves::Fire_Pledge:
		case Moves::Fissure:
		case Moves::Flail:
		case Moves::Flame_Burst:
		case Moves::Flame_Charge:
		case Moves::Focus_Punch:
		case Moves::Foul_Play:
		case Moves::Frost_Breath:
		case Moves::Frustration:
		case Moves::Fury_Attack:
		case Moves::Fury_Cutter:
		case Moves::Fury_Swipes:
		case Moves::Fusion_Bolt:
		case Moves::Fusion_Flare:
		case Moves::Gear_Grind:
		case Moves::Glaciate:
		case Moves::Grass_Knot:
		case Moves::Grass_Pledge:
		case Moves::Guard_Split:
		case Moves::Guillotine:
		case Moves::Gust:
		case Moves::Gyro_Ball:
		case Moves::Head_Charge:
		case Moves::Heal_Pulse:
		case Moves::Heat_Crash:
		case Moves::Heavy_Slam:
		case Moves::Helping_Hand:
		case Moves::Hex:
		case Moves::Hidden_Power:
		case Moves::Hit_Self:
		case Moves::Hone_Claws:
		case Moves::Horn_Attack:
		case Moves::Horn_Drill:
		case Moves::Horn_Leech:
		case Moves::Hydro_Pump:
		case Moves::Hyper_Voice:
		case Moves::Ice_Shard:
		case Moves::Icicle_Spear:
		case Moves::Judgment:
		case Moves::Karate_Chop:
		case Moves::Last_Resort:
		case Moves::Leaf_Blade:
		case Moves::Lock_On:
		case Moves::Mach_Punch:
		case Moves::Magical_Leaf:
		case Moves::Magnet_Bomb:
		case Moves::Megahorn:
		case Moves::Mega_Kick:
		case Moves::Mega_Punch:
		case Moves::Metal_Burst:
		case Moves::Metronome:
		case Moves::Mind_Reader:
		case Moves::Mirror_Coat:
		case Moves::Mirror_Move:
		case Moves::Natural_Gift:
		case Moves::Nature_Power:
		case Moves::Night_Shade:
		case Moves::Night_Slash:
		case Moves::Pass:
		case Moves::Payback:
		case Moves::Pay_Day:
		case Moves::Peck:
		case Moves::Pin_Missile:
		case Moves::Pound:
		case Moves::Power_Gem:
		case Moves::Power_Split:
		case Moves::Power_Whip:
		case Moves::Psycho_Cut:
		case Moves::Psyshock:
		case Moves::Psystrike:
		case Moves::Punishment:
		case Moves::Pursuit:
		case Moves::Quash:
		case Moves::Quick_Attack:
		case Moves::Quick_Guard:
		case Moves::Rage_Powder:
		case Moves::Razor_Leaf:
		case Moves::Reflect_Type:
		case Moves::Retaliate:
		case Moves::Return:
		case Moves::Revenge:
		case Moves::Reversal:
		case Moves::Rock_Blast:
		case Moves::Rock_Throw:
		case Moves::Round:
		case Moves::Sacred_Sword:
		case Moves::Scratch:
		case Moves::Secret_Sword:
		case Moves::Seed_Bomb:
		case Moves::Seismic_Toss:
		case Moves::Shadow_Claw:
		case Moves::Shadow_Punch:
		case Moves::Shadow_Sneak:
		case Moves::Sheer_Cold:
		case Moves::Shock_Wave:
		case Moves::Simple_Beam:
		case Moves::Sky_Drop:
		case Moves::Sky_Uppercut:
		case Moves::Slam:
		case Moves::Slash:
		case Moves::Sleep_Talk:
		case Moves::Smack_Down:
		case Moves::Soak:
		case Moves::Sonic_Boom:
		case Moves::Spacial_Rend:
		case Moves::Spike_Cannon:
		case Moves::Splash:
		case Moves::Stone_Edge:
		case Moves::Stored_Power:
		case Moves::Storm_Throw:
		case Moves::Strength:
		case Moves::Sucker_Punch:
		case Moves::Super_Fang:
		case Moves::Surf:
		case Moves::Swift:
		case Moves::Synchronoise:
		case Moves::Tackle:
		case Moves::Tail_Slap:
		case Moves::Techno_Blast:
		case Moves::Telekinesis:
		case Moves::Teleport:
		case Moves::Triple_Kick:
		case Moves::Trump_Card:
		case Moves::U_turn:
		case Moves::Vacuum_Wave:
		case Moves::Venoshock:
		case Moves::Vice_Grip:
		case Moves::Vine_Whip:
		case Moves::Vital_Throw:
		case Moves::Volt_Switch:
		case Moves::Water_Gun:
		case Moves::Water_Pledge:
		case Moves::Water_Spout:
		case Moves::Weather_Ball:
		case Moves::Wide_Guard:
		case Moves::Wild_Charge:
		case Moves::Wing_Attack:
		case Moves::Wonder_Room:
		case Moves::Work_Up:
		case Moves::Wring_Out:
		case Moves::X_Scissor:
			return no_effect<generation>;

		case Moves::Blue_Flare:
		case Moves::Bolt_Strike:
		case Moves::Relic_Song:
		case Moves::Freeze_Shock:
		case Moves::Heart_Stamp:
		case Moves::Hurricane:
		case Moves::Ice_Burn:
		case Moves::Flying_Press:
		case Moves::Mat_Block:
		case Moves::Belch:
		case Moves::Rototiller:
		case Moves::Sticky_Web:
		case Moves::Fell_Stinger:
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
			return no_effect<generation>;
	}
}

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION(generation) \
	template auto possible_side_effects<generation>(Moves, ActivePokemon<generation> const user, Team<generation> const & other, Weather) -> SideEffects<generation>

TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::one);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::two);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::three);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::four);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::five);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::six);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::seven);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::eight);

} // namespace technicalmachine
