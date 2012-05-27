// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_OCIO_COLORSPACE_PARAM_HPP
#define	RAMEN_OCIO_COLORSPACE_PARAM_HPP

#include<ramen/params/static_param.hpp>

#include<string>
#include<vector>

#include<adobe/string.hpp>

#include<QPointer>

#include<ramen/ui/widgets/ocio_colorspace_combo_fwd.hpp>

namespace ramen
{

class RAMEN_API ocio_colorspace_param_t : public static_param_t
{
    Q_OBJECT

public:

    explicit ocio_colorspace_param_t( const std::string& name);

    void set_default_value( const std::string& cs);
    void set_value( const std::string& cs, change_reason reason = user_edited);

protected:

    ocio_colorspace_param_t( const ocio_colorspace_param_t& other);
    void operator=( const ocio_colorspace_param_t& other);

private:

    virtual param_t *do_clone() const { return new ocio_colorspace_param_t( *this);}

    virtual void do_add_to_hash( hash::generator_t& hash_gen) const;

    virtual boost::python::object to_python( const poly_param_value_t& v) const;
    virtual poly_param_value_t from_python( const boost::python::object& obj) const;

    virtual void do_read( const serialization::yaml_node_t& node);
    virtual void do_write( serialization::yaml_oarchive_t& out) const;

    virtual QWidget *do_create_widgets() RAMEN_WARN_UNUSED_RESULT;
    virtual void do_update_widgets();
    virtual void do_enable_widgets( bool e);

    std::string default_colorspace() const;

    QPointer<ui::ocio_colorspace_combo_t> menu_;

private Q_SLOTS:

    void colorspace_picked( const std::string& cs);
};

} // namespace

#endif
