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
using namespace containers::string_literals;

template<std::size_t size>
constexpr auto equal(ps::MessageBlock const block, containers::c_array<containers::string_view, size> && expected) {
	return containers::equal(
		expected,
		containers::transform(
			block,
			[](ps::InMessage const message) {
				return containers::concatenate<containers::string>(
					"|"_s,
					message.type(),
					"|"_s,
					message.remainder()
				);
			}
		)
	);
}

static_assert(equal(
	ps::message_block("||"_s),
	{"||"_s}
));

static_assert(equal(
	ps::message_block("|switch|fuzz"_s),
	{"|switch|fuzz"_s}
));

} // namespace
} // namespace technicalmachine
