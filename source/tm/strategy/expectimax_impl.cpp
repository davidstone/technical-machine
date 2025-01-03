// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

#include <operators/forward.hpp>

module tm.strategy.expectimax;

import tm.evaluate.all_evaluate;
import tm.evaluate.compressed_battle;
import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.possible_executed_moves;
import tm.evaluate.score;
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

import tm.pokemon.active_pokemon;
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

import tm.strategy.selection_probability;

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
import tm.probability;
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
		Score(0.0)
	);
}

constexpr auto is_fainted(any_team auto const & team) -> bool {
	return team.pokemon().hp().current() == 0_bi;
}

constexpr auto is_damaging(Selection const selection) -> bool {
	return tv::visit(selection, tv::overload(
		[](Switch) { return false; },
		[](MoveName const move) { return is_damaging(move); },
		[](Pass) { return false; }
	));
}

struct FlagProbability {
	bool flag;
	Probability probability;
};

constexpr auto probabilities(auto const basic_probability, auto... args) {
	return containers::filter(
		containers::transform(
			containers::array({true, false}),
			[=](bool const flag) {
				auto const base = basic_probability(args...);
				return FlagProbability(
					flag,
					flag ? base : Probability(1.0) - base
				);
			}
		),
		[](FlagProbability const x) { return x.probability != Probability(0.0); }
	);
};

constexpr auto multi_generic_flag_branch(auto const & basic_probability, auto const & next_branch) -> Score {
	auto average_score = Score(0.0);
	auto const first_probabilities = probabilities(basic_probability, true);
	auto const last_probabilities = containers::make_static_vector(probabilities(basic_probability, false));
	for (auto const first : first_probabilities) {
		for (auto const last : last_probabilities) {
			average_score += first.probability * last.probability * next_branch(first.flag, last.flag);
		}
	}
	return average_score;
}

constexpr auto generic_flag_branch(Probability const basic_probability, auto const & next_branch) -> Score {
	return containers::sum(containers::transform(
		probabilities(bounded::value_to_function(basic_probability)),
		[&](FlagProbability const x) { return x.probability * next_branch(x.flag); }
	));
}

constexpr auto moved(any_active_pokemon auto const pokemon) -> bool {
	return pokemon.last_used_move().moved_this_turn();
}
constexpr auto moved(any_team auto const & team) -> bool {
	return moved(team.pokemon());
}

template<Generation generation>
constexpr auto replacement_before_end_of_turn_required(State<generation> const & state) -> bool {
	if constexpr (generation != Generation::two) {
		return false;
	} else {
		return
			(moved(state.ai) or moved(state.foe)) and
			(is_fainted(state.ai) or is_fainted(state.foe));
	}
}

template<Generation generation>
constexpr auto fainting_forces_end_of_turn(
	State<generation> const & state,
	Selector const selector
) -> bool {
	auto const selected = selector(state);
	BOUNDED_ASSERT(moved(selected.other));
	switch (generation) {
		case Generation::one:
		case Generation::two:
		case Generation::three:
			return is_fainted(state.ai) or is_fainted(state.foe);
		default:
			return is_fainted(selected.selection);
	}
}

struct SelectedAndExecuted {
	MoveName selected;
	MoveName executed;
};

constexpr auto paralysis_probability(StatusName const status) -> Probability {
	return status == StatusName::paralysis ? Probability(0.25) : Probability(0.0);
}

