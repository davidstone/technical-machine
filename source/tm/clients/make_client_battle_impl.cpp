// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module tm.clients.make_client_battle;

import tm.clients.client_battle;
import tm.clients.check_weathers_match;
import tm.clients.teams;

import tm.move.known_move;
import tm.move.move_cures_target_status;
import tm.move.move_name;
import tm.move.move_result;
import tm.move.switch_;

import tm.pokemon.any_pokemon;
import tm.pokemon.get_hidden_power_type;
import tm.pokemon.level;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.nickname;
import tm.pokemon.species;

import tm.stat.hp;

import tm.status.status_name;

import tm.type.move_type;

import tm.ability;
import tm.any_team;
import tm.battle;
import tm.end_of_turn_flags;
import tm.gender;
import tm.generation;
import tm.generation_generic;
import tm.item;
import tm.team;
import tm.visible_hp;
import tm.visible_state;
import tm.weather;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;
using namespace std::string_view_literals;

constexpr auto is_fainted(any_team auto const & team) -> bool {
	return team.pokemon().hp().current() == 0_bi;
}

template<any_team TeamType>
constexpr auto is_done_moving(TeamType const & team) -> bool {
	auto const pokemon = team.pokemon();
	auto const last_move = pokemon.last_used_move();
	return
		last_move.moved_this_turn() and
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
		return m_battle.state();
	}

	auto ai_has(Species const species, Nickname const nickname, Level const level, Gender const gender) const -> TeamIndex final {
		return m_battle.ai_has(species, nickname, level, gender);
	}
	auto foe_has(
		Species const species,
		Nickname const nickname,
		Level const level,
		Gender const gender,
		MaxVisibleHP const max_hp
	) & -> TeamIndex final {
		return m_battle.foe_has(species, nickname, level, gender, max_hp);
	}

	auto active_has(bool const is_ai, MoveName const move_name) & -> void final {
		m_battle.active_has(is_ai, move_name);
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
		auto const ai_is_done_moving = is_done_moving(m_battle.ai());
		auto const foe_is_done_moving = is_done_moving(m_battle.foe());
		switch (generation_) {
			case Generation::one:
			case Generation::three:
				return
					(ai_is_done_moving and (foe_is_done_moving or foe_is_fainted() or ai_is_fainted())) or
					(foe_is_done_moving and (ai_is_fainted() or foe_is_fainted()));
			case Generation::two:
				return ai_is_done_moving and foe_is_done_moving;
			default:
				return
					(ai_is_done_moving and (foe_is_done_moving or foe_is_fainted())) or
					(foe_is_done_moving and ai_is_fainted());
		}
	}

	auto ai_is_fainted() const -> bool final {
		return is_fainted(m_battle.ai());
	}
	auto foe_is_fainted() const -> bool final {
		return is_fainted(m_battle.foe());
	}

	auto use_move(bool const ai_is_user, MoveResult const move_result, bool const status_clears) & -> void final {
		m_battle.use_move(ai_is_user, move_result, status_clears);
	}

	auto use_switch(bool const ai_is_user, Switch const switch_) & -> void final {
		m_battle.use_switch(ai_is_user, switch_);
	}

	auto hit_self_in_confusion(bool ai_is_user, VisibleHP const damage) & -> void final {
		m_battle.hit_self_in_confusion(ai_is_user, damage);
	}

	auto end_disable(bool ai_is_user) & -> void final {
		m_battle.end_disable(ai_is_user);
	}

	auto end_turn(bool const ai_went_first, EndOfTurnFlags const first_flags, EndOfTurnFlags const last_flags) & -> void final {
		m_battle.end_turn(ai_went_first, first_flags, last_flags);
	}

	auto cures_target_status(bool const is_ai, MoveName const move_name) const -> bool final {
		auto function = [&](auto const & target) {
			auto const pokemon = target.pokemon();
			return move_cures_target_status(
				generation_,
				move_name,
				get_hidden_power_type(pokemon),
				pokemon.status().name()
			);
		};
		if (is_ai) {
			return function(m_battle.ai());
		} else {
			return function(m_battle.foe());
		}
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
		check_weathers_match(weather, m_battle.environment().actual_weather());
	}

private:
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
