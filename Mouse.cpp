#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <string>

#include "API.h"
#include "Map.h"
#include "Mouse.h"
#include "State.h"

Mouse::Mouse() :
    _heading( DIR_NORTH ),
    _map( nullptr ),
    _state( &Unknown::Instance() )
{}

Mouse::~Mouse() {
}

void Mouse::Initialize() {
    _heading = DIR_NORTH;
    _map = new Map();
    _map->Initialize();
    ResetFlood();
    ChangeState( Idle::Instance() );
}

void Mouse::Update() {
    _state->PreUpdate( *this );
    _state->Update( *this );
    _state->PostUpdate( *this );
}

void Mouse::ChangeState( IState& state ) {
    _state->Exit( *this );
    _state = &state;
    _state->Enter( *this );
}

void Mouse::AddGoal( Coordinate c ) {
    _goals.push_back( c );
    _flood.at( c.Y() ).at( c.X() ) = 0;
    API::setText( c.X(), c.Y(), std::to_string( 0 ) );
    API::setColor( c.X(), c.Y(), 'g' );
}

void Mouse::RemoveGoal() {
    const Coordinate& c( _map->CurrentPosition().XY() );
    for( auto goal = _goals.begin(); goal != _goals.end(); ++goal ) {
        if( *goal == c ) {
            _goals.erase( goal );
            break;
        }
    }
}

void Mouse::ClearGoals() {
    _goals.clear();
    assert( _goals.size() == 0 );
}

const std::vector< Coordinate >& Mouse::Goals() const {
    return _goals;
}

bool Mouse::AtGoal() const {
    const Coordinate& c( _map->CurrentPosition().XY() );
    return _flood.at( c.Y() ).at( c.X() ) == 0;
}

void Mouse::ResetFlood() {
    for( auto row = _flood.begin(); row != _flood.end(); ++row ) {
        for( auto n = row->begin(); n != row->end(); ++n )
            *n = 255;
    }

    for( auto goal = _goals.begin(); goal != _goals.end(); ++goal ) {
        _flood.at( goal->Y() ).at( goal->X() ) = 0;
        API::setText( goal->X(), goal->Y(), std::to_string( 0 ) );
    }
}

void Mouse::CalcFlood() {
    std::queue< Coordinate > q;
    std::vector< Coordinate > visited;

    for( auto goal = _goals.begin(); goal != _goals.end(); ++goal )
        q.push( *goal );

    while( !q.empty() ) {
        const Coordinate& c( q.front() );
        const Node& n( _map->At( c ) );
        const unsigned int d( _flood.at( c.Y() ).at( c.X() ) + 1 );

        if( !n.North() ) {
            const Coordinate north( c.X(), c.Y() + 1 );
            assert( north.X() < 16 && north.Y() < 16 );

            if( ( std::find( visited.begin(), visited.end(), north ) == visited.end() ) && _flood.at( north.Y() ).at( north.X() ) > d ) {
                _flood.at( north.Y() ).at( north.X() ) = d;
                API::setText( north.X(), north.Y(), std::to_string( d ) );

                q.push( north );
            }
        }

        if( !n.East() ) {
            const Coordinate east( c.X() + 1, c.Y() );
            assert( east.X() < 16 && east.Y() < 16 );

            if( ( std::find( visited.begin(), visited.end(), east ) == visited.end() ) && _flood.at( east.Y() ).at( east.X() ) > d ) {
                _flood.at( east.Y() ).at( east.X() ) = d;
                API::setText( east.X(), east.Y(), std::to_string( d ) );

                q.push( east );
            }
        }

        if( !n.South() ) {
            const Coordinate south( c.X(), c.Y() - 1 );
            assert( south.X() < 16 && south.Y() < 16 );

            if( ( std::find( visited.begin(), visited.end(), south ) == visited.end() ) && _flood.at( south.Y() ).at( south.X() ) > d ) {
                _flood.at( south.Y() ).at( south.X() ) = d;
                API::setText( south.X(), south.Y(), std::to_string( d ) );

                q.push( south );
            }
        }

        if( !n.West() ) {
            const Coordinate west( c.X() - 1, c.Y() );
            assert( west.X() < 16 && west.Y() < 16 );

            if( ( std::find( visited.begin(), visited.end(), west ) == visited.end() ) && _flood.at( west.Y() ).at( west.X() ) > d ) {
                _flood.at( west.Y() ).at( west.X() ) = d;
                API::setText( west.X(), west.Y(), std::to_string( d ) );

                q.push( west );
            }
        }

        visited.push_back( c );
        q.pop();
    }
}