template<Generation generation>
auto execute_move(
	State<generation> const & state,
	Selector const select,
	SelectedAndExecuted const move,
	OtherAction const other_action,
	Depth const depth,
	auto const continuation
) -> Score {
	auto const selected = select(state);
	auto const user_pokemon = selected.selection.pokemon();
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
		moved(other_pokemon)
	);
	auto const ch_probability = critical_hit_probability(user_pokemon, move.executed, other_pokemon.ability(), state.environment);
	auto const chance_to_be_paralyzed = paralysis_probability(user_pokemon.status().name());
	auto const action_end_probability = user_pokemon.last_used_move().action_end_probability();
	return generic_flag_branch(probability_of_clearing_status, [&](bool const clear_status) {
		return generic_flag_branch(specific_chance_to_hit, [&](bool const hits) {
			return generic_flag_branch(chance_to_be_paralyzed, [&](bool const is_fully_paralyzed) {
				return generic_flag_branch(action_end_probability, [&](bool const action_ends) {
					return containers::sum(containers::transform(
						side_effects,
						[&](auto const & side_effect) {
							return side_effect.probability * generic_flag_branch(
								hits ? ch_probability : Probability(0.0),
								[&](bool const critical_hit) {
									auto copy = state;
									auto const selected_copy = select(copy);
									// TODO: https://github.com/davidstone/technical-machine/issues/24
									constexpr auto contact_ability_effect = ContactAbilityEffect::nothing;
									call_move(
										selected_copy.selection,
										UsedMove<Team<generation>>(
											move.selected,
											move.executed,
											critical_hit,
											!hits,
											action_ends,
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
										return *won + Score(double(depth.remaining_general()));
									}
									return continuation(copy);
								}
							);
						}
					));
				});
			});
		});
	});
}

template<Generation generation>
auto execute_switch(State<generation> state, Selector const select, Switch const switch_, Depth const depth, auto const continuation) -> Score {
	auto const selected = select(state);
	selected.selection.switch_pokemon(
		selected.other.pokemon(),
		state.environment,
		switch_.value()
	);
	if (auto const won = win(state.ai, state.foe)) {
		return *won + Score(double(depth.remaining_general()));
	}
	return continuation(state);
}

template<Generation generation>
constexpr auto get_known_move(
	ActivePokemon<generation> const pokemon,
	MoveName const move
) {
	return KnownMove(
		move,
		move_type(generation, move, get_hidden_power_type(pokemon))
	);
}

template<Generation generation>
constexpr auto team_matcher(Team<generation> const & team) {
	return [&](Team<generation> const & match) {
		return std::addressof(team) == std::addressof(match);
	};
}

constexpr auto is_pass(LegalSelections const selections) -> bool {
	return selections == LegalSelections({pass});
}
constexpr auto is_pass(SelectionProbabilities const selections) -> bool {
	return selections == SelectionProbabilities({{pass, Probability(1.0)}});
}

constexpr auto check_is_valid_start_of_turn(any_team auto const & team) -> void {
	BOUNDED_ASSERT(!team_is_empty(team));
	auto const pokemon = team.pokemon();
	BOUNDED_ASSERT(!moved(pokemon));
	BOUNDED_ASSERT(pokemon.hp().current() != 0_bi);
}

constexpr auto average(auto... values) -> Score {
	return (... + values) / double(sizeof...(values));
}

constexpr auto is_delayed_switching(any_team auto const & team) -> bool {
	return team.size() != 1_bi and team.pokemon().last_used_move().is_delayed_switching();
}

template<Generation generation>
auto legal_selections_after_delayed_switch(
	Team<generation> const & old_team,
	Team<generation> const & old_other,
	Environment const old_environment
) -> LegalSelections {
	return LegalSelections(containers::filter(
		get_legal_selections(old_team, old_other, old_environment),
		[&](Selection const selection) {
			// TODO: This should be the last selected, not the last executed
			auto const other_selection = *old_other.pokemon().last_used_move().name();
			auto const order = Order(
				old_team,
				selection,
				old_other,
				other_selection,
				old_environment
			);
			if (!order) {
				return true;
			}
			return std::addressof(old_team) == std::addressof(order->second.team);
		}
	));
}

constexpr auto get_other_action(any_active_pokemon auto const pokemon) -> OtherAction {
	auto const move = pokemon.last_used_move().name();
	return move ?
		OtherAction(get_known_move(pokemon, *move)) :
		OtherAction(IrrelevantAction());
}

template<Generation generation>
struct Evaluator {
	explicit Evaluator(
		Evaluate<generation> const evaluate,
		Strategy const & foe_strategy,
		Depth const depth
	):
		m_evaluate(evaluate),
		m_foe_strategy(foe_strategy),
		m_transposition_table(depth > Depth(1_bi, 1_bi) ?
			std::make_unique<TranspositionTable<generation>>() :
			nullptr
		)
	{
	}

