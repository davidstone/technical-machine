// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.regular_moves;

import tm.move.max_moves_per_pokemon;
import tm.move.move;
import tm.move.move_name;

import tm.string_conversions.move_name;

import tm.compress;
export import tm.operators;

import bounded;
import containers;
import std_module;

namespace technicalmachine {

using namespace std::string_view_literals;

export struct RegularMoves {
	constexpr RegularMoves() = default;
	explicit constexpr RegularMoves(auto const moves) {
		for (auto const move : moves) {
			push_back(move);
		}
	}
	constexpr RegularMoves(std::initializer_list<Move> moves) {
		for (auto const move : moves) {
			push_back(move);
		}
	}

	constexpr auto begin() const {
		return containers::begin(m_moves);
	}
	constexpr auto begin() {
		return containers::begin(m_moves);
	}
	constexpr auto size() const {
		return containers::size(m_moves);
	}

	constexpr auto push_back(Move const move) -> Move & {
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
		return containers::unsafe_push_back(m_moves, move);
	}

	friend auto operator==(RegularMoves, RegularMoves) -> bool = default;
	friend constexpr auto compress(RegularMoves const value) {
		return compress(value.m_moves);
	}

private:
	constexpr explicit RegularMoves(bounded::tombstone_tag, auto const make) noexcept:
		m_moves(make())
	{
	}
	containers::static_vector<Move, max_moves_per_pokemon> m_moves;

	friend bounded::tombstone_traits<RegularMoves>;
	friend bounded::tombstone_traits_composer<&RegularMoves::m_moves>;
};

} // namespace technicalmachine

template<>
struct bounded::tombstone_traits<technicalmachine::RegularMoves> : bounded::tombstone_traits_composer<&technicalmachine::RegularMoves::m_moves> {
};
