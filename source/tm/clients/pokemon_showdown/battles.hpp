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

	void add_pending(auto && ... args) {
		containers::push_back(
			m_pending,
			make_battle_factory(m_log_directory, m_log_foe_teams, OPERATORS_FORWARD(args)...)
		);
	}

	bool handle_message(AllUsageStats const & usage_stats, InMessage message, SendMessageFunction const send_message, auto challenge) {
		if (handle_message_for_active(message, send_message, std::move(challenge))) {
			return true;
		}
		if (handle_message_for_pending(message, usage_stats, send_message)) {
			return true;
		}

		return false;
	}

private:
	using Pending = containers::vector<std::unique_ptr<BattleFactory>>;
	using Active = containers::vector<std::unique_ptr<BattleParser>>;

	static constexpr auto find_battle(auto & container, std::string_view const room) {
		return containers::find_if(container, [=](auto const & battle) { return battle->id() == room; });
	}

	auto move_to_active(AllUsageStats const & usage_stats, containers::iterator_t<Pending &> it) -> void {
		auto make_parser = []<typename Function>(Function function) {
			return std::make_unique<BattleParser>(bounded::detail::superconstructing_super_elider<BattleParser, Function>(std::move(function)));
		};
		containers::push_back(
			m_active,
			make_parser([&] { return std::move(**it).make(usage_stats); })
		);
		containers::erase(m_pending, it);
	}

	auto handle_message_for_pending(InMessage message, AllUsageStats const & usage_stats, SendMessageFunction const send_message) -> bool {
		auto const it = find_battle(m_pending, message.room());
		if (it == containers::end(m_pending)) {
			return false;
		}
		auto & battle = **it;
		battle.handle_message(message);
		if (battle.completed()) {
			move_to_active(usage_stats, it);
			send_message(containers::concatenate<containers::string>(message.room(), "|/timer on"sv));
		}
		return true;
	}

	auto handle_message_for_active(InMessage message, SendMessageFunction const send_message, auto const challenge) -> bool {
		auto const it = find_battle(m_active, message.room());
		if (it == containers::end(m_active)) {
			return false;
		}
		auto & battle = **it;
		auto const response = battle.handle_message(message);
		if (response) {
			send_message(*response);
		}
		if (battle.completed()) {
			containers::erase(m_active, it);
			send_message(containers::concatenate<containers::string>(std::string_view("|/leave "), message.room()));
			challenge();
		}
		return true;
	}

	std::filesystem::path m_log_directory;
	Pending m_pending;
	Active m_active;
	bool m_log_foe_teams;
};

} // namespace ps
} // namespace technicalmachine
