// Copyright (C) 2019 David Stone
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

#pragma once

#include <tm/pokemon/confusion.hpp>
#include <tm/pokemon/damage_blocker.hpp>
#include <tm/pokemon/disable.hpp>
#include <tm/pokemon/embargo.hpp>
#include <tm/pokemon/encore.hpp>
#include <tm/pokemon/heal_block.hpp>
#include <tm/pokemon/last_used_move.hpp>
#include <tm/pokemon/magnet_rise.hpp>
#include <tm/pokemon/partial_trap.hpp>
#include <tm/pokemon/perish_song.hpp>
#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/rampage.hpp>
#include <tm/pokemon/slow_start.hpp>
#include <tm/pokemon/stockpile.hpp>
#include <tm/pokemon/substitute.hpp>
#include <tm/pokemon/taunt.hpp>
#include <tm/pokemon/uproar.hpp>
#include <tm/pokemon/yawn.hpp>

#include <tm/bide/bide.hpp>

#include <tm/move/moves.hpp>
#include <tm/move/is_switch.hpp>

#include <tm/stat/stage.hpp>

#include <tm/type/pokemon_types.hpp>

#include <tm/generation.hpp>
#include <tm/operators.hpp>
#include <tm/random_damage.hpp>
#include <tm/rational.hpp>
#include <tm/weather.hpp>

#include <bounded/integer.hpp>
#include <bounded/detail/variant/variant.hpp>

#include <containers/algorithms/all_any_none.hpp>

#include <variant>

namespace technicalmachine {
struct ActivePokemon;
struct MutableActivePokemon;

// Various states a Pokemon can be in due to vanishing moves.
struct Bouncing {};
struct Digging {};
struct Diving {};
struct Flying {};
struct ShadowForcing {};

struct BatonPassing {};
struct ChargingUp {};
struct Recharging {};
struct UTurning {};

struct ActiveStatus {
	constexpr auto set(Statuses const status) & -> void {
		switch (status) {
			case Statuses::rest:
			case Statuses::sleep:
				m_nightmare = false;
				break;
			case Statuses::toxic:
				m_toxic_counter = 1_bi;
				break;
			default:
				break;
		}
	}
	constexpr auto give_nightmares() & -> void {
		m_nightmare = true;
	}

	auto end_of_turn(Generation, MutableActivePokemon pokemon, Pokemon const & other, bool uproar) & -> void;
private:
	// The discriminator is the status of the active Pokemon. The default value
	// is irrelevant.
	union {
		bool m_nightmare;
		bounded::clamped_integer<1, 15> m_toxic_counter;
	};
};

struct ActivePokemonFlags {
	auto reset_end_of_turn() -> void;
private:
	auto is_baton_passing() const -> bool;
	auto is_charging_up() const -> bool;
	auto is_locked_in_by_move() const -> bool;
	auto reset_switch() -> void;
	auto switch_decision_required(Pokemon const & pokemon) const -> bool;
	auto is_uproaring() const -> bool;
	auto vanish_doubles_power(Moves move_name) const -> bool;

	template<bool>
	friend struct ActivePokemonImpl;
	friend struct MutableActivePokemon;
	
