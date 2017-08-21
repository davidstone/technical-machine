// Collection of moves with index indicating current move
// Copyright (C) 2017 David Stone
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

#include "container.hpp"
#include "max_moves_per_pokemon.hpp"
#include "move.hpp"
#include "moves.hpp"

#include "../collection.hpp"
#include "../operators.hpp"

#include <bounded/integer_range.hpp>
#include <bounded/optional.hpp>

#include <containers/algorithms/find.hpp>

#include <cassert>

namespace technicalmachine {
using namespace bounded::literal;

struct MoveCollection : private MoveContainer {
	using MoveContainer::MoveContainer;

	using typename MoveContainer::value_type;
	using typename MoveContainer::size_type;
	using typename MoveContainer::const_iterator;

	using MoveContainer::emplace_back;
	using MoveContainer::regular;
	using MoveContainer::remove_switch;

	friend decltype(auto) begin(MoveCollection const & collection) {
		return begin(static_cast<MoveContainer const &>(collection));
	}
	friend decltype(auto) begin(MoveCollection & collection) {
		return begin(static_cast<MoveContainer &>(collection));
	}
	friend decltype(auto) end(MoveCollection const & collection) {
		return end(static_cast<MoveContainer const &>(collection));
	}
	friend decltype(auto) end(MoveCollection & collection) {
		return end(static_cast<MoveContainer &>(collection));
	}

	constexpr auto index() const {
		return m_current_index;
	}
	void set_index(containers::index_type<MoveCollection> const new_index) {
		m_current_index = (new_index < containers::size(*this)) ?
			new_index :
			throw InvalidCollectionIndex(new_index, containers::size(*this), value_type::class_name);
	}

private:
	containers::index_type<MoveCollection> m_current_index = 0_bi;
};

inline auto set_index(MoveCollection & moves, Moves const move) -> void {
	auto const it = containers::find(begin(moves), end(moves), move);
	assert(it != end(moves));
	moves.set_index(static_cast<containers::index_type<MoveCollection>>(it - begin(moves)));
}

inline auto index(MoveCollection const & moves, Moves const name) -> bounded::optional<RegularMoveIndex> {
	auto const it = containers::find(begin(moves.regular()), end(moves.regular()), name);
	return (it != end(moves.regular())) ?
		bounded::optional<RegularMoveIndex>(it - begin(moves.regular())) :
		bounded::none;
}

template<typename M, typename... MaybePP>
inline auto & add_seen_move(MoveCollection & moves, M const move, MaybePP... maybe_pp) {
	auto const regular = moves.regular();
	auto const it = containers::find(begin(regular), end(regular), move);
	if (it != end(regular)) {
		moves.set_index(static_cast<containers::index_type<MoveCollection>>(it - begin(regular)));
		return *it;
	} else {
		auto const new_index = size(moves.regular());
		auto & result = moves.emplace_back(move, maybe_pp...);
		moves.set_index(static_cast<containers::index_type<MoveCollection>>(new_index));
		return result;
	}
}


}	// namespace technicalmachine
