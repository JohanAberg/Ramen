// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/params/ocio_colorspace_param.hpp>

#include<QComboBox>
#include<QLabel>

#include<ramen/params/param_set.hpp>
#include<ramen/nodes/node.hpp>

#include<ramen/app/application.hpp>

#include<ramen/ui/inspector/inspector.hpp>

namespace ramen
{

ocio_colorspace_param_t::ocio_colorspace_param_t( const std::string& name) : static_param_t( name)
{
    set_default_value( default_colorspace());
}

ocio_colorspace_param_t::ocio_colorspace_param_t( const ocio_colorspace_param_t& other) : static_param_t( other), menu_( 0) {}

void ocio_colorspace_param_t::set_default_value( const std::string& cs) { value().assign( cs);}

void ocio_colorspace_param_t::set_value( const std::string& cs, change_reason reason)
{
    if( param_set() && can_undo())
        param_set()->add_command( this);

    value().assign( cs);
	emit_param_changed( reason);
}

QWidget *ocio_colorspace_param_t::do_create_widgets()
{
    QWidget *top = new QWidget();
    QLabel *label = new QLabel( top);
    menu_ = new QComboBox( top);
    menu_->setFocusPolicy( Qt::NoFocus);

    OCIO::ConstConfigRcPtr config = app().current_ocio_config();
    std::string current_colorspace = get_value<std::string>( *this);
    int index = 0;
    int num_color_spaces = config->getNumColorSpaces();

    for(int i = 0; i < num_color_spaces; i++)
    {
        std::string csname = config->getColorSpaceNameByIndex( i);

        if( csname == current_colorspace)
            index = i;

        menu_->addItem( csname.c_str());
    }

    QSize s = menu_->sizeHint();

    label->move( 0, 0);
    label->resize( ui::inspector_t::Instance().left_margin() - 5, s.height());
    label->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
    label->setText( name().c_str());
	label->setToolTip( id().c_str());
	
    menu_->move( ui::inspector_t::Instance().left_margin(), 0);
    menu_->resize( s.width(), s.height());
    menu_->setCurrentIndex( index);
    menu_->setEnabled( enabled());
    connect( menu_, SIGNAL( currentIndexChanged( int)), this, SLOT( item_picked( int)));

    top->setMinimumSize( ui::inspector_t::Instance().width(), s.height());
    top->setMaximumSize( ui::inspector_t::Instance().width(), s.height());
    top->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    return top;
}

void ocio_colorspace_param_t::do_update_widgets()
{
    if( menu_)
    {
        menu_->blockSignals( true);

        std::string csname = get_value<std::string>( *this);

        for( int i = 0; i < menu_->count(); ++i)
        {
            if( csname == menu_->itemText( i).toStdString())
            {
                menu_->setCurrentIndex( i);
                break;
            }
        }

        menu_->blockSignals( false);
    }
}

void ocio_colorspace_param_t::do_enable_widgets( bool e)
{
    if( menu_)
        menu_->setEnabled( e);
}

void ocio_colorspace_param_t::item_picked( int index)
{
    param_set()->begin_edit();
    set_value( menu_->itemText( index).toStdString());
    param_set()->end_edit();
}

void ocio_colorspace_param_t::do_add_to_hash( hash_generator_t& hash_gen) const
{ 
	hash_gen << get_value<std::string>( *this);
}

boost::python::object ocio_colorspace_param_t::to_python( const poly_param_value_t& v) const
{
	return boost::python::object( v.cast<std::string>());
}
	
poly_param_value_t ocio_colorspace_param_t::from_python( const boost::python::object& obj) const
{
	std::string str = boost::python::extract<std::string>( obj);
	return poly_param_value_t( str);
}

void ocio_colorspace_param_t::do_read( const serialization::yaml_node_t& node)
{
	serialization::yaml_node_t n = node.get_node( "value");
	std::string val;
	n >> val;
	
    OCIO::ConstConfigRcPtr config = app().current_ocio_config();
    int index = -1;
	
    int num_color_spaces = config->getNumColorSpaces();

    for(int i = 0; i < num_color_spaces; i++)
    {
        std::string csname = config->getColorSpaceNameByIndex( i);

        if( csname == val)
            index = i;
    }

	if( index != -1)
		value().assign( val);
	else
	{
		node.error_stream() << "Node " << parameterised()->name() << ": colorspace " << val << " not found in OCIO config.\n";
		node.error_stream() << "Replacing by default value.\n";
		value().assign( default_colorspace());
	}
}

void ocio_colorspace_param_t::do_write( serialization::yaml_oarchive_t& out) const
{
    out << YAML::Key << "value" << YAML::Value << get_value<std::string>( *this);
}

std::string ocio_colorspace_param_t::default_colorspace() const
{
    OCIO::ConstConfigRcPtr config = app().current_ocio_config();
    return config->getColorSpace( OCIO::ROLE_SCENE_LINEAR)->getName();
}

} // namespace
