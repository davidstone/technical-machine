// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module tm.strategy.net_hp;

import tm.evaluate.score;
import tm.evaluate.scored_selection;

import tm.move.actual_damage;
import tm.move.call_move;
import tm.move.irrelevant_action;
import tm.move.legal_selections;
import tm.move.move_name;
import tm.move.other_action;
import tm.move.pass;
import tm.move.selection;
import tm.move.side_effects;
import tm.move.switch_;
import tm.move.used_move;

import tm.pokemon.pokemon;

import tm.strategy.strategy;
import tm.strategy.weighted_selection;

import tm.environment;
import tm.generation;
import tm.team;

import containers;
import tv;

namespace technicalmachine {

template<Generation generation>
constexpr auto individual_score(Team<generation> const & team) -> Score {
	return containers::sum(containers::transform(
		team.all_pokemon(),
		[](Pokemon<generation> const & pokemon) -> Score {
			return Score(double(pokemon.hp().current()) / double(pokemon.hp().max()));
		}
	));
}

template<Generation generation>
constexpr auto max_net_hp(
	Team<generation> const & original_user,
	LegalSelections const legal_selections,
	Team<generation> const & original_other,
	Environment const original_environment
) -> WeightedSelections {
	auto const scores = containers::make_static_vector(containers::transform(
		legal_selections,
		[&](Selection const selection) -> ScoredSelection {
			return tv::visit(selection, tv::overload(
				[&](MoveName const move) -> ScoredSelection {
					auto user = original_user;
					auto other = original_other;
					auto environment = original_environment;
					auto const side_effects = possible_side_effects(
						move,
						user.pokemon().as_const(),
						other,
						environment
					);

					auto const side_effect = containers::max_element(
						side_effects,
						[](auto const & lhs, auto const & rhs) {
							return lhs.probability > rhs.probability;
						}
					);
					// This means it will never pick Sucker Punch and Counter and
					// usually pick Focus Punch.
					constexpr auto other_action = OtherAction(IrrelevantAction());
					constexpr auto clear_status = false;
					constexpr auto actual_damage = ActualDamage::Unknown();
					constexpr auto is_fully_paralyzed = false;
					call_move(
						user,
						UsedMove<Team<generation>>(
							move,
							side_effect->function
						),
						other,
						other_action,
						environment,
						clear_status,
						actual_damage,
						is_fully_paralyzed
					);
					return ScoredSelection(
						selection,
						individual_score(user) - individual_score(other)
					);
				},
				[&](Switch const switch_) -> ScoredSelection {
					auto user = original_user;
					auto other = original_other;
					auto environment = original_environment;
					user.switch_pokemon(
						other.pokemon(),
						environment,
						switch_.value()
					);
					return ScoredSelection(
						selection,
						individual_score(user) - individual_score(other)
					);
				},
				[](Pass) -> ScoredSelection {
					return ScoredSelection(pass, Score(0.0));
				}
			));
		}
	));
	auto const best = containers::max_element(
		scores,
		[&](ScoredSelection const lhs, ScoredSelection const rhs) {
			return lhs.score > rhs.score;
		}
	);
	auto const all_best = containers::filter(
		scores,
		[&](ScoredSelection const element) {
			return element.score == best->score;
		}
	);
	auto const best_size = double(containers::linear_size(all_best));
	return WeightedSelections(
		containers::transform(all_best, [&](ScoredSelection const element) {
			return WeightedSelection(element.selection, 1.0 / best_size);
		})
	);
}

auto make_net_hp() -> Strategy {
	return Strategy([]<Generation generation>(
		Team<generation> const & ai,
		LegalSelections const ai_selections,
		Team<generation> const & foe,
		[[maybe_unused]] LegalSelections const foe_selections,
		Environment const environment
	) -> WeightedSelections {
		return max_net_hp(ai, ai_selections, foe, environment);
	});
}

} // namespace technicalmachine
