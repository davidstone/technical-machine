export module tm.clients.ps.send_message_function;

import containers;
import std_module;

namespace technicalmachine::ps {

export using SendMessageFunction = containers::trivial_inplace_function<void(containers::string_view) const, sizeof(void *)>;

} // namespace technicalmachine::ps
