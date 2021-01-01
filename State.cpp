#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

#include "State.h"
#include "API.h"
#include "Map.h"
#include "Mouse.h"

void IState::Enter( Mouse& context ) {
}

void IState::Exit( Mouse& context ) {
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

void Exploring::Enter( Mouse& context ) {
    std::clog << "Exploring Enter" << std::endl;
    std::queue< MoveEnum > empty;
    std::swap( _queue, empty );

    context.GetMap()->CurrentPosition().Visit( true );

    context.AddGoal( Coordinate( 7, 7 ) );
    context.AddGoal( Coordinate( 7, 8 ) );
    context.AddGoal( Coordinate( 8, 7 ) );
    context.AddGoal( Coordinate( 8, 8 ) );

    API::setColor( context.GetMap()->CurrentPosition().XY().X(), context.GetMap()->CurrentPosition().XY().Y(), 'c' );
}

void Exploring::Update( Mouse& context ) {
    context.CheckWalls();
    context.ResetFlood();
    context.CalcFlood();
    context.BestStep();

    if( context.AtGoal() )
        context.ChangeState( Mapping::Instance() );
}

void Exploring::Exit( Mouse& context ) {
    context.ClearGoals();
}

Returning::Returning() {
}

Returning::~Returning() {
}

IState& Returning::Instance() {
    static Returning instance;
    return instance;
}

StateEnum Returning::State() const {
    return STATE_RETURNING;
}

void Returning::Enter( Mouse& context ) {
    std::clog << "Returning Enter" << std::endl;
    context.AddGoal( Coordinate( 0, 0 ) );
}

void Returning::Update( Mouse& context ) {
    context.CheckWalls();
    context.ResetFlood();
    context.CalcFlood();
    context.BestStep();

    if( context.AtGoal() )
        context.ChangeState( Speeding::Instance() );
}

void Returning::Exit( Mouse& context ) {
    context.ClearGoals();
}

Mapping::Mapping() {
}

Mapping::~Mapping() {
}

IState& Mapping::Instance() {
    static Mapping instance;
    return instance;
}

StateEnum Mapping::State() const {
    return STATE_MAPPING;
}

void Mapping::Update( Mouse& context ) {
    bool complete = context.Goals().empty();

    if( context.Goals().empty() ) {
        for( auto row = 0; row < 16; ++row ) {
            for( auto col = 0; col < 16; ++col ) {
                if( !context.GetMap()->At( Coordinate( col, row ) ).Visited() ) {
                    complete = false;
                    context.AddGoal( Coordinate( col, row ) );
                    break;
                }
            }
            if( !complete ) break;
        }
    }
    else {
        context.CheckWalls();
        context.ResetFlood();
        context.CalcFlood();
        context.BestStep();
    }

    if( context.AtGoal() )
        context.RemoveGoal();

    if( complete && context.Goals().empty() )
        context.ChangeState( Returning::Instance() );
}

void Mapping::Exit( Mouse& context ) {
    context.ClearGoals();
}

Speeding::Speeding() {
}

Speeding::~Speeding() {
}

IState& Speeding::Instance() {
    static Speeding instance;
    return instance;
}

StateEnum Speeding::State() const {
    return STATE_SPEEDING;
}

void Speeding::Enter( Mouse& context ) {
    context.AddGoal( Coordinate( 7, 7 ) );
    context.AddGoal( Coordinate( 7, 8 ) );
    context.AddGoal( Coordinate( 8, 7 ) );
    context.AddGoal( Coordinate( 8, 8 ) );
    context.ResetFlood();
    context.CalcFlood();
}

void Speeding::Update( Mouse& context ) {
    context.BestStep();

    if( context.AtGoal() )
        context.ChangeState( Complete::Instance() );
}

void Speeding::Exit( Mouse& context ) {
    context.ClearGoals();
}

Complete::Complete() {
}

Complete::~Complete() {
}

IState& Complete::Instance() {
    static Complete instance;
    return instance;
}

StateEnum Complete::State() const {
    return STATE_COMPLETE;
}

void Complete::Update( Mouse& context ){
}
