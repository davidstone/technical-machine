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

MoveCollection const & ActivePokemon::all_moves() const {
	auto const & self = static_cast<Pokemon const &>(*this);
	return self.move;
}

MoveCollection & ActivePokemon::all_moves() {
	auto & self = static_cast<Pokemon &>(*this);
	return self.move;
}

bool ActivePokemon::was_used_last(Moves const move_name) const {
	return m_last_used_move.was_used_last(static_cast<LastUsedMove::index_type>(*technicalmachine::index(all_moves(), move_name)));
}


void ActivePokemon::reset_end_of_turn() {
	m_damaged = 0_bi;
	m_enduring = false;
	m_flinched = false;
	m_moved = false;
	m_me_first_is_active = false;
	m_is_loafing_turn = !m_is_loafing_turn;
	m_is_protecting = false;
}

void ActivePokemon::reset_switch() {
	// TODO: remove some of these when the foe switches, too
	if (!is_baton_passing()) {
		m_aqua_ring = false;
		m_confusion = {};
		m_is_cursed = false;
		m_embargo = {};
		m_has_focused_energy = false;
		m_gastro_acid = false;
		m_ingrained = false;
		m_leech_seeded = false;
		m_locked_on = false;
		m_magnet_rise = {};
		m_perish_song = {};
		m_power_trick_is_active = false;
		m_stage = Stage{};
		m_substitute = {};
	}
	m_attracted = false;
	m_charged = false;
	m_defense_curled = false;
	m_destiny_bond = false;
	m_disable = Disable{};
	m_encore = {};
	m_flash_fire = false;
	m_flinched = false;
	m_fully_trapped = false;
	m_heal_block = {};
	m_identified = false;
	m_used_imprison = false;
	m_last_used_move = {};
	// Do I set to true or false? true makes it wrong when a fainted Pokemon is
	// replaced; false makes it wrong otherwise
	m_is_loafing_turn = false;
	m_minimized = false;
	m_me_first_is_active = false;
	m_mud_sport = false;
	m_is_having_a_nightmare = false;
	m_partial_trap = {};
	m_is_baton_passing = false;
	m_rampage = {};
	m_is_roosting = false;
	m_slow_start = {};
	m_stockpile = {};
	m_is_tormented = false;
	m_u_turning = false;
	m_water_sport = false;
	m_bide = {};
	m_damaged = 0_bi;
	m_taunt = {};
	m_toxic = {};
	m_uproar = {};
	// Whirlwind can hit Flying Pokemon, so it's possible to switch while
	// vanished. Therefore, we need to reset it.
	m_vanish = {};
	m_yawn = {};
	m_will_be_replaced = false;
}

void ActivePokemon::reset_between_turns() {
	m_critical_hit = false;
	m_is_fully_paralyzed = false;
	m_confusion.end_of_turn_reset();
	m_missed = false;
}

void ActivePokemon::clear_field() {
	clear_leech_seed();
	m_partial_trap = {};
}

void ActivePokemon::update_before_move() {
	m_destiny_bond = false;
	m_locked_on = false;
	m_moved = true;
}

void ActivePokemon::clear_leech_seed() {
	m_leech_seeded = false;
}

bool ActivePokemon::aqua_ring_is_active() const {
	return m_aqua_ring;
}

void ActivePokemon::activate_aqua_ring() {
	m_aqua_ring = true;
}

void ActivePokemon::attract() {
	m_attracted = true;
}

void ActivePokemon::awaken(bool const value) {
	m_awakening = value;
}

bool ActivePokemon::is_baton_passing() const {
	return m_is_baton_passing;
}

void ActivePokemon::baton_pass() {
	m_is_baton_passing = true;
}

bool ActivePokemon::cannot_be_koed() const {
	return m_enduring;
}

bool ActivePokemon::charge_boosted() const {
	return m_charged and get_type(move(), *this) == Type::Electric;
}

void ActivePokemon::charge() {
	m_charged = true;
}

bool ActivePokemon::is_confused() const {
	return m_confusion.is_active();
}

void ActivePokemon::confuse() {
	if (!get_ability(*this).blocks_confusion()) {
		m_confusion.activate();
	}
}

void ActivePokemon::handle_confusion() {
	m_confusion.do_turn(*this);
}

bool ActivePokemon::critical_hit() const {
	return m_critical_hit;
}

void ActivePokemon::set_critical_hit(bool const value) {
	m_critical_hit = value;
}

void ActivePokemon::curse() {
	m_is_cursed = true;
}

bool ActivePokemon::is_cursed() const {
	return m_is_cursed;
}

void ActivePokemon::defense_curl() {
	m_defense_curled = true;
}

