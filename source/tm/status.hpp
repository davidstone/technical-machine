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

#include <tm/enum.hpp>
#include <tm/operators.hpp>
#include <tm/rational.hpp>

#include <bounded/integer.hpp>
#include <bounded/optional.hpp>

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
	poison_toxic,
	sleep,
	sleep_rest
};

}	// namespace technicalmachine

namespace std {

template<>
struct numeric_limits<technicalmachine::Statuses> : technicalmachine::enum_numeric_limits<technicalmachine::Statuses::sleep_rest> {};

}	// namespace std

namespace technicalmachine {

struct Status {
	constexpr auto name() const -> Statuses {
		return m_status;
	}

	auto rest() {
		m_status = Statuses::sleep_rest;
		m_turns_already_slept = 0_bi;
	}

	friend auto apply(Statuses status, MutableActivePokemon user, MutableActivePokemon target, Weather weather) -> void;
	friend auto shift_status(MutableActivePokemon user, MutableActivePokemon target, Weather weather) -> void;
	
	auto advance_from_move(Ability ability, bool awaken) -> void;
	auto handle_switch(Ability ability) -> void;
	auto end_of_turn(MutableActivePokemon pokemon, Pokemon const & other) -> void;

	// Returns the probability the status can change from sleeping to awake in
	// this move. Returns 0.0 if the Pokemon is already awake or if, due to the
	// sleep counter, they will definitely not awaken.
	auto probability_of_clearing(Ability ability) const -> double;

	friend constexpr auto operator==(Status const lhs, Status const rhs) {
		return
			lhs.m_status == rhs.m_status and
			lhs.m_turns_already_slept == rhs.m_turns_already_slept;
	}

private:
	// TODO: Implement this with std::variant
	using SleepCounter = bounded::optional<bounded::integer<0, 4>>;
	Statuses m_status = Statuses::clear;
	SleepCounter m_turns_already_slept = bounded::none;
};

auto apply(Statuses status, MutableActivePokemon user, MutableActivePokemon target, Weather weather) -> void;
auto shift_status(MutableActivePokemon user, MutableActivePokemon target, Weather weather) -> void;
auto apply(Statuses status, MutableActivePokemon target, Weather weather) -> void;

constexpr auto is_clear(Status const status) {
	return status.name() == Statuses::clear;
}
constexpr auto is_frozen(Status const status) {
	return status.name() == Statuses::freeze;
}
constexpr auto is_sleeping(Status const status) {
	switch (status.name()) {
		case Statuses::sleep_rest:
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
