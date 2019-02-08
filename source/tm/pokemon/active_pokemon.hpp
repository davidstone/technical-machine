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

#pragma once

#include <tm/pokemon/confusion.hpp>
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

#include <tm/move/damage_type.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/is_switch.hpp>

#include <tm/stat/stage.hpp>

#include <tm/operators.hpp>
#include <tm/random_damage.hpp>
#include <tm/rational.hpp>

#include <bounded/integer.hpp>
#include <bounded/detail/variant/variant.hpp>
#include <bounded/detail/overload.hpp>

#include <variant>

namespace technicalmachine {
struct ActivePokemon;
struct MutableActivePokemon;
struct Weather;

// I use a macro here because I rely on a conversion operator. Friend functions
// declared only in a class body are not found by lookup rules in that case. The
// macro solution seemed better than duplicating all of this by hand.

#define TECHNICALMACHINE_ACTIVE_POKEMON_FRIEND_FUNCTIONS \
	friend auto last_used_move(ActivePokemon pokemon) -> LastUsedMove; \
	friend auto substitute(ActivePokemon pokemon) -> Substitute const &; \
	friend auto aqua_ring_is_active(ActivePokemon pokemon) -> bool; \
	friend auto is_baton_passing(ActivePokemon pokemon) -> bool; \
	friend auto cannot_be_koed(ActivePokemon pokemon) -> bool; \
	friend auto charge_boosted(ActivePokemon pokemon, Moves move) -> bool; \
	friend auto is_charging_up(ActivePokemon pokemon) -> bool; \
	friend auto is_confused(ActivePokemon pokemon) -> bool; \
	friend auto is_cursed(ActivePokemon pokemon) -> bool; \
	friend auto defense_curled(ActivePokemon pokemon) -> bool; \
	/* Requires that move is actually one of this Pokemon's moves */ \
	friend auto is_disabled(ActivePokemon pokemon, Moves move) -> bool; \
	friend auto is_encored(ActivePokemon pokemon) -> bool; \
	friend auto flash_fire_is_active(ActivePokemon pokemon) -> bool; \
	friend auto flinched(ActivePokemon pokemon) -> bool; \
	friend auto has_focused_energy(ActivePokemon pokemon) -> bool; \
	friend auto heal_block_is_active(ActivePokemon pokemon) -> bool; \
	friend auto leech_seeded(ActivePokemon pokemon) -> bool; \
	friend auto is_loafing(ActivePokemon pokemon) -> bool; \
	friend auto locked_on(ActivePokemon pokemon) -> bool; \
	friend auto used_imprison(ActivePokemon pokemon) -> bool; \
	friend auto ingrained(ActivePokemon pokemon) -> bool; \
	friend auto is_fully_paralyzed(ActivePokemon pokemon) -> bool; \
	friend auto magnet_rise(ActivePokemon pokemon) -> MagnetRise const &; \
	friend auto me_first_is_active(ActivePokemon pokemon) -> bool; \
	friend auto minimized(ActivePokemon pokemon) -> bool; \
	friend auto moved(ActivePokemon pokemon) -> bool; \
	friend auto is_having_a_nightmare(ActivePokemon pokemon) -> bool; \
	friend auto power_trick_is_active(ActivePokemon pokemon) -> bool; \
	friend auto is_locked_in_by_move(ActivePokemon pokemon) -> bool; \
	friend auto is_roosting(ActivePokemon pokemon) -> bool; \
	friend auto slow_start_is_active(ActivePokemon pokemon) -> bool; \
	friend auto sport_is_active(ActivePokemon pokemon, Moves foe_move) -> bool; \
	friend auto stage(ActivePokemon pokemon) -> Stage const &; \
	friend auto spit_up_power(ActivePokemon pokemon) -> decltype(std::declval<Stockpile>().spit_up_power()); \
	friend auto switch_decision_required(ActivePokemon pokemon) -> bool; \
	friend auto fully_trapped(ActivePokemon pokemon) -> bool; \
	friend auto trapped(ActivePokemon pokemon) -> bool; \
	friend auto is_tormented(ActivePokemon pokemon) -> bool; \
	friend auto is_taunted(ActivePokemon pokemon) -> bool; \
	friend auto is_uproaring(ActivePokemon pokemon) -> bool; \
	friend auto vanish_doubles_power(ActivePokemon pokemon, Moves move_name) -> bool; \
	friend auto random_damage_multiplier(ActivePokemon pokemon) -> decltype(std::declval<RandomDamage>()())


// Various states a Pokemon can be in due to vanishing moves.
struct Bouncing{};
struct Digging{};
struct Diving{};
struct Flying{};
struct ShadowForcing{};

struct BatonPassing{};
struct ChargingUp{};
struct Recharging{};
struct UTurning{};

struct ActivePokemonFlags {
	TECHNICALMACHINE_ACTIVE_POKEMON_FRIEND_FUNCTIONS;
	friend auto stage(MutableActivePokemon pokemon) -> Stage &;

