export module tm.pokemon.nickname;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export struct Nickname {
	Nickname() = default;
	constexpr Nickname(containers::string_view const str):
		m_str(str)
	{
	}

	constexpr auto str() const -> containers::string_view {
		return containers::string_view(m_str);
	}

	friend auto operator==(Nickname, Nickname) -> bool = default;

private:
	static constexpr auto max_size = 28_bi;
	containers::static_vector<char, max_size> m_str;
};

} // namespace technicalmachine