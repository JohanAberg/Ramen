// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_APP_APPLICATION_HPP
#define RAMEN_APP_APPLICATION_HPP

#include<ramen/config.hpp>

#include<ramen/app/application_fwd.hpp>

#include<ramen/python/python.hpp>

#include<string>
#include<memory>

#include<boost/noncopyable.hpp>
#include<boost/cstdint.hpp>
#include<boost/python.hpp>

#include<tbb/task_scheduler_init.h>

#include<ramen/system/system.hpp>

#include<ramen/app/preferences_fwd.hpp>
#include<ramen/app/document_fwd.hpp>

#include<ramen/memory/manager_fwd.hpp>
#include<ramen/ocio/manager_fwd.hpp>

#include<ramen/filesystem/path.hpp>

#include<ramen_ui/user_interface_fwd.hpp>

namespace ramen
{

/*!
\ingroup app
\brief Application class.
*/
class RAMEN_API application_t : boost::noncopyable
{
public:

    application_t();
    ~application_t();

    int max_threads() const { return max_threads_;}

    // messages
    void fatal_error( const std::string& message, bool no_gui = false) const;
    void error( const std::string& message, bool no_gui = false) const;
    void inform( const std::string& message, bool no_gui = false) const;
    bool question( const std::string& what, bool default_answer = true) const;

    const system::system_t& system() { return system_;}

    const preferences_t& preferences() const;
    preferences_t& preferences();

    const memory::manager_t& memory_manager() const { return *mem_manager_;}
    memory::manager_t& memory_manager()             { return *mem_manager_;}

    // OCIO
    const ocio::manager_t& ocio_manager() const { return *ocio_manager_;}
    ocio::manager_t& ocio_manager()             { return *ocio_manager_;}

    // document handling
    const document_t& document() const;
    document_t& document();

    void create_new_document();
    void open_document( const boost::filesystem::path& p);

    // misc
    bool debug_build() const;

private:

    friend struct py_application_access;

    void create_dirs();

    // init
    void init_threads( int num);
    void init_ocio();

    const ramen_ui::user_interface_t *ui() const    { return ui_;}
    ramen_ui::user_interface_t *ui()                { return ui_;}
    void set_ui( ramen_ui::user_interface_t *ui);

    void set_preferences( boost::python::object prefs);

    // report
    void print_app_info();

    // document handling
    void delete_document();

    // tests
    int run_unit_tests( int argc, char **argv);

    // threads
    boost::uint64_t img_cache_size_;
    int max_threads_;

    tbb::task_scheduler_init task_scheduler_;
    system::system_t system_;
    std::auto_ptr<memory::manager_t> mem_manager_;
    std::auto_ptr<ocio::manager_t> ocio_manager_;
    std::auto_ptr<document_t> document_;

    // created from python
    std::auto_ptr<preferences_t> preferences_;
    ramen_ui::user_interface_t *ui_;
};

RAMEN_API application_t& app();

} // namespace

#endif
