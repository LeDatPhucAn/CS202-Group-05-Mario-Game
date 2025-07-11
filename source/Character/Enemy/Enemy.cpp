#include "Enemy.hpp"
#include "EnemyState.hpp"
#include "MarioState.hpp"
// --- Enemy Methods ---

Enemy::Enemy() : Character() {}

Enemy::Enemy(const Sprite& _sprite, const Movement& _movement, State* _initialState, Vector2 _pos)
    : Character(_sprite, _movement, _initialState, _pos) {}

Enemy::Enemy(const Enemy& other) : Character(other) {
    beCleared = other.beCleared;
}

void Enemy::updateCollision(GameObject* other, int type) {
    Character* character = dynamic_cast<Character*>(other);
    // Make sure the other object is not another enemy
    if (character && !dynamic_cast<Enemy*>(other)) {
        if (dynamic_cast<DeadState*>(character->currentState)  || dynamic_cast<EnemyDeadState*>(this->currentState)) {
            return;
        }
        // Check if the character on top 
        if (character->movement.velocity.y > 0 && CheckCollisionRecs(character->getFeet(), this->getHead())) {
            // Change to dead state
            this->changeState(new EnemyDeadState(this));
            // Give the character a bounce
            character->movement.velocity.y = -200.f; 
        }
        // character colliding from the side -> dead
        else {
            character->changeState(new DeadState(character));
        }
    }

    Koopa* koopa = dynamic_cast<Koopa*>(other);
    if (koopa) {
        // Check if the Koopa that hit us is in its sliding (Run) state.
        if (dynamic_cast<EnemyRunState*>(koopa->currentState)) {
            this->changeState(new EnemyDeadState(this));
            return;
        }
    }
}

void Goomba::updateCollision(GameObject* other, int type) {
    Enemy::updateCollision(other, type);
}

void Koopa::updateCollision(GameObject* other, int type) {
    
    Character* character = dynamic_cast<Character*>(other);    
    if (!character || dynamic_cast<DeadState*>(character->currentState) || dynamic_cast<EnemyDeadState*>(this->currentState)) {
        return; // Do nothing if it's not the player or if the player is already dead
    }

    bool isStomp = character->movement.velocity.y > 0 && CheckCollisionRecs(character->getFeet(), this->getHead());

    // --- Logic for when Koopa is WALKING ---
    if (dynamic_cast<EnemyWalkState*>(this->currentState)) {
        if (isStomp) {
            // turn koopa into a shell
            this->changeState(new EnemyIdleState(this));
            character->movement.velocity.y = -200.f; 
        } else {
            character->changeState(new DeadState(character));
        }
        return;
    }

    // --- Logic for when Koopa is an IDLE SHELL ---
    if (dynamic_cast<EnemyIdleState*>(this->currentState)) {
        if (isStomp) {
            this->changeState(new EnemyRunState(this));
            this->direction = character->direction;
        } else {
            this->changeState(new EnemyRunState(this));
            // Set shell direction
            if (character->pos.x < this->pos.x) {
                this->direction = RIGHT;
            } else {
                this->direction = LEFT;
            }
        }
        return;
    }

    // --- Logic for when Koopa is a SLIDING SHELL ---
    if (dynamic_cast<EnemyRunState*>(this->currentState)) {
        if (isStomp) {
            this->changeState(new EnemyIdleState(this));
            character->movement.velocity.y = -200.f;
        } else {
            character->changeState(new DeadState(character));
        }
        return;
    }
}

// --- Builder Methods ---

Enemy::Builder& Enemy::Builder::setFrames(stateType type, int start, int end) {
    sprite.StartEndFrames[type] = { start, end };
    return *this;
}

Enemy::Builder& Enemy::Builder::setJsonAndTexture(const std::string& name) {
    sprite.texture = UI::textureMap[name];
    sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap[name]);
    return *this;
}

Enemy::Builder& Enemy::Builder::setSpeed(int speed) {
    movement.speed = speed;
    return *this;
}

Enemy::Builder& Enemy::Builder::setPos(Vector2 _pos) {
    pos = _pos;
    return *this;
}

Enemy::Builder& Enemy::Builder::setVelocity(Vector2 velocity) {
    movement.velocity = velocity;
    return *this;
}

Enemy::Builder& Enemy::Builder::setAcceleration(Vector2 acceleration) {
    movement.acceleration = acceleration;
    return *this;
}

Enemy::Builder& Enemy::Builder::setState(State* initialState) {
    state = initialState;
    return *this;
}

Enemy Enemy::Builder::build() {
    return Enemy(sprite, movement, state, pos);
}