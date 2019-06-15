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

#include <tm/ability.hpp>
#include <tm/heal.hpp>

#include <tm/pokemon/pokemon.hpp>

#include <bounded/assert.hpp>
#include <bounded/detail/overload.hpp>

namespace technicalmachine {
using namespace bounded::literal;

auto lowers_speed(Status const status, Ability const ability) -> bool {
	return status.name() == Statuses::paralysis and !blocks_paralysis_speed_penalty(ability);
}
auto boosts_facade(Status const status) -> bool {
	switch (status.name()) {
	case Statuses::burn:
	case Statuses::paralysis:
	case Statuses::poison:
	case Statuses::toxic:
		return true;
	case Statuses::clear:
	case Statuses::freeze:
	case Statuses::sleep:
	case Statuses::rest:
		return false;
	}
}

auto Status::advance_from_move(Ability const ability, bool const clear) & -> void {
	if (clear) {
		m_state = Clear{};
	} else {
		auto advance_sleep = [&](auto & sleep) {
			sleep.turns_slept += BOUNDED_CONDITIONAL(wakes_up_early(ability), 2_bi, 1_bi);
		};
		bounded::visit(m_state, bounded::overload(
			[=](Sleep & sleep) { advance_sleep(sleep); },
			[=](Rest & sleep) { advance_sleep(sleep); },
			[](auto) {}
		));
	}
}

auto Status::handle_switch(Ability const ability) & -> void {
	if (clears_status_on_switch(ability)) {
		*this = {};
	} else {
		bounded::visit(m_state, bounded::overload(
			[](Toxic & toxic) { toxic = {}; },
			[](auto) {}
		));
	}
}

auto Status::end_of_turn(Pokemon & pokemon, bool const is_having_a_nightmare, Pokemon const & other, bool const uproar) & -> void {
	auto handle_sleep = [&]{
		if (uproar) {
			m_state = Clear{};
			return;
		}
		if (is_having_a_nightmare) {
			heal(pokemon, rational(-1_bi, 4_bi));
		}
		if (harms_sleepers(get_ability(other))) {
			heal(pokemon, rational(-1_bi, 8_bi));
		}
	};
	bounded::visit(m_state, bounded::overload(
		[](Clear) {},
		[&](Burn) {
			auto const denominator = BOUNDED_CONDITIONAL(weakens_burn(get_ability(pokemon)), 16_bi, 8_bi);
			heal(pokemon, rational(-1_bi, denominator));
		},
		[](Freeze) {},
		[](Paralysis) {},
		[&](Poison) {
			auto const numerator = BOUNDED_CONDITIONAL(absorbs_poison_damage(get_ability(pokemon)), 1_bi, -1_bi);
			heal(pokemon, rational(numerator, 8_bi));
		},
		[&](Toxic & toxic) {
			toxic.increment();
			if (absorbs_poison_damage(get_ability(pokemon))) {
				heal(pokemon, rational(1_bi, 8_bi));
			} else {
				heal(pokemon, toxic.ratio_drained());
			}
		},
		[&](Sleep) { handle_sleep(); },
		[&](Rest) { handle_sleep(); }
	));
}

namespace {

using SleepCounter = bounded::integer<0, 4>;

// It's possible to acquire Early Bird in the middle of a sleep
constexpr auto early_bird_probability(SleepCounter const turns_slept) {
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

constexpr auto non_early_bird_probability(SleepCounter const turns_slept) {
	return (turns_slept == 0_bi) ? 0.0 : (1.0 / static_cast<double>(SleepCounter::max() + 1_bi - turns_slept));
}

constexpr auto awaken_probability(SleepCounter const turns_slept, Ability const ability) {
	return wakes_up_early(ability) ?
		early_bird_probability(turns_slept) :
		non_early_bird_probability(turns_slept)
	;
}

}	// namespace

auto Status::probability_of_clearing(Ability const ability) const -> double {
	return bounded::visit(m_state, bounded::overload(
		[=](Sleep const sleep) { return awaken_probability(sleep.turns_slept, ability); },
		[](Rest const sleep) { return sleep.turns_slept == sleep.turns_slept.max() ? 1.0 : 0.0; },
		[](Freeze) { return 0.2; },
		[](auto) { return 0.0; }
	));
}

bool blocks_status(Ability const ability, Statuses const status, Weather const weather) {
	switch (status) {
	case Statuses::burn:
		switch (ability) {
		case Ability::Leaf_Guard:
			return weather.sun();
		case Ability::Water_Veil:
			return true;
		default:
			return false;
		}
	case Statuses::freeze:
		return ability == Ability::Magma_Armor;
	case Statuses::paralysis:
		switch (ability) {
		case Ability::Leaf_Guard:
			return weather.sun();
		case Ability::Limber:
			return true;
		default:
			return false;
		}
	case Statuses::poison:
	case Statuses::toxic:
		switch (ability) {
		case Ability::Immunity:
			return true;
		case Ability::Leaf_Guard:
			return weather.sun();
		default:
			return false;
		}
	case Statuses::sleep:
	case Statuses::rest:
		switch (ability) {
			case Ability::Insomnia:
			case Ability::Vital_Spirit:
				return true;
			case Ability::Leaf_Guard:
				return weather.sun();
			default:
				return false;
		}
	case Statuses::clear:
		return false;
	}
}

}	// namespace technicalmachine
