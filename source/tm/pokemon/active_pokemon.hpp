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
#include <tm/weather.hpp>

#include <bounded/integer.hpp>
#include <bounded/detail/variant/variant.hpp>
#include <bounded/detail/overload.hpp>

#include <variant>

namespace technicalmachine {
struct ActivePokemon;
struct MutableActivePokemon;

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
	friend auto damaged(ActivePokemon pokemon) -> bool; \
	friend auto direct_damage_received(ActivePokemon pokemon) -> HP::current_type; \
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
	HP::current_type direct_damage_received = 0_bi;
	bool aqua_ring = false;
	bool attracted = false;
	bool charged = false;
	bool is_cursed = false;
	bool damaged = false;
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

	TECHNICALMACHINE_ACTIVE_POKEMON_FRIEND_FUNCTIONS;

	friend auto operator==(ActivePokemonImpl const lhs, ActivePokemonImpl const rhs) {
		return lhs.m_flags == rhs.m_flags;
	}
	
	operator PokemonRef() const {
		return m_pokemon;
	}

protected:
	PokemonRef m_pokemon;
	FlagsRef m_flags;
};


// A reference to the currently active Pokemon
struct ActivePokemon : ActivePokemonImpl<true> {
	ActivePokemon(Pokemon const & pokemon, ActivePokemonFlags const & flags):
		ActivePokemonImpl(pokemon, flags)
	{
	}
};

#undef TECHNICALMACHINE_ACTIVE_POKEMON_FRIEND_FUNCTIONS

// A mutable reference to the currently active Pokemon
struct MutableActivePokemon : ActivePokemonImpl<false> {
	MutableActivePokemon(Pokemon & pokemon, ActivePokemonFlags & flags):
		ActivePokemonImpl(pokemon, flags)
	{
	}

	operator ActivePokemon() const {
		return ActivePokemon(m_pokemon, m_flags);
	}

