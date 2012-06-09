/*
    Relevant copyright information is provided below and may not be removed from this file.

    Derived from the RSA Data Security, Inc. MD5 Message-Digest Algorithm.

    MD5C.C - RSA Data Security, Inc., MD5 message-digest algorithm

    Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All rights
    reserved.

    License to copy and use this software is granted provided that it is
    identified as the "RSA Data Security, Inc. MD5 Message-Digest Algorithm" in
    all material mentioning or referencing this software or this function.

    License is also granted to make and use derivative works provided that such
    works are identified as "derived from the RSA Data Security, Inc. MD5
    Message-Digest Algorithm" in all material mentioning or referencing the
    derived work.

    RSA Data Security, Inc. makes no representations concerning either the
    merchantability of this software or the suitability of this software for
    any particular purpose. It is provided "as is" without express or implied
    warranty of any kind.

    These notices must be retained in any copies of any part of this
    documentation and/or software.
*/

#ifndef RAMEN_HASH_MD5_HPP
#define RAMEN_HASH_MD5_HPP

#include<ramen/config.hpp>

#include<cstddef>

#include<boost/cstdint.hpp>
#include<boost/array.hpp>

namespace ramen
{
namespace hash
{

class md5_t
{
public:
    typedef boost::array<boost::uint8_t, 16> digest_t;

    md5_t();

    void update(void* input_block, std::size_t input_length);

    digest_t final();

private:
    void reset();

    boost::uint32_t state_m[4];     /* state (ABCD) */
    boost::uint32_t count_m[2];     /* number of bits, modulo 2^64 (lsb first) */
    boost::uint8_t  buffer_m[64];   /* input buffer */
};

inline md5_t::digest_t md5( void *input_block, std::size_t input_length)
{
    md5_t m;
    m.update(input_block, input_length);
    return m.final();
}

} // hash
} // ramen

#endif
