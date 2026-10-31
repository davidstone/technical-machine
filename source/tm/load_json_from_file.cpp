export module tm.load_json_from_file;

export import nlohmann.json;
import tm.open_file;

import std_module;

namespace technicalmachine {

export auto load_json_from_file(std::filesystem::path const & path) -> nlohmann::json {
	auto json = nlohmann::json();
	auto file = open_text_file_for_reading(path);
	file >> json;
	return json;
}

} // namespace technicalmachine
