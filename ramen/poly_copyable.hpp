// From Adobe source libraries. Original license follows.
/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

#ifndef RAMEN_POLY_COPYABLE_HPP
#define RAMEN_POLY_COPYABLE_HPP

#include<ramen/poly.hpp>

#include<boost/concept_check.hpp>

namespace ramen
{

template <typename T>
struct poly_copyable_instance : optimized_storage_type<T, poly_copyable_interface>::type
{
    BOOST_CONCEPT_ASSERT(( boost::CopyConstructibleConcept<T>));

    typedef typename optimized_storage_type<T, poly_copyable_interface>::type base_type;

    poly_copyable_instance( const T& x) : base_type( x) {}

    RAMEN_POLY_INLINE_COPY_AND_ASSIGN( poly_copyable_instance, base_type)
};

struct copyable : poly_base<poly_copyable_interface, poly_copyable_instance>
{
    typedef poly_base<poly_copyable_interface, poly_copyable_instance> base_type;

    template <typename T>
    explicit copyable( const T& s) : base_type( s)  {}

    RAMEN_POLY_INLINE_COPY_AND_ASSIGN( copyable, base_type)
};

typedef poly<copyable> poly_copyable_t;

} // base

#endif
