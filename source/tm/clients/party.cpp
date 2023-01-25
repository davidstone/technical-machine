// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.party;

import bounded;

namespace technicalmachine {

using namespace bounded::literal;

export struct Party {
	using value_type = bounded::integer<0, 1>;
	constexpr explicit Party(value_type const initial):
		m_party(initial)
	{
	}

	constexpr auto value() const {
		return m_party;
	}

	friend auto operator==(Party, Party) -> bool = default;

private:
	value_type m_party;
};

export constexpr auto other(Party const party) -> Party {
	return Party(1_bi - party.value());
}

} // namespace technicalmachine
