#include "IBlockBehavior.hpp"
#include "raylib.h"
#include "Block.hpp"
#include "Particle.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "GameObject.hpp"
#include "GameInfo.hpp"
#include "Mushroom.hpp"
#include "SoundController.hpp"
#include "BlockState.hpp"
#include "Star.hpp"
class QuestionBehavior;

IBlockBehavior::IBlockBehavior(Block *block) : block(block)
{
    prePos = block->getPositionAdapter();
}
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
        if (block->isQuestion)
        {
            block->isUsed = true;
            this->block->changeState(new BlockUsedState(this->block));
            if (this->block->contain == "Mushroom")
                throwMushroom(this->direction);
            else if (this->block->contain == "Coin")
                GameInfo::getInstance()->addScore(100);
            else if (this->block->contain == "Star")
                throwStar(this->direction);
        }

        block->body->SetType(b2_staticBody);
        block->body->SetLinearVelocity(b2Vec2_zero);        // Reset velocity
        block->body->SetTransform(prePos.toMeters(), 0.0f); // Reset position
        timeSinceHit = 0.0f;
    }
    Vec2Adapter adapter(block->body->GetPosition());
    block->setPositionAdapter(adapter);
}
void IBlockBehavior::setForBounce()
{
    SoundController::getInstance().playBlockStateSFX(blockStateType::BOUNCE);
    block->isJumping = true;
}
void IBlockBehavior::setNoBounce()
{
    block->isJumping = false;
}

void IBlockBehavior::throwStar(int direction)
{
    // SoundController::getInstance().playPlayerStateSFX(playerStateType::THROWFB);
    Star *star = new Star();

    if (direction == LEFT)
    {
        star->setDirection(RIGHT);
    }
    else
        star->setDirection(LEFT);
    // star->setDirection(this->getDirection());
    SoundController::getInstance().playBlockStateSFX(blockStateType::SPAWNITEM);
    Vector2 pos = this->block->getPositionAdapter().toPixels();
    pos.x += -star->getDirection() * block->getSize().x;
    pos.y -= block->getSize().y * 2;
    star->setPosition(pos);
    Game::addGameObject(star);
}
void IBlockBehavior::throwMushroom(int direction)
{
    // SoundController::getInstance().playPlayerStateSFX(playerStateType::THROWFB);
    Mushroom *mushroom = new Mushroom();

    if (direction == LEFT)
    {
        mushroom->setDirection(RIGHT);
    }
    else
        mushroom->setDirection(LEFT);
    // mushroom->setDirection(this->getDirection());
    SoundController::getInstance().playBlockStateSFX(blockStateType::SPAWNITEM);
    Vector2 pos = this->block->getPositionAdapter().toPixels();
    pos.y -= block->getSize().y * 2;
    mushroom->setPosition(pos);
    Game::addGameObject(mushroom);
}
// type của Player đối với Block, Type = Bottom là MArio nhảy lên đụng block
void QuestionBehavior::reactToCollision(GameObject *p, int type)
{
    Player *mario = dynamic_cast<Player *>(p);
    if (!mario)
        return;

    if (type == BOTTOM && !this->block->isUsed)
    {
        cout << "Question Block hit!" << endl;
        setForBounce();
        this->direction = p->getDirection();
    }
}

void QuestionBehavior::updateFrame(float dt)
{
    makeBlockBounce(dt);
}

void BrickBehavior::reactToCollision(GameObject *p, int type)
{
    // chỉ phản ứng khi va chạm với Player từ dưới lên
    Player *mario = dynamic_cast<Player *>(p);
    if (!mario)
        return;
    // FEET = đụng từ dưới
    if (type == BOTTOM)
    {
        // bé thì ko phá dc
        if (mario->getForm() == SMALL)
        {
            setForBounce();
        }
        else
        {
            setNoBounce();
            SoundController::getInstance().playBlockStateSFX(blockStateType::BREAK);
            block->needDeletion = true;
            GameInfo::getInstance()->addScore(50);
            Particle::spawnParticles(*block, Game::particles);
        }
    }
}

void BrickBehavior::updateFrame(float dt)
{
    makeBlockBounce(dt);
}

void BrickBehavior::onDraw(float dt)
{
}

void GroundBehavior::reactToCollision(GameObject *p, int type)
{
}

void CoinBehavior::reactToCollision(GameObject *p, int type)
{
    Player *mario = dynamic_cast<Player *>(p);
    if (!mario)
        return;
    this->block->needDeletion = true;
    GameInfo::getInstance()->addScore(100);
    GameInfo::getInstance()->addCoin();
    
    SoundController::getInstance().playBlockStateSFX(blockStateType::GETCOIN);
}
