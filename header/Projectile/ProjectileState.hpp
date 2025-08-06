#pragma once
#include "State.hpp"

class Projectile;
class ProjectileState : public State
{
protected:
    Projectile *projectile = nullptr;

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
private:
    bool removed = false;

public:
    ProjectileStopState(Projectile *_projectile, int _delay = 5);
    void handleInput() override;
};