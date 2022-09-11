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
#include <containers/vector.hpp>

#include <filesystem>
#include <memory>
#include <string_view>
#include <utility>

namespace technicalmachine {

struct AllUsageStats;

namespace ps {

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

	bool handle_message(AllUsageStats const & usage_stats, InMessage message, auto send_message, auto challenge) {
		auto complete_active_battle = [&](containers::iterator_t<Active const &> const it) {
			containers::erase(m_active, it);
			send_message(containers::concatenate<containers::string>(std::string_view("|/leave "), message.room()));
			challenge();
		};
		if (handle_message_impl(m_active, message, complete_active_battle)) {
			return true;
		}
		auto begin_pending_battle = [&](auto const it) { move_to_active(usage_stats, it, std::move(send_message)); };
		if (handle_message_impl(m_pending, message, begin_pending_battle)) {
			return true;
		}

		return false;
	}

private:
	using Pending = containers::vector<std::unique_ptr<BattleFactory>>;
	using Active = containers::vector<std::unique_ptr<BattleParser>>;

	static bool handle_message_impl(auto & container, InMessage message, auto if_completed) {
		auto const function = [=](auto const & battle) { return battle->id() == message.room(); };
		auto const it = containers::find_if(container, function);
		if (it == containers::end(container)) {
			return false;
		}
		(*it)->handle_message(message);
		if ((*it)->completed()) {
			if_completed(it);
		}
		return true;
	}

	void move_to_active(AllUsageStats const & usage_stats, containers::iterator_t<Pending &> it, auto send_message) {
		auto make_parser = []<typename Function>(Function function) {
			return std::make_unique<BattleParser>(bounded::detail::superconstructing_super_elider<BattleParser, Function>(std::move(function)));
		};
		containers::push_back(
			m_active,
			make_parser([&] { return std::move(**it).make(usage_stats, std::move(send_message)); })
		);
		containers::erase(m_pending, it);
	}

	std::filesystem::path m_log_directory;
	Pending m_pending;
	Active m_active;
	bool m_log_foe_teams;
};

} // namespace ps
} // namespace technicalmachine
