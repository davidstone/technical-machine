// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

import tm.clients.ps.action_required;
import tm.clients.ps.battle_init_message;
import tm.clients.ps.battle_manager;
import tm.clients.ps.battle_response_switch;
import tm.clients.ps.event_block;
import tm.clients.ps.parsed_request;
import tm.clients.ps.parsed_side;
import tm.clients.ps.parsed_team;
import tm.clients.ps.slot_memory;

import tm.clients.party;

import tm.move.legal_selections;
import tm.move.move;
import tm.move.move_name;
import tm.move.move_names;
import tm.move.pass;

import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.pokemon;
import tm.pokemon.species;

import tm.ps_usage_stats.battle_log_to_messages;
import tm.ps_usage_stats.battle_result;
import tm.ps_usage_stats.for_each_log;
import tm.ps_usage_stats.rated_side;
import tm.ps_usage_stats.rating;
import tm.ps_usage_stats.thread_count;

import tm.generation;
import tm.get_legal_selections;
import tm.open_file;
import tm.team;
import tm.to_index;
import tm.usage_for;
import tm.visible_state;
import tm.write_bytes;

import bounded;
import concurrent;
import containers;
import std_module;
import tv;

namespace technicalmachine::ps_usage_stats {
using namespace std::string_view_literals;
using namespace bounded::literal;

struct ParsedArgs {
	ThreadCount thread_count;
	std::filesystem::path input_directory;
	std::filesystem::path output_file;
};

auto parse_args(int argc, char const * const * argv) -> ParsedArgs {
	if (argc != 4) {
		throw std::runtime_error("Usage is create_selection_weights thread_count input_directory output_file");
	}
	auto const thread_count = bounded::to_integer<ThreadCount>(argv[1]);
	auto input_directory = std::filesystem::path(argv[2]);
	if (!std::filesystem::exists(input_directory)) {
		throw std::runtime_error(containers::concatenate<std::string>(input_directory.string(), " does not exist"sv));
	}
	auto output_file = std::filesystem::path(argv[3]);
	if (std::filesystem::exists(output_file)) {
		throw std::runtime_error(containers::concatenate<std::string>(output_file.string(), " already exists"sv));
	}
	return ParsedArgs(
		thread_count,
		std::move(input_directory),
		std::move(output_file)
	);
}

struct RelevantTeam {
	template<Generation generation>
	constexpr explicit RelevantTeam(Team<generation> const & team):
		pokemon(containers::transform(
			team.all_pokemon(),
			&Pokemon<generation>::species
		)),
		active(team.all_pokemon().index()),
		moves(containers::transform(team.pokemon().regular_moves(), &Move::name))
	{
	}
	containers::static_vector<Species, max_pokemon_per_team> pokemon;
	TeamIndex active;
	MoveNames moves;
};

struct RelevantBattleState {
	Species other;
	RelevantTeam user;
	ps::SlotMemory slot_memory;
};

struct BothSides {
	Species other;
	RelevantTeam user;
};

constexpr auto get_species_from_state = []<Generation generation>(VisibleState<generation> const & state) -> tv::optional<BothSides> {
	auto const user = Team<generation>(state.ai);
	auto const selections = get_legal_selections(
		user,
		Team<generation>(state.foe),
		state.environment
	);
	if (selections == LegalSelections({pass})) {
		return tv::none;
	}
	return BothSides(state.foe.pokemon().species(), RelevantTeam(user));
};

constexpr auto fake_request() -> ps::ParsedRequest {
	return ps::ParsedRequest(
		ps::ParsedMoves(),
		ps::SwitchPossibilities::maybe_trapped,
		Party(0_bi),
		ps::ParsedTeam()
	);
}

auto battle_states_requiring_selection(
	std::span<ps::EventBlock const> const battle_messages,
	ps::BattleManager & battle
) {
	return containers::remove_none(containers::transform_non_idempotent(
		std::move(battle_messages),
		[&](ps::EventBlock const & message) -> tv::optional<RelevantBattleState> {
			auto const result = battle.handle_request(fake_request());
			auto const both_species = tv::visit(result.state, get_species_from_state);
			battle.handle_message(message);
			if (!both_species) {
				return tv::none;
			}
			return RelevantBattleState(
				both_species->other,
				both_species->user,
				result.slot_memory
			);
		}
	));
}

using Usage = bounded::integer<0, bounded::normalize<100'000'000'000'000>>;
struct Accumulator {
	Usage used = 0_bi;
	Usage did_not_use = 0_bi;
};

constexpr auto to_weight(Accumulator const accumulator) -> double {
	return double(accumulator.used) / double(accumulator.used + accumulator.did_not_use);
}

struct WeightedMove {
	MoveName move;
	double weight;
};

struct PerMatchup {
	constexpr auto used(MoveName const move) & -> void {
		++get_move_data(move).used;
		++m_switch_out.did_not_use;
	}
	constexpr auto did_not_use(MoveName const move) & -> void {
		++get_move_data(move).did_not_use;
	}
	constexpr auto switched_out() & -> void {
		++m_switch_out.used;
	}
	constexpr auto switched_in(TeamSize const possible_switches) & -> void {
		m_switch_in_weight += possible_switches;
		++m_switch_in_chances;
	}
	constexpr auto did_not_switch_in() & -> void {
		++m_switch_in_chances;
	}