	// TODO: Include other mutually exclusive stuff such as DamageBlocker
	// (Protect + Endure)?
	bounded::variant<
		std::monostate,
		Bouncing,
		Digging,
		Diving,
		Flying,
		ShadowForcing,
		BatonPassing,
		Bide,
		ChargingUp,
		Rampage,
		Recharging,
		UproarCounter,
		UTurning
	> lock_in{std::monostate{}};
	Confusion confusion;
	DamageBlocker damage_blocker;
	Disable disable;
	EmbargoCounter embargo;
	EncoreCounter encore;
	HealBlock heal_block;
	LastUsedMove last_used_move;
	MagnetRise magnet_rise;
	Substitute substitute;
	PartialTrap partial_trap;
	PerishSong perish_song;
	RandomDamage random_damage;
	Stage stage;
	ActiveStatus status;
	SlowStart slow_start;
	Stockpile stockpile;
	TauntCounter taunt;
	PokemonTypes types{Type::Typeless};
	YawnCounter yawn;
	HP::current_type direct_damage_received = 0_bi;
	bool aqua_ring = false;
	bool attracted = false;
	bool charged = false;
	bool is_cursed = false;
	bool damaged = false;
	bool defense_curled = false;
	bool destiny_bond = false;
	bool flash_fire = false;
	bool flinched = false;
	bool has_focused_energy = false;
	// Block, Mean Look, Spider Web
	bool fully_trapped = false;
	bool gastro_acid = false;
	bool identified = false;
	bool used_imprison = false;
	bool ingrained = false;
	bool leech_seeded = false;
	bool is_loafing_turn = false;
	bool locked_on = false;
	bool me_first_is_active = false;
	bool minimized = false;
	bool moved = false;
	bool mud_sport = false;
	bool power_trick_is_active = false;
	bool is_roosting = false;
	bool is_tormented = false;
	bool water_sport = false;
};

// TODO: Implement both ActivePokemon and MutableActivePokemon as typedefs once
// we get requires clauses.
template<bool is_const>
struct ActivePokemonImpl {
private:
	using PokemonRef = std::conditional_t<is_const, Pokemon const &, Pokemon &>;
	using FlagsRef = std::conditional_t<is_const, ActivePokemonFlags const &, ActivePokemonFlags &>;
public:
	ActivePokemonImpl(PokemonRef pokemon, FlagsRef flags):
		m_pokemon(pokemon),
		m_flags(flags)
	{
	}
	ActivePokemonImpl(ActivePokemonImpl const & other) = default;
	ActivePokemonImpl(ActivePokemonImpl && other) = default;
	ActivePokemonImpl & operator=(ActivePokemonImpl const & other) = delete;
	ActivePokemonImpl & operator=(ActivePokemonImpl && other) = delete;

	auto last_used_move() const -> LastUsedMove {
		return m_flags.last_used_move;
	}
	auto substitute() const -> Substitute const & {
		return m_flags.substitute;
	}

	auto has_switched() const -> bool;

	auto aqua_ring_is_active() const -> bool {
		return m_flags.aqua_ring;
	}

	auto is_baton_passing() const -> bool {
		return m_flags.is_baton_passing();
	}

	auto cannot_be_koed() const -> bool {
		return m_flags.damage_blocker.is_enduring();
	}

	auto charge_boosted(Type const move_type) const -> bool {
		return m_flags.charged and move_type == Type::Electric;
	}

	auto is_charging_up() const -> bool {
		return m_flags.is_charging_up();
	}

	auto is_confused() const -> bool {
		return m_flags.confusion.is_active();
	}

	auto is_cursed() const -> bool {
		return m_flags.is_cursed;
	}

	auto defense_curled() const -> bool {
		return m_flags.defense_curled;
	}

	auto damaged() const -> bool {
		return m_flags.damaged;
	}

	auto direct_damage_received() const -> HP::current_type {
		return m_flags.direct_damage_received;
	}

	// Requires that move is actually one of this Pokemon's moves
	auto is_disabled(Moves const move_name) const -> bool {
		return m_flags.disable.move_is_disabled(move_name);
	}

	auto is_encored() const -> bool {
		return m_flags.encore.is_active();
	}

	auto flash_fire_is_active() const -> bool {
		return m_flags.flash_fire;
	}

	auto flinched() const -> bool {
		return m_flags.flinched;
	}

	auto has_focused_energy() const -> bool {
		return m_flags.has_focused_energy;
	}

	auto used_imprison() const -> bool {
		return m_flags.used_imprison;
	}

	auto ingrained() const -> bool {
		return m_flags.ingrained;
	}

	auto heal_block_is_active() const -> bool {
		return m_flags.heal_block.is_active();
	}

	auto leech_seeded() const -> bool {
		return m_flags.leech_seeded;
	}

	auto is_loafing() const -> bool {
		return get_ability(m_pokemon) == Ability::Truant and m_flags.is_loafing_turn;
	}

	auto locked_on() const -> bool {
		return m_flags.locked_on;
	}

	auto magnet_rise() const -> MagnetRise const & {
		return m_flags.magnet_rise;
	}

	auto me_first_is_active() const -> bool {
		return m_flags.me_first_is_active;
	}

	auto minimized() const -> bool {
		return m_flags.minimized;
	}

	auto moved() const -> bool {
		return m_flags.moved;
	}

	auto power_trick_is_active() const -> bool {
		return m_flags.power_trick_is_active;
	}
	
	auto is_protecting() const {
		return m_flags.damage_blocker.is_protecting();
	}

	auto is_locked_in_by_move() const -> bool {
		return m_flags.is_locked_in_by_move();
	}

	auto is_roosting() const -> bool {
		return m_flags.is_roosting;
	}

