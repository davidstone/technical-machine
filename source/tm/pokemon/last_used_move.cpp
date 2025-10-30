// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>
#include <bounded/conditional.hpp>

export module tm.pokemon.last_used_move;

import tm.pokemon.immobilize;
import tm.pokemon.rampage;
import tm.pokemon.uproar;

import tm.bide.bide;

import tm.move.is_delayed_switch;
import tm.move.move_name;
import tm.move.recharging;

import tm.stat.current_hp;

import tm.string_conversions.move_name;

import tm.compress;
import tm.generation;
import tm.item;
import tm.probability;
import tm.rational;
import tm.saturating_add;
import tm.weather;

import bounded;
import containers;
import numeric_traits;
import tv;
import std_module;

namespace technicalmachine {

using namespace bounded::literal;
using namespace std::string_view_literals;

template<typename T>
concept ends_at_action = requires(T const & value) {
	value.action_end_probability();
};

template<typename T>
concept ends_at_end_of_turn = requires(T const & value) {
	value.end_of_turn_end_probability();
};

struct Empty {
	friend auto operator==(Empty, Empty) -> bool = default;
};
struct ChargingUp {
	friend auto operator==(ChargingUp, ChargingUp) -> bool = default;
};
struct Protecting {
	friend auto operator==(Protecting, Protecting) -> bool = default;
};
struct Vanishing {
	friend auto operator==(Vanishing, Vanishing) -> bool = default;
};

export template<Generation generation>
struct LastUsedMove {
	constexpr auto name() const -> tv::optional<MoveName> {
		return m_move;
	}

	constexpr auto reset_end_of_turn() & -> void {
		break_protect();
		m_moved_this_turn = false;
	}

	// When the move is not known, like with flinch
	constexpr auto unsuccessful_move() & -> void {
		m_consecutive_successes = 0_bi;
		m_moved_this_turn = true;
		m_effects = Empty();
	}
	constexpr auto unsuccessful_move(MoveName const move) & -> void {
		m_move = move;
		m_consecutive_successes = 0_bi;
		m_moved_this_turn = true;
		m_effects = Empty();
	}