void ActivePokemon::use_destiny_bond() {
	m_destiny_bond = true;
}

bool ActivePokemon::defense_curled() const {
	return m_defense_curled;
}

bool ActivePokemon::is_disabled(Moves const move_name) const {
	return m_disable.move_is_disabled(*technicalmachine::index(all_moves(), move_name));
}

void ActivePokemon::disable() {
	if (is_regular(move())) {
		m_disable.activate(RegularMoveIndex(all_moves().index(), bounded::non_check));
	}
}

void ActivePokemon::advance_disable() {
	m_disable.advance_one_turn();
}

void ActivePokemon::activate_embargo() {
	m_embargo.activate();
}

void ActivePokemon::advance_embargo() {
	m_embargo.advance_one_turn();
}

bool ActivePokemon::is_encored() const {
	return m_encore.is_active();
}

void ActivePokemon::activate_encore() {
	m_encore.activate();
}

void ActivePokemon::advance_encore() {
	m_encore.advance_one_turn();
}

void ActivePokemon::endure() {
	m_enduring = true;
}

bool ActivePokemon::is_fainted() const {
	return m_will_be_replaced;
}

void ActivePokemon::faint() {
	get_hp(*this) = 0_bi;
	m_will_be_replaced = true;
}

bool ActivePokemon::flash_fire_is_active() const {
	return m_flash_fire;
}

void ActivePokemon::activate_flash_fire() {
	m_flash_fire = true;
}

bool ActivePokemon::flinched() const {
	return m_flinched;
}

void ActivePokemon::flinch() {
	m_flinched = true;
}

auto ActivePokemon::has_focused_energy() const -> bool {
	return m_has_focused_energy;
}

void ActivePokemon::focus_energy() {
	m_has_focused_energy = true;
}

void ActivePokemon::fully_trap() {
	m_fully_trapped = true;
}

void ActivePokemon::identify() {
	m_identified = true;
}

bool ActivePokemon::used_imprison() const {
	return m_used_imprison;
}

void ActivePokemon::use_imprison() {
	m_used_imprison = true;
}

PokemonCollection::index_type ActivePokemon::index() const {
	return all_pokemon().index();
}

bool ActivePokemon::ingrained() const {
	return m_ingrained;
}

void ActivePokemon::ingrain() {
	m_ingrained = true;
}

bool ActivePokemon::heal_block_is_active() const {
	return m_heal_block.is_active();
}

void ActivePokemon::activate_heal_block() {
	m_heal_block.activate();
}

void ActivePokemon::advance_heal_block() {
	m_heal_block.advance_one_turn();
}

bool ActivePokemon::is_fully_paralyzed() const {
	return m_is_fully_paralyzed;
}

bool ActivePokemon::leech_seeded() const {
	return m_leech_seeded;
}

void ActivePokemon::hit_with_leech_seed() {
	m_leech_seeded = true;
}

bool ActivePokemon::is_loafing() const {
	return get_ability(*this).is_loafing(m_is_loafing_turn);
}

void ActivePokemon::advance_lock_in() {
	// Cannot be locked into Rampage and Uproar at the same time
	if (m_rampage.is_active()) {
		m_rampage.advance_one_turn();
		if (!m_rampage.is_active()) {
			confuse();
		}
	} else {
		m_uproar.advance_one_turn();
	}
}

bool ActivePokemon::locked_on() const {
	return m_locked_on;
}

void ActivePokemon::use_lock_on() {
	m_locked_on = true;
}

void ActivePokemon::lower_pp(Ability const & target) {
	if (is_regular(move()) and !is_locked_in_to_bide()) {
		regular_move(all_moves()).decrement_pp(target);
	}
}

auto ActivePokemon::magnet_rise() const -> MagnetRise const & {
	return m_magnet_rise;
}

void ActivePokemon::activate_magnet_rise() {
	m_magnet_rise.activate();
}

void ActivePokemon::advance_magnet_rise() {
	m_magnet_rise.advance_one_turn();
}

bool ActivePokemon::me_first_is_active() const {
	return m_me_first_is_active;
}

bool ActivePokemon::minimized() const {
	return m_minimized;
}

bool ActivePokemon::missed() const {
	return m_missed;
}

void ActivePokemon::set_miss(bool const value) {
	m_missed = value;
}

void ActivePokemon::set_moved(bool const value) {
	m_moved = value;
}

bool ActivePokemon::moved() const {
	return m_moved;
}

bool ActivePokemon::moved_since_switch() const {
	return m_last_used_move.has_moved();
}

void ActivePokemon::activate_mud_sport() {
	m_mud_sport = true;
}

bool ActivePokemon::is_having_a_nightmare() const {
	return m_is_having_a_nightmare;
}

