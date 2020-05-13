// Copyright (C) 2017 David Stone
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

#include <tm/pokemon/last_used_move.hpp>

#include <bounded/detail/overload.hpp>

namespace technicalmachine {

auto LastUsedMove::reset_start_of_turn() & -> void {
	m_moved_this_turn = false;
	bounded::visit(m_effects, [&]<typename T>(T) {
		if constexpr (std::is_same_v<T, Protecting>) {
			m_effects = Empty();
		}
	});
}

auto LastUsedMove::use_bide() & -> bounded::optional<HP::current_type> {
	return bounded::visit(m_effects, bounded::overload(
		[&](Bide & bide) {
			auto const result = bide.advance_one_turn();
			m_effects = Empty();
			return bounded::optional<HP::current_type>(result);
		},
		[&](auto) {
			m_effects = Bide{};
			return bounded::optional<HP::current_type>();
		}
	));
}

auto LastUsedMove::direct_damage(HP::current_type const damage) & -> void {
	bounded::visit(m_effects, bounded::overload(
		[=](Bide & bide) { bide.add_damage(damage); },
		[](auto) {}
	));
}

auto LastUsedMove::is_charging_up() const -> bool {
	return bounded::holds_alternative(m_effects, bounded::detail::types<ChargingUp>());
}

auto LastUsedMove::use_charge_up_move() & -> void {
	BOUNDED_ASSERT(!is_locked_in_by_move());
	m_effects = ChargingUp{};
}

auto LastUsedMove::is_locked_in_by_move() const -> bool {
	return !bounded::holds_alternative(m_effects, bounded::detail::types<Empty>());
}

auto LastUsedMove::advance_lock_in(bool const ending) & -> bool {
	return bounded::visit(m_effects, bounded::overload(
		[&](Rampage & rampage) {
			if (ending) {
				m_effects = Empty();
				return true;
			} else {
				rampage.advance_one_turn();
				return false;
			}
		},
		[&](UproarCounter & uproar) {
			if (ending) {
				m_effects = Empty();
			} else {
				uproar.advance_one_turn();
			}
			return false;
		},
		[](auto const &) {
			return false;
		}
	));
}

auto LastUsedMove::is_protecting() const -> bool {
	return bounded::holds_alternative(m_effects, bounded::detail::types<Protecting>());
}

auto LastUsedMove::protect() & -> void {
	BOUNDED_ASSERT(!is_locked_in_by_move());
	m_effects = Protecting();
}

auto LastUsedMove::break_protect() & -> void {
	if (is_protecting()) {
		m_effects = Empty();
	}
}

auto LastUsedMove::activate_rampage() & -> void {
	BOUNDED_ASSERT(!is_locked_in_by_move());
	// TODO: Have it be active when it is constructed
	auto rampage = Rampage();
	rampage.activate();
	m_effects = rampage;
}

auto LastUsedMove::recharge() & -> bool {
	return bounded::visit(m_effects, bounded::overload(
		[&](Recharging) { m_effects = Empty(); return true; },
		[](auto const &) { return false; }
	));
}

auto LastUsedMove::use_recharge_move() & -> void {
	BOUNDED_ASSERT(!is_locked_in_by_move());
	m_effects = Recharging();
}

auto LastUsedMove::is_uproaring() const -> bool {
	return bounded::holds_alternative(m_effects, bounded::detail::types<UproarCounter>());
}

auto LastUsedMove::use_uproar() & -> void {
	bounded::visit(m_effects, bounded::overload(
		[&](Empty) {
			// TODO: Have it be active when it is constructed
			auto counter = UproarCounter();
			counter.advance_one_turn();
			m_effects = counter;
		},
		[](UproarCounter & uproar) { uproar.advance_one_turn(); },
		[](auto) { bounded::assert_or_assume_unreachable(); }
	));
}

auto LastUsedMove::vanish_doubles_power(Generation const generation, Moves const move_name) const -> bool {
	if (generation <= Generation::one) {
		return false;
	}
	if (!bounded::holds_alternative(m_effects, bounded::detail::types<Vanishing>())) {
		return false;
	}
	switch (move_name) {
		case Moves::Earthquake:
		case Moves::Magnitude:
			return m_move == Moves::Dig;
		case Moves::Gust:
		case Moves::Twister:
			return m_move == Moves::Bounce or m_move == Moves::Fly;
		case Moves::Surf:
			return m_move == Moves::Dive;
		default:
			return false;
	}
}

auto LastUsedMove::use_vanish_move(Item const item) & -> VanishOutcome {
	return bounded::visit(m_effects, bounded::overload(
		[&](Vanishing) {
			m_effects = Empty();
			return VanishOutcome::vanishes;
		},
		[&](auto) {
			if (item == Item::Power_Herb) {
				return VanishOutcome::consumes_item;
			}
			m_effects = Vanishing();
			return VanishOutcome::attacks;
		}
	));
}

} // namespace technicalmachine