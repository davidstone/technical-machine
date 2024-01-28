// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <std_module/prelude.hpp>
#include <string_view>

import tm.clients.ps.action_required;
import tm.clients.ps.battle_manager;
import tm.clients.ps.battle_message;
import tm.clients.ps.battle_response_switch;
import tm.clients.ps.in_message;
import tm.clients.ps.parsed_side;
import tm.clients.ps.slot_memory;

import tm.clients.party;

import tm.evaluate.all_evaluate;
import tm.evaluate.action_probability;
import tm.evaluate.predict_action;

import tm.move.action;
import tm.move.move_name;
import tm.move.pass;
import tm.move.switch_;

import tm.ps_usage_stats.add_to_workers;
import tm.ps_usage_stats.battle_log_to_messages;
import tm.ps_usage_stats.battle_result;
import tm.ps_usage_stats.files_in_directory;
import tm.ps_usage_stats.parse_input_log;
import tm.ps_usage_stats.parse_log;
import tm.ps_usage_stats.rating;
import tm.ps_usage_stats.thread_count;
import tm.ps_usage_stats.worker;

import tm.team_predictor.all_usage_stats;
import tm.team_predictor.team_predictor;
import tm.team_predictor.usage_stats;

import tm.get_both_actions;
import tm.generation;
import tm.load_json_from_file;
import tm.team;
import tm.visible_state;

import bounded;
import containers;
import std_module;
import tv;

namespace technicalmachine {
using namespace std::string_view_literals;
using namespace bounded::literal;
using namespace ps;
using namespace ps_usage_stats;

struct ParsedArgs {
	ThreadCount thread_count;
	std::filesystem::path input_directory;
};

auto parse_args(int argc, char const * const * argv) -> ParsedArgs {
	if (argc != 3) {
		throw std::runtime_error("Usage is score_predict_action thread_count input_directory");
	}
	auto const thread_count = bounded::to_integer<ThreadCount>(argv[1]);
	auto input_directory = std::filesystem::path(argv[2]);
	if (!std::filesystem::exists(input_directory)) {
		throw std::runtime_error(containers::concatenate<std::string>(input_directory.string(), " does not exist"sv));
	}
	return ParsedArgs{
		thread_count,
		std::move(input_directory)
	};
}

constexpr auto is_input_for(Party const party) {
	return [=](PlayerInput const input) -> bool {
		return input.party == party;
	};
}

struct PredictedAction {
	ActionProbabilities predicted;
	SlotMemory slot_memory;
};

auto get_predicted_action(
	BattleManager & battle,
	AllUsageStats const & all_usage_stats,
	AllEvaluate const & evaluate
) {
	return [&](BattleMessage const & message) -> tv::optional<PredictedAction> {
		auto function = [&]<Generation generation>(VisibleState<generation> const & state) {
			return get_both_actions(
				Team<generation>(state.ai),
				most_likely_team(all_usage_stats[generation], state.foe),
				state.environment,
				evaluate.get<generation>()
			).predicted;
		};
		auto const battle_result = tv::visit(message, tv::overload(
			[](ps::CreateBattle) -> BattleManager::Result {
				throw std::runtime_error("Should never receive CreateBattle");
			},
			[&](auto const & msg) -> BattleManager::Result {
				return battle.handle_message(msg);
			}
		));
		return tv::visit(battle_result, [&]<typename T>(T const & result) -> tv::optional<PredictedAction> {
			if constexpr (bounded::convertible_to<T, ActionRequired>) {
				auto action = PredictedAction(
					tv::visit(result.state, function),
					result.slot_memory
				);
				if (action.predicted == ActionProbabilities({{pass, 1.0}})) {
					return tv::none;
				}
				return action;
			} else {
				return tv::none;
			}
		});
	};
}

constexpr auto individual_brier_score = [](auto const & tuple) -> double {
	auto const & [evaluated, reported] = tuple;
	auto const actual = tv::visit(reported.action, tv::overload(
		[](MoveName const move) -> Action {
			return move;
		},
		[&](BattleResponseSwitch const response) -> Action {
			return Switch(evaluated.slot_memory.reverse_lookup(response));
		}
	));
	auto score_prediction = [&](ActionProbability const predicted) {
		auto const actual_probability = actual == predicted.action ? 1.0 : 0.0;
		auto const value = predicted.probability - actual_probability;
		return value * value;
	};
	return containers::sum(containers::transform(evaluated.predicted, score_prediction));
};

struct WeightedScore {
	double score = 0.0;
	double weight = 0.0;

