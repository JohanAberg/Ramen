// From Adobe source libraries. Original license follows.
/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

#ifndef RAMEN_POLY_HPP
#define RAMEN_POLY_HPP

#include<boost/type_traits/is_base_of.hpp>
#include<boost/type_traits/remove_reference.hpp>
#include<boost/type_traits/remove_pointer.hpp>
#include<boost/utility/enable_if.hpp>
#include<boost/mpl/or.hpp>
#include<boost/mpl/if.hpp>
#include<boost/mpl/bool.hpp>
#include<boost/type_traits/has_nothrow_constructor.hpp>
#include<boost/move/move.hpp>
#include<boost/swap.hpp>

#include<ramen/typeinfo.hpp>

namespace ramen
{

template<typename T, typename U>
struct is_base_derived_or_same : boost::mpl::or_<boost::is_base_of<T, U>, boost::is_base_of<U, T>, boost::is_same<T, U> > {};

struct poly_copyable_interface
{
    virtual poly_copyable_interface *clone(void*) const = 0;
    virtual poly_copyable_interface *move_clone(void*) = 0;
    virtual void *cast() = 0;
    virtual const void *cast() const = 0;
    virtual const std::type_info& type_info() const = 0;

    virtual void assign(const poly_copyable_interface& x) = 0;
    virtual void exchange(poly_copyable_interface& x) = 0;

    virtual ~poly_copyable_interface() {}
};

namespace implementation
{

template <typename ConcreteType, typename Interface>
class poly_state_remote : public Interface
{
private:

    BOOST_MOVABLE_BUT_NOT_COPYABLE( poly_state_remote)

    // unimplemented
    poly_state_remote& operator=( BOOST_RV_REF( poly_state_remote) x);

public:

    typedef ConcreteType value_type;
    typedef Interface    interface_type;

    poly_state_remote( BOOST_RV_REF( poly_state_remote) x) : value_ptr_m( x.value_ptr_m)
    {
        x.value_ptr_m = NULL;
    }

    explicit poly_state_remote( value_type x) : value_ptr_m( ::new value_type( boost::move( x))) {}

    ~poly_state_remote() { delete value_ptr_m; }

    const value_type& get() const	{ return *value_ptr_m; }
    value_type& get()				{ return *value_ptr_m; }

    // Precondition : this->type_info() == x.type_info()
    void assign( const poly_copyable_interface& x) { *value_ptr_m = *static_cast<const poly_state_remote&>(x).value_ptr_m; }

    const std::type_info& type_info() const	{ return typeid( value_type);}

    const void* cast() const	{ return value_ptr_m; }
    void* cast()				{ return value_ptr_m; }

    // Precondition : this->type_info() == x.type_info()
    void exchange( poly_copyable_interface& x) { return boost::swap( value_ptr_m, static_cast<poly_state_remote&>( x).value_ptr_m);}

    // Precondition : this->type_info() == x.type_info()
    friend bool operator==( const poly_state_remote& x, const poly_state_remote& y) { return *x.value_ptr_m == *y.value_ptr_m; }

    value_type* value_ptr_m;
};

template <typename ConcreteType, typename Interface>
class poly_state_local : public Interface
{
private:

    BOOST_MOVABLE_BUT_NOT_COPYABLE( poly_state_local)

    // unimplemented
    poly_state_local& operator=( BOOST_RV_REF( poly_state_local) x);

public:

    typedef ConcreteType value_type;
    typedef Interface    interface_type;

    explicit poly_state_local( value_type x) : value_m( boost::move(x)) {}

    poly_state_local( BOOST_RV_REF( poly_state_local) x) : value_m( boost::move( x.value_m)) {}

    const value_type& get() const	{ return value_m; }
    value_type& get()				{ return value_m; }

    // Precondition : this->type_info() == x.type_info()
    void assign(const poly_copyable_interface& x) { value_m = static_cast<const poly_state_local&>(x).value_m;}

    const std::type_info& type_info() const { return typeid(value_type); }

    const void* cast() const { return &value_m; }
    void* cast() { return &value_m; }

    // Precondition : this->type_info() == x.type_info()
    void exchange(poly_copyable_interface& x) { return boost::swap(value_m, static_cast<poly_state_local&>(x).value_m);}

    // Precondition : this->type_info() == x.type_info()
    friend bool operator==(const poly_state_local& x, const poly_state_local& y) { return x.value_m == y.value_m;}

    value_type value_m;
};

typedef double storage_t[2];

template<typename T, int N=sizeof(storage_t)>
struct is_small
{
    enum { value = sizeof(T) <= N && (boost::has_nothrow_constructor<typename T::value_type>::value ||
           boost::is_same<std::string, typename T::value_type>::value)};

};

template <typename F>
class poly_instance : public F
{
private:

    BOOST_MOVABLE_BUT_NOT_COPYABLE( poly_instance)

    // unimplemented
    poly_instance& operator=( BOOST_RV_REF( poly_instance) x);

public:

    typedef typename F::value_type value_type;
    typedef typename F::interface_type interface_type;

    poly_instance( const value_type& x): F(x) {}

