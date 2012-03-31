// Byte reordering functions
//
// Copyright (C) 2012 David Stone
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// See http://www.boost.org/libs/endian/ for documentation.
//
// All functions for big-endian <-> little-endian conversions are defined in
// terms of be_to_le. Because this function reverses the order of the bytes, it
// is the same as a le_to_be function.
//
// For similar reasons, big-endian <-> PDP-endian will be defined in terms of
// big-endian to PDP-endian and little-endian <-> PDP-endian converions will be
// defined in terms of little-endian to PDP-endian. PDP-endian conversions are
// not yet supported, other than the trivial one-byte integer conversions.
//
// Where no built-in byte swapping function can be found, I fall back on manual
// bit shifting. The manual shifts are defined recursively, with the one-byte
// integer being the base case that simply returns itself.

#ifndef BOOST_ENDIAN_BYTE_ORDER_HPP_
#define BOOST_ENDIAN_BYTE_ORDER_HPP_

#include <climits>
#include <boost/cstdint.hpp>
#include <boost/detail/endian.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/utility/enable_if.hpp>

#include "unsigned.hpp"

// The built-in versions are much faster than the generic versions (at least for GCC they are).

#ifndef __has_builtin
    #define __has_builtin(x) 0
#endif

// I'm not sure if this is the best test. I want to make sure the header
// byteswap.h contains the builtins with the names listed below. I currently
// use clang's feature test of __has_builtin for clang, which should always
// work. If __has_builtin doesn't find the funcitons, then I check for the
// definition of __GNUC__ in the hopes that the compiler actually is GCC, and
// exclude verions of clang that fail the builtin test.

#undef BOOST_ENDIAN_NO_INTRINSICS
#undef BOOST_ENDIAN_INTRINSIC_BYTE_SWAP_2
#undef BOOST_ENDIAN_INTRINSIC_BYTE_SWAP_4
#undef BOOST_ENDIAN_INTRINSIC_BYTE_SWAP_8

#if (__has_builtin (bswap_16) && __has_builtin (bswap_32) && __has_builtin (bswap_64)) || (defined __GNUC__ && !defined __clang__)
    #include <byteswap.h>
    #define BOOST_ENDIAN_INTRINSIC_BYTE_SWAP_2 bswap_16
    #define BOOST_ENDIAN_INTRINSIC_BYTE_SWAP_4 bswap_32
    #define BOOST_ENDIAN_INTRINSIC_BYTE_SWAP_8 bswap_64
#elif defined _MSC_VER
    #include <cstdlib>
    #define BOOST_ENDIAN_INTRINSIC_BYTE_SWAP_2 _byteswap_ushort
    #define BOOST_ENDIAN_INTRINSIC_BYTE_SWAP_4 _byteswap_ulong
    #define BOOST_ENDIAN_INTRINSIC_BYTE_SWAP_8 _byteswap_uint64
#else
    #define BOOST_ENDIAN_NO_INTRINSICS
#endif

