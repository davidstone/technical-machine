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
#include <bounded/unreachable.hpp>

#include <containers/algorithms/all_any_none.hpp>

namespace technicalmachine {

template struct ActivePokemonImpl<true>;
template struct ActivePokemonImpl<false>;

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

namespace {

auto can_use_substitute(Pokemon const & pokemon) -> bool {
	auto const & hp = pokemon.hp();
	return hp.current() > hp.max() / 4_bi;
}

}	// namespace

auto MutableActivePokemon::use_substitute(Generation const generation, Weather const weather) const -> void {
	if (!can_use_substitute(m_pokemon)) {
		return;
	}
	indirect_damage(generation, weather, m_flags.substitute.create(hp().max()));
}

auto MutableActivePokemon::switch_out() const -> void {
	if (clears_status_on_switch(ability())) {
		clear_status();
	}
	// TODO: remove some of these when the foe switches, too
	if (!m_flags.last_used_move.is_baton_passing()) {
		m_flags.aqua_ring = false;
		m_flags.confusion = {};
		m_flags.is_cursed = false;
		m_flags.embargo = {};
		m_flags.has_focused_energy = false;
		m_flags.gastro_acid = false;
		m_flags.ingrained = false;
		m_flags.leech_seeded = false;
		m_flags.magnet_rise = {};
		m_flags.perish_song = {};
		m_flags.power_trick_is_active = false;
		m_flags.stage = {};
		m_flags.substitute = {};
	}
	m_flags.attracted = false;
	m_flags.charged = false;
	m_flags.defense_curled = false;
	m_flags.disable = Disable{};
	m_flags.encore = {};
	m_flags.flash_fire = false;
	m_flags.flinched = false;
	m_flags.fully_trapped = false;
	m_flags.heal_block = {};
	m_flags.identified = false;
	m_flags.used_imprison = false;
	m_flags.last_used_move = {};
	m_flags.is_loafing_turn = true;
	m_flags.minimized = false;
	m_flags.me_first_is_active = false;
	m_flags.mud_sport = false;
	m_flags.partial_trap = {};
	m_flags.slow_start = {};
	m_flags.stockpile = {};
	m_flags.is_tormented = false;
	m_flags.unburdened = false;
	m_flags.water_sport = false;
	m_flags.taunt = {};
	m_flags.yawn = {};
}

auto MutableActivePokemon::use_bide(Generation const generation, MutableActivePokemon target, Weather const weather) const -> void {
	if (auto const damage = m_flags.last_used_move.use_bide()) {
		change_hp(generation, target, weather, -*damage * 2_bi);
	}
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
	auto const hp = defender.hp();
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

auto MutableActivePokemon::direct_damage(Generation const generation, Moves const move, MutableActivePokemon user, Weather const weather, damage_type const damage) const -> HP::current_type {
	auto const interaction = substitute_interaction(generation, move);
	BOUNDED_ASSERT(!m_flags.substitute or interaction != Substitute::causes_failure);
	if (m_flags.substitute and interaction == Substitute::absorbs) {
		return m_flags.substitute.damage(damage);
	}
	auto const original_hp = hp().current();
	auto const block_ko = original_hp <= damage and handle_ko(generation, *this, m_flags.last_used_move.is_enduring(), move, weather);
	auto const applied_damage = block_ko ?
		static_cast<HP::current_type>(original_hp - 1_bi) :
		bounded::min(damage, original_hp);

	indirect_damage(generation, weather, applied_damage);
	m_flags.direct_damage_received = applied_damage;
	m_flags.last_used_move.direct_damage(applied_damage);

	// TODO: Resolve ties properly
	if (m_flags.last_used_move.is_destiny_bonded() and applied_damage == original_hp) {
		user.set_hp(generation, weather, 0_bi);
	}
	return applied_damage;
}

namespace {

auto status_can_apply(Statuses const status, ActivePokemon const user, ActivePokemon const target, Weather const weather, bool const uproar) {
	return
		is_clear(target.status()) and
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
	if (other_is_uproaring) {
		return;
	}
	if (generation >= Generation::three and is_sleeping(m_pokemon.status())) {
		return;
	}
	if (blocks_status(ability(), ability(), Statuses::rest, weather)) {
		return;
	}
	auto const active_hp = hp();
	if (active_hp.current() == active_hp.max() or healing_move_fails_in_generation_1(active_hp)) {
		return;
	}
	m_pokemon.set_hp(active_hp.max());
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
		m_pokemon.set_hp(hp().current() + amount);
		return true;
	};

	auto confusion_berry = [&](RegularStat const stat) {
		auto const amount = hp().max() / BOUNDED_CONDITIONAL(generation <= Generation::six, 8_bi, 2_bi);
		auto const threshold = generation <= Generation::six ? rational(1_bi, 2_bi) : quarter_threshold();
		auto const activated = healing_berry(threshold, amount);
		if (activated and lowers_stat(m_pokemon.nature(), stat)) {
			confuse(generation, weather);
		}
	};

	auto stat_boost_berry = [&](BoostableStat const stat) {
		if (current_hp > quarter_threshold()) {
			return;
		}
		consume();
		stage()[stat] += 1_bi;
	};

	switch (item(generation, weather)) {
		case Item::Aguav_Berry:
			confusion_berry(RegularStat::spd);
			break;
		case Item::Apicot_Berry:
			stat_boost_berry(BoostableStat::spd);
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
			confusion_berry(RegularStat::atk);
			break;
		case Item::Ganlon_Berry:
			stat_boost_berry(BoostableStat::def);
			break;
		case Item::Gold_Berry:
			healing_berry(rational(1_bi, 2_bi), 30_bi);
			break;
		case Item::Iapapa_Berry:
			confusion_berry(RegularStat::def);
			break;
		case Item::Lansat_Berry:
			consume();
			break;
		case Item::Liechi_Berry:
			stat_boost_berry(BoostableStat::atk);
			break;
		case Item::Mago_Berry:
			confusion_berry(RegularStat::spe);
			break;
		case Item::Micle_Berry:
			consume();
			break;
		case Item::Petaya_Berry:
			stat_boost_berry(BoostableStat::spa);
			break;
		case Item::Salac_Berry:
			stat_boost_berry(BoostableStat::spe);
			break;
		case Item::Sitrus_Berry:
			healing_berry(
				rational(1_bi, 2_bi),
				BOUNDED_CONDITIONAL(generation <= Generation::three, 30_bi, hp().max() / 4_bi)
			);
			break;
		case Item::Starf_Berry:
			// TODO: Raise Atk, Def, SpA, SpD, or Spe +2
			consume();
			break;
		case Item::Wiki_Berry:
			confusion_berry(RegularStat::spa);
			break;
		default:
			break;
	}
}

}	// namespace technicalmachine