	friend constexpr auto operator+(WeightedScore const lhs, WeightedScore const rhs) -> WeightedScore {
		return WeightedScore(
			lhs.score + rhs.score,
			lhs.weight + rhs.weight
		);
	}
};

constexpr auto weighted_score(std::span<double const> const scores) -> WeightedScore {
	auto const weight = static_cast<double>(containers::size(scores));
	return WeightedScore(
		weight * containers::sum(scores),
		weight
	);
}

auto predicted_actions(
	std::span<BattleMessage const> const battle_messages,
	BattleManager & battle,
	AllUsageStats const & all_usage_stats,
	AllEvaluate const & evaluate
) {
	return containers::remove_none(containers::transform_non_idempotent(
		std::move(battle_messages),
		get_predicted_action(battle, all_usage_stats, evaluate)
	));
}

struct RatedSide {
	constexpr RatedSide(Party const party, BattleResult::Side const & side_):
		side(party, side_.team),
		rating(side_.rating)
	{
	}
	ParsedSide side;
	tv::optional<Rating> rating;
};

auto score_one_side_of_battle(
	AllUsageStats const & all_usage_stats,
	AllEvaluate const & evaluate,
	RatedSide const & rated_side,
	std::span<BattleMessage const> const battle_messages,
	std::span<PlayerInput const> const player_inputs
) -> WeightedScore {
	auto battle = BattleManager();
	battle.handle_message(rated_side.side);
	auto const scores = containers::vector(containers::transform(
		containers::zip(
			predicted_actions(battle_messages, battle, all_usage_stats, evaluate),
			containers::filter(player_inputs, is_input_for(rated_side.side.party))
		),
		individual_brier_score
	));
	return weighted_score(scores);
}

auto score_predict_action(ThreadCount const thread_count, std::filesystem::path const & input_directory) -> double {
	auto score = std::atomic<WeightedScore>();
	{
		auto const all_evaluate = AllEvaluate();
		auto const all_usage_stats = AllUsageStats(StatsForGeneration(stats_for_generation));
		auto workers = containers::dynamic_array(containers::generate_n(thread_count, [&] {
			return make_worker<std::filesystem::path>([&](std::filesystem::path const & input_file) {
				try {
					auto const json = load_json_from_file(input_file);
					auto const battle_result = parse_log(json);
					if (!battle_result) {
						return;
					}
					auto const battle_messages = battle_log_to_messages(json.at("log"));
					auto const input_log = parse_input_log(json.at("inputLog"));
					auto sides = containers::array({
						RatedSide(Party(0_bi), battle_result->side1),
						RatedSide(Party(1_bi), battle_result->side2)
					});
					auto const individual = containers::sum(containers::transform(sides, [&](RatedSide const & rated_side) {
						return score_one_side_of_battle(
							all_usage_stats,
							all_evaluate,
							rated_side,
							battle_messages,
							input_log
						);
					}));
					auto previous = score.load();
					while (!score.compare_exchange_weak(previous, previous + individual)) {
					}
				} catch (std::exception const & ex) {
					std::cerr << "Unable to process " << input_file.string() << ": " << ex.what() << ", skipping\n";
					return;
				}
			});
		}));
		add_to_workers(
			files_in_directory(input_directory),
			workers
		);
	}
	auto const result = score.load();
	return result.score / result.weight;
}

} // namespace technicalmachine

auto main(int argc, char ** argv) -> int {
	using namespace technicalmachine;
	auto const args = parse_args(argc, argv);
	auto const result = score_predict_action(args.thread_count, args.input_directory);
	std::cout << "Brier score: " << result << '\n';
	return 0;
}
