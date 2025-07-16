#include "Enemy.hpp"
#include "EnemyState.hpp"
#include "MarioState.hpp"
#include "Mario.hpp"

// --- Base Enemy Methods ---
Enemy::Enemy() : Character() {}
Enemy::Enemy(const Sprite &_sprite, State *_initialState, Vector2 _pos)
    : Character(_sprite, _initialState, _pos) {}

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
            // mario->movement.velocity.y = -200.f / PPM; // Bounce Mario
            b2Vec2 vel = mario->body->GetLinearVelocity();
            mario->body->SetLinearVelocity({vel.x, -200.f / PPM});
        }
        // If Mario hits from the side
        else if (type == LEFTSIDE || type == RIGHTSIDE)
        {
            mario->changeState(new DeadState(mario));
        }
        return; // Collision with Mario handled
    }
    Koopa *koopa = dynamic_cast<Koopa *>(other);
    if (koopa && dynamic_cast<EnemyRunState *>(koopa->currentState))
    {
        this->changeState(new EnemyDeadState(this));
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
    Mario *mario = dynamic_cast<Mario *>(other);
    if (mario)
    {
        // Access currentState directly
        if (dynamic_cast<DeadState *>(mario->currentState) || dynamic_cast<EnemyDeadState *>(this->currentState))
        {
            return;
        }

        // box2d Velocity
        b2Vec2 vel = mario->body->GetLinearVelocity();

        // --- Logic for when Koopa is WALKING ---
        if (dynamic_cast<EnemyWalkState *>(this->currentState))
        {

            if (type == HEAD)
            {                                                // Mario stomps a walking Koopa
                this->changeState(new EnemyIdleState(this)); // Turn into a shell
                // Access movement directly
                // mario->movement.velocity.y = -200.f / PPM;

                // change velocity
                mario->body->SetLinearVelocity({vel.x, -200.f / PPM});
            }
            else
            { // Mario hits a walking Koopa from the side
                mario->changeState(new DeadState(mario));
            }
            return;
        }

        // --- Logic for when Koopa is an IDLE SHELL ---
        if (dynamic_cast<EnemyIdleState *>(this->currentState))
        {
            this->changeState(new EnemyRunState(this)); // Any touch makes it slide
            // Set shell direction away from Mario, accessing pos directly
            this->direction = (mario->pos.x < this->pos.x) ? RIGHT : LEFT;
            if (type == HEAD)
            { // If stomped, give Mario a bounce

                // mario->movement.velocity.y = -200.f ;

                mario->body->SetLinearVelocity({vel.x, -200.f / PPM});
            }
            return;
        }

        // --- Logic for when Koopa is a SLIDING SHELL ---
        if (dynamic_cast<EnemyRunState *>(this->currentState))
        {
            if (type == HEAD)
            { // Stomping a sliding shell stops it
                this->changeState(new EnemyIdleState(this));
                // mario->movement.velocity.y = -200.f;
                mario->body->SetLinearVelocity({vel.x, -200.f / PPM});
            }
            else
            { // Running into a sliding shell from the side
                mario->changeState(new DeadState(mario));
            }
            return;
        }
    }
    Enemy *otherEnemy = dynamic_cast<Enemy *>(other);
    if (otherEnemy && otherEnemy != this)
    {
        // If this Koopa is a sliding shell and hits another enemy
        if (dynamic_cast<EnemyRunState *>(this->currentState))
        {
            otherEnemy->changeState(new EnemyDeadState(otherEnemy));
        }
    }
}

// --- Builder Methods Implementation ---
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

Enemy::Builder &Enemy::Builder::setPos(Vector2 _pos)
{
    pos = _pos;
    return *this;
}

Enemy::Builder &Enemy::Builder::setState(State *initialState)
{
    state = initialState;
    return *this;
}