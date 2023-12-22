// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

#include <operators/forward.hpp>

export module tm.evaluate.evaluator;

import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.possible_executed_moves;
import tm.evaluate.scored_move;
import tm.evaluate.selector;
import tm.evaluate.state;
import tm.evaluate.team_is_empty;
import tm.evaluate.transposition;
import tm.evaluate.victory;
import tm.evaluate.win;

import tm.move.actual_damage;
import tm.move.call_move;
import tm.move.category;
import tm.move.is_switch;
import tm.move.known_move;
import tm.move.legal_selections;
import tm.move.move_name;
import tm.move.other_move;
import tm.move.side_effects;
import tm.move.used_move;

import tm.pokemon.any_pokemon;
import tm.pokemon.get_hidden_power_type;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.pokemon;
import tm.pokemon.pokemon_collection;
import tm.pokemon.species;

import tm.stat.chance_to_hit;
import tm.stat.faster;

import tm.status.clears_status;
import tm.status.status_name;

import tm.string_conversions.move_name;
import tm.string_conversions.species;

import tm.type.move_type;

import tm.ability;
import tm.any_team;
import tm.contact_ability_effect;
import tm.critical_hit_probability;
import tm.end_of_turn;
import tm.end_of_turn_flags;
import tm.environment;
import tm.generation;
import tm.get_legal_selections;
import tm.switch_decision_required;
import tm.team;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

constexpr double multi_generic_flag_branch(auto const & basic_probability, auto const & next_branch) {
	auto const probability = [=](bool const is_first, bool const flag) {
		auto const base = basic_probability(is_first);
		BOUNDED_ASSERT_OR_ASSUME(base >= 0.0);
		BOUNDED_ASSERT_OR_ASSUME(base <= 1.0);
		return flag ? base : (1.0 - base);
	};

	double average_score = 0.0;
	for (auto const first_flag : {true, false}) {
		auto const p1 = probability(true, first_flag);
		if (p1 == 0.0) {
			continue;
		}
		for (auto const last_flag : {true, false}) {
			auto const p2 = probability(false, last_flag);
			if (p2 == 0.0) {
				continue;
			}
			average_score += p1 * p2 * next_branch(first_flag, last_flag);
		}
	}
	return average_score;
}

constexpr double generic_flag_branch(double const basic_probability, auto const & next_branch) {
	BOUNDED_ASSERT_OR_ASSUME(0.0 <= basic_probability and basic_probability <= 1.0);
	double average_score = 0.0;
	for (auto const flag : {false, true}) {
		auto const probability = flag ? basic_probability : (1.0 - basic_probability);
		if (probability == 0.0) {
			continue;
		}
		average_score += probability * next_branch(flag);
	}
	return average_score;
}

struct SelectedAndExecuted {
	MoveName selected;
	KnownMove executed;
};

constexpr auto paralysis_probability(StatusName const status) -> double {
	return status == StatusName::paralysis ? 0.25 : 0.0;
}

template<Generation generation>
auto execute_move(State<generation> const & state, Selector<generation> const select, SelectedAndExecuted const move, OtherMove const other_move, auto const continuation) -> double {
	auto const selected = select(state);
	auto const user_pokemon = selected.team.pokemon();
	auto const other_pokemon = selected.other.pokemon();
	auto const side_effects = possible_side_effects(move.executed.name, user_pokemon, selected.other, state.environment);
	auto const status = user_pokemon.status();
	auto const probability_of_clearing_status = status.probability_of_clearing(generation, user_pokemon.ability());
	auto const specific_chance_to_hit = chance_to_hit(user_pokemon, move.executed, other_pokemon, state.environment, other_pokemon.last_used_move().moved_this_turn());
	auto const ch_probability = critical_hit_probability(user_pokemon, move.executed.name, other_pokemon.ability(), state.environment);
	auto const chance_to_be_paralyzed = paralysis_probability(user_pokemon.status().name());
	return generic_flag_branch(probability_of_clearing_status, [&](bool const clear_status) {
		return generic_flag_branch(specific_chance_to_hit, [&](bool const hits) {
			return generic_flag_branch(chance_to_be_paralyzed, [&](bool const is_fully_paralyzed) {
				auto score = 0.0;
				for (auto const & side_effect : side_effects) {
					score += side_effect.probability * generic_flag_branch(
						hits ? ch_probability : 0.0,
						[&](bool const critical_hit) {
							auto copy = state;
							auto const selected_copy = select(copy);
							// TODO: https://github.com/davidstone/technical-machine/issues/24
							constexpr auto contact_ability_effect = ContactAbilityEffect::nothing;
							call_move(
								selected_copy.team,
								UsedMove<Team<generation>>(
									move.selected,
									move.executed.name,
									critical_hit,
									!hits,
									contact_ability_effect,
									side_effect.function
								),
								selected_copy.other,
								other_move,
								copy.environment,
								clear_status,
								ActualDamage::Unknown{},
								is_fully_paralyzed
							);
							if (auto const won = win(copy.ai, copy.foe)) {
								return *won + double(state.depth.general - 1_bi);
							}
							return continuation(copy);
						}
					);
				}
				return score;
			});
		});
	});
}

