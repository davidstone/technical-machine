export module tm.split_view;

import tm.split_offsets;

import bounded;
import containers;
import std_module;

namespace technicalmachine {

export constexpr auto split_view(containers::string_view const buffer, auto const delimiter) -> std::pair<containers::string_view, containers::string_view> {
	auto const offsets = split_offsets(buffer, delimiter);
	if (offsets.first == buffer.size()) {
		return std::pair(buffer, containers::string_view());
	}
	return std::pair(
		containers::string_view(buffer.data(), offsets.first),
		containers::string_view(
			buffer.data() + offsets.first + offsets.discard,
			bounded::increase_min<0>(buffer.size() - offsets.discard - offsets.first)
		)
	);
}

} // namespace technicalmachine