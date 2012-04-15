// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_PARAM_VALUE_CONCEPT_HPP
#define RAMEN_PARAM_VALUE_CONCEPT_HPP

#include<boost/concept_check.hpp>

namespace ramen
{

/**
\ingroup concepts
\brief concept checking class for param values
*/
template<class Value>
struct ParamValueConcept : boost::CopyConstructible<Value>
{
	// concept checking
	BOOST_CONCEPT_USAGE( ParamValueConcept)
	{
	}
};

/**
\ingroup concepts
\brief concept checking class for param values that have operator[] defined
*/
template<class Value>
struct IndexableParamValueConcept : boost::CopyConstructible<Value>
{
	// concept checking
	BOOST_CONCEPT_USAGE( IndexableParamValueConcept)
	{
	}
};

} // ramen

#endif
