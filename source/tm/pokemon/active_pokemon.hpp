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

#include <tm/move/damage_type.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/is_switch.hpp>

#include <tm/stat/stage.hpp>

#include <tm/generation.hpp>
#include <tm/operators.hpp>
#include <tm/random_damage.hpp>
#include <tm/rational.hpp>
#include <tm/weather.hpp>

#include <bounded/assert.hpp>
#include <bounded/integer.hpp>
#include <bounded/detail/variant/variant.hpp>
#include <bounded/detail/overload.hpp>

#include <variant>

namespace technicalmachine {
struct ActivePokemon;
struct MutableActivePokemon;

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
		return bounded::holds_alternative(m_flags.lock_in, bounded::detail::types<BatonPassing>{});
	}

	auto cannot_be_koed() const -> bool {
		return m_flags.damage_blocker.is_enduring();
	}

	auto charge_boosted(Moves const move) const -> bool {
		constexpr auto generation = Generation::seven;
		return m_flags.charged and get_type(generation, move, m_pokemon) == Type::Electric;
	}

	auto is_charging_up() const -> bool {
		return bounded::holds_alternative(m_flags.lock_in, bounded::detail::types<ChargingUp>{});
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

	auto is_fully_paralyzed() const -> bool {
		return m_flags.is_fully_paralyzed;
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

	auto is_having_a_nightmare() const -> bool {
		return m_flags.is_having_a_nightmare;
	}

	auto power_trick_is_active() const -> bool {
		return m_flags.power_trick_is_active;
	}
	
	auto is_protecting() const {
		return m_flags.damage_blocker.is_protecting();
	}

	auto is_locked_in_by_move() const -> bool {
		return !bounded::holds_alternative(m_flags.lock_in, bounded::detail::types<std::monostate>{});
	}

	auto is_roosting() const -> bool {
		return m_flags.is_roosting;
	}

	auto slow_start_is_active() const -> bool {
		return m_flags.slow_start.is_active();
	}

	auto sport_is_active(Moves const foe_move) const -> bool {
		constexpr auto generation = Generation::seven;
		switch (get_type(generation, foe_move, m_pokemon)) {
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
		return bounded::visit(m_flags.lock_in, bounded::overload(
			[](BatonPassing) { return true; },
			[](UTurning) { return true; },
			[&](auto const &) { return get_hp(m_pokemon) == 0_bi; }
		));
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

	auto is_uproaring() const -> bool {
		return bounded::holds_alternative(m_flags.lock_in, bounded::detail::types<UproarCounter>{});
	}

	auto vanish_doubles_power(Moves const move_name) const -> bool {
		auto const & variant = m_flags.lock_in;
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

	auto random_damage_multiplier() const {
		return m_flags.random_damage();
	}

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
			get_hp(*this) = 0_bi;
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
			boost_defensive(stage(), 1_bi);
		}
	}
	auto release_stockpile() const -> bounded::integer<0, Stockpile::max> {
		auto const stages = m_flags.stockpile.release();
		boost_defensive(stage(), -stages);
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
			[](auto const &) { BOUNDED_ASSERT_OR_ASSUME(false); }
		));
	}
	auto activate_water_sport() const {
		m_flags.water_sport = true;
	}
	auto hit_with_yawn() const {
		m_flags.yawn.activate();
	}
	auto try_to_activate_yawn(Weather const weather, bool const either_is_uproaring) const -> void {
		bool const put_to_sleep = m_flags.yawn.advance_one_turn();
		if (put_to_sleep) {
			apply(Statuses::sleep, *this, weather, either_is_uproaring);
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


}	// namespace technicalmachine