void ActivePokemon::give_nightmares() {
	m_is_having_a_nightmare = true;
}

void ActivePokemon::partially_trap() {
	m_partial_trap.activate();
}

void ActivePokemon::partial_trap_damage() {
	m_partial_trap.damage(*this);
}

void ActivePokemon::activate_perish_song() {
	m_perish_song.activate();
}

void ActivePokemon::perish_song_turn() {
	bool const faints_this_turn = m_perish_song.advance_one_turn_deactivated();
	if (faints_this_turn) {
		faint();
	}
}

bool ActivePokemon::can_be_phazed() const {
	return !ingrained() and !get_ability(*this).blocks_phazing() and all_pokemon().size() > 1;
}

bool ActivePokemon::power_trick_is_active() const {
	return m_power_trick_is_active;
}

void ActivePokemon::activate_power_trick() {
	m_power_trick_is_active = !m_power_trick_is_active;
}

void ActivePokemon::protect() {
	m_is_protecting = true;
}

void ActivePokemon::break_protect() {
	m_is_protecting = false;
}

void ActivePokemon::activate_rampage() {
	m_rampage.activate();
}

bool ActivePokemon::is_recharging() const {
	return m_is_recharging;
}

bool ActivePokemon::recharge() {
	bool const return_value = is_recharging();
	m_is_recharging = false;
	return return_value;
}

void ActivePokemon::use_recharge_move() {
	m_is_recharging = true;
}

bool ActivePokemon::is_roosting() const {
	return m_is_roosting;
}

void ActivePokemon::roost() {
	m_is_roosting = true;
}

bool ActivePokemon::shed_skin_activated() const {
	return m_shed_skin_activated;
}

void ActivePokemon::shed_skin(bool const value) {
	m_shed_skin_activated = value;
}

void ActivePokemon::increase_sleep_counter() {
	auto & status = get_status(*this);
	auto const & ability = get_ability(*this);
	status.increase_sleep_counter(ability, m_awakening);
}

bool ActivePokemon::slow_start_is_active() const {
	return m_slow_start.is_active();
}

bool ActivePokemon::sport_is_active(Move const & foe_move) const {
	switch (get_type(foe_move, *this)) {
	case Type::Electric:
		return m_mud_sport;
	case Type::Fire:
		return m_water_sport;
	default:
		return false;
	}
}

auto ActivePokemon::stage() const -> Stage const & {
	return m_stage;
}
auto ActivePokemon::stage() -> Stage & {
	return m_stage;
}

void ActivePokemon::increment_stockpile() {
	bool const increased = m_stockpile.increment();
	if (increased) {
		boost_defensive(stage(), 1_bi);
	}
}

bounded::integer<0, Stockpile::max> ActivePokemon::release_stockpile() {
	auto const stages = m_stockpile.release();
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
	return moved() and is_switch(move());
}

bool ActivePokemon::switch_decision_required() const {
	return m_is_baton_passing or m_u_turning or will_be_replaced();
}

void switch_pokemon(ActivePokemon & pokemon) {
	if (get_ability(pokemon).clears_status_on_switch()) {
		get_status(pokemon) = Status{};
	}
	pokemon.all_pokemon().set_index(pokemon.all_pokemon().replacement());
}

auto ActivePokemon::fully_trapped() const -> bool {
	return m_fully_trapped;
}

bool ActivePokemon::trapped() const {
	return m_fully_trapped or ingrained() or m_partial_trap.is_active();
}

bool ActivePokemon::is_tormented() const {
	return m_is_tormented;
}

void ActivePokemon::torment() {
	m_is_tormented = true;
}

void ActivePokemon::taunt() {
	m_taunt.activate();
}

bool ActivePokemon::is_taunted() const {
	return m_taunt.is_active();
}

void ActivePokemon::advance_taunt() {
	m_taunt.advance_one_turn();
}

void ActivePokemon::advance_toxic() {
	m_toxic.increment();
}

void ActivePokemon::u_turn() {
	m_u_turning = true;
}

void ActivePokemon::use_uproar() {
	m_uproar.advance_one_turn();
}

bool ActivePokemon::vanish_doubles_power(Moves const move_name) const {
	return m_vanish.doubles_move_power(move_name);
}

void ActivePokemon::activate_water_sport() {
	m_water_sport = true;
}

auto ActivePokemon::hit_with_yawn() -> void {
	m_yawn.activate();
}

auto ActivePokemon::try_to_activate_yawn(Weather const weather) -> void {
	bool const put_to_sleep = m_yawn.advance_one_turn_deactivated();
	if (put_to_sleep) {
		Status::apply<Statuses::sleep>(*this, weather);
	}
}

