#pragma once
#include "State.hpp"
#include "Projectile.hpp"

class ProjectileState : public State
{
protected:
    Projectile *projectile;

public:
    ProjectileState(int Type, Projectile *_projectile, int _delay);
};

class ProjectileMoveState : public ProjectileState
{
public:
    ProjectileMoveState(Projectile *_projectile, int _delay = 5);
    void handleInput() override;
};

class ProjectileStopState : public ProjectileState
{
public:
    ProjectileStopState(Projectile *_projectile, int _delay = 5);
    void handleInput() override;
};