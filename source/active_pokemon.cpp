// Load teams
// Copyright (C) 2012 David Stone
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

#include "ability.hpp"
#include "damage.hpp"
#include "rational.hpp"

#include "move/move.hpp"

#include "pokemon/pokemon.hpp"

namespace technicalmachine {

void ActivePokemon::reset_end_of_turn() {
	damage_done_to_active = 0;
	enduring = false;
	flinched_this_turn = false;
	has_moved = false;
	me_first = false;
	loaf = !loaf;
	protecting = false;
	is_replacing = false;
}

void ActivePokemon::reset_switch() {
	// TODO: remove some of these when the foe switches, too
	if (!is_baton_passing()) {
		aqua_ring = false;
		confusion.reset();
		cursed = false;
		embargo.reset();
		focusing_energy = false;
		gastro_acid = false;
		ingrain_active = false;
		leech_seed = false;
		lock_on = false;
		magnet_rise.reset();
		perish_song.reset();
		power_trick = false;
		active_substitute.destroy();
	}
	attracted = false;
	charged = false;
	used_defense_curl = false;
	destiny_bond = false;
	encore.reset();
	flash_fire = false;
	flinched_this_turn = false;
	fully_trapped = false;
	heal_block.reset();
	identified = false;
	used_imprison = false;
	// Do I set to true or false? true makes it wrong when a fainted Pokemon is
	// replaced; false makes it wrong otherwise
	loaf = false;
	minimize = false;
	me_first = false;
	mud_sport = false;
	nightmares = false;
	partial_trap.reset();
	pass = false;
	roosting = false;
	is_tormented = false;
	u_turning = false;
	water_sport = false;
	bide.reset();
	damage_done_to_active = 0;
	m_taunt.reset();
	toxic.reset();
	uproar.reset();
	// Whirlwind can hit Flying Pokemon, so it's possible to switch while
	// vanished. Therefore, we need to reset it.
	vanish.reset();
	yawn.reset();
}

void ActivePokemon::reset_between_turns() {
	ch = false;
	fully_paralyzed = false;
	confusion.end_of_turn_reset();
	miss = false;
}

void ActivePokemon::clear_field() {
	clear_leech_seed();
	partial_trap.reset();
}

void ActivePokemon::update_before_move() {
	destiny_bond = false;
	lock_on = false;
	has_moved = true;
}

void ActivePokemon::clear_leech_seed() {
	leech_seed = false;
}

bool ActivePokemon::aqua_ring_is_active() const {
	return aqua_ring;
}

void ActivePokemon::activate_aqua_ring() {
	aqua_ring = true;
}

void ActivePokemon::attract() {
	attracted = true;
}

void ActivePokemon::awaken(bool const value) {
	awakening = value;
}

bool ActivePokemon::is_baton_passing() const {
	return pass;
}

void ActivePokemon::baton_pass() {
	pass = true;
}

void ActivePokemon::bounce() {
	vanish.bounce();
}

bool ActivePokemon::cannot_be_koed() const {
	return enduring;
}

bool ActivePokemon::charge_boosted() const {
	return charged;
}

void ActivePokemon::charge() {
	charged = true;
}

bool ActivePokemon::is_confused() const {
	return confusion.is_active();
}

void ActivePokemon::confuse() {
	confusion.activate();
}

void ActivePokemon::handle_confusion(Pokemon & pokemon) {
	confusion.do_turn(pokemon);
}

bool ActivePokemon::critical_hit() const {
	return ch;
}

void ActivePokemon::set_critical_hit(bool const value) {
	ch = value;
}

void ActivePokemon::curse() {
	cursed = true;
}

bool ActivePokemon::is_cursed() const {
	return cursed;
}

void ActivePokemon::defense_curl() {
	used_defense_curl = true;
}

void ActivePokemon::use_destiny_bond() {
	destiny_bond = true;
}

bool ActivePokemon::defense_curled() const {
	return used_defense_curl;
}

void ActivePokemon::dig() {
	vanish.dig();
}

void ActivePokemon::dive() {
	vanish.dive();
}

void ActivePokemon::activate_embargo() {
	embargo.activate();
}

void ActivePokemon::decrement_embargo() {
	embargo.decrement();
}

bool ActivePokemon::is_encored() const {
	return encore.is_active();
}

void ActivePokemon::activate_encore() {
	encore.activate();
}

void ActivePokemon::increment_encore() {
	encore.increment();
}

void ActivePokemon::endure() {
	enduring = true;
}

bool ActivePokemon::flash_fire_is_active() const {
	return flash_fire;
}

void ActivePokemon::activate_flash_fire() {
	flash_fire = true;
}

bool ActivePokemon::flinched() const {
	return flinched_this_turn;
}

void ActivePokemon::flinch() {
	flinched_this_turn = true;
}

void ActivePokemon::fly() {
	vanish.fly();
}

void ActivePokemon::focus_energy() {
	focusing_energy = true;
}

void ActivePokemon::fully_trap() {
	fully_trapped = true;
}

void ActivePokemon::identify() {
	identified = true;
}

bool ActivePokemon::imprisoned() const {
	return used_imprison;
}

void ActivePokemon::imprison() {
	used_imprison = true;
}

bool ActivePokemon::ingrained() const {
	return ingrain_active;
}

void ActivePokemon::ingrain() {
	ingrain_active = true;
}

bool ActivePokemon::heal_block_is_active() const {
	return heal_block.is_active();
}

void ActivePokemon::activate_heal_block() {
	heal_block.activate();
}

void ActivePokemon::decrement_heal_block() {
	heal_block.decrement();
}

bool ActivePokemon::is_fully_paralyzed() const {
	return fully_paralyzed;
}

bool ActivePokemon::leech_seeded() const {
	return leech_seed;
}

void ActivePokemon::hit_with_leech_seed() {
	leech_seed = true;
}

bool ActivePokemon::is_loafing(Ability const & ability) const {
	return ability.is_loafing(loaf);
}

bool ActivePokemon::locked_on() const {
	return lock_on;
}

void ActivePokemon::lock_on_to() {
	lock_on = true;
}

bool ActivePokemon::magnet_rise_is_active() const {
	return magnet_rise.is_active();
}

void ActivePokemon::activate_magnet_rise() {
	magnet_rise.activate();
}

void ActivePokemon::decrement_magnet_rise() {
	magnet_rise.decrement();
}

bool ActivePokemon::me_first_is_active() const {
	return me_first;
}

bool ActivePokemon::minimized() const {
	return minimize;
}

bool ActivePokemon::missed() const {
	return miss;
}

void ActivePokemon::set_miss(bool const value) {
	miss = value;
}

void ActivePokemon::move(bool const value) {
	has_moved = value;
}

bool ActivePokemon::moved() const {
	return has_moved;
}

void ActivePokemon::activate_mud_sport() {
	mud_sport = true;
}

bool ActivePokemon::nightmare() const {
	return nightmares;
}

void ActivePokemon::give_nightmares() {
	nightmares = true;
}

void ActivePokemon::partially_trap(bool const extended) {
	partial_trap.activate(extended);
}

void ActivePokemon::partial_trap_damage(Pokemon & pokemon) {
	partial_trap.damage(pokemon);
}

void ActivePokemon::activate_perish_song() {
	perish_song.activate();
}

bool ActivePokemon::perish_song_turn() {
	return perish_song.next_turn();
}

bool ActivePokemon::power_trick_is_active() const {
	return power_trick;
}

void ActivePokemon::activate_power_trick() {
	power_trick = !power_trick;
}

void ActivePokemon::protect() {
	protecting = true;
}

void ActivePokemon::break_protect() {
	protecting = false;
}

bool ActivePokemon::recharging() const {
	return recharge_lock_in;
}

bool ActivePokemon::recharge() {
	bool const return_value = recharging();
	recharge_lock_in = false;
	return return_value;
}

void ActivePokemon::use_recharge_move() {
	recharge_lock_in = true;
}

bool ActivePokemon::replacing() const {
	return is_replacing;
}

void ActivePokemon::not_replacing() {
	is_replacing = false;
}

bool ActivePokemon::is_roosting() const {
	return roosting;
}

void ActivePokemon::roost() {
	roosting = true;
}

void ActivePokemon::shadow_force() {
	vanish.shadow_force();
}

bool ActivePokemon::shed_skin_activated() const {
	return shedding_skin;
}

void ActivePokemon::shed_skin(bool const value) {
	shedding_skin = value;
}

void ActivePokemon::increase_sleep_counter(Pokemon & pokemon) {
	pokemon.status.increase_sleep_counter(pokemon.ability, awakening);
}

bool ActivePokemon::sport_is_active(Move const & foe_move) const {
	Type const & type = foe_move.type();
	if (type == Type::ELECTRIC)
		return mud_sport;
	else if (type == Type::FIRE)
		return water_sport;
	else
		return false;
}

bool ActivePokemon::switch_decision_required() const {
	return pass or u_turning;
}

bool ActivePokemon::trapped() const {
	return !fully_trapped and !ingrained()  and !partial_trap;
}

bool ActivePokemon::tormented() const {
	return is_tormented;
}

void ActivePokemon::torment() {
	is_tormented = true;
}

void ActivePokemon::taunt() {
	m_taunt.activate();
}

bool ActivePokemon::is_taunted() const {
	return m_taunt.is_active();
}

void ActivePokemon::increment_taunt() {
	m_taunt.increment();
}

Rational ActivePokemon::toxic_ratio() const {
	return toxic.ratio_drained();
}

void ActivePokemon::increment_toxic() {
	toxic.increment();
}

void ActivePokemon::u_turn() {
	u_turning = true;
}

void ActivePokemon::increment_uproar() {
	uproar.increment();
}

void ActivePokemon::use_uproar() {
	uproar.increment();
}

bool ActivePokemon::vanish_doubles_power(Moves const move_name) const {
	return vanish.doubles_move_power(move_name);
}

void ActivePokemon::activate_water_sport() {
	water_sport = true;
}

bool ActivePokemon::decrement_yawn() {
	return yawn.decrement();
}

void ActivePokemon::hit_with_yawn() {
	yawn.activate();
}

void ActivePokemon::use_bide(Pokemon & target) {
	if (!bide.is_active()) {
		bide.activate();
	}
	else {
		unsigned const bide_damage = bide.decrement();
		if (bide_damage != 0)
			damage_side_effect(target, bide_damage * 2);
	}
}

bool ActivePokemon::substitute(unsigned const max_hp) {
	return active_substitute.create(max_hp);
}

bool ActivePokemon::is_locked_in_to_bide() const {
	return bide.is_active();
}

unsigned ActivePokemon::damaged() const {
	return damage_done_to_active;
}

void ActivePokemon::do_damage(unsigned const damage) {
	damage_done_to_active = damage;
	bide.add_damage(damage);
}

void ActivePokemon::update_chance_to_hit(Team const & user, Team const & target, Weather const & weather, bool const target_moved) {
	cached_chance_to_hit.update(user, target, weather, target_moved);
}

ChanceToHit::value_type ActivePokemon::chance_to_hit() const {
	return cached_chance_to_hit();
}

ChanceToHit::value_type ActivePokemon::chance_to_miss() const {
	return cached_chance_to_hit.inverse();
}

bool ActivePokemon::can_miss() const {
	return cached_chance_to_hit.can_miss();
}

ActivePokemon::hash_type ActivePokemon::hash() const {
	hash_type current_hash = 0;
	current_hash *= active_substitute.max_hash();
	current_hash += active_substitute.hash();
	current_hash *= bide.max_hash();
	current_hash += bide.hash();
	current_hash *= confusion.max_hash();
	current_hash += confusion.hash();
	current_hash *= embargo.max_hash();
	current_hash += embargo.hash();
	current_hash *= encore.max_hash();
	current_hash += encore.hash();
	current_hash *= heal_block.max_hash();
	current_hash += heal_block.hash();
	current_hash *= magnet_rise.max_hash();
	current_hash += magnet_rise.hash();
	current_hash *= partial_trap.max_hash();
	current_hash += partial_trap.hash();
	current_hash *= perish_song.max_hash();
	current_hash += perish_song.hash();
	current_hash *= m_taunt.max_hash();
	current_hash += m_taunt.hash();
	current_hash *= toxic.max_hash();
	current_hash += toxic.hash();
	current_hash *= uproar.max_hash();
	current_hash += uproar.hash();
	current_hash *= vanish.max_hash();
	current_hash += vanish.hash();
	current_hash *= yawn.max_hash();
	current_hash += yawn.hash();
	current_hash *= 2;
	current_hash += aqua_ring;
	current_hash *= 2;
	current_hash += attracted;
	current_hash *= 2;
	current_hash += charged;
	current_hash *= 2;
	current_hash += cursed;
	current_hash *= 2;
	current_hash += used_defense_curl;
	current_hash *= 2;
	current_hash += destiny_bond;
	current_hash *= 2;
	current_hash += flash_fire;
	current_hash *= 2;
	current_hash += focusing_energy;
	current_hash *= 2;
	current_hash += fully_trapped;
	current_hash *= 2;
	current_hash += gastro_acid;
	current_hash *= 2;
	current_hash += identified;
	current_hash *= 2;
	current_hash += used_imprison;
	current_hash *= 2;
	current_hash += ingrain_active;
	current_hash *= 2;
	current_hash += leech_seed;
	current_hash *= 2;
	current_hash += loaf;
	current_hash *= 2;
	current_hash += lock_on;
	current_hash *= 2;
	current_hash += minimize;
	current_hash *= 2;
	current_hash += mud_sport;
	current_hash *= 2;
	current_hash += nightmares;
	current_hash *= 2;
	current_hash += power_trick;
	current_hash *= 2;
	current_hash += recharge_lock_in;
	current_hash *= 2;
	current_hash += is_tormented;
	current_hash *= 2;
	current_hash += water_sport;
	return current_hash;
}

ActivePokemon::hash_type ActivePokemon::max_hash() const {
	hash_type current_hash = active_substitute.max_hash();
	current_hash *= bide.max_hash();
	current_hash *= confusion.max_hash();
	current_hash *= embargo.max_hash();
	current_hash *= encore.max_hash();
	current_hash *= heal_block.max_hash();
	current_hash *= magnet_rise.max_hash();
	current_hash *= partial_trap.max_hash();
	current_hash *= perish_song.max_hash();
	current_hash *= m_taunt.max_hash();
	current_hash *= toxic.max_hash();
	current_hash *= uproar.max_hash();
	current_hash *= vanish.max_hash();
	current_hash *= yawn.max_hash();
	current_hash *= 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	return current_hash;
}

bool operator== (ActivePokemon const & lhs, ActivePokemon const & rhs) {
	return 
			lhs.aqua_ring == rhs.aqua_ring and
			lhs.attracted == rhs.attracted and
			lhs.bide == rhs.bide and
			lhs.charged == rhs.charged and
			lhs.confusion == rhs.confusion and
			lhs.cursed == rhs.cursed and
			lhs.used_defense_curl == rhs.used_defense_curl and
			lhs.destiny_bond == rhs.destiny_bond and
			lhs.embargo == rhs.embargo and
			lhs.encore == rhs.encore and
			lhs.flash_fire == rhs.flash_fire and
			lhs.focusing_energy == rhs.focusing_energy and
			lhs.fully_trapped == rhs.fully_trapped and
			lhs.heal_block == rhs.heal_block and
			lhs.identified == rhs.identified and
			lhs.used_imprison == rhs.used_imprison and
			lhs.ingrain_active == rhs.ingrain_active and
			lhs.leech_seed == rhs.leech_seed and
			lhs.loaf == rhs.loaf and
			lhs.lock_on == rhs.lock_on and
			lhs.magnet_rise == rhs.magnet_rise and
			lhs.minimize == rhs.minimize and
			lhs.mud_sport == rhs.mud_sport and
			lhs.nightmares == rhs.nightmares and
			lhs.partial_trap == rhs.partial_trap and
			lhs.perish_song == rhs.perish_song and
			lhs.m_taunt == rhs.m_taunt and
			lhs.is_tormented == rhs.is_tormented and
			lhs.toxic == rhs.toxic and
			lhs.uproar == rhs.uproar and
			lhs.vanish == rhs.vanish and
			lhs.water_sport == rhs.water_sport and
			lhs.yawn == rhs.yawn;
}

bool operator!= (ActivePokemon const & lhs, ActivePokemon const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
