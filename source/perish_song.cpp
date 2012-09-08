// Perish Song duration
// Copyright (C) 2012 David Stone
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

#include "perish_song.hpp"

namespace technicalmachine {
namespace {
constexpr unsigned max_duration = 3;
}	// unnamed namespace

PerishSong::PerishSong() :
	duration(0)
	{
}

bool PerishSong::is_active() const {
	return duration != 0;
}

void PerishSong::activate() {
	if (!is_active())
		duration = max_duration;
}

void PerishSong::reset() {
	duration = 0;
}

bool PerishSong::next_turn() {
	if (!is_active())
		return false;
	--duration;
	return duration == 0;
}

PerishSong::hash_type PerishSong::hash() const {
	return duration;
}

PerishSong::hash_type PerishSong::max_hash() {
	return max_duration;
}

bool operator== (PerishSong const & lhs, PerishSong const & rhs) {
	return lhs.duration == rhs.duration;
}

bool operator!= (PerishSong const & lhs, PerishSong const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
