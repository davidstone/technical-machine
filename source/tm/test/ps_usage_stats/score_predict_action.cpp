// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <std_module/prelude.hpp>
#include <iostream>
#include <string_view>

import tm.clients.ps.battle_manager;
import tm.clients.ps.battle_message_result;
import tm.clients.ps.in_message;

import tm.clients.party;

import tm.evaluate.all_evaluate;
import tm.evaluate.analysis_logger;
import tm.evaluate.depth;
import tm.evaluate.move_probability;
import tm.evaluate.predict_action;
import tm.evaluate.state;

import tm.move.move_name;

import tm.ps_usage_stats.add_to_workers;
import tm.ps_usage_stats.battle_result;
import tm.ps_usage_stats.files_in_directory;
import tm.ps_usage_stats.parse_input_log;
import tm.ps_usage_stats.parse_log;
import tm.ps_usage_stats.rating;
import tm.ps_usage_stats.thread_count;
import tm.ps_usage_stats.worker;

import tm.team_predictor.all_usage_stats;
import tm.team_predictor.usage_stats;

import tm.get_both_actions;
import tm.generation;
import tm.generation_generic;
import tm.load_json_from_file;
import tm.team;

import bounded;
import containers;
import std_module;
import tv;

import tm.test.usage_bytes;
import tm.string_conversions.move_name;

#if 0

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

constexpr auto actions_for(std::span<PlayerInput const> player_inputs, BattleManager const & battle, Party const party) {
	return containers::transform(
		containers::filter(std::move(player_inputs), is_input_for(party)),
		[&](PlayerInput const input) -> MoveName {
			return tv::visit(input.action, tv::overload(
				[](MoveName const move) -> MoveName {
					std::cerr << "Input: move " << to_string(move) << '\n';
					return move;
				},
				[&](BattleResponseSwitch const response) -> MoveName {
					std::cerr << "Input: switch " << response << '\n';
					return battle.switch_index_to_switch(response);
				}
			));
		}
	);
}

auto get_predicted_action(BattleManager & battle, AllEvaluate const evaluate) {
	return [&, evaluate](InMessage const message) -> tv::optional<MoveProbabilities> {
		auto function = [=]<Generation generation>(State<generation> const & state) {
			return get_both_actions(
				state.ai,
				state.foe,
				state.environment,
				evaluate.get<generation>()
			).predicted;
		};
		auto const battle_result = battle.handle_message(message);
		return tv::visit(battle_result, tv::overload(
			[&](bounded::bounded_integer auto) -> tv::optional<MoveProbabilities> {
				return tv::visit(battle.predicted_state(), function);
			},
			[](auto) -> tv::optional<MoveProbabilities> {
				return tv::none;
			}
		));
	};
}

constexpr auto individual_brier_score = [](auto const & tuple) -> double {
	auto const & [predicted, actual] = tuple;
	auto score_prediction = [&](MoveProbability const move) {
		auto const is_correct = actual == move.name;
		auto const value = is_correct ? 1.0 - move.probability : move.probability;
		return value * value;
	};
	return containers::sum(containers::transform(predicted, score_prediction));
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
	std::span<std::string_view const> battle_log,
	BattleManager & battle,
	AllEvaluate const evaluate
) {
	return containers::remove_none(containers::transform(
		containers::transform(std::move(battle_log), bounded::construct<InMessage>),
		get_predicted_action(battle, evaluate)
	));
}

struct Side {
	constexpr explicit Side(Party const party_, BattleResult::Side const & side):
		party(party_),
		team(side.team),
		rating(side.rating)
	{
	}
	Party party;
	GenerationGeneric<Team> team;
	tv::optional<Rating> rating;
};

auto score_one_side_of_battle(
	AllEvaluate const evaluate,
	UsageStats const & usage_stats,
	Side const & side,
	std::span<std::string_view const> const battle_log,
	std::span<PlayerInput const> player_inputs
) -> WeightedScore {
	// This does a bunch of extra work due to searching to a depth of 1
	auto battle = BattleManager(
		tv::visit(side.team, []<Generation generation>(Team<generation> team) {
			return GenerationGeneric<KnownTeam>(KnownTeam<generation>(std::move(team)));
		}),
		side.party,
		evaluate,
		usage_stats,
		Depth(1_bi, 0_bi),
		AnalysisLogger::none()
	);

	// I don't especially like that this code depends on side effects of
	// advancing through `predicted_actions` so that `actions_for` will get the
	// right data at the right time.
	auto const scores = containers::vector(containers::transform(
		containers::zip(
			predicted_actions(battle_log, battle, evaluate),
			actions_for(player_inputs, battle, side.party)
		),
		individual_brier_score
	));
	return weighted_score(scores);
}

auto parse_battle_log(nlohmann::json const & json) -> containers::vector<std::string_view> {
	return containers::vector(containers::transform(json, [](nlohmann::json const & element) {
		return element.get<std::string_view>();
	}));
}

constexpr auto for_generation = [](Generation const generation) {
	return bytes_to_usage_stats(smallest_team_bytes(generation));
};


auto score_predict_action(ThreadCount const thread_count, std::filesystem::path const & input_directory) -> double {
	auto score = std::atomic<WeightedScore>();
	{
		auto const all_evaluate = AllEvaluate();
		auto const all_usage_stats = AllUsageStats(StatsForGeneration(for_generation));
		auto workers = containers::dynamic_array(containers::generate_n(thread_count, [&] {
			return make_worker<std::filesystem::path>([&](std::filesystem::path const & input_file) {
				try {
					auto const json = load_json_from_file(input_file);
					auto const battle_result = parse_log(json);
					if (!battle_result) {
						return;
					}
					auto const generation = get_generation(battle_result->side1.team);
					auto const battle_log = parse_battle_log(json.at("log"));
					auto const input_log = parse_input_log(json.at("inputLog"));
					auto sides = containers::array({
						Side(Party(0_bi), battle_result->side1),
						Side(Party(1_bi), battle_result->side2)
					});
					auto const individual = containers::sum(containers::transform(sides, [&](Side const & side) {
						return score_one_side_of_battle(
							all_evaluate,
							all_usage_stats[generation],
							side,
							battle_log,
							input_log
						);
					}));
					auto previous = score.load();
					while (!score.compare_exchange_weak(previous, previous + individual)) {
					}
				} catch (std::exception const & ex) {
					throw std::runtime_error(containers::concatenate<std::string>(
						"Error parsing "sv,
						input_file.string(),
						": "sv,
						std::string_view(ex.what())
					));
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

#endif

auto main(int argc, char ** argv) -> int {
	static_cast<void>(argc);
	static_cast<void>(argv);
	#if 0
	using namespace technicalmachine;
	auto const args = parse_args(argc, argv);
	auto const result = score_predict_action(args.thread_count, args.input_directory);
	std::cout << "Brier score: " << result << '\n';
	#endif
	return 0;
}
