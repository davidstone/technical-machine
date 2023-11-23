// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <iostream>
#include <string_view>

module tm.clients.make_client_battle;

import tm.clients.client_battle;
import tm.clients.check_weathers_match;
import tm.clients.client_battle_inputs;
import tm.clients.move_result;
import tm.clients.teams;
import tm.clients.to_used_move;
import tm.clients.turn_count;

import tm.evaluate.analysis_logger;
import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.move_probability;
import tm.evaluate.score_moves;
import tm.evaluate.scored_move;
import tm.evaluate.state;

import tm.move.actual_damage;
import tm.move.causes_recoil;
import tm.move.is_switch;
import tm.move.known_move;
import tm.move.move;
import tm.move.move_cures_target_status;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.pokemon.get_hidden_power_type;
import tm.pokemon.level;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.species;
import tm.pokemon.substitute;

import tm.stat.current_hp;
import tm.stat.hp;
import tm.stat.to_real_hp;

import tm.status.status_name;

import tm.string_conversions.move_name;
import tm.string_conversions.species;
import tm.string_conversions.team;
import tm.string_conversions.weather;

import tm.team_predictor.team_predictor;
import tm.team_predictor.usage_stats;

import tm.type.effectiveness;
import tm.type.move_type;

import tm.ability;
import tm.ability_blocks_move;
import tm.any_team;
import tm.battle;
import tm.end_of_turn_flags;
import tm.gender;
import tm.generation;
import tm.generation_generic;
import tm.get_both_actions;
import tm.item;
import tm.team;
import tm.visible_hp;
import tm.weather;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;
using namespace std::string_view_literals;

constexpr auto moved(any_team auto const & team) -> bool {
	auto const pokemon = team.pokemon();
	return pokemon.last_used_move().moved_this_turn() or pokemon.hp().current() == 0_bi;
};

constexpr auto hp_to_damage(CurrentHP const old_hp, CurrentHP const new_hp) -> CurrentHP {
	if (new_hp > old_hp) {
		std::cerr << "Took negative damage\n";
		return CurrentHP(0_bi);
		// throw std::runtime_error("Took negative damage");
	}
	return bounded::assume_in_range<CurrentHP>(old_hp - new_hp);
}

constexpr auto no_damage_function = [](NoDamage) -> FlaggedActualDamage {
	return FlaggedActualDamage{ActualDamage::Known{0_bi}, false};
};

struct MoveHitSubstitute {
	explicit constexpr MoveHitSubstitute(Substitute const other_substitute):
		m_other_substitute(other_substitute)
	{
	}
	constexpr auto operator()(SubstituteDamaged) const -> FlaggedActualDamage {
		if (!m_other_substitute) {
			throw std::runtime_error("Tried to damage a Substitute when the target does not have a Substitute");
		}
		return FlaggedActualDamage{
			ActualDamage::Capped{bounded::increase_min<0>(m_other_substitute.hp() - 1_bi)},
			true
		};
	}
	constexpr auto operator()(SubstituteBroke) const -> FlaggedActualDamage {
		return FlaggedActualDamage{
			ActualDamage::Known{m_other_substitute.hp()},
			true
		};
	}

private:
	Substitute m_other_substitute;
};

template<Generation generation_>
struct ClientBattleImpl final : ClientBattle {
	ClientBattleImpl(
		AnalysisLogger analysis_logger,
		UsageStats const & usage_stats,
		Evaluate<generation_> evaluate,
		Depth const depth,
		Teams<generation_> teams
	):
		m_usage_stats(usage_stats),
		m_analysis_logger(std::move(analysis_logger)),
		m_evaluate(evaluate),
		m_battle(
			std::move(teams).ai,
			std::move(teams).foe
		),
		m_depth(depth)
	{
	}

	auto generation() const -> Generation final {
		return generation_;
	}
	auto team() const -> GenerationGeneric<Team> final {
		return GenerationGeneric<Team>(Team<generation_>(m_battle.ai()));
	}

