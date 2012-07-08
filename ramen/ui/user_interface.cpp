// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/python/python.hpp>

#include<ramen/ui/user_interface.hpp>

#include<QMessageBox>

namespace ramen
{
namespace ui
{

user_interface_t::user_interface_t() : QObject() {}

user_interface_t::~user_interface_t() {}

void user_interface_t::init() {}

void user_interface_t::show() {}

int user_interface_t::run() {}

int user_interface_t::run( const boost::filesystem::path& p) {}

void user_interface_t::quit() {}

void user_interface_t::fatal_error( const std::string& msg) const
{
    QMessageBox::critical( 0, "Fatal Error", msg.c_str());
}

void user_interface_t::error( const std::string& msg) const
{
    QMessageBox::warning( 0, "Error", msg.c_str());
}

void user_interface_t::inform( const std::string& msg) const
{
    QMessageBox::information( 0, "Info", msg.c_str());
}

bool user_interface_t::question( const std::string& what, bool default_answer) const
{
    QMessageBox::StandardButton result;

    if( default_answer)
    {
        result = QMessageBox::question( 0, "Question", what.c_str(),
                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return result == QMessageBox::Yes;
    }
    else
    {
        result = QMessageBox::question( 0, "Question", what.c_str(), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        return result == QMessageBox::Yes;
    }
}

} // namespace
} // namespace