	auto slow_start_is_active() const -> bool {
		return m_flags.slow_start.is_active();
	}

	auto sport_is_active(Type const move_type) const -> bool {
		switch (move_type) {
			case Type::Electric: return m_flags.mud_sport;
			case Type::Fire: return m_flags.water_sport;
			default: return false;
		}
	}

	auto stage() const -> Stage const & {
		return m_flags.stage;
	}

	auto spit_up_power() const {
		return m_flags.stockpile.spit_up_power();
	}

	auto switch_decision_required() const -> bool {
		return m_flags.switch_decision_required(m_pokemon);
	}

	auto fully_trapped() const -> bool {
		return m_flags.fully_trapped;
	}

	auto trapped() const -> bool {
		return m_flags.fully_trapped or ingrained() or m_flags.partial_trap.is_active();
	}

	auto is_tormented() const -> bool {
		return m_flags.is_tormented;
	}

	auto is_taunted() const -> bool {
		return m_flags.taunt.is_active();
	}

	auto types() const {
		return m_flags.types;
	}

	auto is_uproaring() const -> bool {
		return m_flags.is_uproaring();
	}

	auto vanish_doubles_power(Moves const move_name) const -> bool {
		return m_flags.vanish_doubles_power(move_name);
	}

	auto random_damage_multiplier() const {
		return m_flags.random_damage();
	}

	operator PokemonRef() const {
		return m_pokemon;
	}

protected:
	PokemonRef m_pokemon;
	FlagsRef m_flags;
};

extern template struct ActivePokemonImpl<true>;
extern template struct ActivePokemonImpl<false>;

auto apply_own_mental_herb(Generation const generation, MutableActivePokemon const pokemon) -> void;

// A reference to the currently active Pokemon
struct ActivePokemon : ActivePokemonImpl<true> {
	ActivePokemon(Pokemon const & pokemon, ActivePokemonFlags const & flags):
		ActivePokemonImpl(pokemon, flags)
	{
	}
};

inline auto is_type(ActivePokemon const pokemon, Type const type) -> bool {
	return
		(type != Type::Flying or !pokemon.is_roosting()) and
		containers::any_equal(pokemon.types(), type);
}

auto grounded(ActivePokemon, Weather) -> bool;

auto apply_status(Statuses status, MutableActivePokemon user, MutableActivePokemon target, Weather weather, bool uproar = false) -> void;
auto apply_status_to_self(Statuses status, MutableActivePokemon target, Weather weather, bool uproar = false) -> void;

auto activate_berserk_gene(MutableActivePokemon pokemon) -> void;
auto activate_pinch_item(Generation const generation, MutableActivePokemon pokemon) -> void;

// A mutable reference to the currently active Pokemon
struct MutableActivePokemon : ActivePokemonImpl<false> {
	MutableActivePokemon(Pokemon & pokemon, ActivePokemonFlags & flags):
		ActivePokemonImpl(pokemon, flags)
	{
	}
	
	operator ActivePokemon() const {
		return ActivePokemon(m_pokemon, m_flags);
	}

	auto stage() const -> Stage & {
		return m_flags.stage;
	}

	auto clear_field() const {
		m_flags.leech_seeded = false;
		m_flags.partial_trap = {};
	}
	auto update_before_move() const {
		m_flags.destiny_bond = false;
		m_flags.locked_on = false;
		m_flags.moved = true;
	}