	auto select_type_of_action(
		State<generation> const & state,
		LegalSelections const ai_selections,
		SelectionProbabilities const foe_selections,
		Depth const depth
	) -> ScoredSelections {
		BOUNDED_ASSERT(!team_is_empty(state.ai));
		BOUNDED_ASSERT(!team_is_empty(state.foe));
		if (is_delayed_switching(state.ai)) {
			BOUNDED_ASSERT(is_pass(foe_selections));
			return middle_of_turn_action(state, ai_selections, foe_selections, depth);
		} else if (replacement_before_end_of_turn_required(state)) {
			BOUNDED_ASSERT(is_fainted(state.ai));
			return before_end_of_turn_action(state, ai_selections, foe_selections, depth);
		} else if (is_fainted(state.ai) or is_fainted(state.foe)) {
			return after_end_of_turn_action(state, ai_selections, foe_selections, depth);
		} else {
			return start_of_turn_action(state, ai_selections, foe_selections, depth);
		}
	}

private:
	auto start_of_turn_action(
		State<generation> const & state,
		LegalSelections const ai_selections,
		SelectionProbabilities const foe_selections,
		Depth const depth
	) -> ScoredSelections {
		check_is_valid_start_of_turn(state.ai);
		check_is_valid_start_of_turn(state.foe);

		auto const compressed_battle = compress_battle(state);
		if (m_transposition_table) {
			if (auto const score = m_transposition_table->get_score(compressed_battle, depth)) {
				return *score;
			}
		}

		auto const new_depth = depth.reduced(containers::size(ai_selections));
		auto const actions = score_selections(
			ai_selections,
			foe_selections,
			[&](Selection const ai_selection, Selection const foe_selection) {
				return order_branch(state, ai_selection, foe_selection, new_depth);
			}
		);
		if (m_transposition_table) {
			m_transposition_table->add_score(compressed_battle, depth, actions);
		}
		return actions;
	}

	auto start_of_turn_action(State<generation> const & state, Depth const depth) -> ScoredSelections {
		auto const ai_selections = get_legal_selections(state.ai, state.foe, state.environment);
		return start_of_turn_action(
			state,
			ai_selections,
			get_foe_selections(state, ai_selections),
			depth
		);
	}

	auto middle_of_turn_action(
		State<generation> const & original,
		LegalSelections const ai_selections,
		SelectionProbabilities const foe_selections,
		Depth const depth,
		tv::optional<Selection> const forced_continuation = tv::none
	) -> ScoredSelections {
		auto const compressed_battle = compress_battle(original);
		if (m_transposition_table) {
			if (auto const score = m_transposition_table->get_score(compressed_battle, depth)) {
				return *score;
			}
		}
		auto const new_depth = depth.reduced(containers::size(ai_selections));
		auto const actions = score_selections(
			ai_selections,
			foe_selections,
			[&](Selection const ai_selection, Selection const foe_selection) {
				BOUNDED_ASSERT(ai_selection == pass xor foe_selection == pass);
				auto const is_ai = foe_selection == pass;
				auto const selector = Selector(is_ai);
				auto const selection = is_ai ? ai_selection : foe_selection;
				auto continuation = [&](State<generation> const & updated) -> Score {
					auto const selected = selector(updated);
					auto const should_end_turn =
						moved(selected.other) or
						is_fainted(selected.other) or
						(generation <= Generation::three and is_fainted(selected.selection));
					if (should_end_turn) {
						return end_of_turn_flag_branch(updated, new_depth);
					} else {
						return middle_of_turn_after_switch(
							original,
							updated,
							selector.invert(),
							new_depth,
							forced_continuation
						);
					}
				};
				return tv::visit(selection, tv::overload(
					[&](Switch const switch_) -> Score {
						return execute_switch(original, selector, switch_, depth, continuation);
					},
					[](MoveName) -> Score {
						std::unreachable();
					},
					[](Pass) -> Score {
						std::unreachable();
					}
				));
			}
		);

		if (m_transposition_table) {
			m_transposition_table->add_score(compressed_battle, depth, actions);
		}
		return actions;
	}

