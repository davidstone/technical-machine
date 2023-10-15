// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>

#include <boost/iostreams/stream.hpp>

export module tm.evaluate.analysis_logger;

import tm.open_file;

import std_module;

namespace technicalmachine {

using Manipulator = auto (*)(std::ostream &) -> std::ostream &;

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

	friend auto operator<<(AnalysisLogger & logger, auto const & value) -> AnalysisLogger & {
		*logger.m_stream << value;
		return logger;
	}
	friend auto operator<<(AnalysisLogger & logger, Manipulator function) -> AnalysisLogger & {
		*logger.m_stream << function;
		return logger;
	}

private:
	std::unique_ptr<std::ostream> m_stream;
};

} // namespace technicalmachine
