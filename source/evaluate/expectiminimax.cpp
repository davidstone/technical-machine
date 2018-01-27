// Expectiminimax
// Copyright (C) 2016 David Stone
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

#include "expectiminimax.hpp"

#include "evaluate.hpp"
#include "move_scores.hpp"
#include "reorder.hpp"
#include "transposition.hpp"

#include "../ability.hpp"
#include "../block.hpp"
#include "../endofturn.hpp"
#include "../switch.hpp"
#include "../team.hpp"
#include "../variable.hpp"
#include "../weather.hpp"

#include "../move/base_power.hpp"
#include "../move/is_switch.hpp"
#include "../move/move.hpp"
#include "../move/moves.hpp"
#include "../move/use_move.hpp"

#include "../pokemon/pokemon.hpp"

#include "../stat/calculate.hpp"
#include "../stat/chance_to_hit.hpp"
#include "../stat/stat.hpp"

#include "../string_conversions/move.hpp"
#include "../string_conversions/pokemon.hpp"

#include <bounded/integer_range.hpp>

#include <boost/timer.hpp>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>

namespace technicalmachine {
namespace {

// This prints out search equal to the maximum depth normally, but any deeper
// searches will also print out with a single tab. This is not recommended for
// depth greater than 2.
constexpr bool verbose = false;

void print_best_move(Team const & team, BestMove const best_move) {
	if (is_switch(best_move.move)) {
		std::cout << "Switch to " << to_string(static_cast<Species>(team.pokemon(to_replacement(best_move.move))));
	} else {
		std::cout << "Use " << to_string(best_move.move);
	}
	std::cout << " for a minimum expected score of " << static_cast<std::int64_t>(best_move.score) << '\n';
}

void print_action(Team const & team, Moves const move, bool const first_turn) {
	if (verbose or first_turn) {
		unsigned tabs = first_turn ? 0 : 2;
		if (!team.is_me()) {
			++tabs;
		}
		std::cout << std::string(tabs, '\t') << "Evaluating ";
		if (is_switch(move)) {
			auto const replacement_index = to_replacement(move);
			std::cout << "switching to " << to_string(static_cast<Species>(team.pokemon(replacement_index))) << '\n';
		} else {
			std::cout << to_string(move) << '\n';
		}
	}
}

void print_estimated_score(bool const first_turn, bool const is_me, double const estimate) {
	if (verbose or first_turn) {
		unsigned tabs = first_turn ? 0 : 2;
		if (!is_me) {
			++tabs;
		}
		std::cout << std::string(tabs, '\t') + "Estimated score is " << static_cast<std::int64_t>(estimate) << '\n';
	}
}



void update_best_move(Moves & best_move, double & alpha, double const beta, bool const first_turn, Moves const new_move) {
	// If their best response isn't as good as their previous best
	// response, then this new move must be better than the
	// previous AI's best move
	if (beta > alpha) {
		alpha = beta;
		best_move = new_move;
		constexpr bool is_me = true;
		print_estimated_score(first_turn, is_me, alpha);
	}
}

void update_foe_best_move(Moves const move, MoveScores & foe_scores, double & beta, double const max_score, bool const first_turn) {
	if (beta > max_score) {
		beta = max_score;
		foe_scores.set(move, beta);
	}
	constexpr bool is_me = false;
	print_estimated_score(first_turn, is_me, max_score);
}



bool has_follow_up_decision(Moves const move) {
	switch (move) {
		case Moves::Baton_Pass:
		case Moves::U_turn:
			return true;
		default:
			return false;
	}
}

bool can_critical_hit(Moves const move) {
	auto const power = base_power(move);
	return power and *power != 0_bi;
}


// TODO: Use a filter iterator
auto skip_this_replacement(PokemonCollection const & collection, TeamIndex const replacement) {
	auto const would_switch_to_self = replacement == collection.index();
	return would_switch_to_self and size(collection) > 1_bi;
}

auto deorder(Team & first, Team & last) {
	assert(first.is_me() or last.is_me());
	struct Deorder {
		Team & ai;
		Team & foe;
	};
	return Deorder{
		(first.is_me()) ? first : last,
		(!first.is_me()) ? first : last
	};
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
	constexpr MissFlag() = default;
	constexpr explicit MissFlag(bool const miss_):
		miss(miss_)
	{
	}
	bool const miss = false;
};
struct AwakenFlag : MissFlag {
	constexpr AwakenFlag() = default;
	constexpr explicit AwakenFlag(MissFlag const miss, bool const awaken_):
		MissFlag(miss),
		awaken(awaken_)
	{
	}
	bool const awaken = false;
};
struct CriticalHitFlag : AwakenFlag {
	constexpr CriticalHitFlag() = default;
	constexpr explicit CriticalHitFlag(AwakenFlag const awaken, bool const critical_hit_):
		AwakenFlag(awaken),
		critical_hit(critical_hit_)
	{
	}
	bool const critical_hit = false;
};
struct ShedSkinFlag : CriticalHitFlag {
	constexpr explicit ShedSkinFlag(CriticalHitFlag const critical_hit, bool const shed_skin_):
		CriticalHitFlag(critical_hit),
		shed_skin(shed_skin_)
	{
	}
	bool const shed_skin;
};


BestMove move_then_switch_branch(Team const & switcher, Move switcher_move, Team const & other, bounded::optional<Move> other_move, Variable const & switcher_variable, Variable const & other_variable, Weather const weather, unsigned depth, Evaluate const & evaluate, CriticalHitFlag const switcher_flags, CriticalHitFlag const other_flags, bool first_turn);



double end_of_turn_branch(Team first, Team last, Weather weather, unsigned depth, Evaluate const & evaluate, ShedSkinFlag const first_flag, ShedSkinFlag const last_flag) {
	end_of_turn(first, last, weather, first_flag.shed_skin, last_flag.shed_skin);

	auto const last_violated = Evaluate::sleep_clause(first);
	auto const first_violated = Evaluate::sleep_clause(last);
	if (last_violated != 0_bi or first_violated != 0_bi) {
		return static_cast<double>(last_violated + first_violated);
	}

	auto const first_win = Evaluate::win(first);
	auto const last_win = Evaluate::win(last);
	if (first_win != 0_bi or last_win != 0_bi) {
		return static_cast<double>(first_win + last_win);
	}

	auto const teams = deorder(first, last);
	return transposition(teams.ai, teams.foe, weather, depth, evaluate);
}


double end_of_turn_order_branch(Team const & team, Team const & other, Faster const faster, Weather const weather, unsigned depth, Evaluate const & evaluate, ShedSkinFlag const team_flag, ShedSkinFlag const other_flag) {
	auto get_flag = [&](auto const & match) {
		return std::addressof(match) == std::addressof(team) ? team_flag : other_flag;
	};
	return !faster ?
		(end_of_turn_branch(team, other, weather, depth, evaluate, team_flag, other_flag) + end_of_turn_branch(other, team, weather, depth, evaluate, other_flag, team_flag)) / 2.0 :
		end_of_turn_branch(faster->first, faster->second, weather, depth, evaluate, get_flag(faster->first), get_flag(faster->second));
}


double use_move_branch(Team & first, Move const first_move, Team & last, bounded::optional<Move> const last_move, Variable const & first_variable, Variable const & last_variable, Weather weather, unsigned depth, Evaluate const & evaluate, CriticalHitFlag const first_flags, CriticalHitFlag const last_flags) {
	auto use_move_and_follow_up = [&](Team & user, Move const user_move, Variable const & user_variable, CriticalHitFlag const user_flags, Team & other, bounded::optional<UsedMove> const other_move, Variable const & other_variable, CriticalHitFlag const other_flags) -> bounded::optional<double>{
		// If first uses a phazing move before last gets a chance to move, the
		// newly brought out Pokemon would try to move without checking to see
		// if it has already moved. This check is also necessary for my Baton
		// Pass and U-turn implementation to function.
		if (moved(user.pokemon())) {
			return bounded::none;
		}
		// TODO: implement properly
		constexpr auto user_damaged = false;
		constexpr auto other_damaged = false;
		call_move(user, user_move, user_damaged, other, other_move, other_damaged, weather, user_variable, user_flags.miss, user_flags.awaken, user_flags.critical_hit, bounded::none);
		auto const user_win = Evaluate::win(user);
		auto const other_win = Evaluate::win(other);
		if (user_win != 0_bi or other_win != 0_bi) {
			return static_cast<double>(user_win + other_win);
		}
		if (has_follow_up_decision(user_move) and size(user.all_pokemon()) > 1_bi) {
			return move_then_switch_branch(
				user,
				user_move,
				other,
				BOUNDED_CONDITIONAL(other_move, other_move->move, bounded::none),
				user_variable,
				other_variable,
				weather,
				depth,
				evaluate,
				user_flags,
				other_flags,
				false
			).score;
		}
		return bounded::none;
	};

	auto const last_hp = get_hp(last.pokemon());
	auto value = use_move_and_follow_up(first, first_move, first_variable, first_flags, last, bounded::none, last_variable, last_flags);
	if (value) {
		return *value;
	}
	if (last_move) {
		auto const last_damaged = is_damaging(first_move) ? bounded::max(get_hp(last.pokemon()).current() - last_hp.current(), 0_bi) : 0_bi;
		auto const used_move = bounded::optional<UsedMove>(UsedMove{first_move, last_damaged});
		value = use_move_and_follow_up(last, *last_move, last_variable, last_flags, first, used_move, first_variable, first_flags);
		if (value) {
			return *value;
		}
	}

	// Find the expected return on all possible outcomes at the end of the turn
	
	auto const teams = faster_pokemon(first, last, weather);
	auto const end_of_turn_order = [&](ShedSkinFlag const team_flag, ShedSkinFlag const other_flag) {
		return end_of_turn_order_branch(first, last, teams, weather, depth, evaluate, team_flag, other_flag);
	};
	return generic_flag_branch<ShedSkinFlag>(
		first_flags,
		last_flags,
		[&](auto const is_first) { return shed_skin_probability(is_first ? first.pokemon() : last.pokemon()); },
		end_of_turn_order
	);
}



double switch_after_move_branch(Team switcher, Move const switcher_move, Team other, bounded::optional<Move> const other_move, Variable const & switcher_variable, Variable const & other_variable, Weather weather, unsigned depth, Evaluate const & evaluate, TeamIndex const replacement, CriticalHitFlag const switcher_flags, CriticalHitFlag const other_flags) {
	switch_pokemon(switcher, other, weather, replacement);
	assert(!empty(switcher.all_pokemon()));
	assert(!empty(other.all_pokemon()));
	// I don't have to correct for which of the Pokemon moved first because
	// there are only two options:
	
	// Option 1: only the switcher has moved. Then it obviously went first and
	// I'm passing them in the proper order.
	
	// Option 2: Both Pokemon have moved. use_move_branch then
	// recalculates which Pokemon is faster to properly account for end-of-turn
	// effects. In this case, it doesn't matter what order I pass them.
	
	// I also do not need to worry about the game ending due to entry hazards,
	// because at the very least the Pokemon that used Baton Pass / U-turn is
	// still alive.

	return use_move_branch(switcher, switcher_move, other, other_move, switcher_variable, other_variable, weather, depth, evaluate, switcher_flags, other_flags);
}


BestMove move_then_switch_branch(Team const & switcher, Move const switcher_move, Team const & other, bounded::optional<Move> const other_move, Variable const & switcher_variable, Variable const & other_variable, Weather const weather, unsigned depth, Evaluate const & evaluate, CriticalHitFlag const switcher_flags, CriticalHitFlag const other_flags, bool first_turn) {
	unsigned tabs = first_turn ? 0 : 2;
	auto alpha = static_cast<double>(-victory - 1_bi);
	if (!switcher.is_me()) {
		alpha = -alpha;
		++tabs;
	}
	auto best_switch = Moves{};
	for (auto const replacement : integer_range(size(switcher.all_pokemon()))) {
		if (skip_this_replacement(switcher.all_pokemon(), replacement)) {
			continue;
		}
		
		auto const replacement_move = to_switch(replacement);
		print_action(switcher, replacement_move, first_turn);
		auto const value = switch_after_move_branch(switcher, switcher_move, other, other_move, switcher_variable, other_variable, weather, depth, evaluate, replacement, switcher_flags, other_flags);
		if (switcher.is_me()) {
			update_best_move(best_switch, alpha, value, first_turn, replacement_move);
		} else {
			MoveScores foe_scores(switcher.pokemon());
			update_foe_best_move(switcher_move, foe_scores, alpha, value, first_turn);
		}
	}
	return BestMove{best_switch, alpha};
}



constexpr auto all_switches(TeamSize const team_size, containers::index_type<PokemonCollection> const index) {
	containers::static_vector<Moves, static_cast<std::size_t>(TeamSize::max() - 1_bi)> switches;
	for (auto const n : bounded::integer_range(team_size)) {
		if (n != index) {
			push_back(switches, to_switch(n));
		}
	}
	return switches;
}

template<typename Container>
auto random_move_or_switch(std::mt19937 & random_engine, Container const & moves) {
	assert(!empty(moves));
	std::uniform_int_distribution<int> distribution(0, static_cast<int>(size(moves)) - 1);
	return moves[static_cast<containers::index_type<decltype(moves)>>(distribution(random_engine))];
}

auto random_action(Team const & ai, Team const & foe, Weather const weather, std::mt19937 & random_engine) {
	return switch_decision_required(ai.pokemon()) ?
		random_move_or_switch(random_engine, all_switches(size(ai.all_pokemon()), ai.all_pokemon().index())) :
		random_move_or_switch(random_engine, LegalSelections(ai, foe.pokemon(), weather));
}

double fainted(Team first, Team last, Weather weather, unsigned depth, Evaluate const & evaluate, TeamIndex const first_replacement, TeamIndex const last_replacement) {
	if (get_hp(first.pokemon()) == 0_bi) {
		switch_pokemon(first, last, weather, first_replacement);
		auto const first_won = Evaluate::win(first);
		auto const last_won = Evaluate::win(last);
		if (first_won != 0_bi or last_won != 0_bi) {
			return static_cast<double>(first_won + last_won);
		}
	}
	if (get_hp(last.pokemon()) == 0_bi) {
		switch_pokemon(last, first, weather, last_replacement);
		auto const first_won = Evaluate::win(first);
		auto const last_won = Evaluate::win(last);
		if (first_won != 0_bi or last_won != 0_bi) {
			return static_cast<double>(first_won + last_won);
		}
	}

	auto const teams = deorder(first, last);
	return transposition(teams.ai, teams.foe, weather, depth, evaluate);
}


BestMove replace(Team const & ai, Team const & foe, Weather const weather, unsigned depth, Evaluate const & evaluate, bool first_turn) {
	auto const faster = faster_pokemon(ai, foe, weather);
	auto best_move = Moves{};
	auto alpha = static_cast<double>(-victory - 1_bi);
	// TODO: use accumulate instead of a for loop
	for (auto const ai_replacement : integer_range(size(ai.all_pokemon()))) {
		// TODO: Use a filter iterator
		if (skip_this_replacement(ai.all_pokemon(), ai_replacement)) {
			continue;
		}
		auto const ai_move = to_switch(ai_replacement);
		print_action(ai, ai_move, first_turn);
		auto beta = static_cast<double>(victory + 1_bi);
		for (auto const foe_replacement : integer_range(size(foe.all_pokemon()))) {
			if (skip_this_replacement(foe.all_pokemon(), foe_replacement)) {
				continue;
			}
			auto get_replacement = [&](Team const & team) {
				return std::addressof(team) == std::addressof(ai) ? ai_replacement : foe_replacement;
			};
			beta = std::min(beta, !faster ?
				(fainted(ai, foe, weather, depth, evaluate, ai_replacement, foe_replacement) + fainted(foe, ai, weather, depth, evaluate, ai_replacement, foe_replacement)) / 2.0 :
				fainted(faster->first, faster->second, weather, depth, evaluate, get_replacement(faster->first), get_replacement(faster->second))
			);
			if (beta <= alpha) {
				break;
			}
		}
		update_best_move(best_move, alpha, beta, first_turn, ai_move);
	}
	return BestMove{best_move, alpha};
}



double random_move_effects_branch(Team const & first, Move const first_move, Team const & last, Move const last_move, Weather const weather, unsigned depth, Evaluate const & evaluate, AwakenFlag const first_flags, AwakenFlag const last_flags) {
	double score = 0.0;

	auto const first_variables = all_probabilities(first_move, last.size());
	auto const last_variables = all_probabilities(last_move, first.size());
	for (auto const & first_variable : first_variables) {
		for (auto const & last_variable : last_variables) {
			auto const use_move_copy_branch = [&](CriticalHitFlag first_flags_, CriticalHitFlag last_flags_) {
				auto first_ = first;
				auto last_ = last;
				return use_move_branch(first_, first_move, last_, last_move, first_variable, last_variable, weather, depth, evaluate, first_flags_, last_flags_);
			};
			score += last_variable.probability * first_variable.probability * generic_flag_branch<CriticalHitFlag>(
				first_flags,
				last_flags,
				[&](auto const is_first) { return can_critical_hit(is_first ? first_move : last_move) ? (1.0 / 16.0) : 0.0; },
				use_move_copy_branch
			);
		}
	}
	return score;
}


double accuracy_branch(Team const & first, Move const first_move, Team const & last, Move const last_move, Weather const weather, unsigned depth, Evaluate const & evaluate) {
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
				[&](AwakenFlag const first_awaken, AwakenFlag const last_awaken) { return random_move_effects_branch(first, first_move, last, last_move, weather, depth, evaluate, first_awaken, last_awaken); }
			);
		}
	}
	return average_score;
}


