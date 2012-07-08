// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/python/python.hpp>

#include<ramen/nodes/factory.hpp>

#include<boost/foreach.hpp>
#include<boost/algorithm/string/predicate.hpp>

#include<ramen/nodes/node.hpp>

#ifndef NDEBUG
    #include<iostream>
#endif

namespace ramen
{
namespace nodes
{
namespace
{

struct compare_menu_items
{
    bool operator()( const class_metadata_t& a, const class_metadata_t& b) const
    {
        return a.menu_item < b.menu_item;
    }
};

} // unnamed

factory_t& factory_t::instance()
{
    static factory_t f;
    return f;
}

factory_t::~factory_t()
{
    unregister_all();
}

bool factory_t::register_node( const class_metadata_t& m)
{
    RAMEN_ASSERT( m.major_version >= 0);
    RAMEN_ASSERT( m.minor_version >= 0);
    RAMEN_ASSERT( m.create);
    RAMEN_ASSERT( !m.menu.empty());
    RAMEN_ASSERT( !m.submenu.empty());
    RAMEN_ASSERT( !m.menu_item.empty());

    BOOST_FOREACH( const class_metadata_t& metaclass, metaclasses_)
    {
        if( metaclass.id == m.id && metaclass.major_version == m.major_version && metaclass.minor_version == m.minor_version)
        {
            std::cout << "Duplicated metaclass in node factory: " << m.id << "\n";
            RAMEN_ASSERT( 0);
        }
    }

    metaclasses_.push_back( m);

    // if this version of the node is newer that the one we have...
    std::map<std::string, class_metadata_t>::iterator it( newest_node_infos_.find( m.id));

    if( it != latest_versions_end())
    {
        if( m.major_version > it->second.major_version)
            newest_node_infos_[m.id] = m;
        else
        {
            if( m.major_version == it->second.major_version)
            {
                if( m.minor_version > it->second.minor_version)
                    newest_node_infos_[m.id] = m;
            }
        }
    }
    else
        newest_node_infos_[m.id] = m;

    return true;
}

void factory_t::sort_by_menu_item()
{
    std::sort( metaclasses_.begin(), metaclasses_.end(), compare_menu_items());
}

std::auto_ptr<node_t> factory_t::create_by_id( const std::string& id, bool ui)
{
    std::map<std::string, class_metadata_t>::iterator it( newest_node_infos_.find( id));
    std::auto_ptr<node_t> n;

    if( it != newest_node_infos_.end())
    {
        try
        {
            if( it->second.first_time_)
            {
                it->second.first_time_ = false;

                if( it->second.init)
                    it->second.init();
            }

            if( it->second.create_gui && ui)
                n.reset( it->second.create_gui());
            else
                n.reset( it->second.create());
        }
        catch( ...)
        {
        }
    }

    return n;
}

std::auto_ptr<node_t> factory_t::create_by_id_with_version( const std::string& id, const std::pair<int, int>& version)
{
    std::vector<class_metadata_t>::iterator best( metaclasses_.end());
    std::vector<class_metadata_t>::iterator it( metaclasses_.begin());
    int best_minor = -1;

    for( ; it != metaclasses_.end(); ++it)
    {
        if( it->id == id)
        {
            if( it->major_version == version.first)
            {
                if( it->minor_version > best_minor)
                {
                    best = it;
                    best_minor = it->minor_version;
                }
            }
        }
    }

    std::auto_ptr<node_t> n;

    if( best_minor < version.second)
        return n;

    if( best != metaclasses_.end())
    {
        try
        {
            if( best->first_time_)
            {
                best->first_time_ = false;

                if( best->init)
                    best->init();
            }

            n.reset( best->create());
        }
        catch( ...)
        {
        }
    }

    return n;
}

bool factory_t::is_latest_version( const std::string& id) const
{
    const_iterator it( newest_node_infos_.find( id));
    return it != latest_versions_end();
}

void factory_t::unregister_all()
{
    for( int i = 0; i < metaclasses_.size(); ++i)
    {
        if( !metaclasses_[i].first_time_ && metaclasses_[i].cleanup)
            metaclasses_[i].cleanup();
    }

    metaclasses_.clear();
    newest_node_infos_.clear();
}

} // namespace
} // namespace
