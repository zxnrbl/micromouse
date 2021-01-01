#include <cassert>
#include "Map.h"

Node::Node() :
    _north( nullptr ),
    _east( nullptr ),
    _south( nullptr ),
    _west( nullptr ),
    _visited( false )
{
}

Node::~Node() {
}

void Node::Attach( const DirectionEnum d, Node* n ) {
    switch( d ) {
        case DIR_NORTH:
            _north = n;
            break;
        case DIR_EAST:
            _east = n;
            break;
        case DIR_SOUTH:
            _south = n;
            break;
        case DIR_WEST:
            _west = n;
            break;
        default:
            assert( false );
            break;
    }
}

Node* Node::North() const {
    return _north;
}

Node* Node::East() const {
    return _east;
}

Node* Node::South() const {
    return _south;
}

Node* Node::West() const {
    return _west;
}

void Node::Visit( const bool v ) {
    _visited = v;
}

bool Node::Visited() const {
    return _visited;
}

Map::Map() :
    _pos( nullptr ),
    _count( 0 )
{}

Map::~Map() {
}

void Map::Initialize() {
    _pos = &_nodes.at( _count );
    _pos->Visit( true );
}

Node& Map::CurrentPosition() {
    return *_pos;
}

Node& Map::CreateNode() {
    return _nodes.at( ++_count );
}

void Map::Move( DirectionEnum d ) {
    switch( d )
    {
        case DIR_NORTH:
            _pos = _pos->North();
            break;
        case DIR_EAST:
            _pos = _pos->East();
            break;
        case DIR_SOUTH:
            _pos = _pos->South();
            break;
        case DIR_WEST:
            _pos = _pos->West();
            break;
    }

    _pos->Visit( true );
}

bool Map::Visited() const {
    return _pos->Visited();
}
