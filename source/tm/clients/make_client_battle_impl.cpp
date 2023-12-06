// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string_view>

module tm.clients.make_client_battle;

import tm.clients.client_battle;
import tm.clients.check_weathers_match;
import tm.clients.move_result;
import tm.clients.teams;
import tm.clients.to_used_move;
import tm.clients.visible_damage_to_actual_damage;
import tm.clients.visible_state;

import tm.move.causes_recoil;
import tm.move.known_move;
import tm.move.move;
import tm.move.move_cures_target_status;
import tm.move.move_name;

import tm.pokemon.get_hidden_power_type;
import tm.pokemon.level;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.species;

import tm.stat.hp;

import tm.status.status_name;

import tm.type.effectiveness;
import tm.type.move_type;

import tm.ability;
import tm.ability_blocks_move;
import tm.any_team;
import tm.battle;
import tm.end_of_turn_flags;
import tm.gender;
import tm.generation;
import tm.generation_generic;
import tm.item;
import tm.team;
import tm.visible_hp;
import tm.weather;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;
using namespace std::string_view_literals;

constexpr auto is_done_moving(any_team auto const & team) -> bool {
	auto const pokemon = team.pokemon();
	auto const is_fainted = pokemon.hp().current() == 0_bi;
	if (is_fainted) {
		return true;
	}
	auto const last_move = pokemon.last_used_move();
	auto const started_moving = last_move.moved_this_turn();
	return
		started_moving and
		(!last_move.is_delayed_switching() or team.size() == 1_bi);
}

template<Generation generation_>
struct ClientBattleImpl final : ClientBattle {
	ClientBattleImpl(KnownTeam<generation_> ai, SeenTeam<generation_> foe):
		m_battle(
			std::move(ai),
			std::move(foe)
		)
	{
	}

	auto generation() const -> Generation final {
		return generation_;
	}
	auto state() const -> GenerationGeneric<VisibleState> final {
		return VisibleState<generation_>(
			m_battle.ai(),
			m_battle.foe(),
			m_battle.environment()
		);
	}

	auto ai_has(Species const species, std::string_view nickname, Level const level, Gender const gender) & -> TeamIndex final {
		return m_battle.find_ai_pokemon(species, nickname, level, gender);
	}
	auto foe_has(Species const species, std::string_view nickname, Level const level, Gender const gender) & -> TeamIndex final {
		return m_battle.find_or_add_foe_pokemon(species, nickname, level, gender);
	}

	auto active_has(bool const is_ai, MoveName const move_name) & -> void final {
		m_battle.add_move(is_ai, move_name);
	}

	auto active_has(bool const is_ai, Ability const ability) & -> void final {
		m_battle.active_has(is_ai, ability);
	}
	auto replacement_has(bool const is_ai, TeamIndex const index, Ability const ability) & -> void final {
		m_battle.replacement_has(is_ai, index, ability);
	}

	auto active_has(bool const is_ai, Item const item) & -> void final {
		m_battle.active_has(is_ai, item);
	}
	auto replacement_has(bool const is_ai, TeamIndex const index, Item const item) & -> void final {
		m_battle.replacement_has(is_ai, index, item);
	}

	auto is_end_of_turn() const -> bool final {
		return
			is_done_moving(m_battle.ai()) and
			is_done_moving(m_battle.foe());
	}

	auto ai_is_fainted() const -> bool final {
		return m_battle.ai().pokemon().hp().current() == 0_bi;
	}
	auto foe_is_fainted() const -> bool final {
		return m_battle.foe().pokemon().hp().current() == 0_bi;
	}

	auto ai_is_on_last_pokemon() const -> bool final {
		return m_battle.ai().size() == 1_bi;
	}

	auto first_turn(bool const ai_went_first) & -> void final {
		m_battle.first_turn(ai_went_first);
	}
	auto end_turn(bool const ai_went_first, EndOfTurnFlags const first_flags, EndOfTurnFlags const last_flags) & -> void final {
		m_battle.handle_end_turn(ai_went_first, first_flags, last_flags);
	}

