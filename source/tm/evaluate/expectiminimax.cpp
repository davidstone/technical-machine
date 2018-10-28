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

#include <tm/ability.hpp>
#include <tm/block.hpp>
#include <tm/endofturn.hpp>
#include <tm/switch.hpp>
#include <tm/team.hpp>
#include <tm/variable.hpp>
#include <tm/weather.hpp>

#include <tm/move/base_power.hpp>
#include <tm/move/is_switch.hpp>
#include <tm/move/move.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/use_move.hpp>

#include <tm/pokemon/pokemon.hpp>

#include <tm/stat/calculate.hpp>
#include <tm/stat/chance_to_hit.hpp>
#include <tm/stat/stat.hpp>

#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/pokemon.hpp>

#include <containers/algorithms/filter_iterator.hpp>
#include <containers/integer_range.hpp>

#include <boost/timer.hpp>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>

namespace technicalmachine {
namespace {

struct DepthTracker {
	constexpr DepthTracker(unsigned const depth_to_search, unsigned max_print_depth):
		m_searched_so_far(0U),
		m_depth_to_search(depth_to_search),
		m_max_print_depth(max_print_depth),
		m_indentation(1U)
	{
	}
	
	constexpr auto is_final_iteration() const {
		return m_depth_to_search - 1U == m_searched_so_far;
	}
	constexpr auto indentation() const -> bounded::optional<unsigned> {
		if (m_searched_so_far >= m_max_print_depth) {
			return bounded::none;
		}
		return m_indentation;
	}
	
	constexpr auto increased_indentation(Moves const move) const {
		auto copy = *this;
		if (move != Moves::Pass) {
			++copy.m_indentation;
		}
		return copy;
	}
	constexpr auto one_level_deeper() const {
		auto copy = *this;
		++copy.m_searched_so_far;
		return copy;
	}
	constexpr auto iterative_deepening_value() const {
		return DepthTracker(next_depth_to_search(), 0U);
	}
private:
	constexpr auto next_depth_to_search() const -> unsigned {
		assert(m_depth_to_search != m_searched_so_far);
		return m_depth_to_search - m_searched_so_far - 1U;
	}

