export module tm.clients.ps.parse_gender;

import tm.gender;

import containers;
import std_module;

namespace technicalmachine::ps {
using namespace containers::string_literals;

export constexpr auto parse_gender(containers::string_view const str) -> Gender {
	return
		str == "F"_sv ? Gender::female :
		str == "M"_sv ? Gender::male :
		Gender::genderless;
}

} // namespace technicalmachine::ps
