// Byte reordering functions
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

#ifndef CRYPTOGRAPHY_BYTE_ORDER_H_
#define CRYPTOGRAPHY_BYTE_ORDER_H_

#include <cstdint>
#include <boost/detail/endian.hpp>

namespace technicalmachine {
namespace cryptography {

constexpr uint8_t inline be_to_le (uint8_t x) {
	return x;
}

}	// namespace technicalmachine
}	// namespace cryptography

#if defined __GNUC__
#include <byteswap.h>

namespace technicalmachine {
namespace cryptography {

// The built-in versions are much faster than the generic versions.

uint16_t inline be_to_le (uint16_t x) {
	return bswap_16 (x);
}

uint32_t inline be_to_le (uint32_t x) {
	return bswap_32 (x);
}

uint64_t inline be_to_le (uint64_t x) {
	return bswap_64 (x);
}

}	// namespace cryptography
}	// namespace technicalmachine

#else	// !defined __GNUC__
// Should probably add support for other compiler's built-ins.

namespace technicalmachine {
namespace cryptography {

constexpr uint16_t inline be_to_le (uint16_t x) {
	return (x << 8) | (x >> 8);
}

constexpr uint32_t inline be_to_le (uint32_t x) {
	// The static_cast <uint32_t> is there because there is only an implicit
	// conversion to int, which is only guaranteed to be a 16-bit integer. On
	// systems with 16-bit integers, there would not be enough width to shift
	// 16 bits.
	return (static_cast <uint32_t> (be_to_le (static_cast <uint16_t> (x))) << 16) | static_cast <uint32_t> (be_to_le (static_cast <uint16_t> (x >> 16)));
}

constexpr uint64_t inline be_to_le (uint64_t x) {
	return (static_cast <uint64_t> (be_to_le (static_cast <uint32_t> (x))) << 32) | static_cast <uint64_t> (be_to_le (static_cast <uint32_t> (x >> 32)));

	#if 0
	Profiling showed the above to be much faster than this:
	return ((x << 56) |
		((x << 40) & (0xffULL << 48)) |
		((x << 24) & (0xffULL << 40)) |
		((x << 8)  & (0xffULL << 32)) |
		((x >> 8)  & (0xffULL << 24)) |
		((x >> 24) & (0xffULL << 16)) |
		((x >> 40) & (0xffULL << 8)) |
		(x  >> 56));
	#endif
}

}	// namespace cryptography
}	// namespace technicalmachine

#endif	// defined __GNUC__

namespace technicalmachine {
namespace cryptography {

// Little endian to big endian

template <typename T>
T inline le_to_be (T x) {
	return be_to_le (x);
}

#ifdef BOOST_BIG_ENDIAN
// Host to big endian

template <typename T>
constexpr T inline h_to_be (T x) {
	return x;
}
// Big endian to host

template <typename T>
constexpr T inline be_to_h (T x) {
	return x;
}

// Host to little endian

template <typename T>
T inline h_to_le (T x) {
	return be_to_le (x);
}

// Little endian to host

template <typename T>
T inline le_to_h (T x) {
	return le_to_be (x);
}

#elif defined BOOST_LITTLE_ENDIAN
// Host to big endian

template <typename T>
T inline h_to_be (T x) {
	return le_to_be (x);
}

// Big endian to host

template <typename T>
T inline be_to_h (T x) {
	return be_to_le (x);
}

// Host to little endian

template <typename T>
constexpr T inline h_to_le (T x) {
	return x;
}

// Little endian to host

template <typename T>
constexpr T inline le_to_h (T x) {
	return x;
}

#else
// Should probably support PDP-Endian at some point
#error Unknown byte order
#endif

template <typename T>
T inline h_to_n (T x) {
	return h_to_be (x);
}

template <typename T>
T inline n_to_h (T x) {
	return be_to_h (x);
}

}	// namespace cryptography
}	// namespace technicalmachine
#endif	// CRYPTOGRAPHY_BYTE_ORDER_H_
