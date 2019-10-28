// Flags for the active Pokemon
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

#include <tm/pokemon/active_pokemon.hpp>

#include <tm/weather.hpp>

#include <tm/move/move.hpp>

#include <bounded/assert.hpp>
#include <bounded/detail/overload.hpp>
#include <bounded/unreachable.hpp>

namespace technicalmachine {

template struct ActivePokemonImpl<true>;
template struct ActivePokemonImpl<false>;

auto ActivePokemonFlags::reset_end_of_turn() -> void {
	damage_blocker = {};
	damaged = false;
	direct_damage_received = 0_bi;
	flinched = false;
	moved = false;
	me_first_is_active = false;
	is_loafing_turn = !is_loafing_turn;
	is_fully_paralyzed = false;
}

auto ActivePokemonFlags::reset_switch() -> void {
	// TODO: remove some of these when the foe switches, too
	if (!bounded::holds_alternative(lock_in, bounded::detail::types<BatonPassing>{})) {
		aqua_ring = false;
		confusion = {};
		is_cursed = false;
		embargo = {};
		has_focused_energy = false;
		gastro_acid = false;
		ingrained = false;
		leech_seeded = false;
		locked_on = false;
		magnet_rise = {};
		perish_song = {};
		power_trick_is_active = false;
		stage = {};
		substitute = {};
	}
	attracted = false;
	charged = false;
	damage_blocker = {};
	defense_curled = false;
	destiny_bond = false;
	disable = Disable{};
	encore = {};
	flash_fire = false;
	flinched = false;
	fully_trapped = false;
	heal_block = {};
	identified = false;
	used_imprison = false;
	last_used_move = {};
	// Do I set to true or false? true makes it wrong when a fainted Pokemon is
	// replaced; false makes it wrong otherwise
	is_loafing_turn = false;
	minimized = false;
	me_first_is_active = false;
	mud_sport = false;
	is_having_a_nightmare = false;
	partial_trap = {};
	is_roosting = false;
	slow_start = {};
	stockpile = {};
	is_tormented = false;
	water_sport = false;
	taunt = {};
	yawn = {};
	lock_in = std::monostate{};
}

auto ActivePokemonFlags::is_baton_passing() const -> bool {
	return bounded::holds_alternative(lock_in, bounded::detail::types<BatonPassing>{});
}

auto ActivePokemonFlags::is_charging_up() const -> bool {
	return bounded::holds_alternative(lock_in, bounded::detail::types<ChargingUp>{});
}

auto ActivePokemonFlags::is_locked_in_by_move() const -> bool {
	return !bounded::holds_alternative(lock_in, bounded::detail::types<std::monostate>{});
}

auto ActivePokemonFlags::switch_decision_required(Pokemon const & pokemon) const -> bool {
	return bounded::visit(lock_in, bounded::overload(
		[](BatonPassing) { return true; },
		[](UTurning) { return true; },
		[&](auto const &) { return get_hp(pokemon) == 0_bi; }
	));
}

auto ActivePokemonFlags::is_uproaring() const -> bool {
	return bounded::holds_alternative(lock_in, bounded::detail::types<UproarCounter>{});
}

auto ActivePokemonFlags::vanish_doubles_power(Moves const move_name) const -> bool {
	switch (move_name) {
	case Moves::Earthquake:
	case Moves::Magnitude:
		return bounded::holds_alternative(lock_in, bounded::detail::types<Digging>{});
	case Moves::Gust:
	case Moves::Twister:
		return bounded::holds_alternative(lock_in, bounded::detail::types<Bouncing>{}) or bounded::holds_alternative(lock_in, bounded::detail::types<Flying>{});
	case Moves::Surf:
		return bounded::holds_alternative(lock_in, bounded::detail::types<Diving>{});
	default:
		return false;
	}
}



auto MutableActivePokemon::baton_pass() const -> void {
	m_flags.lock_in = BatonPassing{};
}

auto MutableActivePokemon::use_charge_up_move() const -> void {
	m_flags.lock_in = ChargingUp{};
}

auto MutableActivePokemon::advance_lock_in(bool const ending) const -> void {
	bounded::visit(m_flags.lock_in, bounded::overload(
		[&](Rampage & rampage) {
			if (ending) {
				m_flags.lock_in = std::monostate{};
				confuse();
			} else {
				rampage.advance_one_turn();
			}
		},
		[&](UproarCounter & uproar) {
			if (ending) {
				m_flags.lock_in = std::monostate{};
			} else {
				uproar.advance_one_turn();
			}
		},
		[](auto const &) {}
	));
}

auto MutableActivePokemon::activate_rampage() const -> void {
	// TODO: Have it be active when it is constructed
	auto rampage = Rampage{};
	rampage.activate();
	m_flags.lock_in = rampage;
}
auto MutableActivePokemon::recharge() const -> bool {
	return bounded::visit(m_flags.lock_in, bounded::overload(
		[&](Recharging) { m_flags.lock_in = std::monostate{}; return true; },
		[](auto const &) { return false; }
	));
}
auto MutableActivePokemon::use_recharge_move() const -> void {
	m_flags.lock_in = Recharging{};
}

namespace {

auto can_use_substitute(Pokemon const & pokemon) -> bool {
	auto const & hp = get_hp(pokemon);
	return hp.current() > hp.max() / 4_bi;
}

}	// namespace

auto MutableActivePokemon::use_substitute() const -> void {
	if (!can_use_substitute(*this))
		return;
	auto const max_hp = get_hp(*this).max();
	indirect_damage(m_flags.substitute.create(max_hp));
}

auto MutableActivePokemon::u_turn() const -> void {
	m_flags.lock_in = UTurning{};
}
auto MutableActivePokemon::use_uproar() const -> void {
	bounded::visit(m_flags.lock_in, bounded::overload(
		// TODO: Have it be active when it is constructed
		[&](std::monostate) { m_flags.lock_in.emplace(bounded::detail::types<UproarCounter>{}).advance_one_turn(); },
		[](UproarCounter & uproar) { uproar.advance_one_turn(); },
		[](auto const &) { bounded::assert_or_assume_unreachable(); }
	));
}

namespace {

template<typename T>
auto use_vanish_move(auto & lock_in) -> bool {
	return bounded::visit(lock_in, bounded::overload(
		[&](T) { lock_in = std::monostate{}; return false; },
		[&](auto const &) { lock_in = T{}; return true; }
	));
}

} // namespace

auto MutableActivePokemon::bounce() const -> bool {
	return use_vanish_move<Bouncing>(m_flags.lock_in);
}
auto MutableActivePokemon::dig() const -> bool {
	return use_vanish_move<Digging>(m_flags.lock_in);
}
auto MutableActivePokemon::dive() const -> bool {
	return use_vanish_move<Diving>(m_flags.lock_in);
}
auto MutableActivePokemon::fly() const -> bool {
	return use_vanish_move<Flying>(m_flags.lock_in);
}
auto MutableActivePokemon::shadow_force() const -> bool {
	return use_vanish_move<ShadowForcing>(m_flags.lock_in);
}

auto MutableActivePokemon::use_bide(Pokemon & target) const -> void {
	bounded::visit(m_flags.lock_in, bounded::overload(
		[&](auto const &) { m_flags.lock_in = Bide{}; },
		[&](Bide & bide) {
			if (auto const damage = bide.advance_one_turn()) {
				get_hp(target) -= *damage * 2_bi;
				m_flags.lock_in = std::monostate{};
			}
		}
	));
}

auto MutableActivePokemon::direct_damage(HP::current_type const damage) const -> void {
	if (m_flags.substitute) {
		m_flags.substitute.damage(damage);
	} else {
		get_hp(*this) -= damage;
		m_flags.damaged = true;
		m_flags.direct_damage_received = damage;
		bounded::visit(m_flags.lock_in, bounded::overload(
			[=](Bide & bide) { bide.add_damage(damage); },
			[](auto const &) {}
		));
	}
}

}	// namespace technicalmachine
