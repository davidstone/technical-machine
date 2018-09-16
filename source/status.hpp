// Copyright (C) 2018 David Stone
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

#include "enum.hpp"
#include "operators.hpp"
#include "rational.hpp"

#include <bounded/integer.hpp>
#include <bounded/optional.hpp>

#include <cstdint>

namespace technicalmachine {

enum class Ability : std::uint8_t;
struct Pokemon;
struct Weather;

enum class Statuses : uint8_t {
	clear,
	burn,
	freeze,
	paralysis,
	poison,
	poison_toxic,
	sleep,
	sleep_rest
};

}	// namespace technicalmachine

namespace std {

template<>
struct numeric_limits<technicalmachine::Statuses> : technicalmachine::enum_numeric_limits<technicalmachine::Statuses::sleep_rest> {};

}	// namespace bounded

namespace technicalmachine {

struct Status {
	constexpr auto name() const -> Statuses {
		return m_status;
	}

	auto rest() -> void;
	friend auto apply(Statuses status, Pokemon & user, Pokemon & target, Weather weather) -> void;
	friend auto shift_status(Pokemon & user, Pokemon & target, Weather weather) -> void;

	friend constexpr auto compare(Status const lhs, Status const rhs) {
		BOUNDED_COMPARE_ONE_MEMBER(m_status);
		return compare(lhs.m_turns_already_slept, rhs.m_turns_already_slept);
	}

	auto increase_sleep_counter (Ability const & ability, bool awaken) -> void;

	// Returns the probability the status can change from sleeping to awake in
	// this move. Returns 0.0 if the Pokemon is already awake or if, due to the
	// sleep counter, they will definitely not awaken.
	using AwakenProbability = double;
	auto awaken_probability(Ability const & ability) const -> AwakenProbability;

private:
	// TODO: Implement this with std::variant
	using SleepCounter = bounded::optional<bounded::integer<0, 4>>;
	Statuses m_status = Statuses::clear;
	SleepCounter m_turns_already_slept = bounded::none;
};

auto apply(Statuses status, Pokemon & user, Pokemon & target, Weather weather) -> void;
auto shift_status(Pokemon & user, Pokemon & target, Weather weather) -> void;
auto apply(Statuses status, Pokemon & target, Weather weather) -> void;
auto is_clear(Status status) -> bool;
auto is_frozen(Status status) -> bool;
auto is_sleeping(Status status) -> bool;
auto is_sleeping_due_to_other(Status status) -> bool;
auto lowers_speed(Status status, Ability const & ability) -> bool;
auto weakens_physical_attacks(Status status) -> bool;
auto boosts_facade(Status status) -> bool;
auto boosts_smellingsalt(Status status) -> bool;

}	// namespace technicalmachine
