// Evaluate header
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

#pragma once

#include <tm/stat/stage.hpp>
#include <tm/stat/stat_names.hpp>

#include <bounded/integer.hpp>
#include <bounded/optional.hpp>

#include <containers/array/array.hpp>

#include <cstdint>

namespace technicalmachine {
using namespace bounded::literal;

enum class Generation : std::uint8_t;
struct EntryHazards;
struct Pokemon;
struct Status;
struct Team;
struct Weather;

// 100% chance to win
constexpr auto victory = 540672_bi;

struct Evaluate {
	// +1 gives me room to create a value that will always be overwritten
	using type = bounded::integer<-static_cast<int>(victory + 1_bi), static_cast<int>(victory + 1_bi)>;
	Evaluate();
	auto operator()(Generation, Team const & ai, Team const & foe, Weather weather) const -> type;
	// Return victory if the battle is won. Returns -victory if the battle is
	// lost. Returns 0 otherwise.
	static auto win(Team const & team1, Team const & team2) -> bounded::optional<double>;

	// Arbitrary values
	using value_type = bounded::integer<-4096, 4096>;

	auto spikes() const { return m_spikes; }
	auto stealth_rock() const { return m_stealth_rock; }
	auto toxic_spikes() const { return m_toxic_spikes; }
	auto hp() const { return m_hp; }
	auto hidden() const { return m_hidden; }

private:
	value_type m_spikes;
	value_type m_stealth_rock;
	value_type m_toxic_spikes;

	value_type m_hp;
	value_type m_hidden;
};

} // namespace technicalmachine
