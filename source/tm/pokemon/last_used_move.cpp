// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>
#include <bounded/conditional.hpp>

export module tm.pokemon.last_used_move;

import tm.pokemon.rampage;
import tm.pokemon.uproar;

import tm.bide.bide;

import tm.move.is_delayed_switch;
import tm.move.move_name;

import tm.stat.current_hp;

import tm.compress;
import tm.generation;
import tm.item;
import tm.rational;
import tm.saturating_add;

import bounded;
import numeric_traits;
import tv;
import std_module;

namespace technicalmachine {

using namespace bounded::literal;

export enum class VanishOutcome { vanishes, attacks, consumes_item };

export struct LastUsedMove {
	constexpr auto name() const -> tv::optional<MoveName> {
		return m_move;
	}

	constexpr auto reset_end_of_turn() & -> void {
		m_moved_this_turn = false;
		tv::visit(m_effects, [&]<typename T>(T) {
			if constexpr (std::same_as<T, Protecting>) {
				m_effects = Empty();
			}
		});
	}

	constexpr auto unsuccessful_move(MoveName const move) & -> void {
		m_move = move;
		m_consecutive_successes = 0_bi;
		m_moved_this_turn = true;
	}

	constexpr auto successful_move(MoveName const move) & -> void {
		if (m_move == move) {
			saturating_increment(m_consecutive_successes);
		} else {
			m_move = move;
			m_consecutive_successes = 1_bi;
		}
		m_moved_this_turn = true;
		switch (move) {
			case MoveName::Detect:
			case MoveName::Protect:
				tv::visit(m_effects, tv::overload(
					[&](Empty) {
						m_effects = Protecting();
					},
					[](auto) {
						throw std::runtime_error("Tried to use Protect while locked into another move");
					}
				));
				break;
			case MoveName::Outrage:
			case MoveName::Petal_Dance:
			case MoveName::Thrash:
				tv::visit(m_effects, tv::overload(
					[&](Empty) {
						// TODO: Have it be active when it is constructed
						auto rampage = Rampage();
						rampage.activate();
						m_effects = rampage;
					},
					[](Rampage) {
					},
					[](auto) {
						// TODO: Assert instead?
						throw std::runtime_error("Tried to use a rampage move while locked into another move");
					}
				));
				break;
			case MoveName::Uproar:
				tv::visit(m_effects, tv::overload(
					[&](Empty) {
						// TODO: Have it be active when it is constructed
						auto counter = UproarCounter();
						counter.advance_one_turn();
						m_effects = counter;
					},
					[](UproarCounter & uproar) { uproar.advance_one_turn(); },
					[](auto) { std::unreachable(); }
				));
				break;
			default:
				break;
		}
	}

	// Not for the initial switch in or replacing fainted
	constexpr auto use_switch() & {
		m_move = tv::none;
		m_moved_this_turn = true;
	}

	constexpr auto hit_self() & -> void {
		m_move = tv::none;
		m_moved_this_turn = true;
	}

	constexpr auto faint() & -> void {
		m_move = tv::none;
		m_moved_this_turn = true;
	}

	constexpr auto moved_this_turn() const {
		return m_moved_this_turn;
	}

	constexpr auto is_baton_passing() const -> bool {
		return moved_this_turn() and successful_last_move(MoveName::Baton_Pass);
	}

	constexpr auto use_bide() & -> tv::optional<CurrentHP> {
		return tv::visit(m_effects, tv::overload(
			[&](Bide & bide) {
				auto const result = bide.advance_one_turn();
				m_effects = Empty();
				return tv::optional<CurrentHP>(result);
			},
			[&](auto) {
				m_effects = Bide{};
				return tv::optional<CurrentHP>();
			}
		));
	}

	constexpr auto direct_damage(CurrentHP const damage) & -> void {
		tv::visit(m_effects, tv::overload(
			[=](Bide & bide) { bide.add_damage(damage); },
			[](auto) {}
		));
	}

	constexpr auto is_charging_up() const -> bool {
		return m_effects.index() == bounded::type<ChargingUp>;
	}
	constexpr auto use_charge_up_move() & -> void {
		BOUNDED_ASSERT(!locked_in_by_move());
		m_effects = ChargingUp{};
	}

	constexpr auto is_delayed_switching() const -> bool {
		return
			moved_this_turn() and
			m_move and
			is_delayed_switch(*m_move) and
			m_consecutive_successes >= 1_bi;
	}

	constexpr auto is_destiny_bonded() const {
		return successful_last_move(MoveName::Destiny_Bond);
	}

	constexpr auto is_enduring() const -> bool {
		return moved_this_turn() and successful_last_move(MoveName::Endure);
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

	constexpr auto locked_in_by_move() const -> tv::optional<MoveName> {
		return m_effects.index() != bounded::type<Empty> ? m_move : tv::none;
	}
	// Returns whether the use should get confused
	constexpr auto advance_lock_in(bool const ending) & -> bool {
		return tv::visit(m_effects, tv::overload(
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


	constexpr auto locked_on() const {
		return
			successful_last_move(MoveName::Lock_On) or
			successful_last_move(MoveName::Mind_Reader);
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

	constexpr auto is_protecting() const -> bool {
		return m_effects.index() == bounded::type<Protecting>;
	}
	constexpr auto break_protect() & -> void {
		if (is_protecting()) {
			m_effects = Empty();
		}
	}

	constexpr auto recharge() & -> bool {
		return tv::visit(m_effects, tv::overload(
			[&](Recharging) { m_effects = Empty(); return true; },
			[](auto const &) { return false; }
		));
	}
	constexpr auto use_recharge_move() & -> void {
		if (locked_in_by_move()) {
			throw std::runtime_error("Used Recharge move while locked in by move");
		}
		m_effects = Recharging();
	}

	constexpr auto is_roosting() const {
		return moved_this_turn() and successful_last_move(MoveName::Roost);
	}

	constexpr auto switched_in_this_turn() const {
		return moved_this_turn() and !m_move;
	}

	constexpr auto triple_kick_power() const {
		auto const result = 10_bi * bounded::min(m_consecutive_successes + 1_bi, 3_bi);
		static_assert(numeric_traits::min_value<decltype(result)> == 10_bi);
		static_assert(numeric_traits::max_value<decltype(result)> == 30_bi);
		return result;
	}

	constexpr auto is_uproaring() const -> bool {
		return m_effects.index() == bounded::type<UproarCounter>;
	}

	constexpr auto vanish_doubles_power(Generation const generation, MoveName const move_name) const -> bool {
		if (generation <= Generation::one) {
			return false;
		}
		if (m_effects.index() != bounded::type<Vanishing>) {
			return false;
		}
		BOUNDED_ASSERT(m_move);
		switch (move_name) {
			case MoveName::Earthquake:
			case MoveName::Magnitude:
				return *m_move == MoveName::Dig;
			case MoveName::Gust:
			case MoveName::Twister:
				return *m_move == MoveName::Bounce or *m_move == MoveName::Fly;
			case MoveName::Surf:
				return *m_move == MoveName::Dive;
			default:
				return false;
		}
	}
	constexpr auto use_vanish_move(Item const item) & -> VanishOutcome {
		return tv::visit(m_effects, tv::overload(
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

	constexpr auto successful_last_move(MoveName const move) const -> bool {
		return m_move == move and m_consecutive_successes >= 1_bi;
	}

	tv::optional<MoveName> m_move = tv::none;
	bounded::integer<0, 10> m_consecutive_successes = 0_bi;
	bool m_moved_this_turn = false;

	tv::variant<
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