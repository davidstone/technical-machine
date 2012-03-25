// MD5 implementation
// Copyright (C) 2012 David Stone
//
// This program is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// Created from C version found in "md5backup.c".

#include "md5.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>
#include <string>

#include <boost/detail/endian.hpp>

#include "byte_order.hpp"

namespace technicalmachine {
namespace cryptography {
namespace {

constexpr unsigned block_size = 64;

constexpr uint32_t rotate_left (uint32_t x, unsigned n) {
	return (x << n) | (x >> (32 - n));
}

constexpr uint32_t f (uint32_t x, uint32_t y, uint32_t z) {
	return ((x ^ y) & z) ^ y;
}

constexpr uint32_t g (uint32_t x, uint32_t y, uint32_t z) {
	return x ^ y ^ z;
}

constexpr uint32_t h (uint32_t x, uint32_t y, uint32_t z) {
	return (x | ~z) ^ y;
}

void round1 (uint32_t & a, uint32_t b, uint32_t c, uint32_t d, unsigned s, uint32_t k_n, uint32_t X) {
	a = rotate_left (a + f (c, d, b) + X + k_n, s) + b;
}

void round2 (uint32_t & a, uint32_t b, uint32_t c, uint32_t d, unsigned s, uint32_t k_n, uint32_t X) {
	a = rotate_left (a + f (b, c, d) + X + k_n, s) + b;
}

void round3 (uint32_t & a, uint32_t b, uint32_t c, uint32_t d, unsigned s, uint32_t k_n, uint32_t X) {
	a = rotate_left (a + g (b, c, d) + X + k_n, s) + b;
}

void round4 (uint32_t & a, uint32_t b, uint32_t c, uint32_t d, unsigned s, uint32_t k_n, uint32_t X) {
	a = rotate_left (a + h (b, c, d) + X + k_n, s) + b;
}

static void process (std::string const & message, uint32_t digest_buffer [4]) {
	// k [n] = floor (abs (sin (n + 1)) * (2 ** 32))
	constexpr static uint32_t k [block_size] = {
		0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
		0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
		0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
		0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
		0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
		0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
		0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
		0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
		0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
		0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
		0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
		0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
		0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
		0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
		0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
		0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
	};
	for (size_t p = 0; p != message.length (); p += block_size) {
		uint32_t const * X;
		uintmax_t xbuf [block_size / sizeof (uintmax_t)];
		
		// When I get access to a big-endian system, I may change this code to
		// not rely on macros. However, this should work and I don't want to
		// change things without being able to test them.
		#if defined BOOST_LITTLE_ENDIAN
			// Verify 32-bit alignment.
			assert (reinterpret_cast <uintptr_t> (&message [p]) % sizeof (uint32_t) == 0);
			X = reinterpret_cast <uint32_t const *> (&message [p]);
		#elif defined BOOST_BIG_ENDIAN
			for (unsigned n = 0; n != block_size / sizeof (uintmax_t); ++n)
				xbuf [n] = le_to_h (*reinterpret_cast <uintmax_t const *> (&message [p + n * sizeof (uintmax_t)]));
			X = reinterpret_cast <uint32_t *> (xbuf);
		#else
			// When I add PDP-Endian to byte_order.hpp I can remove the else.
			#error Unknown byte order
		#endif

		uint32_t temp [4];
		memcpy (temp, digest_buffer, 4 * sizeof (uint32_t));

		unsigned n = 0;
		while (n != 16 * 1) {
			round1 (temp [0], temp [1], temp [2], temp [3], 7, k [n], X [n]);
			++n;
			round1 (temp [3], temp [0], temp [1], temp [2], 12, k [n], X [n]);
			++n;
			round1 (temp [2], temp [3], temp [0], temp [1], 17, k [n], X [n]);
			++n;
			round1 (temp [1], temp [2], temp [3], temp [0], 22, k [n], X [n]);
			++n;
		}
		while (n != 16 * 2) {
			round2 (temp [0], temp [1], temp [2], temp [3], 5, k [n], X [(1 + 5 * n) % 16]);
			++n;
			round2 (temp [3], temp [0], temp [1], temp [2], 9, k [n], X [(1 + 5 * n) % 16]);
			++n;
			round2 (temp [2], temp [3], temp [0], temp [1], 14, k [n], X [(1 + 5 * n) % 16]);
			++n;
			round2 (temp [1], temp [2], temp [3], temp [0], 20, k [n], X [(1 + 5 * n) % 16]);
			++n;
		}
		while (n != 16 * 3) {
			round3 (temp [0], temp [1], temp [2], temp [3], 4, k [n], X [(5 + 3 * n) % 16]);
			++n;
			round3 (temp [3], temp [0], temp [1], temp [2], 11, k [n], X [(5 + 3 * n) % 16]);
			++n;
			round3 (temp [2], temp [3], temp [0], temp [1], 16, k [n], X [(5 + 3 * n) % 16]);
			++n;
			round3 (temp [1], temp [2], temp [3], temp [0], 23, k [n], X [(5 + 3 * n) % 16]);
			++n;
		}
		while (n != 16 * 4) {
			round4 (temp [0], temp [1], temp [2], temp [3], 6, k [n], X [(7 * n) % 16]);
			++n;
			round4 (temp [3], temp [0], temp [1], temp [2], 10, k [n], X [(7 * n) % 16]);
			++n;
			round4 (temp [2], temp [3], temp [0], temp [1], 15, k [n], X [(7 * n) % 16]);
			++n;
			round4 (temp [1], temp [2], temp [3], temp [0], 21, k [n], X [(7 * n) % 16]);
			++n;
		}

		// Increment each of the four registers by the value it had before this
		// block was started.
		for (unsigned m = 0; m != 4; ++m)
			digest_buffer [m] += temp [m];
	}
}

static void add_length (std::string & message, size_t const original_size) {
	size_t const position_of_length = message.size () - sizeof (uint64_t);
	uint64_t * const length_ptr = reinterpret_cast <uint64_t *> (&message [position_of_length]);
	uint64_t const length_in_bits = original_size * 8;
	*length_ptr = h_to_le (length_in_bits);
}

}	// anonymous namespace

std::string md5 (std::string message) {
	// Assuming we have the entire message loaded into message, rather than
	// supporting partial blocks
	size_t const original_size = message.size ();
	message += static_cast <char> (0x80);
	uint64_t const size_of_non_padding = 1 + sizeof (uint64_t) + original_size;
	message.resize (size_of_non_padding + ((block_size - size_of_non_padding) % block_size), 0);
	add_length (message, original_size);
	uint32_t digest_buffer [4] = {
		0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476
	};
	process (message, digest_buffer);
	return std::string (reinterpret_cast <char *> (digest_buffer), 16);
}
}	// namespace cryptography
}	// namespace technicalmachine