struct OriginalPokemon {
	template<any_active_pokemon ActivePokemonType>
	OriginalPokemon(ActivePokemonType const pokemon, ActivePokemonType const other_pokemon, MoveName const other_move):
		m_species(pokemon.species()),
		m_other_move{
			other_move,
			move_type(generation_from<ActivePokemonType>, other_move, get_hidden_power_type(other_pokemon))
		}
	{
	}
	
	auto is_same_pokemon(Species const species) const {
		return species == m_species;
	}
	auto other_move() const {
		return m_other_move;
	}

private:
	Species m_species;
	KnownMove m_other_move;
};

template<Generation generation>
constexpr auto team_matcher(Team<generation> const & team) {
	return [&](Team<generation> const & match) {
		return std::addressof(team) == std::addressof(match);
	};
}

template<typename T>
struct pair {
	T first;
	T second;
};

template<typename T>
constexpr auto sort_two(bool const condition, T arg1, T arg2) {
	return condition ?
		pair<T>(std::move(arg1), std::move(arg2)) :
		pair<T>(std::move(arg2), std::move(arg1));
}

export template<Generation generation, typename Function>
struct Evaluator {
	explicit Evaluator(Evaluate<generation> const evaluate, Function respond_to_foe_moves):
		m_evaluate(evaluate),
		m_respond_to_foe_moves(std::move(respond_to_foe_moves))
	{
	}

	auto select_type_of_move(State<generation> const & state, LegalSelections const ai_selections, auto const foe_selections) -> ScoredMoves {
		BOUNDED_ASSERT(!team_is_empty(state.ai));
		BOUNDED_ASSERT(!team_is_empty(state.foe));

		if (auto const score = m_transposition_table.get_score(state)) {
			return *score;
		}

		auto const moves = m_respond_to_foe_moves(
			state,
			ai_selections,
			foe_selections,
			m_evaluate,
			[&](auto && ... args) {
				return order_branch(OPERATORS_FORWARD(args)...);
			}
		);
		m_transposition_table.add_score(state, moves);
		return moves;
	}

private:
	auto select_type_of_move(State<generation> const & state) -> ScoredMoves {
		return select_type_of_move(
			state,
			get_legal_selections(state.ai, state.foe, state.environment),
			get_legal_selections(state.foe, state.ai, state.environment)
		);
	}

	auto order_branch(State<generation> const & state, MoveName const ai_move, MoveName const foe_move) -> double {
		auto ordered = Order(state.ai, ai_move, state.foe, foe_move, state.environment);
		auto is_ai = team_matcher(state.ai);
		return !ordered ?
			(use_move_branch(state, Selector<generation>(true), ai_move, foe_move) + use_move_branch(state, Selector<generation>(false), foe_move, ai_move)) / 2.0 :
			use_move_branch(state, Selector<generation>(is_ai(ordered->first.team)), ordered->first.move, ordered->second.move);
	}

	auto use_move_branch_inner(KnownMove const first_used_move, Selector<generation> const select) {
		return [=, this](State<generation> const & state, MoveName const ai_move, MoveName const foe_move) {
			auto const [first_move, last_move] = sort_two(
				team_matcher(state.ai)(select(state).team),
				foe_move,
				ai_move
			);
			BOUNDED_ASSERT_OR_ASSUME(first_move == MoveName::Pass);
			return score_executed_moves(state, select, last_move, first_used_move, [&](State<generation> const & updated) {
				auto shed_skin_probability = [&](bool const is_ai) {
					auto const pokemon = (is_ai ? updated.ai : updated.foe).pokemon();
					return can_clear_status(pokemon.ability(), pokemon.status().name()) ? 0.3 : 0.0;
				};
				auto const teams = Faster<generation>(updated.ai, updated.foe, updated.environment);
				return multi_generic_flag_branch(shed_skin_probability, [&](bool const ai_shed_skin, bool const foe_shed_skin) {
					return multi_generic_flag_branch(
						// TODO
						[&](bool) { return true; },
						[&](bool const ai_lock_in_ends, bool const foe_lock_in_ends) {
							auto thaws = [&](bool const is_ai) {
								if constexpr (generation != Generation::two) {
									return 0.0;
								} else {
									auto const pokemon = (is_ai ? updated.ai : updated.foe).pokemon();
									return pokemon.status().name() == StatusName::freeze ? 0.1 : 0.0;
								}
							};
							return multi_generic_flag_branch(
								thaws,
								[&](bool const ai_thaws, bool const foe_thaws) {
									return end_of_turn_order_branch(
										updated,
										teams,
										EndOfTurnFlags(ai_shed_skin, ai_lock_in_ends, ai_thaws),
										EndOfTurnFlags(foe_shed_skin, foe_lock_in_ends, foe_thaws)
									);
								}
							);
						}
					);
				});
			});
		};
	}

