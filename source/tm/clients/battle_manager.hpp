// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/clients/battle_result.hpp>
#include <tm/clients/move_result.hpp>

#include <tm/evaluate/analysis_logger.hpp>
#include <tm/evaluate/depth.hpp>
#include <tm/evaluate/evaluate.hpp>

#include <tm/generation_generic.hpp>
#include <tm/known_team.hpp>
#include <tm/seen_team.hpp>

#include <fstream>
#include <memory>
#include <random>

namespace technicalmachine {

struct UsageStats;

template<Generation generation>
struct Teams {
	KnownTeam<generation> ai;
	SeenTeam<generation> foe;
};

using TurnCount = bounded::integer<0, 1'000'000'000>;

// BattleManager is intended to be used in implementing clients. The goal is for
// there to be no undefined behavior when calling any of these functions to
// ensure we have a secure interface with the outside world. All of these
// functions will throw an exception if given data that is inconsistent overall.
struct BattleManager {
	virtual ~BattleManager();

	virtual auto generation() const -> Generation = 0;
	virtual auto random_engine() & -> std::mt19937 & = 0;
	virtual auto move_index(MoveName) const -> containers::index_type<RegularMoves> = 0;

	// Returns the switch required to bring in this Pokemon
	virtual auto ai_has(Species, containers::string nickname, Level, Gender) & -> MoveName = 0;
	virtual auto foe_has(Species, containers::string nickname, Level, Gender) & -> MoveName = 0;

	virtual auto active_has(bool is_ai, MoveName) & -> void = 0;
	virtual auto active_has(bool is_ai, Ability) & -> void = 0;
	virtual auto replacement_has(bool is_ai, TeamIndex, Ability) & -> void = 0;
	virtual auto active_has(bool is_ai, Item) & -> void = 0;
	virtual auto replacement_has(bool is_ai, TeamIndex, Item) & -> void = 0;

	virtual auto is_end_of_turn() const -> bool = 0;
	virtual auto ai_is_fainted() const -> bool = 0;
	virtual auto foe_is_fainted() const -> bool = 0;
	virtual auto ai_is_on_last_pokemon() const -> bool = 0;

	virtual auto begin_turn(TurnCount) & -> void = 0;
	virtual auto end_turn(bool ai_went_first, EndOfTurnFlags first_flags, EndOfTurnFlags last_flags) & -> void = 0;
	virtual auto use_move(bool ai_is_user, MoveResult, bool user_status_was_cleared) & -> void = 0;

	// TODO: Delete this function
	virtual auto cures_target_status(bool is_ai, MoveName) -> bool = 0;

	virtual auto determine_action() & -> MoveName = 0;

	virtual auto complete(Result) & -> void = 0;
	virtual auto completed() const -> bool = 0;

	// For a correct implementation of a correct protocol, these functions have
	// no effect.

	virtual auto correct_hp(bool is_ai, VisibleHP, TeamIndex) & -> void = 0;
	virtual auto correct_hp(bool is_ai, VisibleHP) & -> void = 0;

	virtual auto correct_status(bool is_ai, StatusName, TeamIndex) & -> void = 0;
	virtual auto correct_status(bool is_ai, StatusName) & -> void = 0;

	virtual auto weather_is(NormalWeather) const -> void = 0;
};

template<Generation generation>
struct BattleManagerInputs {
	Teams<generation> teams;
	Evaluate<generation> evaluate;
};
auto make_battle_manager(
	AnalysisLogger analysis_logger,
	UsageStats const & usage_stats,
	GenerationGeneric<BattleManagerInputs> inputs,
	DepthValues const depth,
	std::mt19937 random_engine,
	bool log_foe_teams
) -> std::unique_ptr<BattleManager>;

} // namespace technicalmachine
