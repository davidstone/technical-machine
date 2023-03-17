// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.evaluate.all_evaluate;

import tm.evaluate.evaluate;
import tm.evaluate.load_evaluate;

import tm.string_conversions.generation;

import tm.generation;
import tm.get_directory;

import bounded;
import tv;

namespace technicalmachine {

template<Generation generation>
auto settings_for_generation() {
	return Evaluate<generation>(load_evaluate(get_settings_directory() / to_string(generation) / "OU/evaluate.json"));
}

export struct AllEvaluate {
	AllEvaluate():
		m_data(
			settings_for_generation<Generation::one>(),
			settings_for_generation<Generation::two>(),
			settings_for_generation<Generation::three>(),
			settings_for_generation<Generation::four>(),
			settings_for_generation<Generation::five>(),
			settings_for_generation<Generation::six>(),
			settings_for_generation<Generation::seven>(),
			settings_for_generation<Generation::eight>()
		)
	{
	}
	template<Generation generation>
	auto get() const {
		return m_data[bounded::type_t<Evaluate<generation>>()];
	}

private:
	tv::tuple<
		Evaluate<Generation::one>,
		Evaluate<Generation::two>,
		Evaluate<Generation::three>,
		Evaluate<Generation::four>,
		Evaluate<Generation::five>,
		Evaluate<Generation::six>,
		Evaluate<Generation::seven>,
		Evaluate<Generation::eight>
	> m_data;
};

} // namespace technicalmachine