void Mouse::BestStep() {
    const Node& n( _map->CurrentPosition() );
    const Coordinate& c( n.XY() );
    const unsigned int north = n.North() ? 255 : _flood.at( c.Y() + 1 ).at( c.X() );
    const unsigned int east = n.East() ? 255 : _flood.at( c.Y() ).at( c.X() + 1 );
    const unsigned int south = n.South() ? 255 : _flood.at( c.Y() - 1 ).at( c.X() );
    const unsigned int west = n.West() ? 255 : _flood.at( c.Y() ).at( c.X() - 1 );

    std::set< unsigned int > s;
    s.insert( north );
    s.insert( east );
    s.insert( south );
    s.insert( west );

    // TODO: Optimize to minimize turning
    switch( _heading ) {
        case DIR_NORTH:
            if( north <= *s.begin() )
                MoveDirection( DIR_NORTH );
            else if( west <= *s.begin() )
                MoveDirection( DIR_WEST );
            else if( east <= *s.begin() )
                MoveDirection( DIR_EAST );
            else if( south <= *s.begin() )
                MoveDirection( DIR_SOUTH );

            break;
        case DIR_EAST:
            if( east <= *s.begin() )
                MoveDirection( DIR_EAST );
            else if( north <= *s.begin() )
                MoveDirection( DIR_NORTH );
            else if( south <= *s.begin() )
                MoveDirection( DIR_SOUTH );
            else if( west <= *s.begin() )
                MoveDirection( DIR_WEST );

            break;
        case DIR_SOUTH:
            if( south <= *s.begin() )
                MoveDirection( DIR_SOUTH );
            else if( east <= *s.begin() )
                MoveDirection( DIR_EAST );
            else if( west <= *s.begin() )
                MoveDirection( DIR_WEST );
            else if( north <= *s.begin() )
                MoveDirection( DIR_NORTH );

            break;
        case DIR_WEST:
            if( west <= *s.begin() )
                MoveDirection( DIR_WEST );
            else if( south <= *s.begin() )
                MoveDirection( DIR_SOUTH );
            else if( north <= *s.begin() )
                MoveDirection( DIR_NORTH );
            else if( east <= *s.begin() )
                MoveDirection( DIR_EAST );

            break;
        default:
            assert( false );
            break;

    }
}

void Mouse::TurnLeft() {
    std::clog << "Mouse::TurnLeft" << std::endl;
    _heading = Left();
    API::turnLeft();
    _moves.push( MOVE_TURN_LEFT );
}

void Mouse::TurnRight() {
    std::clog << "Mouse::TurnRight" << std::endl;
    _heading = Right();
    API::turnRight();
    _moves.push( MOVE_TURN_RIGHT );
}

void Mouse::MoveForward() {
    std::clog << "Mouse::MoveForward" << std::endl;
    _map->Move( _heading );
    API::moveForward();
    _moves.push( MOVE_FORWARD );

    const Coordinate& c( _map->CurrentPosition().XY() );
    API::setColor( c.X(), c.Y(), 'c' );
}

void Mouse::MoveDirection( DirectionEnum d ) {
    if( _heading == d )
        MoveForward();
    else if( Left() == d ) {
        TurnLeft();
        MoveForward();
    }
    else if( Right() == d ) {
        TurnRight();
        MoveForward();
    }
    else {
        TurnLeft();
        TurnLeft();
        MoveForward();
    }
}

void Mouse::CheckWalls() {
    if( API::wallLeft() )
        AddWallLeft();

    if( API::wallRight() )
        AddWallRight();

    if( API::wallFront() )
        AddWallFront();
}

void Mouse::AddWallLeft() {
    const DirectionEnum left = Left();
    _map->SetWall( left );
}

void Mouse::AddWallRight() {
    const DirectionEnum right = Right();
    _map->SetWall( right );
}

void Mouse::AddWallFront() {
    const DirectionEnum front = Front();
    _map->SetWall( front );
}

Map* Mouse::GetMap() const {
    return _map;
}

DirectionEnum Mouse::Left() const {
    switch( _heading ) {
        case DIR_NORTH: return DIR_WEST;
        case DIR_EAST: return DIR_NORTH;
        case DIR_SOUTH: return DIR_EAST;
        case DIR_WEST: return DIR_SOUTH;
        default:
            assert( false );
             return DIR_NORTH;
    }
}

DirectionEnum Mouse::Right() const {
    switch( _heading ) {
        case DIR_NORTH: return DIR_EAST;
        case DIR_EAST: return DIR_SOUTH;
        case DIR_SOUTH: return DIR_WEST;
        case DIR_WEST: return DIR_NORTH;
        default:
            assert( false );
            return DIR_NORTH;
    }
}

DirectionEnum Mouse::Front() const {
    return _heading;
}

DirectionEnum Mouse::Back() const {
    switch( _heading ) {
        case DIR_NORTH: return DIR_SOUTH;
        case DIR_EAST: return DIR_WEST;
        case DIR_SOUTH: return DIR_NORTH;
        case DIR_WEST: return DIR_EAST;
        default:
            assert( false );
            return DIR_NORTH;
    }
}
