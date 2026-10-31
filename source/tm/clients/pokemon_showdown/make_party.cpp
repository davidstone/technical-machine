export module tm.clients.ps.make_party;

import tm.clients.party;

import bounded;
import containers;
import std_module;

namespace technicalmachine::ps {
using namespace bounded::literal;
using namespace containers::string_literals;

export constexpr auto make_party(containers::string_view const party_str) -> Party {
	if (party_str == "p1"_sv) {
		return Party(0_bi);
	} else if (party_str == "p2"_sv) {
		return Party(1_bi);
	} else {
		throw std::runtime_error(containers::concatenate<std::string>("Invalid player id: "_sv, party_str));
	}
}

} // namespace technicalmachine::ps
