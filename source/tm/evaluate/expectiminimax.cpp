// Expectiminimax
// Copyright (C) 2018 David Stone
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

#include <tm/evaluate/expectiminimax.hpp>

#include <tm/evaluate/evaluate.hpp>
#include <tm/evaluate/move_scores.hpp>
#include <tm/evaluate/reorder.hpp>
#include <tm/evaluate/transposition.hpp>

#include <tm/move/base_power.hpp>
#include <tm/move/call_move.hpp>
#include <tm/move/category.hpp>
#include <tm/move/is_switch.hpp>
#include <tm/move/move.hpp>
#include <tm/move/moves.hpp>

#include <tm/stat/calculate.hpp>
#include <tm/stat/chance_to_hit.hpp>
#include <tm/stat/stat.hpp>

#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/pokemon.hpp>

#include <tm/ability.hpp>
#include <tm/block.hpp>
#include <tm/endofturn.hpp>
#include <tm/team.hpp>
#include <tm/variable.hpp>

#include <bounded/assert.hpp>

#include <containers/algorithms/accumulate.hpp>
#include <containers/algorithms/all_any_none.hpp>
#include <containers/algorithms/filter_iterator.hpp>

#include <boost/timer.hpp>

#include <cstddef>
#include <cstdint>
#include <random>
#include <string>

