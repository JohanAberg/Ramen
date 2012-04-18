// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/params/combo_group_param.hpp>

#include<algorithm>

#include<boost/foreach.hpp>

#include<adobe/algorithm/for_each.hpp>

#include<ramen/nodes/node.hpp>

#include<ramen/params/static_param_command.hpp>

#include<ramen/serialization/yaml_oarchive.hpp>

#include<QStackedWidget>
#include<QComboBox>
#include<QVBoxLayout>
#include<QLabel>
	
#include<ramen/ui/inspector/inspector.hpp>

namespace ramen
{

combo_group_param_t::combo_group_param_t( const std::string& name) : composite_param_t( name)
{
    set_default_value( 0);
}

combo_group_param_t::combo_group_param_t( const combo_group_param_t& other) : composite_param_t( other)
{
	#ifndef RAMEN_NO_GUI	
	    menu_ = 0;
	    stack_ = 0;
	#endif
}

void combo_group_param_t::set_default_value( int x)
{ 
	value().assign( x);
}

void combo_group_param_t::set_value( int x, change_reason reason)
{
    if( can_undo())
		param_set()->add_command( this);

    value().assign( x);
	emit_param_changed( reason);
}

std::auto_ptr<undo::command_t> combo_group_param_t::create_command()
{
    return std::auto_ptr<undo::command_t>( new static_param_command_t( *param_set(), id()));
}

void combo_group_param_t::do_add_to_hash( hash_generator_t& hash_gen) const
{
	int val = get_value<int>( *this);
	hash_gen << params()[val].name();
	params()[val].add_to_hash( hash_gen);
}

boost::python::object combo_group_param_t::to_python( const poly_param_value_t& v) const
{
	return boost::python::object( v.cast<int>());
}
	
poly_param_value_t combo_group_param_t::from_python( const boost::python::object& obj) const
{
	int i = boost::python::extract<int>( obj);
	return poly_param_value_t( i);
}

// serialization
void combo_group_param_t::do_read( const serialization::yaml_node_t& node)
{
	serialization::yaml_node_t n = node.get_node( "value");
	std::string val;
	n >> val;
	
	for( int i = 0; i < params().size(); ++i)
	{
		if( params()[i].name() == val)
		{
			value().assign( i);
			return;
		}
	}

	node.error_stream() << "Unknown string in combo group param\n";
}

void combo_group_param_t::do_write( serialization::yaml_oarchive_t& out) const
{
	int v = get_value<int>( *this);
	out << YAML::Key << "value" << YAML::Value;	
	out << params()[v].name();
	out.check_errors();
}

void combo_group_param_t::do_update_widgets()
{
	#ifndef RAMEN_NO_GUI
		if( menu_)
		{
			menu_->blockSignals( true);
			menu_->setCurrentIndex( get_value<int>( *this));
			stack_->setCurrentIndex( get_value<int>( *this));
			menu_->blockSignals( false);
			adobe::for_each( params(), boost::bind( &param_t::update_widgets, _1));
		}
	#endif
}

void combo_group_param_t::do_enable_widgets( bool e)
{
	#ifndef RAMEN_NO_GUI
	    if( menu_)
	    {
	        menu_->setEnabled( e);
	        stack_->setEnabled( e);
	    }
	#endif
}

#ifndef RAMEN_NO_GUI
	QWidget *combo_group_param_t::do_create_widgets()
	{
		QWidget *top = new QWidget();
	
		QVBoxLayout *layout = new QVBoxLayout();
		layout->setContentsMargins( 0, 0, 0, 0);
		layout->setSpacing( 5);
		layout->setSizeConstraint( QLayout::SetFixedSize);
	
		QWidget *selector = new QWidget();
		QLabel *label = new QLabel( selector);
	
		menu_ = new QComboBox( selector);
		menu_->setFocusPolicy( Qt::NoFocus);
	
		stack_ = new QStackedWidget();
	
		BOOST_FOREACH( param_t& p, params())
		{
			QWidget *w = p.create_widgets();
	
			if( w)
			{
				stack_->addWidget( w);
				menu_->addItem( p.name().c_str());
			}
		}
	
		QSize s = menu_->sizeHint();
	
		label->move( 0, 0);
		label->resize( ui::user_interface_t::Instance().inspector().left_margin() - 5, s.height());
		label->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
		label->setText( name().c_str());
		label->setToolTip( id().c_str());
		
		menu_->move( ui::user_interface_t::Instance().inspector().left_margin(), 0);
		menu_->resize( s.width(), s.height());
		menu_->setEnabled( enabled());
	
		menu_->setCurrentIndex( get_value<int>( *this));
		stack_->setCurrentIndex( get_value<int>( *this));
		stack_->setEnabled( enabled());
		connect( menu_, SIGNAL( currentIndexChanged( int)), this, SLOT( item_picked( int)));
	
		selector->setMinimumSize( ui::user_interface_t::Instance().inspector().width(), s.height());
		selector->setMaximumSize( ui::user_interface_t::Instance().inspector().width(), s.height());
		selector->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
	
		layout->addWidget( selector);
		layout->addWidget( stack_);
	
		top->setLayout( layout);
		return top;
	}

	void combo_group_param_t::item_picked( int index)
	{
		param_set()->begin_edit();
		set_value( index);
	
		if( stack_)
			stack_->setCurrentIndex( index);
	
		param_set()->end_edit();
	}
#endif

} // namespace
