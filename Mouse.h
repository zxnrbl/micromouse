#include <array>
#include <stack>
#include <vector>

#include "Direction.h"
#include "Move.h"

#pragma once

class Coordinate;
class Node;
class Map;
class IState;

class Mouse {
	public:
		Mouse();
		~Mouse();

        void Initialize();
        void Update();

        void ChangeState( IState& state );

        void AddGoal( Coordinate c );
        void RemoveGoal();
        void ClearGoals();
        const std::vector< Coordinate >& Goals() const;

        bool AtGoal() const;
        void ResetFlood();
        void CalcFlood();

        void BestStep();
        void TurnLeft();
        void TurnRight();
        void MoveForward();
        void MoveDirection( DirectionEnum d );

        void CheckWalls();
        void AddWallLeft();
        void AddWallRight();
        void AddWallFront();

        Map* GetMap() const;

        DirectionEnum Left() const;
        DirectionEnum Right() const;
        DirectionEnum Front() const;
        DirectionEnum Back() const;

	private:
        typedef std::array< unsigned int, 16 > Row;

    private:
        DirectionEnum _heading;
        Map* _map;
        std::vector< Coordinate > _goals;
        std::array< Row, 16 > _flood;
        IState* _state;
        std::stack< MoveEnum > _moves;
};
