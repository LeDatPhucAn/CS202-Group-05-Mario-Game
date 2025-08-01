#include "IBlockBehavior.hpp"
#include "raylib.h"
#include "Block.hpp"
#include "Particle.hpp"
#include "Game.hpp"
#include "Mario.hpp"
#include "GameObject.hpp"

void IBlockBehavior::makeBlockBounce(float dt)
{
    // only bounce when the block is jumping
    if (!block->isJumping)
        return;

    if (timeSinceHit == 0.0f)
    {
        block->body->SetType(b2_kinematicBody);
        block->body->SetFixedRotation(true);
        block->body->SetLinearVelocity(b2Vec2(0.0f, -bounceVel)); // Upward velocity
    }

    timeSinceHit += dt;
    if (timeSinceHit > 0.15f && block->body->GetLinearVelocity().y < 0.0f)
    {
        block->body->SetLinearVelocity(b2Vec2(0.0f, bounceVel)); // Go back down
    }
    if (timeSinceHit > 0.3f)
    {
        block->isJumping = false;
        block->body->SetType(b2_staticBody);
        block->body->SetLinearVelocity(b2Vec2_zero);        // Reset velocity
        block->body->SetTransform(prePos.toMeters(), 0.0f); // Reset position
        timeSinceHit = 0.0f;
    }
    Vec2Adapter adapter(block->body->GetPosition());
    block->setPositionAdapter(adapter);
}
// type của Mario đối với Block, Type = TOP là MArio nhảy lên đụng block
void QuestionBehavior::reactToCollision(GameObject *p, int type)
{
    Mario *mario = dynamic_cast<Mario *>(p);
    if (!mario)
        return;

    if (type == BOTTOM)
    {
        cout << "Question Block hit!" << endl;
        block->isJumping = true;
        prePos = block->getPositionAdapter();
    }
}

void QuestionBehavior::updateFrame(float dt)
{
    makeBlockBounce(dt);
}

void BrickBehavior::reactToCollision(GameObject *p, int type)
{
    // chỉ phản ứng khi va chạm với Mario từ dưới lên
    Mario *mario = dynamic_cast<Mario *>(p);
    if (!mario)
        return;
    // FEET = đụng từ dưới
    if (type == BOTTOM)
    {
        block->needDeletion = true;
        Particle::spawnParticles(*block, Game::particles);
    }
}

void BrickBehavior::updateFrame(float dt)
{
}

void BrickBehavior::onDraw(float dt)
{
}

void GroundBehavior::reactToCollision(GameObject *p, int type)
{
}