	auto reset_end_of_turn() -> void;
	auto reset_switch() -> void;

	friend constexpr auto operator==(ActivePokemonFlags const & lhs, ActivePokemonFlags const & rhs) {
		return
			lhs.lock_in == rhs.lock_in and
			lhs.aqua_ring == rhs.aqua_ring and
			lhs.attracted == rhs.attracted and
			lhs.charged == rhs.charged and
			lhs.confusion == rhs.confusion and
			lhs.is_cursed == rhs.is_cursed and
			lhs.defense_curled == rhs.defense_curled and
			lhs.destiny_bond == rhs.destiny_bond and
			lhs.disable == rhs.disable and
			lhs.embargo == rhs.embargo and
			lhs.encore == rhs.encore and
			lhs.flash_fire == rhs.flash_fire and
			lhs.has_focused_energy == rhs.has_focused_energy and
			lhs.fully_trapped == rhs.fully_trapped and
			lhs.heal_block == rhs.heal_block and
			lhs.identified == rhs.identified and
			lhs.used_imprison == rhs.used_imprison and
			lhs.ingrained == rhs.ingrained and
			lhs.last_used_move == rhs.last_used_move and
			lhs.leech_seeded == rhs.leech_seeded and
			lhs.is_loafing_turn == rhs.is_loafing_turn and
			lhs.locked_on == rhs.locked_on and
			lhs.magnet_rise == rhs.magnet_rise and
			lhs.minimized == rhs.minimized and
			lhs.mud_sport == rhs.mud_sport and
			lhs.is_having_a_nightmare == rhs.is_having_a_nightmare and
			lhs.partial_trap == rhs.partial_trap and
			lhs.perish_song == rhs.perish_song and
			lhs.slow_start == rhs.slow_start and
			lhs.stage == rhs.stage and
			lhs.stockpile == rhs.stockpile and
			lhs.taunt == rhs.taunt and
			lhs.is_tormented == rhs.is_tormented and
			lhs.water_sport == rhs.water_sport and
			lhs.yawn == rhs.yawn;
	}

private:
	friend struct ActivePokemon;
	friend struct MutableActivePokemon;
	
	// TODO: Include other mutually exclusive stuff such as Protect and Endure?
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
	SlowStart slow_start;
	Stockpile stockpile;
	TauntCounter taunt;
	YawnCounter yawn;
	bool aqua_ring = false;
	bool attracted = false;
	bool charged = false;
	bool is_cursed = false;
	bool defense_curled = false;
	bool destiny_bond = false;
	bool enduring = false;
	bool flash_fire = false;
	bool flinched = false;
	bool has_focused_energy = false;
	bool is_fully_paralyzed = false;
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
	bool is_having_a_nightmare = false;
	bool power_trick_is_active = false;
	bool is_protecting = false;
	bool is_roosting = false;
	bool is_tormented = false;
	bool water_sport = false;
};


// A reference to the currently active Pokemon
struct ActivePokemon {
	ActivePokemon(Pokemon const & pokemon, ActivePokemonFlags const & flags):
		m_pokemon(pokemon),
		m_flags(flags)
	{
	}
	ActivePokemon(ActivePokemon const & other) = default;
	ActivePokemon(ActivePokemon && other) = default;
	ActivePokemon & operator=(ActivePokemon const & other) = delete;
	ActivePokemon & operator=(ActivePokemon && other) = delete;

	operator Pokemon const & () const {
		return m_pokemon;
	}

	TECHNICALMACHINE_ACTIVE_POKEMON_FRIEND_FUNCTIONS;