	auto ai_has(Species const species, std::string_view nickname, Level const level, Gender const gender) & -> MoveName final {
		return m_battle.find_ai_pokemon(species, nickname, level, gender);
	}
	auto foe_has(Species const species, std::string_view nickname, Level const level, Gender const gender) & -> MoveName final {
		return m_battle.find_or_add_foe_pokemon(species, nickname, level, gender);
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
		if (turn_count == 1_bi) {
			// TODO: properly order this
			m_battle.first_turn(true);
		}
		m_analysis_logger << containers::string(containers::repeat_n(20_bi, '=')) << "\nBegin turn " << turn_count << '\n';
		m_battle.handle_begin_turn();
	}
	auto end_turn(bool const ai_went_first, EndOfTurnFlags const first_flags, EndOfTurnFlags const last_flags) & -> void final {
		m_battle.handle_end_turn(ai_went_first, first_flags, last_flags);
	}

	auto use_move(bool const ai_is_user, MoveResult const move_result, bool const status_clears) & -> void final {
		// TODO: Handle the other states better
		apply_to_teams(ai_is_user, [&](auto const & user_team, auto const & other_team) {
			auto const move = tv::visit(move_result, tv::overload(
				[](Used const used) { return used; },
				[&](Recharging) { return Used(user_team.pokemon().last_used_move().name()); },
				[](auto) { return Used(MoveName::Struggle); }
			));
			active_has(ai_is_user, move.selected);
			if (move.selected == MoveName::Sleep_Talk) {
				active_has(ai_is_user, move.executed);
			}
			auto const user_hidden_power_type = get_hidden_power_type(user_team.pokemon());
			auto const type = move_type(generation_, move.executed, user_hidden_power_type);
			auto const other_pokemon = other_team.pokemon();
			auto const ability_blocks_recoil =
				causes_recoil(move.executed) and
				!move.recoil and
				!Effectiveness(generation_, type, other_pokemon.types()).has_no_effect() and
				!ability_blocks_move(
					generation_,
					other_pokemon.ability(),
					KnownMove{
						move.executed,
						type
					},
					other_pokemon.status().name(),
					other_pokemon.types()
				);
			if (ability_blocks_recoil) {
				// TODO: This could also be Magic Guard
				active_has(ai_is_user, Ability::Rock_Head);
			}
			
			auto const damage = visible_damage_to_actual_damage(
				ai_is_user,
				move,
				other_pokemon.substitute()
			);
			
			m_battle.handle_use_move(
				to_used_move(move, user_team, other_team, m_battle.environment()),
				status_clears,
				move_result.index() == bounded::type<FullyParalyzed>,
				damage
			);
		});
	}

