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
import tm.pokemon.potentially_selectable_moves;
import tm.pokemon.species;

import tm.stat.calculate;
import tm.stat.chance_to_hit;
import tm.stat.faster;

import tm.status.clears_status;
import tm.status.status_name;

import tm.string_conversions.move_name;
import tm.string_conversions.species;

import tm.type.move_type;

import tm.ability;
import tm.any_team;
import tm.block;
import tm.contact_ability_effect;
import tm.critical_hit_probability;
import tm.end_of_turn;
import tm.end_of_turn_flags;
import tm.environment;
import tm.generation;
import tm.team;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

constexpr double generic_flag_branch(auto const & basic_probability, auto const & next_branch) {
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

template<any_team TeamType>
constexpr auto deorder(TeamType const & first, TeamType const & last) {
	BOUNDED_ASSERT(first.is_me() or last.is_me());
	struct Deorder {
		TeamType const & ai;
		TeamType const & foe;
	};
	return Deorder{
		(first.is_me()) ? first : last,
		(!first.is_me()) ? first : last
	};
}

struct SelectedAndExecuted {
	MoveName selected;
	KnownMove executed;
};

constexpr auto paralysis_probability(StatusName const status) -> double {
	return status == StatusName::paralysis ? 0.25 : 0.0;
}

template<any_team TeamType>
auto execute_move(TeamType const & user, SelectedAndExecuted const move, TeamType const & other, OtherMove const other_move, Environment const environment, Depth const depth, auto const continuation) -> double {
	auto const user_pokemon = user.pokemon();
	auto const other_pokemon = other.pokemon();
	auto const side_effects = possible_side_effects(move.executed.name, user_pokemon, other, environment);
	auto const status = user_pokemon.status();
	auto const probability_of_clearing_status = status.probability_of_clearing(generation_from<TeamType>, user_pokemon.ability());
	auto const specific_chance_to_hit = chance_to_hit(user_pokemon, move.executed, other_pokemon, environment, other_pokemon.last_used_move().moved_this_turn());
	auto const ch_probability = critical_hit_probability(user_pokemon, move.executed.name, other.pokemon().ability(), environment);
	auto const chance_to_be_paralyzed = paralysis_probability(user_pokemon.status().name());
	return generic_flag_branch(probability_of_clearing_status, [&](bool const clear_status) {
		return generic_flag_branch(specific_chance_to_hit, [&](bool const hits) {
			return generic_flag_branch(chance_to_be_paralyzed, [&](bool const is_fully_paralyzed) {
				auto score = 0.0;
				for (auto const & side_effect : side_effects) {
					score += side_effect.probability * generic_flag_branch(
						hits ? ch_probability : 0.0,
						[&](bool const critical_hit) {
							auto user_copy = user;
							auto other_copy = other;
							auto environment_copy = environment;
							// TODO: https://github.com/davidstone/technical-machine/issues/24
							constexpr auto contact_ability_effect = ContactAbilityEffect::nothing;
							call_move(
								user_copy,
								UsedMove<TeamType>(
									move.selected,
									move.executed.name,
									critical_hit,
									!hits,
									contact_ability_effect,
									side_effect.function
								),
								other_copy,
								other_move,
								environment_copy,
								clear_status,
								ActualDamage::Unknown{},
								is_fully_paralyzed
							);
							if (auto const won = win(user_copy, other_copy)) {
								return *won + double(depth.general - 1_bi);
							}
							return continuation(user_copy, other_copy, environment_copy);
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

constexpr auto all_are_pass_or_switch(LegalSelections const legal_selections) {
	return
		(containers::size(legal_selections) == 1_bi and containers::front(legal_selections) == MoveName::Pass) or
		containers::all(legal_selections, is_switch);
}

export template<Generation generation, typename Function>
struct Evaluator {
	explicit Evaluator(Evaluate<generation> const evaluate, Function respond_to_foe_moves):
		m_evaluate(evaluate),
		m_respond_to_foe_moves(std::move(respond_to_foe_moves))
	{
	}

	auto select_type_of_move(Team<generation> const & ai, LegalSelections const ai_selections, Team<generation> const & foe, LegalSelections const foe_selections, Environment const environment, Depth const depth, auto && ... respond_to_foe_moves_args) -> ScoredMoves {
		BOUNDED_ASSERT(!team_is_empty(ai));
		BOUNDED_ASSERT(!team_is_empty(foe));

		if (auto const score = m_transposition_table.get_score(ai, foe, environment, depth)) {
			return *score;
		}

		auto const moves = m_respond_to_foe_moves(
			ai,
			ai_selections,
			foe,
			foe_selections,
			environment,
			m_evaluate,
			depth,
			OPERATORS_FORWARD(respond_to_foe_moves_args)...,
			[&](auto && ... args) {
				return order_branch(OPERATORS_FORWARD(args)...);
			}
		);
		m_transposition_table.add_score(ai, foe, environment, depth, moves);
		return moves;
	}

private:
	auto select_type_of_move(Team<generation> const & ai, Team<generation> const & foe, Environment const environment, Depth const depth) -> ScoredMoves {
		return select_type_of_move(
			ai,
			legal_selections(ai, foe, environment),
			foe,
			legal_selections(foe, ai, environment),
			environment,
			depth
		);
	}

	auto order_branch(Team<generation> const & ai, MoveName const ai_move, Team<generation> const & foe, MoveName const foe_move, Environment const environment, Depth const depth) -> double {
		auto ordered = Order(ai, ai_move, foe, foe_move, environment);
		return !ordered ?
			(use_move_branch(ai, ai_move, foe, foe_move, environment, depth) + use_move_branch(foe, foe_move, ai, ai_move, environment, depth)) / 2.0 :
			use_move_branch(ordered->first.team, ordered->first.move, ordered->second.team, ordered->second.move, environment, depth);
	}

	auto use_move_branch_inner(KnownMove const first_used_move) {
		return [=, this](Team<generation> const & first, MoveName const first_move [[maybe_unused]], Team<generation> const & last, MoveName const last_move, Environment const environment, Depth const depth) {
			BOUNDED_ASSERT_OR_ASSUME(first_move == MoveName::Pass);
			return score_executed_moves(last, last_move, first, first_used_move, environment, depth, [&](Team<generation> const & updated_last, Team<generation> const & updated_first, Environment const updated_environment) {
				auto shed_skin_probability = [&](bool const is_first) {
					auto const pokemon = (is_first ? updated_first : updated_last).pokemon();
					return can_clear_status(pokemon.ability(), pokemon.status().name()) ? 0.3 : 0.0;
				};
				auto const teams = Faster<generation>(updated_first, updated_last, updated_environment);
				return generic_flag_branch(shed_skin_probability, [&](bool const team_shed_skin, bool const other_shed_skin) {
					return generic_flag_branch(
						// TODO
						[&](bool) { return true; },
						[&](bool const team_lock_in_ends, bool const other_lock_in_ends) {
							auto thaws = [&](bool const is_first) {
								if constexpr (generation != Generation::two) {
									return 0.0;
								} else {
									auto const pokemon = is_first ? updated_first.pokemon() : updated_last.pokemon();
									return pokemon.status().name() == StatusName::freeze ? 0.1 : 0.0;
								}
							};
							return generic_flag_branch(
								thaws,
								[&](bool const team_thaws, bool const other_thaws) {
									return end_of_turn_order_branch(
										updated_first,
										updated_last,
										teams,
										updated_environment,
										depth,
										EndOfTurnFlags(team_shed_skin, team_lock_in_ends, team_thaws),
										EndOfTurnFlags(other_shed_skin, other_lock_in_ends, other_thaws)
									);
								}
							);
						}
					);
				});
			});
		};
	}

	auto use_move_branch_outer(OriginalPokemon const original_last_pokemon, MoveName const last_move) {
		return [=, this](Team<generation> const & first, MoveName const first_move, Team<generation> const & last, MoveName, Environment const environment, Depth const depth) {
			return score_executed_moves(first, first_move, last, FutureMove{is_damaging(last_move)}, environment, depth, [&](Team<generation> const & pre_updated_first, Team<generation> const & pre_updated_last, Environment const pre_updated_environment) {
				auto const is_same_pokemon = original_last_pokemon.is_same_pokemon(last.pokemon().species());
				auto const actual_last_move = is_same_pokemon ? last_move : MoveName::Pass;
				auto const first_used_move = original_last_pokemon.other_move();
				return score_executed_moves(pre_updated_last, actual_last_move, pre_updated_first, first_used_move, pre_updated_environment, depth, [&](Team<generation> const & updated_last, Team<generation> const & updated_first, Environment const updated_environment) {
					auto const first_selections = LegalSelections({MoveName::Pass});
					auto const last_selections = legal_selections(updated_last, updated_first, environment);
					return max_score(m_respond_to_foe_moves(updated_first, first_selections, updated_last, last_selections, updated_environment, m_evaluate, depth, use_move_branch_inner(first_used_move)));
				});
			});
		};
	}

	auto use_move_branch(Team<generation> const & first, MoveName const first_move, Team<generation> const & last, MoveName const last_move, Environment const environment, Depth const depth) -> double {
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

		auto const first_pokemon = first.pokemon();
		auto const last_pokemon = last.pokemon();
		BOUNDED_ASSERT(containers::maybe_find(potentially_selectable_moves(first), first_move));
		BOUNDED_ASSERT(containers::maybe_find(potentially_selectable_moves(last), last_move));
		auto const original_last_pokemon = OriginalPokemon(last_pokemon, first_pokemon, first_move);

		return score_executed_moves(first, first_move, last, FutureMove{is_damaging(last_move)}, environment, depth, [&](Team<generation> const & updated_first, Team<generation> const & updated_last, Environment const updated_environment) {
			auto const first_selections = legal_selections(updated_first, updated_last, environment);
			BOUNDED_ASSERT(all_are_pass_or_switch(first_selections));
			auto const last_selections = LegalSelections({MoveName::Pass});
			// TODO: Figure out first / last vs ai / foe
			return max_score(m_respond_to_foe_moves(updated_first, first_selections, updated_last, last_selections, updated_environment, m_evaluate, depth, use_move_branch_outer(original_last_pokemon, last_move)));
		});
	}

	auto end_of_turn_order_branch(Team<generation> const & team, Team<generation> const & other, Faster<generation> const faster, Environment const environment, Depth const depth, EndOfTurnFlags const team_flag, EndOfTurnFlags const other_flag) -> double {
		auto get_flag = [&](Team<generation> const & match) {
			return std::addressof(match) == std::addressof(team) ? team_flag : other_flag;
		};
		return !faster ?
			(end_of_turn_branch(team, other, environment, depth, team_flag, other_flag) + end_of_turn_branch(other, team, environment, depth, other_flag, team_flag)) / 2.0 :
			end_of_turn_branch(faster->first, faster->second, environment, depth, get_flag(faster->first), get_flag(faster->second));
	}

	auto end_of_turn_branch(Team<generation> first, Team<generation> last, Environment environment, Depth const depth, EndOfTurnFlags const first_flag, EndOfTurnFlags const last_flag) -> double {
		end_of_turn(first, first_flag, last, last_flag, environment);
		if (auto const won = win(first, last)) {
			return *won + double(depth.general - 1_bi);
		}
		if (first.pokemon().hp().current() == 0_bi or last.pokemon().hp().current() == 0_bi) {
			auto const first_selections = legal_selections(first, last, environment);
			auto const last_selections = legal_selections(last, first, environment);
			return max_score(m_respond_to_foe_moves(first, first_selections, last, last_selections, environment, m_evaluate, depth, [&](auto && ... args) {
				return handle_end_of_turn_replacing(OPERATORS_FORWARD(args)...);
			}));
		}
		first.reset_start_of_turn();
		last.reset_start_of_turn();
		return finish_end_of_turn(first, last, environment, depth);
	}

	auto handle_end_of_turn_replacing(Team<generation> first, MoveName const first_move, Team<generation> last, MoveName const last_move, Environment environment, Depth const depth) -> double {
		if (first_move != MoveName::Pass) {
			first.switch_pokemon(last.pokemon(), environment, to_replacement(first_move));
		}
		if (last_move != MoveName::Pass) {
			last.switch_pokemon(first.pokemon(), environment, to_replacement(last_move));
		}
		if (auto const won = win(first, last)) {
			return *won + double(depth.general - 1_bi);
		}
		first.reset_start_of_turn();
		last.reset_start_of_turn();
		return finish_end_of_turn(first, last, environment, depth);
	}

	auto finish_end_of_turn(Team<generation> const & first, Team<generation> const & last, Environment const environment, Depth const depth) -> double {
		auto const deordered = deorder(first, last);
		auto const & ai = deordered.ai;
		auto const & foe = deordered.foe;
		auto const new_depth = one_level_deeper(depth);
		if (new_depth.general > 0_bi) {
			return max_score(select_type_of_move(ai, foe, environment, new_depth));
		} else if (new_depth.single > 0_bi) {
			return generate_single_matchups(ai, foe, environment, new_depth);
		} else {
			return static_cast<double>(m_evaluate(ai, foe));
		}
	}

	auto generate_single_matchups(Team<generation> const & ai, Team<generation> const & foe, Environment const environment, Depth const depth) -> double {
		auto score = 0.0;
		for (auto const ai_index : containers::integer_range(ai.size())) {
			for (auto const foe_index : containers::integer_range(foe.size())) {
				score += evaluate_single_matchup(ai, ai_index, foe, foe_index, environment, depth);
			}
		}
		auto const difference = ai.size() - foe.size();
		score += static_cast<double>(difference) * victory<generation> * static_cast<double>(bounded::max(ai.size(), foe.size()));
		score /= static_cast<double>(max_pokemon_per_team * max_pokemon_per_team);
		return score;
	}

	auto evaluate_single_matchup(Team<generation> ai, TeamIndex const ai_index, Team<generation> foe, TeamIndex const foe_index, Environment environment, Depth const depth) -> double {
		// TODO: Something involving switch order
		auto remove_all_but_index = [&](Team<generation> & team, TeamIndex const index, Team<generation> & other) {
			if (index != team.all_pokemon().index()) {
				team.switch_pokemon(other.pokemon(), environment, index);
			}
			team.all_pokemon() = PokemonCollection<Pokemon<generation>>({team.all_pokemon()(team.all_pokemon().index())});
		};
		remove_all_but_index(ai, ai_index, foe);
		remove_all_but_index(foe, foe_index, ai);
		if (auto const won = win(ai, foe)) {
			return *won + double(depth.general - 1_bi);
		}
		return max_score(select_type_of_move(ai, foe, environment, depth));
	}


	auto score_executed_moves(Team<generation> const & user, MoveName const selected_move, Team<generation> const & other, OtherMove const other_move, Environment const environment, Depth const depth, auto const continuation) const -> double {
		double score = 0.0;
		auto const executed_moves = possible_executed_moves(selected_move, user);
		for (auto const executed_move : executed_moves) {
			score += execute_move(user, SelectedAndExecuted{selected_move, executed_move}, other, other_move, environment, depth, continuation);
		}
		return score / static_cast<double>(containers::size(executed_moves));
	}

	Evaluate<generation> m_evaluate;
	[[no_unique_address]] Function m_respond_to_foe_moves;
	TranspositionTable<generation> m_transposition_table;
};

} // namespace technicalmachine
