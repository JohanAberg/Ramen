// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_HASH_GENERATOR_HPP
#define	RAMEN_HASH_GENERATOR_HPP

#include<sstream>

#include<boost/optional.hpp>

#include<base/hash/md5.hpp>

#include<ramen/assert.hpp>

namespace ramen
{
namespace hash
{

class generator_t
{
public:

    typedef base::hash::md5_t::digest_type digest_type;

    generator_t();

    void reset();

    std::stringstream& sstream() { return ss_;}

    std::string str() const;

    bool finalized() const { return digest_;}

    const digest_type& digest() const;
    std::string digest_as_string() const;

    static digest_type digest_from_string( const std::string& s);

private:

    std::stringstream ss_;
    mutable boost::optional<digest_type> digest_;
};

template<class T>
generator_t& operator<<( generator_t& hash_gen, const T& x)
{
    RAMEN_ASSERT( !hash_gen.finalized());
    hash_gen.sstream() << x;
    return hash_gen;
}

} // util
} // ramen

#endif

