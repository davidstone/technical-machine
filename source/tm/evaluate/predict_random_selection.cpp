// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.evaluate.predict_random_selection;

import tm.evaluate.predicted;

import tm.move.legal_selections;
import tm.move.move_name;
import tm.move.selection;
import tm.move.switch_;

import bounded;
import containers;
import tv;

namespace technicalmachine {

using namespace bounded::literal;

export constexpr auto predict_random_selection(LegalSelections const selections) -> AllPredicted {
	auto const total_size = double(containers::size(selections));
	return AllPredicted(containers::transform(selections, [=](Selection const selection) {
		return Predicted(selection, 1.0 / total_size);
	}));
}

constexpr auto is_switch = [](Selection const selection) {
	return selection.index() == bounded::type<Switch>;
};

export constexpr auto predict_random_selection(LegalSelections const selections, double const general_switch_probability) -> AllPredicted {
	auto const total_size = containers::size(selections);
	auto const switches = containers::count_if(selections, is_switch);
	auto const switch_probability =
		switches == 0 ? 0.0 :
		switches == total_size ? 1.0 :
		general_switch_probability;
	return AllPredicted(containers::transform(selections, [=](Selection const selection) {
		return Predicted(
			selection,
			is_switch(selection) ?
				switch_probability / double(switches) :
				(1.0 - switch_probability) / double(total_size - switches)
		);
	}));
}

static_assert(
	predict_random_selection(LegalSelections({MoveName::Tackle})) ==
	AllPredicted({{MoveName::Tackle, 1.0}})
);

static_assert(
	predict_random_selection(LegalSelections({MoveName::Tackle, MoveName::Thunder})) ==
	AllPredicted({{MoveName::Tackle, 0.5}, {MoveName::Thunder, 0.5}})
);

static_assert(
	predict_random_selection(LegalSelections({MoveName::Tackle, Switch(0_bi)})) ==
	AllPredicted({{MoveName::Tackle, 0.5}, {Switch(0_bi), 0.5}})
);

static_assert(
	predict_random_selection(LegalSelections({MoveName::Tackle}), 0.2) ==
	AllPredicted({{MoveName::Tackle, 1.0}})
);

static_assert(
	predict_random_selection(LegalSelections({MoveName::Tackle, MoveName::Thunder}), 0.2) ==
	AllPredicted({{MoveName::Tackle, 0.5}, {MoveName::Thunder, 0.5}})
);

static_assert(
	predict_random_selection(LegalSelections({MoveName::Tackle, Switch(0_bi)}), 0.2) ==
	AllPredicted({{MoveName::Tackle, 0.8}, {Switch(0_bi), 0.2}})
);

static_assert(
	predict_random_selection(LegalSelections({Switch(0_bi)}), 0.2) ==
	AllPredicted({{Switch(0_bi), 1.0}})
);

} // namespace technicalmachine