	friend auto stage(MutableActivePokemon pokemon) -> Stage &;

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
	auto attract() const {
		m_flags.attracted = true;
	}
	auto baton_pass() const {
		m_flags.lock_in = BatonPassing{};
	}
	auto charge() const {
		m_flags.charged = true;
	}
	auto use_charge_up_move() const {
		m_flags.lock_in = ChargingUp{};
	}
	auto confuse() const -> void {
		if (!blocks_confusion(get_ability(*this))) {
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
	auto disable(Moves const move) const {
		m_flags.disable.activate(move);
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
	auto activate_encore() const {
		m_flags.encore.activate();
	}
	auto advance_encore() const {
		m_flags.encore.advance_one_turn();
	}
	auto endure() const {
		m_flags.enduring = true;
	}
	auto faint() const {
		get_hp(*this) = 0_bi;
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
	auto advance_lock_in(bool const ending) const -> void {
		bounded::visit(m_flags.lock_in, bounded::overload(
			[&](Rampage & rampage) {
				if (ending) {
					m_flags.lock_in = std::monostate{};
					confuse();
				} else {
					rampage.advance_one_turn();
				}
			},
			[&](UproarCounter & uproar) {
				if (ending) {
					m_flags.lock_in = std::monostate{};
				} else {
					uproar.advance_one_turn();
				}
			},
			[](auto const &) {}
		));
	}
	auto use_lock_on() const {
		m_flags.locked_on = true;
	}
	auto activate_magnet_rise() const {
		m_flags.magnet_rise.activate();
	}
	auto advance_magnet_rise() const {
		m_flags.magnet_rise.advance_one_turn();
	}
	auto set_not_moved() const {
		m_flags.moved = false;
	}
	auto activate_mud_sport() const {
		m_flags.mud_sport = true;
	}
	auto give_nightmares() const {
		m_flags.is_having_a_nightmare = true;
	}
	auto partially_trap() const {
		m_flags.partial_trap.activate();
	}
	auto partial_trap_damage() const {
		m_flags.partial_trap.damage(*this);
	}
	auto activate_perish_song() const {
		m_flags.perish_song.activate();
	}
	auto perish_song_turn() const -> void {
		bool const faints_this_turn = m_flags.perish_song.advance_one_turn();
		if (faints_this_turn) {
			faint();
		}
	}
	auto activate_power_trick() const {
		m_flags.power_trick_is_active = !m_flags.power_trick_is_active;
	}
	auto protect() const {
		m_flags.is_protecting = true;
	}
	auto break_protect() const {
		m_flags.is_protecting = false;
	}
	auto activate_rampage() const {
		// TODO: Have it be active when it is constructed
		auto rampage = Rampage{};
		rampage.activate();
		m_flags.lock_in = rampage;
	}
	auto recharge() const -> bool {
		return bounded::visit(m_flags.lock_in, bounded::overload(
			[&](Recharging) { m_flags.lock_in = std::monostate{}; return true; },
			[](auto const &) { return false; }
		));
	}
	auto use_recharge_move() const {
		m_flags.lock_in = Recharging{};
	}
	auto roost() const {
		m_flags.is_roosting = true;
	}
	
	auto increment_stockpile() const -> void {
		bool const increased = m_flags.stockpile.increment();
		if (increased) {
			boost_defensive(stage(*this), 1_bi);
		}
	}
	auto release_stockpile() const -> bounded::integer<0, Stockpile::max> {
		auto const stages = m_flags.stockpile.release();
		boost_defensive(stage(*this), -stages);
		return stages;
	}

	auto use_substitute() const -> void;

	auto taunt() const {
		m_flags.taunt.activate();
	}
	auto advance_taunt() const {
		m_flags.taunt.advance_one_turn();
	}
	auto torment() const {
		m_flags.is_tormented = true;
	}
	auto u_turn() const {
		m_flags.lock_in = UTurning{};
	}
	auto use_uproar() const {
		bounded::visit(m_flags.lock_in, bounded::overload(
			// TODO: Have it be active when it is constructed
			[&](std::monostate) { m_flags.lock_in.emplace(bounded::detail::types<UproarCounter>{}).advance_one_turn(); },
			[](UproarCounter & uproar) { uproar.advance_one_turn(); },
			[](auto const &) { assert(false); }
		));
	}
	auto activate_water_sport() const {
		m_flags.water_sport = true;
	}
	auto hit_with_yawn() const {
		m_flags.yawn.activate();
	}
	auto try_to_activate_yawn(Weather const weather) const -> void {
		bool const put_to_sleep = m_flags.yawn.advance_one_turn();
		if (put_to_sleep) {
			apply(Statuses::sleep, *this, weather);
		}
	}

	// Returns whether the Pokemon ends up in a Vanished state
	auto bounce() const {
		return use_vanish_move<Bouncing>();
	}
	auto dig() const {
		return use_vanish_move<Digging>();
	}
	auto dive() const {
		return use_vanish_move<Diving>();
	}
	auto fly() const {
		return use_vanish_move<Flying>();
	}
	auto shadow_force() const {
		return use_vanish_move<ShadowForcing>();
	}


	auto use_bide(Pokemon & target) const -> void {
		bounded::visit(m_flags.lock_in, bounded::overload(
			[&](auto const &) { m_flags.lock_in = Bide{}; },
			[&](Bide & bide) {
				if (auto const damage = bide.advance_one_turn()) {
					get_hp(target) -= *damage * 2_bi;
					m_flags.lock_in = std::monostate{};
				}
			}
		));
	}

	auto direct_damage(damage_type const damage) const -> void {
		if (m_flags.substitute) {
			m_flags.substitute.damage(damage);
		} else {
			get_hp(*this) -= damage;
			m_flags.damaged = true;
			m_flags.direct_damage_received = damage;
			bounded::visit(m_flags.lock_in, bounded::overload(
				[=](Bide & bide) { bide.add_damage(damage); },
				[](auto const &) {}
			));
		}
	}
	auto indirect_damage(damage_type const damage) const {
		get_hp(*this) -= damage;
		m_flags.damaged = true;
	}
	auto increment_move_use_counter(Moves const move) const {
		m_flags.last_used_move.increment(move);
	}
	auto unsuccessfully_use_move(Moves const move) const {
		m_flags.last_used_move.unsucessful_move(move);
	}
	
private:
	template<typename T>
	auto use_vanish_move() const -> bool {
		return bounded::visit(m_flags.lock_in, bounded::overload(
			[&](T) { m_flags.lock_in = std::monostate{}; return false; },
			[&](auto const &) { m_flags.lock_in = T{}; return true; }
		));
	}
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

inline auto damaged(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.damaged;
}

inline auto direct_damage_received(ActivePokemon const pokemon) -> HP::current_type {
	return pokemon.m_flags.direct_damage_received;
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
