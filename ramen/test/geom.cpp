// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/python/python.hpp>

#include<ramen/test/test.hpp>

#include<list>
#include<vector>

#include<OpenEXR/ImathBox.h>

#include<ramen/poly.hpp>
#include<ramen/iterator/poly_iterator.hpp>
using namespace ramen;

struct listS {};
struct vecS {};

class primitive_t {};
class face_primitive_t {};
class curve_primitive_t {};
class bez_patch_primitive_t {};

struct mesh_shape_t
{
    typedef iterator::iter<Imath::V3f> point_iterator;

    point_iterator points_begin()
    {
        return point_iterator( points_.begin());
    }

    point_iterator points_end()
    {
        return point_iterator( points_.end());

    }

    /*
    typedef iterator::iter<primitive_t> primitive_iterator;

    primitive_iterator primitives_begin() const
    {
        return primitive_iterator( primitives_.begin());
    }

    primitive_iterator primitives_end() const
    {
        return primitive_iterator( primitives_.end());
    }
    */

private:

    std::vector<Imath::V3f> points_;
    //std::vector<primitive_t> primitives_;
};


void test_geom()
{
}

static bool registered = RAMEN_REGISTER_TEST_CASE( test_geom);
