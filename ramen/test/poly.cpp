// Ported from Adobe source libraries.

#include<ramen/test/test.hpp>

#include<ramen/poly.hpp>
#include<ramen/poly_copyable.hpp>
#include<ramen/poly_regular.hpp>
using namespace ramen;

/*********************************************************************/

// define default implementation of size operation to call member function size
template <typename T>
double size( const T& t)
{
    return static_cast<double>( t.size());
}

inline double size( int x) { return static_cast<double>( x);}

template <class T>
struct MeasurableConcept
{
    MeasurableConcept() {}  // for gcc

    // operations
    static double size( const T& t)
    {
        using ::size;
        return size( t);
    }

    // constraints
    void constraints() { d = size( t);}

    const T t;
    double d;
};

template <typename T> double height(const T& t) { return t.height();}

template <class T>
struct Measurable_2DConcept : MeasurableConcept<T>
{
    // operations
    static double height(const T& t)
    {
        using ::height;
        return height( t);
    }

    // constraints
    void constraints()
    {
        d = height( t);
        d = size( t);
    }

    const T t;
    double d;
};

struct poly_measurable_interface : poly_copyable_interface
{
    virtual ~poly_measurable_interface() {}

    virtual double size() const = 0;
};

struct poly_measurable_2d_interface : poly_measurable_interface
{
    virtual double height() const = 0;
};

// This class template implements the abstract measurable interface in terms of a type T that
// Models the MeasurableConcept

template <typename T>
struct poly_measurable_instance : optimized_storage_type<T, poly_measurable_interface>::type
{
    typedef typename optimized_storage_type<T, poly_measurable_interface>::type base_t;

    BOOST_CLASS_REQUIRE(T, , MeasurableConcept);

    poly_measurable_instance(const T& x) : base_t(x) {}

    RAMEN_POLY_INLINE_COPY_AND_ASSIGN( poly_measurable_instance, base_t)

    double size() const { return MeasurableConcept<T>::size(this->get());}
};

// Another boilerplate class, measurable serves as a template parameter to the poly<> machinery
// tying together the interface and implementation above.

struct measurable : poly_base<poly_measurable_interface, poly_measurable_instance>
{
    typedef poly_base<poly_measurable_interface, poly_measurable_instance> base_t;

    // No delegating constructors (yet), so we call base constructor manually
    template <typename T>
    explicit measurable(const T& s) : base_t(s)  {}

    RAMEN_POLY_INLINE_COPY_AND_ASSIGN( measurable, base_t)

    // No forwarding in C++, so we do it manually
    double size() const { return interface_ref().size();}
};

typedef poly<measurable> poly_measurable;

template <typename T>
struct poly_measurable_2d_instance : optimized_storage_type<T, poly_measurable_2d_interface>::type
{
    typedef typename optimized_storage_type<T, poly_measurable_2d_interface>::type base_t;

    BOOST_CLASS_REQUIRE(T, , Measurable_2DConcept);

    poly_measurable_2d_instance(const T& x) : base_t(x) {}

    RAMEN_POLY_INLINE_COPY_AND_ASSIGN( poly_measurable_2d_instance, base_t)

    double size() const
    {
        return Measurable_2DConcept<T>::size( this->get());
    }

    double height() const
    {
        return Measurable_2DConcept<T>::height( this->get());
    }
};

// Another boilerplate class, measurable_2d serves as a template parameter to the poly<> machinery
// tying together the interface and implementation above.

struct measurable_2d : poly_base<poly_measurable_2d_interface, poly_measurable_2d_instance>
{
    typedef poly_base<poly_measurable_2d_interface, poly_measurable_2d_instance> base_t;

    // No delegating constructors (yet), so we call base constructor manually
    template <typename T>
    explicit measurable_2d(const T& s) : base_t(s) {}

    RAMEN_POLY_INLINE_COPY_AND_ASSIGN( measurable_2d, base_t)