	auto middle_of_turn_after_switch(
		State<generation> const & old,
		State<generation> const & state,
		Selector const selector,
		Depth const depth,
		tv::optional<Selection> const forced_continuation
	) -> Score {
		BOUNDED_ASSERT(!moved(selector(state).selection));
		BOUNDED_ASSERT(!is_fainted(selector(state).selection));
		BOUNDED_ASSERT(moved(selector(state).other));
		auto const old_select = selector(old);
		auto const last_selections = forced_continuation ?
			LegalSelections({*forced_continuation}) :
			legal_selections_after_delayed_switch(
				old_select.selection,
				old_select.other,
				old.environment
			);
		auto const first_selections = LegalSelections({pass});
		auto const [ai_selections, foe_selections] = selector(
			last_selections,
			first_selections
		);
		auto const new_depth = depth.reduced(containers::size(ai_selections));
		return max_score(score_selections(
			ai_selections,
			get_foe_selections(state, ai_selections, foe_selections),
			[&](Selection const ai_selection, Selection const foe_selection) {
				auto const [selection, other] = selector(ai_selection, foe_selection);
				BOUNDED_ASSERT(other == pass);
				return score_executed_actions(
					state,
					selector,
					selection,
					get_other_action(old_select.other.pokemon()),
					depth,
					[&](State<generation> const & updated) {
						return end_of_turn_flag_branch(updated, new_depth);
					}
				);
			}
		));
	}

	auto replace_fainted_action(
		State<generation> const & original,
		LegalSelections const ai_selections,
		SelectionProbabilities const foe_selections,
		auto const continuation
	) -> ScoredSelections {
		BOUNDED_ASSERT(is_fainted(original.ai) or is_pass(ai_selections));
		BOUNDED_ASSERT(is_fainted(original.foe) or is_pass(foe_selections));
		return score_selections(
			ai_selections,
			foe_selections,
			[&](Selection const ai_selection, Selection const foe_selection) {
				auto updated = original;
				// TODO: How does turn order matter here?
				auto switch_one_side = [&](Selection const selection, Team<generation> & switcher, Team<generation> & other) {
					tv::visit(selection, tv::overload(
						[&](Switch const switch_) {
							switcher.switch_pokemon(other.pokemon(), updated.environment, switch_.value());
						},
						[](MoveName) {
							std::unreachable();
						},
						[](Pass) {
						}
					));
				};
				switch_one_side(ai_selection, updated.ai, updated.foe);
				switch_one_side(foe_selection, updated.foe, updated.ai);
				return continuation(updated);
			}
		);
	}

	auto before_end_of_turn_action(
		State<generation> const & original,
		LegalSelections const ai_selections,
		SelectionProbabilities const foe_selections,
		Depth const depth
	) -> ScoredSelections {
		if constexpr (generation != Generation::two) {
			std::unreachable();
		} else {
			auto const new_depth = depth.reduced(containers::size(ai_selections));
			return replace_fainted_action(
				original,
				ai_selections,
				foe_selections,
				[&](State<generation> const & updated) {
					return end_of_turn_flag_branch(updated, new_depth);
				}
			);
		}
	}

	auto after_end_of_turn_action(
		State<generation> const & original,
		LegalSelections const ai_selections,
		SelectionProbabilities const foe_selections,
		Depth const depth
	) -> ScoredSelections {
		auto const new_depth = depth.reduced(containers::size(ai_selections));
		return replace_fainted_action(
			original,
			ai_selections,
			foe_selections,
			[&](State<generation> const & updated) {
				return finish_end_of_turn(updated, new_depth);
			}
		);
	}

