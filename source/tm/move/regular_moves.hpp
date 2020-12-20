// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/max_moves_per_pokemon.hpp>
#include <tm/move/move.hpp>
#include <tm/operators.hpp>
#include <tm/move/pp.hpp>

#include <tm/compress.hpp>

#include <containers/static_vector/static_vector.hpp>
#include <containers/begin_end.hpp>
#include <containers/size.hpp>

namespace technicalmachine {

struct RegularMoves {
	using value_type = Move;
	// TODO: Give this a minimum size of 1 when foe Pokemon are handled better
	using size_type = bounded::integer<0, bounded::normalize<max_moves_per_pokemon.value()>>;

	RegularMoves() = default;
	RegularMoves(auto const moves) {
		for (auto const move : moves) {
			push_back(move);
		}
	}
	RegularMoves(std::initializer_list<Move> moves) {
		for (auto const move : moves) {
			push_back(move);
		}
	}

	constexpr auto begin() const & {
		return containers::begin(m_moves);
	}
	constexpr auto begin() & {
		return containers::begin(m_moves);
	}
	constexpr auto size() const {
		return containers::size(m_moves);
	}

	auto push_back(Move const move) -> Move &;

	friend auto operator==(RegularMoves, RegularMoves) -> bool = default;
	friend constexpr auto compress(RegularMoves const value) {
		return compress(value.m_moves);
	}
private:
	containers::static_vector<Move, max_moves_per_pokemon.value()> m_moves;
};

auto add_seen_move(RegularMoves &, Generation, Moves) -> void;

} // namespace technicalmachine
