// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.strategy.expectimax.generic_flag_branch;

import tm.evaluate.score;

import tm.probability;

import bounded;
import containers;

namespace technicalmachine {

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

export constexpr auto multi_generic_flag_branch(auto const & basic_probability, auto const & next_branch) -> Score {
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

export constexpr auto generic_flag_branch(Probability const basic_probability, auto const & next_branch) -> Score {
	return containers::sum(containers::transform(
		probabilities(bounded::value_to_function(basic_probability)),
		[&](FlagProbability const x) { return x.probability * next_branch(x.flag); }
	));
}

} // namespace technicalmachine
