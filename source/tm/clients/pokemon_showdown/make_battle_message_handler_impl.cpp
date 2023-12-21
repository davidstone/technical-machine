// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module tm.clients.ps.make_battle_message_handler;

import tm.clients.ps.battle_init_message;
import tm.clients.ps.battle_message_handler;
import tm.clients.ps.parsed_team_to_known_team;

import tm.clients.party;
import tm.clients.teams;

import tm.constant_generation;
import tm.generation;
import tm.generation_generic;
import tm.team;

namespace technicalmachine::ps {

template<Generation generation>
constexpr auto make_foe(BattleInitMessage::Team parsed) -> SeenTeam<generation> {
	auto team = SeenTeam<generation>(parsed.size);
	auto & pokemon = parsed.starter;
	team.add_pokemon({
		pokemon.species,
		pokemon.nickname,
		pokemon.level,
		pokemon.gender
	});
	return team;
}

auto make_battle_message_handler(
	ParsedSide const & team,
	BattleInitMessage const message
) -> BattleMessageHandler {
	return BattleMessageHandler(
		team.party,
		constant_generation(message.generation, [&]<Generation generation>(constant_gen_t<generation>) -> GenerationGeneric<Teams> {
			return Teams<generation>(
				parsed_team_to_known_team<generation>(team),
				make_foe<generation>(message.team[other(team.party).value()])
			);
		})
	);
}

} // namespace technicalmachine::ps
