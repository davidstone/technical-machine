// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>

#include <compare>

#include <boost/iostreams/stream.hpp>

export module tm.evaluate.analysis_logger;

import tm.open_file;

import std_module;

namespace technicalmachine {

export struct AnalysisLogger {
	struct none{};
	explicit AnalysisLogger(none):
		m_stream(std::make_unique<boost::iostreams::stream<boost::iostreams::null_sink>>(boost::iostreams::null_sink()))
	{
	}
	explicit AnalysisLogger(std::filesystem::path const & log_directory):
		m_stream(std::make_unique<std::ofstream>(open_text_file_for_writing(log_directory / "analysis.txt")))
	{
	}

	auto get() -> std::ostream & {
		return *m_stream;
	}

private:
	std::unique_ptr<std::ostream> m_stream;
};

} // namespace technicalmachine
