// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/move_name.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/ability.hpp>
#include <tm/item.hpp>

#include <containers/flat_map.hpp>
#include <containers/lookup.hpp>

#include <utility>

namespace technicalmachine {

struct UsageStatsProbabilities {
	template<typename T>
	struct Data {
		using key_type = T;
		using mapped_type = float;
		using Map = containers::flat_map<key_type, mapped_type>;
		Data() {
		}

		explicit Data(bounded::explicitly_convertible_to<Map> auto && source):
			m_map(OPERATORS_FORWARD(source))
		{
		}

		auto map() const -> Map const & {
			return m_map;
		}

		auto operator()(key_type const move) const -> mapped_type {
			auto const element = containers::lookup(m_map, move);
			return element ? *element : 0.0F;
		}

		auto insert(auto && range) & {
			m_map.insert(OPERATORS_FORWARD(range));
		}
		template<typename K = key_type>
		auto lazy_insert(K && key, bounded::construct_function_for<mapped_type> auto && mapped) {
			return m_map.lazy_insert(OPERATORS_FORWARD(key), OPERATORS_FORWARD(mapped));
		}
	private:
		Map m_map;
	};

	struct Inner {
		Data<MoveName> moves;
		Data<Item> items;
		Data<Ability> abilities;
	};

	using Map = containers::flat_map<Species, Inner>;
	explicit UsageStatsProbabilities(Map map):
		m_map(std::move(map))
	{
	}

	auto operator()(Species const species) const -> Inner const * {
		return containers::lookup(m_map, species);
	}

	auto map() const -> Map const & {
		return m_map;
	}

private:
	Map m_map;
};

} // namespace technicalmachine
