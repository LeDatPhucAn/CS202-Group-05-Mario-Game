#include "Enemy.hpp"
#include "EnemyState.hpp"
#include "MarioState.hpp"
#include "Mario.hpp"

// --- Base Enemy Methods ---
Enemy::Enemy() : Character() {}
Enemy::Enemy(const Sprite &_sprite, const Movement &_movement, State *_initialState, Vector2 _pos)
    : Character(_sprite, _movement, _initialState, _pos) {}

Enemy::Enemy(const Enemy &other) : Character(other)
{
    beCleared = other.beCleared;
}

// --- Goomba Methods ---
Goomba::Goomba()
    : Enemy()
{
    // Goomba-specific sprite setup
    this->sprite.StartEndFrames[IDLE] = {0, 0};
    this->sprite.StartEndFrames[WALK] = {0, 1};
    this->sprite.StartEndFrames[DEAD] = {2, 2};
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Enemies2D"]);
    this->sprite.texture = UI::textureMap["Enemies2D"];
}

void Goomba::updateCollision(GameObject *other, int type)
{
    Character::updateCollision(other, type);
    Block *block = dynamic_cast<Block *>(other);
    if (block)
    {
        if (type == LEFTSIDE){
            this->direction = RIGHT;
            this->movement.velocity.x = -20.f;
        }
        else if (type == RIGHTSIDE){
            this->direction = LEFT;
            this->movement.velocity.x = 20.f;
        }
    }

    Mario *mario = dynamic_cast<Mario *>(other);
    if (mario)
    {
        // Access currentState directly
        if (dynamic_cast<DeadState *>(mario->currentState) || dynamic_cast<EnemyDeadState *>(this->currentState))
        {
            return;
        }
        // If Mario is on top (stomp)
        if (type == HEAD)
        {
            this->changeState(new EnemyDeadState(this));
            // Access movement directly
            mario->movement.velocity.y = -200.f; // Bounce Mario
        }
        return; 
    }
}

// --- Koopa Methods ---
Koopa::Koopa()
    : Enemy()
{
    // Koopa-specific sprite setup
    this->sprite.StartEndFrames[IDLE] = {7, 7}; // Shell state
    this->sprite.StartEndFrames[WALK] = {3, 4};
    this->sprite.StartEndFrames[RUN] = {7, 7}; // Sliding shell state
    this->sprite.StartEndFrames[DEAD] = {7, 7};
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Enemies2D"]);
    this->sprite.texture = UI::textureMap["Enemies2D"];
}

void Koopa::updateCollision(GameObject *other, int type)
{
    Character::updateCollision(other, type);
    Block *block = dynamic_cast<Block *>(other);
    if (block)
    {
        if (type == LEFTSIDE){
            this->direction = RIGHT;
            this->movement.velocity.x = -this->movement.velocity.x; 
        }
        else if (type == RIGHTSIDE){
            this->direction = LEFT;
            this->movement.velocity.x = -this->movement.velocity.x;
        }
       
    }
    Mario *mario = dynamic_cast<Mario *>(other);
    if (mario)
    {
        // Access currentState directly
        if (dynamic_cast<DeadState *>(mario->currentState) || dynamic_cast<EnemyDeadState *>(this->currentState))
        {
            return;
        }

        // --- Logic for when Koopa is WALKING ---
        if (dynamic_cast<EnemyWalkState *>(this->currentState))
        {
            if (type == HEAD)
            {                                                
                this->changeState(new EnemyIdleState(this)); // Turn into a shell
                this->pos.y += 12.0f;
            }
            return;
        }

        // --- Logic for when Koopa is an IDLE SHELL ---
        if (dynamic_cast<EnemyIdleState *>(this->currentState))
        {   
            this->direction = (mario->pos.x < this->pos.x) ? LEFT : RIGHT;
            this->changeState(new EnemyRunState(this)); 
            return;
        }

        // --- Logic for when Koopa is a SLIDING SHELL ---
        if (dynamic_cast<EnemyRunState *>(this->currentState))
        {   
            if (type == HEAD)
            { 
                this->changeState(new EnemyIdleState(this));
            }         
            return;
        }
    }
    Enemy *otherEnemy = dynamic_cast<Enemy *>(other);
    if (otherEnemy && otherEnemy != this)
    {
        if (dynamic_cast<DeadState *>(otherEnemy->currentState) || dynamic_cast<EnemyDeadState *>(this->currentState))
        {
            return;
        }
        if (dynamic_cast<EnemyRunState *>(this->currentState))
        {
            otherEnemy->changeState(new EnemyDeadState(otherEnemy));
        }
    }
}

Rectangle Koopa::getBounds() const
{
    // If the Koopa is walking, use the taller bounds.
    if (dynamic_cast<const EnemyWalkState*>(currentState))
    {
        return { pos.x, pos.y, 16.0f, 24.0f }; 
    }
    else
    {
        return { pos.x, pos.y, 16.0f, 12.0f };
    }
}

Rectangle Koopa::getFeet() const
{
    // This now uses the correct bounds (tall or short) from the getBounds() method above
    Rectangle currentBounds = getBounds(); 
    return {
        currentBounds.x + 6, 
        currentBounds.y + currentBounds.height - 2, // The feet are now at the bottom of the correct hitbox
        currentBounds.width - 12,
        6.0f
    };
}


Enemy::Builder &Enemy::Builder::setFrames(stateType type, int start, int end)
{
    sprite.StartEndFrames[type] = {start, end};
    return *this;
}

Enemy::Builder &Enemy::Builder::setJsonAndTexture(const std::string &name)
{
    sprite.texture = UI::textureMap[name];
    sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap[name]);
    return *this;
}

Enemy::Builder &Enemy::Builder::setSpeed(int speed)
{
    movement.speed = speed;
    return *this;
}

Enemy::Builder &Enemy::Builder::setPos(Vector2 _pos)
{
    pos = _pos;
    return *this;
}

Enemy::Builder &Enemy::Builder::setVelocity(Vector2 velocity)
{
    movement.velocity = velocity;
    return *this;
}

Enemy::Builder &Enemy::Builder::setAcceleration(Vector2 acceleration)
{
    movement.acceleration = acceleration;
    return *this;
}

Enemy::Builder &Enemy::Builder::setState(State *initialState)
{
    state = initialState;
    return *this;
}