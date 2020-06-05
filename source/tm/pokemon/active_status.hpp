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

#include <tm/generation.hpp>
#include <tm/status.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

struct ActivePokemon;
struct MutableActivePokemon;
struct Weather;

struct ActiveStatus {
	constexpr auto set(Statuses const status) & -> void {
		switch (status) {
			case Statuses::rest:
			case Statuses::sleep:
				m_nightmare = false;
				break;
			case Statuses::toxic:
				m_toxic_counter = 1_bi;
				break;
			default:
				m_none = {};
				break;
		}
	}
	constexpr auto give_nightmares() & -> void {
		m_nightmare = true;
	}

	auto status_and_leech_seed_effects(Generation, MutableActivePokemon pokemon, MutableActivePokemon const other, Weather, bool uproar) & -> void;

	// TODO: ???
	friend auto operator==(ActiveStatus const & lhs, ActiveStatus const & rhs) -> bool {
		// TODO: std::bit_cast
		return reinterpret_cast<std::byte const &>(lhs) == reinterpret_cast<std::byte const &>(rhs);
	}
private:
	auto end_of_attack(Generation, MutableActivePokemon pokemon, MutableActivePokemon const other, Weather) & -> void;
	auto end_of_turn(Generation, MutableActivePokemon pokemon, MutableActivePokemon const other, Weather, bool uproar) & -> void;
	// The discriminator is the status of the active Pokemon.
	union {
		std::byte m_none{};
		bool m_nightmare;
		bounded::clamped_integer<1, 15> m_toxic_counter;
	};
};

} // namespace technicalmachine