namespace boost {
namespace endian {

template<typename T>
typename boost::enable_if_c< ::boost::is_integral<T>::value && sizeof (T) == 1, T>::type
manual_byte_swap (T x) {
    return x;
}

template<typename T>
typename boost::enable_if_c< ::boost::is_integral<T>::value && sizeof (T) != 1, T>::type
manual_byte_swap (T x) {
    // I break the number into halves, and swap the order of those halves, and then
    // swap their overall order. So UNIX becomes (UN)(IX)->(NU)(XI)->XINU
    typedef typename detail::Unsigned<T>::equal_t equal_t;
    typedef typename detail::Unsigned<T>::half_t half_t;
    size_t const shift = sizeof (T) / 2 * CHAR_BIT;
    // I static_cast to an unsigned type of the same size prior to shifting
    // because of the vague behavior of a left-shift of a signed value. In
    // C++11 (5.8/2), shifting a signed negative value to the left is undefined
    // behavior. In C++98 / C++03 (5.8/2), the result is not explicitly stated
    // for signed negative values.
    equal_t const left = static_cast<equal_t> (manual_byte_swap (static_cast<half_t> (x))) << shift;
    // I static_cast to an unsigned type to avoid implementation-defined right
    // shift of signed integer.
    equal_t const right = manual_byte_swap (static_cast<half_t> (static_cast<equal_t> (x) >> shift));
    return static_cast<T> (left | right);
}

template<typename T>
typename boost::enable_if_c< ::boost::is_integral<T>::value && sizeof (T) == 1, T>::type
intrinsic_byte_swap (T x) {
    return x;
}

template<typename T>
typename boost::enable_if_c< ::boost::is_integral<T>::value && sizeof (T) == 2, T>::type
intrinsic_byte_swap (T x) {
    typedef typename detail::Unsigned<T>::equal_t equal_t;
    return static_cast<T> (BOOST_ENDIAN_INTRINSIC_BYTE_SWAP_2 (static_cast<equal_t> (x)));
}

template<typename T>
typename boost::enable_if_c< ::boost::is_integral<T>::value && sizeof (T) == 4, T>::type
intrinsic_byte_swap (T x) {
    typedef typename detail::Unsigned<T>::equal_t equal_t;
    return static_cast<T> (BOOST_ENDIAN_INTRINSIC_BYTE_SWAP_4 (static_cast<equal_t> (x)));
}

template<typename T>
typename boost::enable_if_c< ::boost::is_integral<T>::value && sizeof (T) == 8, T>::type
intrinsic_byte_swap (T x) {
    typedef typename detail::Unsigned<T>::equal_t equal_t;
    return static_cast<T> (BOOST_ENDIAN_INTRINSIC_BYTE_SWAP_8 (static_cast<equal_t> (x)));
}

template<typename T>
T be_to_le (T x) {
    #if defined BOOST_ENDIAN_NO_INTRINSICS
        return manual_byte_swap (x);
    #else
        return intrinsic_byte_swap (x);
    #endif
}

#undef BOOST_ENDIAN_NO_INTRINSICS
#undef BOOST_ENDIAN_INTRINSIC_BYTE_SWAP_2
#undef BOOST_ENDIAN_INTRINSIC_BYTE_SWAP_4
#undef BOOST_ENDIAN_INTRINSIC_BYTE_SWAP_8

template<typename T>
T le_to_be (T x) {
    return be_to_le (x);
}


template<typename T>
typename boost::enable_if_c< ::boost::is_integral<T>::value && sizeof (T) < 4, T>::type
be_to_pdp (T x) {
    return be_to_le (x);
}


// I have to make the check be >= 4 to allow dynamic endian conversion
// routines. PDP Endianness isn't well defined for 8-byte numbers, so I'll just
// define it as one possible extension of the 4-byte version, which flows
// naturally from my template: DCBAHGFE
template<typename T>
typename boost::enable_if_c< ::boost::is_integral<T>::value && sizeof (T) >= 4, T>::type
be_to_pdp (T x) {
    typedef typename detail::Unsigned<T>::equal_t equal_t;
    typedef typename detail::Unsigned<T>::half_t half_t;
    size_t const shift = sizeof (T) / 2 * CHAR_BIT;
    half_t const unordered = static_cast<half_t> (static_cast<equal_t> (x) >> shift);
    equal_t const left = static_cast<equal_t> (be_to_le (unordered)) << shift;
    equal_t const right = be_to_le (static_cast<half_t> (x));
    return static_cast<T> (left | right);
}



template<typename T>
typename boost::enable_if_c< ::boost::is_integral<T>::value && sizeof (T) < 4, T>::type
le_to_pdp (T x) {
    return x;
}

template<typename T>
typename boost::enable_if_c< ::boost::is_integral<T>::value && sizeof (T) >= 4, T>::type
le_to_pdp (T x) {
    typedef typename detail::Unsigned<T>::equal_t equal_t;
    typedef typename detail::Unsigned<T>::half_t half_t;
    size_t const shift = sizeof (T) / 2 * CHAR_BIT;
    equal_t const left = static_cast<equal_t> (x) << shift;
    equal_t const right = static_cast<equal_t> (x) >> shift;
    return static_cast<T> (left | right);
}

template<typename T>
typename boost::enable_if_c< ::boost::is_integral<T>::value, T>::type
pdp_to_be (T x) {
    return be_to_pdp (x);
}

template<typename T>
typename boost::enable_if_c< ::boost::is_integral<T>::value, T>::type
pdp_to_le (T x) {
    return le_to_pdp (x);
}


// In general, the h_to_format functions are not the same as the format_to_h
// functions. Put another way, x == h_to_format (h_to_format (x)) will not
// always be true. That they are for the three known endians is chance. For
// instance, consider a new byte order, "Example-Endian", the byte order 2314.
// If h_to_be (or any other host function defined below) were defined as simply
// be_to_h, then calling h_to_be (be_to_h (x)) would not give x, as it should,
// but rather, a number with bytes ordered 3124 relative to the original (so
// they would be 1234 on this architecture). Instead, we'd need an extra call:
// x == h_to_be (h_to_be (h_to_be (x))). If I ever need to add a new byte order
// such as my Example-Endian, I'll have to rewrite the format_to_h functions to
// not be defined as their inverse for dynamic byte ordering.

namespace detail {

enum ByteOrder { big_endian, little_endian, pdp_endian };

inline ByteOrder determine_byte_order () {
    boost::uint32_t const n = 0xAABBCCDD;
    switch (*reinterpret_cast<boost::uint8_t const *> (&n)) {
        case 0xDD:
            return little_endian;
        case 0xAA:
            return big_endian;
        default:    // 0xBB
            return pdp_endian;
    }
}

}    // namespace detail


#if defined BOOST_BIG_ENDIAN

template<typename T>
T h_to_be (T x) {
    return x;
}

template<typename T>
T be_to_h (T x) {
    return x;
}

template<typename T>
T h_to_le (T x) {
    return be_to_le (x);
}

template<typename T>
T le_to_h (T x) {
    return le_to_be (x);
}

template<typename T>
T h_to_pdp (T x) {
    return be_to_pdp (x);
}

template<typename T>
T pdp_to_h (T x) {
    return pdp_to_be (x);
}

#elif defined BOOST_LITTLE_ENDIAN

template<typename T>
T h_to_be (T x) {
    return le_to_be (x);
}

template<typename T>
T be_to_h (T x) {
    return be_to_le (x);
}

template<typename T>
T h_to_le (T x) {
    return x;
}

template<typename T>
T le_to_h (T x) {
    return x;
}

template<typename T>
T h_to_pdp (T x) {
    return le_to_pdp (x);
}

template<typename T>
T pdp_to_h (T x) {
    return pdp_to_le (x);
}

#elif defined BOOST_PDP_ENDIAN

template<typename T>
T h_to_be (T x) {
    return pdp_to_be (x);
}

template<typename T>
T be_to_h (T x) {
    return be_to_pdp (x);
}

template<typename T>
T h_to_le (T x) {
    return pdp_to_le (x);
}

template<typename T>
T le_to_h (T x) {
    return le_to_pdp (x);
}

template<typename T>
T h_to_pdp (T x) {
    return x;
}

template<typename T>
T pdp_to_h (T x) {
    return x;
}

#else    // Determine byte order dynamically.

// I could assign the result of detail::determine_byte_order () to a bool const
// to avoid recalculating it on every run. I'm not sure of the best way to do
// this so that dynamic byte ordering can be tested and the most common
// implementation (where static byte ordering is possible) doesn't have any
// performance penalty at all.

namespace detail {

ByteOrder const byte_order = detail::determine_byte_order ();

template<typename T>
T dynamic_h_to_be (T x) {
    switch (byte_order) {
        case little_endian:
            return le_to_be (x);
        case big_endian:
            return x;
        case pdp_endian:
            return pdp_to_be (x);
        default:
            break;
    }
}

template<typename T>
T dynamic_h_to_le (T x) {
    switch (byte_order) {
        case little_endian:
            return x;
        case big_endian:
            return be_to_le (x);
        case pdp_endian:
            return pdp_to_le (x);
        default:
            break;
    }
}

template<typename T>
T dynamic_h_to_pdp (T x) {
    switch (byte_order) {
        case little_endian:
            return le_to_pdp (x);
        case big_endian:
            return be_to_pdp (x);
        case pdp_endian:
            return x;
        default:
            break;
    }
}
}    // namespace detail

template<typename T>
T h_to_be (T x) {
    return detail::dynamic_h_to_be (x);
}

template<typename T>
T h_to_le (T x) {
    return detail::dynamic_h_to_le (x);
}

template<typename T>
T h_to_pdp (T x) {
    return detail::dynamic_h_to_to_pdp (x);
}

template<typename T>
T be_to_h (T x) {
    return h_to_be (x);
}

template<typename T>
T le_to_h (T x) {
    return h_to_le (x);
}

template<typename T>
T pdp_to_h (T x) {
    return h_to_pdp (x);
}

#endif

template<typename T>
T h_to_n (T x) {
    return h_to_be (x);
}

template<typename T>
T n_to_h (T x) {
    return be_to_h (x);
}

}    // namespace endian
}    // namespace boost
#endif    // BOOST_ENDIAN_BYTE_ORDER_HPP_

