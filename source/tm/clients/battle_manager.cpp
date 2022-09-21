// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/battle_manager.hpp>

#include <tm/clients/log_foe_team.hpp>

#include <tm/evaluate/expectiminimax.hpp>

#include <tm/string_conversions/move_name.hpp>
#include <tm/string_conversions/team.hpp>
#include <tm/string_conversions/weather.hpp>

#include <tm/team_predictor/team_predictor.hpp>
#include <tm/team_predictor/usage_stats.hpp>

#include <tm/ability_blocks_move.hpp>
#include <tm/any_team.hpp>
#include <tm/battle.hpp>

#include <bounded/detail/variant/variant.hpp>

#include <iostream>
#include <stdexcept>
#include <string_view>

namespace technicalmachine {

BattleManager::~BattleManager() = default;

namespace {

constexpr auto to_normal_weather(Weather const weather) {
	if (weather.hail()) {
		return NormalWeather::hail;
	} else if (weather.sand()) {
		return NormalWeather::sand;
	} else if (weather.sun()) {
		return NormalWeather::sun;
	} else if (weather.rain()) {
		return NormalWeather::rain;
	} else {
		return NormalWeather::clear;
	}
}

constexpr auto moved(any_team auto const & team) -> bool {
	auto const pokemon = team.pokemon();
	return pokemon.last_used_move().moved_this_turn() or pokemon.hp().current() == 0_bi;
};

constexpr auto get_move_index(RegularMoves const moves, MoveName const move_name) -> containers::index_type<RegularMoves> {
	if (move_name == MoveName::Struggle) {
		return 0_bi;
	}
	auto const it = containers::find_if(moves, [=](Move const move) { return move.name() == move_name; });
	if (it == containers::end(moves)) {
		throw std::runtime_error(containers::concatenate<std::string>("Pokemon does not know "sv, to_string(move_name)));
	}
	return bounded::assume_in_range<containers::index_type<RegularMoves>>(it - containers::begin(moves));
}

constexpr auto causes_recoil(MoveName const move) {
	switch (move) {
		case MoveName::Brave_Bird:
		case MoveName::Double_Edge:
		case MoveName::Flare_Blitz:
		case MoveName::Head_Smash:
		case MoveName::Submission:
		case MoveName::Take_Down:
		case MoveName::Volt_Tackle:
		case MoveName::Wood_Hammer:
			return true;
		default:
			return false;
	}
}

constexpr auto cures_target_status_impl(Generation const generation, MoveName const move_name, bounded::optional<Type> const hidden_power_type, StatusName const status) {
	switch (status) {
		case StatusName::freeze:
			return
				get_type(generation, move_name, hidden_power_type) == Type::Fire or
				(generation == Generation::two and move_name == MoveName::Tri_Attack);
		case StatusName::paralysis:
			return move_name == MoveName::Smelling_Salts;
		case StatusName::rest:
		case StatusName::sleep:
			return move_name == MoveName::Wake_Up_Slap;
		case StatusName::clear:
		case StatusName::burn:
		case StatusName::poison:
		case StatusName::toxic:
			return false;
	}
}

template<any_active_pokemon UserPokemon>
auto get_side_effect(Used const move, UserPokemon const user, OtherTeam<UserPokemon> const & other, Weather const weather) {
	auto const side_effects = possible_side_effects(move.executed, user, other, weather);

	if (containers::size(side_effects) == 1_bi) {
		return containers::front(side_effects).function;
	}

	if (move.phaze_index) {
		if (move.confuse or move.status) {
			throw std::runtime_error("Tried to phaze and do other side effects.");
		}
		auto const index = *move.phaze_index;
		if (index == other.all_pokemon().index()) {
			throw std::runtime_error("Tried to phaze in current Pokemon");
		}
		using PhazeIndex = bounded::integer<0, bounded::normalize<max_pokemon_per_team - 2_bi>>;
		auto const effect_index = (index < other.all_pokemon().index()) ?
			bounded::assume_in_range<PhazeIndex>(index) :
			bounded::assume_in_range<PhazeIndex>(index - 1_bi);
		return side_effects[effect_index].function;
	}

	// TODO: Handle multi-effect situations
	if (move.confuse or move.status) {
		return side_effects[1_bi].function;
	}

	return containers::front(side_effects).function;
}

template<any_team UserTeam>
auto to_used_move(Used const move, UserTeam const & user, OtherTeam<UserTeam> const & other, Weather const weather) {
	return UsedMove<UserTeam>(
		move.selected,
		move.executed,
		move.critical_hit,
		move.miss,
		move.contact_ability_effect,
		get_side_effect(move, user.pokemon(), other, weather)
	);
}

auto const & select_pokemon(any_team auto const & team, MoveName const move) {
	return is_switch(move) ? team.pokemon(to_replacement(move)) : team.all_pokemon()();
}

auto hp_to_damage(any_pokemon auto const & pokemon, HP::current_type const new_hp) {
	auto const old_hp = pokemon.hp().current();
	if (new_hp > old_hp) {
		std::cerr << "Took negative damage\n";
		return HP::current_type(0_bi);
		// throw std::runtime_error("Took negative damage");
	}
	return bounded::assume_in_range<HP::current_type>(old_hp - new_hp);
}

template<Generation generation_>
struct BattleManagerImpl final : BattleManager {
	BattleManagerImpl(
		AnalysisLogger analysis_logger,
		UsageStats const & usage_stats,
		Evaluate<generation_> evaluate,
		DepthValues const depth,
		std::mt19937 random_engine,
		Teams<generation_> teams,
		bool log_foe_teams
	):
		m_usage_stats(usage_stats),
		m_analysis_logger(std::move(analysis_logger)),
		m_random_engine(std::move(random_engine)),
		m_evaluate(evaluate),
		m_battle(
			std::move(teams).ai,
			std::move(teams).foe
		),
		m_depth(depth),
		m_log_foe_teams(log_foe_teams)
	{
	}

