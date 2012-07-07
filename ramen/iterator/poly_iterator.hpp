// Ported from Adobe source libraries.
// Original license follows:
/*
    Copyright 2006-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

/*************************************************************************************************/

#ifndef RAMEN_POLY_ITERATOR_HPP
#define RAMEN_POLY_ITERATOR_HPP

#include<ramen/config.hpp>

#include<boost/concept_check.hpp>
#include<boost/iterator/iterator_facade.hpp>

#include<ramen/poly.hpp>

namespace ramen
{
namespace iterator
{

template <  typename V,                 // T models Regular Type
            typename R = V&,            // R models Reference Type
            typename D = std::ptrdiff_t // D models Signed Integer
         >
struct poly_iterator_interface : public poly_copyable_interface
{
    virtual R dereference() const = 0;
    virtual void increment() = 0;
    virtual bool equals( const poly_iterator_interface &) const = 0;
};

template <  typename V,                 // T models Regular Type
            typename R = V&,            // R models Reference Type
            typename D = std::ptrdiff_t // D models Signed Integer
         >
struct poly_iterator_instance
{
    template <typename I>
    class type : public optimized_storage_type<I, poly_iterator_interface<V, R, D> >::type
    {
        BOOST_CONCEPT_ASSERT(( boost::ForwardIteratorConcept<I>));

    public:

        typedef typename optimized_storage_type<I, poly_iterator_interface<V, R, D> >::type base_type;

        type() : base_type() {}

        type( const I& x) : base_type ( x) {}

        RAMEN_POLY_INLINE_COPY_AND_ASSIGN( type, base_type)

        R dereference() const { return *this->get();}

        void increment() { ++this->get();}

        bool equals( const poly_iterator_interface<V, R, D>& x) const
        {
            return this->type_info() == x.type_info() && this->get() == *static_cast<const I*>( x.cast());
        }
    };
};

template <  typename V,                 // T models Regular Type
            typename R = V&,            // R models Reference Type
            typename D = std::ptrdiff_t // D models Signed Integer
         >
struct iter :   public poly_base<poly_iterator_interface<V, R, D>,
                                poly_iterator_instance<V, R, D>::template type>,
                public boost::iterator_facade<iter<V, R, D>, V, std::forward_iterator_tag, R, D >
{
    typedef poly_base<poly_iterator_interface<V, R, D>, poly_iterator_instance<V, R, D>::template type> base_type;

    template <typename Iter>
    explicit iter( const Iter& s) : base_type( s) {}

    RAMEN_POLY_INLINE_COPY_AND_ASSIGN( iter, base_type)

    R dereference() const { return this->interface_ref().dereference();}

    void increment() { this->interface_ref().increment();}

    bool equal( const iter& x) const { return *this == x;}

    //disambiguate since iter adaptor and poly both try to provide operator==
    friend bool operator==( const iter& x, const iter& y) { return base_type( x) == base_type( y);}
};

template <  typename V,                 // T models Regular Type
            typename R = V&,            // R models Reference Type
            typename D = std::ptrdiff_t // D models Signed Integer
         >
struct any_bidirectional_iterator_interface : public poly_iterator_interface<V, R, D>
{
    virtual void decrement() = 0;
    using poly_iterator_interface<V, R, D>::equals;
};

template <  typename V,                 // T models Regular Type
            typename R = V&,            // R models Reference Type
            typename D = std::ptrdiff_t // D models Signed Integer
         >
struct any_bidirectional_iterator_instance
{
    template <typename I>
    class type : public optimized_storage_type<I, any_bidirectional_iterator_interface<V, R, D> >::type
    {

        BOOST_CONCEPT_ASSERT(( boost::BidirectionalIteratorConcept<I>));

    public:

        typedef typename optimized_storage_type<I, any_bidirectional_iterator_interface<V, R, D> >::type base_type;

        type() : base_type() {}

        type( const I& x) : base_type(x) {}

        RAMEN_POLY_INLINE_COPY_AND_ASSIGN( type, base_type)

        R dereference() const { return *this->get();}

        void increment() { ++this->get();}

        void decrement() { --this->get();}

        bool equals( const poly_iterator_interface<V, R, D>& x) const
        {
            return this->type_info() == x.type_info() && this->get() == *static_cast<const I*>(x.cast());
        }

