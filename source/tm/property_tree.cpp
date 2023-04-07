// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

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

export struct ptree_writer {
	explicit ptree_writer(boost::property_tree::ptree & ptree):
		m_ptree(ptree)
	{
	}
	template<typename T>
	auto add(char const * const key, T const & mapped) -> ptree_writer {
		return ptree_writer(m_ptree.add(key, mapped));
	}
	template<typename T>
	auto put(char const * const key, T const & mapped) -> void {
		m_ptree.put(key, mapped);
	}

	auto write_xml(std::filesystem::path const & file_name) -> void {
		auto settings = boost::property_tree::xml_writer_settings<boost::property_tree::ptree::key_type>('\t', 1);
		boost::property_tree::write_xml(file_name.string(), m_ptree, std::locale(), settings);
	}
private:
	boost::property_tree::ptree & m_ptree;
};

export struct ptree {
	operator ptree_writer() {
		return ptree_writer(m_ptree);
	}
	auto read_xml(std::filesystem::path const & path) -> ptree_reader {
		boost::property_tree::read_xml(path.string(), m_ptree);
		return ptree_reader(m_ptree);
	}

private:
	boost::property_tree::ptree m_ptree;
};

} // namespace technicalmachine::property_tree
