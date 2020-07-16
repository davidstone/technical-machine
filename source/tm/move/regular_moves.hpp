// Copyright (C) 2020 David Stone
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

#pragma once

#include <tm/move/max_moves_per_pokemon.hpp>
#include <tm/move/move.hpp>
#include <tm/move/pp.hpp>

#include <tm/compress.hpp>

#include <containers/static_vector/static_vector.hpp>

namespace technicalmachine {

struct RegularMoves {
	using value_type = Move;
	// TODO: Give this a minimum size of 1 when foe Pokemon are handled better
	using size_type = bounded::integer<0, bounded::detail::normalize<max_moves_per_pokemon.value()>>;

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

	friend auto begin(RegularMoves const & container) {
		return begin(container.m_moves);
	}
	friend auto begin(RegularMoves & container) {
		return begin(container.m_moves);
	}
	friend auto end(RegularMoves const & container) {
		return end(container.m_moves);
	}
	friend auto end(RegularMoves & container) {
		return end(container.m_moves);
	}

	auto push_back(Move const move) -> Move &;

	friend auto operator==(RegularMoves const &, RegularMoves const &) -> bool = default;
	friend auto compress(RegularMoves const value) {
		return compress(value.m_moves);
	}
private:
	containers::static_vector<Move, max_moves_per_pokemon.value()> m_moves;
};

auto add_seen_move(RegularMoves &, Generation, Moves) -> void;

} // namespace technicalmachine
