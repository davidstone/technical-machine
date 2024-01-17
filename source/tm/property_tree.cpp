// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

export module tm.property_tree;

import std_module;

namespace technicalmachine::property_tree {

// Work around some clang modules bug

export struct ptree_reader {
	using value_type = boost::property_tree::ptree::value_type;
	struct const_iterator {
		using difference_type = boost::property_tree::ptree::const_iterator::difference_type;
		explicit const_iterator(boost::property_tree::ptree::const_iterator it):
			m_it(it)
		{
		}
		auto operator++() & {
			++m_it;
			return *this;
		}
		auto operator*() const -> value_type const & {
			return *m_it;
		}
		friend auto operator==(const_iterator, const_iterator) -> bool = default;
	private:
		boost::property_tree::ptree::const_iterator m_it;
	};

	ptree_reader(boost::property_tree::ptree const & ptree):
		m_ptree(ptree)
	{
	}

	auto get_child(char const * const key) const -> ptree_reader {
		return ptree_reader(m_ptree.get_child(key));
	}
	template<typename T>
	auto get(char const * const key) const {
		return m_ptree.get<T>(key);
	}
	template<typename T>
	auto get(char const * const key, T const & default_value) const {
		return m_ptree.get(key, default_value);
	}

	auto begin() const -> const_iterator {
		return const_iterator(m_ptree.begin());
	}
	auto end() const -> const_iterator {
		return const_iterator(m_ptree.end());
	}
private:
	boost::property_tree::ptree const & m_ptree;
};

export struct ptree {
	auto read_xml(std::span<std::byte const> const bytes) -> ptree_reader {
		auto const source = boost::iostreams::array_source(
			reinterpret_cast<char const *>(bytes.data()),
			bytes.size()
		);
		auto stream = boost::iostreams::stream<boost::iostreams::array_source>(source);
		boost::property_tree::read_xml(stream, m_ptree);
		return ptree_reader(m_ptree);
	}

private:
	boost::property_tree::ptree m_ptree;
};

} // namespace technicalmachine::property_tree
