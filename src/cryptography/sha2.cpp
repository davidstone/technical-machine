// Sha-2 implementation
// Copyright (C) 2011 David Stone
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

// This file was inspired by C implementation by Oliver Gay.
// Original source can be found in src/sha.c

// This version runs faster than the unrolled version while almost having an
// executable smaller than the non-unrolled version. This size increase comes
// primarily from the use of std::string. If used with a C++ project that
// already uses std::string (like most C++ projects most likely do), some
// informal testing showed that this version resulted in a smaller overall
// executable. The source code is also almost 1/3 of the size of the C version
// and does define any macros.

// This version supports message lengths of up to 64-bits for any hash type.
// The spec requires support for 128-bit messages for SHA-384 and SHA-512.
// The C version only supported 32-bit message lengths.

#include "sha2.hpp"
#include <cstdint>
#include <cstring>
#include <string>
#include "byte_order.hpp"

namespace technicalmachine {
namespace cryptography {
namespace {

template <typename T>
constexpr T inline rotate_right (T x, unsigned n) {
	return ((x >> n) | (x << ((sizeof (T) << 3) - n)));
}

constexpr uint32_t inline f1 (uint32_t x) {
	return rotate_right (x, 2) ^ rotate_right (x, 13) ^ rotate_right (x, 22);
}
constexpr uint64_t inline f1 (uint64_t x) {
	return rotate_right (x, 28) ^ rotate_right (x, 34) ^ rotate_right (x, 39);
}

constexpr uint32_t inline f2 (uint32_t x) {
	return rotate_right (x, 6) ^ rotate_right (x, 11) ^ rotate_right (x, 25);
}
constexpr uint64_t inline f2 (uint64_t x) {
	return rotate_right (x, 14) ^ rotate_right (x, 18) ^ rotate_right (x, 41);
}

constexpr uint32_t inline f3 (uint32_t x) {
	return rotate_right (x, 7) ^ rotate_right (x, 18) ^ (x >> 3);
}
constexpr uint64_t inline f3 (uint64_t x) {
	return rotate_right (x, 1) ^ rotate_right (x, 8) ^ (x >> 7);
}

constexpr uint32_t inline f4 (uint32_t x) {
	return rotate_right (x, 17) ^ rotate_right (x, 19) ^ (x >> 10);
}
constexpr uint64_t inline f4 (uint64_t x) {
	return rotate_right (x, 19) ^ rotate_right (x, 61) ^ (x >> 6);
}

template <typename T>
constexpr T inline ch (T x, T y, T z) {
	return (x & (y ^ z)) ^ z;
}

template <typename T>
constexpr T inline maj (T x, T y, T z) {
	return (x & y) | ((x | y) & z);
}

template <typename T>
constexpr void inline scr (T * w) {
	*w = f4 (*(w - 2)) + *(w - 7) + f3 (*(w - 15)) + *(w - 16);
}

template <typename T>
static void exp (T a, T b, T c, T & d, T e, T f, T g, T & h, int j, T const k [], T const w []) {
	T t1 = h + f2 (e) + ch (e, f, g) + k [j] + w [j];
	T t2 = f1 (a) + maj (a, b, c);
	d += t1;
	h = t1 + t2;
}

template <unsigned block_size, unsigned array_size, typename T>
static void transform (std::string const & message, unsigned const number_of_blocks, T h [8], T const k []) {
	T w [array_size];
	T wv [8];
	for (unsigned i = 0; i < number_of_blocks; ++i) {
		
		// Cannot work in larger chunks due to endian issues.
		T const * sub_message = reinterpret_cast <T const *> (&message [i * block_size]);
		for (unsigned j = 0; j < 16; ++j)
			w [j] = h_to_be (*(sub_message + j));

		for (unsigned j = 16; j < array_size; ++j)
			scr (&w [j]);

		memcpy (wv, h, 8 * sizeof (T));

		for (unsigned j = 0; j < array_size;) {
			exp (wv [0], wv [1], wv [2], wv [3], wv [4], wv [5], wv [6], wv [7], j, k, w);
			++j;
			exp (wv [7], wv [0], wv [1], wv [2], wv [3], wv [4], wv [5], wv [6], j, k, w);
			++j;
			exp (wv [6], wv [7], wv [0], wv [1], wv [2], wv [3], wv [4], wv [5], j, k, w);
			++j;
			exp (wv [5], wv [6], wv [7], wv [0], wv [1], wv [2], wv [3], wv [4], j, k, w);
			++j;
			exp (wv [4], wv [5], wv [6], wv [7], wv [0], wv [1], wv [2], wv [3], j, k, w);
			++j;
			exp (wv [3], wv [4], wv [5], wv [6], wv [7], wv [0], wv [1], wv [2], j, k, w);
			++j;
			exp (wv [2], wv [3], wv [4], wv [5], wv [6], wv [7], wv [0], wv [1], j, k, w);
			++j;
			exp (wv [1], wv [2], wv [3], wv [4], wv [5], wv [6], wv [7], wv [0], j, k, w);
			++j;
		}

		// Cannot work in larger chunks due to requiring addition modulo T.
		for (unsigned j = 0; j < 8; ++j)
			h [j] += wv [j];
	}
}

static void add_length (std::string & message, unsigned const original_size) {
	// I'm supposed to support up to 128-bits for the message length for SHA-384 / 512.
	unsigned const position_of_length = message.size () - sizeof (uint64_t);
	uint64_t * const length_ptr = reinterpret_cast <uint64_t *> (&message [position_of_length]);
	uint64_t const bit_length = (original_size) * 8;
	*length_ptr = be_to_h (bit_length);
}

template <unsigned block_size, typename T>
static void format_digest (unsigned const words, T h [8], std::string & digest) {
	digest.resize (words * (block_size / 16));
	for (unsigned i = 0 ; i < words; ++i) {
		T * ptr_T = reinterpret_cast <T *> (&digest [i * (block_size / 16)]);
		*ptr_T = h_to_be (h [i]);
	}
}

template <unsigned block_size, unsigned array_size, typename T>
class Sha {
	public:
		static void update (std::string message, T h [8], T const k [], unsigned const words, std::string & digest) {
			unsigned const original_size = message.size ();
			unsigned const number_of_full_blocks = original_size / block_size;

			unsigned const size_of_full_blocks = number_of_full_blocks * block_size;
			message.append (1, static_cast <char> (0x80));
	
			unsigned const remaining_length = original_size - size_of_full_blocks;
			// Not the length of the message, but the bytes in the integer used to give the length of the message
			constexpr unsigned bytes_in_message_length = block_size / 8;
			// Subtract 1 for the byte set at the end of the message
			constexpr unsigned block_sizer = block_size - bytes_in_message_length - 1;
			// I have to do the comparison instead of just adding 1 because the
			// message might be long enough to where the addition of the byte
			// 0x80 and the length of the original message pushes it above the
			// size of one other block, but the message starts out not evenly
			// divisible by the block_size.
			unsigned const number_of_extra_blocks = 1 + (block_sizer < remaining_length);
			unsigned const size_of_extra_blocks = number_of_extra_blocks * block_size;
			message.resize (size_of_full_blocks + size_of_extra_blocks, 0);

			add_length (message, original_size);

			transform <block_size, array_size> (message, number_of_full_blocks + number_of_extra_blocks, h, k);
	
			format_digest <block_size> (words, h, digest);
		}
};

class Sha_224_256 : public Sha <512 / 8, 64, uint32_t> {
	public:
		static uint32_t const k [64];
};

class Sha_384_512 : public Sha <1024 / 8, 80, uint64_t> {
	public:
		static uint64_t const k [80];
};

// First 32 bits of fractional part of the cube roots of the first 64 primes.
uint32_t const Sha_224_256::k [] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

// First 64 bits of the fractional part of the cube roots of the first 64 primes.
uint64_t const Sha_384_512::k [] = {
	0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL,
	0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
	0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
	0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
	0xd807aa98a3030242ULL, 0x12835b0145706fbeULL,
	0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
	0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL,
	0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
	0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
	0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
	0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL,
	0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
	0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL,
	0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL,
	0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
	0x06ca6351e003826fULL, 0x142929670a0e6e70ULL,
	0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL,
	0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
	0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL,
	0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
	0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL,
	0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL,
	0xd192e819d6ef5218ULL, 0xd69906245565a910ULL,
	0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
	0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL,
	0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL,
	0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL,
	0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL,
	0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL,
	0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
	0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL,
	0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL,
	0xca273eceea26619cULL, 0xd186b8c721c0c207ULL,
	0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL,
	0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL,
	0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
	0x28db77f523047d84ULL, 0x32caab7b40c72493ULL,
	0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL,
	0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL,
	0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
};

template <class Sha_t, typename T>
static void sha (std::string const & message, T h [8], unsigned words, std::string & digest) {
	Sha_t ctx;
	ctx.update (message, h, ctx.k, words, digest);
}

}	// anonymous namespace

std::string sha224 (std::string const & message) {
	constexpr static unsigned words = 224 / 8 / sizeof (uint32_t);	// 7
	// Second 32 bits of the fractional parts of the square root of the 9th through 16th primes.
	uint32_t h [8] = {
		0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939,
		0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4
	};
	std::string digest;
	sha <Sha_224_256> (message, h, words, digest);
	return digest;
}

std::string sha256 (std::string const & message) {
	constexpr static unsigned words = 256 / 8 / sizeof (uint32_t);	// 8
	// First 32 bits of the fractional part of the square root of the first 8 primes.
	uint32_t h [8] = {
		0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
		0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
	};
	std::string digest;
	sha <Sha_224_256> (message, h, words, digest);
	return digest;
}

std::string sha384 (std::string const & message) {
	constexpr static unsigned words = 384 / 8 / sizeof (uint64_t);	// 6
	// First 64 bits of the fractional parts of the square root of the 9th through 16th primes.
	uint64_t h [8] = {
		0xcbbb9d5dc1059ed8ULL, 0x629a292a367cd507ULL,
		0x9159015a3070dd17ULL, 0x152fecd8f70e5939ULL,
		0x67332667ffc00b31ULL, 0x8eb44a8768581511ULL,
		0xdb0c2e0d64f98fa7ULL, 0x47b5481dbefa4fa4ULL
	};
	std::string digest;
	sha <Sha_384_512> (message, h, words, digest);
	return digest;
}

std::string sha512 (std::string const & message) {
	constexpr static unsigned words = 512 / 8 / sizeof (uint64_t);	// 8
	// First 64 bits of the square root of the first 8 primes.
	uint64_t h [8] = {
		0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
		0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
		0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
		0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
	};
	std::string digest;
	sha <Sha_384_512> (message, h, words, digest);
	return digest;
}

}	// namespace cryptography
}	// namespace technicalmachine
