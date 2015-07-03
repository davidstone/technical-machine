// Copyright (C) 2015 David Stone
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
#include "hash.hpp"
#include "rational.hpp"

#include <bounded_integer/bounded_integer.hpp>
#include <bounded_integer/optional.hpp>

#include <cstdint>

namespace technicalmachine {

struct Ability;
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
	sleep_rest,
	END
};

}	// namespace technicalmachine

namespace bounded {

template<>
struct basic_numeric_limits<technicalmachine::Statuses> : technicalmachine::basic_numeric_limits<technicalmachine::Statuses> {};

}	// namespace bounded

namespace technicalmachine {

struct Status {
	constexpr auto name() const -> Statuses {
		return m_status;
	}

	auto rest() -> void;
	template<Statuses real_status, Statuses base_status = real_status>
	static auto apply(Pokemon & user, Pokemon & target, Weather weather) -> void;
	template<Statuses real_status>
	static auto apply(Pokemon & target, Weather weather) -> void;
	static auto shift (Pokemon & user, Pokemon & target, Weather weather) -> void;

	friend auto operator==(Status lhs, Status rhs) -> bool;

	auto increase_sleep_counter (Ability const & ability, bool awaken) -> void;

	// Returns the probability the status can change from sleeping to awake in
	// this move. Returns 0.0 if the Pokemon is already awake or if, due to the
	// sleep counter, they will definitely not awaken.
	using AwakenProbability = double;
	auto awaken_probability(Ability const & ability) const -> AwakenProbability;

	constexpr auto hash() const noexcept {
		return technicalmachine::hash(name(), m_turns_already_slept);
	}
private:
	using SleepCounter = bounded::optional<bounded::integer<0, 4>>;

	Statuses m_status = Statuses::clear;
	SleepCounter m_turns_already_slept = bounded::none;
};

auto is_clear(Status status) -> bool;
auto is_frozen(Status status) -> bool;
auto is_sleeping(Status status) -> bool;
auto is_sleeping_due_to_other(Status status) -> bool;
auto lowers_speed(Status status, Ability const & ability) -> bool;
auto weakens_physical_attacks(Status status) -> bool;
auto boosts_facade(Status status) -> bool;
auto boosts_smellingsalt(Status status) -> bool;

auto operator!=(Status lhs, Status rhs) -> bool;

constexpr auto hash(Status const status) noexcept {
	return status.hash();
}

}	// namespace technicalmachine
