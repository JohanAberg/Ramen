// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/python/python.hpp>

#include<ramen/ui/add_node_command.hpp>

#include<ramen/app/document.hpp>

#include<ramen/nodes/graph_algorithm.hpp>

namespace ramen
{
namespace undo
{

add_node_command_t::add_node_command_t( node_ptr_t n, node_t *src) : command_t( "Add Node"), node_( n), src_( src) {}

add_node_command_t::~add_node_command_t() {}

void add_node_command_t::undo()
{
    if( src_)
        document_t::Instance().composition().disconnect( src_, node_.get(), 0);
	
    breadth_first_outputs_search( *node_, boost::bind( &node_t::notify, _1));
    document_t::Instance().composition().release_node( node_.get());
    command_t::undo();
}

void add_node_command_t::redo()
{
    document_t::Instance().composition().add_node( node_);

    if( src_)
	{
        document_t::Instance().composition().connect( src_, node_.get(), 0);
		breadth_first_outputs_search( *node_, boost::bind( &node_t::notify, _1));
	}

    command_t::redo();
}

add_nodes_command_t::add_nodes_command_t() : command_t( "Add Nodes") {}

void add_nodes_command_t::add_node( node_ptr_t n) { node_storage.push_back( n);}

void add_nodes_command_t::undo()
{
    for( std::vector<node_t*>::const_iterator it( nodes.begin()); it != nodes.end(); ++it)
        document_t::Instance().composition().release_node( *it);

    command_t::undo();
}

void add_nodes_command_t::redo()
{
    for( std::vector<node_ptr_t>::iterator it( node_storage.begin()); it != node_storage.end(); ++it)
        document_t::Instance().composition().add_node( *it);

    command_t::redo();
}

} // namespace
} // namespace
