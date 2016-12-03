// Flags for the active Pokemon
// Copyright (C) 2016 David Stone
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

#include "../operators.hpp"
#include "../random_damage.hpp"

#include "../bide/bide.hpp"

#include "../move/moves.hpp"

#include "../stat/stage.hpp"

#include <bounded/integer.hpp>

namespace technicalmachine {

struct ActivePokemon;
struct MutableActivePokemon;

// I use a macro here because I rely on a conversion operator. Friend functions
// only declared in a class body are not found by lookup rules in that case. The
// macro solution seemed better than duplicating all of this by hand.

#define TECHNICALMACHINE_ACTIVE_POKEMON_FRIEND_FUNCTIONS \
	friend auto last_used_move(ActivePokemon pokemon) -> LastUsedMove; \
	friend auto substitute(ActivePokemon pokemon) -> Substitute const &; \
	friend auto aqua_ring_is_active(ActivePokemon pokemon) -> bool; \
	friend auto is_baton_passing(ActivePokemon pokemon) -> bool; \
	friend auto cannot_be_koed(ActivePokemon pokemon) -> bool; \
	friend auto charge_boosted(ActivePokemon pokemon) -> bool; \
	friend auto is_confused(ActivePokemon pokemon) -> bool; \
	friend auto is_cursed(ActivePokemon pokemon) -> bool; \
	friend auto defense_curled(ActivePokemon pokemon) -> bool; \
	/* Requires that move is actually one of this Pokemon's moves */ \
	friend auto is_disabled(ActivePokemon pokemon, Moves move) -> bool; \
	friend auto is_encored(ActivePokemon pokemon) -> bool; \
	/* This function should be used instead of checking if hp == 0 to handle */ \
	/* messages being sent about multiple Pokemon fainting in one turn. */ \
	/* Using this function will allow TM to correctly update an entire turn */ \
	/* from a message. */ \
	friend auto is_fainted(ActivePokemon pokemon) -> bool; \
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
	friend auto is_recharging(ActivePokemon pokemon) -> bool; \
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
	friend auto toxic_ratio(ActivePokemon pokemon) -> decltype(std::declval<Toxic>().ratio_drained()); \
	friend auto vanish_doubles_power(ActivePokemon pokemon, Moves move_name) -> bool; \
	friend auto is_locked_in_to_bide(ActivePokemon pokemon) -> bool; \
	friend auto damaged(ActivePokemon pokemon) -> bounded::integer<0, HP::max_value>; \
	friend auto random_damage_multiplier(ActivePokemon pokemon) -> decltype(std::declval<RandomDamage>()())

struct ActivePokemonFlags {
	TECHNICALMACHINE_ACTIVE_POKEMON_FRIEND_FUNCTIONS;
	friend auto stage(MutableActivePokemon pokemon) -> Stage &;

	auto reset_between_turns() -> void;
	auto reset_end_of_turn() -> void;
	auto reset_switch() -> void;

	friend auto operator==(ActivePokemonFlags const & lhs, ActivePokemonFlags const & rhs) -> bool;

private:
	friend struct ActivePokemon;
	friend struct MutableActivePokemon;
	
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
	bool charged = false;
	bool is_cursed = false;
	bool defense_curled = false;
	bool destiny_bond = false;
	bool enduring = false;
	bool is_fainted = false;
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
	bool is_baton_passing = false;
	bool power_trick_is_active = false;
	bool is_protecting = false;
	bool is_recharging = false;
	bool is_roosting = false;
	bool is_tormented = false;
	bool u_turning = false;
	bool water_sport = false;
};

}	// namespace technicalmachine
