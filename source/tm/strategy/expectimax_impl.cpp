// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

#include <operators/forward.hpp>

module tm.strategy.expectimax;

import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.possible_executed_moves;
import tm.evaluate.scored_selection;
import tm.evaluate.selector;
import tm.evaluate.state;
import tm.evaluate.transposition;
import tm.evaluate.victory;
import tm.evaluate.win;

import tm.move.actual_damage;
import tm.move.call_move;
import tm.move.category;
import tm.move.future_selection;
import tm.move.irrelevant_action;
import tm.move.known_move;
import tm.move.legal_selections;
import tm.move.move_name;
import tm.move.other_action;
import tm.move.pass;
import tm.move.selection;
import tm.move.side_effects;
import tm.move.switch_;
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

import tm.strategy.weighted_selection;

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
import tm.team_is_empty;

import bounded;
import containers;
import std_module;
import tv;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

auto parallel_sum(auto && range) {
	return std::reduce(
		std::execution::par_unseq,
		containers::legacy_iterator(containers::begin(range)),
		containers::legacy_iterator(containers::end(range)),
		0.0
	);
}

template<Generation generation>
constexpr auto is_fainted(Team<generation> const & team) -> bool {
	return team.pokemon().hp().current() == 0_bi;
}

constexpr auto is_damaging(Selection const selection) -> bool {
	return tv::visit(selection, tv::overload(
		[](Switch) { return false; },
		[](MoveName const move) { return is_damaging(move); },
		[](Pass) { return false; }
	));
}

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
	MoveName executed;
};

constexpr auto paralysis_probability(StatusName const status) -> double {
	return status == StatusName::paralysis ? 0.25 : 0.0;
}

