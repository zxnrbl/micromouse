#include <queue>
#include "Move.h"

#pragma once

class Mouse;

enum StateEnum {
    STATE_UNKNOWN,
    STATE_IDLE,
    STATE_INITIALIZING,
    STATE_EXPLORING,
    STATE_RETURNING,
    STATE_MAPPING,
    STATE_SPEEDING,
    STATE_COMPLETE
};

class IState {
    public:
        virtual StateEnum State() const = 0;

        virtual void Enter( Mouse& context );
        virtual void Exit( Mouse& context );

        virtual void PreUpdate( Mouse& context );
        virtual void Update( Mouse& context ) = 0;
        virtual void PostUpdate( Mouse& context );
};

class Unknown : public IState {
    public:
        static IState& Instance();
        virtual StateEnum State() const;

        virtual void Update( Mouse& context );

    private:
        Unknown();
        ~Unknown();
};

class Idle : public IState {
    public:
        static IState& Instance();
        virtual StateEnum State() const;

        virtual void Update( Mouse& context );

    private:
        Idle();
        ~Idle();
};

class Exploring : public IState {
    public:
        static IState& Instance();
        virtual StateEnum State() const;

        virtual void Enter( Mouse& context );
        virtual void Update( Mouse& context );
        virtual void Exit( Mouse& context );

    private:
        Exploring();
        ~Exploring();

    private:
        std::queue< MoveEnum > _queue;
};

class Returning : public IState {
    public:
        static IState& Instance();
        virtual StateEnum State() const;

        virtual void Enter( Mouse& context );
        virtual void Update( Mouse& context );
        virtual void Exit( Mouse& context );

    private:
        Returning();
        ~Returning();
};

class Mapping : public IState {
    public:
        static IState& Instance();
        virtual StateEnum State() const;

        virtual void Update( Mouse& context );
        virtual void Exit( Mouse& context );

    private:
        Mapping();
        ~Mapping();
};

class Speeding : public IState {
    public:
        static IState& Instance();
        virtual StateEnum State() const;

        virtual void Enter( Mouse& context );
        virtual void Update( Mouse& context );
        virtual void Exit( Mouse& context );

    private:
        Speeding();
        ~Speeding();
};

class Complete : public IState {
    public:
        static IState& Instance();
        virtual StateEnum State() const;

        virtual void Update( Mouse& context );

    private:
        Complete();
        ~Complete();
};