    poly_instance( BOOST_RV_REF( poly_instance) x) : F( boost::move( static_cast<F&>( x))) {}

    poly_copyable_interface* clone( void* storage) const { return ::new (storage) poly_instance( this->get());}
    poly_copyable_interface* move_clone( void* storage)  { return ::new (storage) poly_instance( boost::move( *this));}
};

template <typename T>
class has_equals
{
    typedef bool (T::*E)(const T&) const;
    typedef char (&no_type)[1];
    typedef char (&yes_type)[2];
    template <E e> struct sfinae { typedef yes_type type; };
    template <class U>
    static typename sfinae<&U::equals>::type test(int);
    template <class U>
    static no_type test(...);

public:

    enum {value = sizeof(test<T>(1)) == sizeof(yes_type)};
};

} // implementation

template <typename ConcreteType, typename Interface>
struct optimized_storage_type : public boost::mpl::if_<implementation::is_small<implementation::poly_state_local<ConcreteType, Interface> >,
                                                    implementation::poly_state_local<ConcreteType, Interface>,
                                                    implementation::poly_state_remote<ConcreteType, Interface> >
{
};

template <typename I, template <typename> class Instance>
class poly_base
{
private:

    BOOST_COPYABLE_AND_MOVABLE( poly_base)

public:

    template <typename T, template <typename> class U>
    friend class poly_base;

    typedef I interface_type;

    // Construct from value type

    template <typename T>
    explicit poly_base( T x, typename boost::disable_if<boost::is_base_of<poly_base, T> >::type* = 0)
    {
        ::new (storage()) implementation::poly_instance<Instance<T> >( boost::move(x));
    }

    // Construct from related interface (might throw on downcast)
    template <typename J, template <typename> class K>
    explicit poly_base( const poly_base<J, K>& x, typename boost::enable_if<is_base_derived_or_same<I, J> >::type* dummy = 0)
    {
        if(boost::is_base_of<J, I>::value)
            dynamic_cast<const I&>(static_cast<const poly_copyable_interface&>(x.interface_ref()));

        x.interface_ref().clone(storage());
    }

    poly_base( const poly_base& x) { x.interface_ref().clone(storage());}

    poly_base( BOOST_RV_REF( poly_base) x)
    {
        x.interface_ref().move_clone(storage());
    }

    ~poly_base() { interface_ref().~interface_type(); }

    poly_base& operator=( BOOST_COPY_ASSIGN_REF(poly_base) x)
    {
        interface_ref().~interface_type();
        x.interface_ref().clone( storage());
        return *this;
    }

    poly_base& operator=( BOOST_RV_REF( poly_base) x)
    {
        interface_ref().~interface_type();
        x.interface_ref().move_clone( storage());
        return *this;
    }

    friend inline void swap( poly_base& x, poly_base& y)
    {
        interface_type& a(x.interface_ref());
        interface_type& b(y.interface_ref());

        if (a.type_info() == b.type_info()) { a.exchange(b); return; }

        // x->tmp
        poly_base tmp( boost::move( x));
        a.~interface_type();

        // y->x
        b.move_clone( x.storage());
        b.~interface_type();

        // tmp->y
        tmp.interface_ref().move_clone( y.storage());
    }

    template <typename J, template <typename> class K>
    static bool is_dynamic_convertible_from(const poly_base<J, K>& x)
    {
        return dynamic_cast<const I*>(static_cast<const poly_copyable_interface*>(&x.interface_ref()));
    }

    template <typename J>
    bool is_dynamic_convertible_to() const
    {
        return dynamic_cast<const J*>(static_cast<const poly_copyable_interface*>(&interface_ref())) != NULL;
    }

    const std::type_info& type_info() const { return interface_ref().type_info(); }

    template <typename T>
    const T& cast() const
    {
        if (type_info() != typeid(T))
            throw bad_cast();

        return *static_cast<const T*>( interface_ref().cast());
    }

    template <typename T>
    T& cast()
    {
        if (type_info() != typeid(T))
            throw bad_cast();

        return *static_cast<T*>( interface_ref().cast());
    }

    template <typename T>
    bool cast( T& x) const
    {
        if (type_info() != typeid(T))
            return false;

        x = cast<T>();
        return true;
    }

    template <typename T>
    poly_base& assign( const T& x)
    {
        if (type_info() == typeid(T))
            cast<T>() = x;
        else
        {
            poly_base tmp(x);
            swap(*this, tmp);
        }

        return *this;
    }

    // Assign from related (may throw if downcastisng)
    template <typename J, template <typename> class K>
    typename boost::enable_if<is_base_derived_or_same<I, J> >::type
    assign( const poly_base<J, K>& x)
    {
        if(boost::is_base_of<J, I>::value)
            dynamic_cast<I&>(static_cast<J&>(*x.interface_ptr())); //make sure type safe

        interface_ref().~interface_type();
        x.interface_ref().clone( storage());
    }

    const interface_type* operator->() const { return &interface_ref();}

    interface_type* operator->() { return &interface_ref();}

    interface_type& interface_ref() { return *static_cast<interface_type*>(storage()); }

    const interface_type& interface_ref() const { return *static_cast<const interface_type *>(storage()); }

