// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.nickname;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export struct Nickname {
	Nickname() = default;
	constexpr Nickname(std::string_view const str):
		m_str(str)
	{
	}

	constexpr operator std::string_view() const {
		return std::string_view(containers::data(m_str), static_cast<std::size_t>(containers::size(m_str)));
	}

	friend auto operator==(Nickname, Nickname) -> bool = default;

private:
	static constexpr auto max_size = 28_bi;
	containers::static_vector<char, max_size> m_str;
};

} // namespace technicalmachine