#pragma once
#include "Character.hpp"
#include "MarioState.hpp"
#include "UI.hpp"
#include "Structs.hpp"

class Mario : public Character
{
private:
    // Mario-specific friend classes
    friend class MarioState;
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

    MarioForm form = SMALL;

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
    Mario();

    void setFrame(marioStateType type, int start, int end)
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
    MarioForm getForm() const { return form; }
    void changeForm(MarioForm _form);

    // enemy tác động
    void hitByEnemy();
    void jumpFromEnemy();
    // Override collision to handle enemies

    void update() override;
    void display() override;
    void updateCollision(GameObject *other, int type) override;
    void createBody(b2World *world) override;
};