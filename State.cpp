#include "State.h"
#include "API.h"
#include "Map.h"
#include "Mouse.h"

void IState::Enter() {
}

void IState::Exit() {
}

void IState::PreUpdate( Mouse& context ) {
}

void IState::PostUpdate( Mouse& context ) {
}

Unknown::Unknown() {
}

Unknown::~Unknown() {
}

IState& Unknown::Instance() {
    static Unknown instance;
    return instance;
}

StateEnum Unknown::State() const {
    return STATE_UNKNOWN;
}

void Unknown::Update( Mouse& context ) {
}

Idle::Idle() {
}

Idle::~Idle() {
}

IState& Idle::Instance() {
    static Idle instance;
    return instance;
}

StateEnum Idle::State() const {
    return STATE_IDLE;
}

void Idle::Update( Mouse& context ) {
    context.ChangeState( Exploring::Instance() );
}

Exploring::Exploring() {
}

Exploring::~Exploring() {
}

IState& Exploring::Instance() {
    static Exploring instance;
    return instance;
}

StateEnum Exploring::State() const {
    return STATE_EXPLORING;
}

void Exploring::Enter() {
    std::queue< MoveEnum > empty;
    std::swap( _queue, empty );
}

void Exploring::Update( Mouse& context ) {
    if( !API::wallLeft() )
        context.AddLeft();

    if( !API::wallRight() )
        context.AddRight();

    if( !API::wallFront() )
        context.AddFront();

    if( _queue.empty() ) {
        std::queue< const Node* > nodes;
        nodes.push( &context.GetMap()->CurrentPosition() );

        while( !nodes.empty() ) {
            const Node* current( nodes.front() );
            const Node* left( context.LeftNode( current ) );
            const Node* right( context.RightNode( current ) );
            const Node* front( context.FrontNode( current ) );
            const Node* back( context.BackNode( current ) );

            if( left != nullptr ) {
                if( left->Visited() )
                    nodes.push( left );

                _queue.push( MOVE_TURN_LEFT );
                _queue.push( MOVE_FORWARD );
            }
            else if( front != nullptr ) {
                if( front->Visited() )
                    nodes.push( front );

                _queue.push( MOVE_FORWARD );
            }
            else if( right != nullptr ) {
                if( right->Visited() )
                    nodes.push( right );

                _queue.push( MOVE_TURN_RIGHT );
                _queue.push( MOVE_FORWARD );
            }
            else if( back != nullptr ) {
                if( back->Visited() )
                    nodes.push( back );

                _queue.push( MOVE_TURN_LEFT );
                _queue.push( MOVE_TURN_LEFT );
                _queue.push( MOVE_FORWARD );
            }
            
            nodes.pop();
        }

    }
    else {
        switch( _queue.front() ) {
            case MOVE_FORWARD:
                context.MoveForward();
                break;
            case MOVE_TURN_LEFT:
                context.TurnLeft();
                break;
            case MOVE_TURN_RIGHT:
                context.TurnRight();
                break;
        }
        _queue.pop();
    }
}