	auto use_move_branch_outer(OriginalPokemon const original_last_pokemon, Selector<generation> const select) {
		return [=, this](State<generation> const & state, MoveName const ai_move, MoveName const foe_move) {
			auto const [first_move, last_move] = sort_two(
				team_matcher(state.ai)(select(state).team),
				ai_move,
				foe_move
			);
			return score_executed_moves(state, select, first_move, FutureMove{is_damaging(last_move)}, [&](State<generation> const & pre_updated) {
				auto const pre_ordered = select(pre_updated);
				auto const is_same_pokemon = original_last_pokemon.is_same_pokemon(pre_ordered.other.pokemon().species());
				auto const actual_last_move = is_same_pokemon ? last_move : MoveName::Pass;
				auto const first_used_move = original_last_pokemon.other_move();
				return score_executed_moves(pre_updated, select.invert(), actual_last_move, first_used_move, [&](State<generation> const & updated) {
					constexpr auto first_selections = LegalSelections({MoveName::Pass});
					auto const ordered = select(updated);
					auto const last_selections = get_legal_selections(
						ordered.other,
						ordered.team,
						updated.environment
					);
					auto const [ai_selections, foe_selections] = sort_two(
						team_matcher(updated.ai)(ordered.team),
						first_selections,
						last_selections
					);
					return max_score(m_respond_to_foe_moves(
						updated,
						ai_selections,
						foe_selections,
						m_evaluate,
						use_move_branch_inner(first_used_move, select.invert())
					));
				});
			});
		};
	}

	auto use_move_branch(State<generation> const & state, Selector<generation> const select, MoveName const first_move, MoveName const last_move) -> double {
		// This complicated section of code is designed to handle U-turn and Baton
		// Pass: The user of the move needs to go again before the other Pokemon
		// moves and make a new selection. During that selection, the opponent
		// cannot choose a move (which is why we say the only legal selection at
		// this point is Pass). We store the original selected move and execute it
		// after the Pokemon has moved again. In the event that the original move
		// used was not U-turn or Baton Pass, the only legal selection will be Pass
		// and nothing will happen.
		//
		// We also need to do the right thing when the first Pokemon to move phazes
		// out the second Pokemon (for instance, because it was also using a phaze
		// move). To detect this case, we see if the Pokemon is the same before and
		// after the move, and if so, the second Pokemon can only execute Pass.

		auto const ordering = select(state);
		auto const first_pokemon = ordering.team.pokemon();
		auto const last_pokemon = ordering.other.pokemon();
		auto const original_last_pokemon = OriginalPokemon(last_pokemon, first_pokemon, first_move);

		auto function = [&](State<generation> const & updated) {
			auto const updated_ordering = select(updated);
			auto const first_selections =
				switch_decision_required(updated_ordering.team) ?
					get_legal_selections(
						updated_ordering.team,
						updated_ordering.other,
						updated.environment
					) :
					LegalSelections({MoveName::Pass});
			auto const last_selections = LegalSelections({last_move});
			auto is_ai = team_matcher(updated.ai);
			auto const [ai_selections, foe_selections] = sort_two(is_ai(updated_ordering.team), first_selections, last_selections);
			return max_score(m_respond_to_foe_moves(
				updated,
				ai_selections,
				foe_selections,
				m_evaluate,
				use_move_branch_outer(original_last_pokemon, select)
			));
		};
		return score_executed_moves(
			state,
			select,
			first_move,
			FutureMove{is_damaging(last_move)},
			function
		);
	}

	auto end_of_turn_order_branch(State<generation> const & state, Faster<generation> const faster, EndOfTurnFlags const ai_flags, EndOfTurnFlags const foe_flags) -> double {
		auto is_ai = team_matcher(state.ai);
		auto get_flag = [&](Team<generation> const & match) {
			return is_ai(match) ? ai_flags : foe_flags;
		};
		return !faster ?
			(end_of_turn_branch(state, Selector<generation>(true), ai_flags, foe_flags) + end_of_turn_branch(state, Selector<generation>(false), foe_flags, ai_flags)) / 2.0 :
			end_of_turn_branch(state, Selector<generation>(is_ai(faster->first)), get_flag(faster->first), get_flag(faster->second));
	}

