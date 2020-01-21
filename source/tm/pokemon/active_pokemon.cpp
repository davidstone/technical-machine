// Flags for the active Pokemon
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

#include <tm/pokemon/active_pokemon.hpp>

#include <tm/move/move.hpp>

#include <tm/heal.hpp>
#include <tm/weather.hpp>

#include <bounded/assert.hpp>
#include <bounded/insert.hpp>
#include <bounded/detail/overload.hpp>
#include <bounded/unreachable.hpp>

#include <containers/algorithms/all_any_none.hpp>

namespace technicalmachine {

template struct ActivePokemonImpl<true>;
template struct ActivePokemonImpl<false>;

namespace {

auto handle_leech_seed(Generation const generation, MutableActivePokemon pokemon, MutableActivePokemon other, Weather const weather) -> void {
	if (!pokemon.leech_seeded()) {
		return;
	}
	auto const initial = get_hp(pokemon).current();
	heal(generation, pokemon, weather, rational(-1_bi, 8_bi));
	if (get_hp(other) == 0_bi) {
		return;
	}
	auto const hp_change = (initial - get_hp(pokemon).current()) * healing_multiplier(pokemon.item(generation, weather));
	if (damages_leechers(pokemon.ability())) {
		change_hp(generation, other, weather, -hp_change);
	} else {
		change_hp(generation, other, weather, hp_change);
	}
}

auto handle_burn(Generation const generation, MutableActivePokemon pokemon, Weather const weather) -> void {
	auto const denominator = BOUNDED_CONDITIONAL(weakens_burn(pokemon.ability()), 16_bi, 8_bi);
	heal(generation, pokemon, weather, rational(-1_bi, denominator));
}

auto handle_poison(Generation const generation, MutableActivePokemon pokemon, Weather const weather) -> void {
	auto const numerator = BOUNDED_CONDITIONAL(absorbs_poison_damage(pokemon.ability()), 1_bi, -1_bi);
	heal(generation, pokemon, weather, rational(numerator, 8_bi));
}

auto handle_toxic(Generation const generation, MutableActivePokemon pokemon, Weather const weather, bounded::clamped_integer<1, 15> & toxic_counter) -> void {
	if (absorbs_poison_damage(pokemon.ability())) {
		heal(generation, pokemon, weather, rational(1_bi, 8_bi));
	} else {
		heal(generation, pokemon, weather, rational(-toxic_counter, 16_bi));
	}
	++toxic_counter;
}

auto handle_sleep_and_rest(Generation const generation, MutableActivePokemon pokemon, ActivePokemon other, Weather const weather, bool const nightmare, bool const uproar = false) -> void {
	if (uproar) {
		pokemon.clear_status();
		return;
	}
	if (nightmare) {
		heal(generation, pokemon, weather, rational(-1_bi, 4_bi));
	}
	if (harms_sleepers(other.ability())) {
		heal(generation, pokemon, weather, rational(-1_bi, 8_bi));
	}
}

} // namespace

// Generation 1-2
auto ActiveStatus::end_of_attack(Generation const generation, MutableActivePokemon pokemon, MutableActivePokemon const other, Weather const weather) & -> void {
	auto const status = get_status(pokemon).name();
	switch (status) {
		case Statuses::clear:
		case Statuses::freeze:
		case Statuses::paralysis:
			break;
		case Statuses::burn:
			handle_burn(generation, pokemon, weather);
			break;
		case Statuses::poison:
			handle_poison(generation, pokemon, weather);
			break;
		case Statuses::toxic:
			handle_toxic(generation, pokemon, weather, m_toxic_counter);
			break;
		case Statuses::sleep:
		case Statuses::rest:
			break;
	}

	handle_leech_seed(generation, pokemon, other, weather);

	switch (status) {
		case Statuses::clear:
		case Statuses::freeze:
		case Statuses::paralysis:
		case Statuses::burn:
		case Statuses::poison:
		case Statuses::toxic:
			break;
		case Statuses::sleep:
		case Statuses::rest:
			handle_sleep_and_rest(generation, pokemon, other, weather, m_nightmare);
			break;
	}
}

// Generation 3+
auto ActiveStatus::end_of_turn(Generation const generation, MutableActivePokemon pokemon, MutableActivePokemon const other, Weather const weather, bool uproar) & -> void {
	handle_leech_seed(generation, pokemon, other, weather);

	switch (get_status(pokemon).name()) {
		case Statuses::clear:
		case Statuses::freeze:
		case Statuses::paralysis:
			break;
		case Statuses::burn:
			handle_burn(generation, pokemon, weather);
			break;
		case Statuses::poison:
			handle_poison(generation, pokemon, weather);
			break;
		case Statuses::toxic:
			handle_toxic(generation, pokemon, weather, m_toxic_counter);
			break;
		case Statuses::sleep:
		case Statuses::rest:
			handle_sleep_and_rest(generation, pokemon, other, weather, m_nightmare, uproar);
			break;
	}
}

auto ActiveStatus::status_and_leech_seed_effects(Generation const generation, MutableActivePokemon pokemon, MutableActivePokemon const other, Weather const weather, bool const uproar) & -> void {
	if (generation <= Generation::two) {
		end_of_attack(generation, pokemon, other, weather);
	} else {
		end_of_turn(generation, pokemon, other, weather, uproar);
	}
}

auto ActivePokemonFlags::reset_start_of_turn() -> void {
	damage_blocker = {};
	damaged = false;
	direct_damage_received = 0_bi;
	flinched = false;
	moved = false;
	me_first_is_active = false;
	is_loafing_turn = !is_loafing_turn;
}

auto ActivePokemonFlags::reset_switch() -> void {
	// TODO: remove some of these when the foe switches, too
	if (!bounded::holds_alternative(lock_in, bounded::detail::types<BatonPassing>{})) {
		aqua_ring = false;
		confusion = {};
		is_cursed = false;
		embargo = {};
		has_focused_energy = false;
		gastro_acid = false;
		ingrained = false;
		leech_seeded = false;
		locked_on = false;
		magnet_rise = {};
		perish_song = {};
		power_trick_is_active = false;
		stage = {};
		substitute = {};
	}
	attracted = false;
	charged = false;
	damage_blocker = {};
	defense_curled = false;
	destiny_bond = false;
	disable = Disable{};
	encore = {};
	flash_fire = false;
	flinched = false;
	fully_trapped = false;
	heal_block = {};
	identified = false;
	used_imprison = false;
	last_used_move = {};
	is_loafing_turn = true;
	minimized = false;
	me_first_is_active = false;
	mud_sport = false;
	partial_trap = {};
	is_roosting = false;
	slow_start = {};
	stockpile = {};
	is_tormented = false;
	unburdened = false;
	water_sport = false;
	taunt = {};
	yawn = {};
	lock_in = std::monostate{};
}

auto ActivePokemonFlags::is_baton_passing() const -> bool {
	return bounded::holds_alternative(lock_in, bounded::detail::types<BatonPassing>{});
}

auto ActivePokemonFlags::is_charging_up() const -> bool {
	return bounded::holds_alternative(lock_in, bounded::detail::types<ChargingUp>{});
}

auto ActivePokemonFlags::is_locked_in_by_move() const -> bool {
	return !bounded::holds_alternative(lock_in, bounded::detail::types<std::monostate>{});
}

auto ActivePokemonFlags::switch_decision_required(Pokemon const & pokemon) const -> bool {
	return bounded::visit(lock_in, bounded::overload(
		[](BatonPassing) { return true; },
		[](UTurning) { return true; },
		[&](auto const &) { return get_hp(pokemon) == 0_bi; }
	));
}

auto ActivePokemonFlags::is_uproaring() const -> bool {
	return bounded::holds_alternative(lock_in, bounded::detail::types<UproarCounter>{});
}

auto ActivePokemonFlags::vanish_doubles_power(Generation const generation, Moves const move_name) const -> bool {
	switch (move_name) {
	case Moves::Earthquake:
	case Moves::Magnitude:
		return generation >= Generation::two and bounded::holds_alternative(lock_in, bounded::detail::types<Digging>{});
	case Moves::Gust:
	case Moves::Twister:
		return bounded::holds_alternative(lock_in, bounded::detail::types<Bouncing>{}) or bounded::holds_alternative(lock_in, bounded::detail::types<Flying>{});
	case Moves::Surf:
		return bounded::holds_alternative(lock_in, bounded::detail::types<Diving>{});
	default:
		return false;
	}
}

namespace {

auto item_grounds(Generation const generation, ActivePokemon const pokemon, Weather const weather) -> bool {
	auto const item = generation <= Generation::four ?
		pokemon.unrestricted_item(generation) :
		pokemon.item(generation, weather);
	return item == Item::Iron_Ball;
}

} // namespace

auto grounded(Generation const generation, ActivePokemon const pokemon, Weather const weather) -> bool {
	return
		!(
			is_type(pokemon, Type::Flying) or
			is_immune_to_ground(pokemon.ability()) or
			pokemon.magnet_rise().is_active() or
			pokemon.item(generation, weather) == Item::Air_Balloon
		) or
		weather.gravity() or
		item_grounds(generation, pokemon, weather) or
		pokemon.ingrained();
}


auto MutableActivePokemon::attract(Generation const generation, MutableActivePokemon other, Weather const weather) const -> void {
	auto handle_item = [&] {
		switch (item(generation, weather)) {
			case Item::Mental_Herb:
				apply_own_mental_herb(generation, *this, weather);
				break;
			case Item::Destiny_Knot:
				remove_item();
				other.attract(generation, *this, weather);
				break;
			default:
				break;
		}
	};
	auto const ability_cures_attract = ability() == Ability::Oblivious;
	if (generation <= Generation::four) {
		if (!ability_cures_attract) {
			m_flags.attracted = true;
			handle_item();
		}
	} else {
		m_flags.attracted = true;
		handle_item();
		if (ability_cures_attract) {
			m_flags.attracted = false;
		}
	}
}

auto MutableActivePokemon::baton_pass() const -> void {
	m_flags.lock_in = BatonPassing{};
}

auto MutableActivePokemon::use_charge_up_move() const -> void {
	m_flags.lock_in = ChargingUp{};
}

auto MutableActivePokemon::advance_lock_in(Generation const generation, bool const ending, Weather const weather) const -> void {
	bounded::visit(m_flags.lock_in, bounded::overload(
		[&](Rampage & rampage) {
			if (ending) {
				m_flags.lock_in = std::monostate{};
				confuse(generation, weather);
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

auto MutableActivePokemon::activate_rampage() const -> void {
	// TODO: Have it be active when it is constructed
	auto rampage = Rampage{};
	rampage.activate();
	m_flags.lock_in = rampage;
}
auto MutableActivePokemon::recharge() const -> bool {
	return bounded::visit(m_flags.lock_in, bounded::overload(
		[&](Recharging) { m_flags.lock_in = std::monostate{}; return true; },
		[](auto const &) { return false; }
	));
}
auto MutableActivePokemon::use_recharge_move() const -> void {
	m_flags.lock_in = Recharging{};
}

namespace {

auto can_use_substitute(Pokemon const & pokemon) -> bool {
	auto const & hp = get_hp(pokemon);
	return hp.current() > hp.max() / 4_bi;
}

}	// namespace

auto MutableActivePokemon::use_substitute(Generation const generation, Weather const weather) const -> void {
	if (!can_use_substitute(*this))
		return;
	auto const max_hp = get_hp(*this).max();
	indirect_damage(generation, weather, m_flags.substitute.create(max_hp));
}

auto MutableActivePokemon::u_turn() const -> void {
	m_flags.lock_in = UTurning{};
}
auto MutableActivePokemon::use_uproar() const -> void {
	bounded::visit(m_flags.lock_in, bounded::overload(
		// TODO: Have it be active when it is constructed
		[&](std::monostate) { bounded::insert(m_flags.lock_in, UproarCounter()).advance_one_turn(); },
		[](UproarCounter & uproar) { uproar.advance_one_turn(); },
		[](auto const &) { bounded::assert_or_assume_unreachable(); }
	));
}

template<typename T>
auto MutableActivePokemon::use_vanish_move(Generation const generation, Weather const weather) const -> bool {
	return bounded::visit(m_flags.lock_in, bounded::overload(
		[&](T) {
			m_flags.lock_in = std::monostate{};
			return true;
		},
		[&](auto) {
			if (item(generation, weather) == Item::Power_Herb) {
				remove_item();
				return true;
			}
			m_flags.lock_in = T{};
			return false;
		}
	));
}

auto MutableActivePokemon::bounce(Generation const generation, Weather const weather) const -> bool {
	return use_vanish_move<Bouncing>(generation, weather);
}
auto MutableActivePokemon::dig(Generation const generation, Weather const weather) const -> bool {
	return use_vanish_move<Digging>(generation, weather);
}
auto MutableActivePokemon::dive(Generation const generation, Weather const weather) const -> bool {
	return use_vanish_move<Diving>(generation, weather);
}
auto MutableActivePokemon::fly(Generation const generation, Weather const weather) const -> bool {
	return use_vanish_move<Flying>(generation, weather);
}
auto MutableActivePokemon::shadow_force(Generation const generation, Weather const weather) const -> bool {
	return use_vanish_move<ShadowForcing>(generation, weather);
}

auto MutableActivePokemon::use_bide(Generation const generation, MutableActivePokemon target, Weather const weather) const -> void {
	bounded::visit(m_flags.lock_in, bounded::overload(
		[&](auto const &) { m_flags.lock_in = Bide{}; },
		[&](Bide & bide) {
			if (auto const damage = bide.advance_one_turn()) {
				change_hp(generation, target, weather, -*damage * 2_bi);
				m_flags.lock_in = std::monostate{};
			}
		}
	));
}

auto MutableActivePokemon::indirect_damage(Generation const generation, Weather const weather, HP::current_type const damage) const -> void {
	change_hp(generation, *this, weather, -damage);
	m_flags.damaged = true;
}

namespace {

constexpr bool cannot_ko(Moves const move) {
	return move == Moves::False_Swipe;
}

auto handle_ko(Generation const generation, MutableActivePokemon defender, bool const is_enduring, Moves const move, Weather const weather) {
	if (cannot_ko(move) or is_enduring) {
		return true;
	}
	auto const hp = get_hp(defender);
	if (hp.current() != hp.max()) {
		return false;
	}
	if (generation >= Generation::five and defender.ability() == Ability::Sturdy) {
		return true;
	}
	if (defender.item(generation, weather) == Item::Focus_Sash) {
		defender.remove_item();
		return true;
	}
	return false;
}

} // namespace

auto MutableActivePokemon::direct_damage(Generation const generation, Moves const move, Weather const weather, damage_type const damage) const -> HP::current_type {
	auto const interaction = substitute_interaction(generation, move);
	BOUNDED_ASSERT(!m_flags.substitute or interaction != Substitute::causes_failure);
	if (m_flags.substitute and interaction == Substitute::absorbs) {
		return m_flags.substitute.damage(damage);
	}
	auto const original_hp = get_hp(m_pokemon).current();
	auto const block_ko = original_hp <= damage and handle_ko(generation, *this, m_flags.damage_blocker.is_enduring(), move, weather);
	auto const applied_damage = block_ko ?
		static_cast<HP::current_type>(original_hp - 1_bi) :
		bounded::min(damage, original_hp);
	indirect_damage(generation, weather, applied_damage);
	m_flags.direct_damage_received = applied_damage;
	bounded::visit(m_flags.lock_in, bounded::overload(
		[=](Bide & bide) { bide.add_damage(applied_damage); },
		[](auto const &) {}
	));
	return applied_damage;
}

namespace {

auto status_can_apply(Statuses const status, ActivePokemon const user, ActivePokemon const target, Weather const weather, bool const uproar) {
	return
		is_clear(get_status(target)) and
		!blocks_status(target.ability(), user.ability(), status, weather) and
		!containers::any(target.types(), [=](Type const type) { return blocks_status(type, status); }) and
		(!uproar or (status != Statuses::sleep and status != Statuses::rest));
}

constexpr auto reflected_status(Generation const generation, Statuses const status) -> bounded::optional<Statuses> {
	switch (status) {
	case Statuses::burn:
	case Statuses::paralysis:
	case Statuses::poison:
		return status;
	case Statuses::toxic:
		return generation <= Generation::four ? Statuses::poison : Statuses::toxic;
	case Statuses::clear:
	case Statuses::freeze:
	case Statuses::sleep:
	case Statuses::rest:
		return bounded::none;
	}
}

} // namespace

auto MutableActivePokemon::apply_status(Generation const generation, Statuses const status, MutableActivePokemon user, Weather const weather, bool const uproar) const -> void {
	BOUNDED_ASSERT_OR_ASSUME(status != Statuses::clear);
	BOUNDED_ASSERT_OR_ASSUME(status != Statuses::rest);
	if (!status_can_apply(status, user, *this, weather, uproar)) {
		return;
	}
	set_status(generation, status, weather);
	auto const reflected = reflected_status(generation, status);
	if (reflected and reflects_status(ability())) {
		user.apply_status(generation, *reflected, *this, weather, uproar);
	}
}

auto MutableActivePokemon::rest(Generation const generation, Weather const weather, bool const other_is_uproaring) const -> void {
	if (other_is_uproaring or is_sleeping(get_status(m_pokemon))) {
		return;
	}
	if (blocks_status(ability(), ability(), Statuses::rest, weather)) {
		return;
	}
	auto const hp = get_hp(m_pokemon);
	if (hp.current() == hp.max()) {
		return;
	}
	m_pokemon.set_hp(hp.max());
	set_status(generation, Statuses::rest, weather);
}

auto MutableActivePokemon::activate_pinch_item(Generation const generation, Weather const weather) const -> void {
	// TODO: Confusion damage does not activate healing berries in Generation 5+
	auto consume = [&] { remove_item(); };

	auto const current_hp = hp_ratio(m_pokemon);

	auto quarter_threshold = [&] {
		return BOUNDED_CONDITIONAL(ability() == Ability::Gluttony, rational(1_bi, 2_bi), rational(1_bi, 4_bi));
	};

	auto healing_berry = [&](auto const threshold, auto const amount) {
		if (current_hp > threshold) {
			return false;
		}
		consume();
		m_pokemon.set_hp(get_hp(m_pokemon).current() + amount);
		return true;
	};

	auto confusion_berry = [&](StatNames const stat) {
		auto const amount = get_hp(m_pokemon).max() / BOUNDED_CONDITIONAL(generation <= Generation::six, 8_bi, 2_bi);
		auto const threshold = generation <= Generation::six ? rational(1_bi, 2_bi) : quarter_threshold();
		auto const activated = healing_berry(threshold, amount);
		if (activated and lowers_stat(get_nature(m_pokemon), stat)) {
			confuse(generation, weather);
		}
	};

	auto stat_boost_berry = [&](StatNames const stat) {
		if (current_hp > quarter_threshold()) {
			return;
		}
		consume();
		stage()[stat] += 1_bi;
	};

	switch (item(generation, weather)) {
		case Item::Aguav_Berry:
			confusion_berry(StatNames::SPD);
			break;
		case Item::Apicot_Berry:
			stat_boost_berry(StatNames::SPD);
			break;
		case Item::Berry:
		case Item::Oran_Berry:
			healing_berry(rational(1_bi, 2_bi), 10_bi);
			break;
		case Item::Berry_Juice:
			healing_berry(rational(1_bi, 2_bi), 20_bi);
			break;
		case Item::Custap_Berry:
			consume();
			break;
		case Item::Figy_Berry:
			confusion_berry(StatNames::ATK);
			break;
		case Item::Ganlon_Berry:
			stat_boost_berry(StatNames::DEF);
			break;
		case Item::Gold_Berry:
			healing_berry(rational(1_bi, 2_bi), 30_bi);
			break;
		case Item::Iapapa_Berry:
			confusion_berry(StatNames::DEF);
			break;
		case Item::Lansat_Berry:
			consume();
			break;
		case Item::Liechi_Berry:
			stat_boost_berry(StatNames::ATK);
			break;
		case Item::Mago_Berry:
			confusion_berry(StatNames::SPE);
			break;
		case Item::Micle_Berry:
			consume();
			break;
		case Item::Petaya_Berry:
			stat_boost_berry(StatNames::SPA);
			break;
		case Item::Salac_Berry:
			stat_boost_berry(StatNames::SPE);
			break;
		case Item::Sitrus_Berry:
			healing_berry(
				rational(1_bi, 2_bi),
				BOUNDED_CONDITIONAL(generation <= Generation::three, 30_bi, get_hp(m_pokemon).max() / 4_bi)
			);
			break;
		case Item::Starf_Berry:
			// TODO: Raise Atk, Def, SpA, SpD, or Spe +2
			consume();
			break;
		case Item::Wiki_Berry:
			confusion_berry(StatNames::SPA);
			break;
		default:
			break;
	}
}

}	// namespace technicalmachine
