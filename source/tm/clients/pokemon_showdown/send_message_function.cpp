// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.send_message_function;

import containers;
import std_module;

namespace technicalmachine::ps {

export using SendMessageFunction = containers::trivial_inplace_function<void(std::string_view) const, sizeof(void *)>;

} // namespace technicalmachine::ps
