// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_PARAMS_COMPOSITE_PARAMETERISED_HPP
#define	RAMEN_PARAMS_COMPOSITE_PARAMETERISED_HPP

#include<ramen/params/parameterised.hpp>

namespace ramen
{

/**
\ingroup params
\brief Base class for parameterised objects that can contain children parameterised objects.
*/
class RAMEN_API composite_parameterised_t : public parameterised_t
{
public:

	virtual ~composite_parameterised_t();

protected:

	composite_parameterised_t();

	composite_parameterised_t( const composite_parameterised_t& other);
	void operator=( const parameterised_t& other);
};

} // namespace

#endif
