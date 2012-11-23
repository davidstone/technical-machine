// Flags for the active Pokemon
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

#include "pokemon.hpp"

#include "../ability.hpp"
#include "../damage.hpp"
#include "../rational.hpp"

#include "../move/move.hpp"

namespace technicalmachine {

ActivePokemon::ActivePokemon(PokemonCollection & all):
	all_pokemon(&all)
	{
}

void ActivePokemon::update_collection(PokemonCollection & all) {
	all_pokemon = &all;
}

ActivePokemon::operator Pokemon const & () const {
	return get_pokemon();
}

ActivePokemon::operator Pokemon & () {
	return get_pokemon();
}

MoveCollection const & ActivePokemon::all_moves() const {
	return get_pokemon().move;
}

MoveCollection & ActivePokemon::all_moves() {
	return get_pokemon().move;
}

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
		stage.reset();
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
	rampage.reset();
	roosting = false;
	slow_start.reset();
	stockpile.reset();
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
	get_pokemon().move.for_each_regular_move([](Move & move) {
		move.reset();
	});
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

Ability const & ActivePokemon::ability() const {
	return get_pokemon().ability();
}

void ActivePokemon::ability(Ability::Abilities const update) {
	get_pokemon().ability().name = update;
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

bool ActivePokemon::can_awaken() const {
	return status().can_awaken(ability());
}

Rational ActivePokemon::awaken_probability() const {
	return status().awaken_probability(ability(), awakening);
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
	return charged and move().type() == Type::Electric;
}

void ActivePokemon::charge() {
	charged = true;
}

bool ActivePokemon::can_confuse_with_chatter() const {
	return get_pokemon().can_confuse_with_chatter();
}

bool ActivePokemon::is_confused() const {
	return confusion.is_active();
}

void ActivePokemon::confuse() {
	if (!get_pokemon().ability().blocks_confusion())
		confusion.activate();
}

void ActivePokemon::handle_confusion() {
	confusion.do_turn(get_pokemon());
}

bool ActivePokemon::critical_hit() const {
	return ch;
}

void ActivePokemon::set_critical_hit(bool const value) {
	ch = value;
}

Rational ActivePokemon::critical_probability() const {
	constexpr unsigned ch_denominator = 16;
	return move().can_critical_hit() ?
		Rational(critical_hit() ? 1 : ch_denominator - 1, ch_denominator) :
		Rational(critical_hit() ? 0 : 1, 1);
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

void ActivePokemon::faint() {
	get_pokemon().hp().stat = 0;
	get_pokemon().faint();
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

PokemonCollection::index_type ActivePokemon::index() const {
	return all_pokemon->index();
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

Item const & ActivePokemon::item() const {
	return get_pokemon().item();
}

Item & ActivePokemon::item() {
	return get_pokemon().item();
}

bool ActivePokemon::is_fully_paralyzed() const {
	return fully_paralyzed;
}

Gender const & ActivePokemon::gender() const {
	return get_pokemon().gender();
}

bool ActivePokemon::leech_seeded() const {
	return leech_seed;
}

void ActivePokemon::hit_with_leech_seed() {
	leech_seed = true;
}

unsigned ActivePokemon::level() const {
	return get_pokemon().level();
}

bool ActivePokemon::is_loafing() const {
	return ability().is_loafing(loaf);
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

void ActivePokemon::lower_pp(Ability const & target) {
	if (move().is_regular() and !is_locked_in_to_bide())
		regular_move().pp.decrement(target);
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

Move & ActivePokemon::move() {
	return get_pokemon().move();
}

void ActivePokemon::set_moved(bool const value) {
	has_moved = value;
}

bool ActivePokemon::moved() const {
	return has_moved;
}

bool ActivePokemon::moved_since_switch() const {
	return get_pokemon().move.moved_since_switch();
}

void ActivePokemon::activate_mud_sport() {
	mud_sport = true;
}

Species ActivePokemon::name() const {
	return get_pokemon().name();
}

Nature const & ActivePokemon::nature() const {
	return get_pokemon().nature();
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
	partial_trap.damage(get_pokemon());
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
	return !ingrained() and !ability().blocks_phazing() and all_pokemon->size() > 1;
}

unsigned ActivePokemon::power_of_mass_based_moves() const {
	return get_pokemon().power_of_mass_based_moves();
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

Rational ActivePokemon::shed_skin_probability() const {
	if (!can_clear_status()) {
		return Rational(shed_skin_activated() ? 0 : 1, 1);
	}
	Rational const result(3, 10);
	return shed_skin_activated() ? result : complement(result);
}

void ActivePokemon::increase_sleep_counter() {
	status().increase_sleep_counter(ability(), awakening);
}

bool ActivePokemon::slow_start_is_active() const {
	return slow_start.is_active();
}

bool ActivePokemon::sport_is_active(Move const & foe_move) const {
	if (foe_move.type() == Type::Electric)
		return mud_sport;
	else if (foe_move.type() == Type::Fire)
		return water_sport;
	else
		return false;
}

Stat const & ActivePokemon::hp() const {
	return get_pokemon().hp();
}
Stat & ActivePokemon::hp() {
	return get_pokemon().hp();
}

Stat const & ActivePokemon::atk() const {
	return get_pokemon().atk();
}
Stat & ActivePokemon::atk() {
	return get_pokemon().atk();
}

Stat const & ActivePokemon::def() const {
	return get_pokemon().def();
}
Stat & ActivePokemon::def() {
	return get_pokemon().def();
}

Stat const & ActivePokemon::spa() const {
	return get_pokemon().spa();
}
Stat & ActivePokemon::spa() {
	return get_pokemon().spa();
}

Stat const & ActivePokemon::spd() const {
	return get_pokemon().spd();
}
Stat & ActivePokemon::spd() {
	return get_pokemon().spd();
}

Stat const & ActivePokemon::spe() const {
	return get_pokemon().spe();
}
Stat & ActivePokemon::spe() {
	return get_pokemon().spe();
}

int ActivePokemon::current_stage(Stat::Stats const stat) const {
	return stage[stat];
}

unsigned ActivePokemon::positive_stat_boosts() const {
	auto const positive_values = [](int const stage) { return static_cast<unsigned>(std::max(stage, 0)); };
	return stage.accumulate(positive_values);
}

void ActivePokemon::stat_boost(Stat::Stats const stat, int const n) {
	stage.boost(stat, n);
}

void ActivePokemon::stat_boost_physical(int n) {
	stage.boost_physical(n);
}

void ActivePokemon::stat_boost_special(int n) {
	stage.boost_special(n);
}

void ActivePokemon::stat_boost_regular(int n) {
	stage.boost_regular(n);
}

void ActivePokemon::stat_boost_defensive(int const n) {
	stage.boost_defensive(n);
}

void ActivePokemon::stat_boost_offensive(int const n) {
	stage.boost_offensive(n);
}

void ActivePokemon::reset_stats() {
	stage.reset();
}

void ActivePokemon::copy_stat_boosts(ActivePokemon const & other) {
	stage = other.stage;
}

void ActivePokemon::swap_defensive_stages(ActivePokemon & lhs, ActivePokemon & rhs) {
	Stage::swap_defensive(lhs.stage, rhs.stage);
}

void ActivePokemon::swap_offensive_stages(ActivePokemon & lhs, ActivePokemon & rhs) {
	Stage::swap_offensive(lhs.stage, rhs.stage);
}

void ActivePokemon::swap_stat_boosts(ActivePokemon & lhs, ActivePokemon & rhs) {
	using std::swap;
	swap(lhs.stage, rhs.stage);
}

Status const & ActivePokemon::status() const {
	return get_pokemon().status();
}

Status & ActivePokemon::status() {
	return get_pokemon().status();
}

bool ActivePokemon::can_clear_status() const {
	return ability().can_clear_status(status());
}

unsigned ActivePokemon::spit_up_power() const {
	return stockpile.spit_up_power();
}

void ActivePokemon::increment_stockpile() {
	bool const increased = stockpile.increment();
	if (increased)
		stat_boost_defensive(1);
}

int ActivePokemon::release_stockpile() {
	int const stages = stockpile.release();
	stat_boost_defensive(-stages);
	return stages;
}

bool ActivePokemon::is_switching_to_self () const {
	return all_pokemon->is_switching_to_self();
}

bool ActivePokemon::is_switching_to_self (Move const & switch_move) const {
	return all_pokemon->is_switching_to_self(switch_move);
}

bool ActivePokemon::has_switched() const {
	return moved() and move().is_switch();
}

bool ActivePokemon::switch_decision_required() const {
	return pass or u_turning;
}

void ActivePokemon::switch_pokemon() {
	get_pokemon().switch_out();
	all_pokemon->to_replacement();
}

void ActivePokemon::switch_in() {
	get_pokemon().switch_in();
}

void ActivePokemon::update_to_correct_switch() {
	get_pokemon().move.set_index(Move::from_replacement(all_pokemon->replacement()));
}

bool ActivePokemon::trapped() const {
	return fully_trapped or ingrained() or partial_trap;
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

TypeCollection const & ActivePokemon::type() const {
	return get_pokemon().type();
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
			target.apply_damage(bide_damage * 2);
	}
}

void ActivePokemon::use_substitute() {
	if (!get_pokemon().can_use_substitute())
		return;
	bool const created = active_substitute.create(hp().max);
	if (created) {
		indirect_damage(hp().max / 4);
	}
}

bool ActivePokemon::is_locked_in_to_bide() const {
	return bide.is_active();
}

unsigned ActivePokemon::damaged() const {
	return damage_done_to_active;
}

void ActivePokemon::direct_damage(unsigned damage) {
	damage = get_pokemon().apply_damage(damage);
	damage_done_to_active = damage;
	bide.add_damage(damage);
}

void ActivePokemon::indirect_damage(unsigned const damage) {
	get_pokemon().apply_damage(damage);
}

void ActivePokemon::register_damage(unsigned const damage) {
	damage_done_to_active = damage;
}

void ActivePokemon::update_chance_to_hit(ActivePokemon const & target, Weather const & weather, bool target_moved) {
	cached_chance_to_hit.update(*this, target, weather, target_moved);
}

ChanceToHit::value_type ActivePokemon::accuracy_probability() const {
	return miss ? cached_chance_to_hit.inverse() : cached_chance_to_hit();
}

bool ActivePokemon::can_miss() const {
	return cached_chance_to_hit.can_miss();
}

bool ActivePokemon::will_be_replaced() const {
	return get_pokemon().will_be_replaced();
}

void ActivePokemon::normalize_hp() {
	get_pokemon().normalize_hp();
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
	current_hash *= rampage.max_hash();
	current_hash += rampage.hash();
	current_hash *= slow_start.max_hash();
	current_hash += slow_start.hash();
	current_hash *= stage.max_hash();
	current_hash += stage.hash();
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
	current_hash *= rampage.max_hash();
	current_hash *= slow_start.max_hash();
	current_hash *= stage.max_hash();
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
			lhs.all_pokemon->index() == rhs.all_pokemon->index() and
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
			lhs.rampage == rhs.rampage and
			lhs.slow_start == rhs.slow_start and
			lhs.stage == rhs.stage and
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

std::string ActivePokemon::to_string() const {
	return get_pokemon().to_string();
}

}	// namespace technicalmachine
