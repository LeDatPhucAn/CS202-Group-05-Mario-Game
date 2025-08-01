#include "ProjectileState.hpp"
#include "Game.hpp"
ProjectileState::ProjectileState(int Type, Projectile *_projectile, int _delay)
    : State(Type, _projectile, _delay), projectile(_projectile) {}

// Move State
ProjectileMoveState::ProjectileMoveState(Projectile *_projectile, int _delay)
    : ProjectileState((int)projectileStateType::MOVE, _projectile, _delay)
{
}
void ProjectileMoveState::handleInput()
{
    b2Vec2 vel = projectile->getBody()->GetLinearVelocity();
    vel.x = projectile->getDirection() * 8;
    projectile->getBody()->SetLinearVelocity({vel.x, vel.y});
}

// Stop State
ProjectileStopState::ProjectileStopState(Projectile *_projectile, int _delay)
    : ProjectileState((int)projectileStateType::STOP, _projectile, _delay)
{
}

void ProjectileStopState::handleInput()
{
    projectile->getBody()->SetLinearVelocity({0, 0});
    if (frameIndex == numFrames - 1 && !removed)
    {
        removed = true;
        // Game::removeGameObject(projectile);
        projectile->needDeletion = true;
    }
}