double order_branch(Team const & ai, Move const ai_move, Team const & foe, Move const foe_move, Weather const weather, unsigned depth, Evaluate const & evaluate) {
	auto ordered = order(ai, ai_move, foe, foe_move, weather);
	return !ordered ?
		(accuracy_branch(ai, ai_move, foe, foe_move, weather, depth, evaluate) + accuracy_branch(foe, foe_move, ai, ai_move, weather, depth, evaluate)) / 2.0 :
		accuracy_branch(ordered->first.team, ordered->first.move, ordered->second.team, ordered->second.move, weather, depth, evaluate);
}


struct BothMoveScores {
	MoveScores ai;
	MoveScores foe;
};

struct SelectMoveResult {
	BestMove move;
	BothMoveScores move_scores;
};

SelectMoveResult select_move_branch(Team const & ai, Team const & foe, Weather const weather, unsigned depth, Evaluate const & evaluate, bool first_turn) {
	// This calls itself at one lower depth in order to get an initial estimate
	// for move_scores because the algorithm works faster if you start with the
	// correct result. The results from one less depth are used to estimate the
	// correct result.
	auto move_scores = (depth >= 1) ?
		select_move_branch(ai, foe, weather, depth - 1, evaluate, false).move_scores :
		BothMoveScores{MoveScores(ai.pokemon()), MoveScores(foe.pokemon())};
	auto const ai_moves = reorder(LegalSelections(ai, foe.pokemon(), weather), move_scores.ai, true);
	auto const foe_moves = reorder(LegalSelections(foe, ai.pokemon(), weather), move_scores.foe, false);

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
	for (auto const & ai_move : ai_moves) {
		print_action(ai, ai_move, first_turn);
		auto beta = static_cast<double>(victory + 1_bi);
		for (auto const & foe_move : foe_moves) {
			print_action(foe, foe_move, first_turn);
			auto const max_score = order_branch(ai, ai_move, foe, foe_move, weather, depth, evaluate);
			update_foe_best_move(foe_move, move_scores.foe, beta, max_score, first_turn);
			// Alpha-Beta pruning
			if (beta <= alpha)
				break;
		}
		move_scores.ai.set(ai_move, beta);
		update_best_move(best_move, alpha, beta, first_turn, ai_move);
		// The AI cannot have a better move than a guaranteed win
		if (alpha == static_cast<double>(victory))
			break;
	}
	return SelectMoveResult{
		BestMove{best_move, alpha},
		std::move(move_scores)
	};
}

}	// namespace

