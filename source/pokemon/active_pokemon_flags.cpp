// Flags for the active Pokemon
// Copyright (C) 2014 David Stone
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

#include "active_pokemon_flags.hpp"

namespace technicalmachine {

auto ActivePokemonFlags::reset_end_of_turn() -> void {
	damaged = 0_bi;
	enduring = false;
	flinched = false;
	moved = false;
	me_first_is_active = false;
	is_loafing_turn = !is_loafing_turn;
	is_protecting = false;
}

auto ActivePokemonFlags::reset_switch() -> void {
	// TODO: remove some of these when the foe switches, too
	if (!is_baton_passing) {
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
	is_baton_passing = false;
	rampage = {};
	is_roosting = false;
	slow_start = {};
	stockpile = {};
	is_tormented = false;
	u_turning = false;
	water_sport = false;
	bide = {};
	damaged = 0_bi;
	taunt = {};
	toxic = {};
	uproar = {};
	// Whirlwind can hit Flying Pokemon, so it's possible to switch while
	// vanished. Therefore, we need to reset it.
	vanish = {};
	yawn = {};
	will_be_replaced = false;
}

auto ActivePokemonFlags::reset_between_turns() -> void {
	critical_hit = false;
	is_fully_paralyzed = false;
	confusion.end_of_turn_reset();
	missed = false;
}

auto operator== (ActivePokemonFlags const & lhs, ActivePokemonFlags const & rhs) -> bool {
	return
		lhs.aqua_ring == rhs.aqua_ring and
		lhs.attracted == rhs.attracted and
		lhs.bide == rhs.bide and
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
		lhs.rampage == rhs.rampage and
		lhs.slow_start == rhs.slow_start and
		lhs.stage == rhs.stage and
		lhs.stockpile == rhs.stockpile and
		lhs.taunt == rhs.taunt and
		lhs.is_tormented == rhs.is_tormented and
		lhs.toxic == rhs.toxic and
		lhs.uproar == rhs.uproar and
		lhs.vanish == rhs.vanish and
		lhs.water_sport == rhs.water_sport and
		lhs.yawn == rhs.yawn;
}

auto operator!= (ActivePokemonFlags const & lhs, ActivePokemonFlags const & rhs) -> bool {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
