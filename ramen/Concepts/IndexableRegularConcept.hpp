// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_INDEXABLE_REGULAR_CONCEPT_HPP
#define RAMEN_INDEXABLE_REGULAR_CONCEPT_HPP

#include<boost/concept_check.hpp>
#include<boost/swap.hpp>

#include<base/empty.hpp>

namespace ramen
{

template <class T>
struct IndexableRegularConcept :	boost::CopyConstructible<T>,
                                    boost::Assignable<T>,
                                    boost::EqualityComparable<T>
{
    BOOST_CONCEPT_USAGE( IndexableRegularConcept)
    {
        boost::swap( t, t);
        checkIndexOperator( t);
    }

private:

    template<class X>
    static void checkIndexOperator( const X& x)
    {
        boost::any anything( x[0]);
    }

    static void checkIndexOperator( const base::empty_t& x) {}

    T t;
};

} // ramen

#endif
