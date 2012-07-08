// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_HPP
#define RAMEN_UI_HPP

#include<ramen/config.hpp>

#include<ramen/ui/user_interface_fwd.hpp>

#include<ramen/python/python.hpp>

#include<memory>

#include<boost/signals.hpp>
#include<boost/thread/future.hpp>

#include<QObject>
#include<QString>
#include<QFont>

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/filesystem/path.hpp>
#include<boost/filesystem/fstream.hpp>

namespace ramen
{
namespace ui
{

/**
\ingroup ui
\brief user interface class.
*/
class RAMEN_API user_interface_t : public QObject
{
    Q_OBJECT

public:

    user_interface_t();
    ~user_interface_t();

    void init();

    void show();

    int run();
    int run( const boost::filesystem::path& p);

    void quit();

    // error reporting
    void fatal_error( const std::string& msg) const;
    void error( const std::string& msg) const;
    void inform( const std::string& msg) const;
    bool question( const std::string& what, bool default_answer = true) const;

protected:

    // non-copyable
    user_interface_t( const user_interface_t& other);
    user_interface_t& operator=( const user_interface_t& other);
};

} // namespace
} // namespace

#endif
