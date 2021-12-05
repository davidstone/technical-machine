// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/generation.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/single_element_range.hpp>

#include <stdexcept>
#include <string_view>

namespace technicalmachine {
namespace ps {

using namespace std::string_view_literals;

constexpr auto parse_generation_from_format(std::string_view const id) -> Generation {
	// TODO: This won't work for generation 10
	// Expected format: genXou
	constexpr auto generation_index = std::string_view("gen").size();
	if (id.size() < generation_index) {
		throw std::runtime_error(containers::concatenate<std::string>("Invalid format string. Expected something in the format of: \"gen[generation_number]ou\", but got "sv, id));
	}
	auto const generation_char = id[generation_index];
	auto const generation = generation_char - '0';
	if (generation < 1 or 8 < generation) {
		throw std::runtime_error(containers::concatenate<std::string>("Invalid generation. Expected a value between 1 and 8, but got "sv, containers::single_element_range(generation_char)));
	}
	return static_cast<Generation>(generation);
}

} // namespace ps
} // namespace technicalmachine