        bool equals( const any_bidirectional_iterator_interface<V, R, D>& x) const
        {
            return this->type_info() == x.type_info() && this->get() == *static_cast<const I*>(x.cast());
        }
    };

};

template <  typename V,                 // T models Regular Type
            typename R = V&,            // R models Reference Type
            typename D = std::ptrdiff_t // D models Signed Integer
         >
struct bidirectional_iter : public poly_base<any_bidirectional_iterator_interface<V, R, D>,
                                            any_bidirectional_iterator_instance<V, R, D>::template type >,
                            public boost::iterator_facade<bidirectional_iter<V, R, D>,
                                                            V, std::bidirectional_iterator_tag, R, D>

{
    typedef poly_base<any_bidirectional_iterator_interface<V, R, D>,
                        any_bidirectional_iterator_instance<V, R, D>::template type> base_type;

    template <typename Iter>
    explicit bidirectional_iter( const Iter& s) : base_type ( s) {}

    RAMEN_POLY_INLINE_COPY_AND_ASSIGN( bidirectional_iter, base_type)

    R dereference() const { return this->interface_ref().dereference();}

    void increment() { this->interface_ref().increment();}

    void decrement() { this->interface_ref().decrement();}

    bool equal( const bidirectional_iter& x) const { return *this == x;}

    //disambiguate since iter adaptor and poly both try to provide operator==
    friend bool operator==( const bidirectional_iter& x, const bidirectional_iter& y)
    {
        return x.interface_ref().equals(y.interface_ref());
    }
};

template <  typename V,                 // T models Regular Type
            typename R = V&,            // R models Reference Type
            typename D = std::ptrdiff_t // D models Signed Integer
         >
struct any_random_access_iterator_interface : public any_bidirectional_iterator_interface<V, R, D>
{
    virtual void advance(D) = 0;
    virtual D distance_to( const any_random_access_iterator_interface& x) const = 0;
    using any_bidirectional_iterator_interface<V, R, D>::equals;
};

template <  typename V,                 // T models Regular Type
            typename R = V&,            // R models Reference Type
            typename D = std::ptrdiff_t // D models Signed Integer
         >
struct any_random_access_iterator_instance
{
    template <typename I>               // I models Random Access Iterator
    class type : public optimized_storage_type<I, any_random_access_iterator_interface<V, R, D> >::type
    {
        BOOST_CONCEPT_ASSERT(( boost::RandomAccessIteratorConcept<I>));

    public:

        typedef typename optimized_storage_type<I, any_random_access_iterator_interface<V, R, D> >::type base_type;

        type() : base_type() {}

        type( const I& x) : base_type( x) {}

        RAMEN_POLY_INLINE_COPY_AND_ASSIGN( type, base_type)

        R dereference() const { return *this->get();}

        void increment() { ++this->get();}
        void decrement() { --this->get();}

        void advance( D d) { std::advance(this->get(), d);}

        D distance_to( const any_random_access_iterator_interface<V, R, D>& x) const
        {
            return std::distance(this->get(), *static_cast<const I*>(x.cast()));
        }

        bool equals( const poly_iterator_interface<V, R, D>& x) const
        {
            return this->type_info() == x.type_info() && this->get() == *static_cast<const I*>(x.cast());
        }

        bool equals( const any_bidirectional_iterator_interface<V, R, D>& x) const
        {
            return this->type_info() == x.type_info() && this->get() == *static_cast<const I*>(x.cast());
        }


        bool equals( const any_random_access_iterator_interface<V, R, D>& x) const
        {
            return this->type_info() == x.type_info() && this->get() == *static_cast<const I*>(x.cast());
        }

    };
};

template <  typename V,                 // T models Regular Type
            typename R = V&,            // R models Reference Type
            typename D = std::ptrdiff_t // D models Signed Integer
         >
struct random_access_iter : public poly_base<any_random_access_iterator_interface<V, R, D>,
                                            any_random_access_iterator_instance<V, R, D>::template type>,
                            public boost::iterator_facade<random_access_iter<V, R, D>,
                                                            V, std::random_access_iterator_tag, R, D>
{
    typedef poly_base<any_random_access_iterator_interface<V, R, D>,
                    any_random_access_iterator_instance<V, R, D>::template type> base_type;

    template <typename Iter>
    explicit random_access_iter( const Iter& s) : base_type( s) {}

    RAMEN_POLY_INLINE_COPY_AND_ASSIGN( random_access_iter, base_type);

    R dereference() const { return this->interface_ref().dereference();}

    void increment() { this->interface_ref().increment();}
    void decrement() { this->interface_ref().decrement();}

    void advance( D d) { this->interface_ref().advance(d);}

    D distance_to( const random_access_iter& x) const
    {
         return this->interface_ref().distance_to(x.interface_ref());
    }

    bool equal( const random_access_iter& x) const { return *this == x;}

    //disambiguate since iter adaptor and poly both try to provide operator==
    friend bool operator==( const random_access_iter& x, const random_access_iter& y)
    {
        return  x.interface_ref().equals(y.interface_ref());
    }
};

} // iterator
} // ramen

#endif
