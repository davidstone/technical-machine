// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/move/regular_moves.hpp>
#include <tm/move/move_name.hpp>

#include <tm/string_conversions/move_name.hpp>

#include <containers/algorithms/all_any_none.hpp>
#include <containers/algorithms/concatenate.hpp>
#include <containers/push_back.hpp>
#include <containers/size.hpp>

#include <bounded/assert.hpp>

#include <stdexcept>
#include <string>
#include <string_view>

namespace technicalmachine {

using namespace std::string_view_literals;

auto RegularMoves::push_back(Move const move) -> Move & {
	if (!is_regular(move.name())) {
		throw std::runtime_error(containers::concatenate<std::string>("Tried to add an irregular move "sv, to_string(move.name())));
	}
	if (containers::any_equal(m_moves, move.name())) {
		throw std::runtime_error(containers::concatenate<std::string>("Tried to add "sv, to_string(move.name()), "twice"sv));
	}
	if (containers::size(m_moves) == max_moves_per_pokemon) {
		auto message = std::string("Tried to add too many moves. Already have: ");
		for (auto const existing_move : m_moves) {
			message += to_string(existing_move.name());
			message += ", ";
		}
		message += "-- Tried to add ";
		message += to_string(move.name());
		throw std::runtime_error(message);
	}
	return containers::push_back(m_moves, move);
}

} // namespace technicalmachine