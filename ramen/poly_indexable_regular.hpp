// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_POLY_INDEXABLE_REGULAR_HPP
#define RAMEN_POLY_INDEXABLE_REGULAR_HPP

#include<base/poly_regular.hpp>

#include<ramen/Concepts/IndexableRegularConcept.hpp>

namespace ramen
{

// Indexable values ( vectors, colors, ...)
struct poly_indexable_regular_interface : base::poly_regular_interface
{
    virtual float get_component( int index) const = 0;
    virtual void set_component( int index, float x) = 0;
};

template <typename T>
struct poly_indexable_regular_instance : base::optimized_storage_type<T, poly_indexable_regular_interface>::type
{
private:

    BOOST_CONCEPT_ASSERT(( IndexableRegularConcept<T>));

public:

    typedef typename base::optimized_storage_type<T, poly_indexable_regular_interface>::type base_type;

    poly_indexable_regular_instance(const T& x) : base_type( x) {}

    BASE_POLY_INLINE_COPY_AND_ASSIGN( poly_indexable_regular_instance, base_type)

    bool equals( const base::poly_regular_interface& x) const
    {
        return this->type_info() == x.type_info() && this->get() == *static_cast<const T*>( x.cast());
    }

    float get_component( int index) const
    {
        assert( this->type_info() != typeid( base::empty_t()));
        assert( index >= 0);
        //assert( index * sizeof( float) < sizeof( T));

        const float *ptr = reinterpret_cast<const float *>( &( this->get()));
        return ptr[ index];
    }

    void set_component( int index, float x)
    {
        assert( this->type_info() != typeid( base::empty_t()));
        assert( index >= 0);
        assert( index * sizeof( float) < sizeof( T));

        float *ptr = reinterpret_cast<float *>( &( this->get()));
        ptr[ index] = x;
    }
};

struct indexable_regular : base::poly_base<poly_indexable_regular_interface, poly_indexable_regular_instance>
{
    typedef base::poly_base<poly_indexable_regular_interface, poly_indexable_regular_instance> base_type;

    indexable_regular() : base_type( base::empty_t()) {}

    template <typename T>
    explicit indexable_regular( const T& s) : base_type( s) {}

    BASE_POLY_INLINE_COPY_AND_ASSIGN( indexable_regular, base_type)

    bool is_empty() const { return type_info() == typeid( base::empty_t);}

    float get_component( int index) const   { return interface_ref().get_component( index);}
    void set_component( int index, float x) { interface_ref().set_component( index, x);}
};

typedef base::poly<indexable_regular> poly_indexable_regular_t;

} // ramen

#endif