template<Generation generation>
auto execute_move(
	State<generation> const & state,
	Selector<generation> const select,
	SelectedAndExecuted const move,
	OtherAction const other_action,
	Depth const depth,
	auto const continuation
) -> double {
	auto const selected = select(state);
	auto const user_pokemon = selected.team.pokemon();
	auto const other_pokemon = selected.other.pokemon();
	auto const side_effects = possible_side_effects(move.executed, user_pokemon, selected.other, state.environment);
	auto const status = user_pokemon.status();
	auto const probability_of_clearing_status = status.probability_of_clearing(generation, user_pokemon.ability());
	auto const specific_chance_to_hit = chance_to_hit(
		user_pokemon,
		KnownMove(
			move.executed,
			move_type(generation, move.executed, get_hidden_power_type(user_pokemon))
		),
		other_pokemon,
		state.environment,
		other_pokemon.last_used_move().moved_this_turn()
	);
	auto const ch_probability = critical_hit_probability(user_pokemon, move.executed, other_pokemon.ability(), state.environment);
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
									move.executed,
									critical_hit,
									!hits,
									contact_ability_effect,
									side_effect.function
								),
								selected_copy.other,
								other_action,
								copy.environment,
								clear_status,
								ActualDamage::Unknown{},
								is_fully_paralyzed
							);
							if (auto const won = win(copy.ai, copy.foe)) {
								return *won + double(depth.general - 1_bi);
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

template<Generation generation>
auto execute_switch(State<generation> state, Selector<generation> const select, Switch const switch_, Depth const depth, auto const continuation) -> double {
	auto const selected = select(state);
	selected.team.switch_pokemon(
		selected.other.pokemon(),
		state.environment,
		switch_.value()
	);
	if (auto const won = win(state.ai, state.foe)) {
		return *won + double(depth.general - 1_bi);
	}
	return continuation(state);
}

struct OriginalPokemon {
	template<any_active_pokemon ActivePokemonType>
	OriginalPokemon(ActivePokemonType const pokemon, ActivePokemonType const other_pokemon, Selection const other_selection_):
		m_species(pokemon.species()),
		m_other_action(tv::visit(other_selection_, tv::overload(
			[](Switch) -> OtherAction {
				return IrrelevantAction();
			},
			[&](MoveName const move) -> OtherAction {
				return KnownMove(
					move,
					move_type(
						generation_from<ActivePokemonType>,
						move,
						get_hidden_power_type(other_pokemon)
					)
				);
			},
			[](Pass) -> OtherAction {
				// TODO: unreachable()?
				return IrrelevantAction();
			}
		)))
	{
	}
	
	auto is_same_pokemon(Species const species) const -> bool {
		return species == m_species;
	}
	auto other_action() const -> OtherAction {
		return m_other_action;
	}

private:
	Species m_species;
	OtherAction m_other_action;
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

constexpr auto remove_unlikely_foe_selections(WeightedSelections const foe_selections) {
	auto const filtered = containers::filter(
		foe_selections,
		[](WeightedSelection const value) {
			return value.weight > 0.05;
		}
	);
	auto const cummulative_weight = containers::sum(containers::transform(
		filtered,
		&WeightedSelection::weight
	));
	return WeightedSelections(containers::transform(
		filtered,
		[=](WeightedSelection const value) {
			return WeightedSelection(value.selection, value.weight / cummulative_weight);
		}
	));
}

template<Generation generation>
struct Evaluator {
	explicit Evaluator(Evaluate<generation> const evaluate, Strategy const & foe_strategy):
		m_evaluate(evaluate),
		m_foe_strategy(foe_strategy)
	{
	}

	auto select_type_of_action(
		State<generation> const & state,
		LegalSelections const ai_selections,
		auto const foe_selections,
		Depth const depth
	) -> ScoredSelections {
		BOUNDED_ASSERT(!team_is_empty(state.ai));
		BOUNDED_ASSERT(!team_is_empty(state.foe));

		if (auto const score = m_transposition_table.get_score(state, depth)) {
			return *score;
		}

		auto const actions = respond_to_foe_actions(
			state,
			ai_selections,
			foe_selections,
			[&](auto && ... args) {
				return order_branch(OPERATORS_FORWARD(args)..., depth);
			}
		);
		m_transposition_table.add_score(state, depth, actions);
		return actions;
	}

private:
	auto select_type_of_action(State<generation> const & state, Depth const depth) -> ScoredSelections {
		return select_type_of_action(
			state,
			get_legal_selections(state.ai, state.foe, state.environment),
			get_legal_selections(state.foe, state.ai, state.environment),
			depth
		);
	}

	auto order_branch(State<generation> const & state, Selection const ai_selection, Selection const foe_selection, Depth const depth) -> double {
		auto ordered = Order(state.ai, ai_selection, state.foe, foe_selection, state.environment);
		auto is_ai = team_matcher(state.ai);
		return !ordered ?
			(use_action_branch(state, Selector<generation>(true), ai_selection, foe_selection, depth) + use_action_branch(state, Selector<generation>(false), foe_selection, ai_selection, depth)) / 2.0 :
			use_action_branch(state, Selector<generation>(is_ai(ordered->first.team)), ordered->first.selection, ordered->second.selection, depth);
	}

	auto use_action_branch_inner(OtherAction const first_used_action, Selector<generation> const select, Depth const depth) {
		return [=, this](State<generation> const & state, Selection const ai_selection, Selection const foe_selection) {
			auto const [first_selection, last_selection] = sort_two(
				team_matcher(state.ai)(select(state).team),
				foe_selection,
				ai_selection
			);
			BOUNDED_ASSERT(first_selection == pass);
			return score_executed_actions(state, select, last_selection, first_used_action, depth, [&](State<generation> const & updated) {
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
										EndOfTurnFlags(foe_shed_skin, foe_lock_in_ends, foe_thaws),
										depth
									);
								}
							);
						}
					);
				});
			});
		};
	}

	auto use_action_branch_outer(OriginalPokemon const original_last_pokemon, Selector<generation> const select, Depth const depth) {
		return [=, this](State<generation> const & state, Selection const ai_selection, Selection const foe_selection) {
			auto const [first_selection, last_selection] = sort_two(
				team_matcher(state.ai)(select(state).team),
				ai_selection,
				foe_selection
			);
			return score_executed_actions(state, select, first_selection, FutureSelection(is_damaging(last_selection)), depth, [&](State<generation> const & pre_updated) {
				auto const pre_ordered = select(pre_updated);
				auto const is_same_pokemon = original_last_pokemon.is_same_pokemon(pre_ordered.other.pokemon().species());
				auto const actual_last_selection = is_same_pokemon ? last_selection : pass;
				auto const first_used_action = original_last_pokemon.other_action();
				return score_executed_actions(pre_updated, select.invert(), actual_last_selection, first_used_action, depth, [&](State<generation> const & updated) {
					constexpr auto first_selections = LegalSelections({pass});
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
					return max_score(respond_to_foe_actions(
						updated,
						ai_selections,
						foe_selections,
						use_action_branch_inner(first_used_action, select.invert(), depth)
					));
				});
			});
		};
	}

	auto use_action_branch(State<generation> const & state, Selector<generation> const select, Selection const first_selection, Selection const last_selection, Depth const depth) -> double {
		// For U-turn and Baton Pass, the user needs to make a new selection and
		// execute it before the other Pokemon acts. During that selection, the
		// other team's only legal selection at this point is Pass. We store the
		// original selected action and execute it after the Pokemon has acted
		// again. If the original action used was not U-turn or Baton Pass, the
		// only legal selection will be Pass and nothing will happen.
		//
		// We also need to do the right thing when the first Pokemon to act
		// phazes out the second Pokemon (for instance, because it was also
		// using a phaze move). To detect this case, we see if the Pokemon is
		// the same before and after the action, and if not, the second Pokemon
		// can only execute Pass.

		auto const ordering = select(state);
		auto const first_pokemon = ordering.team.pokemon();
		auto const last_pokemon = ordering.other.pokemon();
		auto const original_last_pokemon = OriginalPokemon(last_pokemon, first_pokemon, first_selection);

		auto function = [&](State<generation> const & updated) {
			auto const updated_ordering = select(updated);
			auto const first_selections =
				switch_decision_required(updated_ordering.team) ?
					get_legal_selections(
						updated_ordering.team,
						updated_ordering.other,
						updated.environment
					) :
					LegalSelections({pass});
			auto const last_selections = LegalSelections({last_selection});
			auto is_ai = team_matcher(updated.ai);
			auto const [ai_selections, foe_selections] = sort_two(is_ai(updated_ordering.team), first_selections, last_selections);
			return max_score(respond_to_foe_actions(
				updated,
				ai_selections,
				foe_selections,
				use_action_branch_outer(original_last_pokemon, select, depth)
			));
		};
		return score_executed_actions(
			state,
			select,
			first_selection,
			FutureSelection(is_damaging(last_selection)),
			depth,
			function
		);
	}

	auto end_of_turn_order_branch(State<generation> const & state, Faster<generation> const faster, EndOfTurnFlags const ai_flags, EndOfTurnFlags const foe_flags, Depth const depth) -> double {
		auto is_ai = team_matcher(state.ai);
		auto get_flag = [&](Team<generation> const & match) {
			return is_ai(match) ? ai_flags : foe_flags;
		};
		return !faster ?
			(end_of_turn_branch(state, Selector<generation>(true), ai_flags, foe_flags, depth) + end_of_turn_branch(state, Selector<generation>(false), foe_flags, ai_flags, depth)) / 2.0 :
			end_of_turn_branch(state, Selector<generation>(is_ai(faster->first)), get_flag(faster->first), get_flag(faster->second), depth);
	}

	auto end_of_turn_branch(State<generation> state, Selector<generation> const select, EndOfTurnFlags const first_flag, EndOfTurnFlags const last_flag, Depth const depth) -> double {
		auto const selected = select(state);
		end_of_turn(selected.team, first_flag, selected.other, last_flag, state.environment);
		if (is_fainted(state.ai) or is_fainted(state.foe)) {
			if (auto const won = win(state.ai, state.foe)) {
				return *won + double(depth.general);
			}
			return max_score(respond_to_foe_actions(
				state,
				get_legal_selections(state.ai, state.foe, state.environment),
				get_legal_selections(state.foe, state.ai, state.environment),
				[&](State<generation> const & updated, Selection const ai_selection, Selection const foe_selection) {
					return handle_end_of_turn_replacing(updated, select, ai_selection, foe_selection, depth);
				}
			));
		}
		return finish_end_of_turn(state, depth);
	}

	auto handle_end_of_turn_replacing(State<generation> state, Selector<generation> const select, Selection const ai_selection, Selection const foe_selection, Depth const depth) -> double {
		auto selected = select(state);
		auto const [first_selection, last_selection] = sort_two(
			team_matcher(state.ai)(selected.team),
			ai_selection,
			foe_selection
		);
		auto switch_one_side = [&](Selection const selection, Team<generation> & switcher, Team<generation> & other) {
			tv::visit(selection, tv::overload(
				[&](Switch const switch_) {
					switcher.switch_pokemon(other.pokemon(), state.environment, switch_.value());
				},
				[](MoveName) {
					std::unreachable();
				},
				[](Pass) {
				}
			));
		};
		switch_one_side(first_selection, selected.team, selected.other);
		switch_one_side(last_selection, selected.other, selected.team);
		if (auto const won = win(state.ai, state.foe)) {
			return *won + double(depth.general);
		}
		return finish_end_of_turn(state, depth);
	}

	auto finish_end_of_turn(State<generation> const & state, Depth const old_depth) -> double {
		auto const depth = one_level_deeper(old_depth);
		if (depth.general > 0_bi) {
			return max_score(select_type_of_action(state, depth));
		} else if (depth.single > 0_bi) {
			return generate_single_matchups(state, depth);
		} else {
			return static_cast<double>(m_evaluate(state.ai, state.foe));
		}
	}

	auto generate_single_matchups(State<generation> const & state, Depth const depth) -> double {
		auto score = 0.0;
		for (auto const ai_index : containers::integer_range(state.ai.size())) {
			for (auto const foe_index : containers::integer_range(state.foe.size())) {
				score += evaluate_single_matchup(state, ai_index, foe_index, depth);
			}
		}
		auto const difference = state.ai.size() - state.foe.size();
		score += static_cast<double>(difference) * victory<generation> * static_cast<double>(bounded::max(state.ai.size(), state.foe.size()));
		score /= static_cast<double>(max_pokemon_per_team * max_pokemon_per_team);
		return score;
	}

	auto evaluate_single_matchup(State<generation> state, TeamIndex const ai_index, TeamIndex const foe_index, Depth const depth) -> double {
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
			return *won + double(depth.general - 1_bi);
		}
		return max_score(select_type_of_action(state, depth));
	}


	auto score_executed_actions(
		State<generation> const & state,
		Selector<generation> const select,
		Selection const selection,
		OtherAction const other_action,
		Depth const depth,
		auto const continuation
	) const -> double {
		return tv::visit(selection, tv::overload(
			[&](Switch const switch_) -> double {
				return execute_switch(state, select, switch_, depth, continuation);
			},
			[&](MoveName const selected) -> double {
				auto const & team = select(state).team;
				if (team.pokemon().hp().current() == 0_bi) {
					return continuation(state);
				}
				double score = 0.0;
				auto const executed_moves = possible_executed_moves(selected, team);
				for (auto const executed : executed_moves) {
					score += execute_move(
						state,
						select,
						SelectedAndExecuted(selected, executed),
						other_action,
						depth,
						continuation
					);
				}
				return score / static_cast<double>(containers::size(executed_moves));
			},
			[&](Pass) -> double {
				return continuation(state);
			}
		));
	}

	auto respond_to_foe_actions(
		State<generation> const & state,
		LegalSelections const ai_selections,
		WeightedSelections const foe_selections,
		auto const function
	) const -> ScoredSelections {
		return ScoredSelections(containers::transform(ai_selections, [&](Selection const ai_selection) {
			return ScoredSelection(
				ai_selection,
				parallel_sum(containers::transform(
					foe_selections,
					[&](WeightedSelection const predicted) {
						return predicted.weight * function(state, ai_selection, predicted.selection);
					}
				))
			);
		}));
	}
	auto respond_to_foe_actions(
		State<generation> const & state,
		LegalSelections const ai_selections,
		LegalSelections const foe_selections,
		auto const function
	) const -> ScoredSelections {
		return respond_to_foe_actions(
			state,
			ai_selections,
			remove_unlikely_foe_selections(m_foe_strategy.get()(
				state.foe,
				foe_selections,
				state.ai,
				ai_selections,
				state.environment
			).user),
			std::move(function)
		);
	}

	Evaluate<generation> m_evaluate;
	std::reference_wrapper<Strategy const> m_foe_strategy;
	TranspositionTable<generation> m_transposition_table;
};

