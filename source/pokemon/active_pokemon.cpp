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
	return last_used_move.was_used_last(static_cast<LastUsedMove::index_type>(*technicalmachine::index(all_moves(), move_name)));
}


void ActivePokemon::reset_end_of_turn() {
	damage_done_to_active = 0;
	enduring = false;
	flinched_this_turn = false;
	has_moved = false;
	me_first = false;
	loaf = !loaf;
	protecting = false;
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
		m_magnet_rise = MagnetRise{};
		perish_song.reset();
		power_trick = false;
		m_stage = Stage{};
		m_substitute = Substitute{};
	}
	attracted = false;
	charged = false;
	used_defense_curl = false;
	destiny_bond = false;
	m_disable.reset();
	encore.reset();
	flash_fire = false;
	flinched_this_turn = false;
	m_fully_trapped = false;
	heal_block.reset();
	identified = false;
	used_imprison = false;
	last_used_move = LastUsedMove{};
	// Do I set to true or false? true makes it wrong when a fainted Pokemon is
	// replaced; false makes it wrong otherwise
	loaf = false;
	minimize = false;
	me_first = false;
	mud_sport = false;
	nightmares = false;
	partial_trap.reset();
	pass = false;
	rampage.reset();
	roosting = false;
	slow_start.reset();
	stockpile.reset();
	is_tormented = false;
	u_turning = false;
	water_sport = false;
	bide = Bide{};
	damage_done_to_active = 0;
	m_taunt.reset();
	toxic = Toxic{};
	uproar = Uproar{};
	// Whirlwind can hit Flying Pokemon, so it's possible to switch while
	// vanished. Therefore, we need to reset it.
	vanish = Vanish{};
	yawn = Yawn{};
	m_will_be_replaced = false;
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

Rational ActivePokemon::awaken_probability() const {
	auto const & status = get_status(*this);
	auto const & ability = get_ability(*this);
	return status.awaken_probability(ability, awakening);
}

bool ActivePokemon::is_baton_passing() const {
	return pass;
}

void ActivePokemon::baton_pass() {
	pass = true;
}

bool ActivePokemon::cannot_be_koed() const {
	return enduring;
}

bool ActivePokemon::charge_boosted() const {
	return charged and get_type(move(), *this) == Type::Electric;
}

void ActivePokemon::charge() {
	charged = true;
}

bool ActivePokemon::is_confused() const {
	return confusion.is_active();
}

void ActivePokemon::confuse() {
	if (!get_ability(*this).blocks_confusion())
		confusion.activate();
}

