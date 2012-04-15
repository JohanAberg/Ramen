// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_BUTTON_PARAM_HPP
#define	RAMEN_BUTTON_PARAM_HPP

#include<ramen/params/param.hpp>

#ifndef RAMEN_NO_GUI
	#include<QPointer>

	class QPushButton;
#endif

namespace ramen
{

class RAMEN_API button_param_t : public param_t
{
    Q_OBJECT
    
public:

    explicit button_param_t( const std::string& name);

protected:

    button_param_t( const button_param_t& other);
    void operator=( const button_param_t& other);

private:

    virtual param_t *do_clone() const { return new button_param_t( *this);}
	
    virtual void do_enable_widgets( bool e);

	#ifndef RAMEN_NO_GUI

	    virtual QWidget *do_create_widgets() RAMEN_WARN_UNUSED_RESULT;
	
	    QPointer<QPushButton> button_;

	private Q_SLOTS:

	    void button_pressed();
	#endif
};

} // namespace

#endif