	auto use_move(bool const ai_is_user, MoveResult const move_result, bool const status_clears) & -> void final {
		// TODO: Handle the other states better
		apply_to_teams(ai_is_user, [&](auto const & user_team, auto const & other_team) {
			auto const move = tv::visit(move_result, tv::overload(
				[](Used const used) { return used; },
				[&](Recharging) { return Used(user_team.pokemon().last_used_move().name()); },
				[](auto) { return Used(MoveName::Struggle); }
			));
			active_has(ai_is_user, move.selected);
			if (move.selected == MoveName::Sleep_Talk) {
				active_has(ai_is_user, move.executed);
			}
			auto const user_hidden_power_type = get_hidden_power_type(user_team.pokemon());
			auto const type = move_type(generation_, move.executed, user_hidden_power_type);
			auto const other_pokemon = other_team.pokemon();
			auto const ability_blocks_recoil =
				causes_recoil(move.executed) and
				!move.recoil and
				!Effectiveness(generation_, type, other_pokemon.types()).has_no_effect() and
				!ability_blocks_move(
					generation_,
					other_pokemon.ability(),
					KnownMove{
						move.executed,
						type
					},
					other_pokemon.status().name(),
					other_pokemon.types()
				);
			if (ability_blocks_recoil) {
				// TODO: This could also be Magic Guard
				active_has(ai_is_user, Ability::Rock_Head);
			}
			
			auto const damaged_is_ai = !ai_is_user xor (move.executed == MoveName::Hit_Self);
			auto const old_hp = target_hp(damaged_is_ai);
			auto const damage = visible_damage_to_actual_damage(
				move.damage,
				damaged_is_ai,
				old_hp,
				other_pokemon.substitute()
			);
			
			m_battle.handle_use_move(
				to_used_move(move, user_team, other_team, m_battle.environment()),
				status_clears,
				move_result.index() == bounded::type<FullyParalyzed>,
				damage
			);
		});
	}

	auto cures_target_status(bool const is_ai, MoveName const move_name) -> bool final {
		return apply_to_team(is_ai, [&](auto const & target) {
			auto const pokemon = target.pokemon();
			return move_cures_target_status(
				generation_,
				move_name,
				get_hidden_power_type(pokemon),
				pokemon.status().name()
			);
		});
	}

	auto correct_hp(bool const is_ai, VisibleHP const visible_hp, TeamIndex const team_index) & -> void final {
		m_battle.correct_hp(is_ai, visible_hp, team_index);
	}
	auto correct_hp(bool const is_ai, VisibleHP const visible_hp) & -> void final {
		m_battle.correct_hp(is_ai, visible_hp);
	}

	auto correct_status(bool const is_ai, StatusName const status, TeamIndex const team_index) & -> void final {
		m_battle.correct_status(is_ai, status, team_index);
	}
	auto correct_status(bool const is_ai, StatusName const status) & -> void final {
		m_battle.correct_status(is_ai, status);
	}

	auto weather_is(Weather const weather) const -> void final {
		check_weathers_match(weather, m_battle.environment());
	}

private:
	auto apply_to_teams(bool const is_ai_, auto function) const -> decltype(auto) {
		if (is_ai_) {
			return function(m_battle.ai(), m_battle.foe());
		} else {
			return function(m_battle.foe(), m_battle.ai());
		}
	}
	auto apply_to_team(bool const is_ai_, auto function) const -> decltype(auto) {
		if (is_ai_) {
			return function(m_battle.ai());
		} else {
			return function(m_battle.foe());
		}
	}

	auto target_hp(bool const damaged_is_ai) const -> HP {
		return damaged_is_ai ?
			m_battle.ai().pokemon().hp() :
			m_battle.foe().pokemon().hp();
	}

	Battle<generation_> m_battle;
};

auto make_client_battle(GenerationGeneric<Teams> generic_teams) -> std::unique_ptr<ClientBattle> {
	return tv::visit(
		std::move(generic_teams),
		[]<Generation generation>(Teams<generation> && teams) -> std::unique_ptr<ClientBattle> {
		return std::make_unique<ClientBattleImpl<generation>>(
			std::move(teams).ai,
			std::move(teams).foe
		);
	});
}

} // namespace technicalmachine