	// If the user selects Thunder, but is hit by Encore before it can move and
	// is forced to execute Metronome, which calls Assist, which calls Tackle,
	// Metronome is `first_executed` and Tackle is `last_executed`
	//
	// Returns whether this execution consumed an item
	constexpr auto successful_move(
		MoveName const first_executed,
		MoveName const last_executed,
		bool const end_effect,
		Item const item,
		Weather const weather
	) & -> bool {
		auto check_valid_lock_in = [&] {
			// TODO: Choice Band can be weird
			if (!successful_last_move(first_executed)) {
				throw std::runtime_error(containers::concatenate<std::string>(
					"Tried to use "sv,
					to_string(first_executed),
					" while locked into "sv,
					m_move ? to_string(*m_move) : "nothing"sv
				));
			}
		};
		auto const result = tv::visit(m_effects, tv::overload(
			[&](Empty) -> bool {
				switch (last_executed) {
					case MoveName::Bide:
						m_effects = Bide<generation>();
						return false;
					case MoveName::Bounce:
					case MoveName::Dig:
					case MoveName::Dive:
					case MoveName::Fly:
					case MoveName::Shadow_Force:
						if (item == Item::Power_Herb) {
							return true;
						}
						m_effects = Vanishing();
						return false;
					case MoveName::Meteor_Beam:
					case MoveName::Razor_Wind:
					case MoveName::Skull_Bash:
					case MoveName::Sky_Attack:
						if (item == Item::Power_Herb) {
							return true;
						}
						m_effects = ChargingUp();
						return false;
					case MoveName::Solar_Beam:
						if (weather == Weather::sun) {
							return false;
						}
						if (item == Item::Power_Herb) {
							return true;
						}
						m_effects = ChargingUp();
						return false;
					case MoveName::Bind:
					case MoveName::Clamp:
					case MoveName::Fire_Spin:
					case MoveName::Wrap:
						if constexpr (generation == Generation::one) {
							m_effects = Immobilize();
						}
						return false;
					case MoveName::Detect:
					case MoveName::Protect:
						m_effects = Protecting();
						return false;
					case MoveName::Blast_Burn:
					case MoveName::Eternabeam:
					case MoveName::Frenzy_Plant:
					case MoveName::Giga_Impact:
					case MoveName::Hydro_Cannon:
					case MoveName::Hyper_Beam:
					case MoveName::Meteor_Assault:
					case MoveName::Roar_of_Time:
					case MoveName::Rock_Wrecker:
						m_effects = Recharging();
						return false;
					case MoveName::Outrage:
					case MoveName::Petal_Dance:
					case MoveName::Thrash:
						m_effects = Rampage<generation>();
						return false;
					case MoveName::Uproar:
						m_effects = UproarCounter();
						return false;
					default:
						return false;
				}
			},
			[&](Bide<generation> & bide) -> bool {
				check_valid_lock_in();
				if (!end_effect) {
					bide.advance_one_turn();
					return false;
				}
				m_effects = Empty();
				return false;
			},
			[&](ChargingUp) -> bool {
				check_valid_lock_in();
				m_effects = Empty();
				return false;
			},
			[&](Immobilize) -> bool {
				check_valid_lock_in();
				return false;
			},
			[&](Protecting) -> bool {
				throw std::runtime_error("Cannot use a move while protecting");
			},
			[&](Rampage<generation>) -> bool {
				check_valid_lock_in();
				return false;
			},
			[&](Recharging) -> bool {
				check_valid_lock_in();
				return false;
			},
			[&](UproarCounter) -> bool {
				BOUNDED_ASSERT(m_move == MoveName::Uproar);
				check_valid_lock_in();
				return false;
			},
			[&](Vanishing) -> bool {
				check_valid_lock_in();
				m_effects = Empty();
				return false;
			}
		));

		// TODO: ???
		if (m_move == first_executed) {
			saturating_increment(m_consecutive_successes);
		} else {
			m_move = first_executed;
			m_consecutive_successes = 1_bi;
		}
		m_moved_this_turn = true;
		return result;
	}

	constexpr auto use_switch(bool const replacing_fainted_or_initial_switch) & {
		m_move = tv::none;
		if (!replacing_fainted_or_initial_switch) {
			m_moved_this_turn = true;
		}
		m_effects = Empty();
	}

	constexpr auto hit_self() & -> void {
		m_move = tv::none;
		m_moved_this_turn = true;
		m_effects = Empty();
	}

	constexpr auto moved_this_turn() const {
		return m_moved_this_turn;
	}

	constexpr auto is_baton_passing() const -> bool {
		return moved_this_turn() and successful_last_move(MoveName::Baton_Pass);
	}

	constexpr auto direct_damage(CurrentHP const damage) & -> void {
		tv::visit(m_effects, tv::overload(
			[=](Bide<generation> & bide) { bide.add_damage(damage); },
			[](auto) {}
		));
	}

	constexpr auto bide_damage() const -> CurrentHP {
		return tv::visit(m_effects, tv::overload(
			[](Bide<generation> const bide) -> CurrentHP {
				return bide.released_damage();
			},
			[](auto) -> CurrentHP {
				throw std::runtime_error("Tried to get Bide damage when Bide isn't active");
			}
		));
	}

