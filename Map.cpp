#include <cassert>
#include "API.h"
#include "Map.h"

Coordinate::Coordinate() :
    _x( 0 ),
    _y( 0 )
{
}

Coordinate::Coordinate( unsigned int x, unsigned int y ) :
    _x( x ),
    _y( y )
{
}

Coordinate::~Coordinate() {
}

void Coordinate::X( unsigned int x ) {
    _x = x;
}

void Coordinate::Y( unsigned int y ) {
    _y = y;
}

unsigned int Coordinate::X() const {
    return _x;
}

unsigned int Coordinate::Y() const {
    return _y;
}

bool operator==( const Coordinate& lhs, const Coordinate& rhs ) {
    return ( lhs.X() == rhs.X() ) && ( lhs.Y() == rhs.Y() );
}

bool operator<( const Coordinate& lhs, const Coordinate& rhs ) {
    return ( lhs.X() * lhs.X() + lhs.Y() * lhs.Y() ) < ( rhs.X() * rhs.X() + rhs.Y() * rhs.Y() );
}

Node::Node() :
    _north( false ),
    _east( false ),
    _south( false ),
    _west( false ),
    _visited( false )
{
}

Node::~Node() {
}

void Node::XY( unsigned int x, unsigned int y ) {
    _coordinate.X( x );
    _coordinate.Y( y );
}

const Coordinate& Node::XY() const {
    return _coordinate;
}

void Node::SetWall( DirectionEnum d ) {
    switch( d ) {
        case DIR_NORTH:
            _north = true;
            break;
        case DIR_EAST:
            _east = true;
            break;
        case DIR_SOUTH:
            _south = true;
            break;
        case DIR_WEST:
            _west = true;
            break;
        default:
            assert( false );
            break;
    }
}

bool Node::North() const {
    return _north;
}

bool Node::East() const {
    return _east;
}

bool Node::South() const {
    return _south;
}

bool Node::West() const {
    return _west;
}

void Node::Visit( const bool v ) {
    _visited = v;
}

bool Node::Visited() const {
    return _visited;
}

Map::Map() :
    _pos( nullptr )
{}

Map::~Map() {
}

void Map::Initialize() {
    for( auto row = _nodes.begin(); row != _nodes.end(); ++row ) {
        const unsigned int y( std::distance( _nodes.begin(), row ) );

        for( auto n = row->begin(); n != row->end(); ++n ) {
            const unsigned int x( std::distance( row->begin(), n ) );
            n->XY( x, y );

            if( row == _nodes.begin() ) {
                n->SetWall( DIR_SOUTH );
                API::setWall( x, y, DIR_SOUTH );
            }
            else if( row == ( _nodes.end() - 1 ) ) {
                n->SetWall( DIR_NORTH );
                API::setWall( x, y, DIR_NORTH );
            }

            if( n == row->begin() ) {
                n->SetWall( DIR_WEST );
                API::setWall( x, y, DIR_WEST );
            }
            else if( n == ( row->end() - 1 ) ) {
                n->SetWall( DIR_EAST );
                API::setWall( x, y, DIR_EAST );
            }
        }
    }

    _pos = &At( Coordinate( 0, 0 ) );
}

Node& Map::At( Coordinate c ) {
    return _nodes.at( c.Y() ).at( c.X() );
}

Node& Map::CurrentPosition() {
    return *_pos;
}

void Map::Move( DirectionEnum d ) {
    const Coordinate& c( _pos->XY() );

    switch( d )
    {
        case DIR_NORTH:
            _pos = &At( Coordinate( c.X(), c.Y() + 1 ) );
            break;
        case DIR_EAST:
            _pos = &At( Coordinate( c.X() + 1, c.Y() ) );
            break;
        case DIR_SOUTH:
            _pos = &At( Coordinate( c.X(), c.Y() - 1 ) );
            break;
        case DIR_WEST:
            _pos = &At( Coordinate( c.X() - 1, c.Y() ) );
            break;
        default:
            assert( false );
            break;
    }

    _pos->Visit( true );
}

void Map::SetWall( DirectionEnum d ) {
    const Coordinate& c( _pos->XY() );
    _pos->SetWall( d );

    switch( d ) {
        case DIR_NORTH:
            if( c.Y() < 15 ) {
                const Coordinate north( c.X(), c.Y() + 1 );
                if( !At( north ).Visited() ) {
                    API::setColor( north.X(), north.Y(), 'y' );
                    At( north ).SetWall( DIR_SOUTH );
                }
            }
            break;
        case DIR_EAST:
            if( c.X() < 15 ) {
                const Coordinate east( c.X() + 1, c.Y() );
                if( !At( east ).Visited() ) {
                    API::setColor( east.X(), east.Y(), 'y' );
                    At( east ).SetWall( DIR_WEST );
                }
            }
            break;
        case DIR_SOUTH:
            if( c.Y() > 0 ) {
                const Coordinate south( c.X(), c.Y() - 1 );
                if( !At( south ).Visited() ) {
                    API::setColor( south.X(), south.Y(), 'y' );
                    At( south ).SetWall( DIR_NORTH );
                }
            }
            break;
        case DIR_WEST:
            if( c.X() > 0 ) {
                const Coordinate west( c.X() - 1, c.Y() );
                if( !At( west ).Visited() ) {
                    API::setColor( west.X(), west.Y(), 'y' );
                    At( west ).SetWall( DIR_EAST );
                }
            }
            break;
        default:
            assert( false );
            break;
    }

    API::setWall( c.X(), c.Y(), d );
}
