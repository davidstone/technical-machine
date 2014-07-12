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

#include "pokemon.hpp"

#include "../ability.hpp"
#include "../damage.hpp"
#include "../rational.hpp"
#include "../weather.hpp"

#include "../move/is_switch.hpp"
#include "../move/move.hpp"
#include "../move/moves.hpp"

namespace technicalmachine {

PokemonCollection const & ActivePokemon::all_pokemon() const {
	return m_all_pokemon;
}
PokemonCollection & ActivePokemon::all_pokemon() {
	return m_all_pokemon;
}

ActivePokemon::operator Pokemon const & () const {
	return all_pokemon()();
}

ActivePokemon::operator Pokemon & () {
	return all_pokemon()();
}

ActivePokemon::operator Species() const {
	return static_cast<Species>(static_cast<Pokemon const &>(*this));
}

bool ActivePokemon::was_used_last(Moves const move_name) const {
	return m_flags.last_used_move.was_used_last(
		static_cast<LastUsedMove::index_type>(*technicalmachine::index(all_moves(*this), move_name))
	);
}


void ActivePokemon::reset_end_of_turn() {
	m_flags.reset_end_of_turn();
}

void ActivePokemon::reset_switch() {
	m_flags.reset_switch();
}

void ActivePokemon::reset_between_turns() {
	m_flags.reset_between_turns();
}

void ActivePokemon::clear_field() {
	m_flags.leech_seeded = false;
	m_flags.partial_trap = {};
}

void ActivePokemon::update_before_move() {
	m_flags.destiny_bond = false;
	m_flags.locked_on = false;
	m_flags.moved = true;
}

bool ActivePokemon::aqua_ring_is_active() const {
	return m_flags.aqua_ring;
}

void ActivePokemon::activate_aqua_ring() {
	m_flags.aqua_ring = true;
}

void ActivePokemon::attract() {
	m_flags.attracted = true;
}

void ActivePokemon::awaken(bool const value) {
	m_flags.awakening = value;
}

bool ActivePokemon::is_baton_passing() const {
	return m_flags.is_baton_passing;
}

void ActivePokemon::baton_pass() {
	m_flags.is_baton_passing = true;
}

bool ActivePokemon::cannot_be_koed() const {
	return m_flags.enduring;
}

bool ActivePokemon::charge_boosted() const {
	return m_flags.charged and get_type(current_move(*this), *this) == Type::Electric;
}

void ActivePokemon::charge() {
	m_flags.charged = true;
}

bool ActivePokemon::is_confused() const {
	return m_flags.confusion.is_active();
}

void ActivePokemon::confuse() {
	if (!get_ability(*this).blocks_confusion()) {
		m_flags.confusion.activate();
	}
}

void ActivePokemon::handle_confusion() {
	m_flags.confusion.do_turn(*this);
}

bool ActivePokemon::critical_hit() const {
	return m_flags.critical_hit;
}

void ActivePokemon::set_critical_hit(bool const value) {
	m_flags.critical_hit = value;
}

void ActivePokemon::curse() {
	m_flags.is_cursed = true;
}

bool ActivePokemon::is_cursed() const {
	return m_flags.is_cursed;
}

void ActivePokemon::defense_curl() {
	m_flags.defense_curled = true;
}

void ActivePokemon::use_destiny_bond() {
	m_flags.destiny_bond = true;
}

bool ActivePokemon::defense_curled() const {
	return m_flags.defense_curled;
}

bool ActivePokemon::is_disabled(Moves const move_name) const {
	return m_flags.disable.move_is_disabled(*index(all_moves(*this), move_name));
}

void ActivePokemon::disable() {
	if (is_regular(current_move(*this))) {
		m_flags.disable.activate(RegularMoveIndex(all_moves(*this).index(), bounded::non_check));
	}
}

void ActivePokemon::advance_disable() {
	m_flags.disable.advance_one_turn();
}

void ActivePokemon::activate_embargo() {
	m_flags.embargo.activate();
}

void ActivePokemon::advance_embargo() {
	m_flags.embargo.advance_one_turn();
}

bool ActivePokemon::is_encored() const {
	return m_flags.encore.is_active();
}

void ActivePokemon::activate_encore() {
	m_flags.encore.activate();
}

void ActivePokemon::advance_encore() {
	m_flags.encore.advance_one_turn();
}

void ActivePokemon::endure() {
	m_flags.enduring = true;
}

bool ActivePokemon::is_fainted() const {
	return m_flags.will_be_replaced;
}

void ActivePokemon::faint() {
	get_hp(*this) = 0_bi;
	m_flags.will_be_replaced = true;
}

bool ActivePokemon::flash_fire_is_active() const {
	return m_flags.flash_fire;
}

void ActivePokemon::activate_flash_fire() {
	m_flags.flash_fire = true;
}

bool ActivePokemon::flinched() const {
	return m_flags.flinched;
}

void ActivePokemon::flinch() {
	m_flags.flinched = true;
}

auto ActivePokemon::has_focused_energy() const -> bool {
	return m_flags.has_focused_energy;
}

void ActivePokemon::focus_energy() {
	m_flags.has_focused_energy = true;
}

void ActivePokemon::fully_trap() {
	m_flags.fully_trapped = true;
}

void ActivePokemon::identify() {
	m_flags.identified = true;
}

bool ActivePokemon::used_imprison() const {
	return m_flags.used_imprison;
}

void ActivePokemon::use_imprison() {
	m_flags.used_imprison = true;
}

bool ActivePokemon::ingrained() const {
	return m_flags.ingrained;
}

void ActivePokemon::ingrain() {
	m_flags.ingrained = true;
}

bool ActivePokemon::heal_block_is_active() const {
	return m_flags.heal_block.is_active();
}

void ActivePokemon::activate_heal_block() {
	m_flags.heal_block.activate();
}

void ActivePokemon::advance_heal_block() {
	m_flags.heal_block.advance_one_turn();
}

bool ActivePokemon::is_fully_paralyzed() const {
	return m_flags.is_fully_paralyzed;
}

bool ActivePokemon::leech_seeded() const {
	return m_flags.leech_seeded;
}

void ActivePokemon::hit_with_leech_seed() {
	m_flags.leech_seeded = true;
}

bool ActivePokemon::is_loafing() const {
	return get_ability(*this).is_loafing(m_flags.is_loafing_turn);
}

void ActivePokemon::advance_lock_in() {
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

bool ActivePokemon::locked_on() const {
	return m_flags.locked_on;
}

void ActivePokemon::use_lock_on() {
	m_flags.locked_on = true;
}

void ActivePokemon::lower_pp(Ability const & target) {
	if (is_regular(current_move(*this)) and !is_locked_in_to_bide()) {
		regular_move(all_moves(*this)).decrement_pp(target);
	}
}

auto ActivePokemon::magnet_rise() const -> MagnetRise const & {
	return m_flags.magnet_rise;
}

void ActivePokemon::activate_magnet_rise() {
	m_flags.magnet_rise.activate();
}

void ActivePokemon::advance_magnet_rise() {
	m_flags.magnet_rise.advance_one_turn();
}

bool ActivePokemon::me_first_is_active() const {
	return m_flags.me_first_is_active;
}

bool ActivePokemon::minimized() const {
	return m_flags.minimized;
}

bool ActivePokemon::missed() const {
	return m_flags.missed;
}

void ActivePokemon::set_miss(bool const value) {
	m_flags.missed = value;
}

void ActivePokemon::set_moved(bool const value) {
	m_flags.moved = value;
}

bool ActivePokemon::moved() const {
	return m_flags.moved;
}

bool ActivePokemon::moved_since_switch() const {
	return m_flags.last_used_move.has_moved();
}

void ActivePokemon::activate_mud_sport() {
	m_flags.mud_sport = true;
}

bool ActivePokemon::is_having_a_nightmare() const {
	return m_flags.is_having_a_nightmare;
}

void ActivePokemon::give_nightmares() {
	m_flags.is_having_a_nightmare = true;
}

void ActivePokemon::partially_trap() {
	m_flags.partial_trap.activate();
}

void ActivePokemon::partial_trap_damage() {
	m_flags.partial_trap.damage(*this);
}

void ActivePokemon::activate_perish_song() {
	m_flags.perish_song.activate();
}

void ActivePokemon::perish_song_turn() {
	bool const faints_this_turn = m_flags.perish_song.advance_one_turn_deactivated();
	if (faints_this_turn) {
		faint();
	}
}

bool ActivePokemon::power_trick_is_active() const {
	return m_flags.power_trick_is_active;
}

void ActivePokemon::activate_power_trick() {
	m_flags.power_trick_is_active = !m_flags.power_trick_is_active;
}

void ActivePokemon::protect() {
	m_flags.is_protecting = true;
}

void ActivePokemon::break_protect() {
	m_flags.is_protecting = false;
}

void ActivePokemon::activate_rampage() {
	m_flags.rampage.activate();
}

bool ActivePokemon::is_recharging() const {
	return m_flags.is_recharging;
}

bool ActivePokemon::recharge() {
	bool const return_value = is_recharging();
	m_flags.is_recharging = false;
	return return_value;
}

void ActivePokemon::use_recharge_move() {
	m_flags.is_recharging = true;
}

bool ActivePokemon::is_roosting() const {
	return m_flags.is_roosting;
}

void ActivePokemon::roost() {
	m_flags.is_roosting = true;
}

bool ActivePokemon::shed_skin_activated() const {
	return m_flags.shed_skin_activated;
}

void ActivePokemon::shed_skin(bool const value) {
	m_flags.shed_skin_activated = value;
}

void ActivePokemon::increase_sleep_counter() {
	auto & status = get_status(*this);
	auto const & ability = get_ability(*this);
	status.increase_sleep_counter(ability, m_flags.awakening);
}

bool ActivePokemon::slow_start_is_active() const {
	return m_flags.slow_start.is_active();
}

bool ActivePokemon::sport_is_active(Move const & foe_move) const {
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

void ActivePokemon::increment_stockpile() {
	bool const increased = m_flags.stockpile.increment();
	if (increased) {
		boost_defensive(stage(), 1_bi);
	}
}

bounded::integer<0, Stockpile::max> ActivePokemon::release_stockpile() {
	auto const stages = m_flags.stockpile.release();
	boost_defensive(stage(), -stages);
	return stages;
}

bool ActivePokemon::is_switching_to_self () const {
	return all_pokemon().is_switching_to_self();
}

bool ActivePokemon::is_switching_to_self(Moves const switch_move) const {
	return all_pokemon().is_switching_to_self(switch_move);
}

bool ActivePokemon::has_switched() const {
	return moved() and is_switch(current_move(*this));
}

bool ActivePokemon::switch_decision_required() const {
	return m_flags.is_baton_passing or m_flags.u_turning or will_be_replaced();
}

void switch_pokemon(ActivePokemon & pokemon) {
	if (get_ability(pokemon).clears_status_on_switch()) {
		get_status(pokemon) = Status{};
	}
	pokemon.all_pokemon().set_index(pokemon.all_pokemon().replacement());
}

auto ActivePokemon::fully_trapped() const -> bool {
	return m_flags.fully_trapped;
}

bool ActivePokemon::trapped() const {
	return m_flags.fully_trapped or ingrained() or m_flags.partial_trap.is_active();
}

bool ActivePokemon::is_tormented() const {
	return m_flags.is_tormented;
}

void ActivePokemon::torment() {
	m_flags.is_tormented = true;
}

void ActivePokemon::taunt() {
	m_flags.taunt.activate();
}

bool ActivePokemon::is_taunted() const {
	return m_flags.taunt.is_active();
}

void ActivePokemon::advance_taunt() {
	m_flags.taunt.advance_one_turn();
}

void ActivePokemon::advance_toxic() {
	m_flags.toxic.increment();
}

void ActivePokemon::u_turn() {
	m_flags.u_turning = true;
}

void ActivePokemon::use_uproar() {
	m_flags.uproar.advance_one_turn();
}

bool ActivePokemon::vanish_doubles_power(Moves const move_name) const {
	return m_flags.vanish.doubles_move_power(move_name);
}

void ActivePokemon::activate_water_sport() {
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

bool ActivePokemon::bounce() {
	return m_flags.vanish.bounce();
}
bool ActivePokemon::dig() {
	return m_flags.vanish.dig();
}
bool ActivePokemon::dive() {
	return m_flags.vanish.dive();
}
bool ActivePokemon::fly() {
	return m_flags.vanish.fly();
}
bool ActivePokemon::shadow_force() {
	return m_flags.vanish.shadow_force();
}

void ActivePokemon::use_bide(Pokemon & target) {
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

bool can_use_substitute(Pokemon const & pokemon) {
	auto const & hp = get_hp(pokemon);
	return hp.current() > hp.max() / 4_bi;
}

}	// namespace

void ActivePokemon::use_substitute() {
	if (!can_use_substitute(*this))
		return;
	auto const max_hp = get_hp(*this).max();
	indirect_damage(m_flags.substitute.create(max_hp));
}

bool ActivePokemon::is_locked_in_to_bide() const {
	return m_flags.bide.is_active();
}

bounded::integer<0, HP::max_value> ActivePokemon::damaged() const {
	return m_flags.damaged;
}

void ActivePokemon::direct_damage(damage_type const damage) {
	auto & hp = get_hp(*this);
	auto const initial_hp = hp.current();
	hp -= damage;
	m_flags.damaged = initial_hp - hp.current();
	m_flags.bide.add_damage(damage);
}

void ActivePokemon::indirect_damage(damage_type const damage) {
	get_hp(*this) -= damage;
}

void ActivePokemon::register_damage(damage_type const damage) {
	m_flags.damaged = damage;
}

void ActivePokemon::increment_move_use_counter() {
	if (is_regular(current_move(*this))) {
		m_flags.last_used_move.increment(static_cast<LastUsedMove::index_type>(all_moves(*this).index()));
	} else {
		m_flags.last_used_move = LastUsedMove{};
	}
}

void ActivePokemon::update_chance_to_hit(ActivePokemon const & target, Weather const weather, bool target_moved) {
	m_flags.chance_to_hit = chance_to_hit(*this, target, weather, target_moved);
}

auto ActivePokemon::accuracy_probability() const -> ChanceToHit {
	return missed() ? complement(m_flags.chance_to_hit) : m_flags.chance_to_hit;
}

bool ActivePokemon::will_be_replaced() const {
	return m_flags.will_be_replaced;
}

void ActivePokemon::normalize_hp(bool fainted) {
	if (fainted) {
		faint();
	}
	else {
		HP & hp = get_hp(*this);
		hp = bounded::max(hp.current(), 1_bi);
	}
}

bool operator== (ActivePokemon const & lhs, ActivePokemon const & rhs) {
	return lhs.m_flags == rhs.m_flags;
}

bool operator!= (ActivePokemon const & lhs, ActivePokemon const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