	auto generation() const -> Generation final {
		return generation_;
	}
	auto random_engine() & -> std::mt19937 & final {
		return m_random_engine;
	}
	auto move_index(MoveName const move_name) const -> containers::index_type<RegularMoves> final {
		return get_move_index(m_battle.ai().pokemon().regular_moves(), move_name);
	}

	auto ai_has(Species const species, containers::string nickname, Level const level, Gender const gender) & -> MoveName final {
		return m_battle.find_ai_pokemon(species, std::move(nickname), level, gender);
	}
	auto foe_has(Species const species, containers::string nickname, Level const level, Gender const gender) & -> MoveName final {
		return m_battle.find_or_add_foe_pokemon(species, std::move(nickname), level, gender);
	}

	auto active_has(bool const is_ai, MoveName const move_name) & -> void final {
		m_battle.add_move(is_ai, move_name);
	}

	auto active_has(bool const is_ai, Ability const ability) & -> void final {
		m_battle.active_has(is_ai, ability);
	}
	auto replacement_has(bool const is_ai, TeamIndex const index, Ability const ability) & -> void final {
		m_battle.replacement_has(is_ai, index, ability);
	}

	auto active_has(bool const is_ai, Item const item) & -> void final {
		m_battle.active_has(is_ai, item);
	}
	auto replacement_has(bool const is_ai, TeamIndex const index, Item const item) & -> void final {
		m_battle.replacement_has(is_ai, index, item);
	}

	auto is_end_of_turn() const -> bool final {
		return moved(m_battle.ai()) and moved(m_battle.foe());
	}

	auto ai_is_fainted() const -> bool final {
		return m_battle.ai().pokemon().hp().current() == 0_bi;
	}
	auto foe_is_fainted() const -> bool final {
		return m_battle.foe().pokemon().hp().current() == 0_bi;
	}

