
#include<vector>
#include<iostream>

#include<boost/timer/timer.hpp>

#include<OpenEXR/ImathBox.h>
#include<OpenEXR/ImathMath.h>

#include<base/poly.hpp>

#include<emmintrin.h>

/*******************************************/
// globals

int num_instances = 10000;
int num_runs = 2000;


struct classic_base_t
{
	virtual ~classic_base_t() {}

	virtual Imath::Box3d return_box() const = 0;

	virtual double sin( double x) const = 0;
};

struct classic_derived_t : public classic_base_t
{
	classic_derived_t() {}

	virtual Imath::Box3d return_box() const
	{
		return Imath::Box3d( Imath::V3d( 1, 2, 3), Imath::V3d( 7, 8, 9));
	}

	virtual double sin( double x) const { return Imath::Math<double>::sin( x);}

	int x;
	int y;
	double z[10];
};

void classic_polymorphism_test()
{
	std::vector<classic_base_t*> objs;

	for( int i = 0; i < num_instances; ++i)
		objs.push_back( new classic_derived_t());

	Imath::Box3d box;

	std::cout << "Classic polymorphism\n";
	std::cout << "--------------------" << std::endl;

	{
		boost::timer::auto_cpu_timer timer;

		for( int run = 0; run < num_runs; ++run)
		{
			for( int i = 0; i < objs.size(); ++i)
				box = objs[i]->return_box();

			for( int i = 0; i < objs.size(); ++i)
				double x = objs[i]->sin( x + i * 0.5);
		}
	}

	std::cout << num_instances * num_runs * 2 << " virtual funs" << std::endl;

	for( int i = 0; i < objs.size(); ++i)
		delete objs[i];
}

/*******************************************************************/

struct poly_test_interface : base::poly_copyable_interface
{
	virtual Imath::Box3d return_box() const = 0;
	virtual double sin( double x) const = 0;
};

template <typename T>
class poly_test_instance : public base::optimized_storage_type<T, poly_test_interface>::type
{
public:

	typedef typename base::optimized_storage_type<T, poly_test_interface>::type base_t;

	poly_test_instance( const T& x) : base_t(x) {}

	BASE_POLY_INLINE_COPY_AND_ASSIGN( poly_test_instance, base_t)

	Imath::Box3d return_box() const
	{
		return this->get().return_box();
	}

	double sin( double x) const
	{
		return this->get().sin( x);
	}
};

struct test : public base::poly_base<poly_test_interface, poly_test_instance>
{
public:

	typedef base::poly_base<poly_test_interface, poly_test_instance> base_t;

	template <typename T>
	explicit test( const T& s) : base_t( s) {}

	BASE_POLY_INLINE_COPY_AND_ASSIGN( test, base_t)

	Imath::Box3d return_box() const
	{
		return interface_ref().return_box();
	}

	double sin( double x) const
	{
		return interface_ref().sin( x);
	}
};

typedef base::poly<test> poly_test_t;

struct test_model
{
	Imath::Box3d return_box() const
	{
		return Imath::Box3d( Imath::V3d( 1, 2, 3), Imath::V3d( 7, 8, 9));
	}

	double sin( double x) const { return Imath::Math<double>::sin( x);}

	int x;
	int y;
	double z[10];
};

template<class Model>
void poly_test()
{
	std::vector<poly_test_t> objs;

	for( int i = 0; i < num_instances; ++i)
		objs.push_back( poly_test_t(( Model())));

	Imath::Box3d box;

	{
		boost::timer::auto_cpu_timer timer;

		for( int run = 0; run < num_runs; ++run)
		{
			for( int i = 0; i < objs.size(); ++i)
				box = objs[i].return_box();

			for( int i = 0; i < objs.size(); ++i)
				double x = objs[i].sin( x + i * 0.5);
		}
	}

	std::cout << num_instances * num_runs * 2 << " virtual funs" << std::endl;
}

/*******************************************************************/

void non_poly_test()
{
	std::vector<test_model> objs;

	for( int i = 0; i < num_instances; ++i)
		objs.push_back( test_model());

	Imath::Box3d box;

	std::cout << "Non poly tests\n";
	std::cout << "----------" << std::endl;

	{
		boost::timer::auto_cpu_timer timer;

		for( int run = 0; run < num_runs; ++run)
		{
			for( int i = 0; i < objs.size(); ++i)
				box = objs[i].return_box();

			for( int i = 0; i < objs.size(); ++i)
				double x = objs[i].sin( x + i * 0.5);
		}
	}

	std::cout << "0 virtual funcs" << std::endl;
}

/*******************************************************************/

int main( int argc, char **argv)
{
	non_poly_test();

	classic_polymorphism_test();

	std::cout << "Poly tests\n";
	std::cout << "----------" << std::endl;
	poly_test<test_model>();

	std::cout << "Poly + derived\n";
	std::cout << "--------------" << std::endl;
	poly_test<classic_derived_t>();

	return 0;
}
