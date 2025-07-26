#pragma once
#include "State.hpp"
#include "Projectile.hpp"

class ProjectileState : public State
{
protected:
    Projectile *projectile;

public:
    ProjectileState(stateType Type, Projectile *_projectile, int _delay)
        : State(Type, _projectile, _delay), projectile(_projectile) {}
};