	auto ai_is_on_last_pokemon() const -> bool final {
		return m_battle.ai().size() == 1_bi;
	}

	auto begin_turn(TurnCount const turn_count) & -> void final {
		m_analysis_logger.get() << containers::string(containers::repeat_n(20_bi, '=')) << "\nBegin turn " << turn_count << '\n';
		m_battle.handle_begin_turn();
	}
	auto end_turn(bool const ai_went_first, EndOfTurnFlags const first_flags, EndOfTurnFlags const last_flags) & -> void final {
		m_battle.handle_end_turn(ai_went_first, first_flags, last_flags);
	}

	auto use_move(bool const ai_is_user, MoveResult const move_result, bool const status_clears) & -> void final {
		// TODO: Handle the other states better
		apply_to_teams(ai_is_user, [&](auto const & user_team, auto const & other_team) {
			auto const move = bounded::visit(move_result, bounded::overload(
				[](Used const used) { return used; },
				[&](Recharging) { return Used{user_team.pokemon().last_used_move().name()}; },
				[](auto) { return Used{MoveName::Struggle}; }
			));
			active_has(ai_is_user, move.selected);
			if (move.selected == MoveName::Sleep_Talk) {
				active_has(ai_is_user, move.executed);
			}
			auto const user_hidden_power_type = get_hidden_power_type(user_team.pokemon());
			auto const move_type = get_type(generation_, move.executed, user_hidden_power_type);
			auto const other_pokemon = other_team.pokemon();
			auto const ability_blocks_recoil =
				causes_recoil(move.executed) and
				!move.recoil and
				!Effectiveness(generation_, move_type, other_pokemon.types()).has_no_effect() and
				!ability_blocks_move(
					generation_,
					other_pokemon.ability(),
					KnownMove{
						move.executed,
						move_type
					},
					other_pokemon.status().name(),
					other_pokemon.types()
				);
			if (ability_blocks_recoil) {
				// TODO: This could also be Magic Guard
				active_has(ai_is_user, Ability::Rock_Head);
			}
			
			auto const damage = bounded::visit(move.damage, bounded::overload(
				[](NoDamage) {
					return FlaggedActualDamage{ActualDamage::Known{0_bi}, false};
				},
				[&](VisibleHP const hp) -> FlaggedActualDamage {
					auto const value = compute_damage(ai_is_user, move.executed, hp);
					return FlaggedActualDamage{
						ActualDamage::Known{value},
						value != 0_bi
					};
				},
				[&](SubstituteDamaged) -> FlaggedActualDamage {
					if (!other_pokemon.substitute()) {
						throw std::runtime_error("Tried to damage a Substitute when the target does not have a Substitute");
					}
					return FlaggedActualDamage{
						ActualDamage::Capped{bounded::increase_min<0>(other_pokemon.substitute().hp() - 1_bi)},
						true
					};
				},
				[&](SubstituteBroke) -> FlaggedActualDamage {
					return FlaggedActualDamage{
						ActualDamage::Known{other_pokemon.substitute().hp()},
						true
					};
				}
			));
			
			m_battle.handle_use_move(
				to_used_move(move, user_team, other_team, m_battle.weather()),
				status_clears,
				damage
			);
		});
	}

	auto cures_target_status(bool const is_ai, MoveName const move_name) -> bool final {
		return apply_to_team(is_ai, [&](auto const & target) {
			auto const pokemon = target.pokemon();
			return cures_target_status_impl(
				generation_,
				move_name,
				get_hidden_power_type(pokemon),
				pokemon.status().name()
			);
		});
	}

