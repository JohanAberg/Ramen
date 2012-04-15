// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_DEPENDENCY_NODE_HPP
#define RAMEN_DEPENDENCY_NODE_HPP

#include<ramen/config.hpp>

#include<utility>
#include<vector>

#include<boost/noncopyable.hpp>
#include<boost/signals2/signal.hpp>

#include<ramen/dependency/dgraph_fwd.hpp>

namespace ramen
{
namespace dependency
{

/*!
\ingroup depgraph
\brief A node in Ramen's dependency graph.
*/

class RAMEN_API dnode_t : boost::noncopyable
{
public:

	dnode_t();
	virtual ~dnode_t();

	/// Returns true if this node is dirty.
	bool dirty() const;

protected:

	dnode_t( const dnode_t& other);
	void operator=( const dnode_t& other);

	/// Sets this node dirty flag.
	void set_dirty( bool d);

private:

	/// Notify observers that this node changed.
	virtual void notify();

	friend class dgraph_t;

	bool dirty_;
};

/**************************************************/

/*!
\ingroup depgraph
\brief A output node in Ramen's dependency graph.
*/
class RAMEN_API output_dnode_t : public dnode_t
{
public:

	output_dnode_t();

	/// Signal emitted when this node needs updating.
    boost::signals2::signal<void()> changed;

protected:

	output_dnode_t( const output_dnode_t& other);

private:

	/// Notify observers that this node changed. Emits the changed signal.
	virtual void notify();
};

} // namespace
} // namespace

#endif