	friend auto operator==(ActivePokemon const lhs, ActivePokemon const rhs) {
		return lhs.m_flags == rhs.m_flags;
	}

private:
	Pokemon const & m_pokemon;
	ActivePokemonFlags const & m_flags;
};

#undef TECHNICALMACHINE_ACTIVE_POKEMON_FRIEND_FUNCTIONS

// A mutable reference to the currently active Pokemon
struct MutableActivePokemon {
	MutableActivePokemon(Pokemon & pokemon, ActivePokemonFlags & flags):
		m_pokemon(pokemon),
		m_flags(flags)
	{
	}
	MutableActivePokemon(MutableActivePokemon const & other) = default;
	MutableActivePokemon(MutableActivePokemon && other) = default;
	MutableActivePokemon & operator=(MutableActivePokemon const & other) = delete;
	MutableActivePokemon & operator=(MutableActivePokemon && other) = delete;

	operator ActivePokemon() const {
		return ActivePokemon(m_pokemon, m_flags);
	}
	operator Pokemon & () const {
		return m_pokemon;
	}

	friend auto stage(MutableActivePokemon pokemon) -> Stage &;

	auto clear_field() {
		m_flags.leech_seeded = false;
		m_flags.partial_trap = {};
	}
	auto update_before_move() {
		m_flags.destiny_bond = false;
		m_flags.locked_on = false;
		m_flags.moved = true;
	}

	auto activate_aqua_ring() {
		m_flags.aqua_ring = true;
	}
	auto attract() {
		m_flags.attracted = true;
	}
	auto baton_pass() {
		m_flags.lock_in = BatonPassing{};
	}
	auto charge() {
		m_flags.charged = true;
	}
	auto use_charge_up_move() {
		m_flags.lock_in = ChargingUp{};
	}
	auto confuse() -> void;
	auto handle_confusion() {
		m_flags.confusion.do_turn();
	}
	auto curse() {
		m_flags.is_cursed = true;
	}
	auto defense_curl() {
		m_flags.defense_curled = true;
	}
	auto use_destiny_bond() {
		m_flags.destiny_bond = true;
	}
	auto disable(Moves const move) {
		if (is_regular(move)) {
			m_flags.disable.activate(move);
		}
	}
	auto advance_disable() {
		m_flags.disable.advance_one_turn();
	}
	auto activate_embargo() {
		m_flags.embargo.activate();
	}
	auto advance_embargo() {
		m_flags.embargo.advance_one_turn();
	}
	auto activate_encore() {
		m_flags.encore.activate();
	}
	auto advance_encore() {
		m_flags.encore.advance_one_turn();
	}
	auto endure() {
		m_flags.enduring = true;
	}
	auto faint() {
		get_hp(*this) = 0_bi;
	}
	auto activate_flash_fire() {
		m_flags.flash_fire = true;
	}
	auto flinch() {
		m_flags.flinched = true;
	}
	auto focus_energy() {
		m_flags.has_focused_energy = true;
	}
	auto fully_trap() {
		m_flags.fully_trapped = true;
	}
	auto activate_heal_block() {
		m_flags.heal_block.activate();
	}
	auto advance_heal_block() {
		m_flags.heal_block.advance_one_turn();
	}
	auto identify() {
		m_flags.identified = true;
	}
	auto use_imprison() {
		m_flags.used_imprison = true;
	}
	auto ingrain() {
		m_flags.ingrained = true;
	}
	auto hit_with_leech_seed() {
		m_flags.leech_seeded = true;
	}
	
	auto advance_lock_in(bool ending) -> void;
	auto use_lock_on() {
		m_flags.locked_on = true;
	}
	auto activate_magnet_rise() {
		m_flags.magnet_rise.activate();
	}
	auto advance_magnet_rise() {
		m_flags.magnet_rise.advance_one_turn();
	}
	auto set_not_moved() {
		m_flags.moved = false;
	}
	auto activate_mud_sport() {
		m_flags.mud_sport = true;
	}
	auto give_nightmares() {
		m_flags.is_having_a_nightmare = true;
	}
	auto partially_trap() {
		m_flags.partial_trap.activate();
	}
	auto partial_trap_damage() {
		m_flags.partial_trap.damage(*this);
	}
	auto activate_perish_song() {
		m_flags.perish_song.activate();
	}
	auto perish_song_turn() -> void;
	auto activate_power_trick() {
		m_flags.power_trick_is_active = !m_flags.power_trick_is_active;
	}
	auto protect() {
		m_flags.is_protecting = true;
	}
	auto break_protect() {
		m_flags.is_protecting = false;
	}
	auto activate_rampage() {
		// TODO: Have it be active when it is constructed
		auto rampage = Rampage{};
		rampage.activate();
		m_flags.lock_in = rampage;
	}
	auto recharge() -> bool;
	auto use_recharge_move() {
		m_flags.lock_in = Recharging{};
	}
	auto roost() {
		m_flags.is_roosting = true;
	}

