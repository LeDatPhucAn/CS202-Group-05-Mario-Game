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
    bool isInvincible = false;
    float invincibleTime = 1.5f;
    float beenInvincibleFor = 0.0f;
    float invincibleDrawRate = 0.15f;
    float invincibleDrawTimer = 0.0f;

    float throwPerSecond = 2.0f;
    float sinceLastThrow = 0.0f;

public:
    Mario();
    void setFrame(marioStateType type, int start, int end)
    {
        this->sprite.StartEndFrames[(int)type] = {start, end};
    }
    void throwFireBall();

    void reset();
    // getter
    MarioForm getForm() const { return form; }
    void changeForm(MarioForm _form);

    // Invincible
    bool getInvincibility() const { return isInvincible; }
    void turnInvincible() { isInvincible = true; }

    // enemy tác động
    void hitByEnemy();
    void jumpFromEnemy();
    // Override collision to handle enemies

    void update() override;
    void display() override;
    void updateCollision(GameObject *other, int type) override;
    void createBody(b2World *world) override;
};