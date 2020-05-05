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

#include <tm/pokemon/rampage.hpp>
#include <tm/pokemon/uproar.hpp>

#include <tm/bide/bide.hpp>

#include <tm/move/moves.hpp>
#include <tm/move/is_switch.hpp>

#include <tm/generation.hpp>
#include <tm/rational.hpp>
#include <tm/weather.hpp>

#include <bounded/integer.hpp>
#include <bounded/detail/variant/variant.hpp>

namespace technicalmachine {

using namespace bounded::literal;

enum class VanishOutcome { vanishes, attacks, consumes_item };

struct LastUsedMove {
	constexpr auto name() const {
		return m_move;
	}
	
	auto reset_start_of_turn() & -> void;

	constexpr auto unsucessful_move(Moves const move) & {
		m_move = move;
		m_consecutive_successes = 0_bi;
		m_moved_this_turn = true;
	}

	constexpr auto successful_move(Moves const move) & {
		if (m_move == move) {
			++m_consecutive_successes;
		} else {
			m_move = move;
			m_consecutive_successes = 1_bi;
		}
		m_moved_this_turn = true;
	}

	constexpr auto moved_this_turn() const {
		return m_moved_this_turn;
	}

	constexpr auto is_baton_passing() const -> bool {
		return moved_this_turn() and successful_last_move(Moves::Baton_Pass);
	}

	auto use_bide() & -> bounded::optional<HP::current_type>;
	auto direct_damage(HP::current_type damage) & -> void;

	auto is_charging_up() const -> bool;
	auto use_charge_up_move() & -> void;

	constexpr auto is_destiny_bonded() const {
		return successful_last_move(Moves::Destiny_Bond);
	}

	constexpr auto is_enduring() const -> bool {
		return moved_this_turn() and successful_last_move(Moves::Endure);
	}

	constexpr auto fury_cutter_power() const {
		// 10 * 2 ^ n
		auto const result = 10_bi << bounded::min(m_consecutive_successes, 4_bi);
		static_assert(bounded::min_value<decltype(result)> == 10_bi);
		static_assert(bounded::max_value<decltype(result)> == 160_bi);
		return result;
	}

	auto is_locked_in_by_move() const -> bool;
	// Returns whether the use should get confused
	auto advance_lock_in(bool const ending) & -> bool;

	constexpr auto locked_on() const {
		return successful_last_move(Moves::Lock_On) or successful_last_move(Moves::Mind_Reader);
	}

	// TODO: Does Metronome boost Struggle?
	constexpr auto metronome_boost(Generation const generation) const {
		auto const boost = BOUNDED_CONDITIONAL(
			generation <= Generation::four,
			m_consecutive_successes,
			2_bi * bounded::min(m_consecutive_successes, 5_bi)
		);
		return rational(10_bi + boost, 10_bi);
	}

	constexpr auto momentum_move_power() const {
		auto const result = 30_bi << bounded::min(m_consecutive_successes, 4_bi);
		static_assert(bounded::min_value<decltype(result)> == 30_bi);
		static_assert(bounded::max_value<decltype(result)> == 480_bi);
		return result;
	}

	auto is_protecting() const -> bool;
	auto protect() & -> void;
	auto break_protect() & -> void;
	auto activate_rampage() & -> void;
	auto recharge() & -> bool;
	auto use_recharge_move() & -> void;

	constexpr auto is_roosting() const {
		return moved_this_turn() and successful_last_move(Moves::Roost);
	}

	constexpr auto switched_in_this_turn() const {
		return moved_this_turn() and m_consecutive_successes != 0_bi and is_switch(m_move);
	}
	constexpr auto switch_decision_required() const -> bool {
		return is_baton_passing() or is_u_turning();
	}

	constexpr auto triple_kick_power() const {
		auto const result = 10_bi * bounded::min(m_consecutive_successes + 1_bi, 3_bi);
		static_assert(bounded::min_value<decltype(result)> == 10_bi);
		static_assert(bounded::max_value<decltype(result)> == 30_bi);
		return result;
	}

	auto is_uproaring() const -> bool;
	auto use_uproar() & -> void;
	auto vanish_doubles_power(Generation const generation, Moves const move_name) const -> bool;

	auto use_vanish_move(Item) & -> VanishOutcome;

	friend auto operator==(LastUsedMove const &, LastUsedMove const &) -> bool = default;
private:
	constexpr auto successful_last_move(Moves const move) const -> bool {
		return m_move == move and m_consecutive_successes >= 1_bi;
	}
	constexpr auto is_u_turning() const -> bool {
		return moved_this_turn() and successful_last_move(Moves::U_turn);
	}

	Moves m_move = Moves::Switch0;
	bounded::clamped_integer<0, 10> m_consecutive_successes = 0_bi;
	bool m_moved_this_turn = false;

	struct Empty {
		friend auto operator==(Empty const &, Empty const &) -> bool = default;
	};
	struct ChargingUp {
		friend auto operator==(ChargingUp const &, ChargingUp const &) -> bool = default;
	};
	struct Protecting {
		friend auto operator==(Protecting const &, Protecting const &) -> bool = default;
	};
	struct Recharging {
		friend auto operator==(Recharging const &, Recharging const &) -> bool = default;
	};
	struct Vanishing {
		friend auto operator==(Vanishing const &, Vanishing const &) -> bool = default;
	};

	bounded::variant<
		Empty,
		Bide,
		ChargingUp,
		Protecting,
		Rampage,
		Recharging,
		UproarCounter,
		Vanishing
	> m_effects{Empty()};
};

}	// namespace technicalmachine