	constexpr auto switch_out_weight() const -> double {
		return m_switch_out.used == 0_bi ? 0.0 : to_weight(m_switch_out);
	}
	constexpr auto switch_in_multiplier() const -> double {
		return m_switch_in_weight == 0_bi ? 0.0 : double(m_switch_in_weight) / double(m_switch_in_chances);
	}
	constexpr auto move_weights() const {
		return containers::transform(
			m_data,
			[](containers::map_value_type<MoveName, Accumulator> const value) {
				return WeightedMove(value.key, to_weight(value.mapped));
			}
		);
	}
private:
	constexpr auto get_move_data(MoveName const move) & -> Accumulator & {
		return m_data.lazy_insert(move, bounded::construct<Accumulator>).iterator->mapped;
	}
	containers::flat_map<MoveName, Accumulator> m_data;
	Accumulator m_switch_out;
	Usage m_switch_in_weight = 0_bi;
	Usage m_switch_in_chances = 0_bi;
};

using PerOther = UsageFor<Species, concurrent::locked_access<PerMatchup>>;
using SelectionWeightsMaker = UsageFor<Species, PerOther>;

auto update_weights_for_one_side_of_battle(
	std::filesystem::path const & input_file,
	SelectionWeightsMaker & weights,
	RatedSide const & rated_side,
	BattleLogMessages const & battle_messages
) -> void {
	auto battle = ps::BattleManager(battle_messages.init);
	battle.handle_request(parsed_side_to_request(rated_side.side));
	auto selections = containers::zip_smallest(
		battle_states_requiring_selection(battle_messages.messages, battle),
		rated_side.inputs
	);
	try {
		auto first = containers::begin(std::move(selections));
		auto const last = containers::end(std::move(selections));
		for (; first != last; ++first) {
			auto const & [state, input] = *first;
			auto & other_weights = weights[to_index(state.other)];
			auto get_weight = [&] {
				return other_weights[to_index(state.user.pokemon[to_index(state.user.active)])].locked();
			};
			tv::visit(input, tv::overload(
				[&](MoveName const move) {
					auto const weight = get_weight();
					for (auto const possible : state.user.moves) {
						if (move == possible) {
							weight.value().used(possible);
						} else {
							weight.value().did_not_use(possible);
						}
					}
				},
				[&](ps::BattleResponseSwitch const switch_) {
					auto const team_size = containers::size(state.user.pokemon);
					for (auto const index : containers::integer_range(team_size)) {
						if (index == state.user.active) {
							continue;
						}
						auto const potential_replacement = state.user.pokemon[index];
						auto const weight = other_weights[to_index(potential_replacement)].locked();
						if (state.slot_memory.reverse_lookup(switch_) == index) {
							weight.value().switched_in(bounded::assume_in_range<TeamSize>(team_size - 1_bi));
						} else {
							weight.value().did_not_switch_in();
						}
					}
					auto const weight = get_weight();
					for (auto const possible : state.user.moves) {
						weight.value().did_not_use(possible);
					}
					weight.value().switched_out();
				}
			));
		}
	} catch (std::exception const & ex) {
		auto const party_str = rated_side.side.party == Party(0_bi) ? "p1"sv : "p2"sv;
		std::cerr << "Unable to process " << input_file.string() << ", side " << party_str << ": " << ex.what() << ", skipping\n";
	}
}

auto write_to_file(std::ostream & stream, SelectionWeightsMaker const & weights) {
	for (auto const other : containers::enum_range<Species>()) {
		auto const & per_other = weights[to_index(other)];
		for (auto const user : containers::enum_range<Species>()) {
			auto const & per_matchup = per_other[to_index(user)].unlocked();
			auto const switch_out_weight = per_matchup.switch_out_weight();
			auto const switch_in_multiplier = per_matchup.switch_in_multiplier();
			auto const move_weights = per_matchup.move_weights();
			if (switch_out_weight == 0.0 and switch_in_multiplier == 0.0 and containers::is_empty(move_weights)) {
				continue;
			}
			write_bytes(stream, other, 2_bi);
			write_bytes(stream, user, 2_bi);
			write_bytes(stream, switch_out_weight, 8_bi);
			write_bytes(stream, switch_in_multiplier, 8_bi);
			write_bytes(stream, containers::size(move_weights), 2_bi);
			for (auto const & value : move_weights) {
				write_bytes(stream, value.move, 2_bi);
				write_bytes(stream, value.weight, 8_bi);
			}
		}
	}
}

auto create_selection_weights(SelectionWeightsMaker & weights, ThreadCount const thread_count, std::filesystem::path const & input_directory) -> void {
	for_each_log(
		thread_count,
		input_directory,
		[&](
			std::filesystem::path const & input_file,
			RatedSide const & side,
			BattleLogMessages const & battle_messages
		) {
			update_weights_for_one_side_of_battle(
				input_file,
				weights,
				side,
				battle_messages
			);
			return 0_bi;
		}
	);
}

} // namespace technicalmachine::ps_usage_stats

auto main(int argc, char ** argv) -> int {
	using namespace technicalmachine;
	using namespace technicalmachine::ps_usage_stats;
	auto const args = parse_args(argc, argv);
	auto weights = std::make_unique<SelectionWeightsMaker>();
	create_selection_weights(*weights, args.thread_count, args.input_directory);
	auto out_file = open_binary_file_for_writing(args.output_file);
	write_to_file(out_file, *weights);
	return 0;
}