	auto increment_stockpile() -> void;
	auto release_stockpile() -> bounded::integer<0, Stockpile::max>;

	auto use_substitute() -> void;

	auto taunt() {
		m_flags.taunt.activate();
	}
	auto advance_taunt() {
		m_flags.taunt.advance_one_turn();
	}
	auto torment() {
		m_flags.is_tormented = true;
	}
	auto u_turn() {
		m_flags.lock_in = UTurning{};
	}
	auto use_uproar() {
		bounded::visit(m_flags.lock_in, bounded::overload(
			// TODO: Have it be active when it is constructed
			[&](std::monostate) { m_flags.lock_in.emplace(bounded::detail::types<UproarCounter>{}).advance_one_turn(); },
			[](UproarCounter & uproar) { uproar.advance_one_turn(); },
			[](auto const &) { assert(false); }
		));
	}
	auto activate_water_sport() {
		m_flags.water_sport = true;
	}
	auto hit_with_yawn() {
		m_flags.yawn.activate();
	}
	// Advance the yawn counter and possibly put the Pokemon to sleep
	auto try_to_activate_yawn(Weather weather) -> void;

	// Returns whether the Pokemon ends up in a Vanished state
	auto bounce() {
		return use_vanish_move<Bouncing>();
	}
	auto dig() {
		return use_vanish_move<Digging>();
	}
	auto dive() {
		return use_vanish_move<Diving>();
	}
	auto fly() {
		return use_vanish_move<Flying>();
	}
	auto shadow_force() {
		return use_vanish_move<ShadowForcing>();
	}


	auto use_bide(Pokemon & target) -> void;

	auto direct_damage(damage_type damage) -> void;
	auto indirect_damage(damage_type const damage) {
		get_hp(*this) -= damage;
	}
	auto increment_move_use_counter(Moves const move) {
		m_flags.last_used_move.increment(move);
	}
	
private:
	template<typename T>
	auto use_vanish_move() -> bool {
		return bounded::visit(m_flags.lock_in, bounded::overload(
			[&](T) { m_flags.lock_in = std::monostate{}; return false; },
			[&](auto const &) { m_flags.lock_in = T{}; return true; }
		));
	}

