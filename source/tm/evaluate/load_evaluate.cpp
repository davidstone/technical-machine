// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.evaluate.load_evaluate;

import tm.evaluate.evaluate_settings;

import tm.load_json_from_file;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace containers::string_literals;

export auto load_evaluate(std::filesystem::path const & path) -> EvaluateSettings {
	auto const json = load_json_from_file(path);
	auto const & config = json.at("score");

	auto get = [&](containers::string_view const field) {
		return bounded::check_in_range<EvaluateSettings::value_type>(bounded::integer(config.value(field, 0)));
	};
	return EvaluateSettings{
		.hp = get("hp"_sv),
		.hidden = get("hidden"_sv),
		.spikes = get("spikes"_sv),
		.stealth_rock = get("stealth rock"_sv),
		.toxic_spikes = get("toxic spikes"_sv)
	};
}

} // namespace technicalmachine
