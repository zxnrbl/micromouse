#include <array>
#include "Direction.h"

#pragma once

class Coordinate {
    public:
        Coordinate();
        Coordinate( unsigned int x, unsigned int y );
        ~Coordinate();

        void X( unsigned int x );
        void Y( unsigned int y );

        unsigned int X() const;
        unsigned int Y() const;

    private:
        unsigned int _x;
        unsigned int _y;
};

bool operator==( const Coordinate& lhs, const Coordinate& rhs );
bool operator<( const Coordinate& lhs, const Coordinate& rhs );

class Node {
    public:
        Node();
        ~Node();

        void XY( unsigned int x, unsigned int y );
        const Coordinate& XY() const;

        void SetWall( DirectionEnum d );
        bool North() const;
        bool East() const;
        bool South() const;
        bool West() const;

        void Visit( const bool v );
        bool Visited() const;

    private:
        Coordinate _coordinate;

        bool _north;
        bool _east;
        bool _south;
        bool _west;

        bool _visited;
};

class Map {
    public:
        typedef std::array< Node, 16 > Row;

        Map();
        ~Map();

        void Initialize();
        Node& At( Coordinate pos );
        Node& CurrentPosition();
        void Move( DirectionEnum d );

        void SetWall( DirectionEnum d );

    private:
        std::array< Row, 16 > _nodes;
        Node* _pos;
};