    void* storage() { return &data_m; }
    const void* storage() const { return &data_m; }

    implementation::storage_t data_m;
};

template <class J, template <typename> class K>
inline typename boost::enable_if<implementation::has_equals<J>, bool>::type
operator==(const poly_base<J, K>& x, const poly_base<J, K>& y) { return x.interface_ref().equals(y.interface_ref());}

template <class F>
class poly : public F
{
private:

    BOOST_COPYABLE_AND_MOVABLE( poly)

public:

    poly() : F() {}

    /// T must be a regular type modeling the concept represented by F
    template <typename T>
    explicit poly( const T& x) : F( x) {}

    poly( const poly& x) : F( x) {}

    poly( BOOST_RV_REF( poly) x) : F( boost::move( static_cast<F&>( x))) {}

    poly& operator=( BOOST_COPY_ASSIGN_REF( poly) x)
    {
        F::operator=(static_cast<const F&>( x));
        return *this;
    }

    poly& operator=( BOOST_RV_REF( poly) x)
    {
        F::operator=( boost::move( static_cast<F&>( x)));
        return *this;
    }
};

template <typename T, typename U>
T poly_cast(poly<U>& x)
{
    typedef typename boost::remove_reference<T>::type target_type;
    typedef typename target_type::interface_type target_interface_type;

    if( !x.template is_dynamic_convertible_to<target_interface_type>())
        throw bad_cast();

    return reinterpret_cast<T>( x);
}

template <typename T, typename U>
T poly_cast(const poly<U>& x)
{
    typedef typename boost::remove_reference<T>::type target_type;
    typedef typename target_type::interface_type target_interface_type;

    if( !x.template is_dynamic_convertible_to<target_interface_type>())
        throw bad_cast();

    return reinterpret_cast<T>( x);
}

template <typename T, typename U>
T poly_cast( poly<U> *x)
{
    typedef typename boost::remove_pointer<T>::type target_type;
    typedef typename target_type::interface_type target_interface_type;

    return x->template is_dynamic_convertible_to<target_interface_type>() ? reinterpret_cast<T>( x) : NULL;
}

template <typename T, typename U>
T poly_cast( const poly<U> *x)
{
    typedef typename boost::remove_pointer<T>::type target_type;
    typedef typename target_type::interface_type target_interface_type;

    return x->template is_dynamic_convertible_to<target_interface_type>() ? reinterpret_cast<T>(x) : NULL;
}

template <class T>
inline bool operator!=( const poly<T>& x, const poly<T>& y)
{
    return !(x == y);
}

} // namespace

namespace boost
{

template<class F>
struct has_nothrow_move<ramen::poly<F> >
{
   static const bool value = true;
};

} // namespace boost


#define RAMEN_POLY_DECLARE_COPY_AND_ASSIGN( TYPENAME, BASETYPENAME) \
private:\
    BOOST_COPYABLE_AND_MOVABLE( TYPENAME)\
public:\
    TYPENAME( const TYPENAME& x);\
    TYPENAME( BOOST_RV_REF( TYPENAME) x);{}\
    TYPENAME& operator=( BOOST_COPY_ASSIGN_REF( TYPENAME) x);\
    TYPENAME& operator=( BOOST_RV_REF( TYPENAME) x);\

#define RAMEN_POLY_IMPLEMENT_COPY_AND_ASSIGN( TYPENAME, BASETYPENAME) \
    TYPENAME::TYPENAME( const TYPENAME& x) : BASETYPENAME( x) {}\
    TYPENAME::TYPENAME( BOOST_RV_REF( TYPENAME) x) : BASETYPENAME( boost::move( static_cast<BASETYPENAME&>( x))) {}\
    TYPENAME& TYPENAME::operator=( BOOST_COPY_ASSIGN_REF( TYPENAME) x)\
    {\
        BASETYPENAME::operator=(static_cast<const BASETYPENAME&>( x));\
        return *this;\
    }\
    TYPENAME& TYPENAME::operator=( BOOST_RV_REF( TYPENAME) x)\
    {\
        BASETYPENAME::operator=( boost::move( static_cast<BASETYPENAME&>( x)));\
        return *this;\
    }

#define RAMEN_POLY_INLINE_COPY_AND_ASSIGN( TYPENAME, BASETYPENAME) \
private:\
    BOOST_COPYABLE_AND_MOVABLE( TYPENAME)\
public:\
    TYPENAME( const TYPENAME& x) : BASETYPENAME( x) {}\
    TYPENAME( BOOST_RV_REF( TYPENAME) x) : BASETYPENAME( boost::move( static_cast<BASETYPENAME&>( x))) {}\
    TYPENAME& operator=( BOOST_COPY_ASSIGN_REF( TYPENAME) x)\
    {\
        BASETYPENAME::operator=(static_cast<const BASETYPENAME&>( x));\
        return *this;\
    }\
    TYPENAME& operator=( BOOST_RV_REF( TYPENAME) x)\
    {\
        BASETYPENAME::operator=( boost::move( static_cast<BASETYPENAME&>( x)));\
        return *this;\
    }

#endif