	auto activate_aqua_ring() const {
		m_flags.aqua_ring = true;
	}
	auto attract(Generation, MutableActivePokemon other) const -> void;
	auto baton_pass() const -> void;
	auto charge() const {
		m_flags.charged = true;
	}
	auto use_charge_up_move() const -> void;
	auto confuse() const -> void {
		if (blocks_confusion(get_ability(*this))) {
			return;
		}
		if (clears_confusion(get_item(*this))) {
			set_item(*this, Item::None);
		} else {
			m_flags.confusion.activate();
		}
	}
	auto handle_confusion() const {
		m_flags.confusion.do_turn();
	}
	auto curse() const {
		m_flags.is_cursed = true;
	}
	auto defense_curl() const {
		m_flags.defense_curled = true;
	}
	auto use_destiny_bond() const {
		m_flags.destiny_bond = true;
	}
	auto disable(Generation const generation, Moves const move) const {
		m_flags.disable.activate(move);
		apply_own_mental_herb(generation, *this);
	}
	auto advance_disable() const {
		m_flags.disable.advance_one_turn();
	}
	auto activate_embargo() const {
		m_flags.embargo.activate();
	}
	auto advance_embargo() const {
		m_flags.embargo.advance_one_turn();
	}
	auto activate_encore(Generation const generation) const {
		m_flags.encore.activate();
		apply_own_mental_herb(generation, *this);
	}
	auto advance_encore() const {
		m_flags.encore.advance_one_turn();
	}
	auto endure() const {
		m_flags.damage_blocker.endure();
	}
	auto activate_flash_fire() const {
		m_flags.flash_fire = true;
	}
	auto flinch() const {
		m_flags.flinched = true;
	}
	auto focus_energy() const {
		m_flags.has_focused_energy = true;
	}
	auto fully_trap() const {
		m_flags.fully_trapped = true;
	}
	auto activate_heal_block() const {
		m_flags.heal_block.activate();
	}
	auto advance_heal_block() const {
		m_flags.heal_block.advance_one_turn();
	}
	auto identify() const {
		m_flags.identified = true;
	}
	auto use_imprison() const {
		m_flags.used_imprison = true;
	}
	auto ingrain() const {
		m_flags.ingrained = true;
	}
	auto hit_with_leech_seed() const {
		m_flags.leech_seeded = true;
	}
	auto advance_lock_in(bool const ending) const -> void;
	auto use_lock_on() const {
		m_flags.locked_on = true;
	}
	auto activate_magnet_rise() const {
		m_flags.magnet_rise.activate();
	}
	auto advance_magnet_rise() const {
		m_flags.magnet_rise.advance_one_turn();
	}
	friend auto apply_mental_herb(Generation const generation, MutableActivePokemon const pokemon) {
		pokemon.m_flags.attracted = false;
		if (generation >= Generation::five) {
			pokemon.m_flags.disable = {};
			pokemon.m_flags.encore = {};
			pokemon.m_flags.taunt = {};
			pokemon.m_flags.is_tormented = false;
		}
	}
	auto set_not_moved() const {
		m_flags.moved = false;
	}
	auto activate_mud_sport() const {
		m_flags.mud_sport = true;
	}
	auto try_to_give_nightmares() const {
		if (is_sleeping(get_status(m_pokemon))) {
			m_flags.status.give_nightmares();
		}
	}
	auto partially_trap() const {
		m_flags.partial_trap.activate();
	}
	auto partial_trap_damage(Generation const generation) const {
		m_flags.partial_trap.damage(generation, *this);
	}
	auto activate_perish_song() const {
		m_flags.perish_song.activate();
	}
	auto perish_song_turn() const -> void {
		bool const faints_this_turn = m_flags.perish_song.advance_one_turn();
		if (faints_this_turn) {
			m_pokemon.set_hp(0_bi);
		}
	}
	auto activate_power_trick() const {
		m_flags.power_trick_is_active = !m_flags.power_trick_is_active;
	}
	auto protect() const {
		m_flags.damage_blocker.protect();
	}
	auto break_protect() const {
		m_flags.damage_blocker.break_protect();
	}
	auto activate_rampage() const -> void;
	auto recharge() const -> bool;
	auto use_recharge_move() const -> void;
	auto roost() const {
		m_flags.is_roosting = true;
	}

	auto apply_status(Statuses status, MutableActivePokemon user, Weather weather, bool uproar = false) const -> void;
	void rest(bool const other_is_uproaring) const {
		if (other_is_uproaring or is_sleeping(get_status(m_pokemon))) {
			return;
		}
		auto const hp = get_hp(m_pokemon);
		if (hp.current() != hp.max()) {
			m_pokemon.set_hp(hp.max());
			m_pokemon.set_status(Statuses::rest);
			m_flags.status.set(Statuses::rest);
		}
	}
	auto end_of_turn_status(Generation const generation, Pokemon const & other, bool const uproar) const {
		m_flags.status.end_of_turn(generation, *this, other, uproar);
	}
	
	auto increment_stockpile() const -> void {
		bool const increased = m_flags.stockpile.increment();
		if (increased) {
			boost_defensive(stage(), 1_bi);
		}
	}
	auto release_stockpile() const -> bounded::integer<0, Stockpile::max> {
		auto const stages = m_flags.stockpile.release();
		boost_defensive(stage(), -stages);
		return stages;
	}

	auto use_substitute(Generation) const -> void;