	unsigned m_searched_so_far;
	unsigned m_depth_to_search;
	unsigned m_max_print_depth;
	unsigned m_indentation;
};

struct BestMove {
	Moves move;
	double score;
};
BestMove select_type_of_move(Team const & ai, Team const & foe, Weather weather, Evaluate evaluate, DepthTracker depth);


void print_best_move(Team const & team, BestMove const best_move) {
	if (is_switch(best_move.move)) {
		std::cout << "Switch to " << to_string(get_species(team.pokemon(to_replacement(best_move.move))));
	} else {
		std::cout << "Use " << to_string(best_move.move);
	}
	std::cout << " for a minimum expected score of " << static_cast<std::int64_t>(best_move.score) << '\n';
}

void print_action(Team const & team, Moves const move, unsigned const indentation) {
	if (move == Moves::Pass) {
		return;
	}
	std::cout << std::string(indentation, '\t') << "Evaluating " << team.who();
	if (is_switch(move)) {
		auto const replacement_index = to_replacement(move);
		std::cout << " switching to " << to_string(get_species(team.pokemon(replacement_index))) << '\n';
	} else {
		std::cout << " using " << to_string(move) << '\n';
	}
}

void print_estimated_score(double const estimate, Moves const move, bounded::optional<unsigned> const indentation) {
	if (indentation and move != Moves::Pass) {
		std::cout << std::string(*indentation, '\t') << "Estimated score is " << static_cast<std::int64_t>(estimate) << '\n';
	}
}

void update_best_move(Moves & best_move, double & alpha, double const beta, Moves const new_move, bounded::optional<unsigned> const indentation) {
	// If their best response isn't as good as their previous best
	// response, then this new move must be better than the
	// previous AI's best move
	if (beta > alpha) {
		alpha = beta;
		best_move = new_move;
		print_estimated_score(alpha, new_move, indentation);
	}
}

void update_foe_best_move(Moves const move, MoveScores & foe_scores, double & beta, double const max_score, bounded::optional<unsigned> const indentation) {
	if (beta > max_score) {
		beta = max_score;
		foe_scores.set(move, beta);
	}
	print_estimated_score(max_score, move, indentation);
}



bool can_critical_hit(Moves const move) {
	auto const power = base_power(move);
	return power and *power != 0_bi;
}


template<typename Flag, typename BaseFlag, typename Probability, typename NextBranch>
double generic_flag_branch(BaseFlag const first_flags, BaseFlag const last_flags, Probability const & basic_probability, NextBranch const & next_branch) {
	auto const probability = [=](auto const is_first, bool const flag) {
		auto const base = basic_probability(is_first);
		assert(base >= 0.0);
		assert(base <= 1.0);
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
			average_score += p1 * p2 * next_branch(
				Flag(first_flags, first_flag),
				Flag(last_flags, last_flag)
			);
		}
	}
	return average_score;
}



struct MissFlag {
	constexpr explicit MissFlag(bool const miss_):
		miss(miss_)
	{
	}
	bool const miss;
};
struct AwakenFlag : MissFlag {
	constexpr explicit AwakenFlag(MissFlag const miss_, bool const awaken_):
		MissFlag(miss_),
		awaken(awaken_)
	{
	}
	bool const awaken;
};
struct CriticalHitFlag : AwakenFlag {
	constexpr explicit CriticalHitFlag(AwakenFlag const awaken_, bool const critical_hit_):
		AwakenFlag(awaken_),
		critical_hit(critical_hit_)
	{
	}
	bool const critical_hit;
};
struct ShedSkinFlag : CriticalHitFlag {
	constexpr explicit ShedSkinFlag(CriticalHitFlag const critical_hit_, bool const shed_skin_):
		CriticalHitFlag(critical_hit_),
		shed_skin(shed_skin_)
	{
	}
	bool const shed_skin;
};


auto deorder(Team const & first, Team const & last) {
	assert(first.is_me() or last.is_me());
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

template<typename Function>
SelectMoveResult select_move_branch(Team const & ai, StaticVectorMove const ai_selections, Team const & foe, StaticVectorMove const foe_selections, Weather const weather, Evaluate const evaluate, DepthTracker const depth, Function const function) {
	// This calls itself at one lower depth in order to get an initial estimate
	// for move_scores because the algorithm works faster if you start with the
	// correct result. The results from one less depth are used to estimate the
	// correct result.
	auto move_scores = !depth.is_final_iteration() ?
		select_move_branch(ai, ai_selections, foe, foe_selections, weather, evaluate, depth.iterative_deepening_value(), function).move_scores :
		BothMoveScores{MoveScores(ai.pokemon()), MoveScores(foe.pokemon())};
	auto const ai_moves = reorder(ai_selections, move_scores.ai, true);
	auto const foe_moves = reorder(foe_selections, move_scores.foe, false);

	// Working from the inside loop out:

	// The following begins by setting beta to the largest possible value. This
	// is the variable that the opposing player is trying to minimize. As long
	// as the opposing player has any move that won't guarantee their loss,
	// that move will score lower (more negative) than victory, and thus
	// the opponent will set that as their best response to the particular move
	// that the AI uses.

	// After looking at each response the opponent has to a given move, beta is
	// finally set to whatever the score will be if the AI uses that move.
	// alpha is initially set to the lowest possible value, so as long as the
	// AI has any move that won't guarantee its loss, that move will score
	// higher (more positive) than -victory, and thus the AI will set
	// that as its best response. It then replaces that move if it finds a move
	// for which the opponent's best response is more positive than the first
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
			print_action(ai, ai_move, *ai_indentation);
		}
		auto beta = static_cast<double>(victory + 1_bi);
		auto const foe_depth = depth.increased_indentation(ai_move);
		auto const foe_indentation = foe_depth.indentation();
		for (auto const & foe_move : foe_moves) {
			if (foe_indentation) {
				print_action(foe, foe_move, *foe_indentation);
			}
			auto const next_depth = foe_depth.increased_indentation(foe_move);
			auto const max_score = function(ai, ai_move, foe, foe_move, weather, evaluate, next_depth);
			update_foe_best_move(foe_move, move_scores.foe, beta, max_score, foe_indentation);
			// Alpha-Beta pruning
			if (beta <= alpha) {
				break;
			}
		}
		move_scores.ai.set(ai_move, beta);
		update_best_move(best_move, alpha, beta, ai_move, ai_indentation);
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


auto finish_end_of_turn(Team const & first, Team const & last, Weather const weather, Evaluate const evaluate, DepthTracker const depth) -> double {
	// TODO: Use TranspositionTable here
	auto const & [ai, foe] = deorder(first, last);
	if (depth.is_final_iteration()) {
		return static_cast<double>(evaluate(ai, foe, weather));
	}
	return select_type_of_move(ai, foe, weather, evaluate, depth.one_level_deeper()).score;
};

auto handle_end_of_turn_replacing(Team first, Moves const first_move, Team last, Moves const last_move, Weather weather, Evaluate const evaluate, DepthTracker const depth) -> double {
	if (first_move != Moves::Pass) {
		switch_pokemon(first, last, weather, to_replacement(first_move));
	}
	if (last_move != Moves::Pass) {
		switch_pokemon(last, first, weather, to_replacement(last_move));
	}
	if (auto const won = Evaluate::win(first, last)) {
		return *won;
	}
	return finish_end_of_turn(first, last, weather, evaluate, depth);
};

double end_of_turn_branch(Team first, Team last, Weather weather, Evaluate const evaluate, DepthTracker const depth, ShedSkinFlag const first_flag, ShedSkinFlag const last_flag) {
	end_of_turn(first, last, weather, first_flag.shed_skin, last_flag.shed_skin);
	if (auto const won = Evaluate::win(first, last)) {
		return *won;
	}
	if (get_hp(first.pokemon()) == 0_bi or get_hp(last.pokemon()) == 0_bi) {
		auto const first_selections = legal_selections(first, last.pokemon(), weather);
		auto const last_selections = legal_selections(last, first.pokemon(), weather);
		return select_move_branch(first, first_selections, last, last_selections, weather, evaluate, depth, handle_end_of_turn_replacing).move.score;
	}
	return finish_end_of_turn(first, last, weather, evaluate, depth);
}


double end_of_turn_order_branch(Team const & team, Team const & other, Faster const faster, Weather const weather, Evaluate const evaluate, DepthTracker const depth, ShedSkinFlag const team_flag, ShedSkinFlag const other_flag) {
	auto get_flag = [&](auto const & match) {
		return std::addressof(match) == std::addressof(team) ? team_flag : other_flag;
	};
	return !faster ?
		(end_of_turn_branch(team, other, weather, evaluate, depth, team_flag, other_flag) + end_of_turn_branch(other, team, weather, evaluate, depth, other_flag, team_flag)) / 2.0 :
		end_of_turn_branch(faster->first, faster->second, weather, evaluate, depth, get_flag(faster->first), get_flag(faster->second));
}


auto use_move_branch_inner(Variable const & last_variable, CriticalHitFlag const first_flags, CriticalHitFlag const last_flags) {
	return [=, &last_variable](Team first, Moves, Team last, Moves const last_move, Weather weather, Evaluate const evaluate, DepthTracker const depth) {
		// TODO: properly handle used move here
		// TODO: implement properly
		constexpr auto first_damaged = false;
		constexpr auto last_damaged = false;
		call_move(last, last_move, last_damaged, first, bounded::none, first_damaged, weather, last_variable, false, false, false, bounded::none);
		if (auto const won = Evaluate::win(first, last)) {
			return *won;
		}

		auto const teams = faster_pokemon(first, last, weather);
		auto const end_of_turn_order = [&](ShedSkinFlag const team_flag, ShedSkinFlag const other_flag) {
			return end_of_turn_order_branch(first, last, teams, weather, evaluate, depth, team_flag, other_flag);
		};
		return generic_flag_branch<ShedSkinFlag>(
			first_flags,
			last_flags,
			[&](auto const is_first) { return shed_skin_probability(is_first ? first.pokemon() : last.pokemon()); },
			end_of_turn_order
		);
	};
}

auto use_move_branch_outer(Moves const last_move, Variable const & first_variable, Variable const & last_variable, CriticalHitFlag const first_flags, CriticalHitFlag const last_flags) {
	return [=, &first_variable, &last_variable](Team first, Moves const first_move, Team last, Moves, Weather weather, Evaluate const evaluate, DepthTracker const depth) {
		// TODO: implement properly
		constexpr auto first_damaged = false;
		constexpr auto last_damaged = false;
		call_move(first, first_move, first_damaged, last, bounded::none, last_damaged, weather, first_variable, false, false, false, bounded::none);
		if (auto const won = Evaluate::win(first, last)) {
			return *won;
		}
		// TODO: properly handle used move here
		call_move(last, last_move, last_damaged, first, bounded::none, first_damaged, weather, last_variable, last_flags.miss, last_flags.awaken, last_flags.critical_hit, bounded::none);
		if (auto const won = Evaluate::win(first, last)) {
			return *won;
		}
		
		auto const first_selections = StaticVectorMove({Moves::Pass});
		auto const last_selections = legal_selections(last, first.pokemon(), weather);
		return select_move_branch(first, first_selections, last, last_selections, weather, evaluate, depth, use_move_branch_inner(last_variable, first_flags, last_flags)).move.score;
	};
};

double use_move_branch(Team first, Moves const first_move, Team last, Moves const last_move, Variable const & first_variable, Variable const & last_variable, Weather weather, Evaluate const evaluate, DepthTracker const depth, CriticalHitFlag const first_flags, CriticalHitFlag const last_flags) {
#if 0
	auto const initial_last_hp = get_hp(last.pokemon());
	auto const last_damaged = is_damaging(first_move) ? bounded::max(get_hp(last.pokemon()).current() - last_hp.current(), 0_bi) : 0_bi;
	auto const used_move = bounded::optional<UsedMove>(UsedMove{first_move, last_damaged});
#endif

	// TODO: implement properly
	constexpr auto first_damaged = false;
	constexpr auto last_damaged = false;
	
	call_move(first, first_move, first_damaged, last, bounded::none, last_damaged, weather, first_variable, first_flags.miss, first_flags.awaken, first_flags.critical_hit, bounded::none);
	if (auto const won = Evaluate::win(first, last)) {
		return *won;
	}
	auto const first_selections = legal_selections(first, last.pokemon(), weather);
	auto const last_selections = StaticVectorMove({Moves::Pass});
	// TODO: Figure out first / last vs ai / foe
	return select_move_branch(first, first_selections, last, last_selections, weather, evaluate, depth, use_move_branch_outer(last_move, first_variable, last_variable, first_flags, last_flags)).move.score;
}



double random_move_effects_branch(Team const & first, Moves const first_move, Team const & last, Moves const last_move, Weather const weather, Evaluate const evaluate, DepthTracker const depth, AwakenFlag const first_flags, AwakenFlag const last_flags) {
	double score = 0.0;

	auto const first_variables = all_probabilities(first_move, last.size());
	auto const last_variables = all_probabilities(last_move, first.size());
	for (auto const & first_variable : first_variables) {
		for (auto const & last_variable : last_variables) {
			auto const use_move_branch_adaptor = [&](CriticalHitFlag first_flags_, CriticalHitFlag last_flags_) {
				return use_move_branch(first, first_move, last, last_move, first_variable, last_variable, weather, evaluate, depth, first_flags_, last_flags_);
			};
			score += last_variable.probability * first_variable.probability * generic_flag_branch<CriticalHitFlag>(
				first_flags,
				last_flags,
				[&](auto const is_first) { return can_critical_hit(is_first ? first_move : last_move) ? (1.0 / 16.0) : 0.0; },
				use_move_branch_adaptor
			);
		}
	}
	return score;
}


double accuracy_branch(Team const & first, Moves const first_move, Team const & last, Moves const last_move, Weather const weather, Evaluate const evaluate, DepthTracker const depth) {
	auto const probability = [=](auto const & user, auto const user_move, auto const & target, bool const target_moved, bool const miss) {
		auto const base = chance_to_hit(user, user_move, target, weather, target_moved);
		assert(base >= 0.0);
		assert(base <= 1.0);
		return miss ? (1.0 - base) : base;
	};

	double average_score = 0.0;
	for (auto const first_flag : { false, true }) {
		constexpr bool last_moved = false;
		auto const p1 = probability(first.pokemon(), first_move, last.pokemon(), last_moved, first_flag);
		if (p1 == 0.0) {
			continue;
		}
		for (auto const last_flag : { false, true }) {
			constexpr bool first_moved = true;
			auto const p2 = probability(last.pokemon(), last_move, first.pokemon(), first_moved, last_flag);
			if (p2 == 0.0) {
				continue;
			}
			average_score += p1 * p2 * generic_flag_branch<AwakenFlag>(
				MissFlag(first_flag),
				MissFlag(last_flag),
				[&](auto const is_first) { return awaken_probability(is_first ? first.pokemon() : last.pokemon()); },
				[&](AwakenFlag const first_awaken, AwakenFlag const last_awaken) { return random_move_effects_branch(first, first_move, last, last_move, weather, evaluate, depth, first_awaken, last_awaken); }
			);
		}
	}
	return average_score;
}


double order_branch(Team const & ai, Moves const ai_move, Team const & foe, Moves const foe_move, Weather const weather, Evaluate const evaluate, DepthTracker const depth) {
	auto ordered = order(ai, ai_move, foe, foe_move, weather);
	return !ordered ?
		(accuracy_branch(ai, ai_move, foe, foe_move, weather, evaluate, depth) + accuracy_branch(foe, foe_move, ai, ai_move, weather, evaluate, depth)) / 2.0 :
		accuracy_branch(ordered->first.team, ordered->first.move, ordered->second.team, ordered->second.move, weather, evaluate, depth);
}


BestMove select_type_of_move(Team const & ai, Team const & foe, Weather const weather, Evaluate const evaluate, DepthTracker const depth) {
	auto team_is_empty [[maybe_unused]] = [](Team const & team) {
		return team.size() == 0_bi or (team.size() == 1_bi and get_hp(team.pokemon()) == 0_bi);
	};
	assert(!team_is_empty(ai));
	assert(!team_is_empty(foe));
	
	auto const ai_selections = legal_selections(ai, foe.pokemon(), weather);
	auto const foe_selections = legal_selections(foe, ai.pokemon(), weather);
	return select_move_branch(ai, ai_selections, foe, foe_selections, weather, evaluate, depth, order_branch).move;
}

}	// namespace

Moves expectiminimax(Team const & ai, Team const & foe, Weather const weather, Evaluate const evaluate, unsigned const depth) {
	std::cout << std::string(20, '=') + "\nEvaluating to a depth of " << depth << "...\n";
	boost::timer timer;
	constexpr auto max_print_depth = 1U;
	auto const best_move = select_type_of_move(ai, foe, weather, evaluate, DepthTracker(depth, max_print_depth));
	std::cout << "Determined best move in " << timer.elapsed() << " seconds.\n";
	print_best_move(ai, best_move);
	return best_move.move;
}

}	// namespace technicalmachine
