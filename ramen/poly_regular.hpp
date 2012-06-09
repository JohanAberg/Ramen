// From Adobe source libraries. Original license follows.
/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

#ifndef RAMEN_POLY_REGULAR_HPP
#define RAMEN_POLY_REGULAR_HPP

#include<ramen/concepts/RegularConcept.hpp>
#include<ramen/poly.hpp>
#include<ramen/empty.hpp>

namespace ramen
{

struct poly_regular_interface : poly_copyable_interface
{
    virtual bool equals( const poly_regular_interface& new_value) const = 0;
};

template <typename T>
class poly_regular_instance : public optimized_storage_type<T, poly_regular_interface>::type
{
    BOOST_CONCEPT_ASSERT(( RegularConcept<T>));

public:

    typedef typename optimized_storage_type<T, poly_regular_interface>::type base_t;

    poly_regular_instance( const T& x) : base_t(x) {}

    RAMEN_POLY_INLINE_COPY_AND_ASSIGN( poly_regular_instance, base_t)

    bool equals( const poly_regular_interface& x) const
    {
        return this->type_info() == x.type_info() && this->get() == *static_cast<const T*>(x.cast());
    }
};

class regular : public poly_base<poly_regular_interface, poly_regular_instance>
{
public:

    typedef poly_base<poly_regular_interface, poly_regular_instance> base_t;

    regular() : base_t( empty_t()) {}

    template <typename T>
    explicit regular( const T& s) : base_t( s) {}

    RAMEN_POLY_INLINE_COPY_AND_ASSIGN( regular, base_t)

    bool is_empty() const { return type_info() == typeid( ramen::empty_t);}
};

typedef poly<regular> poly_regular_t;

} // base

#endif
