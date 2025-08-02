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

    MarioForm form = SMALL;
    void changeForm(MarioForm form);
    float throwPerSecond = 0.5f;
    float sinceLastThrow = 0;

public:
    Mario();
    void setFrame(marioStateType type, int start, int end)
    {
        this->sprite.StartEndFrames[(int)type] = {start, end};
    }

    // Override collision to handle enemies
    void throwFireBall();
    void update() override;
    void updateCollision(GameObject *other, int type) override;
};