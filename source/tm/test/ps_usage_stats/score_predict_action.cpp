// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <std_module/prelude.hpp>
#include <string_view>

#include <bounded/assert.hpp>

import tm.clients.ps.action_required;
import tm.clients.ps.battle_manager;
import tm.clients.ps.battle_message;
import tm.clients.ps.battle_response_switch;
import tm.clients.ps.in_message;
import tm.clients.ps.parsed_side;
import tm.clients.ps.slot_memory;

import tm.clients.party;

import tm.move.move_name;
import tm.move.pass;
import tm.move.selection;
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

import tm.strategy.parse_strategy;
import tm.strategy.strategy;
import tm.strategy.weighted_selection;

import tm.team_predictor.all_usage_stats;
import tm.team_predictor.team_predictor;
import tm.team_predictor.usage_stats;

import tm.generation;
import tm.get_legal_selections;
import tm.load_json_from_file;
import tm.open_file;
import tm.team;
import tm.team_is_empty;
import tm.visible_state;

import bounded;
import concurrent;
import containers;
import std_module;
import tv;

import tm.string_conversions.move_name;

namespace technicalmachine {
using namespace std::string_view_literals;
using namespace bounded::literal;
using namespace ps;
using namespace ps_usage_stats;

struct ParsedArgs {
	ThreadCount thread_count;
	std::filesystem::path input_directory;
	Strategy strategy;
};

auto parse_args(int argc, char const * const * argv) -> ParsedArgs {
	if (argc < 4) {
		throw std::runtime_error("Usage is score_predict_selection thread_count input_directory strategy");
	}
	auto const thread_count = bounded::to_integer<ThreadCount>(argv[1]);
	auto input_directory = std::filesystem::path(argv[2]);
	if (!std::filesystem::exists(input_directory)) {
		throw std::runtime_error(containers::concatenate<std::string>(input_directory.string(), " does not exist"sv));
	}
	return ParsedArgs{
		thread_count,
		std::move(input_directory),
		parse_strategy(argc - 3, argv + 3)
	};
}

constexpr auto is_input_for(Party const party) {
	return [=](PlayerInput const input) -> bool {
		return input.party == party;
	};
}

struct PredictedSelection {
	WeightedSelections predicted;
	SlotMemory slot_memory;
};

auto get_predicted_selection(
	Strategy const & strategy,
	BattleManager & battle,
	AllUsageStats const & all_usage_stats
) {
	return [&](BattleMessage const & message) -> tv::optional<PredictedSelection> {
		auto function = [&]<Generation generation>(VisibleState<generation> const & state) {
			if (team_is_empty(state.ai) or team_is_empty(state.foe)) {
				throw std::runtime_error("Tried to predict a selection with an empty team");
			}
			auto const user_team = Team<generation>(state.ai);
			auto const predicted_team = most_likely_team(all_usage_stats[generation], state.foe);
			BOUNDED_ASSERT(!team_is_empty(predicted_team));
			return strategy(
				user_team,
				get_legal_selections(user_team, predicted_team, state.environment),
				predicted_team,
				get_legal_selections(predicted_team, user_team, state.environment),
				state.environment
			).user;
		};
		auto const battle_result = tv::visit(message, tv::overload(
			[](ps::CreateBattle) -> BattleManager::Result {
				throw std::runtime_error("Should never receive CreateBattle");
			},
			[&](auto const & msg) -> BattleManager::Result {
				return battle.handle_message(msg);
			}
		));
		return tv::visit(battle_result, [&]<typename T>(T const & result) -> tv::optional<PredictedSelection> {
			if constexpr (bounded::convertible_to<T, ActionRequired>) {
				auto selection = PredictedSelection(
					tv::visit(result.state, function),
					result.slot_memory
				);
				if (selection.predicted == WeightedSelections({{pass, 1.0}})) {
					return tv::none;
				}
				return selection;
			} else {
				return tv::none;
			}
		});
	};
}

constexpr auto individual_brier_score = [](auto const & tuple) -> double {
	auto const & [evaluated, reported] = tuple;
	auto const actual = tv::visit(reported.selection, tv::overload(
		[](MoveName const move) -> Selection {
			return move;
		},
		[&](BattleResponseSwitch const response) -> Selection {
			return Switch(evaluated.slot_memory.reverse_lookup(response));
		}
	));
	auto score_prediction = [&](WeightedSelection const predicted) {
		auto const actual_probability = actual == predicted.selection ? 1.0 : 0.0;
		auto const value = predicted.weight - actual_probability;
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
		containers::sum(scores),
		weight
	);
}

auto predicted_selections(
	Strategy const & strategy,
	std::span<BattleMessage const> const battle_messages,
	BattleManager & battle,
	AllUsageStats const & all_usage_stats
) {
	return containers::remove_none(containers::transform_non_idempotent(
		std::move(battle_messages),
		get_predicted_selection(strategy, battle, all_usage_stats)
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

auto print_predicted(auto && all_predicted) {
	std::cerr << "Predicted: " << '\n';
	for (auto && predicted : all_predicted.predicted) {
		std::cerr << '\t';
		tv::visit(predicted.selection, tv::overload(
			[](Switch const switch_) { std::cerr << "Switch" << switch_.value(); },
			[](MoveName const move) { std::cerr << to_string(move); },
			[](Pass) { std::cerr << "Pass"; }
		));
		std::cerr << ": " << predicted.weight << '\n';
	}
}

auto print_actual(auto && all_actual) {
	tv::visit(all_actual.selection, tv::overload(
		[](MoveName const move) { std::cerr << "Use " << to_string(move); },
		[](BattleResponseSwitch const move) { std::cerr << "Switch " << move; }
	));
	std::cerr << '\n';
}

auto print_all(
	Strategy const & strategy,
	AllUsageStats const & all_usage_stats,
	RatedSide const & rated_side,
	std::span<BattleMessage const> const battle_messages,
	std::span<PlayerInput const> const player_inputs
) -> void {
	auto battle = BattleManager();
	battle.handle_message(rated_side.side);
	auto range = containers::zip_smallest(
		predicted_selections(strategy, battle_messages, battle, all_usage_stats),
		containers::filter(player_inputs, is_input_for(rated_side.side.party))
	);
	auto const last = containers::end(std::move(range));
	for (auto it = containers::begin(std::move(range)); it != last; ++it) {
		auto && values = *it;
		print_predicted(values[0_bi]);
		print_actual(values[1_bi]);
		std::cerr << "Score: " << individual_brier_score(values) << '\n';
		std::cerr << '\n';
	}
}

auto score_one_side_of_battle(
	Strategy const & strategy,
	AllUsageStats const & all_usage_stats,
	RatedSide const & rated_side,
	std::span<BattleMessage const> const battle_messages,
	std::span<PlayerInput const> const player_inputs
) -> WeightedScore {
	//print_all(strategy, all_usage_stats, rated_side, battle_messages, player_inputs);
	auto battle = BattleManager();
	battle.handle_message(rated_side.side);
	auto const scores = containers::vector(containers::transform(
		containers::zip_smallest(
			predicted_selections(strategy, battle_messages, battle, all_usage_stats),
			containers::filter(player_inputs, is_input_for(rated_side.side.party))
		),
		individual_brier_score
	));
	return weighted_score(scores);
}

auto score_predict_selection(ThreadCount const thread_count, std::filesystem::path const & input_directory, Strategy const & strategy) -> double {
	auto score = std::atomic<WeightedScore>();
	{
		auto const all_usage_stats = AllUsageStats();
		auto queue = concurrent::basic_blocking_queue<std::deque<std::filesystem::path>>(1000);
		auto workers = containers::dynamic_array<std::jthread>(containers::generate_n(thread_count, [&] {
			return std::jthread([&](std::stop_token token) {
				while (auto input_file = queue.pop_one(token)) {
					try {
						auto const json = load_json_from_file(*input_file);
						auto const battle_result = parse_log(json);
						if (!battle_result) {
							continue;
						}
						auto const battle_messages = battle_log_to_messages(json.at("log"));
						auto const input_log = parse_input_log(json.at("inputLog"));
						auto sides = containers::array({
							RatedSide(Party(0_bi), battle_result->side1),
							RatedSide(Party(1_bi), battle_result->side2)
						});
						auto const individual = containers::sum(containers::transform(sides, [&](RatedSide const & rated_side) {
							return score_one_side_of_battle(
								strategy,
								all_usage_stats,
								rated_side,
								battle_messages,
								input_log
							);
						}));
						auto previous = score.load();
						while (!score.compare_exchange_weak(previous, previous + individual)) {
						}
					} catch (std::exception const & ex) {
						std::cerr << "Unable to process " << input_file->string() << ": " << ex.what() << ", skipping\n";
					}
				}
			});
		}));
		for (auto file : files_in_directory(input_directory)) {
			queue.push(std::move(file));
		}
	}
	auto const result = score.load();
	return result.weight != 0.0 ? result.score / result.weight : 0.0;
}

} // namespace technicalmachine

auto main(int argc, char ** argv) -> int {
	using namespace technicalmachine;
	auto const args = parse_args(argc, argv);
	auto const result = score_predict_selection(args.thread_count, args.input_directory, args.strategy);
	std::cout << "Brier score: " << result << '\n';
	return 0;
}
