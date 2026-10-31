export module tm.clients.ps.message_block;

import tm.clients.ps.in_message;

import bounded;
import containers;
import std_module;

namespace technicalmachine::ps {
using namespace containers::string_literals;

// Guaranteed to return a range of at least one (possibly empty) message
export constexpr auto message_block(containers::string_view str) {
	return containers::transform(
		containers::split(std::move(str), '\n'),
		bounded::construct<InMessage>
	);
}
export using MessageBlock = decltype(message_block(""_sv));

} // namespace technicalmachine::ps