namespace technicalmachine {
namespace {

struct BestMove {
	Moves move;
	double score;
};
BestMove select_type_of_move(Generation, Team const & ai, Team const & foe, Weather, Evaluate, Depth, std::ostream & log);


void print_best_move(Team const & team, BestMove const best_move, std::ostream & log) {
	if (is_switch(best_move.move)) {
		log << "Switch to " << to_string(get_species(team.pokemon(to_replacement(best_move.move))));
	} else {
		log << "Use " << to_string(best_move.move);
	}
	log << " for a minimum expected score of " << static_cast<std::int64_t>(best_move.score) << '\n';
}

void print_action(Team const & team, Moves const move, unsigned const indentation, std::ostream & log) {
	if (move == Moves::Pass) {
		return;
	}
	log << std::string(indentation, '\t') << "Evaluating " << team.who();
	if (is_switch(move)) {
		auto const replacement_index = to_replacement(move);
		log << " switching to " << to_string(get_species(team.pokemon(replacement_index))) << '\n';
	} else {
		log << " using " << to_string(move) << '\n';
	}
}

void print_estimated_score(double const estimate, Moves const move, bounded::optional<unsigned> const indentation, std::ostream & log) {
	if (indentation and move != Moves::Pass) {
		log << std::string(*indentation, '\t') << "Estimated score is " << static_cast<std::int64_t>(estimate) << '\n';
	}
}

void update_best_move(Moves & best_move, double & alpha, double const beta, Moves const new_move, bounded::optional<unsigned> const indentation, std::ostream & log) {
	// If their best response isn't as good as their previous best
	// response, then this new move must be better than the
	// previous AI's best move
	if (beta > alpha) {
		alpha = beta;
		best_move = new_move;
		print_estimated_score(alpha, new_move, indentation, log);
	}
}

void update_foe_best_move(Moves const move, MoveScores & foe_scores, double & beta, double const max_score, bounded::optional<unsigned> const indentation, std::ostream & log) {
	if (beta > max_score) {
		beta = max_score;
		foe_scores.set(move, beta);
	}
	print_estimated_score(max_score, move, indentation, log);
}



bool can_critical_hit(Generation const generation, Moves const move, Ability const defender_ability) {
	switch (defender_ability) {
		case Ability::Battle_Armor:
		case Ability::Shell_Armor:
			return false;
		default: {
			auto const power = base_power(generation, move);
			return power and *power != 0_bi;
		}
	}
}


double generic_flag_branch(auto const & basic_probability, auto const & next_branch) {
	auto const probability = [=](bool const is_first, bool const flag) {
		auto const base = basic_probability(is_first);
		BOUNDED_ASSERT_OR_ASSUME(base >= 0.0);
		BOUNDED_ASSERT_OR_ASSUME(base <= 1.0);
		return flag ? base : (1.0 - base);
	};

	double average_score = 0.0;
	for (auto const first_flag : { true, false }) {
		auto const p1 = probability(true, first_flag);
		if (p1 == 0.0) {
			continue;
		}
		for (auto const last_flag : { true, false }) {
			auto const p2 = probability(false, last_flag);
			if (p2 == 0.0) {
				continue;
			}
			average_score += p1 * p2 * next_branch(first_flag, last_flag);
		}
	}
	return average_score;
}



auto deorder(Team const & first, Team const & last) {
	BOUNDED_ASSERT(first.is_me() or last.is_me());
	struct Deorder {
		Team const & ai;
		Team const & foe;
	};
	return Deorder{
		(first.is_me()) ? first : last,
		(!first.is_me()) ? first : last
	};
}


struct BothMoveScores {
	MoveScores ai;
	MoveScores foe;
};

struct SelectMoveResult {
	BestMove move;
	BothMoveScores move_scores;
};

SelectMoveResult select_move_branch(Generation const generation, Team const & ai, StaticVectorMove const ai_selections, Team const & foe, StaticVectorMove const foe_selections, Weather const weather, Evaluate const evaluate, Depth const depth, std::ostream & log, auto const function) {
	// This calls itself at one lower depth in order to get an initial estimate
	// for move_scores because the algorithm works faster if you start with the
	// correct result. The results from one less depth are used to estimate the
	// correct result.
	auto move_scores = !depth.is_final_iteration() ?
		select_move_branch(generation, ai, ai_selections, foe, foe_selections, weather, evaluate, depth.iterative_deepening_value(), log, function).move_scores :
		BothMoveScores{MoveScores(ai.pokemon()), MoveScores(foe.pokemon())};
	auto const ai_moves = reorder(ai_selections, move_scores.ai, true);
	auto const foe_moves = reorder(foe_selections, move_scores.foe, false);

	// Working from the inside loop out:

	// The following begins by setting beta to the largest possible value. This
	// is the variable that the opposing player is trying to minimize. As long
	// as the opposing player has any move that won't guarantee their loss,
	// that move will score lower (more negative) than victory, and thus
	// the foe will set that as their best response to the particular move
	// that the AI uses.

	// After looking at each response the foe has to a given move, beta is
	// finally set to whatever the score will be if the AI uses that move.
	// alpha is initially set to the lowest possible value, so as long as the
	// AI has any move that won't guarantee its loss, that move will score
	// higher (more positive) than -victory, and thus the AI will set
	// that as its best response. It then replaces that move if it finds a move
	// for which the foe's best response is more positive than the first
	// move found. In other words, it finds the move for the AI for which the
	// foe's best response is the weakest.
	
	// Something to consider as a potential speed up at the cost of some
	// accuracy (but would allow a deeper, thus more accurate, search) would be
	// to pick from all random numbers randomly, rather than seeing the outcome
	// of all of them and averaging it. In other words, do several trials
	// assuming a particular (but different for each trial) set of random
	// numbers are selected, and then average that result. This would give
	// massive reductions to the branching factor, and with a large enough
	// number of trials could be close enough to the average to potentially
	// speed up the program enough to justify the loss in accuracy.
	
	// I subtract 1 from -victory to make sure that even a guaranteed
	// loss is seen as better than not returning a result. This way, I can do
	// some things when my intermediate scores are strictly greater than alpha,
	// rather than greater than or equal to, which can save a few calculations.
	
	// For a similar reason, I later set beta to victory + 1.
	
	// This change also has the advantage of making sure a move is always put
	// into best_move without any additional logic, such as pre-filling it with
	// some result.

	auto alpha = static_cast<double>(-victory - 1_bi);
	auto best_move = Moves{};
	auto const ai_indentation = depth.indentation();
	for (auto const & ai_move : ai_moves) {
		if (ai_indentation) {
			print_action(ai, ai_move, *ai_indentation, log);
		}
		auto beta = static_cast<double>(victory + 1_bi);
		auto const foe_depth = depth.increased_indentation(ai_move);
		auto const foe_indentation = foe_depth.indentation();
		for (auto const & foe_move : foe_moves) {
			if (foe_indentation) {
				print_action(foe, foe_move, *foe_indentation, log);
			}
			auto const next_depth = foe_depth.increased_indentation(foe_move);
			auto const max_score = function(generation, ai, ai_move, foe, foe_move, weather, evaluate, next_depth, log);
			update_foe_best_move(foe_move, move_scores.foe, beta, max_score, foe_indentation, log);
			// Alpha-Beta pruning
			if (beta <= alpha) {
				break;
			}
		}
		move_scores.ai.set(ai_move, beta);
		update_best_move(best_move, alpha, beta, ai_move, ai_indentation, log);
		// The AI cannot have a better move than a guaranteed win
		if (alpha == static_cast<double>(victory)) {
			break;
		}
	}
	return SelectMoveResult{
		BestMove{best_move, alpha},
		std::move(move_scores)
	};
}


auto finish_end_of_turn(Generation const generation, Team const & first, Team const & last, Weather const weather, Evaluate const evaluate, Depth const depth, std::ostream & log) -> double {
	// TODO: Use TranspositionTable here
	auto const deordered = deorder(first, last);
	auto const & ai = deordered.ai;
	auto const & foe = deordered.foe;
	if (depth.is_final_iteration()) {
		return static_cast<double>(evaluate(generation, ai, foe, weather));
	}
	return select_type_of_move(generation, ai, foe, weather, evaluate, depth.one_level_deeper(), log).score;
}

auto handle_end_of_turn_replacing(Generation const generation, Team first, Moves const first_move, Team last, Moves const last_move, Weather weather, Evaluate const evaluate, Depth const depth, std::ostream & log) -> double {
	if (first_move != Moves::Pass) {
		first.switch_pokemon(generation, last.pokemon(), weather, to_replacement(first_move));
	}
	if (last_move != Moves::Pass) {
		last.switch_pokemon(generation, first.pokemon(), weather, to_replacement(last_move));
	}
	if (auto const won = Evaluate::win(first, last)) {
		return *won;
	}
	return finish_end_of_turn(generation, first, last, weather, evaluate, depth, log);
}

double end_of_turn_branch(Generation const generation, Team first, Team last, Weather weather, Evaluate const evaluate, Depth const depth, EndOfTurnFlags const first_flag, EndOfTurnFlags const last_flag, std::ostream & log) {
	end_of_turn(generation, first, first_flag, last, last_flag, weather);
	if (auto const won = Evaluate::win(first, last)) {
		return *won;
	}
	if (get_hp(first.pokemon()) == 0_bi or get_hp(last.pokemon()) == 0_bi) {
		auto const first_selections = legal_selections(generation, first, last.pokemon(), weather);
		auto const last_selections = legal_selections(generation, last, first.pokemon(), weather);
		return select_move_branch(generation, first, first_selections, last, last_selections, weather, evaluate, depth, log, handle_end_of_turn_replacing).move.score;
	}
	return finish_end_of_turn(generation, first, last, weather, evaluate, depth, log);
}


double end_of_turn_order_branch(Generation const generation, Team const & team, Team const & other, Faster const faster, Weather const weather, Evaluate const evaluate, Depth const depth, EndOfTurnFlags const team_flag, EndOfTurnFlags const other_flag, std::ostream & log) {
	auto get_flag = [&](Team const & match) {
		return std::addressof(match) == std::addressof(team) ? team_flag : other_flag;
	};
	return !faster ?
		(end_of_turn_branch(generation, team, other, weather, evaluate, depth, team_flag, other_flag, log) + end_of_turn_branch(generation, other, team, weather, evaluate, depth, other_flag, team_flag, log)) / 2.0 :
		end_of_turn_branch(generation, faster->first, faster->second, weather, evaluate, depth, get_flag(faster->first), get_flag(faster->second), log);
}

constexpr auto can_be_selected_by_sleep_talk(Moves const move) {
	switch (move) {
		case Moves::Assist:
		case Moves::Bide:
		case Moves::Chatter:
		case Moves::Copycat:
		case Moves::Focus_Punch:
		case Moves::Me_First:
		case Moves::Metronome:
		case Moves::Mirror_Move:
		case Moves::Sleep_Talk:
		case Moves::Uproar:
		case Moves::Blast_Burn:
		case Moves::Frenzy_Plant:
		case Moves::Giga_Impact:
		case Moves::Hydro_Cannon:
		case Moves::Hyper_Beam:
		case Moves::Roar_of_Time:
		case Moves::Rock_Wrecker:
		case Moves::Solar_Beam:
			return false;
		default:
			return true;
	}
}

auto selected_move_to_executed_move(Moves const selected_move, Team const & user_team) {
	using result = containers::static_vector<Moves, 3>;
	auto const user_pokemon = user_team.pokemon();
	using containers::filter;
	using containers::transform;
	switch (selected_move) {
		case Moves::Sleep_Talk:
			return is_sleeping(get_status(user_pokemon)) ?
				result{filter(transform(regular_moves(user_pokemon), &Move::name), can_be_selected_by_sleep_talk)} :
				result{selected_move};
		default:
			return result{selected_move};
	}
}

constexpr auto average_transformed_sum(auto && range, auto transformation) {
	auto const range_size = size(range);
	return containers::accumulate(containers::transform(OPERATORS_FORWARD(range), std::move(transformation))) / static_cast<double>(range_size);
}

double generic_flag_branch(double const basic_probability, auto const & next_branch) {
	BOUNDED_ASSERT_OR_ASSUME(0.0 <= basic_probability and basic_probability <= 1.0);
	double average_score = 0.0;
	for (auto const flag : { false, true }) {
		auto const probability = flag ? basic_probability : (1.0 - basic_probability);
		if (probability == 0.0) {
			continue;
		}
		average_score += probability * next_branch(flag);
	}
	return average_score;
}

struct SelectedAndExecuted {
	Moves selected;
	Moves executed;
};

auto execute_move(Generation const generation, Team const & user, SelectedAndExecuted const move, Team const & other, OtherMove const other_move, Weather const weather, auto const continuation) -> double {
	auto const user_pokemon = user.pokemon();
	auto const other_pokemon = other.pokemon();
	auto const variables = all_probabilities(move.executed, other.size());
	auto const status = get_status(user_pokemon);
	auto const probability_of_clearing_status = status.probability_of_clearing(user_pokemon.ability());
	auto const specific_chance_to_hit = chance_to_hit(generation, user_pokemon, move.executed, other_pokemon, weather, other_pokemon.moved());
	auto const move_can_critical_hit = can_critical_hit(generation, move.executed, other.pokemon().ability());
	return generic_flag_branch(probability_of_clearing_status, [&](bool const clear_status) {
		return generic_flag_branch(specific_chance_to_hit, [&](bool const hits) {
			auto score = 0.0;
			for (auto const & variable : variables) {
				score += variable.probability * generic_flag_branch(
					hits and move_can_critical_hit ? (1.0 / 16.0) : 0.0,
					[&](bool const critical_hit) {
						auto user_copy = user;
						auto other_copy = other;
						auto weather_copy = weather;
						call_move(
							generation,
							user_copy,
							UsedMove{move.selected, move.executed, variable.variable, critical_hit, !hits},
							other_copy,
							other_move,
							weather_copy,
							clear_status,
							ActualDamage::Unknown{}
						);
						if (auto const won = Evaluate::win(user_copy, other_copy)) {
							return *won;
						}
						return continuation(user_copy, other_copy, weather_copy);
					}
				);
			}
			return score;
		});
	});
}

auto score_executed_moves(Generation const generation, Team const & user, Moves const selected_move, Team const & other, OtherMove const other_move, Weather const weather, auto const continuation) -> double {
	auto const score_move = [&](Moves const executed_move) {
		return execute_move(generation, user, SelectedAndExecuted{selected_move, executed_move}, other, other_move, weather, continuation);
	};
	return average_transformed_sum(selected_move_to_executed_move(selected_move, user), score_move);
}


auto use_move_branch_inner(Moves const first_used_move) {
	return [=](Generation const generation, Team const & first, Moves const first_move [[maybe_unused]], Team const & last, Moves const last_move, Weather const weather, Evaluate const evaluate, Depth const depth, std::ostream & log) {
		BOUNDED_ASSERT_OR_ASSUME(first_move == Moves::Pass);
		return score_executed_moves(generation, last, last_move, first, first_used_move, weather, [&](Team const & updated_last, Team const & updated_first, Weather const updated_weather) {
			auto shed_skin_probability = [&](bool const is_first) {
				auto const pokemon = (is_first ? updated_first : updated_last).pokemon();
				return can_clear_status(pokemon.ability(), get_status(pokemon)) ? 0.3 : 0.0;
			};
			auto const teams = Faster(generation, updated_first, updated_last, updated_weather);
			return generic_flag_branch(shed_skin_probability, [&](bool const team_shed_skin, bool const other_shed_skin) {
				return generic_flag_branch(
					// TODO
					[&](bool) { return true; },
					[&](bool const team_lock_in_ends, bool const other_lock_in_ends) {
						return end_of_turn_order_branch(
							generation,
							updated_first,
							updated_last,
							teams,
							updated_weather,
							evaluate,
							depth,
							EndOfTurnFlags{team_shed_skin, team_lock_in_ends},
							EndOfTurnFlags{other_shed_skin, other_lock_in_ends},
							log
						);
					}
				);
			});
		});
	};
}

struct OriginalPokemon {
	explicit OriginalPokemon(Pokemon const & pokemon, Moves const other_move):
		m_species(get_species(pokemon)),
		m_hp(get_hp(pokemon).current()),
		m_other_move(other_move)
	{
	}
	
