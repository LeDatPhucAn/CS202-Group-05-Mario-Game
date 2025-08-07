#include "ProjectileState.hpp"
#include "Game.hpp"
ProjectileState::ProjectileState(int Type, Projectile *_projectile, int _delay)
    : State(Type, _projectile, _delay)
{
}

// Move State
ProjectileMoveState::ProjectileMoveState(Projectile *_projectile, int _delay)
    : ProjectileState((int)projectileStateType::MOVE, _projectile, _delay)
{
}
void ProjectileMoveState::handleInput()
{
    b2Vec2 vel = gameObject->getBody()->GetLinearVelocity();
    vel.x = gameObject->getDirection() * 8;
    gameObject->getBody()->SetLinearVelocity({vel.x, vel.y});
}

// Stop State
ProjectileStopState::ProjectileStopState(Projectile *_projectile, int _delay)
    : ProjectileState((int)projectileStateType::STOP, _projectile, _delay)
{
}

void ProjectileStopState::handleInput()
{
    if (!removed)
    {
        gameObject->getBody()->SetLinearVelocity({0, 0});

        if (frameIndex == numFrames - 1)
        {
            removed = true;
            gameObject->needDeletion = true;
        }
    }
}
