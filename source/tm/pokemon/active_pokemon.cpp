// Flags for the active Pokemon
// Copyright (C) 2017 David Stone
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

#include <tm/weather.hpp>

#include <tm/move/move.hpp>

namespace technicalmachine {


auto MutableActivePokemon::confuse() -> void {
	if (!blocks_confusion(get_ability(*this))) {
		m_flags.confusion.activate();
	}
}

auto MutableActivePokemon::advance_lock_in() -> void {
	bounded::visit(m_flags.lock_in, bounded::overload(
		[&](Rampage & rampage) {
			rampage.advance_one_turn();
			if (!rampage.is_active()) {
				m_flags.lock_in = std::monostate{};
				confuse();
			}
		},
		[](UproarCounter & uproar) { uproar.advance_one_turn(); },
		[](auto const &) {}
	));
}

auto MutableActivePokemon::perish_song_turn() -> void {
	bool const faints_this_turn = m_flags.perish_song.advance_one_turn_deactivated();
	if (faints_this_turn) {
		faint();
	}
}

auto MutableActivePokemon::recharge() -> bool {
	return bounded::visit(m_flags.lock_in, bounded::overload(
		[&](Recharging) { m_flags.lock_in = std::monostate{}; return true; },
		[](auto const &) { return false; }
	));
}

auto MutableActivePokemon::increase_sleep_counter(bool const awakens) -> void {
	auto & status = get_status(*this);
	auto const & ability = get_ability(*this);
	status.increase_sleep_counter(ability, awakens);
}

auto MutableActivePokemon::increment_stockpile() -> void {
	bool const increased = m_flags.stockpile.increment();
	if (increased) {
		boost_defensive(stage(*this), 1_bi);
	}
}

auto MutableActivePokemon::release_stockpile() -> bounded::integer<0, Stockpile::max> {
	auto const stages = m_flags.stockpile.release();
	boost_defensive(stage(*this), -stages);
	return stages;
}

auto MutableActivePokemon::try_to_activate_yawn(Weather const weather) -> void {
	bool const put_to_sleep = m_flags.yawn.advance_one_turn_deactivated();
	if (put_to_sleep) {
		apply(Statuses::sleep, *this, weather);
	}
}

auto MutableActivePokemon::use_bide(Pokemon & target) -> void {
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

namespace {

auto can_use_substitute(Pokemon const & pokemon) -> bool {
	auto const & hp = get_hp(pokemon);
	return hp.current() > hp.max() / 4_bi;
}

}	// namespace

auto MutableActivePokemon::use_substitute() -> void {
	if (!can_use_substitute(*this))
		return;
	auto const max_hp = get_hp(*this).max();
	indirect_damage(m_flags.substitute.create(max_hp));
}

auto MutableActivePokemon::direct_damage(damage_type const damage) -> void {
	if (m_flags.substitute) {
		m_flags.substitute.damage(damage);
	} else {
		get_hp(*this) -= damage;
		constexpr auto bide_index = bounded::detail::types<Bide>{};
		if (bounded::holds_alternative(m_flags.lock_in, bide_index)) {
			m_flags.lock_in[bide_index].add_damage(damage);
		}
	}
}


}	// namespace technicalmachine
