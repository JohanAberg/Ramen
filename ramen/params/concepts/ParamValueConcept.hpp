// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_PARAM_VALUE_CONCEPT_HPP
#define RAMEN_PARAM_VALUE_CONCEPT_HPP

#include<boost/any.hpp>

#include<ramen/concepts/RegularConcept.hpp>

namespace ramen
{

/**
\ingroup concepts
\brief concept checking class for param values
*/
template<class T>
struct ParamValueConcept :  boost::DefaultConstructible<T>,
                            boost::CopyConstructible<T>,
                            boost::Assignable<T>,
                            boost::EqualityComparable<T>
{
private:

    BOOST_CONCEPT_ASSERT(( RegularConcept<T>));

public:

	BOOST_CONCEPT_USAGE( ParamValueConcept)
	{
    }
};

/**
\ingroup concepts
\brief concept checking class for param values that have operator[] defined
*/
template<class T>
struct IndexableParamValueConcept : boost::DefaultConstructible<T>,
                                    boost::CopyConstructible<T>,
                                    boost::Assignable<T>,
                                    boost::EqualityComparable<T>
{
private:

    BOOST_CONCEPT_ASSERT(( ParamValueConcept<T>));

public:

	BOOST_CONCEPT_USAGE( IndexableParamValueConcept)
	{
        boost::any anything( t[0]);
	}

    T t;
};

} // ramen

#endif