	auto end_of_turn_branch(State<generation> state, Selector<generation> const select, EndOfTurnFlags const first_flag, EndOfTurnFlags const last_flag) -> double {
		auto const selected = select(state);
		end_of_turn(selected.team, first_flag, selected.other, last_flag, state.environment);
		state.depth = one_level_deeper(state.depth);
		if (auto const won = win(state.ai, state.foe)) {
			return *won + double(state.depth.general);
		}
		auto is_fainted = [](Team<generation> const & team) {
			return team.pokemon().hp().current() == 0_bi;
		};
		if (is_fainted(state.ai) or is_fainted(state.foe)) {
			return max_score(m_respond_to_foe_moves(
				state,
				get_legal_selections(state.ai, state.foe, state.environment),
				get_legal_selections(state.foe, state.ai, state.environment),
				m_evaluate,
				[&](State<generation> const & updated, MoveName const ai_move, MoveName const foe_move) {
					return handle_end_of_turn_replacing(updated, select, ai_move, foe_move);
				}
			));
		}
		return finish_end_of_turn(state);
	}

	auto handle_end_of_turn_replacing(State<generation> state, Selector<generation> const select, MoveName const ai_move, MoveName const foe_move) -> double {
		auto selected = select(state);
		auto const [first_move, last_move] = sort_two(
			team_matcher(state.ai)(selected.team),
			ai_move,
			foe_move
		);
		if (first_move != MoveName::Pass) {
			selected.team.switch_pokemon(selected.other.pokemon(), state.environment, to_replacement(first_move));
		}
		if (last_move != MoveName::Pass) {
			selected.other.switch_pokemon(selected.team.pokemon(), state.environment, to_replacement(last_move));
		}
		if (auto const won = win(state.ai, state.foe)) {
			return *won + double(state.depth.general);
		}
		return finish_end_of_turn(state);
	}

	auto finish_end_of_turn(State<generation> const & state) -> double {
		if (state.depth.general > 0_bi) {
			return max_score(select_type_of_move(state));
		} else if (state.depth.single > 0_bi) {
			return generate_single_matchups(state);
		} else {
			return static_cast<double>(m_evaluate(state.ai, state.foe));
		}
	}

	auto generate_single_matchups(State<generation> const & state) -> double {
		auto score = 0.0;
		for (auto const ai_index : containers::integer_range(state.ai.size())) {
			for (auto const foe_index : containers::integer_range(state.foe.size())) {
				score += evaluate_single_matchup(state, ai_index, foe_index);
			}
		}
		auto const difference = state.ai.size() - state.foe.size();
		score += static_cast<double>(difference) * victory<generation> * static_cast<double>(bounded::max(state.ai.size(), state.foe.size()));
		score /= static_cast<double>(max_pokemon_per_team * max_pokemon_per_team);
		return score;
	}

	auto evaluate_single_matchup(State<generation> state, TeamIndex const ai_index, TeamIndex const foe_index) -> double {
		// TODO: Something involving switch order
		auto remove_all_but_index = [&](Team<generation> & team, TeamIndex const index, Team<generation> & other) {
			if (index != team.all_pokemon().index()) {
				team.switch_pokemon(other.pokemon(), state.environment, index);
			}
			team.all_pokemon() = PokemonCollection<Pokemon<generation>>({team.all_pokemon()(team.all_pokemon().index())});
		};
		remove_all_but_index(state.ai, ai_index, state.foe);
		remove_all_but_index(state.foe, foe_index, state.ai);
		if (auto const won = win(state.ai, state.foe)) {
			return *won + double(state.depth.general - 1_bi);
		}
		return max_score(select_type_of_move(state));
	}


	auto score_executed_moves(State<generation> const & state, Selector<generation> const select, MoveName const selected_move, OtherMove const other_move, auto const continuation) const -> double {
		double score = 0.0;
		auto const executed_moves = possible_executed_moves(selected_move, select(state).team);
		for (auto const executed_move : executed_moves) {
			score += execute_move(
				state,
				select,
				SelectedAndExecuted{selected_move, executed_move},
				other_move,
				continuation
			);
		}
		return score / static_cast<double>(containers::size(executed_moves));
	}

	Evaluate<generation> m_evaluate;
	[[no_unique_address]] Function m_respond_to_foe_moves;
	TranspositionTable<generation> m_transposition_table;
};

} // namespace technicalmachine
