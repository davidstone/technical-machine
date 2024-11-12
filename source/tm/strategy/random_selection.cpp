// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.strategy.random_selection;

import tm.move.legal_selections;
import tm.move.move_name;
import tm.move.selection;
import tm.move.switch_;

import tm.strategy.strategy;
import tm.strategy.weighted_selection;

import tm.environment;
import tm.generation;
import tm.team;

import bounded;
import containers;
import tv;

namespace technicalmachine {

using namespace bounded::literal;

constexpr auto random_selection(LegalSelections const selections) -> WeightedSelections {
	return WeightedSelections(containers::transform(selections, [](Selection const selection) {
		return WeightedSelection(selection, 1.0);
	}));
}

constexpr auto is_switch = [](Selection const selection) {
	return selection.index() == bounded::type<Switch>;
};

constexpr auto random_selection(LegalSelections const selections, double const switch_probability) -> WeightedSelections {
	auto const total_size = containers::size(selections);
	auto const switches = containers::count_if(selections, is_switch);
	return WeightedSelections(containers::transform(selections, [=](Selection const selection) {
		return WeightedSelection(
			selection,
			is_switch(selection) ?
				switch_probability / double(switches) :
				(1.0 - switch_probability) / double(total_size - switches)
		);
	}));
}

static_assert(
	random_selection(LegalSelections({MoveName::Tackle})) ==
	WeightedSelections({{MoveName::Tackle, 1.0}})
);

static_assert(
	random_selection(LegalSelections({MoveName::Tackle, MoveName::Thunder})) ==
	WeightedSelections({{MoveName::Tackle, 1.0}, {MoveName::Thunder, 1.0}})
);

static_assert(
	random_selection(LegalSelections({MoveName::Tackle, Switch(0_bi)})) ==
	WeightedSelections({{MoveName::Tackle, 1.0}, {Switch(0_bi), 1.0}})
);

static_assert(
	random_selection(LegalSelections({MoveName::Tackle}), 0.2) ==
	WeightedSelections({{MoveName::Tackle, 0.8}})
);

static_assert(
	random_selection(LegalSelections({MoveName::Tackle, MoveName::Thunder}), 0.2) ==
	WeightedSelections({{MoveName::Tackle, 0.4}, {MoveName::Thunder, 0.4}})
);

static_assert(
	random_selection(LegalSelections({MoveName::Tackle, Switch(0_bi)}), 0.2) ==
	WeightedSelections({{MoveName::Tackle, 0.8}, {Switch(0_bi), 0.2}})
);

static_assert(
	random_selection(
		LegalSelections({MoveName::Tackle, MoveName::Thunder, Switch(0_bi)}),
		0.2
	) ==
	WeightedSelections({
		{MoveName::Tackle, 0.4},
		{MoveName::Thunder, 0.4},
		{Switch(0_bi), 0.2}
	})
);

static_assert(
	random_selection(
		LegalSelections({MoveName::Tackle, Switch(0_bi), Switch(1_bi)}),
		0.2
	) ==
	WeightedSelections({
		{MoveName::Tackle, 0.8},
		{Switch(0_bi), 0.1},
		{Switch(1_bi), 0.1}
	})
);

static_assert(
	random_selection(LegalSelections({Switch(0_bi)}), 0.2) ==
	WeightedSelections({{Switch(0_bi), 0.2}})
);

export auto make_random_selection() -> Strategy {
	return Strategy([]<Generation generation>(
		[[maybe_unused]] Team<generation> const & ai,
		LegalSelections const ai_selections,
		[[maybe_unused]] Team<generation> const & foe,
		[[maybe_unused]] LegalSelections const foe_selections,
		[[maybe_unused]] Environment const environment
	) -> WeightedSelections {
		return random_selection(ai_selections);
	});
}

export auto make_random_selection(double const switch_probability) -> Strategy {
	return Strategy([=]<Generation generation>(
		[[maybe_unused]] Team<generation> const & ai,
		LegalSelections const ai_selections,
		[[maybe_unused]] Team<generation> const & foe,
		[[maybe_unused]] LegalSelections const foe_selections,
		[[maybe_unused]] Environment const environment
	) -> WeightedSelections {
		return random_selection(ai_selections, switch_probability);
	});
}

} // namespace technicalmachine