	auto switch_in(Generation const generation) const {
		m_pokemon.mark_as_seen();
		m_flags.types = PokemonTypes(generation, get_species(m_pokemon));
		m_flags.status.set(get_status(m_pokemon).name());
		if (get_item(m_pokemon) == Item::Berserk_Gene) {
			activate_berserk_gene(*this);
		}
	}
	auto switch_out() const {
		if (clears_status_on_switch(get_ability(*this))) {
			clear_status(*this);
		}
		m_flags.reset_switch();
	}

	auto taunt(Generation const generation) const {
		m_flags.taunt.activate();
		apply_own_mental_herb(generation, *this);
	}
	auto advance_taunt() const {
		m_flags.taunt.advance_one_turn();
	}
	auto torment(Generation const generation) const {
		m_flags.is_tormented = true;
		apply_own_mental_herb(generation, *this);
	}
	auto set_type(Type const type) const {
		m_flags.types = PokemonTypes(type);
	}
	auto u_turn() const -> void;
	auto use_uproar() const -> void;
	auto activate_water_sport() const {
		m_flags.water_sport = true;
	}
	auto hit_with_yawn() const {
		m_flags.yawn.activate();
	}
	auto try_to_activate_yawn(Weather const weather, bool const either_is_uproaring) const -> void {
		bool const put_to_sleep = m_flags.yawn.advance_one_turn();
		if (put_to_sleep) {
			apply_status_to_self(Statuses::sleep, *this, weather, either_is_uproaring);
		}
	}

	// Returns whether the Pokemon ends up in a Vanished state
	auto bounce() const -> bool;
	auto dig() const -> bool;
	auto dive() const -> bool;
	auto fly() const -> bool;
	auto shadow_force() const -> bool;

	auto use_bide(Generation, MutableActivePokemon target) const -> void;

	auto set_hp(Generation const generation, auto const hp) const -> void {
		m_pokemon.set_hp(hp);
		activate_pinch_item(generation);
	}
	auto indirect_damage(Generation, HP::current_type const damage) const -> void;
	auto direct_damage(Generation, HP::current_type const damage) const -> void;

	auto increment_move_use_counter(Moves const move) const {
		m_flags.last_used_move.increment(move);
	}
	auto unsuccessfully_use_move(Moves const move) const {
		m_flags.last_used_move.unsucessful_move(move);
	}

private:
	auto activate_pinch_item(Generation) const -> void;
};

inline auto change_hp(Generation const generation, MutableActivePokemon pokemon, auto const change) {
	pokemon.set_hp(generation, get_hp(pokemon).current() + change);
}

inline auto shift_status(MutableActivePokemon user, MutableActivePokemon target, Weather const weather) -> void {
	auto const status = get_status(user).name();
	// TODO: How does this work with Toxic? How does this work with Rest?
	switch (status) {
		case Statuses::burn:
		case Statuses::paralysis:
		case Statuses::poison:
		case Statuses::toxic:
			target.apply_status(status, user, weather);
			break;
		case Statuses::sleep:
		case Statuses::rest: // TODO: How does Rest shift?
			target.apply_status(Statuses::sleep, user, weather);
			break;
		default:
			break;
	}
	// TODO: Does this clear status when the shift fails?
	clear_status(user);
}

auto apply_status(Statuses const status, MutableActivePokemon target, Weather const weather, bool const uproar) -> void;
inline auto apply_status_to_self(Statuses const status, MutableActivePokemon target, Weather const weather, bool const uproar) -> void {
	target.apply_status(status, target, weather, uproar);
}


inline auto activate_berserk_gene(MutableActivePokemon pokemon) -> void {
	boost(pokemon.stage(), StatNames::ATK, 2_bi);
	// TODO: Berserk Gene causes 256-turn confusion, unless the Pokemon
	// switching out was confused.
	pokemon.confuse();
	set_item(pokemon, Item::None);
}

inline auto apply_own_mental_herb(Generation const generation, MutableActivePokemon const pokemon) -> void {
	if (get_item(pokemon) == Item::Mental_Herb) {
		apply_mental_herb(generation, pokemon);
		set_item(pokemon, Item::None);
	}
}

inline auto apply_white_herb(MutableActivePokemon const pokemon) {
	for (auto & stage : pokemon.stage()) {
		if (stage < 0_bi) {
			stage = 0_bi;
		}
	}
}

inline auto apply_own_white_herb(MutableActivePokemon const pokemon) {
	if (get_item(pokemon) == Item::White_Herb) {
		apply_white_herb(pokemon);
		set_item(pokemon, Item::None);
	}
}

}	// namespace technicalmachine
