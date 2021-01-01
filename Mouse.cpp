#include <cassert>

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
    ChangeState( Idle::Instance() );
}

void Mouse::Update() {
    _state->PreUpdate( *this );
    _state->Update( *this );
    _state->PostUpdate( *this );
}

void Mouse::ChangeState( IState& state ) {
    _state->Exit();
    _state = &state;
    _state->Enter();
}

void Mouse::TurnLeft() {
    _heading = Left();
    API::turnLeft();
}

void Mouse::TurnRight() {
    _heading = Right();
    API::turnRight();
}

void Mouse::MoveForward() {
    _map->Move( _heading );
    API::moveForward();
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

Node* Mouse::Neighbor( const Node* n, DirectionEnum d ) const {
    switch( d ) {
        case DIR_NORTH: return n->North();
        case DIR_EAST: return n->East();
        case DIR_SOUTH: return n->South();
        case DIR_WEST: return n->West();
        default:
            assert( false );
            return nullptr;
    }
}

Node* Mouse::LeftNode( const Node* n ) const {
    return Neighbor( n, Left() );
}

Node* Mouse::RightNode( const Node* n ) const {
    return Neighbor( n, Right() );
}

Node* Mouse::FrontNode( const Node* n ) const {
    return Neighbor( n, Front() );
}

Node* Mouse::BackNode( const Node* n ) const {
    return Neighbor( n, Back() );
}

void Mouse::AddLeft() {
    const DirectionEnum dir = Left();
    const DirectionEnum rev = Right();
    Node& pos( _map->CurrentPosition() );

    bool is_known = false;

    switch( dir ) {
        case DIR_NORTH:
            if( pos.North() != nullptr ) is_known = true;
            break;
        case DIR_EAST:
            if( pos.East() != nullptr ) is_known = true;
            break;
        case DIR_SOUTH:
            if( pos.South() != nullptr ) is_known = true;
            break;
        case DIR_WEST:
            if( pos.West() != nullptr ) is_known = true;
            break;
        default:
            assert( false );
            break;
    }

    if( !is_known ) {
        Node& left( _map->CreateNode() );

        left.Attach( rev, &pos );
        pos.Attach( dir, &left );
    }
}

void Mouse::AddRight() {
    const DirectionEnum dir = Right();
    const DirectionEnum rev = Left();
    Node& pos( _map->CurrentPosition() );

    bool is_known = false;

    switch( dir ) {
        case DIR_NORTH:
            if( pos.North() != nullptr ) is_known = true;
            break;
        case DIR_EAST:
            if( pos.East() != nullptr ) is_known = true;
            break;
        case DIR_SOUTH:
            if( pos.South() != nullptr ) is_known = true;
            break;
        case DIR_WEST:
            if( pos.West() != nullptr ) is_known = true;
            break;
        default:
            assert( false );
            break;
    }

    if( !is_known ) {
        Node& right( _map->CreateNode() );

        right.Attach( rev, &pos );
        pos.Attach( dir, &right );
    }
}

void Mouse::AddFront() {
    const DirectionEnum dir = Front();
    const DirectionEnum rev = Back();
    Node& pos( _map->CurrentPosition() );

    bool is_known = false;

    switch( dir ) {
        case DIR_NORTH:
            if( pos.North() != nullptr ) is_known = true;
            break;
        case DIR_EAST:
            if( pos.East() != nullptr ) is_known = true;
            break;
        case DIR_SOUTH:
            if( pos.South() != nullptr ) is_known = true;
            break;
        case DIR_WEST:
            if( pos.West() != nullptr ) is_known = true;
            break;
        default:
            assert( false );
            break;
    }

    if( !is_known ) {
        Node& front( _map->CreateNode() );
        front.Attach( rev, &pos );
        pos.Attach( dir, &front );
    }
}

bool Mouse::Visited( DirectionEnum dir ) const {
    Node& pos( _map->CurrentPosition() );

    switch( dir ) {
        case DIR_NORTH:
            return ( pos.North() == nullptr ) || pos.North()->Visited();
        case DIR_EAST:
            return ( pos.East() == nullptr ) || pos.East()->Visited();
        case DIR_SOUTH:
            return ( pos.South() == nullptr ) || pos.South()->Visited();
        case DIR_WEST:
            return ( pos.West() == nullptr ) || pos.West()->Visited();
        default:
            assert( false );
            return false;
    }
}
