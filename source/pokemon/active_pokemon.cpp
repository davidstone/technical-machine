// Flags for the active Pokemon
// Copyright (C) 2014 David Stone
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


auto ActivePokemon::reset_end_of_turn() -> void {
	m_flags.reset_end_of_turn();
}

auto ActivePokemon::reset_switch() -> void {
	m_flags.reset_switch();
}

auto ActivePokemon::reset_between_turns() -> void {
	m_flags.reset_between_turns();
}

auto ActivePokemon::clear_field() -> void {
	m_flags.leech_seeded = false;
	m_flags.partial_trap = {};
}

auto ActivePokemon::update_before_move() -> void {
	m_flags.destiny_bond = false;
	m_flags.locked_on = false;
	m_flags.moved = true;
}

auto ActivePokemon::activate_aqua_ring() -> void {
	m_flags.aqua_ring = true;
}

auto ActivePokemon::attract() -> void {
	m_flags.attracted = true;
}

auto ActivePokemon::awaken(bool const value) -> void {
	m_flags.awakening = value;
}

auto ActivePokemon::baton_pass() -> void {
	m_flags.is_baton_passing = true;
}

auto ActivePokemon::charge() -> void {
	m_flags.charged = true;
}

auto ActivePokemon::confuse() -> void {
	if (!get_ability(*this).blocks_confusion()) {
		m_flags.confusion.activate();
	}
}

auto ActivePokemon::handle_confusion() -> void {
	m_flags.confusion.do_turn(*this);
}

auto ActivePokemon::set_critical_hit(bool const value) -> void {
	m_flags.critical_hit = value;
}

auto ActivePokemon::curse() -> void {
	m_flags.is_cursed = true;
}

auto ActivePokemon::defense_curl() -> void {
	m_flags.defense_curled = true;
}

auto ActivePokemon::use_destiny_bond() -> void {
	m_flags.destiny_bond = true;
}

auto ActivePokemon::disable() -> void {
	if (is_regular(current_move(*this))) {
		m_flags.disable.activate(RegularMoveIndex(all_moves(*this).index(), bounded::non_check));
	}
}

auto ActivePokemon::advance_disable() -> void {
	m_flags.disable.advance_one_turn();
}

auto ActivePokemon::activate_embargo() -> void {
	m_flags.embargo.activate();
}

auto ActivePokemon::advance_embargo() -> void {
	m_flags.embargo.advance_one_turn();
}

auto ActivePokemon::activate_encore() -> void {
	m_flags.encore.activate();
}

auto ActivePokemon::advance_encore() -> void {
	m_flags.encore.advance_one_turn();
}

auto ActivePokemon::endure() -> void {
	m_flags.enduring = true;
}

auto ActivePokemon::faint() -> void {
	get_hp(*this) = 0_bi;
	m_flags.will_be_replaced = true;
}

auto ActivePokemon::activate_flash_fire() -> void {
	m_flags.flash_fire = true;
}

auto ActivePokemon::flinch() -> void {
	m_flags.flinched = true;
}

auto ActivePokemon::focus_energy() -> void {
	m_flags.has_focused_energy = true;
}

auto ActivePokemon::fully_trap() -> void {
	m_flags.fully_trapped = true;
}

auto ActivePokemon::identify() -> void {
	m_flags.identified = true;
}

auto ActivePokemon::use_imprison() -> void {
	m_flags.used_imprison = true;
}

auto ActivePokemon::ingrain() -> void {
	m_flags.ingrained = true;
}

auto ActivePokemon::activate_heal_block() -> void {
	m_flags.heal_block.activate();
}

auto ActivePokemon::advance_heal_block() -> void {
	m_flags.heal_block.advance_one_turn();
}

auto ActivePokemon::hit_with_leech_seed() -> void {
	m_flags.leech_seeded = true;
}

