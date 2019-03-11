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

#include <tm/weather.hpp>

#include <tm/move/move.hpp>

namespace technicalmachine {

auto ActivePokemonFlags::reset_end_of_turn() -> void {
	damaged = false;
	direct_damage_received = 0_bi;
	enduring = false;
	flinched = false;
	moved = false;
	me_first_is_active = false;
	is_loafing_turn = !is_loafing_turn;
	is_protecting = false;
	is_fully_paralyzed = false;
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
	is_having_a_nightmare = false;
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

namespace {

auto can_use_substitute(Pokemon const & pokemon) -> bool {
	auto const & hp = get_hp(pokemon);
	return hp.current() > hp.max() / 4_bi;
}

}	// namespace

auto MutableActivePokemon::use_substitute() const -> void {
	if (!can_use_substitute(*this))
		return;
	auto const max_hp = get_hp(*this).max();
	indirect_damage(m_flags.substitute.create(max_hp));
}

}	// namespace technicalmachine