	Pokemon & m_pokemon;
	ActivePokemonFlags & m_flags;
};


inline auto last_used_move(ActivePokemon const pokemon) -> LastUsedMove {
	return pokemon.m_flags.last_used_move;
}
inline auto substitute(ActivePokemon const pokemon) -> Substitute const & {
	return pokemon.m_flags.substitute;
}


inline auto shed_skin_probability(ActivePokemon const pokemon) {
	return can_clear_status(get_ability(pokemon), get_status(pokemon)) ? 0.3 : 0.0;
}

auto has_switched(ActivePokemon const pokemon) -> bool;

inline auto aqua_ring_is_active(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.aqua_ring;
}

inline auto is_baton_passing(ActivePokemon const pokemon) -> bool {
	return bounded::holds_alternative(pokemon.m_flags.lock_in, bounded::detail::types<BatonPassing>{});
}

inline auto cannot_be_koed(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.enduring;
}

inline auto charge_boosted(ActivePokemon const pokemon, Moves const move) -> bool {
	return pokemon.m_flags.charged and get_type(move, pokemon) == Type::Electric;
}

inline auto is_charging_up(ActivePokemon const pokemon) -> bool {
	return bounded::holds_alternative(pokemon.m_flags.lock_in, bounded::detail::types<ChargingUp>{});
}

inline auto is_confused(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.confusion.is_active();
}

inline auto is_cursed(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.is_cursed;
}

inline auto defense_curled(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.defense_curled;
}

inline auto is_disabled(ActivePokemon const pokemon, Moves const move_name) -> bool {
	return pokemon.m_flags.disable.move_is_disabled(move_name);
}

inline auto is_encored(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.encore.is_active();
}

inline auto flash_fire_is_active(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.flash_fire;
}

inline auto flinched(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.flinched;
}

inline auto has_focused_energy(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.has_focused_energy;
}

inline auto used_imprison(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.used_imprison;
}

inline auto ingrained(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.ingrained;
}

inline auto heal_block_is_active(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.heal_block.is_active();
}

inline auto is_fully_paralyzed(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.is_fully_paralyzed;
}

inline auto leech_seeded(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.leech_seeded;
}

inline auto is_loafing(ActivePokemon const pokemon) -> bool {
	return is_loafing(get_ability(pokemon), pokemon.m_flags.is_loafing_turn);
}

inline auto locked_on(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.locked_on;
}

inline auto magnet_rise(ActivePokemon const pokemon) -> MagnetRise const & {
	return pokemon.m_flags.magnet_rise;
}

inline auto me_first_is_active(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.me_first_is_active;
}

inline auto minimized(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.minimized;
}

inline auto moved(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.moved;
}

inline auto is_having_a_nightmare(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.is_having_a_nightmare;
}

inline auto power_trick_is_active(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.power_trick_is_active;
}

inline auto is_locked_in_by_move(ActivePokemon const pokemon) -> bool {
	return !bounded::holds_alternative(pokemon.m_flags.lock_in, bounded::detail::types<std::monostate>{});
}

inline auto is_roosting(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.is_roosting;
}

inline auto slow_start_is_active(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.slow_start.is_active();
}

inline auto sport_is_active(ActivePokemon const pokemon, Moves const foe_move) -> bool {
	switch (get_type(foe_move, pokemon)) {
	case Type::Electric:
		return pokemon.m_flags.mud_sport;
	case Type::Fire:
		return pokemon.m_flags.water_sport;
	default:
		return false;
	}
}

inline auto stage(ActivePokemon const pokemon) -> Stage const & {
	return pokemon.m_flags.stage;
}
inline auto stage(MutableActivePokemon pokemon) -> Stage & {
	return pokemon.m_flags.stage;
}

inline auto spit_up_power(ActivePokemon const pokemon) -> decltype(pokemon.m_flags.stockpile.spit_up_power()) {
	return pokemon.m_flags.stockpile.spit_up_power();
}

inline auto switch_decision_required(ActivePokemon const pokemon) -> bool {
	return bounded::visit(pokemon.m_flags.lock_in, bounded::overload(
		[](BatonPassing) { return true; },
		[](UTurning) { return true; },
		[&](auto const &) { return get_hp(pokemon) == 0_bi; }
	));
}

inline auto fully_trapped(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.fully_trapped;
}

inline auto trapped(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.fully_trapped or ingrained(pokemon) or pokemon.m_flags.partial_trap.is_active();
}

inline auto is_tormented(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.is_tormented;
}

inline auto is_taunted(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.taunt.is_active();
}

inline auto is_uproaring(ActivePokemon const pokemon) -> bool {
	return bounded::holds_alternative(pokemon.m_flags.lock_in, bounded::detail::types<UproarCounter>{});
}

inline auto vanish_doubles_power(ActivePokemon const pokemon, Moves const move_name) -> bool {
	auto const & variant = pokemon.m_flags.lock_in;
	switch (move_name) {
	case Moves::Earthquake:
	case Moves::Magnitude:
		return bounded::holds_alternative(variant, bounded::detail::types<Digging>{});
	case Moves::Gust:
	case Moves::Twister:
		return bounded::holds_alternative(variant, bounded::detail::types<Bouncing>{}) or bounded::holds_alternative(variant, bounded::detail::types<Flying>{});
	case Moves::Surf:
		return bounded::holds_alternative(variant, bounded::detail::types<Diving>{});
	default:
		return false;
	}
}

inline auto random_damage_multiplier(ActivePokemon const pokemon) -> decltype(pokemon.m_flags.random_damage()) {
	return pokemon.m_flags.random_damage();
}



}	// namespace technicalmachine
