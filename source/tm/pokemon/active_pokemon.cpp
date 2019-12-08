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
#include <bounded/detail/overload.hpp>
#include <bounded/unreachable.hpp>

#include <containers/algorithms/all_any_none.hpp>

namespace technicalmachine {

template struct ActivePokemonImpl<true>;
template struct ActivePokemonImpl<false>;

auto ActiveStatus::end_of_turn(Generation const generation, MutableActivePokemon pokemon, Pokemon const & other, bool const uproar) & -> void {
	switch (get_status(pokemon).name()) {
		case Statuses::clear:
		case Statuses::freeze:
		case Statuses::paralysis:
			return;
		case Statuses::burn: {
			auto const denominator = BOUNDED_CONDITIONAL(weakens_burn(get_ability(pokemon)), 16_bi, 8_bi);
			heal(generation, pokemon, rational(-1_bi, denominator));
			return;
		}
		case Statuses::poison: {
			auto const numerator = BOUNDED_CONDITIONAL(absorbs_poison_damage(get_ability(pokemon)), 1_bi, -1_bi);
			heal(generation, pokemon, rational(numerator, 8_bi));
			return;
		}
		case Statuses::toxic: {
			if (absorbs_poison_damage(get_ability(pokemon))) {
				heal(generation, pokemon, rational(1_bi, 8_bi));
			} else {
				heal(generation, pokemon, rational(-m_toxic_counter, 16_bi));
			}
			++m_toxic_counter;
			return;
		}
		case Statuses::sleep:
		case Statuses::rest:
			if (uproar) {
				clear_status(pokemon);
				return;
			}
			if (m_nightmare) {
				heal(generation, pokemon, rational(-1_bi, 4_bi));
			}
			if (harms_sleepers(get_ability(other))) {
				heal(generation, pokemon, rational(-1_bi, 8_bi));
			}
			return;
	}
}

auto ActivePokemonFlags::reset_end_of_turn() -> void {
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
	// Do I set to true or false? true makes it wrong when a fainted Pokemon is
	// replaced; false makes it wrong otherwise
	is_loafing_turn = false;
	minimized = false;
	me_first_is_active = false;
	mud_sport = false;
	partial_trap = {};
	is_roosting = false;
	slow_start = {};
	stockpile = {};
	is_tormented = false;
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

auto ActivePokemonFlags::vanish_doubles_power(Moves const move_name) const -> bool {
	switch (move_name) {
	case Moves::Earthquake:
	case Moves::Magnitude:
		return bounded::holds_alternative(lock_in, bounded::detail::types<Digging>{});
	case Moves::Gust:
	case Moves::Twister:
		return bounded::holds_alternative(lock_in, bounded::detail::types<Bouncing>{}) or bounded::holds_alternative(lock_in, bounded::detail::types<Flying>{});
	case Moves::Surf:
		return bounded::holds_alternative(lock_in, bounded::detail::types<Diving>{});
	default:
		return false;
	}
}


auto grounded(ActivePokemon const pokemon, Weather const weather) -> bool {
	return
		!(
			is_type(pokemon, Type::Flying) or
			is_immune_to_ground(get_ability(pokemon)) or
			pokemon.magnet_rise().is_active()
		) or
		weather.gravity() or
		grounds(get_item(pokemon)) or
		pokemon.ingrained();
}


auto MutableActivePokemon::attract(Generation const generation, MutableActivePokemon other) const -> void {
	auto handle_item = [&] {
		switch (get_item(m_pokemon)) {
			case Item::Mental_Herb:
				apply_own_mental_herb(generation, *this);
				break;
			case Item::Destiny_Knot:
				set_item(*this, Item::None);
				other.attract(generation, *this);
				break;
			default:
				break;
		}
	};
	auto const ability_cures_attract = get_ability(*this) == Ability::Oblivious;
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

auto MutableActivePokemon::advance_lock_in(bool const ending) const -> void {
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

auto MutableActivePokemon::use_substitute(Generation const generation) const -> void {
	if (!can_use_substitute(*this))
		return;
	auto const max_hp = get_hp(*this).max();
	indirect_damage(generation, m_flags.substitute.create(max_hp));
}

auto MutableActivePokemon::u_turn() const -> void {
	m_flags.lock_in = UTurning{};
}
auto MutableActivePokemon::use_uproar() const -> void {
	bounded::visit(m_flags.lock_in, bounded::overload(
		// TODO: Have it be active when it is constructed
		[&](std::monostate) { m_flags.lock_in.emplace(bounded::detail::types<UproarCounter>{}).advance_one_turn(); },
		[](UproarCounter & uproar) { uproar.advance_one_turn(); },
		[](auto const &) { bounded::assert_or_assume_unreachable(); }
	));
}

namespace {

template<typename T>
auto use_vanish_move(auto & lock_in) -> bool {
	return bounded::visit(lock_in, bounded::overload(
		[&](T) { lock_in = std::monostate{}; return false; },
		[&](auto const &) { lock_in = T{}; return true; }
	));
}

} // namespace

auto MutableActivePokemon::bounce() const -> bool {
	return use_vanish_move<Bouncing>(m_flags.lock_in);
}
auto MutableActivePokemon::dig() const -> bool {
	return use_vanish_move<Digging>(m_flags.lock_in);
}
auto MutableActivePokemon::dive() const -> bool {
	return use_vanish_move<Diving>(m_flags.lock_in);
}
auto MutableActivePokemon::fly() const -> bool {
	return use_vanish_move<Flying>(m_flags.lock_in);
}
auto MutableActivePokemon::shadow_force() const -> bool {
	return use_vanish_move<ShadowForcing>(m_flags.lock_in);
}

auto MutableActivePokemon::use_bide(Generation const generation, MutableActivePokemon target) const -> void {
	bounded::visit(m_flags.lock_in, bounded::overload(
		[&](auto const &) { m_flags.lock_in = Bide{}; },
		[&](Bide & bide) {
			if (auto const damage = bide.advance_one_turn()) {
				change_hp(generation, target, -*damage * 2_bi);
				m_flags.lock_in = std::monostate{};
			}
		}
	));
}

auto MutableActivePokemon::indirect_damage(Generation const generation, HP::current_type const damage) const -> void {
	change_hp(generation, *this, -damage);
	m_flags.damaged = true;
}

namespace {

constexpr bool cannot_ko(Moves const move) {
	return move == Moves::False_Swipe;
}

auto handle_ohko(MutableActivePokemon defender, bool const is_enduring, Moves const move) {
	if (cannot_ko(move) or is_enduring) {
		return true;
	}
	auto const hp = get_hp(defender);
	if (hp.current() == hp.max() and get_item(defender) == Item::Focus_Sash) {
		set_item(defender, Item::None);
		return true;
	}
	return false;
}

} // namespace

auto MutableActivePokemon::direct_damage(Generation const generation, Moves const move, damage_type const damage) const -> HP::current_type {
	if (m_flags.substitute) {
		return m_flags.substitute.damage(damage);
	}
	auto const original_hp = get_hp(m_pokemon).current();
	auto const block_ohko = original_hp <= damage and handle_ohko(*this, m_flags.damage_blocker.is_enduring(), move);
	auto const applied_damage = block_ohko ?
		static_cast<HP::current_type>(original_hp - 1_bi) :
		bounded::min(damage, original_hp);
	indirect_damage(generation, applied_damage);
	m_flags.direct_damage_received = applied_damage;
	bounded::visit(m_flags.lock_in, bounded::overload(
		[=](Bide & bide) { bide.add_damage(applied_damage); },
		[](auto const &) {}
	));
	return applied_damage;
}

namespace {

auto status_can_apply(Statuses const status, Pokemon const user, ActivePokemon const target, Weather const weather, bool const uproar) {
	return
		is_clear(get_status(target)) and
		(ignores_blockers(get_ability(user)) or !blocks_status(get_ability(target), status, weather)) and
		!containers::any(target.types(), [=](Type const type) { return blocks_status(type, status); }) and
		!weather.blocks_status(status) and
		(!uproar or (status != Statuses::sleep and status != Statuses::rest));
}

constexpr auto reflected_status(Statuses const status) -> bounded::optional<Statuses> {
	switch (status) {
	case Statuses::burn:
	case Statuses::paralysis:
	case Statuses::poison:
		return status;
	case Statuses::toxic:
		return Statuses::poison;
	case Statuses::clear:
	case Statuses::freeze:
	case Statuses::sleep:
	case Statuses::rest:
		return bounded::none;
	}
}

} // namespace

auto MutableActivePokemon::apply_status(Statuses const status, MutableActivePokemon user, Weather const weather, bool const uproar) const -> void {
	BOUNDED_ASSERT_OR_ASSUME(status != Statuses::clear);
	BOUNDED_ASSERT_OR_ASSUME(status != Statuses::rest);
	if (!status_can_apply(status, user, *this, weather, uproar)) {
		return;
	}
	m_pokemon.set_status(status);
	m_flags.status.set(status);
	auto const reflected = reflected_status(status);
	if (reflected and reflects_status(get_ability(*this))) {
		apply_status_to_self(*reflected, user, weather, uproar);
	}
}

auto MutableActivePokemon::activate_pinch_item(Generation const generation) const -> void {
	// TODO: Confusion damage does not activate healing berries in Generation 5+
	auto consume = [&] { set_item(m_pokemon, Item::None); };

	auto const current_hp = hp_ratio(m_pokemon);

	auto healing_berry = [&](auto const amount) {
		auto const threshold = BOUNDED_CONDITIONAL(generation <= Generation::six, rational(1_bi, 2_bi), rational(1_bi, 4_bi));
		if (current_hp > threshold) {
			return false;
		}
		consume();
		m_pokemon.set_hp(get_hp(m_pokemon).current() + amount);
		return true;
	};

	auto confusion_berry = [&](StatNames const stat) {
		auto const amount = get_hp(m_pokemon).max() / BOUNDED_CONDITIONAL(generation <= Generation::six, 8_bi, 2_bi);
		auto const activated = healing_berry(amount);
		if (activated and lowers_stat(get_nature(m_pokemon), stat)) {
			confuse();
		}
	};

	auto stat_boost_berry = [&](StatNames const stat) {
		auto const threshold = BOUNDED_CONDITIONAL(get_ability(*this) == Ability::Gluttony, rational(1_bi, 2_bi), rational(1_bi, 4_bi));
		if (current_hp > threshold) {
			return;
		}
		consume();
		boost(stage(), stat, 1_bi);
	};

	switch (get_item(m_pokemon)) {
		case Item::Aguav_Berry:
			confusion_berry(StatNames::SPD);
			break;
		case Item::Apicot_Berry:
			stat_boost_berry(StatNames::SPD);
			break;
		case Item::Berry:
		case Item::Oran_Berry:
			healing_berry(10_bi);
			break;
		case Item::Berry_Juice:
			healing_berry(20_bi);
			break;
		case Item::Figy_Berry:
			confusion_berry(StatNames::ATK);
			break;
		case Item::Ganlon_Berry:
			stat_boost_berry(StatNames::DEF);
			break;
		case Item::Gold_Berry:
			healing_berry(30_bi);
			break;
		case Item::Iapapa_Berry:
			confusion_berry(StatNames::DEF);
			break;
		case Item::Lansat_Berry:
			// TODO: Raise CH rate
			consume();
			break;
		case Item::Liechi_Berry:
			stat_boost_berry(StatNames::ATK);
			break;
		case Item::Mago_Berry:
			confusion_berry(StatNames::SPE);
			break;
		case Item::Petaya_Berry:
			stat_boost_berry(StatNames::SPA);
			break;
		case Item::Salac_Berry:
			stat_boost_berry(StatNames::SPE);
			break;
		case Item::Sitrus_Berry:
			healing_berry(BOUNDED_CONDITIONAL(generation <= Generation::three, 30_bi, get_hp(m_pokemon).max() / 4_bi));
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