void ActivePokemon::handle_confusion() {
	confusion.do_turn(*this);
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

bool ActivePokemon::is_disabled(Moves const move_name) const {
	return m_disable.move_is_disabled(static_cast<uint8_t>(*technicalmachine::index(all_moves(), move_name)));
}

void ActivePokemon::disable() {
	m_disable.activate(static_cast<uint8_t>(all_moves().index()));
}

void ActivePokemon::advance_disable() {
	m_disable.advance_one_turn();
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

bool ActivePokemon::is_fainted() const {
	return m_will_be_replaced;
}

void ActivePokemon::faint() {
	get_hp(*this) = 0_bi;
	m_will_be_replaced = true;
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

auto ActivePokemon::has_focused_energy() const -> bool {
	return focusing_energy;
}

void ActivePokemon::focus_energy() {
	focusing_energy = true;
}

void ActivePokemon::fully_trap() {
	m_fully_trapped = true;
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

PokemonCollection::index_type ActivePokemon::index() const {
	return all_pokemon().index();
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

bool ActivePokemon::is_loafing() const {
	return get_ability(*this).is_loafing(loaf);
}

void ActivePokemon::decrement_lock_in() {
	// Cannot be locked into Rampage and Uproar at the same time
	if (rampage.decrement())
		confuse();
	else
		uproar.increment();
}

bool ActivePokemon::locked_on() const {
	return lock_on;
}

void ActivePokemon::lock_on_to() {
	lock_on = true;
}

namespace {

bool is_regular(Moves const move) {
	return move != Moves::Struggle and !is_switch(move);
}

}	// namespace

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

void ActivePokemon::decrement_magnet_rise() {
	m_magnet_rise.decrement();
}

bool ActivePokemon::me_first_is_active() const {
	return me_first;
}

bounded::integer<10, 160> ActivePokemon::fury_cutter_power() const {
	return last_used_move.fury_cutter_power();
}
bounded::integer<30, 480> ActivePokemon::momentum_move_power() const {
	return last_used_move.momentum_move_power();
}
bounded::integer<0, 30> ActivePokemon::triple_kick_power() const {
	return last_used_move.triple_kick_power();
}

bounded_rational<bounded::integer<10, 20>, bounded::integer<10, 10>> ActivePokemon::metronome_boost() const {
	return last_used_move.metronome_boost();
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

void ActivePokemon::set_moved(bool const value) {
	has_moved = value;
}

bool ActivePokemon::moved() const {
	return has_moved;
}

bool ActivePokemon::moved_since_switch() const {
	return last_used_move.has_moved();
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

void ActivePokemon::partial_trap_damage() {
	partial_trap.damage(*this);
}

void ActivePokemon::activate_perish_song() {
	perish_song.activate();
}

void ActivePokemon::perish_song_turn() {
	bool const faints_this_turn = perish_song.next_turn();
	if (faints_this_turn) {
		faint();
	}
}

bool ActivePokemon::can_be_phazed() const {
	return !ingrained() and !get_ability(*this).blocks_phazing() and all_pokemon().size() > 1;
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

void ActivePokemon::activate_rampage() {
	rampage.activate();
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

bool ActivePokemon::is_roosting() const {
	return roosting;
}

void ActivePokemon::roost() {
	roosting = true;
}

bool ActivePokemon::shed_skin_activated() const {
	return shedding_skin;
}

void ActivePokemon::shed_skin(bool const value) {
	shedding_skin = value;
}

Rational ActivePokemon::shed_skin_probability() const {
	auto const & status = get_status(*this);
	if (!get_ability(*this).can_clear_status(status)) {
		return Rational(shed_skin_activated() ? 0 : 1, 1);
	}
	Rational const result(3, 10);
	return shed_skin_activated() ? result : complement(result);
}

void ActivePokemon::increase_sleep_counter() {
	auto & status = get_status(*this);
	auto const & ability = get_ability(*this);
	status.increase_sleep_counter(ability, awakening);
}

bool ActivePokemon::slow_start_is_active() const {
	return slow_start.is_active();
}

bool ActivePokemon::sport_is_active(Move const & foe_move) const {
	switch (get_type(foe_move, *this)) {
	case Type::Electric:
		return mud_sport;
	case Type::Fire:
		return water_sport;
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

bounded::integer<0, Stockpile::max * 100> ActivePokemon::spit_up_power() const {
	return stockpile.spit_up_power();
}

void ActivePokemon::increment_stockpile() {
	bool const increased = stockpile.increment();
	if (increased) {
		boost_defensive(stage(), 1_bi);
	}
}

bounded::integer<0, Stockpile::max> ActivePokemon::release_stockpile() {
	auto const stages = stockpile.release();
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
	return pass or u_turning or will_be_replaced();
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
	return m_fully_trapped or ingrained() or partial_trap;
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

void ActivePokemon::increment_toxic() {
	toxic.increment();
}

void ActivePokemon::u_turn() {
	u_turning = true;
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

auto ActivePokemon::hit_with_yawn() -> void {
	yawn.activate();
}

auto ActivePokemon::try_to_activate_yawn() -> bool {
	return yawn.advance_turn();
}

bool ActivePokemon::bounce() {
	return vanish.bounce();
}
bool ActivePokemon::dig() {
	return vanish.dig();
}
bool ActivePokemon::dive() {
	return vanish.dive();
}
bool ActivePokemon::fly() {
	return vanish.fly();
}
bool ActivePokemon::shadow_force() {
	return vanish.shadow_force();
}

void ActivePokemon::use_bide(Pokemon & target) {
	if (!bide.is_active()) {
		bide.activate();
	}
	else {
		auto const bide_damage = bide.decrement();
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
	return bide.is_active();
}

bounded::integer<0, HP::max_value> ActivePokemon::damaged() const {
	return damage_done_to_active;
}

Rational ActivePokemon::random_damage_multiplier() const {
	return random_damage();
}

void ActivePokemon::direct_damage(damage_type const damage) {
	auto & hp = get_hp(*this);
	auto const initial_hp = hp.current();
	hp -= damage;
	damage_done_to_active = initial_hp - hp.current();
	bide.add_damage(damage);
}

void ActivePokemon::indirect_damage(damage_type const damage) {
	get_hp(*this) -= damage;
}

void ActivePokemon::register_damage(damage_type const damage) {
	damage_done_to_active = damage;
}

void ActivePokemon::increment_move_use_counter() {
	if (is_regular(move())) {
		last_used_move.increment(static_cast<LastUsedMove::index_type>(all_moves().index()));
	}
	else {
		last_used_move = LastUsedMove{};
	}
}

void ActivePokemon::update_chance_to_hit(ActivePokemon const & target, Weather const & weather, bool target_moved) {
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

ActivePokemon::hash_type ActivePokemon::hash() const {
	hash_type current_hash = 0;
	current_hash *= technicalmachine::max_hash(substitute());
	current_hash += technicalmachine::hash(substitute());
	current_hash *= bide.max_hash();
	current_hash += bide.hash();
	current_hash *= confusion.max_hash();
	current_hash += confusion.hash();
	current_hash *= m_disable.max_hash();
	current_hash += m_disable.hash();
	current_hash *= embargo.max_hash();
	current_hash += embargo.hash();
	current_hash *= encore.max_hash();
	current_hash += encore.hash();
	current_hash *= heal_block.max_hash();
	current_hash += heal_block.hash();
	current_hash *= technicalmachine::max_hash(last_used_move);
	current_hash += technicalmachine::hash(last_used_move);
	current_hash *= magnet_rise().max_hash();
	current_hash += magnet_rise().hash();
	current_hash *= partial_trap.max_hash();
	current_hash += partial_trap.hash();
	current_hash *= perish_song.max_hash();
	current_hash += perish_song.hash();
	current_hash *= rampage.max_hash();
	current_hash += rampage.hash();
	current_hash *= slow_start.max_hash();
	current_hash += slow_start.hash();
	current_hash *= technicalmachine::max_hash(stage());
	current_hash += technicalmachine::hash(stage());
	current_hash *= stockpile.max_hash();
	current_hash += stockpile.hash();
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
	current_hash += has_focused_energy();
	current_hash *= 2;
	current_hash += fully_trapped();
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
	hash_type current_hash = 0;
	current_hash *= technicalmachine::max_hash(substitute());
	current_hash *= bide.max_hash();
	current_hash *= confusion.max_hash();
	current_hash *= m_disable.max_hash();
	current_hash *= embargo.max_hash();
	current_hash *= encore.max_hash();
	current_hash *= heal_block.max_hash();
	current_hash *= technicalmachine::max_hash(last_used_move);
	current_hash *= magnet_rise().max_hash();
	current_hash *= partial_trap.max_hash();
	current_hash *= perish_song.max_hash();
	current_hash *= rampage.max_hash();
	current_hash *= slow_start.max_hash();
	current_hash *= technicalmachine::max_hash(stage());
	current_hash *= stockpile.max_hash();
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
			lhs.m_all_pokemon == rhs.m_all_pokemon and
			lhs.aqua_ring == rhs.aqua_ring and
			lhs.attracted == rhs.attracted and
			lhs.bide == rhs.bide and
			lhs.charged == rhs.charged and
			lhs.confusion == rhs.confusion and
			lhs.cursed == rhs.cursed and
			lhs.used_defense_curl == rhs.used_defense_curl and
			lhs.destiny_bond == rhs.destiny_bond and
			lhs.m_disable == rhs.m_disable and
			lhs.embargo == rhs.embargo and
			lhs.encore == rhs.encore and
			lhs.flash_fire == rhs.flash_fire and
			lhs.has_focused_energy() == rhs.has_focused_energy() and
			lhs.fully_trapped() == rhs.fully_trapped() and
			lhs.heal_block == rhs.heal_block and
			lhs.identified == rhs.identified and
			lhs.used_imprison == rhs.used_imprison and
			lhs.ingrain_active == rhs.ingrain_active and
			lhs.last_used_move == rhs.last_used_move and
			lhs.leech_seed == rhs.leech_seed and
			lhs.loaf == rhs.loaf and
			lhs.lock_on == rhs.lock_on and
			lhs.magnet_rise() == rhs.magnet_rise() and
			lhs.minimize == rhs.minimize and
			lhs.mud_sport == rhs.mud_sport and
			lhs.nightmares == rhs.nightmares and
			lhs.partial_trap == rhs.partial_trap and
			lhs.perish_song == rhs.perish_song and
			lhs.rampage == rhs.rampage and
			lhs.slow_start == rhs.slow_start and
			lhs.stage() == rhs.stage() and
			lhs.stockpile == rhs.stockpile and
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
