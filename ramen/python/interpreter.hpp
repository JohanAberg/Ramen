// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_PYTHON_INTERPRETER_HPP
#define RAMEN_PYTHON_INTERPRETER_HPP

#include<ramen/config.hpp>

#include<ramen/python/python.hpp>

#include<sstream>

#include<boost/noncopyable.hpp>
#include<boost/python.hpp>
#include<boost/filesystem/path.hpp>

#include<ramen/system/system.hpp>

namespace ramen
{
namespace python
{

class RAMEN_API interpreter_t : boost::noncopyable
{
public:

    static interpreter_t& instance();

    int run_main( int argc, char **argv);

    boost::python::object& main_namespace() { return main_namespace_;}

    boost::python::object exec( const std::string& cmd);
    boost::python::object exec( const std::string& cmd, boost::python::object global, boost::python::object local);

	boost::python::object eval( const std::string& expr);
    boost::python::object eval( const std::string& expr, boost::python::object global, boost::python::object local);

	boost::python::object exec_file( const boost::filesystem::path& p);

    // logging
    void log_debug( const std::string& message);

private:

    interpreter_t();
	
	void setup_python_paths( boost::python::object name_space);
	void add_path_to_sys_cmd( std::stringstream& cmd, const boost::filesystem::path& p) const;

    system::system_t system_;
    boost::python::object main_namespace_;
};

} // namespace
} // namespace

#endif
