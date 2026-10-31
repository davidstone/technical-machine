export module tm.web_server.extension_to_content_type;

import containers;
import std_module;

namespace technicalmachine {
using namespace containers::string_literals;

export auto extension_to_content_type(std::filesystem::path const & ext) -> containers::string_view {
	if (ext == ".html") {
		return "text/html"_sv;
	} else if (ext == ".js") {
		return "application/javascript"_sv;
	} else {
		throw std::runtime_error(containers::concatenate<std::string>(
			"Unknown content type "_sv,
			ext.string()
		));
	}
}

} // namespace technicalmachine
