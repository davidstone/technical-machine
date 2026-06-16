// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <doctest/doctest.h>

export module tm.test.web_server.normalize_requested_file;

import tm.web_server.normalize_requested_file;
import std_module;

namespace technicalmachine {
namespace {

TEST_CASE("normalize_requested_file: Valid paths normalized") {
	CHECK(normalize_requested_file("/", "f.txt") == "/f.txt");
	CHECK(normalize_requested_file("/a", "f.txt") == "/a/f.txt");
	CHECK(normalize_requested_file("/a/", "f.txt") == "/a/f.txt");
	CHECK(normalize_requested_file("/a", "/f.txt") == "/a/f.txt");
	CHECK(normalize_requested_file("/a/", "/f.txt") == "/a/f.txt");
	CHECK(normalize_requested_file("/a", "b/f.txt") == "/a/b/f.txt");
	CHECK(normalize_requested_file("/a/", "b/f.txt") == "/a/b/f.txt");
	CHECK(normalize_requested_file("/a/", "/a/f.txt") == "/a/a/f.txt");
	CHECK(normalize_requested_file("/a", "/a/b/../f.txt") == "/a/a/f.txt");
	CHECK(normalize_requested_file("/a", "/a/../f.txt") == "/a/f.txt");
	CHECK(normalize_requested_file("/a", "/b") == "/a/b");
}

TEST_CASE("normalize_requested_file: Unnamed file becomes index") {
	CHECK(normalize_requested_file("/", "") == "/index.html");
	CHECK(normalize_requested_file("/", "/") == "/index.html");
	CHECK(normalize_requested_file("/a/", "/") == "/a/index.html");
	CHECK(normalize_requested_file("/", "a/") == "/a/index.html");
}

TEST_CASE("normalize_requested_file: Relative root throws") {
	CHECK_THROWS(normalize_requested_file("", "f.txt"));
	CHECK_THROWS(normalize_requested_file("a", "f.txt"));
	CHECK_THROWS(normalize_requested_file("./", "f.txt"));
}

TEST_CASE("normalize_requested_file: Path outside of root throws") {
	CHECK_THROWS(normalize_requested_file("/a", "../f.txt"));
	CHECK_THROWS(normalize_requested_file("/a", "/../f.txt"));
}

} // namespace
} // namespace technicalmachine
