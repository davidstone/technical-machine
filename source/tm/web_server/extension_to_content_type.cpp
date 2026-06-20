// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.web_server.extension_to_content_type;

import containers;
import std_module;

namespace technicalmachine {
using namespace containers::string_literals;

export auto extension_to_content_type(std::filesystem::path const & ext) -> containers::string_view {
	if (ext == ".html") {
		return "text/html"_s;
	} else if (ext == ".js") {
		return "application/javascript"_s;
	} else {
		throw std::runtime_error(containers::concatenate<std::string>(
			"Unknown content type "_s,
			ext.string()
		));
	}
}

} // namespace technicalmachine
