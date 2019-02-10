// Copyright (C) 2018 David Stone
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

#include <tm/clients/pokemon_showdown/battle_factory.hpp>
#include <tm/clients/pokemon_showdown/chat.hpp>

#include <tm/stat/combined_stats.hpp>
#include <tm/stat/stat_to_ev.hpp>

#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/pokemon.hpp>

#include <bounded/integer.hpp>

#include <containers/algorithms/transform.hpp>
#include <containers/legacy_iterator.hpp>

#include <algorithm>

namespace technicalmachine {
namespace ps {

namespace {

auto hp_to_ev(Species const species, Level const level, HP::max_type const stat) {
	auto const stat_range = containers::transform(ev_range(), [=](EV const ev) { return HP(species, level, ev).max(); });
	auto const it = std::lower_bound(containers::legacy_iterator(begin(stat_range)), containers::legacy_iterator(end(stat_range)), stat);
	if (it.base() == end(stat_range)) {
		throw std::runtime_error("No valid HP EV for a given stat value");
	}
	return *it.base().base();
}

auto calculate_evs(boost::property_tree::ptree const & stats, Species const species, Level const level, HP::max_type const hp) -> CombinedStats {
	using stat_value = bounded::checked_integer<4, 614>;
	auto const attack = stats.get<stat_value>("atk");
	auto const defense = stats.get<stat_value>("def");
	auto const special_attack = stats.get<stat_value>("spa");
	auto const special_defense = stats.get<stat_value>("spd");
	auto const speed = stats.get<stat_value>("spe");
	// TODO: Give the correct IVs for the Hidden Power type
	
	auto base_stat = [=](StatNames const stat) { return Stat(species, stat).base(); };
	
	auto const base_attack = base_stat(StatNames::ATK);
	auto const base_defense = base_stat(StatNames::DEF);
	auto const base_special_attack = base_stat(StatNames::SPA);
	auto const base_special_defense = base_stat(StatNames::SPD);
	auto const base_speed = base_stat(StatNames::SPE);
	
	auto to_ev = [](auto const integer) { return EV(EV::value_type(integer)); };
	auto const hp_ev = hp_to_ev(species, level, hp);
	for (auto const nature : containers::enum_range<Nature>()) {
		auto const attack_ev = stat_to_ev(attack, nature, StatNames::ATK, base_attack, IV(31_bi), level);
		if (attack_ev > EV::max) {
			continue;
		}
		auto const defense_ev = stat_to_ev(defense, nature, StatNames::DEF, base_defense, IV(31_bi), level);
		if (defense_ev > EV::max) {
			continue;
		}
		auto const special_attack_ev = stat_to_ev(special_attack, nature, StatNames::SPA, base_special_attack, IV(31_bi), level);
		if (special_attack_ev > EV::max) {
			continue;
		}
		auto const special_defense_ev = stat_to_ev(special_defense, nature, StatNames::SPD, base_special_defense, IV(31_bi), level);
		if (special_defense_ev > EV::max) {
			continue;
		}
		auto const speed_ev = stat_to_ev(speed, nature, StatNames::SPE, base_speed, IV(31_bi), level);
		if (speed_ev > EV::max) {
			continue;
		}

		auto const combined = CombinedStats{nature, hp_ev, to_ev(attack_ev), to_ev(defense_ev), to_ev(special_attack_ev), to_ev(special_defense_ev), to_ev(speed_ev)};
		if (ev_sum(combined) > EV::max_total) {
			continue;
		}
		
		return combined;
	}
	throw std::runtime_error("No Nature + EV combination combines to give the received stats");
}

Team parse_team(boost::property_tree::ptree const & pt) {
	auto const team_data = range_view(pt.get_child("side").get_child("pokemon").equal_range(""));
	constexpr bool is_me = true;
	auto team = Team(TeamSize(containers::distance(team_data.begin(), team_data.end())), is_me);
	for (auto const & pokemon_data : team_data) {
		auto get = [&](auto const & key) { return pokemon_data.second.get<std::string>(key); };

		auto const details = parse_details(get("details"));
		
		auto const condition = get("condition");
		// current_hp/max_hp
		// Presumably also gives me status information? Should be useful for
		// rejoining battles
		// TODO: If we disconnect in a battle when the HP is 0, we might not
		// have a '/'
		auto const hp = bounded::to_integer<HP::max_type>(split(condition, '/').first);
		
		auto const evs = calculate_evs(pokemon_data.second.get_child("stats"), details.species, details.level, hp);

		auto const ability = from_string<Ability>(get("baseAbility"));
		
		auto const item = from_string<Item>(get("item"));
		
		Pokemon & pokemon = team.add_pokemon(details.species, details.level, details.gender, item, ability, evs.nature);
		
		for (auto const & move : pokemon_data.second.get_child("moves")) {
			 add_seen_move(all_moves(pokemon), from_string<Moves>(move.second.get<std::string>("")));
		}

		set_hp_ev(pokemon, evs.hp);
		set_stat_ev(pokemon, StatNames::ATK, evs.attack);
		set_stat_ev(pokemon, StatNames::DEF, evs.defense);
		set_stat_ev(pokemon, StatNames::SPA, evs.special_attack);
		set_stat_ev(pokemon, StatNames::SPD, evs.special_defense);
		set_stat_ev(pokemon, StatNames::SPE, evs.speed);
	}
	team.all_pokemon().reset_index();
	return team;
}

}	// namespace

void BattleFactory::handle_message(InMessage message) {
	m_battle_logger.log(message);
	
	if (handle_chat_message(message)) {
		return;
	}

	// Documented at
	// https://github.com/Zarel/Pokemon-Showdown/blob/master/PROTOCOL.md
	// under the section "Battle Initialization"
	if (message.type() == "") {
		// Unnecessary
	} else if (message.type() == "clearpoke") {
		// This appears to mean nothing
	} else if (message.type() == "gametype") {
		m_type.emplace(message.next());
	} else if (message.type() == "gen") {
		m_generation.emplace(bounded::to_integer<1, 7>(message.next()));
	} else if (message.type() == "player") {
		auto const player_id = message.next();
		auto const username = message.next();
		if (username == m_username) {
			m_party.emplace(make_party(player_id));
		}
		// message.remainder() == AVATAR
	} else if (message.type() == "poke") {
		// message.remainder() == PLAYER_ID|DETAILS|ITEM
	} else if (message.type() == "rated") {
		// Received if and only if the game is rated. We don't care about this
	} else if (message.type() == "request") {
		// It seems that the game starts off with a single "request" that has
		// no data. Then it sends another request with data, which we need to
		// parse for Random Battles.

		// TODO: maybe just do this on random battles, since we know what our
		// team is otherwise?
		auto const json_data = message.remainder();
		if (!json_data.empty()) {
			m_team = parse_team(m_parse_json(json_data));
		}
	} else if (message.type() == "rule") {
		// message.remainder() == RULE: DESCRIPTION
		// Received for each clause in effect
	} else if (message.type() == "seed") {
		// I have no idea what this is
	} else if (message.type() == "start") {
		// We can't actually start the battle until we see the initial switch-in
	} else if (message.type() == "switch") {
		if (!m_party) {
			throw std::runtime_error("Received a switch message before receiving a party");
		}
		auto const parsed = parse_switch(message);
		if (*m_party == parsed.party) {
			if (m_ai_switched_in) {
				throw std::runtime_error("AI switched in twice");
			}
			m_ai_switched_in = true;
		} else {
			if (m_opponent_starter) {
				throw std::runtime_error("Foe switched in twice");
			}
			m_opponent_starter.emplace(parsed);
		}
	} else if (message.type() == "teampreview") {
		// This appears to mean nothing
	} else if (message.type() == "teamsize") {
		if (!m_party) {
			throw std::runtime_error("Received a teamsize message before receiving a player id");
		}
		auto const party = make_party(message.next());
		auto const team_size = bounded::to_integer<TeamSize>(message.next());
		// TODO: validate that the received teamsize matches my team size
		if (*m_party != party) {
			m_opponent_team_size.emplace(team_size);
		}
	} else if (message.type() == "tier") {
		m_tier.emplace(message.next());
	} else if (message.type() == "title") {
		// message.remainder() == PLAYER1 vs. PLAYER2
	} else {
		std::cerr << "Received battle setup message of unknown type: " << message.type() << ": " << message.remainder() << '\n';
	}
}

BattleParser BattleFactory::make(BattleParser::SendMessageFunction send_message) && {
	assert(completed());
	if (!m_party) {
		throw std::runtime_error("Did not receive party");
	}
	if (!m_type) {
		throw std::runtime_error("Did not receive battle format");
	}
	if (!m_tier) {
		throw std::runtime_error("Did not receive tier");
	}
	if (!m_generation) {
		throw std::runtime_error("Did not receive generation");
	}
	if (!m_opponent_team_size) {
		throw std::runtime_error("Did not receive opponent team size");
	}
	if (!m_opponent_starter) {
		throw std::runtime_error("Did not receive opponent's starting species");
	}
	if (*m_type != "singles") {
		throw std::runtime_error("Unsupported format " + *m_type);
	}
	if (*m_generation != 4) {
		throw std::runtime_error("Unsupported generation " + bounded::to_string(*m_generation));
	}
	auto make_foe_team = [&]{
		auto team = Team(*m_opponent_team_size, false);
		auto const pokemon = *m_opponent_starter;
		team.add_pokemon(pokemon.species, pokemon.level, pokemon.gender);
		return team;
	};
	return BattleParser(
		std::move(send_message),
		std::move(m_battle_logger),
		std::ofstream(m_log_directory / "analysis.txt"),
		std::move(m_id),
		std::move(m_username),
		m_usage_stats,
		m_evaluate,
		*m_party,
		m_depth,
		m_random_engine,
		std::move(m_team),
		make_foe_team(),
		m_log_foe_teams
	);
}

}	// namespace ps
}	// namespace technicalmachine
