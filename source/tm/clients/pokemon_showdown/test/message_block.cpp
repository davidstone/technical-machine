// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

export module tm.test.clients.ps.message_block;

import tm.clients.ps.in_message;
import tm.clients.ps.message_block;

import containers;
import std_module;

namespace technicalmachine {
namespace {

using namespace std::string_view_literals;

template<std::size_t size>
constexpr auto equal(ps::MessageBlock const block, containers::c_array<std::string_view, size> && expected) {
	return containers::equal(
		expected,
		containers::transform(
			block,
			[](ps::InMessage const message) {
				return containers::concatenate<containers::string>(
					"|"sv,
					message.type(),
					"|"sv,
					message.remainder()
				);
			}
		)
	);
}

static_assert(equal(
	ps::message_block("||"),
	{"||"}
));

static_assert(equal(
	ps::message_block("|switch|fuzz"),
	{"|switch|fuzz"}
));

} // namespace
} // namespace technicalmachine
