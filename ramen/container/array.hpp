
#ifndef RAMEN_CONTAINER_ARRAY_HPP
#define RAMEN_CONTAINER_ARRAY_HPP

#include<boost/container/vector.hpp>

#include<ramen/poly_regular.hpp>

namespace ramen
{
namespace container
{

typedef boost::container::vector<poly_regular_t> array_t;

} // container
} // ramen

#endif
