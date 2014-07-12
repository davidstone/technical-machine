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

#ifndef ACTIVE_POKEMON_FLAGS_HPP_
#define ACTIVE_POKEMON_FLAGS_HPP_

#include "confusion.hpp"
#include "disable.hpp"
#include "embargo.hpp"
#include "encore.hpp"
#include "heal_block.hpp"
#include "last_used_move.hpp"
#include "magnet_rise.hpp"
#include "partial_trap.hpp"
#include "perish_song.hpp"
#include "rampage.hpp"
#include "slow_start.hpp"
#include "stockpile.hpp"
#include "substitute.hpp"
#include "taunt.hpp"
#include "toxic.hpp"
#include "uproar.hpp"
#include "vanish.hpp"
#include "yawn.hpp"

#include "../random_damage.hpp"

#include "../bide/bide.hpp"

#include "../stat/stage.hpp"

#include <bounded_integer/bounded_integer.hpp>

namespace technicalmachine {

class ActivePokemonFlags {
public:
	friend auto operator==(ActivePokemonFlags const & lhs, ActivePokemonFlags const & rhs) -> bool;
	auto hash() const noexcept {
		return technicalmachine::hash(
			substitute,
			bide,
			confusion,
			disable,
			embargo,
			last_used_move,
			stage,
			aqua_ring,
			attracted,
			charged,
			is_cursed,
			defense_curled,
			destiny_bond,
			encore,
			flash_fire,
			has_focused_energy,
			fully_trapped,
			gastro_acid,
			heal_block,
			identified,
			used_imprison,
			ingrained,
			leech_seeded,
			is_loafing_turn,
			locked_on,
			magnet_rise,
			minimized,
			mud_sport,
			is_having_a_nightmare,
			partial_trap,
			perish_song,
			power_trick_is_active,
			rampage,
			is_recharging,
			slow_start,
			stockpile,
			taunt,
			is_tormented,
			toxic,
			uproar,
			vanish,
			water_sport,
			yawn
		);
	}

private:
	friend class ActivePokemon;
	
	auto reset_end_of_turn() -> void;
	auto reset_switch() -> void;
	auto reset_between_turns() -> void;

	HP::current_type damaged = 0_bi;
	Bide bide;
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
	Rampage rampage;
	RandomDamage random_damage;
	Stage stage;
	SlowStart slow_start;
	Stockpile stockpile;
	TauntCounter taunt;
	Toxic toxic;
	UproarCounter uproar;
	Vanish vanish;
	YawnCounter yawn;
	bool aqua_ring = false;
	bool attracted = false;
	// Will it wake up
	bool awakening = false;
	bool charged = false;
	bool critical_hit = false;
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
	bool missed = false;
	bool moved = false;
	bool mud_sport = false;
	bool is_having_a_nightmare = false;
	bool is_baton_passing = false;
	bool power_trick_is_active = false;
	bool is_protecting = false;
	bool is_recharging = false;
	bool is_roosting = false;
	bool shed_skin_activated = false;
	bool is_tormented = false;
	bool u_turning = false;
	bool water_sport = false;
	bool will_be_replaced = false;
};
auto operator!= (ActivePokemonFlags const & lhs, ActivePokemonFlags const & rhs) -> bool;

inline auto hash(ActivePokemonFlags const & flags) noexcept {
	return flags.hash();
}

}	// namespace technicalmachine
#endif	// ACTIVE_POKEMON_FLAGS_HPP_
