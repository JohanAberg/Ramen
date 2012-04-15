// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_UI_COMMANDS_HPP
#define RAMEN_UI_COMMANDS_HPP

#include<ramen/undo/command.hpp>
#include<ramen/nodes/node_fwd.hpp>

namespace ramen
{
namespace undo
{

class connect_command_t : public command_t
{
public:

	connect_command_t( node_t *src, node_t *dest, int port);

	virtual void undo();
	virtual void redo();

private:

	node_t *src_;
	node_t *dest_;
	node_t *prev_src_;
	int port_;
};

class disconnect_command_t : public command_t
{
public:

	disconnect_command_t( node_t *src, node_t *dest, int port);

	virtual void undo();
	virtual void redo();

private:

	node_t *src_;
	node_t *dest_;
	int port_;
};

} // namespace
} // namespace

#endif
