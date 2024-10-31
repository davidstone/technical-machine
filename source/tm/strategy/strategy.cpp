// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

export module tm.strategy.strategy;

import tm.move.legal_selections;

import tm.strategy.weighted_selection;

import tm.environment;
import tm.generation;
import tm.generation_generic;
import tm.team;
import tm.team_is_empty;

import bounded;
import containers;
import std_module;
import tv;

namespace technicalmachine {
using namespace bounded::literal;

using Signature = auto(
	GenerationGeneric<Team> const &,
	LegalSelections,
	GenerationGeneric<Team> const &,
	LegalSelections,
	Environment
) const -> BothWeightedSelections;

using Function = std::move_only_function<Signature>;

template<typename Impl>
struct Wrapper {
	constexpr explicit Wrapper(bounded::construct_function_for<Impl> auto function):
		m_impl(std::invoke(std::move(function)))
	{
	}
	constexpr auto operator()(
		GenerationGeneric<Team> const & generic_ai,
		LegalSelections const ai_selections,
		GenerationGeneric<Team> const & generic_foe,
		LegalSelections const foe_selections,
		Environment const environment
	) const -> BothWeightedSelections {
		if (containers::size(ai_selections) == 1_bi) {
			return BothWeightedSelections(
				WeightedSelections({{containers::front(ai_selections), 1.0}}),
				WeightedSelections()
			);
		}
		return tv::visit(
			generic_ai,
			generic_foe,
			tv::overload(
				[&]<Generation generation>(
					Team<generation> const & ai,
					Team<generation> const & foe
				) -> BothWeightedSelections {
					BOUNDED_ASSERT(!team_is_empty(ai));
					BOUNDED_ASSERT(!team_is_empty(foe));
					auto result = m_impl(
						ai,
						ai_selections,
						foe,
						foe_selections,
						environment
					);
					if constexpr (std::same_as<decltype(result), WeightedSelections>) {
						return BothWeightedSelections(result, WeightedSelections());
					} else {
						return result;
					}
				},
				[](auto const &, auto const &) -> BothWeightedSelections {
					std::unreachable();
				}
			)
		);
	}

private:
	Impl m_impl;
};

// Strategy can be constructed from one of two options:
// * A function that returns the weighted selections of the AI as well as the
// predicted selections of the foe, or
// * A function that returns just the weighted selections of the AI, to be used
// if the strategy does not attempt to predict the foe's actions. In this case,
// an empty set of predictions is returned
export struct Strategy : private Function {
	template<typename MakeImpl>
	explicit Strategy(bounded::lazy_init_t, MakeImpl make_impl):
		Function(
			std::in_place_type<Wrapper<std::remove_reference_t<std::invoke_result_t<MakeImpl>>>>,
			std::move(make_impl)
		)
	{
	}
	template<typename Impl>
	explicit Strategy(Impl impl):
		Strategy(bounded::lazy_init, bounded::value_to_function(std::move(impl)))
	{
	}

	using Function::operator();
};

} // namespace technicalmachine
