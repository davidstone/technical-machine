export module tm.ps_usage_stats.rated_side;

import tm.clients.ps.parsed_side;

import tm.clients.party;

import tm.ps_usage_stats.battle_result;
import tm.ps_usage_stats.parse_input_log;
import tm.ps_usage_stats.rating;

import containers;
import std_module;
import tv;

namespace technicalmachine::ps_usage_stats {

export struct RatedSide {
	constexpr RatedSide(
		Party const party,
		BattleResult::Side const & side_,
		containers::dynamic_array<PlayerInput> inputs_
	):
		side(party, side_.team),
		inputs(std::move(inputs_)),
		rating(side_.rating)
	{
	}
	ps::ParsedSide side;
	containers::dynamic_array<PlayerInput> inputs;
	tv::optional<Rating> rating;
};

} // namespace technicalmachine::ps_usage_stats
