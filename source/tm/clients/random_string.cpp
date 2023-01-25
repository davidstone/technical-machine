// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.random_string;

import containers;
import std_module;

namespace technicalmachine {

template<typename Engine>
struct random_character {
	constexpr explicit random_character(Engine & random_engine):
		m_random_engine(random_engine),
		m_distribution(0, legal_characters.size() - 1)
	{
	}

	constexpr auto operator()() -> char {
		return legal_characters[m_distribution(m_random_engine)];
	}

private:
	static constexpr auto legal_characters = std::string_view("abcdefghijklmnopqrstuvwxyz0123456789");
	Engine & m_random_engine;
	std::uniform_int_distribution<std::size_t> m_distribution;
};

export constexpr auto random_string(auto & random_engine, auto const size) {
	return containers::string(containers::generate_n(size, random_character(random_engine)));
}

} // namespace technicalmachine
