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

auto ActivePokemon::last_used_move() const -> LastUsedMove {
	return m_flags.last_used_move;
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

auto ActivePokemon::aqua_ring_is_active() const -> bool {
	return m_flags.aqua_ring;
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

auto ActivePokemon::is_baton_passing() const -> bool {
	return m_flags.is_baton_passing;
}

auto ActivePokemon::baton_pass() -> void {
	m_flags.is_baton_passing = true;
}

auto ActivePokemon::cannot_be_koed() const -> bool {
	return m_flags.enduring;
}

auto ActivePokemon::charge_boosted() const -> bool {
	return m_flags.charged and get_type(current_move(*this), *this) == Type::Electric;
}

auto ActivePokemon::charge() -> void {
	m_flags.charged = true;
}

auto ActivePokemon::is_confused() const -> bool {
	return m_flags.confusion.is_active();
}

auto ActivePokemon::confuse() -> void {
	if (!get_ability(*this).blocks_confusion()) {
		m_flags.confusion.activate();
	}
}

auto ActivePokemon::handle_confusion() -> void {
	m_flags.confusion.do_turn(*this);
}

auto ActivePokemon::critical_hit() const -> bool {
	return m_flags.critical_hit;
}

auto ActivePokemon::set_critical_hit(bool const value) -> void {
	m_flags.critical_hit = value;
}

auto ActivePokemon::curse() -> void {
	m_flags.is_cursed = true;
}

auto ActivePokemon::is_cursed() const -> bool {
	return m_flags.is_cursed;
}

auto ActivePokemon::defense_curl() -> void {
	m_flags.defense_curled = true;
}

auto ActivePokemon::use_destiny_bond() -> void {
	m_flags.destiny_bond = true;
}

auto ActivePokemon::defense_curled() const -> bool {
	return m_flags.defense_curled;
}

auto ActivePokemon::is_disabled(Moves const move_name) const -> bool {
	return m_flags.disable.move_is_disabled(*index(all_moves(*this), move_name));
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

auto ActivePokemon::is_encored() const -> bool {
	return m_flags.encore.is_active();
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

auto ActivePokemon::is_fainted() const -> bool {
	return m_flags.will_be_replaced;
}

auto ActivePokemon::faint() -> void {
	get_hp(*this) = 0_bi;
	m_flags.will_be_replaced = true;
}

auto ActivePokemon::flash_fire_is_active() const -> bool {
	return m_flags.flash_fire;
}

auto ActivePokemon::activate_flash_fire() -> void {
	m_flags.flash_fire = true;
}

auto ActivePokemon::flinched() const -> bool {
	return m_flags.flinched;
}

auto ActivePokemon::flinch() -> void {
	m_flags.flinched = true;
}

auto ActivePokemon::has_focused_energy() const -> bool {
	return m_flags.has_focused_energy;
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

auto ActivePokemon::used_imprison() const -> bool {
	return m_flags.used_imprison;
}

auto ActivePokemon::use_imprison() -> void {
	m_flags.used_imprison = true;
}

auto ActivePokemon::ingrained() const -> bool {
	return m_flags.ingrained;
}

auto ActivePokemon::ingrain() -> void {
	m_flags.ingrained = true;
}

auto ActivePokemon::heal_block_is_active() const -> bool {
	return m_flags.heal_block.is_active();
}

auto ActivePokemon::activate_heal_block() -> void {
	m_flags.heal_block.activate();
}

auto ActivePokemon::advance_heal_block() -> void {
	m_flags.heal_block.advance_one_turn();
}

auto ActivePokemon::is_fully_paralyzed() const -> bool {
	return m_flags.is_fully_paralyzed;
}

auto ActivePokemon::leech_seeded() const -> bool {
	return m_flags.leech_seeded;
}

auto ActivePokemon::hit_with_leech_seed() -> void {
	m_flags.leech_seeded = true;
}

auto ActivePokemon::is_loafing() const -> bool {
	return get_ability(*this).is_loafing(m_flags.is_loafing_turn);
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

auto ActivePokemon::locked_on() const -> bool {
	return m_flags.locked_on;
}

auto ActivePokemon::use_lock_on() -> void {
	m_flags.locked_on = true;
}

auto lower_pp(ActivePokemon & user, Ability const target) -> void {
	if (is_regular(current_move(user)) and !user.is_locked_in_to_bide()) {
		regular_move(all_moves(user)).decrement_pp(target);
	}
}

auto ActivePokemon::magnet_rise() const -> MagnetRise const & {
	return m_flags.magnet_rise;
}

auto ActivePokemon::activate_magnet_rise() -> void {
	m_flags.magnet_rise.activate();
}

auto ActivePokemon::advance_magnet_rise() -> void {
	m_flags.magnet_rise.advance_one_turn();
}

auto ActivePokemon::me_first_is_active() const -> bool {
	return m_flags.me_first_is_active;
}

auto ActivePokemon::minimized() const -> bool {
	return m_flags.minimized;
}

auto ActivePokemon::missed() const -> bool {
	return m_flags.missed;
}

auto ActivePokemon::set_miss(bool const value) -> void {
	m_flags.missed = value;
}

auto ActivePokemon::set_moved(bool const value) -> void {
	m_flags.moved = value;
}

auto ActivePokemon::moved() const -> bool {
	return m_flags.moved;
}

auto ActivePokemon::activate_mud_sport() -> void {
	m_flags.mud_sport = true;
}

auto ActivePokemon::is_having_a_nightmare() const -> bool {
	return m_flags.is_having_a_nightmare;
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

auto ActivePokemon::power_trick_is_active() const -> bool {
	return m_flags.power_trick_is_active;
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

auto ActivePokemon::is_recharging() const -> bool {
	return m_flags.is_recharging;
}

auto ActivePokemon::recharge() -> bool{
	bool const return_value = is_recharging();
	m_flags.is_recharging = false;
	return return_value;
}

auto ActivePokemon::use_recharge_move() -> void {
	m_flags.is_recharging = true;
}

auto ActivePokemon::is_roosting() const -> bool {
	return m_flags.is_roosting;
}

auto ActivePokemon::roost() -> void {
	m_flags.is_roosting = true;
}

auto ActivePokemon::shed_skin_activated() const -> bool {
	return m_flags.shed_skin_activated;
}

auto ActivePokemon::shed_skin(bool const value) -> void {
	m_flags.shed_skin_activated = value;
}

auto ActivePokemon::increase_sleep_counter() -> void {
	auto & status = get_status(*this);
	auto const & ability = get_ability(*this);
	status.increase_sleep_counter(ability, m_flags.awakening);
}

auto ActivePokemon::slow_start_is_active() const -> bool {
	return m_flags.slow_start.is_active();
}

auto ActivePokemon::sport_is_active(Move const & foe_move) const -> bool {
	switch (get_type(foe_move, *this)) {
	case Type::Electric:
		return m_flags.mud_sport;
	case Type::Fire:
		return m_flags.water_sport;
	default:
		return false;
	}
}

auto ActivePokemon::stage() const -> Stage const & {
	return m_flags.stage;
}
auto ActivePokemon::stage() -> Stage & {
	return m_flags.stage;
}

auto ActivePokemon::increment_stockpile() -> void {
	bool const increased = m_flags.stockpile.increment();
	if (increased) {
		boost_defensive(stage(), 1_bi);
	}
}

auto ActivePokemon::release_stockpile() -> bounded::integer<0, Stockpile::max> {
	auto const stages = m_flags.stockpile.release();
	boost_defensive(stage(), -stages);
	return stages;
}

auto has_switched(ActivePokemon const & pokemon) -> bool {
	return pokemon.moved() and is_switch(current_move(pokemon));
}

auto ActivePokemon::switch_decision_required() const -> bool {
	return m_flags.is_baton_passing or m_flags.u_turning or will_be_replaced();
}

auto ActivePokemon::fully_trapped() const -> bool {
	return m_flags.fully_trapped;
}

auto ActivePokemon::trapped() const -> bool {
	return m_flags.fully_trapped or ingrained() or m_flags.partial_trap.is_active();
}

auto ActivePokemon::is_tormented() const -> bool {
	return m_flags.is_tormented;
}

auto ActivePokemon::torment() -> void {
	m_flags.is_tormented = true;
}

auto ActivePokemon::taunt() -> void {
	m_flags.taunt.activate();
}

auto ActivePokemon::is_taunted() const -> bool {
	return m_flags.taunt.is_active();
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

auto ActivePokemon::vanish_doubles_power(Moves const move_name) const -> bool {
	return m_flags.vanish.doubles_move_power(move_name);
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

auto ActivePokemon::substitute() const -> Substitute const & {
	return m_flags.substitute;
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

auto ActivePokemon::is_locked_in_to_bide() const -> bool {
	return m_flags.bide.is_active();
}

auto ActivePokemon::damaged() const -> bounded::integer<0, HP::max_value> {
	return m_flags.damaged;
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

auto ActivePokemon::update_chance_to_hit(ActivePokemon const & target, Weather const weather, bool target_moved) -> void {
	m_flags.chance_to_hit = chance_to_hit(*this, target, weather, target_moved);
}

auto ActivePokemon::accuracy_probability() const -> ChanceToHit {
	return missed() ? complement(m_flags.chance_to_hit) : m_flags.chance_to_hit;
}

auto ActivePokemon::will_be_replaced() const -> bool {
	return m_flags.will_be_replaced;
}

auto operator==(ActivePokemon const & lhs, ActivePokemon const & rhs) -> bool {
	return lhs.m_flags == rhs.m_flags;
}

auto operator!=(ActivePokemon const & lhs, ActivePokemon const & rhs) -> bool {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
