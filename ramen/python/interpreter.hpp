// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_PYTHON_INTERPRETER_HPP
#define RAMEN_PYTHON_INTERPRETER_HPP

#include<sstream>

#include<boost/noncopyable.hpp>
#include<boost/python.hpp>
#include<boost/filesystem/path.hpp>

#include<loki/Singleton.h>

namespace ramen
{
namespace python
{

class interpreter_impl : boost::noncopyable
{
public:

    boost::python::object& main_namespace() { return main_namespace_;}

    boost::python::object exec( const std::string& cmd);
    boost::python::object exec( const std::string& cmd, boost::python::object global, boost::python::object local);

	boost::python::object eval( const std::string& expr);
    boost::python::object eval( const std::string& expr, boost::python::object global, boost::python::object local);

private:

    friend class Loki::CreateUsingNew<interpreter_impl>;

    interpreter_impl();

	boost::python::object exec_file( const boost::filesystem::path& p);
	
	void setup_python_paths( boost::python::object name_space);
	void add_path_to_sys_cmd( std::stringstream& cmd, const boost::filesystem::path& p) const;

    boost::python::object main_namespace_;
};

typedef Loki::SingletonHolder<interpreter_impl> interpreter_t;

} // namespace
} // namespace

#endif