Moves expectiminimax(Team const & ai, Team const & foe, Weather const weather, unsigned depth, Evaluate const & evaluate, std::mt19937 & random_engine) {
	std::cout << std::string(20, '=') + "\nEvaluating to a depth of " << depth << "...\n";
	boost::timer timer;
	try {
		constexpr auto full_evaluation = true;
		auto const best_move = select_type_of_move(ai, foe, weather, depth, evaluate, full_evaluation);
		std::cout << "Determined best move in " << timer.elapsed() << " seconds.\n";
		print_best_move(ai, best_move);
		return best_move.move;
	} catch (InvalidCollectionIndex const & ex) {
		// If there was an error, I just use a random move.
		std::cerr << std::string(20, '=') + '\n';
		std::cerr << ex.what();
		std::cerr << std::string(20, '=') + '\n';
		return random_action(ai, foe, weather, random_engine);
	}
}

BestMove select_type_of_move(Team const & ai, Team const & foe, Weather const weather, unsigned depth, Evaluate const & evaluate, bool first_turn) {
	assert(depth > 0);
	--depth;
	
	if (get_hp(ai.pokemon()) == 0_bi or get_hp(foe.pokemon()) == 0_bi) {
		return replace(ai, foe, weather, depth, evaluate, first_turn);
	} else if (switch_decision_required(ai.pokemon())) {
		auto const switcher_move_name = is_baton_passing(ai.pokemon()) ? Moves::Baton_Pass : Moves::U_turn;
		auto const & ai_moves = all_moves(ai.pokemon());
		auto const move_it = containers::find(begin(ai_moves), end(ai_moves), switcher_move_name);
		assert(move_it != end(ai_moves));
		return move_then_switch_branch(
			ai,
			*move_it,
			foe,
			bounded::none,
			Variable{},
			Variable{},
			weather,
			depth,
			evaluate,
			CriticalHitFlag{},
			CriticalHitFlag{},
			first_turn
		);
	} else {
		assert(!switch_decision_required(foe.pokemon()));
		return select_move_branch(ai, foe, weather, depth, evaluate, first_turn).move;
	}
}

}	// namespace technicalmachine
