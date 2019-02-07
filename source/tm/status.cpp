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

#include <tm/status.hpp>

#include <algorithm>

#include <tm/ability.hpp>
#include <tm/heal.hpp>
#include <tm/weather.hpp>

#include <tm/pokemon/active_pokemon.hpp>

#include <tm/type/type.hpp>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

auto status_can_apply(Statuses const status, MutableActivePokemon user, MutableActivePokemon target, Weather const weather) {
	return
		is_clear(get_status(target)) and
		(ignores_blockers(get_ability(user)) or !blocks_status(get_ability(target), status, weather)) and
		!blocks_status(get_type(target), status) and
		!weather.blocks_status(status) and
		(
			(status != Statuses::sleep and status != Statuses::sleep_rest) or
			(!is_uproaring(user) and !is_uproaring(target))
		);
}

constexpr auto reflected_status(Statuses const status) -> bounded::optional<Statuses> {
	switch (status) {
	case Statuses::burn:
	case Statuses::paralysis:
	case Statuses::poison:
		return status;
	case Statuses::poison_toxic:
		return Statuses::poison;
	case Statuses::clear:
	case Statuses::freeze:
	case Statuses::sleep:
	case Statuses::sleep_rest:
		return bounded::none;
	}
}

}	// namespace

auto lowers_speed(Status const status, Ability const ability) -> bool {
	return status.name() == Statuses::paralysis and !blocks_paralysis_speed_penalty(ability);
}
auto boosts_facade(Status const status) -> bool {
	switch (status.name()) {
	case Statuses::burn:
	case Statuses::paralysis:
	case Statuses::poison:
	case Statuses::poison_toxic:
		return true;
	case Statuses::clear:
	case Statuses::freeze:
	case Statuses::sleep:
	case Statuses::sleep_rest:
		return false;
	}
}

auto apply(Statuses const status, MutableActivePokemon user, MutableActivePokemon target, Weather const weather) -> void {
	assert(status != Statuses::clear);
	assert(status != Statuses::sleep_rest);
	if (status_can_apply(status, user, target, weather)) {
		get_status(target).m_status = status;
		if (status == Statuses::sleep) {
			get_status(target).m_turns_already_slept = 0_bi;
		}
		auto const reflected = reflected_status(status);
		if (reflected and reflects_status(get_ability(target))) {
			apply(*reflected, target, user, weather);
		}
	}
}

auto apply(Statuses const status, MutableActivePokemon target, Weather const weather) -> void {
	apply(status, target, target, weather);
}

auto shift_status(MutableActivePokemon user, MutableActivePokemon target, Weather const weather) -> void {
	auto & status = get_status(user);
	switch (status.name()) {
		case Statuses::burn:
		case Statuses::paralysis:
		case Statuses::poison:
		case Statuses::poison_toxic:
			apply(status.name(), user, target, weather);
			break;
		case Statuses::sleep:
		case Statuses::sleep_rest:		// Fix
			apply(Statuses::sleep, user, target, weather);
			get_status(target).m_turns_already_slept = status.m_turns_already_slept;
			break;
		default:
			break;
	}
	status = Status{};
}

auto Status::advance_from_move(Ability const ability, bool awaken) -> void {
	if (awaken) {
		m_turns_already_slept = bounded::none;
		m_status = Statuses::clear;
	} else {
		*m_turns_already_slept += BOUNDED_CONDITIONAL(wakes_up_early(ability), 2_bi, 1_bi);
	}
}

namespace {

constexpr auto max_sleep_turns = 4_bi;
using DefiniteSleepCounter = bounded::integer<0, static_cast<intmax_t>(max_sleep_turns)>;

// It's possible to acquire Early Bird in the middle of a sleep
auto early_bird_probability(DefiniteSleepCounter const turns_slept) {
	switch (turns_slept.value()) {
	case 0:
		return 1.0 / 4.0;
	case 1:
		return 1.0 / 2.0;
	case 2:
		return 2.0 / 3.0;
	default:	// case 3, 4
		return 1.0;
	}
}

auto non_early_bird_probability(DefiniteSleepCounter const turns_slept) {
	return (turns_slept == 0_bi) ? 0.0 : (1.0 / static_cast<double>(max_sleep_turns + 1_bi - turns_slept));
}

}	// namespace

auto Status::handle_switch(Ability const ability) -> void {
	if (clears_status_on_switch(ability)) {
		*this = {};
	}
}

auto Status::end_of_turn(MutableActivePokemon pokemon, Pokemon const & other) -> void {
	switch (name()) {
		case Statuses::burn: {
			auto const denominator = BOUNDED_CONDITIONAL(weakens_burn(get_ability(pokemon)), 16_bi, 8_bi);
			heal(pokemon, rational(-1_bi, denominator));
			break;
		}
		case Statuses::poison: {
			auto const numerator = BOUNDED_CONDITIONAL(absorbs_poison_damage(get_ability(pokemon)), 1_bi, -1_bi);
			heal(pokemon, rational(numerator, 8_bi));
			break;
		}
		case Statuses::poison_toxic:
			pokemon.advance_toxic();
			if (absorbs_poison_damage(get_ability(pokemon))) {
				heal(pokemon, rational(1_bi, 8_bi));
			} else {
				heal(pokemon, toxic_ratio(pokemon));
			}
			break;
		case Statuses::sleep:
			if (is_having_a_nightmare(pokemon)) {
				heal(pokemon, rational(-1_bi, 4_bi));
			}
			if (harms_sleepers(get_ability(other))) {
				heal(pokemon, rational(-1_bi, 8_bi));
			}
			break;
		default:
			break;
	}
}

auto Status::probability_of_clearing(Ability const ability) const -> double {
	static_assert(DefiniteSleepCounter::min() == SleepCounter::value_type::min());
	static_assert(DefiniteSleepCounter::max() == SleepCounter::value_type::max());
	if (!m_turns_already_slept) {
		return 0.0;
	}
	return wakes_up_early(ability) ?
		early_bird_probability(*m_turns_already_slept) :
		non_early_bird_probability(*m_turns_already_slept)
	;
}

}	// namespace technicalmachine
