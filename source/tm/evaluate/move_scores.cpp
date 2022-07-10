// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/evaluate/move_scores.hpp>

#include <tm/evaluate/evaluate.hpp>

#include <tm/constant_generation.hpp>

#include <bounded/assert.hpp>

#include <containers/algorithms/maybe_find.hpp>
#include <containers/algorithms/sort.hpp>
#include <containers/algorithms/transform.hpp>

#include <numeric_traits/min_max_value.hpp>

namespace technicalmachine {

MoveScores::MoveScores(Generation const generation, LegalSelections const legal_selections, bool const ai):
	m_scores(
		containers::transform(legal_selections, [=](MoveName const move) {
			auto cg = []<Generation g>(constant_gen_t<g>) {
				return victory<g>;
			};
			auto const v = constant_generation(generation, cg);
			return value_type{
				move,
				ai ? -v - 1.0 : v + 1.0
			};
		})
	)
{
}

void MoveScores::set(MoveName const move_name, double const score) {
	auto const ptr = containers::maybe_find_if(m_scores, [=](value_type const value) {
		return value.move_name == move_name;
	});
	BOUNDED_ASSERT(ptr);
	ptr->score = score;
}

auto MoveScores::ordered_moves(bool const ai) const -> LegalSelections {
	auto copy = m_scores;
	auto compare = [=](value_type const lhs, value_type const rhs) {
		return ai ? lhs.score > rhs.score : lhs.score < rhs.score;
	};
	containers::sort(copy, compare);
	return LegalSelections(containers::transform(copy, [](value_type const value) { return value.move_name; }));
}

} // namespace technicalmachine