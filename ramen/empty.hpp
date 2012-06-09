// From Adobe source libraries. Original license follows.
/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

/*************************************************************************************************/

#ifndef RAMEN_EMPTY_HPP
#define RAMEN_EMPTY_HPP

#include <boost/operators.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_empty.hpp>
#include <boost/type_traits/is_pod.hpp>
#include <boost/type_traits/has_trivial_constructor.hpp>
#include <boost/type_traits/has_trivial_destructor.hpp>

#include <iosfwd>

namespace ramen
{

struct empty_t : private boost::totally_ordered<empty_t>
{
    friend inline bool operator == (const empty_t&, const empty_t&) { return true; }
    friend inline bool operator < (const empty_t&, const empty_t&) { return false; }
    friend inline void swap(empty_t&, empty_t&) { }
};

std::ostream& operator << (std::ostream& stream, const empty_t&);

} // namespace ramen

namespace boost
{

template <> struct is_pod<ramen::empty_t> : boost::mpl::true_ { };
template <> struct is_empty<ramen::empty_t> : boost::mpl::true_ { };
template <> struct has_trivial_constructor<ramen::empty_t> : boost::mpl::true_ { };
template <> struct has_trivial_destructor<ramen::empty_t> : boost::mpl::true_ { };

}

#endif