	auto cures_target_status(bool const is_ai, MoveName const move_name) -> bool final {
		return apply_to_team(is_ai, [&](auto const & target) {
			auto const pokemon = target.pokemon();
			return move_cures_target_status(
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

		log_team("AI", m_battle.ai());
		auto const state = predicted_state();
		log_team("Predicted Foe", state.foe);
		m_analysis_logger << std::flush;

		m_analysis_logger << "Evaluating to a depth of " << state.depth.general << ", " << state.depth.single << "...\n";
		auto const start = std::chrono::steady_clock::now();

		auto [foe_moves, ai_selections] = get_both_actions(
			state.foe,
			state.ai,
			state.environment,
			m_evaluate
		);
		containers::sort(foe_moves, [](MoveProbability const lhs, MoveProbability const rhs) {
			return lhs.probability > rhs.probability;
		});
		log_foe_move_probabilities(foe_moves, state.foe);

		auto scored_moves = score_moves(
			state,
			ai_selections,
			foe_moves,
			m_evaluate
		);
		auto const finish = std::chrono::steady_clock::now();
		m_analysis_logger << "Scored moves in " << std::chrono::duration<double>(finish - start).count() << " seconds: ";
		containers::sort(scored_moves, [](ScoredMove const lhs, ScoredMove const rhs) {
			return lhs.score > rhs.score;
		});
		log_move_scores(scored_moves);
		m_analysis_logger << std::flush;
		return containers::front(scored_moves).name;
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

	auto weather_is(Weather const weather) const -> void final {
		check_weathers_match(weather, m_battle.environment());
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
		if (is_ai_) {
			return function(m_battle.ai());
		} else {
			return function(m_battle.foe());
		}
	}

	auto target_hp(bool const damaged_is_ai) const -> HP {
		return damaged_is_ai ?
			m_battle.ai().pokemon().hp() :
			m_battle.foe().pokemon().hp();
	}

	auto predicted_state() const -> State<generation_> {
		return State<generation_>(
			Team<generation_>(m_battle.ai()),
			most_likely_team(m_usage_stats, m_battle.foe()),
			m_battle.environment(),
			m_depth
		);
	}

	auto visible_damage_to_actual_damage(bool const user_is_ai, Used const move, Substitute const other_substitute) const -> FlaggedActualDamage {
		return tv::visit(move.damage, tv::overload(
			no_damage_function,
			[&](VisibleHP const hp) -> FlaggedActualDamage {
				auto const damaged_is_ai = !user_is_ai xor (move.executed == MoveName::Hit_Self);
				auto const old_hp = target_hp(damaged_is_ai);
				auto const new_hp = damaged_is_ai ?
					hp.current.value() :
					to_real_hp(old_hp.max(), hp).value;
				auto const value = hp_to_damage(old_hp.current(), new_hp);
				return FlaggedActualDamage{
					ActualDamage::Known{value},
					value != 0_bi
				};
			},
			MoveHitSubstitute(other_substitute)
		));
	}

	auto log_team(std::string_view const label, auto const & team) & -> void {
		m_analysis_logger << label << "'s " << to_string(team) << '\n';
	}

	auto log_move_scores(ScoredMoves const moves) & -> void {
		for (auto const move : moves) {
			if (is_switch(move.name)) {
				m_analysis_logger << "Switch to " << to_string(m_battle.ai().pokemon(to_replacement(move.name)).species());
			} else {
				m_analysis_logger << "Use " << to_string(move.name);
			}
			m_analysis_logger << " for an expected score of " << static_cast<std::int64_t>(move.score) << '\n';
		}
	}
	auto log_foe_move_probabilities(MoveProbabilities const moves, Team<generation_> const & foe) & -> void {
		for (auto const move : moves) {
			m_analysis_logger << "Predicted " << move.probability * 100.0 << "% chance of ";
			if (is_switch(move.name)) {
				m_analysis_logger << "switching to " << to_string(foe.pokemon(to_replacement(move.name)).species());
			} else {
				m_analysis_logger << "using " << to_string(move.name);
			}
			m_analysis_logger << '\n';
		}
	}

	UsageStats const & m_usage_stats;

	AnalysisLogger m_analysis_logger;

	Evaluate<generation_> m_evaluate;
	Battle<generation_> m_battle;
	Depth m_depth;
};

// `usage_stats` must remain valid for the lifetime of the return value
auto make_client_battle(
	AnalysisLogger analysis_logger,
	UsageStats const & usage_stats,
	GenerationGeneric<ClientBattleInputs> generic_inputs,
	Depth const depth
) -> std::unique_ptr<ClientBattle> {
	return tv::visit(std::move(generic_inputs), [&]<Generation generation>(ClientBattleInputs<generation> && inputs) -> std::unique_ptr<ClientBattle> {
		return std::make_unique<ClientBattleImpl<generation>>(
			std::move(analysis_logger),
			usage_stats,
			inputs.evaluate,
			depth,
			std::move(inputs.teams)
		);
	});
}

} // namespace technicalmachine
