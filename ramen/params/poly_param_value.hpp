// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_PARAM_VALUE_HPP
#define RAMEN_PARAM_VALUE_HPP

#include<boost/static_assert.hpp>
#include<boost/type_traits/is_same.hpp>

#include<ramen/params/concepts/ParamValueConcept.hpp>

#include<ramen/poly_regular.hpp>
#include<ramen/empty.hpp>

namespace ramen
{

struct poly_param_value_interface : poly_regular_interface {};

template <typename T>
struct poly_param_value_instance : optimized_storage_type<T, poly_param_value_interface>::type
{
private:

    BOOST_CONCEPT_ASSERT(( ParamValueConcept<T>));

public:

    typedef typename optimized_storage_type<T, poly_param_value_interface>::type base_t;

    poly_param_value_instance( const T& x) : base_t( x) {}

    RAMEN_POLY_INLINE_COPY_AND_ASSIGN( poly_param_value_instance, base_t)

    bool equals( const poly_regular_interface& x) const
    {
        return this->type_info() == x.type_info() && this->get() == *static_cast<const T*>( x.cast());
    }
};

struct param_value : poly_base<poly_param_value_interface, poly_param_value_instance>
{
    typedef poly_base<poly_param_value_interface, poly_param_value_instance> base_t;

    param_value() : base_t( empty_t()) {}

    template <typename T>
    explicit param_value( const T& s) : base_t( s) {}

    RAMEN_POLY_INLINE_COPY_AND_ASSIGN( param_value, base_t)

    bool is_empty() const { return type_info() == typeid( empty_t);}
};

typedef poly<param_value> poly_param_value_t;

// Indexable values ( vectors, colors, ...)
struct poly_param_indexable_value_interface : poly_param_value_interface
{
    virtual float get_component( int index) const = 0;
    virtual void set_component( int index, float x) = 0;
};

template <typename T>
struct poly_param_indexable_value_instance : optimized_storage_type<T, poly_param_indexable_value_interface>::type
{
private:

    BOOST_CONCEPT_ASSERT(( IndexableParamValueConcept<T>));

public:

    typedef typename optimized_storage_type<T, poly_param_indexable_value_interface>::type base_t;

    poly_param_indexable_value_instance(const T& x) : base_t( x) {}

    RAMEN_POLY_INLINE_COPY_AND_ASSIGN( poly_param_indexable_value_instance, base_t)

    bool equals( const poly_regular_interface& x) const
    {
        return this->type_info() == x.type_info() && this->get() == *static_cast<const T*>( x.cast());
    }

    float get_component( int index) const
    {
        assert( this->type_info() != typeid( empty_t()));
        assert( index >= 0);
        assert( index * sizeof( float) < sizeof( T));

        const float *ptr = reinterpret_cast<const float *>( &( this->get()));
        return ptr[ index];
    }

    void set_component( int index, float x)
    {
        assert( this->type_info() != typeid( empty_t()));
        assert( index >= 0);
        assert( index * sizeof( float) < sizeof( T));

        float *ptr = reinterpret_cast<float *>( &( this->get()));
        ptr[ index] = x;
    }
};

struct param_indexable_value : poly_base<poly_param_indexable_value_interface, poly_param_indexable_value_instance>
{
    typedef poly_base<poly_param_indexable_value_interface, poly_param_indexable_value_instance> base_t;

    param_indexable_value() : base_t( empty_t()) {}

    template <typename T>
    explicit param_indexable_value( const T& s) : base_t( s) {}

    RAMEN_POLY_INLINE_COPY_AND_ASSIGN( param_indexable_value, base_t)

    bool is_empty() const { return type_info() == typeid( empty_t);}

    float get_component( int index) const   { return interface_ref().get_component( index);}
    void set_component( int index, float x) { interface_ref().set_component( index, x);}
};

typedef poly<param_indexable_value> poly_param_indexable_value_t;

} // namespace

#endif