	auto order_branch(
		State<generation> const & state,
		Selection const ai_selection,
		Selection const foe_selection,
		Depth const depth
	) -> Score {
		auto ordered = Order(
			state.ai,
			ai_selection,
			state.foe,
			foe_selection,
			state.environment
		);
		return !ordered ?
			average(
				use_action_branch(
					state,
					Selector(true),
					ai_selection,
					foe_selection,
					depth
				),
				use_action_branch(
					state,
					Selector(false),
					foe_selection,
					ai_selection,
					depth
				)
			) :
			use_action_branch(
				state,
				Selector(team_matcher(state.ai)(ordered->first.team)),
				ordered->first.selection,
				ordered->second.selection,
				depth
			);
	}

	auto use_action_branch(
		State<generation> const & original,
		Selector const selector,
		Selection const first_selection,
		Selection const last_selection,
		Depth const depth
	) -> Score {
		return score_executed_actions(
			original,
			selector,
			first_selection,
			FutureSelection(is_damaging(last_selection)),
			depth,
			[&](State<generation> const & after_first) {
				return after_action_branch(after_first, selector, depth, last_selection);
			}
		);
	}

	auto after_action_branch(
		State<generation> const & state,
		Selector const selector,
		Depth const depth,
		tv::optional<Selection> const possible_forced_continuation
	) -> Score {
		auto const selected = selector(state);
		if (is_delayed_switching(state.ai)) {
			auto const ai_selections = get_legal_selections(
				state.ai,
				state.foe,
				state.environment
			);
			auto const foe_selections = SelectionProbabilities({{pass, Probability(1.0)}});
			return max_score(middle_of_turn_action(
				state,
				ai_selections,
				foe_selections,
				depth
			));
		} else if (is_delayed_switching(state.foe)) {
			auto const ai_selections = LegalSelections({pass});
			auto const foe_selections = get_foe_selections(state, ai_selections);
			return max_score(middle_of_turn_action(
				state,
				ai_selections,
				foe_selections,
				depth,
				possible_forced_continuation
			));
		} else if (replacement_before_end_of_turn_required(state)) {
			auto const ai_selections = LegalSelections({pass});
			auto const foe_selections = get_foe_selections(state, ai_selections);
			return max_score(before_end_of_turn_action(
				state,
				ai_selections,
				foe_selections,
				depth
			));
		} else if (moved(selected.other) or fainting_forces_end_of_turn(state, selector.invert())) {
			return end_of_turn_flag_branch(state, depth);
		} else {
			return score_executed_actions(
				state,
				selector.invert(),
				*possible_forced_continuation,
				get_other_action(selected.selection.pokemon()),
				depth,
				[&](State<generation> const & updated) {
					return after_action_branch(updated, selector, depth, tv::none);
				}
			);
		}
	}