auto to_weighted_selections(ScoredSelections selections) -> WeightedSelections {
	auto const best = containers::max_element(
		selections,
		[](ScoredSelection const lhs, ScoredSelection const rhs) {
			return lhs.score > rhs.score;
		}
	);
	return WeightedSelections({
		WeightedSelection(best->selection, 1.0)
	});
}

} // namespace

auto make_expectimax(
	AllEvaluate all_evaluate,
	Depth const depth,
	Strategy foe_strategy_
) -> Strategy {
	return Strategy([=, foe_strategy = std::move(foe_strategy_)]<Generation generation>(
		Team<generation> const & ai,
		LegalSelections const ai_selections,
		Team<generation> const & foe,
		LegalSelections const foe_selections,
		Environment const environment
	) -> BothWeightedSelections {
		auto evaluator = Evaluator(
			all_evaluate.get<generation>(),
			foe_strategy
		);
		auto const predicted_foe_selections = foe_strategy(
			foe,
			foe_selections,
			ai,
			ai_selections,
			environment
		).user;
		auto const scored_selections = evaluator.select_type_of_action(
			State<generation>(ai, foe, environment),
			ai_selections,
			remove_unlikely_foe_selections(predicted_foe_selections),
			depth
		);
		return BothWeightedSelections(
			to_weighted_selections(scored_selections),
			predicted_foe_selections
		);
	});
}

} // namespace technicalmachine