auto ActivePokemon::advance_lock_in() -> void {
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

auto ActivePokemon::use_lock_on() -> void {
	m_flags.locked_on = true;
}

auto lower_pp(ActivePokemon & user, Ability const target) -> void {
	if (is_regular(current_move(user)) and !is_locked_in_to_bide(user)) {
		regular_move(all_moves(user)).decrement_pp(target);
	}
}

auto ActivePokemon::activate_magnet_rise() -> void {
	m_flags.magnet_rise.activate();
}

auto ActivePokemon::advance_magnet_rise() -> void {
	m_flags.magnet_rise.advance_one_turn();
}

auto ActivePokemon::set_miss(bool const value) -> void {
	m_flags.missed = value;
}

auto ActivePokemon::set_moved(bool const value) -> void {
	m_flags.moved = value;
}

auto ActivePokemon::activate_mud_sport() -> void {
	m_flags.mud_sport = true;
}

auto ActivePokemon::give_nightmares() -> void {
	m_flags.is_having_a_nightmare = true;
}

auto ActivePokemon::partially_trap() -> void {
	m_flags.partial_trap.activate();
}

auto ActivePokemon::partial_trap_damage() -> void {
	m_flags.partial_trap.damage(*this);
}

auto ActivePokemon::activate_perish_song() -> void {
	m_flags.perish_song.activate();
}

auto ActivePokemon::perish_song_turn() -> void {
	bool const faints_this_turn = m_flags.perish_song.advance_one_turn_deactivated();
	if (faints_this_turn) {
		faint();
	}
}

auto ActivePokemon::activate_power_trick() -> void {
	m_flags.power_trick_is_active = !m_flags.power_trick_is_active;
}

auto ActivePokemon::protect() -> void {
	m_flags.is_protecting = true;
}

auto ActivePokemon::break_protect() -> void {
	m_flags.is_protecting = false;
}

auto ActivePokemon::activate_rampage() -> void {
	m_flags.rampage.activate();
}

auto ActivePokemon::recharge() -> bool{
	bool const return_value = is_recharging(*this);
	m_flags.is_recharging = false;
	return return_value;
}

auto ActivePokemon::use_recharge_move() -> void {
	m_flags.is_recharging = true;
}

auto ActivePokemon::roost() -> void {
	m_flags.is_roosting = true;
}

auto ActivePokemon::shed_skin(bool const value) -> void {
	m_flags.shed_skin_activated = value;
}

auto ActivePokemon::increase_sleep_counter() -> void {
	auto & status = get_status(*this);
	auto const & ability = get_ability(*this);
	status.increase_sleep_counter(ability, m_flags.awakening);
}

auto ActivePokemon::increment_stockpile() -> void {
	bool const increased = m_flags.stockpile.increment();
	if (increased) {
		boost_defensive(stage(*this), 1_bi);
	}
}

auto ActivePokemon::release_stockpile() -> bounded::integer<0, Stockpile::max> {
	auto const stages = m_flags.stockpile.release();
	boost_defensive(stage(*this), -stages);
	return stages;
}

auto has_switched(ActivePokemon const & pokemon) -> bool {
	return moved(pokemon) and is_switch(current_move(pokemon));
}

auto ActivePokemon::torment() -> void {
	m_flags.is_tormented = true;
}

auto ActivePokemon::taunt() -> void {
	m_flags.taunt.activate();
}

auto ActivePokemon::advance_taunt() -> void {
	m_flags.taunt.advance_one_turn();
}

auto ActivePokemon::advance_toxic() -> void {
	m_flags.toxic.increment();
}

auto ActivePokemon::u_turn() -> void {
	m_flags.u_turning = true;
}

auto ActivePokemon::use_uproar() -> void {
	m_flags.uproar.advance_one_turn();
}

auto ActivePokemon::activate_water_sport() -> void {
	m_flags.water_sport = true;
}

auto ActivePokemon::hit_with_yawn() -> void {
	m_flags.yawn.activate();
}

auto ActivePokemon::try_to_activate_yawn(Weather const weather) -> void {
	bool const put_to_sleep = m_flags.yawn.advance_one_turn_deactivated();
	if (put_to_sleep) {
		Status::apply<Statuses::sleep>(*this, weather);
	}
}

auto ActivePokemon::bounce() -> bool {
	return m_flags.vanish.bounce();
}
auto ActivePokemon::dig() -> bool {
	return m_flags.vanish.dig();
}
auto ActivePokemon::dive() -> bool {
	return m_flags.vanish.dive();
}
auto ActivePokemon::fly() -> bool {
	return m_flags.vanish.fly();
}
auto ActivePokemon::shadow_force() -> bool {
	return m_flags.vanish.shadow_force();
}

auto ActivePokemon::use_bide(Pokemon & target) -> void {
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

auto ActivePokemon::use_substitute() -> void {
	if (!can_use_substitute(*this))
		return;
	auto const max_hp = get_hp(*this).max();
	indirect_damage(m_flags.substitute.create(max_hp));
}

auto ActivePokemon::direct_damage(damage_type const damage) -> void {
	auto & hp = get_hp(*this);
	auto const initial_hp = hp.current();
	hp -= damage;
	m_flags.damaged = initial_hp - hp.current();
	m_flags.bide.add_damage(damage);
}

auto ActivePokemon::indirect_damage(damage_type const damage) -> void {
	get_hp(*this) -= damage;
}

auto ActivePokemon::register_damage(damage_type const damage) -> void {
	m_flags.damaged = damage;
}

auto ActivePokemon::increment_move_use_counter() -> void {
	if (is_regular(current_move(*this))) {
		auto const move_index = static_cast<LastUsedMove::index_type>(all_moves(*this).index());
		m_flags.last_used_move.increment(move_index);
	} else {
		m_flags.last_used_move = {};
	}
}

auto operator==(ActivePokemon const & lhs, ActivePokemon const & rhs) -> bool {
	return lhs.m_flags == rhs.m_flags;
}

auto operator!=(ActivePokemon const & lhs, ActivePokemon const & rhs) -> bool {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