	auto determine_action() & -> MoveName final {
		if (m_battle.ai().size() == 0_bi or m_battle.foe().size() == 0_bi) {
			throw std::runtime_error("Tried to determine an action with an empty team.");
		}

		m_analysis_logger.get() << to_string(m_battle.ai()) << '\n';
		m_analysis_logger.get() << "Seen " << to_string(m_battle.foe()) << '\n';
		auto predicted = most_likely_team(m_usage_stats, m_random_engine, m_battle.foe());
		m_analysis_logger.get() << "Predicted " << to_string(predicted) << '\n' << std::flush;

		return expectiminimax(
			Team<generation_>(m_battle.ai()),
			predicted,
			m_battle.weather(),
			m_evaluate,
			Depth(m_depth, 1_bi),
			m_analysis_logger.get(),
			m_random_engine
		).name;
	}

	auto complete(Result const result) & -> void final {
		if (m_log_foe_teams and result == Result::lost) {
			log_foe_team(m_usage_stats, m_battle.foe(), m_random_engine);
		}
		m_completed = true;
	}

	auto completed() const -> bool final {
		return m_completed;
	}

	auto correct_hp(bool const is_ai, VisibleHP const visible_hp, TeamIndex const team_index) & -> void final {
		m_battle.correct_hp(is_ai, visible_hp, team_index);
	}
	auto correct_hp(bool const is_ai, VisibleHP const visible_hp) & -> void final {
		m_battle.correct_hp(is_ai, visible_hp);
	}

	auto correct_status(bool const is_ai, StatusName const status, TeamIndex const team_index) & -> void final {
		m_battle.correct_status(is_ai, status, team_index);
	}
	auto correct_status(bool const is_ai, StatusName const status) & -> void final {
		m_battle.correct_status(is_ai, status);
	}

	auto weather_is(NormalWeather const weather) const -> void final {
		auto const normal_weather = to_normal_weather(m_battle.weather());
		if (weather != normal_weather) {
			std::cerr << "Inconsistent weather. Received "sv << to_string(weather) << " but expected "sv << to_string(normal_weather) << '\n';
		}
	}

private:
	auto apply_to_teams(bool const is_ai_, auto function) const -> decltype(auto) {
		if (is_ai_) {
			return function(m_battle.ai(), m_battle.foe());
		} else {
			return function(m_battle.foe(), m_battle.ai());
		}
	}
	auto apply_to_team(bool const is_ai_, auto function) const -> decltype(auto) {
		return apply_to_teams(is_ai_, [&](auto const & team, auto const &) -> decltype(auto) { return function(team); });
	}

	auto compute_damage(bool const user_is_ai, MoveName const move, VisibleHP const hp) -> const HP::current_type {
		auto const ai_damaged = !user_is_ai xor (move == MoveName::Hit_Self);
		return apply_to_team(ai_damaged, [&](auto const & team) {
			auto const & pokemon = select_pokemon(team, move);
			auto const new_hp = ai_damaged ?
				hp.current.value() :
				to_real_hp(pokemon.hp().max(), hp).value;
			return hp_to_damage(pokemon, new_hp);
		});
	}

	UsageStats const & m_usage_stats;

	AnalysisLogger m_analysis_logger;
	std::mt19937 m_random_engine;

	Evaluate<generation_> m_evaluate;
	Battle<generation_> m_battle;
	DepthValues m_depth;

	bool m_log_foe_teams;
	bool m_completed = false;
	bool m_replacing_fainted = false;
};

} // namespace

auto make_battle_manager(
	AnalysisLogger analysis_logger,
	AllUsageStats const & usage_stats,
	AllEvaluate evaluate,
	DepthValues const depth,
	std::mt19937 random_engine,
	GenerationGeneric<Teams> generic_teams,
	bool const log_foe_teams
) -> std::unique_ptr<BattleManager> {
	return bounded::visit(std::move(generic_teams), [&]<Generation generation>(Teams<generation> && teams) -> std::unique_ptr<BattleManager> {
		return std::make_unique<BattleManagerImpl<generation>>(
			std::move(analysis_logger),
			usage_stats[generation],
			evaluate.get<generation>(),
			depth,
			std::move(random_engine),
			std::move(teams),
			log_foe_teams
		);
	});
}

} // namespace technicalmachine
