// Copyright (C) 2019 David Stone
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

#include <tm/enum.hpp>
#include <tm/operators.hpp>
#include <tm/rational.hpp>

#include <bounded/integer.hpp>
#include <bounded/detail/variant/variant.hpp>

#include <cstdint>

namespace technicalmachine {

enum class Ability : std::uint8_t;
struct MutableActivePokemon;
struct Pokemon;
struct Weather;

enum class Statuses : uint8_t {
	clear,
	burn,
	freeze,
	paralysis,
	poison,
	toxic,
	sleep,
	rest
};

}	// namespace technicalmachine

namespace std {

template<>
struct numeric_limits<technicalmachine::Statuses> : technicalmachine::enum_numeric_limits<technicalmachine::Statuses::rest> {};

}	// namespace std

namespace technicalmachine {

struct Status {
	constexpr Status() = default;
	constexpr explicit Status(Statuses const status):
		m_state([=]{
			switch (status) {
				case Statuses::clear: return State(Clear{});
				case Statuses::burn: return State(Burn{});
				case Statuses::freeze: return State(Freeze{});
				case Statuses::paralysis: return State(Paralysis{});
				case Statuses::poison: return State(Poison{});
				case Statuses::toxic: return State(Toxic{});
				case Statuses::sleep: return State(Sleep{});
				case Statuses::rest: return State(Rest{});
			}
		}())
	{
	}

	constexpr auto name() const {
		return static_cast<Statuses>(m_state.index());
	}
	
	constexpr Status & operator=(Statuses const status) & {
		*this = Status(status);
		return *this;
	}

	auto advance_from_move(Ability ability, bool clear) & -> void;
	auto handle_switch(Ability ability) & -> void;
	auto end_of_turn(Pokemon & pokemon, bool is_having_a_nightmare, Pokemon const & other, bool uproar) & -> void;

	// If the current status is sleep or rest, returns the probability the
	// status can change from sleeping to awake on this move. If the current
	// status is freeze, returns the probability of thawing. Returns 0.0 if the
	// Pokemon is not asleep or frozen or if, due to the sleep counter, they
	// will definitely not awaken.
	auto probability_of_clearing(Ability ability) const -> double;

	friend constexpr auto operator==(Status const lhs, Status const rhs) {
		return lhs.m_state == rhs.m_state;
	}

private:
	struct Clear{};
	struct Burn{};
	struct Freeze{};
	struct Paralysis{};
	struct Poison{};
	struct Toxic {
		constexpr auto increment() {
			++m_counter;
		}
		constexpr auto ratio_drained() const {
			return rational(-m_counter, 16_bi);
		}
		friend constexpr auto operator==(Toxic const lhs, Toxic const rhs) {
			return lhs.m_counter == rhs.m_counter;
		}
	private:
		// Number of turns this Pokemon has already taken Toxic damage (or
		// would have if Magic Guard / Poison Heal weren't in play)
		bounded::clamped_integer<0, 15> m_counter = 0_bi;
	};
	struct Sleep {
		bounded::clamped_integer<0, 4> turns_slept = 0_bi;
	};
	struct Rest {
		bounded::clamped_integer<0, 2> turns_slept = 0_bi;
	};

	using State = bounded::variant<
		Clear,
		Burn,
		Freeze,
		Paralysis,
		Poison,
		Toxic,
		Sleep,
		Rest
	>;
	State m_state{Clear{}};
};


constexpr auto is_clear(Status const status) {
	return status.name() == Statuses::clear;
}
constexpr auto is_frozen(Status const status) {
	return status.name() == Statuses::freeze;
}
constexpr auto is_sleeping(Status const status) {
	switch (status.name()) {
		case Statuses::rest:
		case Statuses::sleep:
			return true;
		default:
			return false;
	}
}
constexpr auto is_sleeping_due_to_other(Status const status) {
	return status.name() == Statuses::sleep;
}
constexpr auto weakens_physical_attacks(Status const status) {
	return status.name() == Statuses::burn;
}
constexpr auto boosts_smellingsalt(Status const status) {
	return status.name() == Statuses::paralysis;
}

auto lowers_speed(Status status, Ability ability) -> bool;
auto boosts_facade(Status status) -> bool;

}	// namespace technicalmachine
