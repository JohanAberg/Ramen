// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_TAB_GROUP_PARAM_HPP
#define	RAMEN_TAB_GROUP_PARAM_HPP

#include<ramen/params/composite_param.hpp>

#include<QPointer>

class QTabWidget;

namespace ramen
{

class tab_group_param_t : public composite_param_t
{
    Q_OBJECT

public:

	tab_group_param_t();
    explicit tab_group_param_t( const std::string& name);

protected:

    tab_group_param_t( const tab_group_param_t& other);
    void operator=( const tab_group_param_t& other);

private:

    virtual param_t *do_clone() const { return new tab_group_param_t( *this);}

    virtual QWidget *do_create_widgets() RAMEN_WARN_UNUSED_RESULT;
    virtual void do_enable_widgets( bool e);

    virtual void do_create_tracks( anim::track_t *parent);

    QPointer<QTabWidget> tab_;
};

} // namespace

#endif
