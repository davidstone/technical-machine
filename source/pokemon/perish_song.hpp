// Perish Song duration
// Copyright (C) 2014 David Stone
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

#ifndef PERISH_SONG_HPP_
#define PERISH_SONG_HPP_

#include "../hash.hpp"

#include <bounded_integer/optional.hpp>

namespace technicalmachine {

class PerishSong {
public:
	auto activate() -> void;
	// return value: whether this Pokemon faints
	auto advance_one_turn() -> bool;
	friend auto operator==(PerishSong lhs, PerishSong rhs) -> bool;
	constexpr auto hash() const noexcept {
		return technicalmachine::hash(m_turns_active);
	}
	constexpr auto max_hash() const noexcept {
		return technicalmachine::max_hash(m_turns_active);
	}
private:
	auto is_active() const -> bool;
	using type = bounded::integer<0, 2>;
	bounded::optional<type> m_turns_active;
};

auto operator!=(PerishSong lhs, PerishSong rhs) -> bool;

constexpr auto hash(PerishSong const perish_song) noexcept {
	return perish_song.hash();
}
constexpr auto max_hash(PerishSong const perish_song) noexcept {
	return perish_song.max_hash();
}

}	// namespace technicalmachine
#endif	// PERISH_SONG_HPP_