	constexpr auto is_charging() const -> bool {
		return
			m_effects.index() == bounded::type<Bide<generation>> or
			m_effects.index() == bounded::type<ChargingUp>;
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

	constexpr auto fury_cutter_power() const {
		constexpr auto base = 
			bounded::conditional_function<generation <= Generation::four>(10_bi,
			bounded::conditional_function<generation <= Generation::five>(20_bi,
			40_bi
		));
		// base * 2 ^ n
		return bounded::min(base << m_consecutive_successes, 160_bi);
	}

	constexpr auto locked_in() const -> tv::optional<MoveName> {
		return m_effects.index() != bounded::type<Empty> ? m_move : tv::none;
	}
	constexpr auto action_end_probability() const -> Probability {
		return tv::visit(m_effects, tv::overload(
			[](ends_at_action auto const value) -> Probability {
				return value.action_end_probability();
			},
			[](auto) -> Probability {
				return Probability(0.0);
			}
		));
	}
	// Returns whether the user should get confused
	constexpr auto advance_lock_in(bool const ending) & -> bool {
		auto common = [&](auto & element) {
			if (ending) {
				m_effects = Empty();
				return true;
			} else {
				element.advance_one_turn();
				return false;
			}
		};
		return tv::visit(m_effects, tv::overload(
			[&](Immobilize & immobilize) {
				common(immobilize);
				return false;
			},
			[&](Rampage<generation> & rampage) {
				return common(rampage);
			},
			[&](UproarCounter & uproar) {
				common(uproar);
				return false;
			},
			[](auto const &) {
				return false;
			}
		));
	}
	constexpr auto end_of_turn_end_probability() const -> Probability {
		return tv::visit(m_effects, tv::overload(
			[](ends_at_end_of_turn auto const value) -> Probability {
				return value.end_of_turn_end_probability();
			},
			[](auto) -> Probability {
				return Probability(0.0);
			}
		));
	}

	constexpr auto locked_on() const {
		return
			successful_last_move(MoveName::Lock_On) or
			successful_last_move(MoveName::Mind_Reader);
	}

	// TODO: Does Metronome boost Struggle?
	constexpr auto metronome_boost() const {
		auto const boost = bounded::conditional_function<generation <= Generation::four>(
			m_consecutive_successes,
			2_bi * bounded::min(m_consecutive_successes, 5_bi)
		);
		return rational(10_bi + boost, 10_bi);
	}

	constexpr auto momentum_move_power() const -> bounded::integer<30, 480> {
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

	constexpr auto is_roosting() const {
		return moved_this_turn() and successful_last_move(MoveName::Roost);
	}

	constexpr auto switched_in_this_turn() const {
		return moved_this_turn() and !m_move;
	}

	constexpr auto triple_kick_power() const -> bounded::integer<10, 30> {
		auto const result = 10_bi * bounded::min(m_consecutive_successes + 1_bi, 3_bi);
		static_assert(numeric_traits::min_value<decltype(result)> == 10_bi);
		static_assert(numeric_traits::max_value<decltype(result)> == 30_bi);
		return result;
	}

	constexpr auto foe_switched() & -> void {
		tv::visit(m_effects, tv::overload(
			[&](Immobilize) { m_effects = Empty(); },
			[](auto) { }
		));
	}

	constexpr auto is_immobilizing() const -> bool {
		return m_effects.index() == bounded::type<Immobilize>;
	}

	constexpr auto end_immobilize() & -> void {
		if (is_immobilizing()) {
			m_effects = Empty();
		}
	}

	constexpr auto is_uproaring() const -> bool {
		return m_effects.index() == bounded::type<UproarCounter>;
	}

	constexpr auto vanish_doubles_power(MoveName const move_name) const -> bool {
		if constexpr (generation == Generation::one) {
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
	constexpr auto successful_last_move(MoveName const move) const -> bool {
		return m_move == move and m_consecutive_successes >= 1_bi;
	}

	tv::optional<MoveName> m_move = tv::none;
	bounded::integer<0, 10> m_consecutive_successes = 0_bi;
	bool m_moved_this_turn = false;

	tv::variant<
		Empty,
		Bide<generation>,
		ChargingUp,
		Immobilize,
		Protecting,
		Rampage<generation>,
		Recharging,
		UproarCounter,
		Vanishing
	> m_effects{Empty()};
};

} // namespace technicalmachine