bool ActivePokemon::bounce() {
	return m_vanish.bounce();
}
bool ActivePokemon::dig() {
	return m_vanish.dig();
}
bool ActivePokemon::dive() {
	return m_vanish.dive();
}
bool ActivePokemon::fly() {
	return m_vanish.fly();
}
bool ActivePokemon::shadow_force() {
	return m_vanish.shadow_force();
}

void ActivePokemon::use_bide(Pokemon & target) {
	if (!m_bide.is_active()) {
		m_bide.activate();
	}
	else {
		auto const bide_damage = m_bide.decrement();
		get_hp(target) -= bide_damage * 2_bi;
	}
}

auto ActivePokemon::substitute() const -> Substitute const & {
	return m_substitute;
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
	indirect_damage(m_substitute.create(max_hp));
}

bool ActivePokemon::is_locked_in_to_bide() const {
	return m_bide.is_active();
}

bounded::integer<0, HP::max_value> ActivePokemon::damaged() const {
	return m_damaged;
}

void ActivePokemon::direct_damage(damage_type const damage) {
	auto & hp = get_hp(*this);
	auto const initial_hp = hp.current();
	hp -= damage;
	m_damaged = initial_hp - hp.current();
	m_bide.add_damage(damage);
}

void ActivePokemon::indirect_damage(damage_type const damage) {
	get_hp(*this) -= damage;
}

void ActivePokemon::register_damage(damage_type const damage) {
	m_damaged = damage;
}

void ActivePokemon::increment_move_use_counter() {
	if (is_regular(move())) {
		m_last_used_move.increment(static_cast<LastUsedMove::index_type>(all_moves().index()));
	} else {
		m_last_used_move = LastUsedMove{};
	}
}

void ActivePokemon::update_chance_to_hit(ActivePokemon const & target, Weather const weather, bool target_moved) {
	m_chance_to_hit = chance_to_hit(*this, target, weather, target_moved);
}

auto ActivePokemon::accuracy_probability() const -> ChanceToHit {
	return missed() ? complement(m_chance_to_hit) : m_chance_to_hit;
}

bool ActivePokemon::will_be_replaced() const {
	return m_will_be_replaced;
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
	return
			lhs.m_all_pokemon == rhs.m_all_pokemon and
			lhs.m_aqua_ring == rhs.m_aqua_ring and
			lhs.m_attracted == rhs.m_attracted and
			lhs.m_bide == rhs.m_bide and
			lhs.m_charged == rhs.m_charged and
			lhs.m_confusion == rhs.m_confusion and
			lhs.m_is_cursed == rhs.m_is_cursed and
			lhs.m_defense_curled == rhs.m_defense_curled and
			lhs.m_destiny_bond == rhs.m_destiny_bond and
			lhs.m_disable == rhs.m_disable and
			lhs.m_embargo == rhs.m_embargo and
			lhs.m_encore == rhs.m_encore and
			lhs.m_flash_fire == rhs.m_flash_fire and
			lhs.has_focused_energy() == rhs.has_focused_energy() and
			lhs.fully_trapped() == rhs.fully_trapped() and
			lhs.m_heal_block == rhs.m_heal_block and
			lhs.m_identified == rhs.m_identified and
			lhs.m_used_imprison == rhs.m_used_imprison and
			lhs.m_ingrained == rhs.m_ingrained and
			lhs.m_last_used_move == rhs.m_last_used_move and
			lhs.m_leech_seeded == rhs.m_leech_seeded and
			lhs.m_is_loafing_turn == rhs.m_is_loafing_turn and
			lhs.m_locked_on == rhs.m_locked_on and
			lhs.magnet_rise() == rhs.magnet_rise() and
			lhs.m_minimized == rhs.m_minimized and
			lhs.m_mud_sport == rhs.m_mud_sport and
			lhs.m_is_having_a_nightmare == rhs.m_is_having_a_nightmare and
			lhs.m_partial_trap == rhs.m_partial_trap and
			lhs.m_perish_song == rhs.m_perish_song and
			lhs.m_rampage == rhs.m_rampage and
			lhs.m_slow_start == rhs.m_slow_start and
			lhs.stage() == rhs.stage() and
			lhs.m_stockpile == rhs.m_stockpile and
			lhs.m_taunt == rhs.m_taunt and
			lhs.m_is_tormented == rhs.m_is_tormented and
			lhs.m_toxic == rhs.m_toxic and
			lhs.m_uproar == rhs.m_uproar and
			lhs.m_vanish == rhs.m_vanish and
			lhs.m_water_sport == rhs.m_water_sport and
			lhs.m_yawn == rhs.m_yawn;
}

bool operator!= (ActivePokemon const & lhs, ActivePokemon const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
