// Metafunctions that give access to an unsigned type that is equal in size to
// the template parameter and an unsigned type that is half the size of the
// template parameter.
//
// Copyright (C) 2012 David Stone
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// See http://www.boost.org/libs/endian/ for documentation.

#ifndef BOOST_ENDIAN_UNSIGNED_HPP_
#define BOOST_ENDIAN_UNSIGNED_HPP_

#include <boost/cstdint.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost {
namespace endian {
namespace detail {

template<class T, class Enable = void>
class Unsigned;

template<class T>
class Unsigned<T, typename boost::enable_if_c< ::boost::is_integral<T>::value && sizeof (T) == 2>::type> {
public:
    typedef boost::uint8_t half_t;
    typedef boost::uint16_t equal_t;
};

template<class T>
class Unsigned<T, typename boost::enable_if_c< ::boost::is_integral<T>::value && sizeof (T) == 4>::type> {
public:
    typedef boost::uint16_t half_t;
    typedef boost::uint32_t equal_t;
};

template<class T>
class Unsigned<T, typename boost::enable_if_c< ::boost::is_integral<T>::value && sizeof (T) == 8>::type> {
public:
    typedef boost::uint32_t half_t;
    typedef boost::uint64_t equal_t;
};

}    // namespace detail
}    // namespace endian
}    // namespace boost

#endif    // BOOST_ENDIAN_UNSIGNED_HPP_

