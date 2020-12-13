// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/max_moves_per_pokemon.hpp>

#include <containers/static_vector/static_vector.hpp>

#include <tm/generation.hpp>

namespace technicalmachine {

struct MoveScores {
	MoveScores(Generation, StaticVectorMove legal_selections, bool ai);
	void set(Moves move_name, double score);
	auto ordered_moves(bool ai) const -> StaticVectorMove;
private:
	struct value_type {
		Moves move_name;
		double score;
	};
	containers::static_vector<value_type, bounded::detail::builtin_max_value<MoveSize>> m_scores;
};

}	// namespace technicalmachine
