// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_FILE_PARAM_HPP
#define	RAMEN_FILE_PARAM_HPP

#include<ramen/params/static_param.hpp>

namespace ramen
{
namespace params
{

class RAMEN_API file_param_t : public static_param_t
{
public:

    explicit file_param_t( const std::string& name);

    void set_default_value( const boost::filesystem::path& p);

    void set_value( const boost::filesystem::path& p, change_reason reason = user_edited);

    void set_is_input( bool b) { is_input_ = b;}

    bool empty() const;
    bool file_exists() const;

    // extensions
    void set_extension_list_string( const std::string& str) { ext_list_string_ = str;}

    std::string extension() const;
    void set_extension( const std::string& ext);

protected:

    file_param_t( const file_param_t& other);
    void operator=( const file_param_t& other);

private:

    virtual param_t *do_clone() const { return new file_param_t( *this);}

    virtual void do_add_to_hash( hash::generator_t& hash_gen) const;

    virtual boost::python::object to_python( const base::poly_regular_t& v) const;
    virtual base::poly_regular_t from_python( const boost::python::object& obj) const;

    // paths
    virtual void do_convert_relative_paths( const boost::filesystem::path& old_base, const boost::filesystem::path& new_base);
    virtual void do_make_paths_absolute();
    virtual void do_make_paths_relative();

    bool is_input_;
    std::string ext_list_string_;
};

} // namespace
} // namespace

#endif
