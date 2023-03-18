// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <compare>
#include <iostream>
#include <string_view>

export module tm.clients.make_battle_manager;

import tm.clients.battle_manager;
import tm.clients.check_weathers_match;
import tm.clients.log_foe_team;
import tm.clients.make_battle_manager_inputs;
import tm.clients.move_result;
import tm.clients.result;
import tm.clients.teams;
import tm.clients.to_used_move;
import tm.clients.turn_count;
import tm.clients.write_team;

import tm.evaluate.analysis_logger;
import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.predict_action;
import tm.evaluate.score_moves;
import tm.evaluate.scored_move;

import tm.move.actual_damage;
import tm.move.causes_recoil;
import tm.move.find_required_move_index;
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
import tm.block;
import tm.end_of_turn_flags;
import tm.gender;
import tm.generation;
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

auto const & select_pokemon(any_team auto const & team, MoveName const move) {
	return is_switch(move) ? team.pokemon(to_replacement(move)) : team.all_pokemon()();
}

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
struct BattleManagerImpl final : BattleManager {
	BattleManagerImpl(
		AnalysisLogger analysis_logger,
		tv::optional<WriteTeam> write_team,
		UsageStats const & usage_stats,
		Evaluate<generation_> evaluate,
		Depth const depth,
		std::mt19937 random_engine,
		Teams<generation_> teams
	):
		m_usage_stats(usage_stats),
		m_analysis_logger(std::move(analysis_logger)),
		m_write_team(std::move(write_team)),
		m_random_engine(std::move(random_engine)),
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
	auto random_engine() & -> std::mt19937 & final {
		return m_random_engine;
	}
	auto move_index(MoveName const move_name) const -> containers::index_type<RegularMoves> final {
		return find_required_move_index(m_battle.ai().pokemon().regular_moves(), move_name);
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
		m_analysis_logger.get() << containers::string(containers::repeat_n(20_bi, '=')) << "\nBegin turn " << turn_count << '\n';
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
				[&](Recharging) { return Used{user_team.pokemon().last_used_move().name()}; },
				[](auto) { return Used{MoveName::Struggle}; }
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
			
			auto const damage = tv::visit(move.damage, tv::overload(
				no_damage_function,
				[&](VisibleHP const hp) -> FlaggedActualDamage {
					auto const value = compute_damage(ai_is_user, move.executed, hp);
					return FlaggedActualDamage{
						ActualDamage::Known{value},
						value != 0_bi
					};
				},
				MoveHitSubstitute(other_pokemon.substitute())
			));
			
			m_battle.handle_use_move(
				to_used_move(move, user_team, other_team, m_battle.weather()),
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

		m_analysis_logger.get() << "AI's " << to_string(m_battle.ai()) << '\n';
		m_analysis_logger.get() << "Seen Foe's " << to_string(m_battle.foe()) << '\n';
		auto const predicted = most_likely_team(m_usage_stats, m_random_engine, m_battle.foe());
		m_analysis_logger.get() << "Predicted Foe's " << to_string(predicted) << '\n' << std::flush;

		m_analysis_logger.get() << "Evaluating to a depth of " << m_depth.general << ", " << m_depth.single << "...\n";
		auto const start = std::chrono::steady_clock::now();

		auto const ai = Team<generation_>(m_battle.ai());
		auto const predicted_selections = legal_selections(predicted, ai, m_battle.weather());
		auto const ai_selections = legal_selections(ai, predicted, m_battle.weather());

		auto foe_moves = predict_action(
			predicted,
			predicted_selections,
			ai,
			ai_selections,
			m_battle.weather(),
			m_evaluate
		);
		containers::sort(foe_moves, [](MoveProbability const lhs, MoveProbability const rhs) {
			return lhs.probability > rhs.probability;
		});
		log_foe_move_probabilities(foe_moves, predicted);

		auto scored_moves = score_moves(
			ai,
			ai_selections,
			predicted,
			predicted_selections,
			m_battle.weather(),
			m_evaluate,
			m_depth,
			foe_moves
		);
		auto const finish = std::chrono::steady_clock::now();
		m_analysis_logger.get() << "Scored moves in " << std::chrono::duration<double>(finish - start).count() << " seconds: ";
		containers::sort(scored_moves, [](ScoredMove const lhs, ScoredMove const rhs) {
			return lhs.score > rhs.score;
		});
		log_move_scores(scored_moves);
		m_analysis_logger.get() << std::flush;
		return containers::front(scored_moves).name;
	}

	auto complete(Result const result) & -> void final {
		if (m_write_team and result == Result::lost) {
			log_foe_team(m_usage_stats, m_battle.foe(), m_random_engine, *m_write_team);
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
		check_weathers_match(weather, m_battle.weather());
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

	auto compute_damage(bool const user_is_ai, MoveName const move, VisibleHP const hp) const -> CurrentHP {
		auto const ai_damaged = !user_is_ai xor (move == MoveName::Hit_Self);
		auto const old_hp = apply_to_team(ai_damaged, [&](auto const & team) {
			return select_pokemon(team, move).hp();
		});
		auto const new_hp = ai_damaged ?
			hp.current.value() :
			to_real_hp(old_hp.max(), hp).value;
		return hp_to_damage(old_hp.current(), new_hp);
	}

	auto log_move_scores(ScoredMoves const moves) & -> void {
		for (auto const move : moves) {
			if (is_switch(move.name)) {
				m_analysis_logger.get() << "Switch to " << to_string(m_battle.ai().pokemon(to_replacement(move.name)).species());
			} else {
				m_analysis_logger.get() << "Use " << to_string(move.name);
			}
			m_analysis_logger.get() << " for an expected score of " << static_cast<std::int64_t>(move.score) << '\n';
		}
	}
	auto log_foe_move_probabilities(MoveProbabilities const moves, Team<generation_> const & foe) & -> void {
		for (auto const move : moves) {
			m_analysis_logger.get() << "Predicted " << move.probability * 100.0 << "% chance of ";
			if (is_switch(move.name)) {
				m_analysis_logger.get() << "switching to " << to_string(foe.pokemon(to_replacement(move.name)).species());
			} else {
				m_analysis_logger.get() << "using " << to_string(move.name);
			}
			m_analysis_logger.get() << '\n';
		}
	}

	UsageStats const & m_usage_stats;

	AnalysisLogger m_analysis_logger;
	tv::optional<WriteTeam> m_write_team;

	std::mt19937 m_random_engine;

	Evaluate<generation_> m_evaluate;
	Battle<generation_> m_battle;
	Depth m_depth;

	bool m_completed = false;
};

// `usage_stats` must remain valid for the lifetime of the return value
export auto make_battle_manager(
	AnalysisLogger analysis_logger,
	tv::optional<WriteTeam> write_team,
	UsageStats const & usage_stats,
	GenerationGeneric<BattleManagerInputs> generic_inputs,
	Depth const depth,
	std::mt19937 random_engine
) -> std::unique_ptr<BattleManager> {
	return tv::visit(std::move(generic_inputs), [&]<Generation generation>(BattleManagerInputs<generation> && inputs) -> std::unique_ptr<BattleManager> {
		return std::make_unique<BattleManagerImpl<generation>>(
			std::move(analysis_logger),
			std::move(write_team),
			usage_stats,
			inputs.evaluate,
			depth,
			std::move(random_engine),
			std::move(inputs.teams)
		);
	});
}

} // namespace technicalmachine