    // No forwarding in C++, so we do it manually
    double size() const { return interface_ref().size();}

    // No forwarding in C++, so we do it manually
    double height() const { return interface_ref().height();}
};

typedef poly<measurable_2d> poly_measurable2d;

double max_size_poly( const poly_measurable& x, const poly_measurable& y)
{
    return std::max( size( x), size( y));
}

/*********************************************************************/

struct concrete_measurable2d
{
    concrete_measurable2d() : size_( 0.0), height_( 0.0) {}
    concrete_measurable2d(double size, double height) : size_( size), height_( height) {}

    double size() const { return size_;}
    double height() const { return height_;}

    double size_;
    double height_;
};

bool operator==(const concrete_measurable2d& x, const concrete_measurable2d& y) { return x.size() == y.size() && x.height() == y.height();}
bool operator!=(const concrete_measurable2d& x, const concrete_measurable2d& y) { return !(x == y);}

inline double perimeter(poly_measurable2d x) { return 2*(x.size() + x.height());}

void run_time_poly_test()
{
    poly_measurable four( 4), five( 5);
    poly_measurable y( std::string("hello"));

    BOOST_CHECK_EQUAL( max_size_poly( four, five), 5);
    BOOST_CHECK_EQUAL( max_size_poly( four, y), 5);
}

static bool registered1 = RAMEN_REGISTER_TEST_CASE( run_time_poly_test);

void poly_refinement_test()
{
    concrete_measurable2d a(2,3), b(1,4);

    poly_measurable ma(a), mb(b);
    BOOST_CHECK_EQUAL( ma.size(), mb.size()+1);
    BOOST_CHECK_THROW( poly_cast<poly_measurable2d&>(ma), std::bad_cast);
    BOOST_CHECK_EQUAL( poly_cast<poly_measurable2d*>(&ma)== NULL, true);

    poly_measurable2d m2a(a), m2b(b);
    BOOST_CHECK_EQUAL( ma.size(), m2a.size());
    BOOST_CHECK_EQUAL( perimeter(m2a), perimeter(m2b));
    BOOST_CHECK_EQUAL( max_size_poly( poly_cast<poly_measurable&>(m2a),
                                      poly_cast<poly_measurable&>(m2b)), 2);

    poly_measurable x( m2a);
    BOOST_CHECK_EQUAL( x.size(), m2a.size());
}

static bool registered2 = RAMEN_REGISTER_TEST_CASE( poly_refinement_test);

void poly_assign_test()
{
    concrete_measurable2d a(2,3), b(1,4);
    poly_measurable ma(a), mb(b);
    poly_measurable2d m2a(a), m2b(b);

    m2b.assign(a);
    BOOST_CHECK_EQUAL(m2a.size(), m2b.size());
    BOOST_CHECK_EQUAL(m2a.height(), m2b.height());

    int i=1;
    ma.assign(i);
}

static bool registered3 = RAMEN_REGISTER_TEST_CASE( poly_assign_test);

void poly_move_test()
{
    concrete_measurable2d a(2,3), b(1,4);
    poly_measurable ma(a), mb(b);
    poly_measurable2d m2a(a), m2b(b);
    poly_measurable mc( boost::move(ma));
}

static bool registered4 = RAMEN_REGISTER_TEST_CASE( poly_move_test);

void poly_copyable_test()
{
    concrete_measurable2d a(2,3), b(1,4);
    {
        poly<copyable> ma(a), mb(b);
        poly<copyable> m2a(a), m2b(b);

        poly<copyable> mc( boost::move(ma));
    }
    {
        poly<regular> ma(a), mb(b);
        poly<regular> m2a(a), m2b(b);

        //poly<regular> mc(ma, move_ctor());
        BOOST_CHECK(true);
        BOOST_CHECK(ma == m2a);
        BOOST_CHECK(ma != poly<regular>());
    }
}

static bool registered5 = RAMEN_REGISTER_TEST_CASE( poly_copyable_test);
