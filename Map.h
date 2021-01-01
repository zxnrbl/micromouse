#include <array>
#include "Direction.h"

#pragma once

class Node {
    public:
        Node();
        ~Node();

        void Attach( const DirectionEnum d, Node* n );

        Node* North() const;
        Node* East() const;
        Node* South() const;
        Node* West() const;

        void Visit( const bool v );
        bool Visited() const;

    private:
        Node* _north;
        Node* _east;
        Node* _south;
        Node* _west;

        bool _visited;
};

class Map {
    public:
        Map();
        ~Map();

        void Initialize();
        Node& CurrentPosition();
        Node& CreateNode();
        void Move( DirectionEnum d );
        bool Visited() const;

    private:
        std::array< Node, 16 * 16 > _nodes;
        Node* _pos;
        unsigned int _count;
};
