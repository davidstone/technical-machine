// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module tm.strategy.expectimax;

import tm.evaluate.evaluator;
import tm.evaluate.scored_selection;
import tm.evaluate.state;

import tm.move.legal_selections;
import tm.move.selection;

import tm.strategy.weighted_selection;

import tm.environment;
import tm.generation;
import tm.team;

import bounded;
import containers;
import std_module;

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
struct ScoreMovesEvaluator {
	explicit ScoreMovesEvaluator(Strategy const & foe_strategy):
		m_foe_strategy(foe_strategy)
	{
	}
	auto operator()(
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
	auto operator()(
		State<generation> const & state,
		LegalSelections const ai_selections,
		LegalSelections const foe_selections,
		auto const function
	) const -> ScoredSelections {
		return operator()(
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

private:
	std::reference_wrapper<Strategy const> m_foe_strategy;
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
			ScoreMovesEvaluator<generation>(foe_strategy)
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
