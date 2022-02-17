// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/rampage.hpp>
#include <tm/pokemon/uproar.hpp>

#include <tm/bide/bide.hpp>

#include <tm/move/move.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/is_switch.hpp>

#include <tm/compress.hpp>
#include <tm/generation.hpp>
#include <tm/rational.hpp>
#include <tm/saturating_add.hpp>
#include <tm/weather.hpp>

#include <bounded/integer.hpp>
#include <bounded/detail/variant/variant.hpp>

#include <numeric_traits/min_max_value.hpp>

namespace technicalmachine {

using namespace bounded::literal;

enum class VanishOutcome { vanishes, attacks, consumes_item };

struct LastUsedMove {
	constexpr auto name() const {
		return m_move;
	}

	auto reset_start_of_turn() & -> void;

	constexpr auto unsuccessful_move(Moves const move) & {
		m_move = move;
		m_consecutive_successes = 0_bi;
		m_moved_this_turn = true;
	}

	constexpr auto successful_move(Moves const move) & {
		if (m_move == move) {
			saturating_increment(m_consecutive_successes);
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

	constexpr auto fury_cutter_power(Generation const generation) const {
		auto const base =
			BOUNDED_CONDITIONAL(generation <= Generation::four, 10_bi,
			BOUNDED_CONDITIONAL(generation <= Generation::five, 20_bi,
			40_bi
		));
		// base * 2 ^ n
		return bounded::min(base << m_consecutive_successes, 160_bi);
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
		static_assert(numeric_traits::min_value<decltype(result)> == 30_bi);
		static_assert(numeric_traits::max_value<decltype(result)> == 480_bi);
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
		return is_baton_passing() or is_delayed_switching();
	}

	constexpr auto triple_kick_power() const {
		auto const result = 10_bi * bounded::min(m_consecutive_successes + 1_bi, 3_bi);
		static_assert(numeric_traits::min_value<decltype(result)> == 10_bi);
		static_assert(numeric_traits::max_value<decltype(result)> == 30_bi);
		return result;
	}

	auto is_uproaring() const -> bool;
	auto use_uproar() & -> void;
	auto vanish_doubles_power(Generation const generation, Moves const move_name) const -> bool;

	auto use_vanish_move(Item) & -> VanishOutcome;

	friend auto operator==(LastUsedMove, LastUsedMove) -> bool = default;
	friend constexpr auto compress(LastUsedMove const value) {
		return compress_combine(
			value.m_move,
			value.m_consecutive_successes,
			value.m_moved_this_turn,
			value.m_effects
		);
	}

private:
	constexpr auto successful_last_move(Moves const move) const -> bool {
		return m_move == move and m_consecutive_successes >= 1_bi;
	}
	constexpr auto is_delayed_switching() const -> bool {
		return moved_this_turn() and is_delayed_switch(m_move) and m_consecutive_successes >= 1_bi;
	}

	Moves m_move = Moves::Switch0;
	bounded::integer<0, 10> m_consecutive_successes = 0_bi;
	bool m_moved_this_turn = false;

	struct Empty {
		friend auto operator==(Empty, Empty) -> bool = default;
		friend constexpr auto compress(Empty) -> bounded::constant_t<0> {
			return 0_bi;
		}
	};
	struct ChargingUp {
		friend auto operator==(ChargingUp, ChargingUp) -> bool = default;
		friend constexpr auto compress(ChargingUp) -> bounded::constant_t<0> {
			return 0_bi;
		}
	};
	struct Protecting {
		friend auto operator==(Protecting, Protecting) -> bool = default;
		friend constexpr auto compress(Protecting) -> bounded::constant_t<0> {
			return 0_bi;
		}
	};
	struct Recharging {
		friend auto operator==(Recharging, Recharging) -> bool = default;
		friend constexpr auto compress(Recharging) -> bounded::constant_t<0> {
			return 0_bi;
		}
	};
	struct Vanishing {
		friend auto operator==(Vanishing, Vanishing) -> bool = default;
		friend constexpr auto compress(Vanishing) -> bounded::constant_t<0> {
			return 0_bi;
		}
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

} // namespace technicalmachine