	auto end_of_turn_flag_branch(
		State<generation> const & state,
		Depth const depth
	) -> Score {
		auto shed_skin_probability = [&](bool const is_ai) {
			auto const pokemon = (is_ai ? state.ai : state.foe).pokemon();
			return can_clear_status(pokemon.ability(), pokemon.status().name()) ? Probability(0.3) : Probability(0.0);
		};
		auto const teams = Faster<generation>(state.ai, state.foe, state.environment);
		return multi_generic_flag_branch(shed_skin_probability, [&](bool const ai_shed_skin, bool const foe_shed_skin) {
			return multi_generic_flag_branch(
				[&](bool const is_ai) {
					auto const pokemon = (is_ai ? state.ai : state.foe).pokemon();
					return pokemon.last_used_move().end_of_turn_end_probability();
				},
				[&](bool const ai_lock_in_ends, bool const foe_lock_in_ends) {
					auto thaws = [&](bool const is_ai) {
						if constexpr (generation == Generation::two) {
							auto const pokemon = (is_ai ? state.ai : state.foe).pokemon();
							return pokemon.status().name() == StatusName::freeze ? Probability(0.1) : Probability(0.0);
						} else {
							return Probability(0.0);
						}
					};
					return multi_generic_flag_branch(
						thaws,
						[&](bool const ai_thaws, bool const foe_thaws) {
							return end_of_turn_order_branch(
								state,
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
	}
	
	auto end_of_turn_order_branch(
		State<generation> const & state,
		Faster<generation> const faster,
		EndOfTurnFlags const ai_flags,
		EndOfTurnFlags const foe_flags,
		Depth const depth
	) -> Score {
		auto is_ai = team_matcher(state.ai);
		auto get_flag = [&](Team<generation> const & match) {
			return is_ai(match) ? ai_flags : foe_flags;
		};
		return !faster ?
			average(
				end_of_turn_branch(
					state,
					Selector(true),
					ai_flags,
					foe_flags,
					depth
				),
				end_of_turn_branch(
					state,
					Selector(false),
					foe_flags,
					ai_flags,
					depth
				)
			) :
			end_of_turn_branch(
				state,
				Selector(is_ai(faster->first)),
				get_flag(faster->first),
				get_flag(faster->second),
				depth
			);
	}

	auto end_of_turn_branch(
		State<generation> state,
		Selector const select,
		EndOfTurnFlags const first_flag,
		EndOfTurnFlags const last_flag,
		Depth const depth
	) -> Score {
		auto const selected = select(state);
		end_of_turn(
			selected.selection,
			first_flag,
			selected.other,
			last_flag,
			state.environment
		);
		return finish_end_of_turn(state, depth);
	}

	auto finish_end_of_turn(State<generation> const & state, Depth const original_depth) -> Score {
		if (is_fainted(state.ai) or is_fainted(state.foe)) {
			if (auto const won = win(state.ai, state.foe)) {
				return *won + Score(double(original_depth.remaining_general()));
			}
			auto const ai_selections = get_legal_selections(state.ai, state.foe, state.environment);
			return max_score(after_end_of_turn_action(
				state,
				ai_selections,
				get_foe_selections(state, ai_selections),
				original_depth.reduced(containers::size(ai_selections))
			));
		}
		auto const depth = original_depth.one_level_deeper();
		switch (depth.search_type()) {
			case SearchType::full:
				return max_score(start_of_turn_action(state, depth));
			case SearchType::single:
				return generate_single_matchups(state, depth);
			case SearchType::evaluate:
				return Score(double(m_evaluate(state.ai, state.foe)));
		}
	}

	auto generate_single_matchups(State<generation> const & state, Depth const depth) -> Score {
		// There are max_size^2 == 36 possible pairings, which can be divided into four categories (`ai` is the size of the AI's team, `foe` is the size of the foe's team):
		// 1) ai * foe single matchups
		// 2) ai * (max_size - foe) forced wins
		// 3) foe * (max_size - ai) forced losses
		// 4) (max_size - ai) * (max_size * foe) forced ties

		// The nested for loop takes care of the first category.
		auto score = Score(0.0);
		for (auto const ai_index : containers::integer_range(state.ai.size())) {
			for (auto const foe_index : containers::integer_range(state.foe.size())) {
				score += evaluate_single_matchup(state, ai_index, foe_index, depth);
			}
		}
		// The second category expands to
		// max_size * ai - ai * foe
		// And the third category expands to
		// max_size * foe - ai * foe
		// The score of a forced loss is the opposite of the score of a forced win, so we negate the count of forced losses and add it to the count of forced wins and we get
		// max_size * ai - ai * foe - max_size * foe + ai * foe
		// Which simplifies to
		// max_size * ai - max_size * foe
		// Which factors to
		// max_size * (ai_size - foe_size)
		score += double(max_pokemon_per_team * state.ai.size() - state.foe.size()) * victory<generation>;
		// The last category has a score of 0.0 and thus needs no special handling.

		// We divide by the total number of possible pairings to ensure that the
		// score is normalized to remain in the range [-victory, victory] so
		// that early returns still give the correct value.
		score /= double(max_pokemon_per_team * max_pokemon_per_team);
		return score;
	}

	auto evaluate_single_matchup(State<generation> state, TeamIndex const ai_index, TeamIndex const foe_index, Depth const depth) -> Score {
		// TODO: Something involving switch order
		auto remove_all_but_index = [&](Team<generation> & team, TeamIndex const index, Team<generation> & other) {
			if (index != team.all_pokemon().index()) {
				team.switch_pokemon(other.pokemon(), state.environment, index);
				team.reset_end_of_turn();
			}
			team.all_pokemon() = PokemonCollection<Pokemon<generation>>({team.all_pokemon()(team.all_pokemon().index())});
		};
		remove_all_but_index(state.ai, ai_index, state.foe);
		remove_all_but_index(state.foe, foe_index, state.ai);
		if (auto const won = win(state.ai, state.foe)) {
			return *won;
		}
		return max_score(start_of_turn_action(state, depth));
	}


	auto score_executed_actions(
		State<generation> const & state,
		Selector const select,
		Selection const selection,
		OtherAction const other_action,
		Depth const depth,
		auto const continuation
	) const -> Score {
		return tv::visit(selection, tv::overload(
			[&](Switch const switch_) -> Score {
				return execute_switch(state, select, switch_, depth, continuation);
			},
			[&](MoveName const selected) -> Score {
				auto const & team = select(state).selection;
				if (team.pokemon().hp().current() == 0_bi) {
					return continuation(state);
				}
				auto const executed_moves = possible_executed_moves(selected, team);
				auto const sum = containers::sum(containers::transform(
					executed_moves,
					[&](MoveName const executed) {
						return execute_move(
							state,
							select,
							SelectedAndExecuted(selected, executed),
							other_action,
							depth,
							continuation
						);
					}
				));
				return sum / double(containers::size(executed_moves));
			},
			[&](Pass) -> Score {
				return continuation(state);
			}
		));
	}

	auto score_selections(
		LegalSelections const ai_selections,
		SelectionProbabilities const foe_selections,
		auto const function
	) const -> ScoredSelections {
		return ScoredSelections(containers::transform(ai_selections, [&](Selection const ai_selection) {
			return ScoredSelection(
				ai_selection,
				parallel_sum(containers::transform(
					foe_selections,
					[&](SelectionProbability const predicted) {
						return predicted.probability * function(ai_selection, predicted.selection);
					}
				))
			);
		}));
	}

	auto get_foe_selections(
		State<generation> const & state,
		LegalSelections const ai_selections,
		LegalSelections const foe_selections
	) const -> SelectionProbabilities {
		return m_foe_strategy.get()(
			state.foe,
			foe_selections,
			state.ai,
			ai_selections,
			state.environment
		).user;
	}
	auto get_foe_selections(
		State<generation> const & state,
		LegalSelections const ai_selections
	) const -> SelectionProbabilities {
		return get_foe_selections(
			state,
			ai_selections,
			get_legal_selections(state.foe, state.ai, state.environment)
		);
	}

	Evaluate<generation> m_evaluate;
	std::reference_wrapper<Strategy const> m_foe_strategy;
	std::unique_ptr<TranspositionTable<generation>> m_transposition_table;
};

auto to_selection_probabilities(ScoredSelections selections) -> SelectionProbabilities {
	auto const best = containers::max_element(
		selections,
		[](ScoredSelection const lhs, ScoredSelection const rhs) {
			return lhs.score > rhs.score;
		}
	);
	return SelectionProbabilities({
		SelectionProbability(best->selection, Probability(1.0))
	});
}

} // namespace

auto make_expectimax(
	Depth const depth,
	Strategy foe_strategy_
) -> Strategy {
	return Strategy([
		depth,
		all_evaluate = AllEvaluate(),
		foe_strategy = std::move(foe_strategy_)
	]<Generation generation>(
		Team<generation> const & ai,
		LegalSelections const ai_selections,
		Team<generation> const & foe,
		LegalSelections const foe_selections,
		Environment const environment
	) -> BothSelectionProbabilities {
		auto evaluator = Evaluator(
			all_evaluate.get<generation>(),
			foe_strategy,
			depth
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
			predicted_foe_selections,
			depth
		);
		return BothSelectionProbabilities(
			to_selection_probabilities(scored_selections),
			predicted_foe_selections
		);
	});
}

} // namespace technicalmachine