	auto is_same_pokemon(Pokemon const & pokemon) const {
		return get_species(pokemon) == m_species;
	}
	auto other_move() const {
		return m_other_move;
	}

private:
	Species m_species;
	HP::current_type m_hp;
	Moves m_other_move;
};

constexpr auto all_are_pass_or_switch [[maybe_unused]](StaticVectorMove const legal_selections) {
	return
		(size(legal_selections) == 1_bi and front(legal_selections) == Moves::Pass) or
		containers::all(legal_selections, is_switch);
}

auto use_move_branch_outer(OriginalPokemon const original_last_pokemon, Moves const last_move) {
	return [=](Generation const generation, Team const & first, Moves const first_move, Team const & last, Moves, Weather const weather, Evaluate const evaluate, Depth const depth, std::ostream & log) {
		return score_executed_moves(generation, first, first_move, last, FutureMove{is_damaging(last_move)}, weather, [&](Team const & pre_updated_first, Team const & pre_updated_last, Weather const pre_updated_weather) {
			auto const is_same_pokemon = original_last_pokemon.is_same_pokemon(last.pokemon());
			auto const actual_last_move = is_same_pokemon ? last_move : Moves::Pass;
			auto const first_used_move = original_last_pokemon.other_move();
			return score_executed_moves(generation, pre_updated_last, actual_last_move, pre_updated_first, first_used_move, pre_updated_weather, [&](Team const & updated_first, Team const & updated_last, Weather const updated_weather) {
				auto const first_selections = StaticVectorMove({Moves::Pass});
				auto const last_selections = legal_selections(generation, updated_last, updated_first.pokemon(), weather);
				BOUNDED_ASSERT(all_are_pass_or_switch(last_selections));
				return select_move_branch(generation, updated_first, first_selections, updated_last, last_selections, updated_weather, evaluate, depth, log, use_move_branch_inner(first_used_move)).move.score;
			});
		});
	};
}

double use_move_branch(Generation const generation, Team const & first, Moves const first_move, Team const & last, Moves const last_move, Weather const weather, Evaluate const evaluate, Depth const depth, std::ostream & log) {
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

	auto const last_pokemon = last.pokemon();
	auto const original_last_pokemon = OriginalPokemon(last_pokemon, first_move);

	return score_executed_moves(generation, first, first_move, last, FutureMove{is_damaging(last_move)}, weather, [&](Team const & updated_first, Team const & updated_last, Weather const updated_weather) {
		auto const first_selections = legal_selections(generation, updated_first, updated_last.pokemon(), weather);
		BOUNDED_ASSERT(all_are_pass_or_switch(first_selections));
		auto const last_selections = StaticVectorMove({Moves::Pass});
		// TODO: Figure out first / last vs ai / foe
		return select_move_branch(generation, updated_first, first_selections, updated_last, last_selections, updated_weather, evaluate, depth, log, use_move_branch_outer(original_last_pokemon, last_move)).move.score;
	});
}


double order_branch(Generation const generation, Team const & ai, Moves const ai_move, Team const & foe, Moves const foe_move, Weather const weather, Evaluate const evaluate, Depth const depth, std::ostream & log) {
	auto ordered = Order(generation, ai, ai_move, foe, foe_move, weather);
	return !ordered ?
		(use_move_branch(generation, ai, ai_move, foe, foe_move, weather, evaluate, depth, log) + use_move_branch(generation, foe, foe_move, ai, ai_move, weather, evaluate, depth, log)) / 2.0 :
		use_move_branch(generation, ordered->first.team, ordered->first.move, ordered->second.team, ordered->second.move, weather, evaluate, depth, log);
}

auto team_is_empty(Team const & team) {
	return team.size() == 0_bi or (team.size() == 1_bi and get_hp(team.pokemon()) == 0_bi);
};

BestMove select_type_of_move(Generation const generation, Team const & ai, Team const & foe, Weather const weather, Evaluate const evaluate, Depth const depth, std::ostream & log) {
	BOUNDED_ASSERT(!team_is_empty(ai));
	BOUNDED_ASSERT(!team_is_empty(foe));
	
	auto const ai_selections = legal_selections(generation, ai, foe.pokemon(), weather);
	auto const foe_selections = legal_selections(generation, foe, ai.pokemon(), weather);
	return select_move_branch(generation, ai, ai_selections, foe, foe_selections, weather, evaluate, depth, log, order_branch).move;
}

}	// namespace

Moves expectiminimax(Generation const generation, Team const & ai, Team const & foe, Weather const weather, Evaluate const evaluate, Depth const depth, std::ostream & log) {
	if (team_is_empty(ai) or team_is_empty(foe)) {
		throw std::runtime_error("Tried to evaluate a position with an empty team");
	}
	log << "Evaluating to a depth of " << depth.depth_to_search() << "...\n";
	boost::timer timer;
	auto const best_move = select_type_of_move(generation, ai, foe, weather, evaluate, depth, log);
	log << "Determined best move in " << timer.elapsed() << " seconds: ";
	print_best_move(ai, best_move, log);
	return best_move.move;
}

}	// namespace technicalmachine
