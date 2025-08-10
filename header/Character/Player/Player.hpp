#pragma once
#include "Character.hpp"
#include "PlayerState.hpp"
#include "UI.hpp"
#include "Structs.hpp"

class Player : public Character
{
private:
    // Player-specific friend classes
    friend class PlayerState;
    friend class IdleState;
    friend class WalkState;
    friend class RunState;
    friend class JumpState;
    friend class FallState;
    friend class SkidState;
    friend class CrouchState;
    friend class GrowState;
    friend class UnGrowState;
    friend class DeadState;
    friend class Goomba;
    friend class Koopa;
    friend class ChooseLevel;

    PlayerForm form = SMALL;

    // Invincibility
    bool isInvincible = false;
    float invincibleTime = 1.5f;
    float beenInvincibleFor = 0.0f;
    float invincibleDrawRate = 0.15f;
    float invincibleDrawTimer = 0.0f;

    // Fireball related
    bool throwingFireBall = false;
    int throwDelay = 5;
    int currentDelayTime = 0;
    float secondsPerThrow = 1.5f;
    float sinceLastThrow = 0.0f;

    // Star related
    bool starMode = false;
    float starTimer = 0.0f;     // seconds remaining
    float starColorTime = 0.0f; // for color cycling
protected:
    unordered_map<Action, int> keyBindings;
    
    void bindKey(Action action, int key)
    {
        keyBindings[action] = key;
    }

    int getKey(Action action)
    {
        return keyBindings[action];
    }

public:
    Player();

    void setFrame(playerStateType type, int start, int end)
    {
        this->sprite.StartEndFrames[(int)type] = {start, end};
    }
    // fireball
    void throwFireBall();

    // Star
    void DrawLGBT(Texture2D texture, Rectangle srcRect, Vector2 position);
    void EatStar();
    bool getStarMode() const { return starMode; }
    // Invincible
    bool getInvincibility() const { return isInvincible; }
    void turnInvincible() { isInvincible = true; }

    // reset game
    void reset();
    // getter
    PlayerForm getForm() const { return form; }
    void changeForm(PlayerForm _form);

    // enemy tác động
    void hitByEnemy();
    void jumpFromEnemy();
    // Override collision to handle enemies

    void update() override;
    void display() override;
    void updateCollision(GameObject *other, int type) override;
    void createBody(b2World *world) override;
};

class PlayerMario : public Player
{
public:
    PlayerMario() : Player()
    {
        setTexture("Mario2D");
        changeState(new IdleState(this));

        // Press A to move left
        bindKey(Action::BUTTON_LEFT, KEY_A);

        // Press D to move right
        bindKey(Action::BUTTON_RIGHT, KEY_D);

        // Press W to look/jump up (or climb ladders, if implemented)
        bindKey(Action::BUTTON_UP, KEY_W);

        // Press S to crouch or move down
        bindKey(Action::BUTTON_DOWN, KEY_S);

        // Hold Left Shift to run
        bindKey(Action::BUTTON_RUN, KEY_LEFT_SHIFT);

        // Press F to throw a fireball
        bindKey(Action::BUTTON_THROW_FIREBALL, KEY_F);
    }
};

class PlayerLuigi : public Player
{
public:
    PlayerLuigi() : Player()
    {
        setTexture("Luigi2D");
        changeState(new IdleState(this));

        // Press Left Arrow to move lefts
        bindKey(Action::BUTTON_LEFT, KEY_LEFT);

        // Press Right Arrow to move right
        bindKey(Action::BUTTON_RIGHT, KEY_RIGHT);

        // Press Up Arrow to look/jump up (or climb ladders, if implemented)
        bindKey(Action::BUTTON_UP, KEY_UP);

        // Press Down Arrow to crouch or move down
        bindKey(Action::BUTTON_DOWN, KEY_DOWN);

        // Hold Right Shift to run
        bindKey(Action::BUTTON_RUN, KEY_RIGHT_SHIFT);

        // Press Slash (/) to throw a fireball
        bindKey(Action::BUTTON_THROW_FIREBALL, KEY_SLASH);
    }
};
