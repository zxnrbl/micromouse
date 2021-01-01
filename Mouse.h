#include "Direction.h"

#pragma once

struct Node;
class Map;
class IState;

class Mouse {
	public:
		Mouse();
		~Mouse();

        void Initialize();
        void Update();

        void ChangeState( IState& state );

        void TurnLeft();
        void TurnRight();
        void MoveForward();

        Map* GetMap() const;

        DirectionEnum Left() const;
        DirectionEnum Right() const;
        DirectionEnum Front() const;
        DirectionEnum Back() const;

        Node* LeftNode( const Node* n ) const;
        Node* RightNode( const Node* n ) const;
        Node* FrontNode( const Node* n ) const;
        Node* BackNode( const Node* n ) const;

        void AddLeft();
        void AddRight();
        void AddFront();

        bool Visited( DirectionEnum d ) const;

    private:
        Node* Neighbor( const Node* n, DirectionEnum d ) const;

	private:
        DirectionEnum _heading;
        Map* _map;
        IState* _state;
};
