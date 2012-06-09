// From Adobe source libraries. Original license follows.
/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

#include <ramen/empty.hpp>

#include <ostream>

namespace ramen
{

std::ostream& operator << ( std::ostream& stream, const empty_t&)
{
    stream << "empty"; // REVISIT (sparent) : Go through formatter.
    return stream;
}

} // namespace ramen
