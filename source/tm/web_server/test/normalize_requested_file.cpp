// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <catch2/catch_test_macros.hpp>

export module tm.test.web_server.normalize_requested_file;

import tm.web_server.normalize_requested_file;
import std_module;

namespace technicalmachine {
namespace {

TEST_CASE("Valid paths normalized", "[normalize_requested_file]") {
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

TEST_CASE("Unnamed file becomes index", "[normalize_requested_file]") {
	CHECK(normalize_requested_file("/", "") == "/index.html");
	CHECK(normalize_requested_file("/", "/") == "/index.html");
	CHECK(normalize_requested_file("/a/", "/") == "/a/index.html");
	CHECK(normalize_requested_file("/", "a/") == "/a/index.html");
}

TEST_CASE("Relative root throws", "[normalize_requested_file]") {
	CHECK_THROWS(normalize_requested_file("", "f.txt"));
	CHECK_THROWS(normalize_requested_file("a", "f.txt"));
	CHECK_THROWS(normalize_requested_file("./", "f.txt"));
}

TEST_CASE("Path outside of root throws", "[normalize_requested_file]") {
	CHECK_THROWS(normalize_requested_file("/a", "../f.txt"));
	CHECK_THROWS(normalize_requested_file("/a", "/../f.txt"));
}

} // namespace
} // namespace technicalmachine
