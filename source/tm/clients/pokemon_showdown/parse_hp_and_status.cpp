export module tm.clients.ps.parse_hp_and_status;

import tm.clients.ps.parse_hp;
import tm.clients.ps.parse_status;

import tm.status.status_name;

import tm.split_view;
import tm.visible_hp;

import containers;
import std_module;

namespace technicalmachine::ps {

struct HPAndStatus {
	VisibleHP hp;
	StatusName status;
};
export constexpr auto parse_hp_and_status(containers::string_view const hp_and_status) {
	auto const [hp_fraction, status] = split_view(hp_and_status, ' ');
	return HPAndStatus{parse_hp(hp_fraction), parse_status(status)};
}

} // namespace technicalmachine::ps
