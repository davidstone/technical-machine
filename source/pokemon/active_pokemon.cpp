// Flags for the active Pokemon
// Copyright (C) 2016 David Stone
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

#include "active_pokemon.hpp"

#include "../damage.hpp"
#include "../weather.hpp"

#include "../move/is_switch.hpp"
#include "../move/move.hpp"

namespace technicalmachine {

ActivePokemon::operator Species() const {
	return static_cast<Species>(static_cast<Pokemon const &>(*this));
}


auto MutableActivePokemon::clear_field() -> void {
	m_flags.leech_seeded = false;
	m_flags.partial_trap = {};
}

auto MutableActivePokemon::update_before_move() -> void {
	m_flags.destiny_bond = false;
	m_flags.locked_on = false;
	m_flags.moved = true;
}

auto MutableActivePokemon::activate_aqua_ring() -> void {
	m_flags.aqua_ring = true;
}

auto MutableActivePokemon::attract() -> void {
	m_flags.attracted = true;
}

auto MutableActivePokemon::baton_pass() -> void {
	m_flags.is_baton_passing = true;
}

auto MutableActivePokemon::charge() -> void {
	m_flags.charged = true;
}

auto MutableActivePokemon::confuse() -> void {
	if (!get_ability(*this).blocks_confusion()) {
		m_flags.confusion.activate();
	}
}

auto MutableActivePokemon::handle_confusion() -> void {
	m_flags.confusion.do_turn(*this);
}

auto MutableActivePokemon::curse() -> void {
	m_flags.is_cursed = true;
}

auto MutableActivePokemon::defense_curl() -> void {
	m_flags.defense_curled = true;
}

auto MutableActivePokemon::use_destiny_bond() -> void {
	m_flags.destiny_bond = true;
}

auto MutableActivePokemon::disable() -> void {
	if (is_regular(current_move(*this))) {
		m_flags.disable.activate(RegularMoveIndex(all_moves(*this).index(), bounded::non_check));
	}
}

auto MutableActivePokemon::advance_disable() -> void {
	m_flags.disable.advance_one_turn();
}

auto MutableActivePokemon::activate_embargo() -> void {
	m_flags.embargo.activate();
}

auto MutableActivePokemon::advance_embargo() -> void {
	m_flags.embargo.advance_one_turn();
}

auto MutableActivePokemon::activate_encore() -> void {
	m_flags.encore.activate();
}

auto MutableActivePokemon::advance_encore() -> void {
	m_flags.encore.advance_one_turn();
}

auto MutableActivePokemon::endure() -> void {
	m_flags.enduring = true;
}

auto MutableActivePokemon::faint() -> void {
	get_hp(*this) = 0_bi;
	m_flags.is_fainted = true;
}

auto MutableActivePokemon::activate_flash_fire() -> void {
	m_flags.flash_fire = true;
}

auto MutableActivePokemon::flinch() -> void {
	m_flags.flinched = true;
}

auto MutableActivePokemon::focus_energy() -> void {
	m_flags.has_focused_energy = true;
}

auto MutableActivePokemon::fully_trap() -> void {
	m_flags.fully_trapped = true;
}

auto MutableActivePokemon::identify() -> void {
	m_flags.identified = true;
}

auto MutableActivePokemon::use_imprison() -> void {
	m_flags.used_imprison = true;
}

auto MutableActivePokemon::ingrain() -> void {
	m_flags.ingrained = true;
}

auto MutableActivePokemon::activate_heal_block() -> void {
	m_flags.heal_block.activate();
}

auto MutableActivePokemon::advance_heal_block() -> void {
	m_flags.heal_block.advance_one_turn();
}

auto MutableActivePokemon::hit_with_leech_seed() -> void {
	m_flags.leech_seeded = true;
}

auto MutableActivePokemon::advance_lock_in() -> void {
	// Cannot be locked into Rampage and Uproar at the same time
	if (m_flags.rampage.is_active()) {
		m_flags.rampage.advance_one_turn();
		if (!m_flags.rampage.is_active()) {
			confuse();
		}
	} else {
		m_flags.uproar.advance_one_turn();
	}
}

auto MutableActivePokemon::use_lock_on() -> void {
	m_flags.locked_on = true;
}

auto lower_pp(MutableActivePokemon user, Ability const target) -> void {
	if (is_regular(current_move(user)) and !is_locked_in_to_bide(user)) {
		regular_move(all_moves(user)).decrement_pp(target);
	}
}

auto MutableActivePokemon::activate_magnet_rise() -> void {
	m_flags.magnet_rise.activate();
}

auto MutableActivePokemon::advance_magnet_rise() -> void {
	m_flags.magnet_rise.advance_one_turn();
}

auto MutableActivePokemon::set_moved(bool const value) -> void {
	m_flags.moved = value;
}

auto MutableActivePokemon::activate_mud_sport() -> void {
	m_flags.mud_sport = true;
}

auto MutableActivePokemon::give_nightmares() -> void {
	m_flags.is_having_a_nightmare = true;
}

auto MutableActivePokemon::partially_trap() -> void {
	m_flags.partial_trap.activate();
}

auto MutableActivePokemon::partial_trap_damage() -> void {
	m_flags.partial_trap.damage(*this);
}

auto MutableActivePokemon::activate_perish_song() -> void {
	m_flags.perish_song.activate();
}

auto MutableActivePokemon::perish_song_turn() -> void {
	bool const faints_this_turn = m_flags.perish_song.advance_one_turn_deactivated();
	if (faints_this_turn) {
		faint();
	}
}

auto MutableActivePokemon::activate_power_trick() -> void {
	m_flags.power_trick_is_active = !m_flags.power_trick_is_active;
}

auto MutableActivePokemon::protect() -> void {
	m_flags.is_protecting = true;
}

auto MutableActivePokemon::break_protect() -> void {
	m_flags.is_protecting = false;
}

auto MutableActivePokemon::activate_rampage() -> void {
	m_flags.rampage.activate();
}

auto MutableActivePokemon::recharge() -> bool {
	bool const return_value = is_recharging(*this);
	m_flags.is_recharging = false;
	return return_value;
}

auto MutableActivePokemon::use_recharge_move() -> void {
	m_flags.is_recharging = true;
}

auto MutableActivePokemon::roost() -> void {
	m_flags.is_roosting = true;
}

auto MutableActivePokemon::increase_sleep_counter(bool const awakens) -> void {
	auto & status = get_status(*this);
	auto const & ability = get_ability(*this);
	status.increase_sleep_counter(ability, awakens);
}

auto MutableActivePokemon::increment_stockpile() -> void {
	bool const increased = m_flags.stockpile.increment();
	if (increased) {
		boost_defensive(stage(*this), 1_bi);
	}
}

auto MutableActivePokemon::release_stockpile() -> bounded::integer<0, Stockpile::max> {
	auto const stages = m_flags.stockpile.release();
	boost_defensive(stage(*this), -stages);
	return stages;
}

auto has_switched(ActivePokemon const pokemon) -> bool {
	return moved(pokemon) and is_switch(current_move(pokemon));
}

auto MutableActivePokemon::torment() -> void {
	m_flags.is_tormented = true;
}

auto MutableActivePokemon::taunt() -> void {
	m_flags.taunt.activate();
}

auto MutableActivePokemon::advance_taunt() -> void {
	m_flags.taunt.advance_one_turn();
}

auto MutableActivePokemon::advance_toxic() -> void {
	m_flags.toxic.increment();
}

auto MutableActivePokemon::u_turn() -> void {
	m_flags.u_turning = true;
}

auto MutableActivePokemon::use_uproar() -> void {
	m_flags.uproar.advance_one_turn();
}

auto MutableActivePokemon::activate_water_sport() -> void {
	m_flags.water_sport = true;
}

auto MutableActivePokemon::hit_with_yawn() -> void {
	m_flags.yawn.activate();
}

auto MutableActivePokemon::try_to_activate_yawn(Weather const weather) -> void {
	bool const put_to_sleep = m_flags.yawn.advance_one_turn_deactivated();
	if (put_to_sleep) {
		apply(Statuses::sleep, *this, weather);
	}
}

auto MutableActivePokemon::bounce() -> bool {
	return m_flags.vanish.bounce();
}
auto MutableActivePokemon::dig() -> bool {
	return m_flags.vanish.dig();
}
auto MutableActivePokemon::dive() -> bool {
	return m_flags.vanish.dive();
}
auto MutableActivePokemon::fly() -> bool {
	return m_flags.vanish.fly();
}
auto MutableActivePokemon::shadow_force() -> bool {
	return m_flags.vanish.shadow_force();
}

auto MutableActivePokemon::use_bide(Pokemon & target) -> void {
	if (!m_flags.bide.is_active()) {
		m_flags.bide.activate();
	}
	else {
		auto const bide_damage = m_flags.bide.decrement();
		get_hp(target) -= bide_damage * 2_bi;
	}
}

namespace {

auto can_use_substitute(Pokemon const & pokemon) -> bool {
	auto const & hp = get_hp(pokemon);
	return hp.current() > hp.max() / 4_bi;
}

}	// namespace

auto MutableActivePokemon::use_substitute() -> void {
	if (!can_use_substitute(*this))
		return;
	auto const max_hp = get_hp(*this).max();
	indirect_damage(m_flags.substitute.create(max_hp));
}

auto MutableActivePokemon::direct_damage(damage_type const damage) -> void {
	if (m_flags.substitute) {
		m_flags.substitute.damage(damage);
	} else {
		auto & hp = get_hp(*this);
		auto const initial_hp = hp.current();
		hp -= damage;
		m_flags.damaged = initial_hp - hp.current();
		m_flags.bide.add_damage(damage);
	}
}

auto MutableActivePokemon::indirect_damage(damage_type const damage) -> void {
	get_hp(*this) -= damage;
}

auto MutableActivePokemon::register_damage(damage_type const damage) -> void {
	m_flags.damaged = damage;
}

auto MutableActivePokemon::increment_move_use_counter() -> void {
	if (is_regular(current_move(*this))) {
		auto const move_index = static_cast<LastUsedMove::index_type>(all_moves(*this).index());
		m_flags.last_used_move.increment(move_index);
	} else {
		m_flags.last_used_move = {};
	}
}

auto operator==(ActivePokemon const lhs, ActivePokemon const rhs) -> bool {
	return lhs.m_flags == rhs.m_flags;
}

}	// namespace technicalmachine
