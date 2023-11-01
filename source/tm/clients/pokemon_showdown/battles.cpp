// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.battles;

import tm.clients.ps.battle_interface;
import tm.clients.ps.battle_factory;
import tm.clients.ps.battle_logger;
import tm.clients.ps.battle_parser;
import tm.clients.ps.inmessage;
import tm.clients.ps.send_message_function;

import tm.clients.write_team;

import tm.evaluate.all_evaluate;
import tm.evaluate.depth;

import tm.team_predictor.all_usage_stats;

import containers;
import tv;
import std_module;

namespace technicalmachine::ps {
using namespace std::string_view_literals;

export struct Battles {
	explicit Battles(std::filesystem::path log_directory, tv::optional<WriteTeam> write_team):
		m_log_directory(std::move(log_directory)),
		m_write_team(std::move(write_team))
	{
		std::filesystem::create_directories(m_log_directory);
	}

	auto add_pending(
		containers::string id,
		containers::string username,
		AllEvaluate evaluate,
		Depth depth,
		std::mt19937 random_engine
	) -> void {
		containers::lazy_push_back(
			m_container,
			[&] {
				auto battle_logger = std::make_unique<BattleLogger>(m_log_directory / std::string_view(id), id);
				return Element{
					std::move(battle_logger),
					make_battle_factory(
						std::move(id),
						std::move(username),
						evaluate,
						depth,
						std::move(random_engine)
					)
				};
			}
		);
	}

	auto handle_message(AllUsageStats const & usage_stats, InMessage message, SendMessageFunction const send_message, auto challenge) -> bool {
		auto matches_room = [&](auto const & element) {
			return element.battle->id() == message.room();
		};
		auto const it = containers::find_if(m_container, matches_room);
		if (it == containers::end(m_container)) {
			return false;
		}
		it->logger->log(message);
		auto const response = it->battle->handle_message(message);
		if (response) {
			send_message(*response);
		}
		switch (it->battle->completed()) {
			case BattleInterface::Complete::none:
				break;
			case BattleInterface::Complete::start:
				make_active(usage_stats, it->battle);
				send_message(containers::concatenate<containers::string>(message.room(), "|/timer on"sv));
				break;
			case BattleInterface::Complete::finish:
				containers::erase(m_container, it);
				send_message(containers::concatenate<containers::string>(std::string_view("|/leave "), message.room()));
				challenge();
				break;
		}
		return true;
	}

private:
	struct Element {
		// Must be `unique_ptr` because `ofstream` is not nothrow movable
		std::unique_ptr<BattleLogger> logger;
		std::unique_ptr<BattleInterface> battle;
	};

	auto make_active(AllUsageStats const & usage_stats, std::unique_ptr<BattleInterface> & battle) -> void {
		auto & battle_factory = static_cast<BattleFactory &>(*battle);
		auto const battle_log_directory = m_log_directory / battle_factory.id();
		battle = std::unique_ptr<BattleInterface>(new BattleParser(
			std::move(battle_factory).make(
				usage_stats,
				battle_log_directory,
				m_write_team
			)
		));
	}

	std::filesystem::path m_log_directory;
	containers::vector<Element> m_container;
	tv::optional<WriteTeam> m_write_team;
};

} // namespace technicalmachine::ps
