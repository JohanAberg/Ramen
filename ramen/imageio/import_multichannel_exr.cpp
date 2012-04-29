// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/python/python.hpp>

#include<ramen/imageio/import_multichannel_exr.hpp>

#include<vector>
#include<set>
#include<map>

#include<OpenEXR/ImfInputFile.h>
#include<OpenEXR/ImfTestFile.h>
#include<OpenEXR/ImfChannelList.h>

#include<ramen/assert.hpp>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>

#include<ramen/undo/stack.hpp>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/main_window.hpp>
#include<ramen/ui/add_node_command.hpp>

#include<ramen/ui/compview/composition_view.hpp>

#include<ramen/nodes/image/input/input_node.hpp>

namespace ramen
{
namespace imageio
{

void import_layer( node_t *node, std::vector<std::string>& channels, std::set<std::string>& channel_set)
{
    image::input_node_t *n = dynamic_cast<image::input_node_t*>( node);
    RAMEN_ASSERT( n);

    switch( channels.size())
    {
    case 4:
        n->set_channels( channels[3], channels[2], channels[1], channels[0]);
    break;

    case 3:
        n->set_channels( channels[2], channels[1], channels[0], "");
    break;

    case 2:
        n->set_channels( channels[1], channels[0], "", "");
    break;

    case 1:
        n->set_channels( channels[0], "", "", "");
    break;
    };

    for( int i = 0; i < channels.size(); ++i)
        channel_set.erase( channels[i]);

    channels.clear();
}

void import_layer( node_t *n, Imf::ChannelList::ConstIterator first, Imf::ChannelList::ConstIterator last, std::set<std::string>& channel_set)
{
    std::vector<std::string> channels;

    for( Imf::ChannelList::ConstIterator it( first); it != last; ++it)
    {
        channels.push_back( it.name());

        if( channels.size() == 4)
            import_layer( n, channels, channel_set);
    }

    if( !channels.empty())
        import_layer( n, channels, channel_set);
}

void import_multichannel_exr( const boost::filesystem::path& p, bool relative, bool sequence)
{
    bool tiled;

    if(!Imf::isOpenExrFile( filesystem::file_cstring( p), tiled))
    {
        app().error( "Can't open EXR file");
        return;
    }

    Imf::InputFile ifile( filesystem::file_cstring( p));
    const Imf::ChannelList& ch_list( ifile.header().channels());

    std::set<std::string> channel_set;
    for( Imf::ChannelList::ConstIterator it( ch_list.begin()); it != ch_list.end(); ++it)
        channel_set.insert( std::string( it.name()));

    if( channel_set.empty())
    {
        app().error( "EXR file has no channels");
        return;
    }

    document_t::Instance().composition().deselect_all();
    std::auto_ptr<undo::add_nodes_command_t> command( new undo::add_nodes_command_t());

    image::input_node_t *node = new image::input_node_t( p, sequence, document_t::Instance().composition().composition_dir());
    node->set_composition( &document_t::Instance().composition());
    node->create_params();
    node->select( true);
    ui::user_interface_t::Instance().main_window()->composition_view().place_node( node);

    const int node_spacing = 120;

    Imath::V2f loc( node->location());
    loc.x += node_spacing;

    std::set<std::string> layer_set;
    ch_list.layers( layer_set);

    for( std::set<std::string>::iterator sit( layer_set.begin()); sit != layer_set.end(); ++sit)
    {
        Imf::ChannelList::ConstIterator it, last;
        ch_list.channelsInLayer( *sit, it, last);

        std::auto_ptr<node_t> node_copy( new_clone( *node));
        node->set_location( loc);
        loc.x += node_spacing;
        import_layer( node_copy.get(), it, last, channel_set);
        command->add_node( node_copy);
    }

    std::vector<std::string> channels;

    // handle color layer
    if( channel_set.count( "R") || channel_set.count( "B") || channel_set.count( "B"))
    {
        channels.push_back( "A");
        channels.push_back( "B");
        channels.push_back( "G");
        channels.push_back( "R");

        std::auto_ptr<node_t> node_copy( new_clone( *node));
        node->set_location( loc);
        loc.x += node_spacing;
        import_layer( node_copy.get(), channels, channel_set);
        command->add_node( node_copy);
    }

    if( channel_set.count( "Y") || channel_set.count( "RY") || channel_set.count( "BY"))
    {
        channels.push_back( "A");
        channels.push_back( "BY");
        channels.push_back( "RY");
        channels.push_back( "Y");

        std::auto_ptr<node_t> node_copy( new_clone( *node));
        node->set_location( loc);
        loc.x += node_spacing;
        import_layer( node_copy.get(), channels, channel_set);
        command->add_node( node_copy);
    }

    if( !channel_set.empty())
    {
        for( std::set<std::string>::iterator cit( channel_set.begin()); cit != channel_set.end(); ++cit)
        {
            channels.push_back( *cit);

            if( channels.size() == 4)
            {
                std::auto_ptr<node_t> node_copy( new_clone( *node));
                node->set_location( loc);
                loc.x += node_spacing;
                import_layer( node_copy.get(), channels, channel_set);
                command->add_node( node_copy);
            }
        }

        if( !channels.empty())
        {
            std::auto_ptr<node_t> node_copy( new_clone( *node));
            node->set_location( loc);
            loc.x += node_spacing;
            import_layer( node_copy.get(), channels, channel_set);
            command->add_node( node_copy);
        }
    }

    command->redo();
    document_t::Instance().undo_stack().push_back( command);
    document_t::Instance().composition().selection_changed();
    ui::user_interface_t::Instance().update();
    delete node;
}

} // namespace
} // namespace
