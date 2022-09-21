// Handles challenges / current battles
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/clients/pokemon_showdown/battle_factory.hpp>
#include <tm/clients/pokemon_showdown/battle_parser.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/algorithms/erase.hpp>
#include <containers/begin_end.hpp>
#include <containers/push_back.hpp>
#include <containers/string.hpp>
#include <containers/trivial_inplace_function.hpp>
#include <containers/vector.hpp>

#include <filesystem>
#include <memory>
#include <string_view>
#include <utility>

namespace technicalmachine {

struct AllUsageStats;

namespace ps {

using SendMessageFunction = containers::trivial_inplace_function<void(std::string_view) const, sizeof(void *)>;

struct Battles {
	explicit Battles(std::filesystem::path log_directory, bool const log_foe_teams):
		m_log_directory(std::move(log_directory)),
		m_log_foe_teams(log_foe_teams)
	{
		std::filesystem::create_directories(m_log_directory);
	}

	auto add_pending(auto && ... args) -> void {
		containers::push_back(
			m_container,
			make_battle_factory(m_log_directory, m_log_foe_teams, OPERATORS_FORWARD(args)...)
		);
	}

	auto handle_message(AllUsageStats const & usage_stats, InMessage message, SendMessageFunction const send_message, auto challenge) -> bool {
		auto matches_room = [&](auto const & battle) { return battle->id() == message.room(); };
		auto const it = containers::find_if(m_container, matches_room);
		if (it == containers::end(m_container)) {
			return false;
		}
		auto & battle = **it;
		auto const response = battle.handle_message(message);
		if (response) {
			send_message(*response);
		}
		switch (battle.completed()) {
			case BattleInterface::Complete::none:
				break;
			case BattleInterface::Complete::start:
				make_active(usage_stats, it);
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
	using Container = containers::vector<std::unique_ptr<BattleInterface>>;

	auto make_active(AllUsageStats const & usage_stats, containers::iterator_t<Container &> it) -> void {
		auto & battle_factory = static_cast<BattleFactory &>(**it);
		*it = std::unique_ptr<BattleInterface>(new BattleParser(std::move(battle_factory).make(usage_stats)));
	}

	std::filesystem::path m_log_directory;
	Container m_container;
	bool m_log_foe_teams;
};

} // namespace ps
} // namespace technicalmachine
